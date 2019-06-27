#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024


#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); }



/*
popen
http://www.cs.uleth.ca/~holzmann/C/system/shell_commands.html   (popen)
http://pubs.opengroup.org/onlinepubs/009696699/functions/popen.html
https://groups.google.com/forum/#!topic/comp.unix.programmer/AZ22u2EvKJk
http://www.gnu.org/software/libc/manual/html_node/Pipe-to-a-Subprocess.html
https://stackoverflow.com/questions/35443876/executing-commands-via-sockets-with-popen


Running a linux command through socket
https://www.unix.com/programming/88529-how-run-linux-command-redirect-its-output-socket-c.html
https://stackoverflow.com/questions/42989315/how-to-write-multiple-lines-over-a-socket-in-c

misc
http://pubs.opengroup.org/onlinepubs/009696899/functions/send.html
https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm
https://www.tutorialspoint.com/c_standard_library/c_function_system.htm
www.gnu.org/software/libc/manual/html_node/Pipe-to-a-Subprocess.html




*/
int main (int argc, char *argv[]) {
    
      
    
	FILE *fp; 						//whatever fp stands for
	char rcvBuf[BUFFER_SIZE];		//The buffer that takes in the info 		(receive buffer)
	char comBuf[BUFFER_SIZE];		//The buffer that holds the command result 	(command buffer) 
	char sendBuf[BUFFER_SIZE];		//The buffer that actually sends it			(send buffer)
	

	 
    //int port = atoi("23");			//argument to integer is unnecessary, leaving code for future changes.
    int port = 2300;
    
    
    int server_fd, client_fd, err;
    struct sockaddr_in server, client;
    
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) on_error("Could not create socket. Try using 'sudo' when running the program on port 23.\n");
    
    server.sin_family = AF_INET;					//connects on IPV4
    server.sin_port = htons(port);					//connects on the specified port (23)
    server.sin_addr.s_addr = htonl(INADDR_ANY);		//connects on any address
    
    
    //
    int opt_val = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);
    
    
    //LISTENS ON BINDS SOCKET 	(-1 return for error, throws error)
    err = bind(server_fd, (struct sockaddr *) &server, sizeof(server));
    if (err < 0) on_error("Could not bind socket\n");
    
    //LISTENS ON SOCKET			(-1 return for error, throws error)
    err = listen(server_fd, 128);
    if (err < 0) on_error("Could not listen on socket\n");
    
    printf("Server is listening on %d\n", port);
    
    
    
    while (1) 
	{
		//ACCEPT CONNECTIONS
        socklen_t client_len = sizeof(client);
        client_fd = accept(server_fd, (struct sockaddr *) &client, &client_len);
        
        if (client_fd < 0) on_error("Could not establish new connection\n");
        
        //FOR EACH CONNECTION/FOREVER
        while (1) 
		{
			
			//CLEARS THE BUFFERS
			memset( sendBuf, 0, 1024);
			memset( comBuf, 0, 1024);
			memset( rcvBuf, 0, 1024);
			
			
			//READS USER INPUT (into buf)
            int read = recv(client_fd, rcvBuf, BUFFER_SIZE, 0);
            if (!read) break; // done reading
            if (read < 0) on_error("Client read failed\n");
                       
            
            //RUNS COMMAND (and stores result in fp file)
            fp=popen(rcvBuf, "r");
			
			
			//COPIES EVERY LINE FROM THE FP FILE INTO THE COMMAND BUFFER
			while (fgets(comBuf, sizeof fp, fp) != NULL)
			{	
			
				//prints the current line on server side 
				printf("%s", comBuf);
				//and appends the current line to the send buffer
				strcat(sendBuf, comBuf);
            }
            
			//printf("%s", sendBuf);
            
            //SENDS THE DATA ON THE SOCKET
        	err= send(client_fd, sendBuf, sizeof sendBuf, 0);
            if (err < 0) {on_error("Client write failed\n");}
           	printf("Bytes of data sent is  %d", err);
           	
           	//CLOSES FILE 
           	pclose(fp);
           	
	        
	    }
    }
    
    
    return 0;
}





		/*
		while true
		read in
		throw error if error
		
		open the file pointer
		
		while the bufferr is not null and test is not 1
			while the buffer value is not null
				set test to 0
				print to screen
				send to client
			if test == 1
			read in one line
				
				
				*/
