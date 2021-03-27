# NAME: Belle Lerdworatawee
# EMAIL: bellelee@g.ucla.edu
# ID: 105375663

CC=gcc
CFLAGS=-Wall -Wextra -g
LDLIBS = -lssl -lcrypto

default:
	if [ `uname -a | grep -c "armv7l"` -ne 0 ]; then \
		$(CC) $(CFLAGS) -o lab4c_tcp lab4c_tcp.c -lm -lmraa; \
		$(CC) $(CFLAGS) -o lab4c_tls lab4c_tls.c -lm -lmraa $(LDLIBS); \
	else \
		$(CC) $(CFLAGS) -DDUMMY -o lab4c_tcp lab4c_tcp.c -lm; \
		$(CC) $(CFLAGS) -DDUMMY -o lab4c_tls lab4c_tls.c -lm $(LDLIBS); \
	fi

clean:
	rm -rf lab4c_tcp lab4c_tls *.tar.gz

dist: default
	tar -czvf lab4c-105375663.tar.gz lab4c_tcp.c lab4c_tls.c Makefile README