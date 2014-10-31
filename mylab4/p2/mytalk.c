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
	
	// Bind said port to the acquired socket
	if(bind(udpSocket, (struct sockaddr*)&myaddr, sizeof(struct sockaddr)))
	{
		perror("Bind");
		return -1;
	}
	
	setPacketHandler();
	setAlarmHandler();

	// remember original terminal setting
	tcgetattr(0, &term1);
	term2 = term1;
	// set to noncanonical mode and disable automatic echoing, i.e.,
	// keyboard characters are immediately returned and not echo'ed 
	// to stdout
	term2.c_lflag &= ~(ICANON | ECHO); 
	tcsetattr(0, TCSANOW, &term2);

	char c;
	int totalRead = 0;
	int printFlag = 0;
	int inChat = 0;
	while(1)
	{
		memset(buffer, 0, 1024);
		write(1, "> ", 2);
		c = 0;
		totalRead = 0;
		while(c != '\n' && totalRead < 1024)
		{
			printFlag = 0;
			c = getchar();
			
			if ((c == 8 || c == 127) && totalRead > 0)
			{
				totalRead--;
				buffer[totalRead] = '\0';
				#ifdef Debug
					printf("\r\n New buffer: %s \r\n", buffer);
				#endif
				printFlag = 1;
			}
			
			else if(c > 31 && c != 127)
			{
				buffer[totalRead] = c;
				totalRead++;
				printFlag = 1;
			}
			
			if (printFlag)
			{
				fputs("\r", stdout);
				fputs("> ", stdout);
				fprintf(stdout, "%s", buffer);
			}
			
		}
		
		fputs("\n", stdout);
	
		#ifdef Debug
			printf("Read: (%d)  %s\r\n", totalRead, buffer);
		#endif
		
		if (!strcmp(buffer, "q"))
		{
			// Quit, which amounts to breaking the loop
			break;
		}
		
		else if (!strcmp(buffer, "c"))
		{
			// Accept the chat request
			#ifdef Debug
				printf("Accepting request\r\n");
			#endif
			sendto(udpSocket, "ok", 2, 0, (struct sockaddr*)&chataddr, addrlen);
			inChat = 1;
		}
		
		else if (!strcmp(buffer, "n"))
		{
			// Reject the chat request
			#ifdef Debug
				printf("Rejecting request\r\n");
			#endif
			sendto(udpSocket, "ko", 2, 0, (struct sockaddr*)&chataddr, addrlen);
		}
		
		else if (!strcmp(buffer, "d"))
		{
			#ifdef Debug
				printf("Ending chat\r\n");
			#endif
			sendto(udpSocket, "D", 1, 0, (struct sockaddr*)&chataddr, addrlen);
			inChat = 0;
		}
		
		else
		{
			// Switch on whether or not we're in a chat currently
			if(inChat)
			{
				// send a message if we are
				int messageLen = strlen(buffer) + 2;
				char* message = mallocAndCheck(messageLen);
				memset(message, 0, messageLen);
				strcat(message, "M");
				strcat(message, buffer);
				sendto(udpSocket, message, messageLen, 0, (struct sockaddr*)&chataddr, addrlen);
			}
			
			else
			{
				// Send a chat request
				#ifdef Debug
					printf("Sending request\r\n");
				#endif
				
				// Find where the space between IP and port is
				char* sp = strstr(buffer, " ");
				if (!sp)
				{
					printf("\r\n Failed to find IP!\r\n");
					continue;
				}
				
				// Calculate the length of the IP out of the input
				int iplen = sp-buffer;
				#ifdef Debug
					printf("\r\n IP Len: %d \r\n", iplen);
				#endif
				
				char* ip = mallocAndCheck(iplen+1);
				memset(ip, 0, iplen+1);
				strncpy(ip, buffer, iplen);
				
				#ifdef Debug
					printf("\r\n IP: %s \r\n", ip);
				#endif
				
				// Parse the string given to us for the IP
				if(inet_pton(AF_INET, ip, &(chataddr.sin_addr)) <= 0)
				{
					write(2, "Failed to parse IP Address!\r\n", 29);
					return IMPROPER_ARGUMENTS;
				}
				
				#ifdef Debug
					printf("\r\n Port Len: %d \r\n", totalRead - iplen);
				#endif
				
				// Parse the port out of the input
				char* strport = mallocAndCheck(totalRead-iplen);
				memset(strport, 0, totalRead-iplen);
				strncpy(strport, sp, totalRead-iplen);
				
				#ifdef Debug
					printf("\r\n Port: %s \r\n", strport);
				#endif
				
				// Populate the address struct
				int chatPort = numberFromString(strport);
				chataddr.sin_port = htons(chatPort);
				chataddr.sin_family = AF_INET;
				
				// Send chat request
				sendto(udpSocket, "letschat", 8, 0, (struct sockaddr*)&chataddr, addrlen);
				
				// Alarm for 5 minutes
				setAlarm(5);
			}
			
		}
		
	}
	
	
	tcsetattr(0, TCSANOW, &term1);	// return terminal to default mode
	alarm(0);
	
	return OK;
}
