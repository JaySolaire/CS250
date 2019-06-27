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
 
 
/*Sources
1. http://pubs.opengroup.org/onlinepubs/9699919799/
2. https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/
3. http://www.tenouk.com/Module40c.html

*/

int main(int argc,char **argv)
{
	//CHECK ARGUMENTS
	if (argc < 3) on_error("Usage: %s [IP Address] [Port]\n", argv[0]); //if the user didn't enter a port and an ip
	
	//DECLARE VARIABLES
    int sockfd;						//the socket file descriptor
    char sendline[100];				//array of chars because no strings
    char recvline[100];
    int port = atoi(argv[2]);		//converts the port argument from 'string' to integer
    struct sockaddr_in servaddr;	//(Ask why this struct is named server and not client )
	struct hostent *hostAddress;	//code from [3]. Allows the user to input a host ip or name.
	
	//COLLECT HOSTNAME OR THROW AN ERROR
    if((hostAddress=gethostbyname(argv[1])) == NULL)
	{   on_error("C: Unable to set host.");					}
	else 
	{	printf("C: The remote host is: %s\n", argv[1]);	}	
	
	//CREATE SOCKET (with file descriptor) OR THROW AN ERROR
    sockfd=socket(AF_INET,SOCK_STREAM,0);
	if (sockfd<0)
	{	on_error("C: Could not create socket\n");				}
	printf("C: Socket created successfully\n");
	
	//EMPTY THE SERVER ADDDRESS STRUCT
    memset(&servaddr, 0, sizeof servaddr);
 
 	//SET THE ADDRESS FAMILY, PORT, AND IP 
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(port);
    servaddr.sin_addr= *((struct in_addr *)hostAddress->h_addr); //sets the ip. Code from [3]

  
 	//MAKE THE CONNECTION, RETURN ERROR IF FAIL
    if (connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))==-1)
	{	on_error("C: Could not connect\n");}
	printf("C: Connected successfully\n");
 
 	
    while(1)						//format based off of [2]. Replaced bzero with memset bc bzero is legacy
    {
    	printf("C: ");
        memset( sendline, 0, 100);		//Clear the sendline (all 100 chars)
        memset( recvline, 0, 100);		//clear the receive line
        fgets ( sendline, 100, stdin); 	//takes info and sends it to sendline (STDIN represents 0 for standard input, but replacing it with 0 will throw an error)
        
 
        write(sockfd,sendline,strlen(sendline)+1); //add one to include NULL value
        read(sockfd,recvline,100);		//read it back in
        printf("S: %s",recvline);		//and send it to the screen
    }
 
 
	printf("C: Closing socket\n");	//useful if the while loop is removed. See below.
	close(sockfd);
	
	//If the server is closed, the client will automatically close after ~2 iterations of the while loop
	//Idk why it closes. Idk why it never continues onto the printf or the close() function.
	//I replaced while(1) with while(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!=-1) to..
	//..see if perhaps while (the connection is not failing) might be a better loop, but it had the same result
	//when i removed the loop, it sent and received a single message before closing the socket, as expected.
	//TESTING BY CLOSING THE SERVER AFTER CONNECTION IS MADE:
		//If I remove the read statement, client still closes after two iterations of the while loop.
		//If I remove the write statement, then it obv won't send or recieve, but it won't close after the server closes
	//This means that the write function is what ends the program if server is closed. Idk how or why. Will ask in class.
	//The good news is, the socket still closes automatically on kill, even though my program does not manually do it. 
	
}
	/*
	notes:
	cant use string, has to use array of chars
	socket: socket, bind, connect, listen, write, read, accept, close
	socket, write, read, close, connect, fgets, fputs< client
	write then read
	listen, bind, accept, close< server
	read then write

	int socket(int domain, int type, int protocol);    //address family, type, protocol, returns file descriptor [AF_INET, SOCK_STREAM, 0]
	
	int bind(int socket, const struct sockaddr *address, socklen_t address_len);	//socket, ip address, address length
	
	int connect(int socket, const struct sockaddr *address,socklen_t address_len); 	//same as bind
	
	int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);	
       
	int listen(int socket, int backlog); 

	int atoi(const char *str)   //string to integer
	
	void bzero(void *s, size_t n);		//zeros out the location *s with n zeros.
*/

