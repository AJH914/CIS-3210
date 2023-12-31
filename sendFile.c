#include "protocol.h"

//File includes code taken from TCPserver.c and Beej's Guide to Network Programming 

int main(int argc, char *argv[])
{
    int bufSize = DEFAULT_BUFFER;
    struct addrinfo hints, *res;
    char fileName[MAX_FILE_LENGTH];
    int error;
    char tempArg[MAX_ADDRESS];
    char address[MAX_ADDRESS];
    char port[MAX_PORT];
    char * received;
    int rLength;
    char * readBuffer;
    int mysocket;
    char * confirm = CONFIRMATION_MSG;
    int bytesSent = 0;
    int bytesRead = 0;

    if (argc == 3 || argc == 4) {
        //Initialize the destination socket information
        if (argc == 4) {
            bufSize = atoi(argv[3]);
        }
        strcpy(fileName,argv[1]);
        char* suffix = strrchr(fileName, '.');
        if (strcasecmp(suffix + 1, "txt") != 0) {
            printf("The file is not a .txt file.\n");
            exit(-1);
        }

        //Code taken from TCPserver.c
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
        token = strtok(NULL, ":");
        strcpy(port,token);
        //Code obtained from Beej's Guide to Network Programming
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
        hints.ai_socktype = SOCK_STREAM;
        error = getaddrinfo(address,port,&hints,&res);
        if (error != 0) {
            fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(error));
            exit(-1);
        }        
        
    } else {
        printf("Invalid argument format. Input should be formatted as  $./sendFile fileName server-IP-address:port-number bufSize. \n");
        exit(-1);
    }
    mysocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);;
    //Code taken from TCPserver.c
    // Connect to the server
	error = connect(mysocket, res->ai_addr, res->ai_addrlen);
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
    bytesSent = send(mysocket, fileName, strlen(fileName), 0);
    if (bytesSent == -1) {
        fprintf(stderr,"Error: File name transmission failed: %s\n",strerror(errno));
        fclose(fptr);
        free(received);
        close(mysocket);
        exit(-1);
    }


    
    rLength = recv(mysocket, received, bufSize, 0);
    received[rLength] = '\0';

    if (rLength == -1) {
        fprintf(stderr,"Error: Failed to hear back from server: %s\n",strerror(errno));
        fclose(fptr);
        free(received);
        close(mysocket);
        exit(-1);
    } else if (strcmp(fileName,received) != 0) {
        fprintf(stderr,"Error: File name transmission failed, received back \"%s\" from server instead of %s\n",received,fileName);
        fclose(fptr);
        free(received);
        close(mysocket);
        exit(-1);
    }
    /* Put error check here*/

    readBuffer = malloc(bufSize*sizeof(char));
    bytesRead = 1;
    while (!feof(fptr)){
        bytesRead = fread(readBuffer, sizeof(char), bufSize, fptr);
        if (ferror(fptr)) {
            printf("Error in reading from file : %s\n",fileName);
            fclose(fptr);
            free(received);
            free(readBuffer);
            close(mysocket);
            exit(-1);
            break;
        }
        readBuffer[bytesRead] = '\0';
        bytesSent = send(mysocket, readBuffer, strlen(readBuffer), 0);
        if (bytesSent == -1) {
            fprintf(stderr,"Error: File name transmission failed: %s\n",strerror(errno));
            fclose(fptr);
            free(received);
            free(readBuffer);
            close(mysocket);
            exit(-1);
        } 
    }
    printf("Finished transmission of file\n");

    fclose(fptr);
    free(received);
    free(readBuffer);
    close(mysocket);
}
