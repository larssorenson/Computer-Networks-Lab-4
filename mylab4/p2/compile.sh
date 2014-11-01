make clean

DEBUG="-U"
if [ $# -gt 0 ]
then
	DEBUG="-D"
fi

TEST="-U"
if [ $# -gt 1 ]
then
	TEST="-D"
fi

make all EXECUTABLE=mytalk DEBUG="$DEBUG Debug" TEST="$TEST Testing"
