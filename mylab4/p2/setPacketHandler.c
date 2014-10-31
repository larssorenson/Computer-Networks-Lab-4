#include "mytalk.h"

int setPacketHandler()
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	
	sa.sa_handler = &handlePacket;
	
	return sigaction(SIGPOLL, &sa, NULL );
}
