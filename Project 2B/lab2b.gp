#! /usr/bin/gnuplot
#
# purpose:
#	 generate data reduction graphs for the multi-threaded list project
#
# input: lab2_list.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. run time per operation (ns)
#	8. wait-for-lock time
#
# output:
#	lab2_1.png
#	lab2_2.png
#	lab2_3.png
#	lab2_4.png 
#	lab2_5.png
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.
#
#	Early in your implementation, you will not have data for all of the
#	tests, and the later sections may generate errors for missing data.
#

# general plot parameters
set terminal png
set datafile separator ","

# throughput for each synchronization method
set title "List-1: Total operations per second vs threads"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Total operations per second"
set logscale y 10
set output 'lab2b_1.png'
set key right top

plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title '1000 iterations w/mutex' with linespoints lc rgb 'blue', \
    "< grep -e 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title '1000 iterations w/spin-lock' with linespoints lc rgb 'green'

# average time it took to get the lock
set title "List-2: Average Wait-for-Lock Time and Cost per Operation"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange[0.75:]
set ylabel "Wait-for-Lock time and Cost per Operation (ns)"
set logscale y 10
set output 'lab2b_2.png'
set key left top

plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($7) \
	title 'average cost per operation' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($8) \
	title 'wait-for-lock' with linespoints lc rgb 'green'

# successful runs vs threads with and without protection 
set title "List-3: Unprotected and Protected Threads that run without failure"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Successful Iterations"
set logscale y 10
set output 'lab2b_3.png'
# note that unsuccessful runs should have produced no output
plot \
     "< grep 'list-id-none,[0-9]*,[0-9]*,4' lab2b_list.csv" using ($2):($3) \
	title 'yield=id' with points lc rgb 'red', \
     "< grep 'list-id-m,[0-9]*,[0-9]*,4' lab2b_list.csv" using ($2):($3) \
	title 'yield=id w/ mutex' with points lc rgb 'blue', \
     "< grep 'list-id-s,[0-9]*,[0-9]*,4' lab2b_list.csv" using ($2):($3) \
	title 'yield=id w/ spinlock' with points lc rgb 'orange'

# throughput of mutex protected version w/o yield      
set title "List-4: Total operations per second vs threads w/ mutex"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange[0.75:32]
set ylabel "Total operations per second"
set logscale y 10
set output 'lab2b_4.png'
set key right top

plot \
    "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title '1 list' with linespoints lc rgb 'blue', \
    "< grep -e 'list-none-m,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title '4 lists' with linespoints lc rgb 'green', \
    "< grep -e 'list-none-m,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title '8 lists' with linespoints lc rgb 'red', \
    "< grep -e 'list-none-m,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title '16 lists' with linespoints lc rgb 'orange'

# throughput of spinlock protected version w/o yield      
set title "List-5: Total operations per second vs threads w/ spinlock"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange[0.75:32]
set ylabel "Total operations per second"
set logscale y 10
set output 'lab2b_5.png'
set key right top

plot \
    "< grep -e 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title '1 list' with linespoints lc rgb 'blue', \
    "< grep -e 'list-none-s,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title '4 lists' with linespoints lc rgb 'green', \
    "< grep -e 'list-none-s,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title '8 lists' with linespoints lc rgb 'red', \
    "< grep -e 'list-none-s,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000)/($7) \
	title '16 lists' with linespoints lc rgb 'orange'
