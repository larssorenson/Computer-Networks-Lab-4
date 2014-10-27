#!/bin/bash

testOutput()
{
	$1 >> output 2>&1
	if [ $3 -eq 1 ]
	then
		if [ $? -eq $2 ]
		then
			return 1
		else
			return 0
		fi
	else
		if [ $? -ne $2 ]
		then
			return 1
		else
			return 0
		fi
	fi
}

(sh compile.sh 1 1 > output 2>&1 && echo "Compiled!") || echo "Failed to Compile"

echo -n "Server Compilation\t\t\t"
if [ ! -f "./fsserver" ]
then
	echo "Failed!"
else
	echo "Passed!"
fi

echo -n "Client Compilation\t\t\t"
if [ ! -f "./fsclient" ]
then
	echo "Failed!"
else
	echo "Passed!"
fi

echo -n "Argument Check\t\t\t\t"
testOutput "./fsserver" 255 0
if [ $? -eq 1 ]
then
	echo "Passed!"
else
	echo "Failed!"
fi

echo -n "Port Parsing\t\t\t\t"
testOutput "./fsserver 50505" 254 0
if [ $? -eq 1 ]
then
	echo "Passed!"
else
	echo "Failed!"
fi

echo -n "Port Lower Bounds\t\t\t"
testOutput "./fsserver 49152" 254 0
if [ $? -eq 1 ]
then
	echo "Passed!"
else
	echo "Failed!"
fi

echo -n "Port Lower Bounds 2\t\t\t"
testOutput "./fsserver 49151" 254 1
if [ $? -eq 0 ]
then
	echo "Passed!"
else
	echo "Failed!"
fi

echo -n "Port Lower Bounds 3\t\t\t"
testOutput "./fsserver -1" 254 1
if [ $? -eq 0 ]
then
	echo "Passed!"
else
	echo "Failed!"
fi

echo -n "Port Upper Bounds\t\t\t"
testOutput "./fsserver 65535" 254 0
if [ $? -eq 1 ]
then
	echo "Passed!"
else
	echo "Failed!"
fi

echo -n "Port Upper Bounds 2\t\t\t"
testOutput "./fsserver 65536" 254 0
if [ $? -eq 1 ]
then
	echo "Passed!"
else
	echo "Failed!"
fi

echo -n "Port Upper Bounds 3\t\t\t"
testOutput "./fsserver 123123123123" 254 0
if [ $? -eq 1 ]
then
	echo "Passed!"
else
	echo "Failed!"
fi
