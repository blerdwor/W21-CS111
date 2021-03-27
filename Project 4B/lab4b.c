// NAME: Belle Lerdworatawee
// EMAIL: bellelee@g.ucla.edu
// ID: 105375663

#ifdef DUMMY
#define MRAA_GPIO_IN 0

typedef int mraa_aio_context;
typedef int mraa_gpio_context;

// return fake device handler
mraa_aio_context mraa_aio_init(int p) { p++;return 0; }

// return fake temperature
int mraa_aio_read(mraa_aio_context c) { c++;return 650; }

// empty close function
void mraa_aio_close(mraa_aio_context c) { c++; }

// return fake device handler
mraa_gpio_context mraa_gpio_init(int p) { p++;return 0; }

// empty dir function
void mraa_gpio_dir(mraa_gpio_context c, int mode) { c++;mode++; }

// return button not pushed
int mraa_gpio_read(mraa_gpio_context c) { c++;return 0; }

// empty close function
void mraa_gpio_close(mraa_gpio_context c) { c++; }

#else

#include <mraa.h>

#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <signal.h>
#include <getopt.h>
#include <time.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define B 4275 // thermistor value
#define R0 100000.0 // nominal base value
#define BUFFERSIZE 1024

int period = 1;
int log_flag = 0;
char scale = 'F';
char buffer[BUFFERSIZE];

float convert_temp_reading(int reading) {
  float R = 1023.0/((float) reading) - 1.0;
  R = R0 * R;
   
  float celsius = 1.0/(log(R/R0)/B + 1/298.15) - 273.15;
  float fahrenheit = (celsius * 9)/5 + 32;

  return (scale == 'C') ? celsius : fahrenheit;
}

int main(int argc, char* argv[]) {
  int ret = 0;
  int start = 1, stop = 0;
  float temp = 0.0;
  FILE *log_fd = NULL;
  
  // process input arguments
  struct option longopts[] = {
    {"period", required_argument, NULL, 'p'},
    {"scale", required_argument, NULL, 's'},
    {"log", required_argument, NULL, 'l'},
    {0,0,0,0}
  };

  while ((ret = getopt_long(argc, argv, "", longopts, NULL)) > 0) {
    switch(ret) {
      case 'p':
        period = atoi(optarg);
        break;
      case 's':
        if (*optarg != 'C' && *optarg != 'F') {
          fprintf(stderr, "Usage: ./lab4b [--options]\r\nOptions: --period=N --scale=C/F --log=file_path\n");
          exit(1);
        }
        scale = *optarg;
        break;
      case 'l':
        if ((log_fd = fopen(optarg, "w")) == NULL) {
          fprintf(stderr, "Error with opening file\n");
          exit(1);
        }
        log_flag = 1;
        break;
      default:
        fprintf(stderr, "Usage: ./lab4b [--options]\r\nOptions: --period=N --scale=C/F --log=file_path\n");
        exit(1);
    }
  } 

  // initialize button
  mraa_gpio_context button = mraa_gpio_init(60); // assume button is at address 73
  /*if (button == NULL)
    fprintf(stderr, "Error with connecting button\n");*/
  mraa_gpio_dir(button, MRAA_GPIO_IN);
  
  // initialize temperature sensor
  mraa_aio_context temp_sensor = mraa_aio_init(1); // assume temp sensor is at address 0
  /*if (temp_sensor == NULL)
    fprintf(stderr, "Error with connecting sensor\n");*/

  struct timespec ts;
  struct tm* tm;  
  struct pollfd poll_stdin = {0, POLLIN, 0};

  // read temperature / poll for arguments
  int loop = 1;
  while(loop) {

    // print time
    if (start == 1 && stop == 0) {
      clock_gettime(CLOCK_REALTIME, &ts);
      tm = localtime(&(ts.tv_sec));
      if ((tm->tm_sec % period) == 0) {
        temp = convert_temp_reading(mraa_aio_read(temp_sensor));
        fprintf(stdout, "%02d:%02d:%02d %.1f\n", tm->tm_hour, tm->tm_min, tm->tm_sec, temp);
        if (log_flag)
          fprintf(log_fd, "%02d:%02d:%02d %.1f\n", tm->tm_hour, tm->tm_min, tm->tm_sec, temp);
      }
    }

    // poll stdin for input
    if ((ret = poll(&poll_stdin, 1, 1000)) == 1) {
      int r = read(0, buffer, BUFFERSIZE);

      if (log_flag)
        fprintf(log_fd, "%s", buffer);

      int i;
      for (i = 0; i < r; i++) {
        if (strncmp((buffer+i), "SCALE=F", 7) == 0) {
          i += 7;
          scale = 'F';
        }
        else if (strncmp((buffer+i), "SCALE=C", 7) == 0) {
          i += 7;
          scale = 'C';
        }
        else if (strncmp((buffer+i), "PERIOD=", 7) == 0) {
          i += 7;
          int num = 0;
          while (isdigit(buffer[i])) {
            num *= 10;
            num += (buffer[i++] - '0');
          }
          period = num;
        }
        else if (strncmp((buffer+i), "STOP", 4) == 0) {
          i += 4;
          start = 0;
          stop = 1;
        }
        else if (strncmp((buffer+i), "START", 5) == 0) {
          i += 5;
          stop = 0;
          start = 1;
        }
        /*else if (strncmp((buffer+i), "LOG ", 4) == 0) {
          if (log_flag) {
            while (buffer[i] != '\n')
              fprintf(log_fd, "%c", buffer[i++]);
            fprintf(log_fd, "\n");
          }
        }*/
        else if (strncmp((buffer+i), "OFF", 3) == 0) {
          clock_gettime(CLOCK_REALTIME, &ts);
          tm = localtime(&(ts.tv_sec));
          fprintf(stdout, "%02d:%02d:%02d SHUTDOWN\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
          if (log_flag)
            fprintf(log_fd,"%02d:%02d:%02d SHUTDOWN\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
          loop = 0;
        }
      }
      memset(buffer, '\0', r); // clear the buffer so leftover characters don't get written
    }
    else if (ret < 0)
      fprintf(stderr, "Error with polling\n");

    // button was pushed to signal stop
    if ((ret = mraa_gpio_read(button)) == 1) {
      clock_gettime(CLOCK_REALTIME, &ts);
      tm = localtime(&(ts.tv_sec));
      fprintf(stdout, "%02d:%02d:%02d SHUTDOWN\n", tm->tm_hour, tm->tm_min, tm->tm_sec);  
      if (log_flag)
        fprintf(log_fd, "%02d:%02d:%02d SHUTDOWN\n", tm->tm_hour, tm->tm_min, tm->tm_sec); 
      loop = 0;
    }
  }

  // close devices
  mraa_gpio_close(button);
  mraa_aio_close(temp_sensor);

  if (log_fd != NULL) {
    if ((ret = fclose(log_fd)) < 0) {
      fprintf(stderr, "Error with closing log file\n");
      exit(1);
    }
  }

  exit(0);
}
