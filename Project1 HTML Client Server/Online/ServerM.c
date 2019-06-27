#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    
	
	int server_fd, client_fd, portno;
    socklen_t clilen;
    char buffer[256];
    
	struct sockaddr_in serv_addr, cli_addr;
    
	int n;
    
	if (argc < 2) {
         fprintf(stderr, "ERROR, no port provided\n");
         exit(1);
    
	}
    
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
	if (server_fd < 0)
         error("ERROR opening socket");
    
	bzero((char *) &serv_addr, sizeof(serv_addr));
    
	portno = atoi(argv[1]);
    
	serv_addr.sin_family = AF_INET;
    
	serv_addr.sin_addr.s_addr = INADDR_ANY;
    
	serv_addr.sin_port = htons(portno);
    
	if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
        listen(server_fd, 5);
    
	clilen = sizeof(cli_addr);
    //Below code is modified to handle multiple clients using fork
    //------------------------------------------------------------------
    int pid;
    while (1) {
         client_fd = accept(server_fd, (struct sockaddr *) &cli_addr, &clilen);
         if (client_fd < 0)
                error("ERROR on accept");
         //fork new process
         pid = fork();
         if (pid < 0) {
              error("ERROR in new process creation");
         }
         if (pid == 0) {
            //child process
            close(server_fd);
            //do whatever you want
            bzero(buffer, 256);
            n = read(client_fd, buffer, 255);
            if (n < 0)
                error("ERROR reading from socket");
            printf("Here is the message: %s\n", buffer);
            n = write(client_fd, "I got your message", 18);
            if (n < 0)
                 error("ERROR writing to socket");
            close(client_fd);
          } else {
             //parent process
             close(client_fd);
          }
           }
    //-------------------------------------------------------------------
   return 0;
}
