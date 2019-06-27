#include <stdio.h>
#include <stdlib.h>		//exit
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>		//gethostbyname
#include <stdlib.h>
#include <string.h>
#include <unistd.h>		//read/write
#include <arpa/inet.h>	//inet pton
 
#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); } //define errors
#define BUFFER 512


int main(int argc,char **argv)
{
	//CHECK ARGUMENTS
	if (argc < 3) on_error("Usage: %s [IP Address] [Port]\n", argv[0]); //if the user didn't enter a port and an ip
	
	//DECLARE VARIABLES
    int client_fd;					//the socket file descriptor
    char sendline[BUFFER];				//array of chars because no strings
    char recvline[BUFFER];
    int port = atoi(argv[2]);		//converts the port argument from 'string' to integer
    struct sockaddr_in server;	
	struct hostent *hostAddress;	//code from [3]. Allows the user to input a host ip or name.
	socklen_t slen=sizeof(server);
	
	//COLLECT HOSTNAME OR THROW AN ERROR
    if((hostAddress=gethostbyname(argv[1])) == NULL)
	{   on_error("C: Unable to set host.");					}
	else 
	{	printf("C: The remote host is: %s\n", argv[1]);	}	
	
	//CREATE SOCKET (with file descriptor) OR THROW AN ERROR
    client_fd=socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP); //or af_inet, sock_dgram, 0
	if (client_fd<0)
	{	on_error("C: Could not create socket\n");				}
	printf("C: Socket created successfully\n");
	
 	//EMPTY THE SERVER ADDDRESS STRUCT
    memset(&server, 0, sizeof server);
 
 	//SET THE ADDRESS FAMILY, PORT, AND IP 
    server.sin_family=AF_INET;
    server.sin_port=htons(port);
    server.sin_addr= *((struct in_addr *)hostAddress->h_addr); //sets the ip to what the user entered. Code from [3]

	int counter = 0;
 	//original while loop based off of [2]. Replaced bzero with memset bc bzero is legacy
    while(counter <1000000) 			
    {
    
        memset( sendline, 0, 100);		//Clear the sendline (all 100 chars)
    	snprintf(sendline, BUFFER, "%d", counter );
        sendto(client_fd,sendline,BUFFER, 0 ,(struct sockaddr*)&server, slen);
		counter++;
    }
 
 
	printf("C: Closing socket\n");
	close(client_fd);
	
	

}
