#include "mytalk.h"

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printUsage();
		return INSUFFICIENT_ARGUMENTS;
	}
	
	int port = parseAndCheckPort(argv[1]);
	#ifdef Debug
		printf("Port: %d\r\n", port);
	#endif
	
	char *buffer = mallocAndCheck(sizeof(char)*1024);
	memset(buffer, 0, 1024);
	
	socklen_t addrlen = (socklen_t)sizeof(struct sockaddr_storage);
	
	int udpSocket = bindUDPSocket();
	
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(port);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	
	while(1)
	{
		printf(">");
		char c[2];
		int totalRead = 0;
		c[0] = 0; c[1] = 0;
		while(c != '\n' && totalRead < 1024)
		{
			read(0, c, 1);
			if(c > 31 && c != 127)
			{
				strcat(buffer, c);
				totalRead++;
			}
			
		}
	
		totalRead = 0;
	}
	
	// Parse the arg given to us for the IP
	if(inet_pton(AF_INET, argv[1], &(myaddr.sin_addr)) <= 0)
	{
		write(2, "Failed to parse IP Address!\r\n", 29);
		return -1;
	}
}
