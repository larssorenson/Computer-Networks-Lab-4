#include "mytalk.h"

// Handler for an incoming packet
void handlePacket(int sig, siginfo_t* siginf, void* ucontext)
{
	#ifdef Debug
		printf("\rHandling packet.\r\n");
	#endif
	char* buffer = mallocAndCheck(1024);
	memset(buffer, 0, 1024);
	
	int restore = 0;
	
	// If we're not in a chat, we care about who sent the message
	// otherwise, we assume it is part of the conversation and we don't care about the address
	if(inChat)
	{
		if(read(udpSocket, buffer, 1024) < 0)
		{
			perror("Packet Handler Read");
		}
		
	}
	
	else
	{
		if(recvfrom(udpSocket, buffer, 1024, 0, (struct sockaddr*)&chataddr, &addrlen) < 0)
		{
			perror("Packet Handler Read");
		}
		
	}
	
	#ifdef Debug
		printf("Received: %s\r\n", buffer);
	#endif
	
	// If else chain on all control messages
	// First case: Chat request coming in
	if (strcmp(buffer, "letschat") == 0)
	{
		if(!inChat)
		{
			printf("\r%s wants to chat!\r\n\tType 'c' to accept or 'n' to reject.\r\n", inet_ntoa(chataddr.sin_addr));
			restore = 1;
		}
		
	}
	
	// Second case: Rejected chat request
	else if (strcmp(buffer, "ko") == 0)
	{
		if (!inChat)
		{
			printf("\r%s rejected your chat!\r\n", inet_ntoa(chataddr.sin_addr));
			inChat = 0;
			restore = 1;
		}
		
	}
	
	// Third: Accepted chat request
	else if (strcmp(buffer, "ok") == 0)
	{
		if(!inChat)
		{
			printf("\r%s accepted your chat!\r\n", inet_ntoa(chataddr.sin_addr));
			inChat = 1;
			restore = 1;
			alarm(0);
		}
		
	}
	
	// Fourth: Ended chat request
	else if (strcmp(buffer, "D") == 0)
	{
		if(inChat)
		{
			printf("\r%s ended your chat!\r\n", inet_ntoa(chataddr.sin_addr));
			inChat = 0;
			restore = 1;
		}
		
	}
	
	// Base Case: Received a message	
	else
	{
		char* message = buffer+1;
		printf("\r: %s\r\n", message);
		restore = 1;
	}
	
	if(restore)
	{
		printTerminal();
	}
	
}
