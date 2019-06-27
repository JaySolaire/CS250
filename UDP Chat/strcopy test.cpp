#include <stdio.h>
#include <stdlib.h>		//exit
#include <sys/types.h>


#include <stdio.h>
#include <string.h>

 
#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); } //define errors
#define BUFFER 2048
#define PORT 1137

int main()
{
	
	char string1[] = "string1";
	char string2[] = "string2";
	
	strcat(string1, string2);
	printf("%s", string1);
}
