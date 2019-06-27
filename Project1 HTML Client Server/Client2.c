#include <stdio.h>
#include <stdlib.h>		//exit
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>		//gethostbyname
#include <stdio.h>
#include <string.h>
#include <unistd.h>		//read/write
#include <arpa/inet.h>	//inet pton
 
#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); } //define errors
 
 

int main(int argc,char **argv)
{
	//CHECK ARGUMENTS
	if (argc < 3) on_error("Usage: %s [IP Address] [Port]\n", argv[0]); //if the user didn't enter a port and an ip
	
	//DECLARE VARIABLES
    int client_fd;					//the socket file descriptor
    char sendline[1024];			//array of chars because no strings
    char recvline[1024];
    int port = atoi(argv[2]);		//converts the port argument from 'string' to integer
    struct sockaddr_in server;	
	struct hostent *hostAddress;	//code from [3]. Allows the user to input a host ip or name.
	
	//COLLECT HOSTNAME OR THROW AN ERROR
    if((hostAddress=gethostbyname(argv[1])) == NULL)
	{   on_error("C: Unable to set host.");					}
	else 
	{	printf("C: The remote host is: %s\n", argv[1]);	}	
	
	//CREATE SOCKET (with file descriptor) OR THROW AN ERROR
    client_fd=socket(AF_INET,SOCK_STREAM,0);
	if (client_fd<0)
	{	on_error("C: Could not create socket\n");				}
	printf("C: Socket created successfully\n");
	
	//EMPTY THE SERVER ADDDRESS STRUCT
    memset(&server, 0, sizeof server);
 
 	//SET THE ADDRESS FAMILY, PORT, AND IP 
    server.sin_family=AF_INET;
    server.sin_port=htons(port);
    server.sin_addr= *((struct in_addr *)hostAddress->h_addr); //sets the ip to what the user entered. Code from [3]

  
 	//MAKE THE CONNECTION, RETURN ERROR IF FAIL
    if (connect(client_fd,(struct sockaddr *)&server,sizeof(server))==-1)
	{	on_error("C: Could not connect\n");}
	printf("C: Connected successfully\n");
 	printf("C: Type 'close' to exit.\n");
 	
 	//SETUP FOR CONNECTION ERRORS
 	int error = 0;
	socklen_t len = sizeof (error);
 	
 	
 	
 	
    while(!((sendline[0]=='c')&&(sendline[1]=='l')&&(sendline[2]=='o')&&(sendline[3]=='s')&&(sendline[4]=='e'))) //rather proud of this solution				
    {
    	printf("C: ");
        memset( sendline, 0, 1024);		//Clear the sendline (all 100 chars)
        memset( recvline, 0, 1024);		//clear the receive line
        //fgets ( sendline, 100, stdin); 	//takes info and sends it to sendline (STDIN represents 0 for standard input, but replacing it with 0 will throw an error)
        scanf("%s", sendline);
 
        write(client_fd,sendline, 1024); 
        read(client_fd,recvline,1024);	//read it back in
        printf("S: %s",recvline);		//and send it to the screen
		
		/*
		INSERT CONNECTION STATUS TEST
		*/	
		
	}
 
 
	printf("C: Closing socket\n");
	close(client_fd);
	
	

}
	
