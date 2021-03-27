# NAME: Belle Lerdworatawee
# EMAIL: bellelee@g.ucla.edu
# ID: 105375663

CC=gcc
CFLAGS=-Wall -Wextra -g

default:
	if [ `uname -a | grep -c "armv7l"` -ne 0 ]; then \
		$(CC) $(CFLAGS) -o lab4b lab4b.c -lm -lmraa; \
	else \
		$(CC) $(CFLAGS) -DDUMMY -o lab4b lab4b.c -lm; \
	fi

check:
	chmod 777 check.sh
	./check.sh

clean:
	rm -rf lab4b *.tar.gz

dist: default
	tar -czvf lab4b-105375663.tar.gz lab4b.c Makefile README check.sh
