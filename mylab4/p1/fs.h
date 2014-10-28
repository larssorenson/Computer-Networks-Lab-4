#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <wait.h>
#define FILE_SIZE 131072

enum ERROR				/* Defines ERROR Enumeration 					*/
{
	OK = 0,				/* Error code for no error 					*/
	INSUFFICIENT_ARGUMENTS = -1,	/* Error code for not providing the correct number of arguments */
	IMPROPER_ARGUMENTS = -2,	/* Error code for improper argument(s) supplied			*/
	FUNCTION_ERROR = -3		/* Error code for a function call failing			*/
};

void printUsage();
int parseAndCheckPort(char* port);
void* mallocAndCheck(size_t size);
int numberFromString(char* str);
void handleChild(int sig);
int setChildHandler();
