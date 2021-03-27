# NAME: Belle Lerdworatawee
# EMAIL: bellelee@g.ucla.edu
# ID: 105375663

CC=gcc
CFLAGS=-Wall -Wextra -g -pthread

default: lab2_list.c
	$(CC) $(CFLAGS) -o lab2_list lab2_list.c SortedList.c

tests: default 
	-rm -rf lab2b_list.csv
	
	-./lab2_list --iterations=1000 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2b_list.csv
	-./lab2_list --iterations=1000 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2b_list.csv
	
	-./lab2_list --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --iterations=2 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --iterations=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --iterations=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --iterations=16 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=2 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=16 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=2 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=16 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=2 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=16 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=2 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=16 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --iterations=10 --yield=id --lists=4 --sync=m	 >> lab2b_list.csv
	-./lab2_list --iterations=20 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --iterations=40 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --iterations=80 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=10 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=20 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=40 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=80 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=10 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=20 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=40 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=80 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=10 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=20 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=40 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=80 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=10 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=20 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=40 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=80 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --iterations=10 --yield=id --lists=4 --sync=s	 >> lab2b_list.csv
	-./lab2_list --iterations=20 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --iterations=40 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --iterations=80 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=10 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=20 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=40 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=80 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=10 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=20 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=40 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=80 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=10 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=20 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=40 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=80 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=10 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=20 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=40 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=80 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	
	-./lab2_list --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
	-./lab2_list --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
	-./lab2_list --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
	-./lab2_list --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=2 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv

profile: default
	rm -f ./raw.gperf
	LD_PRELOAD=/usr/lib64/libprofiler.so CPUPROFILE=./raw.gperf ./lab2_list --threads=12 --iterations=1000 --sync=s
	rm -rf profile.out
	pprof --text ./lab2_list ./raw.gperf >> profile.out
	pprof --list=th_worker ./lab2_list ./raw.gperf >> profile.out
	rm -f ./raw.gperf

graphs: tests
	./lab2b.gp

clean:
	rm -rf lab2_list *.tar.gz

dist: graphs
	tar -czvf lab2b-105375663.tar.gz lab2_list.c Makefile README SortedList.h SortedList.c lab2b_list.csv *.png lab2b.gp profile.out
