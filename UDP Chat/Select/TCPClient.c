//My normal tcp echo client, converted to work with theTCP server.

#include <stdio.h>
#include <stdlib.h>		//exit
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>		//gethostbyname
#include <unistd.h>		//read/write
#include <arpa/inet.h>	//inet pton
 
#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); } //define errors
 
 
/*References
1. http://pubs.opengroup.org/onlinepubs/9699919799/
2. https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/
3. http://www.tenouk.com/Module40c.html
Also worked with Josh Joseph on ending the while loop. Couldn't get EOF test to work, instead tested for specific word.
*/

int main(int argc,char **argv)
{
	//CHECK ARGUMENTS
	if (argc < 3) on_error("Usage: %s [IP Address] [Port]\n", argv[0]); //if the user didn't enter a port and an ip
	
	//DECLARE VARIABLES
    int client_fd;					//the socket file descriptor
    char sendline[1000];				//array of chars because no strings
    char recvline[1000];
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
 	
 	//original while loop based off of [2]. Replaced bzero with memset bc bzero is legacy
    while(!((sendline[0]=='c')&&(sendline[1]=='l')&&(sendline[2]=='o')&&(sendline[3]=='s')&&(sendline[4]=='e'))) //rather proud of this solution				
    {
    	
        memset( sendline, 0, 100);		//Clear the sendline (all 100 chars)
        memset( recvline, 0, 100);		//clear the receive line
        
		//RECV
 		read(client_fd,recvline,100);	//read it back in
        //system( recvline);
		printf("S: %s",recvline);		//and send it to the screen
		
		//SEND
		printf("C: ");
		fgets ( sendline, 100, stdin); 	//takes info and sends it to sendline (STDIN represents 0 for standard input, but replacing it with 0 will throw an error)
        write(client_fd,sendline,strlen(sendline)+1); //add one to include NULL value
       
    }
 
 
	printf("C: Closing socket\n");
	close(client_fd);
	
	

}
	/*
	NOTES
	If the server is closed (like ctrl-C), the client will automatically close after ~2 iterations of the while loop
	Idk why the client closes because of this. It will close no matter what, whether while(1) or while("close" not typed) or while(connect()!=1)
	It never continues onto the printf or the close() function in this case.
	
	I replaced while(1) with while(connect(client_fd,(struct sockaddr *)&server,sizeof(server))!=-1) to..
	..see if perhaps while (the connection is not failing) might be a better loop, but it had the same result- **later told because connect only happens once
	When i removed the loop, it sent and received a single message before closing the socket, as expected.
	
	TESTING BY CLOSING THE SERVER AFTER CONNECTION IS MADE:
		If I remove the read statement, client still closes after two iterations of the while loop.
		If I remove the write statement, then it obv won't send or recieve, but it won't close after the server closes
	This means that the write function is what ends the program if server is closed. Idk how or why. Will ask in class.
	The good news is, the socket still closes automatically on kill, even though my program does not manually do it- **now does by checking for 'close'
	*/
	
	/*
	FUNCTION REFERENCES
	cant use string, has to be array of chars
	socket: socket, bind, connect, listen, write, read, accept, close
	client functions: socket, write, read, close, connect, fgets, fputs
	write then read
	server functions: listen, bind, accept, close
	read then write

	int socket(int domain, int type, int protocol);    //address family, type, protocol, returns file descriptor [AF_INET, SOCK_STREAM, 0]
	
	int bind(int socket, const struct sockaddr *address, socklen_t address_len);	//socket, ip address, address length
	
	int connect(int socket, const struct sockaddr *address,socklen_t address_len); 	//same args as bind
	
	int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);	
       
	int listen(int socket, int backlog); 

	int atoi(const char *str)   //string to integer
	
	void bzero(void *s, size_t n);		//zeros out the location *s with n zeros.
*/

