#include "mytalk.h"

int numberFromString(char* str)
{
	// If we have a null pointer, return 0. Behaviour is undefined so what do you expect?
	if(!str)
		return INSUFFICIENT_ARGUMENTS;
	
	// While we still have characters to parse, and we haven't exceeded the size of the integer
	int x = 0;
	int result = 0;
	while(str[x] != '\0' && x < 12)
	{
		#ifdef Debug
			printf("%02x %c %d ", str[x], str[x], str[x]);
			printf("\r\n");
		#endif
		// Convert from character to number and add result
		if(str[x] < 58 && str[x] > 47)
		{
			result += ((int)(str[x]))-48;
			// Move digits forward one space for the next digit
			result *= 10;
		}
		
		x++;
		
	}
	// End case we have a 0 on the end, which we will remove
	result /= 10;
	
	return result;
}
