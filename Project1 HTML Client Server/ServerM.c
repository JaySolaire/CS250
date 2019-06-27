#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <unistd.h>
#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); } //define errors
#define BUFFER_SIZE 1024

int main(int argc,char **argv)
{
	//CHECK ARGUMENTS
	if (argc < 2) on_error("Usage: %s [Port]\n", argv[0]); //if the user didn't enter a port 
		
	
	//CREATING FILES
	//FILE *htmlPage;									//File to display on webpage and at start of terminal
	//htmlPage = fopen("Welcome.html", "r");
	FILE *fp;										//File to hold the html that the user retrieves
	
	//CREATING FORK ID
	int pid;
	
	//CREATING BUFFERS 
	//char fileData[BUFFER_SIZE];
	//fgets(fileData, BUFFER_SIZE, htmlPage);

	char sendline[BUFFER_SIZE];
//	strcat(sendline, fileData);
	char recvline[BUFFER_SIZE];
	char commline[BUFFER_SIZE];
	
	//SETTING PORT		
	int port = atoi(argv[1]);		//converts the port argument from 'string' to integer
	
	//CREATE A SOCKET
	int server_fd = socket(AF_INET,SOCK_STREAM,0);
	if (server_fd<0)
	{	on_error("C: Could not create socket\n");}				
	else
	{	printf("C: Socket created successfully\n");}
	
	//DEFINE THE ADDRESS
	struct sockaddr_in server;
	server.sin_family = AF_INET;					//connects on IPV4
    server.sin_port = htons(port);					//connects on the specified port 
    server.sin_addr.s_addr = htonl(INADDR_ANY);		//connects on any address
	
	bind(server_fd, (struct sockaddr *) &server, sizeof(server));
	listen(server_fd, 5);
	int client_fd;
	while (1)
	{

		client_fd = accept(server_fd, NULL, NULL);
		{
			pid = fork();
			
			//FAILED
	        if (pid < 0) 												
			{
	            on_error("ERROR in new process creation");
	        }
	        
	        //CHILD
	        if (pid == 0) 											
			{
				int recvVal=5;
	        	//close(server_fd);
				while (recvVal > 0)
				{
					memset(recvline, 0, BUFFER_SIZE);								//clears the recieve line 
					recvVal = recv(client_fd, &recvline, sizeof(recvline), 0);		//recieves data on that line
					printf("%d\n", recvVal);
					fp = fopen(recvline, "r");										//opens file, stores data (or null) in fp
				
					if (fp == NULL)
					{	
					perror("");
					strncpy(sendline, "Error! File does not exist. \n", BUFFER_SIZE);
					}
					else 
					{	
						while (fgets(commline, BUFFER_SIZE, fp) != NULL)			//for each line in the file fp, stores in commline
						{
							strcat(sendline, commline);								//appends each line of commline to the end of sendline
						}
					}
					send(client_fd,  sendline, sizeof(sendline), 0);				//sends the file
					memset(sendline, 0, 2048);										//Then clears the sendline
				
				
				}
				fclose(fp);
				close (client_fd);
			}
			//PARENT
			else
			{
				close (client_fd);
			}
		

		}
		
	}

	return 0;
}
		
		
