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

#define BILLION 1E9

int opt_yield;
char sync_ch = 'x';
long lock = 0;
pthread_mutex_t mutex;

typedef struct add_args {
  long long* c_ptr;
  int iterations;
}add_args;

void add(long long *pointer, long long value) {
  long long sum = *pointer + value;
  if (opt_yield)
    sched_yield();
  *pointer = sum;
}

void add_m(long long *pointer, long long value) {
  pthread_mutex_lock(&mutex);
  long long sum = *pointer + value;
  if (opt_yield)
    sched_yield();
  *pointer = sum;
  pthread_mutex_unlock(&mutex);
}

void add_s(long long *pointer, long long value) {
  while (__sync_lock_test_and_set(&lock, 1)); 
  long long sum = *pointer + value;
  if (opt_yield)
    sched_yield();
  *pointer = sum;
  __sync_lock_release(&lock);
}

void add_c(long long *pointer, long long value) {
  long long prev, sum;
  do {
    prev = *pointer;
    sum = prev + value;
    if (opt_yield)
      sched_yield();
  } while (__sync_val_compare_and_swap(pointer, prev, sum) != prev);
  // this function returns prev only after it swaps sum into pointer
}

void* choose_add(void* arguments) { 
  int iter = ((add_args*)arguments)->iterations; 
 
  for (int i = 0; i < iter; i++) {
    if (sync_ch == 'm') {
      add_m(((add_args*)arguments)->c_ptr, 1);
      add_m(((add_args*)arguments)->c_ptr, -1);  
    }
    else if (sync_ch == 's') {
      add_s(((add_args*)arguments)->c_ptr, 1);
      add_s(((add_args*)arguments)->c_ptr, -1); 
    }
    else if (sync_ch == 'c') {
      add_c(((add_args*)arguments)->c_ptr, 1);
      add_c(((add_args*)arguments)->c_ptr, -1);
    }
    else {
      add(((add_args*)arguments)->c_ptr, 1);
      add(((add_args*)arguments)->c_ptr, -1);
    }
  }
  
  return NULL; 
}

int main(int argc, char* argv[]) {
  int ret;
  long thread_num = 1, iter = 1;
  struct timespec start, stop;
  long total_time = 0;
  long long c = 0;

  struct option longopts[] = {
    {"threads", required_argument, NULL, 't'},
    {"iterations", required_argument, NULL, 'i'},
    {"yield", no_argument, NULL, 'y'},
    {"sync", required_argument, NULL, 's'},
    {0,0,0,0}
  };

  while ((ret = getopt_long(argc, argv, "", longopts, NULL)) > 0) {
    switch(ret) {
      case 't':
        thread_num = atoi(optarg);
        break;
      case 'i':
        iter = atoi(optarg);
        break;
      case 'y':
        opt_yield = 1;
        break;
      case 's':
        sync_ch = *optarg;
        if (sync_ch == 'm')
          ret = pthread_mutex_init(&mutex, NULL);
        else if (sync_ch != 's' && sync_ch != 'c') {
          fprintf(stderr, "\rUsage: ./lab2_add [--options]\r\nOptions: --threads=# --iterations=# --yield --sync=[msc]\n");
          exit(1); 
        }
        break;
      default:
        fprintf(stderr, "\rUsage: ./lab2_add [--options]\r\nOptions: --threads=# --iterations=# --yield --sync=[msc]\n");
        exit(1);
    }
  }
  
  // initialize struct to hold arguments
  add_args args = { .c_ptr = &c, .iterations = iter };
 
  // get start time
  if ((ret = clock_gettime(CLOCK_MONOTONIC, &start)) < 0) {
    fprintf(stderr, "Error with getting start time\n");
    exit(1);
  }

  // initialize array of threads
  pthread_t th_arr[thread_num];

  // create threads
  for (int i = 0; i < thread_num; i++) {
    if ((ret = pthread_create(&th_arr[i], NULL, choose_add, &args)) < 0) {
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

  // get total time
  total_time = (stop.tv_sec - start.tv_sec)*BILLION + (stop.tv_nsec - start.tv_nsec);

  long total_op = 2*thread_num*iter;
  long avg_time_per_op = total_time / total_op;

  fprintf(stdout, "add-");
  if (opt_yield)
    fprintf(stdout, "yield-");

  switch(sync_ch) {
    case 'm':
      fprintf(stdout, "m");
      break;
    case 's':
      fprintf(stdout, "s");
      break;
    case 'c':
      fprintf(stdout, "c");
      break;
    default:
      fprintf(stdout, "none"); 
  }

  // print out name, thread_num, iter, # of operations, total runtime, avg time per operation, total at end of run
  fprintf(stdout, ",%ld,%ld,%ld,%ld,%ld,%lld\n", thread_num, iter, \
          total_op, total_time, avg_time_per_op, c);

  exit(0);
}
