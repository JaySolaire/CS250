#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h> //gethostbyname
  
#define BUFFER_SIZE 1024
#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); }

int main (int argc, char *argv[]) 
{
	//Throw error if not enough arguments 
    if (argc < 3) on_error("Usage: %s [IP Address] [Port]\n", argv[0]); //if the user didn't enter a port and an ip
    
    //Define the Variables.
    int server_fd, client_fd, err;  		//file descriptor 
    struct sockaddr_in server, client;		//creates the server and client
    int port = atoi(argv[2]);				//set the port to the first argument
    char sendline[100];
	char recvline[100];
	//char buf[BUFFER_SIZE];
    struct hostent *hostAddress;
    
    
    //Collect the host address or throw an error.
    if((hostAddress=gethostbyname(argv[1])) == NULL)
	{
	    on_error("Unable to set host.");
	}
	else 
	    printf("Client-The remote host is: %s\n", argv[1]);
	    
    //MAKE THE CONNECTION	CHECK THIS LINE OUT LATER MAY BE WRONG ARGUMENTs
	if ( (connect(server_fd,(struct sockaddr *)&server,sizeof(server)))==0)
	{
		printf("Connection success.");
	} 
	else
	{
	on_error("Connection Fail");
	}
    
    
    //Assign Values to the server struct
    server.sin_family=AF_INET;			//set protocols 
	server.sin_port=htons(port);		//set port
    
    
    
    return 0;
    
}
    /*
    stdin fge ts() //take input from the user (command line?)
	write function- sends it
		server- takes it in and writes it back out see the server 
	then we read it back in using the same read function
	and print it out. stdout fputs
	*/
	
	
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

	int atoi(const char *str)   string to integer
	
	void bzero(void *s, size_t n);		//zeros out the location *s with n zeros.

http://pubs.opengroup.org/onlinepubs/9699919799/
https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/

	*/

