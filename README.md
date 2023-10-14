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

Our script for running it with multiple files is a1script.sh, and can be run using ./a1script.sh
The server IP and port must be manually set in the script for it to work. Add all the test file names 
into the array the 

Test file size must be less than 4 gb