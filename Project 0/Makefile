# NAME: Belle Lerdworatawee
# EMAIL: bellelee@g.ucla.edu
# ID: 105375663

CC=gcc
CFLAGS=-Wall -Wextra -g

default: lab0

lab0: lab0.c
	$(CC) $(CFLAGS) -o lab0 lab0.c 

check: lab0
	# unrecognized argument
	./lab0 --bad_arg; \
	if [ $$? -eq 1 ]; then \
		echo "-----------------------------------------------Test incorrect arguments passed"; \
	else \
		echo "-----------------------------------------------Test incorrect arguments failed"; \
	fi; \
	# existing input file, and create non-existing output file
	echo "testing testing" > i_file.txt; \
	./lab0 --input=i_file.txt --output=o_file.txt; \
	if [ $$? -eq 0 ]; then \
		echo "-----------------------------------------------Program exited with correct 0 status"; \
	else \
		echo "-----------------------------------------------Program did not exit with correct 0 status"; \
	fi; \
	cmp i_file.txt o_file.txt; \
	if [ $$? -eq 0 ]; then \
		echo "-----------------------------------------------Test existing input file successfully copied to created output file passed"; \
	else \
		echo "-----------------------------------------------Test existing input file successfully copied to created output file failed"; \
	fi; \
	# non-exisiting input file
	./lab0 --input=nonexistant_file.txt --output=o_file.txt; \
	if [ $$? -eq 2 ]; then \
		echo "-----------------------------------------------Test non-existant input file passed"; \
	else \
		echo "-----------------------------------------------Test non-existant input file failed"; \
	fi; \
	# input file without read permissions
	touch closed_file.txt; \
	chmod 000 closed_file.txt; \
	./lab0 --input=closed_file.txt --output=o_file.txt; \
	if [ $$? -eq 2 ]; then \
		echo "-----------------------------------------------Test unable to open input file passed"; \
	else \
		echo "-----------------------------------------------Test unable to open input file failed"; \
	fi; \
	# output file without read permissions
	./lab0 --input=i_file.txt --output=closed_file.txt; \
	if [ $$? -eq 3 ]; then \
		echo "-----------------------------------------------Test unable to write to output passed"; \
	else \
		echo "-----------------------------------------------Test unable to write to output passed"; \
	fi; \
	# overwrite existing file
	echo "These words need to be overwritten" > overwrite_file.txt; \
	./lab0 --input=i_file.txt --output=overwrite_file.txt; \
	if [ $$? -eq 0 ]; then \
		echo "-----------------------------------------------Program read and wrote successfully"; \
	else \
		echo "-----------------------------------------------Program read and wrote failed"; \
	fi; \
	cmp i_file.txt overwrite_file.txt; \
	if [ $$? -eq 0 ]; then \
		echo "-----------------------------------------------Test overwrite file passed"; \
	else \
		echo "-----------------------------------------------Test overwrite file failed"; \
	fi; \
	# catch
	./lab0 --segfault --catch; \
	if [ $$? -eq 4 ]; then \
		echo "-----------------------------------------------Test caught and received SIGSEGV passed"; \
	else \
		echo "-----------------------------------------------Test caught and received SIGSEGV passed"; \
	fi; \
	# segfault; no stdin to stdout
	echo "Do not change" > no_change.txt; \
	./lab0 --input=i_file.txt --output=no_change.txt --segfault; \
	if [ $$? -eq 139 ]; then \
		echo "-----------------------------------------------Segfault caused"; \
	else \
		echo "-----------------------------------------------Segfault not caused"; \
 	fi; \
	cmp i_file.txt no_change.txt; \
	if [ $$? -ne 0 ]; then \
		echo "-----------------------------------------------Test no stdin to stdout passed\n"; \
	else \
		echo "-----------------------------------------------Test no stdin to stdout failed\n"; \
	fi; \
	# invalid file AND segfault
	./lab0 --input=nonexistant_file.txt --segfault; \
        if [ $$? -eq 2 ]; then \
                echo "-----------------------------------------------Test invalid file and segfault passed"; \
        else \
                echo "-----------------------------------------------Test invalid file and segfault failed"; \
        fi; \
	# clean up files 
	rm -f i_file.txt o_file.txt overwrite_file.txt closed_file.txt no_change.txt

clean:
	rm -rf lab0 *.tar.gz

dist: lab0.c 
	tar -czvf lab0-105375663.tar.gz lab0.c Makefile README backtrace.png breakpoint.png
