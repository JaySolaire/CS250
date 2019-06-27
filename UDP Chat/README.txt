Project 2 Chat Program
by David Rhett

Written for Linux Operating Systems. Please compile in the terminal, and place the compiled file anywhere you want. 

...is what I would like to say if the program worked. Unfortunately, there were some issues.

Client Features:
- Takes input as IP address and Port 
- Uses select function to switch between read and write, and timeouts after a 10 seconds.
- Collects a name for the user that will be added to the beginning of every message sent
- Appends the date to every message sent
- Formats the message in a readable manner, such that if the user John types "hi", the message recieved by everyone else will be:
John: hi
Fri May 4 12:00:00 DST 2018
- sends a Sent! message to the Users screen after a successful message send.
- Can send multiple messages consecutively.

Client Bugs:
- It doesn't atually recieve.. the (FD_ISSET(client_fd, &readfds)!=0) was supposed to test if the socket was available for reading, but this never returns true, even if the server is sending messages to it.
- After the timeout function, it doesn't loop. Once it's timed out, it can't do anything.

Client Notes:
When I first heard about the select's timeout feature, I thought that it could be appplied to each individual function. I planned to alternate between the recv and send, each with a timeout of one second. The other function would be blocked for this time, but a one second delay would be no issue. this way, the program would constantly check to see if it had something to recieve, then check for things to send, effectively allowing the user to send multiple times without recieving or vice versa.
Further research yielded the information that the select function simply checks for availability on a socket, so I would have to use it for both functions. My final solution was to try
1. Check for errors
2. Check something to read in from the socket (recv)
3. Check for something to read from stdin (send)
4. loop


Server Features:
- Successfully runs in the background as a daemon. Can be run from anywhere.
- Successfully reads in any UDP messages recieved.
- Has the functionality to send messages back on the socket they were recieved on.

Server Bugs
- Does not send to multiple clients

I tried a couple different ways to achieve multiple client functionality. The two methods that I found online were multithreading and select(). I heard multithreading was messy and inefficient, so I focused on the latter.
The most popular method by far was using an fd_set of socket descriptors to hold each of the different connecting clients. Whenever a new client connected, it would be added to the set. This way, there was a "master socket" that each client would connect to, and as soon as they were connected, the server would redirect them to a different socket. 
The only issue with this method is that it relied heavily on TCP functionality. The client file descriptor would be generated during the connect function, which did not exist in UDP. I eventually had to scrap that server file. I put some of the basic ideas back in version 2 for reference.
I tried again using the ideas from fd_set. I created an array of sockaddr_in to store the client addresses, which I would collect from a recvfrom(). This seemed to work pretty well at first, except that I couldn't compare the data in the array to the data coming in. I couldn't check to see whether a message was from a client in the array or a new client that I need to add. While the struct could be made an array and values stored in it, I cannot binarily compare structs. 
if (struct1 ==struct2) is not valid code. Perhaps if I used the cli_addr.sin_addr and cli_addr.sin_port.. I'll look into that next.





