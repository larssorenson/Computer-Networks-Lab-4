#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>

enum ERROR				/* Defines ERROR Enumeration 					*/
{
	OK = 0,				/* Error code for no error 					*/
	INSUFFICIENT_ARGUMENTS = -1,	/* Error code for not providing the correct number of arguments */
	IMPROPER_ARGUMENTS = -2,	/* Error code for improper argument(s) supplied			*/
};

void printUsage();
int parseAndCheckPort(char* port);
void* mallocAndCheck(size_t size);
int numberFromString(char* str);
