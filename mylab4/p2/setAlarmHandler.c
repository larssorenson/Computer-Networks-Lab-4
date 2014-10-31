#include "mytalk.h"

// Set the sig handler for SIGALRM
int setAlarmHandler()
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	
	sa.sa_handler = &noResponse;
	
	return sigaction(SIGALRM, &sa, NULL);
}
