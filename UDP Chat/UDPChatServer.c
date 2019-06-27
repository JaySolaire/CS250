#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define BUFLEN 2048
#define PORT 1137

#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); } //define errors

/*
This is the server.
*/


int main(void)
{
	
	//MAKING IT A DAEMON
	chdir("/");
	
	fork();
	
	close(0);
	close(1);
	close(2);
	open("/dev/null", O_RDWR);
	dup(0);
	dup(0);

	setsid();
	
	
	
	char sendBuf[BUFLEN];
	char recvBuf[BUFLEN];
	
	struct sockaddr_in serv_addr, cli_addr;
	int client_fd, serv_fd, serv_master;
	socklen_t slen = sizeof(cli_addr);
	
	
	
	
	//CREATE THE SOCKET
	if ((serv_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == 1)
	{	on_error("Socket");					}
	else
	{	printf("Socket Successful. \n");	}
	
	int opt = 1;
	//ALLOW MULTIPLE CONNECTIONS	
	if( setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )  
    {  
        on_error("setsockopt");  
    }  	
		
		
	//SET THE DATA FOR THE SOCKET
	bzero (&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	
	
	
	//BIND THE SOCKET
	
	    
    if (bind(serv_fd, (struct sockaddr* ) &serv_addr, sizeof(serv_addr))==-1)
        err("bind");
        
	//SELECT ============================
	fd_set read_fds;			
	FD_ZERO(&serv_master);			//clears it
	FD_SET(listening, serv_master); 	//need to define listening. 
	
	
	while (1)
	
	{
		
		
		fd_set copy = serv_master;		//so the copy is overwritten, and the master(serv_fd) holds everything
		
		int socketCount = select(0, %copy, NULL, NULL, NULL);
		
	/*	for(int i =0; i<socketCount; i++)
		{
			
			//This part I couldn't figure out without tcp. I spent so much time with select() on the client, that I didn't have enough time to explore multiple udp chat connections for the server.
			
			//if the socket is listening
				//add it to the array copy.fd_set
				//send a message to ensure connection
			
			//if each socket is not listening
				//recieve it
				//recvfrom(?????, recvBuf, BUFLEN, 0, (struct sockaddr*)&cli_addr, &slen)
				
				/*send it to every other client. Something like this...
				for (int i; i<socketCount; i++)															//for each client
					if (copy.fd_set != ???)																//if that client is not the one that just sent a message
						sendto(copy.fd_set,recvBuf,BUFFER, 0 ,(struct sockaddr*)&serv_addr, slen);		//send the message to that client.
						
				
		
			
		} */
		
		
	}
	
	return 0;
}
		
		
