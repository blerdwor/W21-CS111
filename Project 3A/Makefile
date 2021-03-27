# NAME: Belle Lerdworatawee
# EMAIL: bellelee@g.ucla.edu
# ID: 105375663

CC=gcc
CFLAGS=-Wall -Wextra -g

default: lab3a.c
	$(CC) $(CFLAGS) -o lab3a lab3a.c

clean:
	rm -rf lab3a *.tar.gz

dist: default
	tar -czvf lab3a-105375663.tar.gz lab3a.c Makefile README ext2_fs.h
