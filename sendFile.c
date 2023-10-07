#include "protocol.h"

int main(int argc, char *argv[])
{
    int bufSize = DEFAULT_BUFFER;
    struct sockaddr_in dest;
    struct addrinfo hints;
    struct addrinfo *res;
    char * fileName;
    int error;
    char * tempArg;
    char * address;
    char * port;

    if (argc == 3 || argc == 4) {
        fileName = argv[1];

        dest.sin_family = AF_INET;	
        
        
        strcpy(tempArg, argv[2]);
        char * token = strtok(tempArg,":");
        if (token == NULL) {
            printf("Invalid argument format. Input should be formatted as  $./sendFile fileName server-IP-address:port-number bufSize. \n");
            exit(0);
        }
        
        strcpy(address,token);

        if (token == NULL) {
            printf("Invalid argument format. Input should be formatted as  $./sendFile fileName server-IP-address:port-number bufSize. \n");
            exit(0);
        }

        strcpy(port,token);

        error = getaddrinfo(address,port,&hints,&res);
        if (error != 0) {
            fprintf(stderr, "%s: %s\n", address, gai_strerror(error));
            exit(-1);
        }
        


        inet_aton(token, &dest.sin_addr);
        dest.sin_port = htons(atoi(token));  
        //Initialize the destination socket information
        				   // Use the IPv4 address family
        if (argc == 1){
            dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Set destination IP number - localhost, 127.0.0.1
        }else if (argc == 2){
            inet_aton(argv[1], &dest.sin_addr); // Set destination IP number from command line arg
        }else{
            printf("Invalid number of arguments\n");
            printf("Usage: %s <ip>, where ip is an optional IP4 address in the dot-decimal notation\n", argv[0]);
            printf("If the IP address is not proided, client attempts to connect to localhost\n");
            exit(0);
        }

	    //dest.sin_port = htons(PORTNUM);                // Set destination port number



        if (argc == 4) {
            bufSize = atoi(argv[3]);
        }
    } else {
        printf("Invalid argument format. Input should be formatted as  $./sendFile fileName server-IP-address:port-number bufSize. \n");
        exit(0);
    }


}