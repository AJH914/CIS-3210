#include "protocol.h"

//File includes code taken from TCPserver.c and Beej's Guide to Network Programming 

int main(int argc, char *argv[])
{
    int bufSize = DEFAULT_BUFFER;
    struct sockaddr_in dest;
    struct addrinfo hints, *res;
    char * fileName;
    int error;
    char * tempArg;
    char * address;
    char * port;
    char * received;
    int rLength;
    char * readBuffer;
    int mysocket;
    char * confirm = CONFIRMATION_MSG;

    if (argc == 3 || argc == 4) {
        fileName = argv[1];
        char* suffix = strrchr(fileName, '.');
        if (strcasecmp(suffix + 1, "txt") != 0) {
            printf("The file is not a .txt file.\n");
            exit(-1);
        }

        //Code taken from TCPserver.c
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

        //Code obtained from Beej's Guide to Network Programming
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
        hints.ai_socktype = SOCK_STREAM;

        error = getaddrinfo(address,port,&hints,&res);
        if (error != 0) {
            fprintf(stderr, "%s: %s\n", address, gai_strerror(error));
            exit(-1);
        }        
        
        inet_aton(address, &dest.sin_addr);
        dest.sin_port = htons(atoi(port));
        //Initialize the destination socket information
        if (argc == 4) {
            bufSize = atoi(argv[3]);
        }
    } else {
        printf("Invalid argument format. Input should be formatted as  $./sendFile fileName server-IP-address:port-number bufSize. \n");
        exit(-1);
    }

    mysocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    //Code taken from TCPserver.c
    // Connect to the server
	error = connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));
    if (error != 0) {
        fprintf(stderr,"Error Connecting to Server: %s\n",strerror(errno));
        close(mysocket);
        exit(-1);
    }

    if (strlen(fileName) > bufSize) {
        fprintf(stderr,"Error: File name %s longer than buffer size of: %d\n",fileName,bufSize);
        close(mysocket);
        exit(-1);
    }

    FILE * fptr;
    fptr = fopen(fileName,"r");

    if (fptr == NULL) {
        fprintf(stderr,"Error: File %s failed to open: %s\n",fileName,strerror(errno));
        close(mysocket);
        exit(-1);
    }

    received = malloc(bufSize*sizeof(char)+1);

    // Send initial message
    send(mysocket, fileName, strlen(fileName), 0);

    rLength = recv(mysocket, received, bufSize, 0);
    received[rLength] = '\0';

    if (rLength == -1) {
        fprintf(stderr,"Error: File name transmission failed: %s\n",strerror(errno));
        fclose(fptr);
        free(received);
        free(readBuffer);
        close(mysocket);
        exit(-1);
    } else if (strcmp(fileName,received) != 0) {
        fprintf(stderr,"Error: File name transmission failed, received back \"%s\" from server instead of %s\n",received,fileName);
        fclose(fptr);
        free(received);
        free(readBuffer);
        close(mysocket);
        exit(-1);
    }

    /* Put error check here*/

    readBuffer = malloc(bufSize*sizeof(char));
    while (fread(readBuffer, bufSize, sizeof(char), fptr) != NULL) {
        send(mysocket, readBuffer, strlen(readBuffer), 0);  
    }


    rLength = recv(mysocket, received, bufSize, 0);
    received[rLength] = '\0';

    if (strcmp(received,confirm) == 0) {
        printf("File transmitted successfully\n");
    }

    /* Put error check here*/
    fclose(fptr);
    free(received);
    free(readBuffer);
    close(mysocket);
}