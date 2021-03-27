# NAME: Belle Lerdworatawee
# EMAIL: bellelee@g.ucla.edu
# ID: 105375663

CC=gcc
CFLAGS=-Wall -Wextra -g -pthread

default: lab2_add lab2_list

lab2_add: lab2_add.c
	$(CC) $(CFLAGS) -o lab2_add lab2_add.c

lab2_list: lab2_list.c
	$(CC) $(CFLAGS) -o lab2_list lab2_list.c SortedList.c

tests: default test_add test_list

test_add:
	-rm -rf lab2_add.csv
		
	./lab2_add --threads=2 --iterations=10 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=20 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=40 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=80 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=20 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=40 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=80 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=20 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=40 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=80 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100000 >> lab2_add.csv	
	./lab2_add --threads=2 --iterations=10 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=20 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=40 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=80 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=20 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=40 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=80 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=1000 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100000 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=20 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=40 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=80 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100000 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100000 --yield >> lab2_add.csv
	
	./lab2_add --threads=2 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 >> lab2_add.csv	
	./lab2_add --threads=2 --iterations=100 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100000 --yield >> lab2_add.csv
	
	./lab2_add --iterations=5 >> lab2_add.csv
	./lab2_add --iterations=10 >> lab2_add.csv
	./lab2_add --iterations=100 >> lab2_add.csv
	./lab2_add --iterations=200 >> lab2_add.csv
	./lab2_add --iterations=500 >> lab2_add.csv
	./lab2_add --iterations=1000 >> lab2_add.csv
	./lab2_add --iterations=10000 >> lab2_add.csv
	./lab2_add --iterations=100000 >> lab2_add.csv
	
	./lab2_add --threads=2 --iterations=10000 --yield >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 --yield >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --yield >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=4 --iterations=1000 --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000 --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --yield --sync=c >> lab2_add.csv
	
	./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --sync=m >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=10000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=10000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=1000 --sync=s >> lab2_add.csv
	-./lab2_add --threads=1 --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=2 --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=4 --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=8 --iterations=10000 --sync=c >> lab2_add.csv
	-./lab2_add --threads=12 --iterations=10000 --sync=c >> lab2_add.csv
	
test_list:
	-rm -rf lab2_list.csv
		
	-./lab2_list --iterations=10 >> lab2_list.csv
	-./lab2_list --iterations=100 >> lab2_list.csv
	-./lab2_list --iterations=1000 >> lab2_list.csv
	-./lab2_list --iterations=10000 >> lab2_list.csv
	-./lab2_list --iterations=20000 >> lab2_list.csv
	
	-./lab2_list --threads=2 >> lab2_list.csv
	-./lab2_list --threads=4 >> lab2_list.csv
	-./lab2_list --threads=8 >> lab2_list.csv
	-./lab2_list --threads=12 >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=10 >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=10 >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=10 >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=10 >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=100 >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=100 >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=100 >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=100 >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=1000 >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1000 >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1000 >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1000 >> lab2_list.csv
	-./lab2_list --threads=2 --yield=i >> lab2_list.csv
	-./lab2_list --threads=4 --yield=i >> lab2_list.csv
	-./lab2_list --threads=8 --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --yield=i >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=10 --yield=i >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=10 --yield=i >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=10 --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=10 --yield=i >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=100 --yield=i >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=100 --yield=i >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=100 --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=100 --yield=i >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=1000 --yield=i >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1000 --yield=i >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1000 --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1000 --yield=i >> lab2_list.csv
	-./lab2_list --threads=2 --yield=d >> lab2_list.csv
	-./lab2_list --threads=4 --yield=d >> lab2_list.csv
	-./lab2_list --threads=8 --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --yield=d >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=10 --yield=d >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=10 --yield=d >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=10 --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=10 --yield=d >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=100 --yield=d >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=100 --yield=d >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=100 --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=100 --yield=d >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=1000 --yield=d >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1000 --yield=d >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1000 --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1000 --yield=d >> lab2_list.csv
	-./lab2_list --threads=2 --yield=il >> lab2_list.csv
	-./lab2_list --threads=4 --yield=il >> lab2_list.csv
	-./lab2_list --threads=8 --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --yield=il >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=10 --yield=il >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=10 --yield=il >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=10 --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=10 --yield=il >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=100 --yield=il >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=100 --yield=il >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=100 --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=100 --yield=il >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=1000 --yield=il >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1000 --yield=il >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1000 --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1000 --yield=il >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=10 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=10 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=10 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=10 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=100 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=100 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=100 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=100 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=1000 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1000 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1000 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1000 --yield=dl >> lab2_list.csv
	
	-./lab2_list --threads=12 --iterations=32 --yield=i  --sync=m >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=d  --sync=m >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=il --sync=m >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=dl --sync=m >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=i  --sync=s >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=d  --sync=s >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=il --sync=s >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=32 --yield=dl --sync=s >> lab2_list.csv
	
	-./lab2_list --iterations=1000 >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=1000 >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1000 >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1000 >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1000 >> lab2_list.csv
	-./lab2_list --threads=16 --iterations=1000 >> lab2_list.csv
	-./lab2_list --threads=24 --iterations=1000 >> lab2_list.csv
	-./lab2_list --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2_list.csv
	-./lab2_list --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2_list.csv
	-./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2_list.csv

graphs: tests
	./lab2_add.gp
	./lab2_list.gp

clean:
	rm -rf lab2_add lab2_list *.tar.gz

dist: graphs
	tar -czvf lab2a-105375663.tar.gz lab2_add.c lab2_list.c Makefile README SortedList.h SortedList.c *.csv *.png *.gp
