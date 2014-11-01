#include "mytalk.h"

void noResponse()
{
	// Output a message if we got no response
	fputs("\rNo Response For Chat Request.\r\n", stdout);
	
	printTerminal();
}
