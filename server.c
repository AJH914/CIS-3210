#include "protocol.h"

//File includes code taken from TCPserver.c and Beej's Guide to Network Programming 

//Code taken from TCPserver.c
int mysocket;            // socket used to listen for incoming connections
int consocket;

int main(int argc, char *argv[])
{
    int bufSize = DEFAULT_BUFFER;
    int status;
    char port[MAX_PORT];
    int len;
    unsigned long fileSize;
    unsigned long chunks;
    int receivingFlag = 0;
    char * confirm = CONFIRMATION_MSG;
    FILE * fptr;
    void *addr;
    //Code taken from TCPserver.c
    struct sockaddr_in dest; // socket info about the machine connecting to us
    struct sigaction signaler;

    struct addrinfo hints, *res,*p;
    char ipstr[INET6_ADDRSTRLEN];
    

    socklen_t socksize = sizeof(struct sockaddr_in);

    if (argc == 2 || argc == 3) {
        strcpy(port,argv[1]);
        if (argc == 3) {
            bufSize = atoi(argv[2]);
        }
    } else {
        printf("Invalid argument format. Input should be formatted as  $./server port-number bufSize \n");
        freeaddrinfo(res);
        exit(-1);
    }    


    // Add error checking
    //Code obtained from Beej's Guide to Network Programming

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    //hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
    status = getaddrinfo(NULL, port, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(-1);
    }

    mysocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    int flag=1;
    //Check if Address is in use
	if (setsockopt(mysocket,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag)) == -1) {
    	printf("setsockopt() failed\n");
		printf("%s\n", strerror(errno));
        freeaddrinfo(res);
		close(mysocket);
    	exit(-1);
	} 

    //Code taken from TCPserver.c
    if (bind(mysocket, res->ai_addr, res->ai_addrlen) != 0){
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
        addr = &(ipv4->sin_addr);
        inet_ntop(res->ai_family, addr, ipstr, sizeof ipstr);
        fprintf(stderr,"Unable to open TCP socket on: at %s:%s\n", ipstr,port);
		fprintf(stderr,"%s\n", strerror(errno));
        freeaddrinfo(res);
		close(mysocket);
		exit(-1);
	}
    
    //Code from beej's for printing IP
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    addr = &(ipv4->sin_addr);
    inet_ntop(res->ai_family, addr, ipstr, sizeof ipstr);
    printf("Server is running at %s:%s\n", ipstr,port);

    if (listen(mysocket, 0) == -1) {
        fprintf(stderr,"Error: Listen failed: %s\n", strerror(errno));
        freeaddrinfo(res);
		close(mysocket);
		exit(-1);
    }
    consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);

    char * textReceived = malloc(sizeof(char)*(bufSize+1));
    char * fileName = malloc(sizeof(char)*(bufSize+1));

    //Code taken from TCPserver.c
    while(consocket)
	{
		printf("\nIncoming connection from %s on port %d\n", inet_ntoa(dest.sin_addr), ntohs(dest.sin_port));
		
		// Receive data from the client
		len = recv(consocket, textReceived, bufSize, 0);

        if (len == -1) {
            fprintf(stderr,"Error: File name transmission failed: %s\n",strerror(errno));
            free(fileName);
            free(textReceived);
            freeaddrinfo(res);
            close(consocket);
            close(mysocket);
            exit(-1); 
        }

		textReceived[len] = '\0';
        strcpy(fileName,textReceived); 

        send(consocket, fileName, strlen(fileName), 0); 
        //Code taken from TCPserver.c
        printf("Receiving %s from %s on port %d\n",fileName,inet_ntoa(dest.sin_addr), ntohs(dest.sin_port));


        //Receive file from clients
        
        fptr = fopen(fileName,"w");
        if (fptr == NULL) {
            fprintf(stderr,"Error: File %s failed to open: %s\n",fileName,strerror(errno));
            free(fileName);
            free(textReceived);
            freeaddrinfo(res);
            close(consocket);
            close(mysocket);
            exit(-1);
        }
        chunks = 0;
        receivingFlag = 0;

        while (receivingFlag == 0) {
            len = recv(consocket, textReceived, bufSize, 0);
            if (len > 0 && textReceived != NULL) {
                textReceived[len] = '\0';
                fputs(textReceived,fptr);
                chunks++;
            } else if (len == 0) {
                receivingFlag = 1;
            } else if (len == -1) {
                fprintf(stderr,"Error: File name transmission failed: %s\n",strerror(errno));
                free(fileName);
                free(textReceived);
                freeaddrinfo(res);
                close(consocket);
                close(mysocket);
                fclose(fptr);
                receivingFlag = 0;
                exit(-1); 
            }
        }

        fclose(fptr);
        fptr = NULL;

        fptr = fopen(fileName,"r");
        fseek(fptr, 0, SEEK_END);
        fileSize = ftell(fptr);
        fseek(fptr, 0, SEEK_SET);
        fclose(fptr);
        fptr = NULL;
        printf("Received %s from %s on port %d\n",fileName,inet_ntoa(dest.sin_addr), ntohs(dest.sin_port));
        printf("%s contains %lu bytes, received in %lu chunks\n",fileName,fileSize,chunks);
		
		close(consocket);
		consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
	}

    free(fileName);
    free(textReceived);
    freeaddrinfo(res);
    close(mysocket);
}

