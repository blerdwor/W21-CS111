// NAME: Belle Lerdworatawee
// EMAIL: bellelee@g.ucla.edu
// ID: 105375663

/*****Client*****/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <getopt.h>
#include <poll.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include "zlib.h"

#define BUFFERSIZE 1024
char buffer[BUFFERSIZE];

extern int errno;
struct termios original_term;

int socket_fd = -1;

// zlib stuff
z_stream to_shell; // compress data to shell
z_stream from_shell; // decompress data from term

// flags
int log_fd = -1;
int log_flag = -1;
int port = -1;
int compress_flag = -1;

void terminal_setup(void) { // function to setup the terminal
  struct termios tmp;
  
  if (!isatty(0)) { // error handling
    fprintf(stderr, "Not a terminal");
    exit(1);
  }

  // modify terminal attributes
  tcgetattr(0, &original_term);
  tmp = original_term;
  tmp.c_iflag = ISTRIP, tmp.c_oflag = 0, tmp.c_lflag = 0;
  tcsetattr(0, TCSANOW, &tmp);
}

void restore_terminal(void) { // reset terminal
  
  close(socket_fd);

  if (log_flag == 1)
    close(log_fd);

  if (compress_flag == 1) {
    inflateEnd(&from_shell);
    deflateEnd(&to_shell);
  }

  if (tcsetattr(0, TCSANOW, &original_term) < 0) {
    fprintf(stderr, "Error with restoring terminal - %d: %s\n", errno, strerror(errno));
    exit(1);
  }
}

void init_compression(void) {
  
  int ret; 
  to_shell.zalloc = Z_NULL;
  to_shell.zfree = Z_NULL;
  to_shell.opaque = Z_NULL;
  if ((ret = deflateInit(&to_shell, Z_DEFAULT_COMPRESSION)) != Z_OK) {
    fprintf(stderr, "Error with deflating the stream - %d: %s\n", errno, strerror(errno));
    exit(1);
  }

  from_shell.zalloc = Z_NULL;
  from_shell.zfree = Z_NULL;
  from_shell.opaque = Z_NULL;
  if ((ret = inflateInit(&from_shell)) != Z_OK) {
    fprintf(stderr, "Error with inflating the stream - %d: %s\n", errno, strerror(errno));
    exit(1);
  }
}

int client_connect(unsigned int port) { // client body
  
  int sockfd, ret;
  struct sockaddr_in serv_addr;
  struct hostent* srvr;
  int addr_len = sizeof(serv_addr);

  // create socket
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "Error in creating socket - %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  
  // fill in socket address info
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  srvr = gethostbyname("localhost"); // convert host_name to an IP address
  memcpy(&serv_addr.sin_addr.s_addr, srvr->h_addr, srvr->h_length); // copy the IP address from srvr to serv_addr 
  memset(serv_addr.sin_zero, '\0', sizeof(serv_addr.sin_zero)); // padding zeros

  // connect socket to address
  if ((ret = connect(sockfd, (struct sockaddr*) &serv_addr, addr_len)) < 0) {
    fprintf(stderr, "Error in connecting to server\n");
    exit(1);
  }

  return sockfd;
}

int main(int argc, char* argv[]) {
  int ret, r, w;

  struct option longopts[] = {
    {"port", required_argument, NULL, 'p'},
    {"log", required_argument, NULL, 'l'},
    {"compress", no_argument, NULL, 'c'},
    {0,0,0,0}
  };

  // process arguments
  while ((ret = getopt_long(argc, argv, "", longopts, NULL)) > 0) {
    switch(ret) {
      case 'p':
         port = atoi(optarg);
         break;
      case 'l':
        if ((log_fd = open(optarg, O_WRONLY | O_CREAT, 0666)) < 0) {
          fprintf(stderr, "Error with opening or creating log_file\n");
          exit(1); 
        }
        log_flag = 1;
        break;
      case 'c':
        compress_flag = 1;
        init_compression();
        break;
      default:
        fprintf(stderr, "\rUsage: ./lab1b-client --port=portnum [--options]\r\nOptions: --compress --log\r\n");
        exit(1);
    }
  }
  
  if (port == -1) { // mandate port option
    fprintf(stderr, "Error: --port=portnum option is required\n");
    exit(1);
  }
    
  // create a socket
  socket_fd = client_connect(port);
  
  terminal_setup();

  // start poll process  
  struct pollfd pollfds_arr[2];
  pollfds_arr[0].fd = 0; // read from keyboard
  pollfds_arr[0].events = POLLIN | POLLHUP | POLLERR;
  pollfds_arr[1].fd = socket_fd; // read from socket
  pollfds_arr[1].events = POLLIN | POLLHUP | POLLERR;
 
  // process data
  while (1) {
    if ((ret = poll(pollfds_arr, 2, -1)) < 0) {
      restore_terminal();
      fprintf(stderr, "Error with poll - %d: %s\n", errno, strerror(errno));
      exit(1);
    }
             
    if (pollfds_arr[0].revents & POLLIN) { // read from keyboard
      
      if ((r = read(0, buffer, BUFFERSIZE)) < 0) {
        restore_terminal();
        fprintf(stderr, "Error with read - %d: %s\n", errno, strerror(errno));
        exit(1);
      }

      // first write to stdout so we can see what we type
      for (int i = 0; i < r; i++) {
        if (buffer[i] == 0x4) // ^D
          w = write(1, "^D\r\n", 4);
        else if (buffer[i] == 0x3) // ^C
          w = write(1, "^C\r\n", 4);
        else if (buffer[i] == '\r' || buffer[i] == '\n') // newline
          write(1, "\r\n", 2);
        else
          w = write(1, &buffer[i], 1);

        if (w < 0) {
          restore_terminal();
          fprintf(stderr, "Error with read and write - %d: %s\n", errno, strerror(errno));
          exit(1);
        }
      }
  
      if (compress_flag == 1) { // compress data
        unsigned char out_buf[BUFFERSIZE];
        to_shell.avail_in = r;
        to_shell.next_in = (unsigned char*) buffer;
        to_shell.avail_out = BUFFERSIZE;
        to_shell.next_out = out_buf;

        do {
            if (deflate(&to_shell, Z_SYNC_FLUSH) != Z_OK) {
              restore_terminal();
              fprintf(stderr, "Error with inflating data\n");
              exit(1);
            }  
        }
        while (to_shell.avail_in > 0);
         
        int compressed_bytes = BUFFERSIZE - to_shell.avail_out; 
        // write the data to the socket
        if ((w = write(socket_fd, out_buf, compressed_bytes)) < 0) {
          restore_terminal();
          fprintf(stderr, "Error with sending message to socket");
          exit(1);
        }
        
        // write the data to a file if need be
        if (log_flag == 1)
          dprintf(log_fd, "SENT %d bytes: %s\n", compressed_bytes, out_buf);
      }
      else { // no compression needed
        if ((w = write(socket_fd, buffer, r)) < 0) {
          restore_terminal();
          fprintf(stderr, "Error with sending message to socket");
          exit(1);
        }

        if (log_flag == 1)
          dprintf(log_fd, "SENT %d bytes: %s\n", r, buffer);
      }
    }
    else if (pollfds_arr[0].revents & POLLERR) { // error with polling terminal
      restore_terminal();
      fprintf(stderr, "Error with polling the terminal");
      exit(1);
    }

    if (pollfds_arr[1].revents & POLLIN) { // read from socket
      if ((r = read(socket_fd, buffer, BUFFERSIZE)) < 0) {
        restore_terminal();
        fprintf(stderr, "Error with reading from socket - %d: %s\n", errno, strerror(errno));
        exit(1);
      }
      else if (r == 0) { // socket has closed
        restore_terminal();
        exit(0);
      } 

      if (log_flag == 1)
        dprintf(log_fd, "RECEIVED %d bytes: %s\n", r, buffer);

      if (compress_flag == 1) { // decompress data from shell
        unsigned char out_buf[BUFFERSIZE];
        from_shell.avail_in = r;
        from_shell.next_in = (unsigned char*) buffer;
        from_shell.avail_out = BUFFERSIZE;
        from_shell.next_out = out_buf;

        do {
            if (inflate(&from_shell, Z_SYNC_FLUSH) != Z_OK) {
              restore_terminal();
              fprintf(stderr, "Error with inflating data\n");
              exit(1);
            }  
        }
        while (from_shell.avail_in > 0);

        int decompressed_bytes = BUFFERSIZE - from_shell.avail_out;
        for (int i = 0; i < decompressed_bytes; i++) { // write to stdout
          if (out_buf[i] == '\r' || out_buf[i] == '\n')
            w = write(1, "\r\n", 2);
          else
            w = write(1, &out_buf[i], 1);

          if (w < 0) {
            restore_terminal();
            fprintf(stderr, "Error with read and write - %d: %s\n", errno, strerror(errno));
            exit(1);
          }
        }
      }
      else { // no compression

        if (log_flag == 1) 
          dprintf(log_fd, "RECEIVED %d bytes: %s\n", r, buffer);

        for (int i = 0; i < r; i++) {
          if (buffer[i] == '\r' || buffer[i] == '\n')
            w = write(1, "\r\n", 2);
          else
            w = write(1, &buffer[i], 1);

          if (w < 0) {
            restore_terminal();
            fprintf(stderr, "Error with read and write - %d: %s\n", errno, strerror(errno));
            exit(1);
          }
        }
      }
    }
    else if (pollfds_arr[1].revents & (POLLHUP | POLLERR)) {
      restore_terminal();
      fprintf(stderr, "Server has disconnected\n");
      exit(0);
    } 
  }

  restore_terminal();
  exit(0);
}
