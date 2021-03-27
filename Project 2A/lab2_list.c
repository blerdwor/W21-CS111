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
long iter = 1;
char* yield_ch = NULL;
char sync_ch = 'x';
long lock = 0;
SortedListElement_t *head, *pool;
pthread_mutex_t mutex;

char* gen_rand_key() {
  char* str = malloc(4 * sizeof(char)); // arbitrary key length
  char alpha[] = "abcdefghijklmnopqrstuvwxyz";

  for (int i = 0; i < 3; i++)
    str[i] = alpha[rand() % 25];
  str[3] = '\0';
  
  return str;
}

void* th_worker(void* start) {
  int start_index = *((int *) start);
  int end_index = start_index + iter;

  for (int i = start_index; i < end_index; i++) {
    if (sync_ch == 'm')
      pthread_mutex_lock(&mutex);
    else if (sync_ch == 's')
      while(__sync_lock_test_and_set(&lock, 1));

    SortedList_insert(head, &(pool[i]));

    if (sync_ch == 'm')
      pthread_mutex_unlock(&mutex);
    else if (sync_ch == 's')
      __sync_lock_release(&lock);
  }

  if (sync_ch == 'm')
    pthread_mutex_lock(&mutex);
  else if (sync_ch == 's')
    while(__sync_lock_test_and_set(&lock, 1));

  if (SortedList_length(head) < 0) {
    fprintf(stderr, "Error the list is corrupted\n");
    exit(2);
  }

  if (sync_ch == 'm')
    pthread_mutex_unlock(&mutex);
  else if (sync_ch == 's')
    __sync_lock_release(&lock);

  for (int i = start_index; i < end_index; i++) {
    if (sync_ch == 'm')
      pthread_mutex_lock(&mutex);
    else if (sync_ch == 's')
      while(__sync_lock_test_and_set(&lock, 1));

    SortedListElement_t* elem = SortedList_lookup(head, pool[i].key);
    if (elem == NULL) {
      fprintf(stderr, "Error the key cannot be found\n");
      exit(2);
    }
    if (SortedList_delete(elem) == 1) {
      fprintf(stderr, "Error the list is corrupted.\n");
      exit(2);
    }

    if (sync_ch == 'm')
      pthread_mutex_unlock(&mutex);
    else if (sync_ch == 's')
      __sync_lock_release(&lock);
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
  int i_flag = 0, d_flag = 0, l_flag = 0;

  struct option longopts[] = {
    {"threads", required_argument, NULL, 't'},
    {"iterations", required_argument, NULL, 'i'},
    {"yield", required_argument, NULL, 'y'},
    {"sync", required_argument, NULL, 's'},
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
        if (sync_ch == 'm')
          ret = pthread_mutex_init(&mutex, NULL);
        else if (sync_ch != 's') {
          fprintf(stderr, "Error with options provided. Only sm are accepted.\n");
          exit(1);
        }
        break;
      default:
        fprintf(stderr, "\rUsage: ./lab2_add [--options]\r\nOptions: --threads=# --iterations=# --yield=[idl] --sync=[sm]\n");
        exit(1);
    }
  }
   
  signal(SIGSEGV, seg_handler);
 
  // initialize an empty list
  head = malloc(sizeof(SortedList_t));
  head->key = NULL;
  head->next = head->prev = head;

  int total_nodes = thread_num * iter;
  srand(time(NULL)); // seed the random function  
  // create a "pool" of elements that need to be assigned to threads
  pool = (SortedListElement_t *) malloc(total_nodes * sizeof(SortedListElement_t));
  if (pool == NULL) {
    fprintf(stderr, "Error with allocating memory to pool\n");
    exit(2);
  }
  
  // create an array of indices for sectioning the element pool
  int start_index[thread_num];
  for (int i = 0, j = 0; i < thread_num; i++, j+=iter)
    start_index[i] = j;

  // generate a random key for each element
  for (int i = 0; i < total_nodes; i++)
    pool[i].key = gen_rand_key();

  // get start time
  if ((ret = clock_gettime(CLOCK_MONOTONIC, &start)) < 0) {
    fprintf(stderr, "Error with getting start time\n");
    exit(1);
  } 

  // initialize array of threads
  pthread_t th_arr[thread_num];

  // create threads, i = thread_i
  for (int i = 0; i < thread_num; i++) {
    int thID = i;
    if ((ret = pthread_create(&th_arr[i], NULL, th_worker, &start_index[thID])) < 0) {
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
  if ((ret = SortedList_length(head)) != 0) {
    fprintf(stdout, "The linked list is not 0\n");
    exit(2);
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
  if (sync_ch == 'm')
    fprintf(stdout, "-m");
  else if (sync_ch == 's')
    fprintf(stdout, "-s");
  else
    fprintf(stdout, "-none");

  // thread_num, iter, number of lists, total operations, total runtime, avg time per operation
  fprintf(stdout, ",%ld,%ld,1,%ld,%ld,%ld\n", thread_num, iter, \
          total_op, total_time, avg_time_per_op);

  // REMEMBER TO FREE STRINGS
  for (int i = 0; i < total_nodes; i++)
    free((char*) pool[i].key);
  free(pool);
  free(head);

  exit(0);
}
