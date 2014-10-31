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
#include <termios.h>

enum ERROR				/* Defines ERROR Enumeration 					*/
{
	OK = 0,				/* Error code for no error 					*/
	INSUFFICIENT_ARGUMENTS = -1,	/* Error code for not providing the correct number of arguments */
	IMPROPER_ARGUMENTS = -2,	/* Error code for improper argument(s) supplied			*/
	FUNCTION_ERROR = -3		/* Error code for a function call failing			*/
};


struct sockaddr_in chataddr;
struct termios term1, term2;

void printUsage();
int parseAndCheckPort(char* port);
void* mallocAndCheck(size_t size);
int numberFromString(char* str);
void handleChild(int sig);
int setChildHandler();
void handlePacket(int sig);
int setPacketHandler();
int bindUDPSocket();
void setAlarm(int time);
int setAlarmHandler();
void noResponse();
