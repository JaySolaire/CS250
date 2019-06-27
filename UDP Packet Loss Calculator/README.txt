This was just a fun little project I created to see how many UDP packets would be dropped if all a client did was send them and all the server did was recieve them.

For the most part, the server did a pretty good job. But fun stuff happens when you add stuff to the server loop. For example, if you add just a printf() statement to the while() loop of the server, then that will cause it to drop over 2/3 the packets. 
What?
Yeah, printf() is so slow that the client can send two different datagrams before the printing has completed.
Play around with this. Put the client on one computer and the server on another. Compare speeds. Its fun :)