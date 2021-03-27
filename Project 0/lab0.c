// NAME: Belle Lerdworatawee
// EMAIL: bellelee@g.ucla.edu
// ID: 105375663

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <signal.h>

#define BUFFERSIZE 1024
char buffer[BUFFERSIZE];

void sigsegv_handler(int sig) {
  fprintf(stderr, "Segmentation fault caught by SIGSEGV handler, signal number: %d\n", sig);
  exit(4);
}

void cause_segfault(void) {
  char* ptr = NULL;
  *ptr = 'a';
}

int main(int argc, char* argv[]) {
  int segfault;
  int c, r, w;

  // arguments that can get passed in
  struct option longopts[] = {
    {"input", required_argument, NULL, 'i'},
    {"output", required_argument, NULL, 'o'},
    {"segfault", no_argument, NULL, 's'},
    {"catch", no_argument, NULL, 'c'},
    {0,0,0,0}
  };

  // determining the outputs
  while ((c = getopt_long(argc, argv, "", longopts, NULL)) > 0) {
    switch(c) {
      case 'i': ;
        int new_fd = open(optarg, O_RDONLY);
        if (new_fd >= 0) {
          close(0);
          dup(new_fd);
          close(new_fd);
        }
        else {
          fprintf(stderr,"%s could not be opened\n", optarg);
          fprintf(stderr, "%d: %s\n", errno, strerror(errno));
          exit(2);
        }
        break;
      case 'o': ;
        int old_fd = creat(optarg, 0666);
        if (old_fd >= 0) {
          close(1);
          dup(old_fd);
          close(old_fd);
        }
        else {
          fprintf(stderr, "%s could not be opened\n", optarg);
          fprintf(stderr, "%d: %s\n", errno, strerror(errno));
          exit(3);
        }
        break;
      case 's':
        segfault = 1;
        break;
      case 'c':
        signal(SIGSEGV, sigsegv_handler);
        break;
      default:
        fprintf(stderr, "Usage: ./lab0 [--options]\nOptions: --input=fileName --output=fileName --segfault --catch\n");
        exit(1);
    }
  }

  // if segfault is set, cause an error first
  if (segfault == 1)
    cause_segfault();

  // read from fd 0 and write to fd 1
  while ((r = read(0, &buffer, BUFFERSIZE)) > 0) {
    w = write(1, buffer, r);

    // check for any errors with read and write
    if (w != r) {
      fprintf(stderr, "Error: the number of bytes read does not match the number of bytes written. There was an error somewhere during the copying process\n");
      exit(2);
    } 
  }
  
  exit(0);
}
