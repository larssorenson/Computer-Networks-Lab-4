#include "mytalk.h"

void printTerminal()
{	
	fputs("\r", stdout);
	fputs("> ", stdout);
	fprintf(stdout, "%s", commandLineBuffer);
}
