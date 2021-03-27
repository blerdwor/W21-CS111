// NAME: Belle Lerdworatawee
// EMAIL: bellelee@g.ucla.edu
// ID: 105375663

#ifdef DUMMY

typedef int mraa_aio_context;

// return fake device handler
mraa_aio_context mraa_aio_init(int p) { p++;return 0; }

// return fake temperature
int mraa_aio_read(mraa_aio_context c) { c++;return 650; }

// empty close function
void mraa_aio_close(mraa_aio_context c) { c++; }

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
#include <poll.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define B 4275 // thermistor value
#define R0 100000.0 // nominal base value
#define BUFFERSIZE 1024

int sockfd;
int period = 1;
int log_flag = 0;
int id = 0;
int portnum = -1;
char scale = 'F';
char *hostname = NULL;
char buffer[BUFFERSIZE];

float convert_temp_reading(int reading) {
  float R = 1023.0/((float) reading) - 1.0;
  R = R0 * R;
   
  float celsius = 1.0/(log(R/R0)/B + 1/298.15) - 273.15;
  float fahrenheit = (celsius * 9)/5 + 32;

  return (scale == 'C') ? celsius : fahrenheit;
}

// hostname: level.cs.ucla.edu
// portnum: 18000
int client_connect(char* hostname, unsigned int portnum) {
  
  struct sockaddr_in serv_addr;
  // AF_INET = IPv4 and SOCK_STREAM = TCP connection
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct hostent* server = gethostbyname(hostname);

  // convert hostname to ip address
  memset(&serv_addr, 0, sizeof(struct sockaddr_in));
  serv_addr.sin_family = AF_INET;
  memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

  // copy ip address from server to serv_addr and convert the portnum
  serv_addr.sin_port = htons(portnum);

  // initiate connection to server
  connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  return sock_fd;
}

int main(int argc, char* argv[]) {
  int ret = 0;
  int start = 1;
  float temp = 0.0;
  FILE *log_fd = NULL;
  
  // process input arguments
  struct option longopts[] = {
    {"period", required_argument, NULL, 'p'},
    {"scale", required_argument, NULL, 's'},
    {"log", required_argument, NULL, 'l'},
    {"id", required_argument, NULL, 'd'},
    {"host", required_argument, NULL, 'h'},
    {0,0,0,0}
  };

  while ((ret = getopt_long(argc, argv, "", longopts, NULL)) > 0) {
    switch(ret) {
      case 'p':
        period = atoi(optarg);
        break;
      case 's':
        if (*optarg != 'C' && *optarg != 'F') {
          fprintf(stderr, "Usage: ./lab4b --id=9-digit-# --host=name/address --log=file_path port_number [--options]\r\nOptions: --period=N --scale=C/F\n");
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
      case 'd':
        if (strlen(optarg) != 9) {
          fprintf(stderr, "Usage: ./lab4b --id=9-digit-# --host=name/address --log=file_path port_number [--options]\r\nOptions: --period=N --scale=C/F\n");
          exit(1);
        }
        id = atoi(optarg);
        break;
      case 'h':
        hostname = optarg;
        break;
      default:
        fprintf(stderr, "Usage: ./lab4b --id=9-digit-# --host=name/address --log=file_path port_number [--options]\r\nOptions: --period=N --scale=C/F\n");
        exit(1);
    }
  } 

  // check for portnum
  if (argc - optind == 1) 
    portnum = atoi(argv[optind]);
  else {
    fprintf(stderr, "Usage: ./lab4b --id=9-digit-# --host=name/address --log=file_path port_number [--options]\r\nOptions: --period=N --scale=C/F\n");
    exit(1);
  }

  // check for mandatory arguments
  if (log_flag ==  0 || id < 0 || hostname == NULL || portnum < 0) {
    fprintf(stderr, "Usage: ./lab4b --id=9-digit-# --host=name/address --log=file_path port_number [--options]\r\nOptions: --period=N --scale=C/F\n");
    exit(1);
  }

  // cpnnect to server
  sockfd = client_connect(hostname, portnum);
  dprintf(sockfd, "ID=105375663\n");
 
  // initialize temperature sensor
  mraa_aio_context temp_sensor = mraa_aio_init(1); // assume temp sensor is at address 0
  struct timespec ts;
  struct tm* tm;  
  struct pollfd poll_serv = {sockfd, POLLIN, 0};

  // read temperature / poll for arguments
  int loop = 1;
  while(loop) {

    // poll server for input
    if ((ret = poll(&poll_serv, 1, 1000)) == 1) {
      int r = read(sockfd, buffer, BUFFERSIZE);

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
        }
        else if (strncmp((buffer+i), "START", 5) == 0) {
          i += 5;
          start = 1;
        }
        else if (strncmp((buffer+i), "OFF", 3) == 0) {
          clock_gettime(CLOCK_REALTIME, &ts);
          tm = localtime(&(ts.tv_sec));
          dprintf(sockfd, "%02d:%02d:%02d SHUTDOWN\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
          if (log_flag)
            fprintf(log_fd,"%02d:%02d:%02d SHUTDOWN\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
          loop = 0;
        }
      }
      memset(buffer, '\0', r); // clear the buffer so leftover characters don't get written
    }
    else if (ret < 0)
      fprintf(stderr, "Error with polling\n");

    // print time
    if (start == 1 && loop == 1) {
      clock_gettime(CLOCK_REALTIME, &ts);
      tm = localtime(&(ts.tv_sec));
      if ((tm->tm_sec % period) == 0) {
        temp = convert_temp_reading(mraa_aio_read(temp_sensor));
        dprintf(sockfd, "%02d:%02d:%02d %.1f\n", tm->tm_hour, tm->tm_min, tm->tm_sec, temp);
        if (log_flag)
          fprintf(log_fd, "%02d:%02d:%02d %.1f\n", tm->tm_hour, tm->tm_min, tm->tm_sec, temp);
      }
    }
  }

  // close button
  mraa_aio_close(temp_sensor);

  if (log_fd != NULL) {
    if ((ret = fclose(log_fd)) < 0) {
      fprintf(stderr, "Error with closing log file\n");
      exit(1);
    }
  }

  exit(0);
}
