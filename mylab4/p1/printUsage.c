#include "fs.h"

/*
	printUsage
		Arguments:
			None
		Returns:
			None
		Function:
			Displays the arguments to the corresponding binary
*/
void printUsage()
{
	printf("Usage:\r\n");
#ifdef Server
	printf("\t./fsserver server-port\r\n");	
#endif
#ifdef Client
	printf("\t./fsclient server-IP server-port remotepathname localfilename\r\n");
#endif
}
