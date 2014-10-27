#include "fs.h"

int parseAndCheckPort(char* port)
{
	int port_res = numberFromString(port);
	if (port_res < 49152 || port_res > 65535)
	{
		write(2, "The port must be between 49152 and 65535!\r\n", 43);
		return -1;
	}
	
	return port_res;
}
