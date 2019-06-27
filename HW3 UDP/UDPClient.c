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
#define BUFFER 2048
//#define PORT 1137


int main(int argc,char **argv)
{

	//CHECK ARGUMENTS
	if (argc < 3) on_error("Usage: %s [IP Address] [Port]\n", argv[0]); //i
	int port = atoi(argv[2]);
	
	//DECLARE VARIABLES
	char sendline[BUFFER];			
    char recvline[BUFFER];

   	
	int client_fd;		
    struct sockaddr_in serv_addr;	
	struct hostent *hostAddress;
	socklen_t slen=sizeof(serv_addr);
	
	
	
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
    memset(&serv_addr, 0, sizeof serv_addr);
 
 	//SET THE ADDRESS FAMILY, PORT, AND IP 
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(port);
    serv_addr.sin_addr= *((struct in_addr *)hostAddress->h_addr); 


	
    while(!((sendline[0]=='c')&&(sendline[1]=='l')&&(sendline[2]=='o')&&(sendline[3]=='s')&&(sendline[4]=='e'))) 				
    {
    	printf("C: ");
        memset( sendline, 0, BUFFER);		
		memset( recvline, 0, BUFFER);	
 		scanf("%s", sendline);
 		
 		
 		//Send the data
        sendto(client_fd,sendline,BUFFER, 0 ,(struct sockaddr*)&serv_addr, slen);
	//	if (recvfrom(client_fd, recvline, BUFFER, 0, (struct sockaddr*)&serv_addr, &slen)==-1)
    //    	on_error("recvfrom()");			//read it back in
    //	printf("S: %s",recvline);		//and send it to the screen
    }
 	
 
	printf("C: Closing socket\n");
	close(client_fd);
	
	

}
