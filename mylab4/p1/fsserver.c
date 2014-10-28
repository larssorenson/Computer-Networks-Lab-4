#include "fs.h"

/*
	File Server...Server
	
	Argument:
		Server Port
		* The Port number through which the server will respond to requests and serve files

*/

#ifdef Testing
#define INF 0
#else
#define INF 1
#endif


int main (int argc, char** argv)
{

	// Check for proper arguments
	if(argc < 2)
	{
		printUsage();
		return INSUFFICIENT_ARGUMENTS;
	}
	
	// Parse the port provided for us
	int port = parseAndCheckPort(argv[1]);
	if (port < 0)
		return IMPROPER_ARGUMENTS;
	
	if(setChildHandler())
	{
		perror("Child Handler");
		return FUNCTION_ERROR;
	}
	
	// Malloc space for the buffer and zero it
	char* buffer = mallocAndCheck(1024);
	memset(buffer, 0, 1024);
	
	// Var to hold the size of the sock struct
	socklen_t addrlen = (socklen_t)sizeof(struct sockaddr_storage);
	
	// Struct for a connecting client's information
	struct sockaddr_in clientaddr;
	
	// Set up my address, for port binding
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(port);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	
	// Grab a TCP Socket
	int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(tcpSocket <= 0)
	{
		write(2, "Unable to bind socket!\r\n", 24);
		return FUNCTION_ERROR;
	}
	
	// Bind up the socket to myself
	if(bind(tcpSocket, (struct sockaddr*)&myaddr, sizeof(struct sockaddr)))
	{
		perror("Bind");
		return FUNCTION_ERROR;
	}
	
	// Listen on said socket
	if(listen(tcpSocket, 1) != 0)
	{
		perror("Listen");
		return FUNCTION_ERROR;
	}
	
	int new_socket;
	// Infinite loop
	while( INF )
	{
		// Get the new socket from accept
		if((new_socket = accept(tcpSocket, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
		{
			perror("Accept");
			return FUNCTION_ERROR;
		}

		memset(buffer, 0, 1024);

		// receive the message
		while(read(new_socket, buffer, 1024) <= 0)
		{}
		
		// Debugging, output which file was requested
		printf("Packet:\r\n%s\r\n", buffer);
		
		int notAllowed = access(buffer, R_OK);
		
		if (notAllowed)
		{
			// Not allowed
			if(write(new_socket, "INVALID_REQUEST", 15) <= 0)
			{
				perror("Invalid Request");
				return FUNCTION_ERROR;
			}
			
			continue;
		}
		
		// FORKING TIME!
		int pid = fork();
		
		// Error case
		if (pid == -1)
		{
			perror("Fork");
			return FUNCTION_ERROR;
		}
		
		// Child case
		else if (pid == 0)
		{
			// Malloc space for buffering the file as we read/write
			char* file = mallocAndCheck(1025);
			memset(file, 0, 1025);
			
			// Open the file for read only
			int fd = open(buffer, O_RDONLY);
			if(fd < 0)
			{
				perror("Open");
				return FUNCTION_ERROR;
			}
			
			int count = 1024;
		
			while( INF )
			{
				count = read(fd, file, 1024);
			
				// Send the file requested
				if(write(new_socket, file, count) <= 0)
				{
					perror("File Send");
					return FUNCTION_ERROR;
				}
			
				if (count < 1024)
					break;
			
			}
			
			close(new_socket);
			
		}
		
		// Neglect Parent case, because parent will just loop back around.
	}
	
	return OK;
}
