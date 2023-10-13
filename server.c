#include "protocol.h"

//File includes code taken from TCPserver.c and Beej's Guide to Network Programming 

#define PORTNUM 50940

//Code taken from TCPserver.c
int mysocket;            // socket used to listen for incoming connections
int consocket;

static void sigintCatcher(int signal,  siginfo_t* si, void *arg)
{
    printf("\n\n************** Caught SIG_INT: shutting down the server ********************\n");

	close(consocket);
	close(mysocket);
	exit(-1);
}

int main(int argc, char *argv[])
{
    int bufSize = DEFAULT_BUFFER;
    char * port;
    int len;
    unsigned long fileSize;
    unsigned long chunks;
    int recievingFlag = 0;
    char * confirm = CONFIRMATION_MSG;
    FILE * fptr;

    //Code taken from TCPserver.c
    struct sockaddr_in dest; // socket info about the machine connecting to us
	struct sockaddr_in serv; // socket info about our server
    struct sigaction signaler;

    struct addrinfo hints, *res;
    

    socklen_t socksize = sizeof(struct sockaddr_in);

    if (argc == 2 || argc == 3) {
        port = argv[2];
        if (argc == 3) {
            bufSize = atoi(argv[2]);
        }
    } else {
        printf("Invalid argument format. Input should be formatted as  $./server port-number bufSize \n");
        exit(-1);
    }    


    // Add error checking
    //Code obtained from Beej's Guide to Network Programming

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
    getaddrinfo(NULL, port, &hints, &res);

    mysocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    /*
    memset(&serv, 0, sizeof(serv));           // zero the struct before filling the fields
	serv.sin_family = AF_INET;                // Use the IPv4 address family
	serv.sin_addr.s_addr = htonl(INADDR_ANY); // Set our address to any interface
    serv.sin_port = htons(atoi(port));        // Set the server port number 
    mysocket = socket(AF_INET, SOCK_STREAM, 0);
    */


    //Code taken from TCPserver.c
    if (bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr)) != 0){
		printf("Unable to open TCP socket on localhost:%d\n", PORTNUM);
		printf("%s\n", strerror(errno));
		close(mysocket);
		exit(-1);
	}

    listen(mysocket, 0);
    consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);


    char * textReceived = malloc(sizeof(char)*(bufSize+1));
    char * fileName = malloc(sizeof(char)*(bufSize+1));

    //Code taken from TCPserver.c
    while(consocket)
	{
		printf("Incoming connection from %s on port %d\n", inet_ntoa(dest.sin_addr), ntohs(dest.sin_port));
		
		// Receive data from the client
		len = recv(consocket, textReceived, bufSize, 0);

        if (len == -1) {
            fprintf(stderr,"Error: File name transmission failed: %s\n",strerror(errno));
            free(fileName);
            free(textReceived);
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
            close(consocket);
            close(mysocket);
            exit(-1);
        }
        chunks = 0;
        recievingFlag = 0;
        while (recievingFlag == 0) {
            len = recv(consocket, textReceived, bufSize, 0);
            if (len > 0 && textReceived != NULL) {
                textReceived[len] = '\0';
                fputs(textReceived,fptr);
                chunks++;
            } else if (len == 0) {
                recievingFlag = 1;
            } else if (len == -1) {
                fprintf(stderr,"Error: File name transmission failed: %s\n",strerror(errno));
                free(fileName);
                free(textReceived);
                close(consocket);
                close(mysocket);
                fclose(fptr);
                recievingFlag = 0;
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
        printf("Recieved %s from %s on port %d\n",fileName,inet_ntoa(dest.sin_addr), ntohs(dest.sin_port));
        printf("%s contains %lu bytes, received in %lu chunks\n",fileName,fileSize,chunks);
		//Send data to client
		send(consocket, confirm, strlen(confirm), 0); 
		
		close(consocket);
		consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
	}

    free(fileName);
    free(textReceived);
    close(mysocket);
}

