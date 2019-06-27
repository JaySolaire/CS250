#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

int main()
{
	
	int fd =0;
	char buf[10];
	int readreturn;
	
	fd_set readfds;
	struct timeval timeout;

	FD_ZERO( &readfds);
	FD_SET (fd, &readfds);
	timeout.tv_sec= 1;
	timeout.tv_usec=0;
	
	while(1)
	{
		
	
		
		readreturn = select(3, &readfds, NULL, NULL, &timeout);
		
		if (readreturn!=0)
		{
			memset(buf, 0, 10);
			readreturn = read(fd, buf, 9);
			printf("readreturn = %d\n", readreturn);
			
			if (readreturn != -1)
			{
				printf(" buf = %s\n", buf);
			}
		}
	}
	return 0;
}

