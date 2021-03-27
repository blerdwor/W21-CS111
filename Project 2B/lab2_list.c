// NAME: Belle Lerdworatawee
// EMAIL: bellelee@g.ucla.edu
// ID: 105375663

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include "SortedList.h"

#define BILLION 1E9

int opt_yield;
long thread_num = 1;
long list_num = 1;
long iter = 1;
long total_nodes;
long* s_locks = 0; /*for spin-lock*/
long* wait_times;
char* yield_ch = NULL;
char sync_ch = 'x';
SortedListElement_t *head, *pool;
pthread_mutex_t* m_locks = NULL; /*for mutex*/

char* gen_rand_key() {
  char* str = malloc(4 * sizeof(char)); // arbitrary key length
  char alpha[] = "abcdefghijklmnopqrstuvwxyz";

  for (int i = 0; i < 3; i++)
    str[i] = alpha[rand() % 25];
  str[3] = '\0';
  
  return str;
}

void* th_worker(void* start) {

  struct timespec mutex_start, mutex_stop;
  int start_index = *((int *) start);
  int end_index = start_index + iter;
  int th_ID = start_index / iter;

  // printf("%ld Passed start index: %d\n", pthread_self(), start_index);

  // insert
  for (int i = start_index; i < end_index; i++) {
    int list_index = *(pool[i].key) % list_num;
    if (sync_ch == 'm') {
      clock_gettime(CLOCK_MONOTONIC, &mutex_start);              
      pthread_mutex_lock(&m_locks[list_index]);
      clock_gettime(CLOCK_MONOTONIC, &mutex_stop);    
    }
    else if (sync_ch == 's')
      while(__sync_lock_test_and_set(&s_locks[list_index], 1));
    // printf("%ld insert list_idx: %d\n", pthread_self(), list_index);
    SortedList_insert(&head[list_index], &pool[i]);

    if (sync_ch == 'm')  {
      pthread_mutex_unlock(&m_locks[list_index]);
      long total_mutex_time = (mutex_stop.tv_sec - mutex_start.tv_sec)*BILLION + \
                         (mutex_stop.tv_nsec - mutex_start.tv_nsec);
      wait_times[th_ID] += total_mutex_time;
    }
    else if (sync_ch == 's')
      __sync_lock_release(&s_locks[list_index]);
  }
  
  // check length
  if (sync_ch == 'm') { 
    clock_gettime(CLOCK_MONOTONIC, &mutex_start);
    for (int i = 0; i < list_num; i++)
      pthread_mutex_lock(&m_locks[i]);
    clock_gettime(CLOCK_MONOTONIC, &mutex_stop);
    long total_mutex_time = (mutex_stop.tv_sec - mutex_start.tv_sec)*BILLION + \
                         (mutex_stop.tv_nsec - mutex_start.tv_nsec);
    wait_times[th_ID] += total_mutex_time;
  }
  else if (sync_ch == 's') {
    for (int i = 0; i < list_num; i++)
      while(__sync_lock_test_and_set(&s_locks[i], 1)); 
  }

  for (int i = 0; i < list_num; i++) {
    if (SortedList_length(&head[i]) < 0) {
      fprintf(stderr, "Error the list is corrupted\n");
      exit(2);
    }
  }
    
  if (sync_ch == 'm')  {
    for (int i = 0; i < list_num; i++)
      pthread_mutex_unlock(&m_locks[i]);
  }
  else if (sync_ch == 's') {
    for (int i = 0; i < list_num; i++)
      __sync_lock_release(&s_locks[i]);
  }

  // lookup and delete
  for (int i = start_index; i < end_index; i++) {
    int list_index = *(pool[i].key) % list_num;
    if (sync_ch == 'm') {
      clock_gettime(CLOCK_MONOTONIC, &mutex_start);
      pthread_mutex_lock(&m_locks[list_index]);
      clock_gettime(CLOCK_MONOTONIC, &mutex_stop);
    }
    else if (sync_ch == 's')
      while(__sync_lock_test_and_set(&s_locks[list_index], 1));

    // printf("%ld delete list_idx: %d\n", pthread_self(), list_index);
    SortedListElement_t* elem = SortedList_lookup(&head[list_index], pool[i].key);
    if (elem == NULL) {
      fprintf(stderr, "Error the key cannot be found\n");
      exit(2);
    }
    if (SortedList_delete(elem) == 1) {
      fprintf(stderr, "Error the list is corrupted.\n");
      exit(2);
    }
  
    if (sync_ch == 'm')  {
      pthread_mutex_unlock(&m_locks[list_index]);
      long total_mutex_time = (mutex_stop.tv_sec - mutex_start.tv_sec)*BILLION + \
                         (mutex_stop.tv_nsec - mutex_start.tv_nsec);
      wait_times[th_ID] += total_mutex_time;
    }
    else if (sync_ch == 's')
      __sync_lock_release(&s_locks[list_index]);
  }

  return NULL;
}

void seg_handler() {
  fprintf(stderr, "Segmentation error caught\n");
  exit(2);
}

int main(int argc, char* argv[]) {
  int ret;
  struct timespec start, stop;
  long total_time = 0;
  long total_lock_time = 0;
  int i_flag = 0, d_flag = 0, l_flag = 0;

  struct option longopts[] = {
    {"threads", required_argument, NULL, 't'},
    {"iterations", required_argument, NULL, 'i'},
    {"yield", required_argument, NULL, 'y'},
    {"sync", required_argument, NULL, 's'},
    {"lists", required_argument, NULL, 'l'},
    {0,0,0,0}
  };

  while ((ret = getopt_long(argc, argv, "", longopts, NULL)) > 0) {
    switch(ret) {
      case 't':
        thread_num = atoi(optarg);
        if (thread_num < 1) {
          fprintf(stderr, "Error: the number of threads must be greater than 0\n");
          exit(1);
        }
        break;
      case 'i':
        iter = atoi(optarg);
        if (iter < 1) {
          fprintf(stderr, "Error: the number of iterations must be greater than 0\n");
          exit(1);
        }
        break;
      case 'y':
        opt_yield = 1;
        yield_ch = optarg;
        int len = strlen(yield_ch);
        for (int i = 0; i < len; i++) {
          if (yield_ch[i] == 'i') {
              opt_yield |= INSERT_YIELD;
              i_flag = 1;
          }
          else if (yield_ch[i] == 'd') {
            opt_yield |= DELETE_YIELD;
            d_flag = 1;
          }
          else if (yield_ch[i] == 'l') {
            opt_yield |= LOOKUP_YIELD;
            l_flag = 1;
          }
          else {
            fprintf(stderr, "Error with options provided to --yield. Only idl are accepted\n");
            exit(1);
          }
        }
        break;
      case 's':
        sync_ch = *optarg;
        if (sync_ch != 'm' && sync_ch != 's') {
          fprintf(stderr, "Error with options provided. Only sm are accepted.\n");
          exit(1);
        }
        break;
      case 'l':
        list_num = atoi(optarg);
        break;
      default:
        fprintf(stderr, "\rUsage: ./lab2_add [--options]\r\nOptions: --threads=# --iterations=# --yield=[idl] --sync=[sm]\n");
        exit(1);
    }
  }
   
  signal(SIGSEGV, seg_handler);

  if (sync_ch == 'm') {
    m_locks = malloc(list_num * sizeof(pthread_mutex_t));
    for (int i = 0; i < list_num; i++)
      ret = pthread_mutex_init(&m_locks[i], NULL);
    wait_times = malloc(thread_num * sizeof(long));
    for (int i = 0; i < thread_num; i++)
      wait_times[i] = 0;
  }
  else if (sync_ch == 's')
          s_locks = malloc(list_num * sizeof(long));
 
  // initialize empty lists
  head = malloc(list_num * sizeof(SortedList_t));
  for (int i = 0; i < list_num; i++) {
    head[i].key = NULL;
    head[i].next = head[i].prev = &head[i];
  }

  total_nodes = thread_num * iter;
  srand(time(NULL)); // seed the random function  
  
  // create a "pool" of elements that need to be assigned to threads
  pool = (SortedListElement_t *) malloc(total_nodes * sizeof(SortedListElement_t));
  if (pool == NULL) {
    fprintf(stderr, "Error with allocating memory to pool\n");
    exit(2);
  }

  // generate a random key for each element
  for (long i = 0; i < total_nodes; i++)
    pool[i].key = gen_rand_key();

  // create an array of indices for sectioning the element pool
  int start_index[thread_num];
  for (int i = 0, j = 0; i < thread_num; i++, j+=iter)
    start_index[i] = j;

  // get start time
  if ((ret = clock_gettime(CLOCK_MONOTONIC, &start)) < 0) {
    fprintf(stderr, "Error with getting start time\n");
    exit(1);
  } 

  // initialize array of threads
  pthread_t th_arr[thread_num];

  // create threads
  for (int i = 0; i < thread_num; i++) {
    if ((ret = pthread_create(&th_arr[i], NULL, th_worker, &start_index[i])) < 0) {
      fprintf(stderr, "Error with creating thread\n");
      exit(2);
    }
  }

  // join threads
  for (int i = 0; i < thread_num; i++) {
    if ((ret = pthread_join(th_arr[i], NULL)) < 0) {
      fprintf(stderr, "Error with joining thread\n");
      exit(2);
    }
  }

  // get end time
  if ((ret = clock_gettime(CLOCK_MONOTONIC, &stop)) < 0) {
    fprintf(stderr, "Error with getting end time\n");
    exit(1);
  }

  // check if list is 0
  for (int i = 0; i < list_num; i++) {
    if ((ret = SortedList_length(&head[i])) != 0) {
      fprintf(stdout, "The linked list is not 0\n");
      exit(2);
    }
  }

  // get total time
  total_time = (stop.tv_sec - start.tv_sec)*BILLION + (stop.tv_nsec - start.tv_nsec);
  long total_op = 3*thread_num*iter;
  long avg_time_per_op = total_time / total_op;

  fprintf(stdout, "list-");

  // yieldopts 
  if (i_flag == 1)
    fprintf(stdout, "i");
  if (d_flag == 1)
    fprintf(stdout, "d");
  if (l_flag == 1)
    fprintf(stdout, "l"); 
  if (!i_flag && !d_flag && !l_flag)
    fprintf(stdout, "none");

  // sync options
  if (sync_ch == 'm') {
    fprintf(stdout, "-m");
    for (int i = 0; i < thread_num; i++)
      total_lock_time += wait_times[i];
    free(m_locks);
    free(wait_times);
  }
  else if (sync_ch == 's') {
    fprintf(stdout, "-s");
    free(s_locks);
  }
  else
    fprintf(stdout, "-none");

  // thread_num, iter, number of lists, total operations, total runtime, avg time per operation, total lock acquisition time
  fprintf(stdout, ",%ld,%ld,%ld,%ld,%ld,%ld,%ld\n", thread_num, iter, list_num, \
          total_op, total_time, avg_time_per_op, (total_lock_time / total_op) );

  // REMEMBER TO FREE STRINGS
  for (long i = 0; i < total_nodes; i++)
    free((char*) pool[i].key);
  free(pool);
  free(head);

  exit(0);
}
