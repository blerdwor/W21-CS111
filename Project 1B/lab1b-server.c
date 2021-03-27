// NAME: Belle Lerdworatawee
// EMAIL: bellelee@g.ucla.edu
// ID: 105375663

/*****Server*****/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <getopt.h>
#include <poll.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "zlib.h"

#define BUFFERSIZE 1024
char buffer[BUFFERSIZE];

extern int errno;

int socket_fd;

// pipe stuff
int to_shell[2];
int from_shell[2];
int pid;
int status;

// zlib stuff
z_stream to_client; // compress data to shell
z_stream from_client; // decompress data from term

// flag
int compress_flag = -1;

// didn't change the name, but this makes sure everything exits correctly
void restore_terminal(void) {
  
  close(socket_fd);

  if (compress_flag == 1) {
    inflateEnd(&from_client);
    deflateEnd(&to_client);
  }
  
}

void sigpipe_handler() { // SIGPIPE handler
  restore_terminal();
  if (close(to_shell[1]) < 0 || close(from_shell[0]) < 0) {
    fprintf(stderr, "Error with closing to_shell[1] and from_shell[0] - %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  waitpid(pid, &status, 0);
  fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", WTERMSIG(status), WEXITSTATUS(status));
  exit(0);
}

// server body
int server_connect(unsigned int port_num) {
  
  int sockfd, nfd, ret;
  struct sockaddr_in my_addr;
  struct sockaddr_in their_addr;
  socklen_t sin_size = sizeof(my_addr);
 
  // create socket
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "Error in creating socket - %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  
  // fill in address info
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(port_num); // converts from host byte order to network byte order
  my_addr.sin_addr.s_addr = INADDR_ANY;

  memset(my_addr.sin_zero, '\0', sizeof(my_addr.sin_zero)); // padding zeros

  bind(sockfd, (struct sockaddr*) &my_addr, sin_size); // bind socket to IP address and port number

  listen(sockfd, 5); // listen up to 5 connections

  // wait for client connection and store client addr in their_addr
  nfd = accept(sockfd, (struct sockaddr*) &their_addr, &sin_size);

  // close sockfd
  if ((ret = close(sockfd)) < 0) {
    fprintf(stderr, "Error with closing sockfd on server side - %d: %s\n", errno, strerror(errno));
    exit(1);
  }

  return nfd;
}

void init_compression(void) {

  int ret;
  to_client.zalloc = Z_NULL;
  to_client.zfree = Z_NULL;
  to_client.opaque = Z_NULL;
  ret = deflateInit(&to_client, Z_DEFAULT_COMPRESSION);
  if (ret != Z_OK) {
    restore_terminal();
    fprintf(stderr, "Error with deflating the stream - %d: %s\n", errno, strerror(errno));
    exit(1);
  }

  from_client.zalloc = Z_NULL;
  from_client.zfree = Z_NULL;
  from_client.opaque = Z_NULL;
  ret = inflateInit(&from_client);
  if (ret != Z_OK) {
    restore_terminal();
    fprintf(stderr, "Error with inflating the stream - %d: %s\n", errno, strerror(errno));
    exit(1);
  }
}

int main(int argc, char* argv[]) {
  int c, r, w;
  int socket_fd; 
  int port = -1;

  struct option longopts[] = {
    {"port", required_argument, NULL, 'p'},
    {"compress", no_argument, NULL, 'c'},
    {0,0,0,0}
  };

  // process argument
  while ((c = getopt_long(argc, argv, "", longopts, NULL)) > 0) {
    switch(c) {
      case 'p':
        port = atoi(optarg);
        break;
      case 'c':
        compress_flag = 1;
        init_compression();
        break;
      default:
        fprintf(stderr, "\rUsage: ./lab1b-server --port=portnum [--options]\r\nOptions: --compress\r\n");
        restore_terminal();
        exit(1);
    }
  }

  if (port == -1) { // mandate port option
    fprintf(stderr, "--port=portnum option is required");
    exit(1);
  }

  // create a socket
  socket_fd = server_connect(port);

  // set up shell
  signal(SIGPIPE, sigpipe_handler); // register SIGPIPE handler

  // create pipes
  if (pipe(to_shell) < 0 || pipe(from_shell) < 0) {
    fprintf(stderr, "Error with creating pipes - %d: %s\n", errno, strerror(errno));
    exit(1);
  }

  if ((pid = fork()) < 0) { // error handling
    fprintf(stderr, "Error: child could not be created");
    exit(1);
  }

  if (pid == 0) { // child process, stdin/ stdout redirection

    // terminal --> shell (read)
    if (close(to_shell[1]) < 0 || close(0) < 0) {
      restore_terminal();
      fprintf(stderr, "Error with closing shell pipes in child - %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    if (dup(to_shell[0]) < 0 || close(to_shell[0]) < 0) {
      restore_terminal();
      fprintf(stderr, "Error with duplicating pipes or closing pipes - %d: %s\n", errno, strerror(errno));
      exit(1);
    }

    // shell --> terminal (write)
    if (close(from_shell[0]) < 0 || close(1) < 0) {
      restore_terminal();
      fprintf(stderr, "Error with closing terminal pipes in child - %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    if (dup(from_shell[1]) < 0 || close(from_shell[1]) < 0) {
      restore_terminal();
      fprintf(stderr, "Error with duplicating pipes or closing pipes - %d: %s\n", errno, strerror(errno));
      exit(1);
    }

    // execute shell program
    if (execlp("/bin/bash", "/bin/bash",(char*)NULL) < 0) {
      restore_terminal();
      fprintf(stderr, "Error with executing shell - %d: %s\n", errno, strerror(errno));
      exit(1);
    }
  }
  else { // parent process, stdin/stdout redirection
    
    if (close(to_shell[0]) < 0 || close(from_shell[1]) < 0) {
      restore_terminal();
      fprintf(stderr, "Error with closing pipes in parent - %d: %s\n", errno, strerror(errno));
      exit(1);
    }

    // set up polling structs
    struct pollfd pollfds_arr[2];   
    pollfds_arr[0].fd = socket_fd;
    pollfds_arr[0].events = POLLIN | POLLHUP | POLLERR;
    pollfds_arr[1].fd = from_shell[0];
    pollfds_arr[1].events = POLLIN | POLLHUP | POLLERR;
 
    while (1) { // read and write
      if ((r = poll(pollfds_arr, 2, -1)) < 0) {
        restore_terminal();
        fprintf(stderr, "Error with poll - %d: %s\n", errno, strerror(errno));
        exit(1);
      }
      if (r == 0) {
        restore_terminal();
        fprintf(stderr, "Nothing to poll\n");
        exit(1);
      }
             
      if (pollfds_arr[0].revents & POLLIN) { // read from client, write to_shell
        if ((r = read(socket_fd, buffer, BUFFERSIZE)) < 0) {
          restore_terminal();
          fprintf(stderr, "Error with read - %d: %s\n", errno, strerror(errno));
          exit(1);
        }
        
        if (compress_flag == 1) { // decompress data from client
          unsigned char out_buf[BUFFERSIZE];
          from_client.avail_in = r;
          from_client.next_in = (unsigned char*) buffer;
          from_client.avail_out = BUFFERSIZE;
          from_client.next_out = out_buf;

          do {
            if (inflate(&from_client, Z_SYNC_FLUSH) != Z_OK) {
              restore_terminal();
              fprintf(stderr, "Error with inflating data\n");
              exit(1);
            }  
          }
          while (from_client.avail_in > 0);
          
          // forward to shell
         int decompressed_bytes = BUFFERSIZE - (int) from_client.avail_out;
         for (int i = 0; i < decompressed_bytes; i++) {
           if (out_buf[i] == 0x4) // ^D
             close(to_shell[1]); 
           else if (out_buf[i] == 0x3) { // ^C
             if (kill(pid, SIGINT) < 0) { 
               fprintf(stderr, "Error with kill()");
               exit(1);
             }
           }
           else if (out_buf[i] == '\r' || out_buf[i] == '\n')
             w = write(to_shell[1], "\n", 1);
           else
             w = write(to_shell[1], &out_buf[i], 1);

           if (w < 0) {
             restore_terminal();
             fprintf(stderr, "Error with read and write - %d: %s\n", errno, strerror(errno));
             exit(1);
           }
         }
       }
       else { // no compression
         // forward to shell
          for (int i = 0; i < r; i++) {
            if (buffer[i] == 0x4) // ^D
              close(to_shell[1]);
            else if (buffer[i] == 0x3) { // ^C
              if (kill(pid, SIGINT) < 0) {
                fprintf(stderr, "Error with kill()");
                exit(1);
              }
            }
            else if (buffer[i] == '\r' || buffer[i] == '\n')
              w = write(to_shell[1], "\n", 1);
            else
              w = write(to_shell[1], &buffer[i], 1);

            if (w < 0) {
              restore_terminal();
              fprintf(stderr, "Error with read and write - %d: %s\n", errno, strerror(errno));
              exit(1);
            }
          }
        }
      }
      else if (pollfds_arr[0].revents & (POLLERR | POLLHUP) ) {
        restore_terminal();
        fprintf(stderr, "Error with polling client socket - %d: %s\n", errno, strerror(errno));
        exit(0);
      }

      if (pollfds_arr[1].revents & POLLIN) { // read from shell, write to client
        if ((r = read(from_shell[0], buffer, BUFFERSIZE)) < 0) {
          restore_terminal();
          fprintf(stderr, "Error with read - %d: %s\n", errno, strerror(errno));
          exit(1);
        }
        
        if (compress_flag == 1) { // compress data from shell
          unsigned char out_buf[BUFFERSIZE];
          to_client.avail_in = r;
          to_client.next_in = (unsigned char*) buffer;
          to_client.avail_out = BUFFERSIZE;
          to_client.next_out = out_buf;

          do {
            if (deflate(&to_client, Z_SYNC_FLUSH) != Z_OK) {
              restore_terminal();
              fprintf(stderr, "Error with inflating data\n");
              exit(1);
            }  
          }
          while (to_client.avail_in > 0);

          // write the data to the socket
          int compressed_bytes = BUFFERSIZE - to_client.avail_out;
          if ((w = write(socket_fd, out_buf, compressed_bytes)) < 0) {
            restore_terminal();
            fprintf(stderr, "Error with sending message to socket");
            exit(1);
          }
        }
        else { // forward data normally 
        
          for (int i = 0; i < r; i++) {
            if (buffer[i] == '\r' || buffer[i] == '\n')
              w = write(socket_fd, "\n", 1);
            else
              w = write(socket_fd, &buffer[i], 1);

            if (w < 0) {
              restore_terminal();
              fprintf(stderr, "Error with read and write - %d: %s\n", errno, strerror(errno));
              exit(1);
            }
          }
        }
      }
      else if (pollfds_arr[1].revents & (POLLERR | POLLHUP)) { // error with polling shell
        waitpid(pid, &status, 0);
        close(to_shell[1]);
        close(from_shell[1]);
        fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", WTERMSIG(status), WEXITSTATUS(status));
        restore_terminal();
        exit(0);
      }
    }   
  } 

  restore_terminal();
  exit(0);
}
