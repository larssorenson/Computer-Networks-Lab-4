#include "mytalk.h"

// Generic check for malloc failure
// WHICH CAN HAPPEN PEOPLE
void* mallocAndCheck(size_t size)
{
	void* res = malloc(size);
	if (!res)
	{
		perror("Malloc Failed");
		tcsetattr(0, TCSANOW, &term1);	// return terminal to default mode
		exit(1);
	}
	
	return res;
	
}
