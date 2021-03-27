#! /usr/bin/env bash

# bad argument
echo | ./lab4b --bogus &> /dev/null
if [[ $? -ne 1 ]]; then
	echo "Test bogus argument returns wrong exit code"
else
	echo "Test bogus argument passed"
fi

# correct argument
./lab4b --period=3 --scale="C"--log="file" <<-EOF
SCALE=C
STOP
OFF
EOF
if [ $? -ne 0 ]
then
	echo "Test valid arguments returns wrong exit code"
else
	echo "Test valid arguments passed"
fi

# log file content
./lab4b --log="file" <<-EOF
SCALE=C
PERIOD=3
LOG hello
STOP
OFF
EOF
if [ $? -ne 0 ]
then
	echo "Test valid arguments returns wrong exit code"
else
	echo "Test valid arguments passed"
fi
 
if [ ! -s file ]
then
        echo "Test did not create a log file"
fi

grep "SCALE=C" file; \
if [ $? -eq 0 ]; 
then
	echo "SCALE=C logged correctly"
else
	echo "SCALE=C logged incorrectly"
fi

grep "PERIOD=3" file; \
if [ $? -eq 0 ]; 
then
	echo "PERIOD=3 logged correctly"
else
	echo "PERIOD=3 logged incorrectly"
fi

grep "LOG hello" file; \
if [ $? -eq 0 ]; 
then
	echo "LOG hello logged correctly"
else
	echo "LOG hello logged incorrectly"
fi
