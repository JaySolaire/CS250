
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); }

int main(int argc,char **argv)
{
	if (argc < 2) on_error("Usage: %s[ip address] [port]\n", argv[0]);
	
	//DECLARE VARIABLES
	int sockfd,n;
	char sendline[100];
	char recvline[100];
	struct sockaddr_in servaddr;
	
	//int ip = atoi(argv[0]);  
	int port = atoi(argv[1]);
	sockfd=socket(AF_INET,SOCK_STREAM,0); 	//socket file descriptor creator. 
	bzero(&servaddr,sizeof servaddr);		//Zero out server address

	servaddr.sin_family=AF_INET;			//set protocols
	servaddr.sin_port=htons(port);			//set port number (host to network short)

	inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr));	//connect to server ip self.

	//MAKE THE CONNECTION	
	connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

	while(1)
	{
		bzero( sendline, 100);		
		bzero( recvline, 100);
        fgets(sendline,100,stdin); /*stdin = 0 , for standard input */

        write(sockfd,sendline,strlen(sendline)+1);
        read(sockfd,recvline,100);
        printf("%s",recvline);
	}

}

