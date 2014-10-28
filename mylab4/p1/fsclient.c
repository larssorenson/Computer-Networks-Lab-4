#include "fs.h"

int main(int argc, char** argv)
{
	// Check argument count, need IP, Port, remote file, and local filename
	if (argc < 5)
	{
		printUsage();
		return INSUFFICIENT_ARGUMENTS;
	}
	
	// Check the bounds of the port, following lab spec
	int port = parseAndCheckPort(argv[2]);
	if (port < 0)
		return IMPROPER_ARGUMENTS;
	
	// Malloc our buffer for the file
	char *buffer = mallocAndCheck(sizeof(char)*1025);
	memset(buffer, 0, 1025);
	
	// Get the length of our file
	int filelen = strlen(argv[3]);
	
	socklen_t addrlen = (socklen_t)sizeof(struct sockaddr_storage);
	
	// Pick up a socket
	int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(tcpSocket <= 0)
	{
		write(2, "Unable to bind socket!\r\n", 24);
		return FUNCTION_ERROR;
	}
	
	// Struct to hold the port and address of the server
	struct sockaddr_in serveraddr;
	
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	
	// Parse the ipv4 address to the struct
	if(inet_pton(AF_INET, argv[1], &serveraddr.sin_addr) <= 0)
	{
		write(2, "Failed to parse IP Address!\r\n", 29);
		return IMPROPER_ARGUMENTS;
	}
	
	if(connect(tcpSocket, (struct sockaddr *)&serveraddr, addrlen))
	{
		perror("Connect");
		return FUNCTION_ERROR;
	}
	
	if(write(tcpSocket, argv[3], filelen) < 0)
	{
		perror("Write");
		return FUNCTION_ERROR;
	}
	
	// Get our current working directory
	char* cwd = mallocAndCheck(2049);
	getcwd(cwd, 2048);
	#ifdef Debug
		printf("Current Working Directory: %s\r\n", cwd);
	#endif
	strcat(cwd, "/");
	// Append the name of the local file
	strcat(cwd, argv[4]);
	#ifdef Debug
		printf("Local File: %s\r\n", cwd);
	#endif
	
	
	
	int count = 1024;
	
	// Read and write on response
	while( 1 )
	{
		// Busy wait for response
		while((count = read(tcpSocket, buffer, 1024)) == 0)
		{}
		
		if(!strcmp(buffer, "INVALID_REQUEST"))
		{
			printf("Invalid file specified. Please check the arguments.\r\n");
			return IMPROPER_ARGUMENTS;
		}
		
		// Open and/or create the file we're going to be copying to
		int fd = open(cwd, O_WRONLY | O_CREAT, S_IRWXU);
		if(fd <= 0)
		{
			perror("Open");
			return FUNCTION_ERROR;
		}
		
		write(fd, buffer, count);
		
		close(fd);
		if (count != 1024)
			break;
	}
	
	
	return OK;
}
