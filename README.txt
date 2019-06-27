HW1-HW4 are probably the most usefule files here for anyone trying to understand basic socket programming.

Difference between TCP and UDP:
Let me just summarize saying that since TCP requires a connection, it needs the following APIs on server and client
server : socket(), bind(), listen(), accept(), recv(), send()
client : socket(), connect(), send(), recv()

And since UDP is a connectionless protocol, it does not require connect() on the client and listen() and accept() on the server
server : socket(), bind(), recvfrom(), sendto()
client : socket(), sendto(), recvfrom()
