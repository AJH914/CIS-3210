CIS 3210 Assignment 1: Andrew Heft & Robert Stegmann

The client is run using $./sendFile fileName server-IP-address:port-number bufSize
Where: 
fileName is the name of the file you are sending
server-IP-address is the IP address of the server
port-number is the TCP port the server listens on
bufSize is the optional size of the buffer used by the client to handle large files/sends. 

The server is run using $./server port-number bufSize
Where: 
port-number is the TCP port the server listens on
bufSize is the optional size of the buffer used by the client to handle large files/sends. 

Test file size must be less than 4 gb