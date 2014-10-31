#include "mytalk.h"

void noResponse()
{
	fputs("No Response For Chat Request.\r\n", stdout);
	fputc(' ', stdout);
	fputc('\x08', stdout);
	
}
