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
#include <unistd.h>  	//for sleep


#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); } //define errors

int daemonic(void)			//MAKING IT A DAEMON
{

	chdir("/");
	
	pid_t process_id;
	process_id=fork();
	if (process_id < 0)
		exit(EXIT_FAILURE);
	if (process_id > 0)
		exit(EXIT_SUCCESS);
	if (setsid() < 0)
		exit(EXIT_FAILURE);
	
	
	//close standard in, standard out, and standard error, and redirect to /dev/null
	close(0);
	close(1);
	close(2);
	open("/dev/null", O_RDWR);
	
	dup(0);
	dup(0);

	setsid();
	
	return 0;
}









int main(int argc, char *argv[])
{
	//CHECK ARGUMENTS
	if (argc < 2) on_error("Usage: %s [Port]\n", argv[0]); 
	int port = atoi(argv[1]);
	
	if (daemonic() < 0)
		on_error("Deamon");
		
	char sendBuf[BUFLEN];
	char recvBuf[BUFLEN];
	
	struct sockaddr_in serv_addr, cli_addr;

	int client_fd, serv_fd;// serv_master;  //OPTION #1
	socklen_t slen = sizeof(cli_addr);

	
	/*
	OPTION #2 ===========================
	
	
	struct sockaddr_in cli_addrss[6];
	int maxClient=0;
	int test=0;
	socklen_t slenss[6];
	
	OPTION #1 ===========================	
	fd_set read_fds;			
	FD_ZERO(&serv_master);			//clears it
	FD_SET(listening, serv_master); 	//need to define listening. 
	*/
		
	//CREATE THE SOCKET
	if ((serv_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == 1)
	{	on_error("Socket");					}
	else
	{	printf("Socket Successful. \n");	}
	
	
	//ALLOW MULTIPLE CONNECTIONS
	int opt = 1;	
	if( setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )  
    {  
        on_error("setsockopt");  
    }  	
	
	
		//SET THE DATA FOR THE SOCKET
	bzero (&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	
	
	
	//BIND THE SOCKET
    if (bind(serv_fd, (struct sockaddr* ) &serv_addr, sizeof(serv_addr))==-1)
        on_error("bind");
    
    printf("Bind successful\n");
	
	
	while (1)
	
	{
		memset(recvBuf, 0, BUFLEN);
        if (recvfrom(serv_fd, recvBuf, BUFLEN, 0, (struct sockaddr*)&cli_addr, &slen)==-1)
           on_error("recvfrom()");
        
        /*
		OPTION #2 ===========================
	
		//ADD THE ADDRESS TO THE CLIENT ARRAY
		test = 0;
        for (int i=0; i<maxClient;i++)
	    {	if (cli_addrss[i] == cli_addr)			//This line was the error. Apparently you can't just compare structs..
	        	{test=1;}				//that address is in the array somewhere
		}
		if (test==0) 					//that address isn't in the array, so add it.
		{	cli_addrss[maxClient] = cli_addr;
			slenss[maxClient] = sizeof cli_addr;
			maxClient+=1; 
		}
		OPTION #1 ===========================
		
		fd_set copy = serv_master;		//so the copy is overwritten, and the master(serv_fd) holds everything
		
		int socketCount = select(0, %copy, NULL, NULL, NULL);
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
					
				
				====================================*/
			
        	
        
        printf("Received packet from %s:%d\nData: %s\n\n",
               inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), recvBuf);
       
	   
	   /* THIS TESTED WHETHER THE CLIENT RECV FUNCTION WAS WORKING BY SENDING BACK ANYTHING THAT WAS RECEIVED 20 TIMES
	   
	    for (int i=0; i<20; i++)
        {sendto(serv_fd, recvBuf, BUFLEN, 0, (struct sockaddr*)&cli_addr, slen);
		printf("%d\t", i);
		}
		*/
		
		
	}
}



