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


//YOOO I JUST FIGURED OUT THAT I CAN USE SELECT TO TIMEOUT FOR THE CLIENT SEND

int main(int argc,char **argv)
{
	//CHECK ARGUMENTS
	if (argc < 3) on_error("Usage: %s [IP Address] [Port]\n", argv[0]); //i
	int port = atoi(argv[2]);
	
	
	//DECLARE VARIABLES
	char sendline[BUFFER];			
    char recvline[BUFFER];
    char userdata[BUFFER];
   	char name[BUFFER];
   	char date[BUFFER];
   	FILE *fp;
   	
   	//DECLARE SOCKET VARIABLES
	int client_fd;		
    struct sockaddr_in serv_addr;	
	struct hostent *hostAddress;
	socklen_t slen=sizeof(serv_addr);
	
	
   	
   	//SPECIFIC TO SELECT
   	int fd =0;
	char buf[10];
	int selRet;
	fd_set readfds;
	fd_set client_socket;
	struct timeval timeout;
   		
	
	//SELECT ADD TO SET
	FD_ZERO( &readfds);
	FD_ZERO( &client_socket);

	FD_SET (fd, &readfds);
	FD_SET (client_fd, &client_socket);
	FD_SET (client_fd, &readfds);
	

	

	
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

	//GET USER NAME
	printf("Enter a name for the chat: ");
	scanf("%s", name);
	strcat(name, ": ");
	
	timeout.tv_sec= 10;
	timeout.tv_usec=0;
	//CHAT 
    while(!((sendline[0]=='c')&&(sendline[1]=='l')&&(sendline[2]=='o')&&(sendline[3]=='s')&&(sendline[4]=='e'))) 				
    {
  
  		//CLEAR THE BUFFERS
        memset( sendline, 0, BUFFER);		
		memset( recvline, 0, BUFFER);
		memset( userdata, 0, BUFFER);	
		memset( date, 0, BUFFER);
		
		
	  	
		selRet = select(6, &readfds, NULL, NULL, &timeout);
		
		if (selRet==0)								//TIMOUT
		{
			printf("timeout\n");
			timeout.tv_sec= 10;		//reset the timeout?
			timeout.tv_usec=0;
			//	I tried placing these two lines in a couple different places. If I placed them before the while(1), then it would work for ten seconds then spam TIMEOUT!
			//	If I placed them within the loop, then it would timeout ever second or so. I ended up placing them here, which caused it to timeout only when the countdown
			//	ended, but it still won't go to any of the else-if statements after ten seconds.
			
		}
		else if (selRet ==-1)						//ERROR
		{
			printf("error\n");
		}
		else										//NOT TIMED OUT, NOT ERROR, TRY TO SEND OR RECIEVE
		{
			if (FD_ISSET(client_fd, &readfds)!=0)	//RECIEVE<<< except this line doesn't really work.
			{
				printf("RECIEVING");
				FD_CLR(client_fd, &readfds);
				if (recvfrom(client_fd, recvline, BUFFER, 0, (struct sockaddr*)&serv_addr, &slen)==-1)
	        		on_error("recvfrom()");			//read it back in
	    		printf("S: %s",recvline);		//and send it to the screen
				
			}
			else 								//SENDING>>>
			{
				read(fd, userdata, 9);			//read in the user input
				
				strcat(sendline, name);			//add the name to sendline
		 		strcat(sendline, userdata);		//add the user input
		 		
		 		//Get the Date and append it.
		 		fp=popen("date", "r");			
				while (fgets(date, sizeof fp, fp) != NULL)
				{
					strcat(sendline, date);
				}
		 		pclose(fp);
				
				//and send it.
				if (sendto(client_fd,sendline,BUFFER, 0 ,(struct sockaddr*)&serv_addr, slen) < 0)
				{	on_error("sendto\n");}
				else
					printf("\t\tSent!\n");
				
			}
		}
 	
	}
	printf("C: Closing socket\n");
	close(client_fd);
}
