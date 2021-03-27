// NAME: Belle Lerdworatawee
// EMAIL: bellelee@g.ucla.edu
// ID: 105375663

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <getopt.h>
#include <poll.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#define BUFFERSIZE 1024
char buffer[BUFFERSIZE];

struct termios original_term;
int to_shell[2], to_term[2], pid, status;

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
  if (tcsetattr(0, TCSANOW, &original_term) < 0) {
    fprintf(stderr, "Error with restoring terminal - %d: %s\n", errno, strerror(errno));
    exit(1);
  }
}

void sigpipe_handler() {
  restore_terminal();
  if (close(to_shell[1]) < 0 || close(to_term[0]) < 0) {
    fprintf(stderr, "Error with closing to_shell[1] and to_term[0] - %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  waitpid(pid, &status, 0);
  fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", WTERMSIG(status), WEXITSTATUS(status));
  exit(0);
}

int main(int argc, char* argv[]) {
  int shell_flag = 0, c, r, w;

  terminal_setup(); // set terminal to no echo mode, non-canonical input 

  struct option longopts[] = {
    {"shell", no_argument, NULL, 's'},
    {0,0,0,0}
  };

  // process argument
  while ((c = getopt_long(argc, argv, "", longopts, NULL)) > 0) {
    switch(c) {
      case 's': 
        shell_flag = 1;
        break;
      default:
        fprintf(stderr, "\rUsage: ./lab1a [--options]\r\nOptions: --shell\r\n");
        restore_terminal();
        exit(1);
    }
  }
  
  if (shell_flag == 1) { // if shell is specified
    signal(SIGPIPE, sigpipe_handler);
    
    if (pipe(to_shell) < 0 || pipe(to_term) < 0) {
      fprintf(stderr, "Error with creating pipes - %d: %s\n", errno, strerror(errno));
      restore_terminal();
      exit(1);
    }

    if ((pid = fork()) < 0) { // error handling
      fprintf(stderr, "Error: child could not be created");
      restore_terminal();
      exit(1);
    }
    
      if (pid == 0) { // child process

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
        if (close(to_term[0]) < 0 || close(1) < 0) {
          restore_terminal();
          fprintf(stderr, "Error with closing terminal pipes in child - %d: %s\n", errno, strerror(errno));
          exit(1);
        }
        if (dup(to_term[1]) < 0 || close(to_term[1]) < 0) {
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
      else if (pid > 0) { // parent process
        struct pollfd pollfds_arr[2];

        if (close(to_shell[0]) < 0 || close(to_term[1]) < 0) {
          restore_terminal();
          fprintf(stderr, "Error with closing pipes in parent - %d: %s\n", errno, strerror(errno));
          exit(1);
        }
     
        pollfds_arr[0].fd = 0;
        pollfds_arr[0].events = POLLIN | POLLHUP | POLLERR;
        pollfds_arr[1].fd = to_term[0];
        pollfds_arr[1].events = POLLIN | POLLHUP | POLLERR;
 
        while (1) { // read and write
          if (poll(pollfds_arr, 2, -1) < 0) {
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

            for (int i = 0; i < r; i++) {
              if (buffer[i] == 0x4) { // ^D
                w = write(1, "^D\r\n", 4);
                close(to_shell[1]);
              }
              else if (buffer[i] == 0x3) { // ^C
                w = write(1, "^C\r\n", 4);
                if (kill(pid, SIGINT) < 0) {
                  fprintf(stderr, "Error with kill()");
                  exit(1);
                }
              }
              else if (buffer[i] == '\r' || buffer[i] == '\n') {
                w = write(to_shell[1], "\n", 1);
                write(1, "\r\n", 2);
              }
              else {
                w = write(to_shell[1], &buffer[i], 1);
                w = write(1, &buffer[i], 1);
              }

              if (w < 0) {
                restore_terminal();
                fprintf(stderr, "Error with read and write - %d: %s\n", errno, strerror(errno));
                exit(1);
              }
            }
          }

          if (pollfds_arr[1].revents & POLLIN) { // read from shell
            if ((r = read(to_term[0], buffer, BUFFERSIZE)) < 0) {
              restore_terminal();
              fprintf(stderr, "Error with read - %d: %s\n", errno, strerror(errno));
              exit(1);
            }

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
          
          if (pollfds_arr[0].revents & (POLLERR | POLLHUP)) { // error with polling terminal
            restore_terminal();
            fprintf(stderr, "Error with polling terminal - %d: %s\n", errno, strerror(errno));
            exit(1);      
          }
 
          if (pollfds_arr[1].revents & (POLLERR | POLLHUP)) { // error with polling shell
            restore_terminal();
            waitpid(pid, &status, 0);
            fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", WTERMSIG(status), WEXITSTATUS(status));
            if (close(to_shell[1]) < 0 || close(to_term[0]) < 0) {
              fprintf(stderr, "Error with closing pipes - %d: %s\n", errno, strerror(errno));
              exit(1);
            }
            exit(0);
          }
        }
      }
  }   
  else { //default mode
    while(1) {
      if ((r = read(0, buffer, BUFFERSIZE)) < 0) {
         restore_terminal();
         fprintf(stderr, "Error with read - %d: %s\n", errno, strerror(errno));
         exit(1);
      }

      for (int i = 0; i < r; i++) {
        if (buffer[i] == 0x4) { // ^D
          w = write(1, "^D\r\n", 4);
          restore_terminal();
          exit(0);
        }
        else if (buffer[i] == '\r' || buffer[i] == '\n')
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
  restore_terminal();
  exit(0);
}
