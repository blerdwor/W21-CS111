# NAME: Belle Lerdworatawee
# EMAIL: bellelee@g.ucla.edu
# ID: 105375663

CC=gcc
CFLAGS=-Wall -Wextra -g -lz

default: lab1b

lab1b: lab1b-client.c lab1b-server.c
	$(CC) $(CFLAGS) -o lab1b-client lab1b-client.c
	$(CC) $(CFLAGS) -o lab1b-server lab1b-server.c 

client: lab1b-client.c
	$(CC) $(CFLAGS) -o lab1b-client lab1b-client.c

server: lab1b-server.c
	$(CC) $(CFLAGS) -o lab1b-server lab1b-server.c

clean:
	rm -rf lab1b-client lab1b-server *.tar.gz

dist: lab1b-client.c lab1b-server.c 
	tar -czvf lab1b-105375663.tar.gz lab1b-client.c lab1b-server.c Makefile README
