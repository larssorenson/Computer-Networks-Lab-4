#include "mytalk.h"

int setPacketHandler()
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	
	sa.sa_sigaction = &handlePacket;
	sa.sa_flags = SA_SIGINFO;
	
	int resp = sigaction(SIGIO, &sa, NULL);
	resp |= sigaction(SIGPOLL, &sa, NULL );
	return resp;
}
