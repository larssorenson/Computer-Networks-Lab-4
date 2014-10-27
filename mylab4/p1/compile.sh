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

make all EXECUTABLE=fsserver DEBUG="$DEBUG Debug" EXEC="-D Server" TEST="$TEST Testing"
make all EXECUTABLE=fsclient DEBUG="$DEBUG Debug" EXEC="-D Client" TEST="$TEST Testing"
