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
	
	commandLineBuffer = mallocAndCheck(sizeof(char)*1024);
	memset(commandLineBuffer, 0, 1024);
	
	addrlen = (socklen_t)sizeof(struct sockaddr_storage);
	
	udpSocket = bindUDPSocket();
	
	#ifdef Debug
		printf("\r\nUDP Socket: %d\r\n", udpSocket);
	#endif
	
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(port);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	
	// Bind said port to the acquired socket
	if(bind(udpSocket, (struct sockaddr*)&myaddr, sizeof(struct sockaddr)))
	{
		perror("Bind");
		return FUNCTION_ERROR;
	}
	
	// Set our handler for SIGPOLL/IO
	if(setPacketHandler())
	{
		perror("Packet Handler");
		return FUNCTION_ERROR;
	}
	
	// Set our handler for SIGALRM
	if(setAlarmHandler())
	{
		perror("Alarm Handler");
		return FUNCTION_ERROR;
	}

	// remember original terminal setting
	tcgetattr(0, &term1);
	term2 = term1;
	
	// set to noncanonical mode and disable automatic echoing, i.e.,
	// keyboard characters are immediately returned and not echo'ed 
	// to stdout
	term2.c_lflag &= ~(ICANON | ECHO); 
	tcsetattr(0, TCSANOW, &term2);

	// Mark the socket as being "owned" by the current process
	// Necessary for SIGIO and SIGPOLL to call the handler
	fcntl(udpSocket, F_SETOWN, getpid());

	// Set the socket to non-blocking and asynchronous mode to allow for
	// reading and writing in here and the handler
	if(fcntl(udpSocket, F_SETFL, O_ASYNC|O_NONBLOCK|FASYNC) < 0)
	{
		perror("Set Flag For Socket");
		return FUNCTION_ERROR;
	}

	char c;
	int totalRead = 0;
	int printFlag = 0;
	inChat = 0;
	
	// REPL!
	while(1)
	{
		// Clear the buffer for input
		memset(commandLineBuffer, 0, 1024);
		
		// Write our prompt
		write(1, "> ", 2);
		c = 0;
		totalRead = 0;
		
		// Put us in a loop until we've gotten a new line or
		// exceeded 1024 characters
		while(c != '\n' && totalRead < 1024)
		{
			// Flag on whether or not to print
			// this will prevent buggy printing behaviour
			printFlag = 0;
			c = getchar();
			
			// Backspace/delete do the same thing here
			// We just move the buffer back a space and zero it
			// Then flag it to print
			if ((c == 8 || c == 127) && totalRead > 0)
			{
				// Move backwards, print a space over the character
				// we're deleting
				// then do the same thing
				// to remove it from the buffer
				fputc('\b', stdout);
				fputc(' ', stdout);
				totalRead--;
				commandLineBuffer[totalRead] = '\0';
				#ifdef Debug
					printf("\rNew buffer: %s \r\n", commandLineBuffer);
				#endif
				printFlag = 1;
			}
			
			// If we have a printable ASCII character
			// add it to the buffer and flag it for printing
			else if(c > 31 && c != 127)
			{
				commandLineBuffer[totalRead] = c;
				totalRead++;
				printFlag = 1;
			}
			
			// If we're flagged to print
			// we move to the beginning of the line, write our prompt
			// and write out the buffer
			if (printFlag)
			{
				printTerminal();
			}
			
		}
		
		// Move us to a new line
		fputs("\n", stdout);
	
		#ifdef Debug
			printf("Read: (%d)  %s\r\n", totalRead, commandLineBuffer);
		#endif
		
		// If else chain for control input
		// First case: Exit the REPL (and by extension the program)
		if (!strcmp(commandLineBuffer, "q"))
		{
			// Quit, which amounts to breaking the loop
			// If we were in a chat, let them know we left
			if (inChat)
			{
				sendto(udpSocket, "D", 1, 0, (struct sockaddr*)&chataddr, addrlen);
			}
			
			break;
		}
		
		// Second case: accept a chat request
		else if (!strcmp(commandLineBuffer, "c") && !inChat)
		{
			// Accept the chat request
			#ifdef Debug
				printf("Accepting request\r\n");
			#endif
			if(sendto(udpSocket, "ok", 2, 0, (struct sockaddr*)&chataddr, addrlen) < 0)
			{
				perror("Chat Accept");
				return FUNCTION_ERROR;
			}
			
			inChat = 1;
		}
		
		// Third case: Reject a chat request
		else if (!strcmp(commandLineBuffer, "n") && !inChat)
		{
			// Reject the chat request
			#ifdef Debug
				printf("Rejecting request\r\n");
			#endif
			if(sendto(udpSocket, "ko", 2, 0, (struct sockaddr*)&chataddr, addrlen) < 0)
			{
				perror("Chat Reject");
				return FUNCTION_ERROR;
			}
			
		}
		
		// Fourth case: Cancel an inprogress chat
		else if (!strcmp(commandLineBuffer, "d") && inChat)
		{
			#ifdef Debug
				printf("Ending chat\r\n");
			#endif
			if (sendto(udpSocket, "D", 1, 0, (struct sockaddr*)&chataddr, addrlen) < 0)
			{
				perror("Chat End");
				return FUNCTION_ERROR;
			}
			
			inChat = 0;
		}
		
		// Otherwise, we interpret the input as either
		// a message for an inprogress chat
		// or a chat request being made.
		else
		{
			// Switch on whether or not we're in a chat currently
			if(inChat)
			{
				// send a message if we are
				int messageLen = strlen(commandLineBuffer) + 2;
				char* message = mallocAndCheck(messageLen);
				memset(message, 0, messageLen);
				strcat(message, "M");
				strcat(message, commandLineBuffer);
				sendto(udpSocket, message, messageLen, 0, (struct sockaddr*)&chataddr, addrlen);
			}
			
			else
			{
				// Send a chat request
				#ifdef Debug
					printf("Sending request\r\n");
				#endif
				
				// Find where the space between IP and port is
				char* sp = strstr(commandLineBuffer, " ");
				if (!sp)
				{
					printf("\rFailed to find IP!\r\n");
					continue;
				}
				
				// Calculate the length of the IP out of the input
				int iplen = sp-commandLineBuffer;
				#ifdef Debug
					printf("\rIP Len: %d \r\n", iplen);
				#endif
				
				char* ip = mallocAndCheck(iplen+1);
				memset(ip, 0, iplen+1);
				strncpy(ip, commandLineBuffer, iplen);
				
				#ifdef Debug
					printf("\rIP: %s \r\n", ip);
				#endif
				
				// Parse the string given to us for the IP
				if(inet_pton(AF_INET, ip, &(chataddr.sin_addr)) <= 0)
				{
					write(2, "Failed to parse IP Address!\r\n", 29);
					continue;
				}
				
				#ifdef Debug
					printf("\rPort Len: %d \r\n", totalRead - iplen);
				#endif
				
				// Parse the port out of the input
				char* strport = mallocAndCheck(totalRead-iplen);
				memset(strport, 0, totalRead-iplen);
				strncpy(strport, sp, totalRead-iplen);
				
				#ifdef Debug
					printf("\rPort: %s \r\n", strport);
				#endif
				
				// Populate the address struct for our request
				int chatPort = numberFromString(strport);
				chataddr.sin_port = htons(chatPort);
				chataddr.sin_family = AF_INET;
				
				// Send chat request
				if (sendto(udpSocket, "letschat", 8, MSG_DONTWAIT, (struct sockaddr*)&chataddr, addrlen) < 0)
				{
					perror("Chat Start");
					continue;
				}
				
				// Alarm for 5 minutes
				setAlarm(5 * 60);
			}
			
		}
		
	}
	
	
	tcsetattr(0, TCSANOW, &term1);	// return terminal to default mode
	alarm(0); // cancel any alarms, just in case
	
	return OK;
}
