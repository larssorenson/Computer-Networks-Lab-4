#include "fs.h"

// Set our Child Hander
int setChildHandler()
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	
	sa.sa_handler = &handleChild;
	
	return sigaction(SIGCHLD, &sa, NULL );
}
