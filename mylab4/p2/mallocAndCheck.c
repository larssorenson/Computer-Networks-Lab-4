#include "mytalk.h"

// Generic check for malloc failure
// WHICH CAN HAPPEN PEOPLE
void* mallocAndCheck(size_t size)
{
	void* res = malloc(size);
	if (!res)
	{
		perror("Malloc Failed");
		exit(1);
	}
	
	return res;
	
}
