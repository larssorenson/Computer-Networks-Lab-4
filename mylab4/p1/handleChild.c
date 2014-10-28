#include "fs.h"

// Waiting on our zombie children
void handleChild(int sig)
{
	wait3(0,0,NULL);
	while(waitpid(-1,NULL,WNOHANG) > 0){}
}
