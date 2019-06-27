#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define BUFLEN 512
#define PORT 1137
 
#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); } //define errors

/*
The purpose of this program is to send the time when the user asks for it. I did this by using the UNIX 'date' command. My program first checks to see if the first letters are "date", 
then runs the command and sends the output.

However, this is vulnerable in that if the client could fit two commands on separate lines in their sendbuffer, my program will run both as long as the first command is "date."

In essence, this makes it about the same vulnerability as the program that simply runs commands on the server and sends back the result. It meets the requirements of the project, but
obviously would never be implemented on a real server.

*/


int main(void)
{
	
	
	//DECLARE BUFFERS
	FILE *fp; 					//The file
	char rcvBuf[BUFLEN];		//The buffer that takes in the info 		(receive buffer)
	char comBuf[BUFLEN];		//The buffer that holds the command result 	(command buffer) 
	char sendBuf[BUFLEN];		//The buffer that actually sends it			(send buffer)
	

	
	//DECLARE VARIABLES
    struct sockaddr_in server, cli_addr;
    int  server_fd, client_fd;
    socklen_t slen=sizeof(cli_addr);
    
    
    
    
    //CREATES THE SOCKET
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    {    on_error("Socket:");}
    else
        printf("S: Socket() successful\n");
    
    
    
    //SETS DATA FOR SOCKET
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    
	
	//BINDS THE SOCKET OR ERRORS.
    if (bind(server_fd, (struct sockaddr* ) &server, sizeof(server))==-1)
    {    on_error("Bind:");}
    else
        printf("S: Bind() successful\n");
    
    while(1)
    {
		//CLEARS THE BUFFERS
		memset( sendBuf, 0, BUFLEN);
		memset( comBuf, 0, BUFLEN);
		memset( rcvBuf, 0, BUFLEN);
    	
		//COLLECTS USER INPUT
		if (recvfrom(server_fd, rcvBuf, BUFLEN, 0, (struct sockaddr*)&cli_addr, &slen)==-1)
            on_error("Recvfrom():");
            
            
        if (rcvBuf[0] == 'd' && rcvBuf[1] == 'a' && rcvBuf[2] == 't' && rcvBuf[3] == 'e')
		{
			//RUNS COMMAND (and stores result in fp file)
	        fp=popen(rcvBuf, "r");
	        
			//COPIES EVERY LINE FROM THE FP FILE INTO THE COMMAND BUFFER
			while (fgets(comBuf, sizeof fp, fp) != NULL)
			{	
				//printf("%s", comBuf);
				strcat(sendBuf, comBuf);
	        }
	        //CLOSES FILE 
        	pclose(fp); 
	        
   		}
   		else
   		{strcpy(sendBuf, "Sorry, unacceptable command. Please use the date command\n");}
      
        //SENDS THE RESPONSE
        if (sendto(server_fd, sendBuf, BUFLEN, 0, (struct sockaddr*)&cli_addr, slen) ==-1)
        	on_error("Sendto():");
        	
		 
    	
       
     
    }
    
    
    close(server_fd);
    return 0;
}
