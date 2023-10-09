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
    char * received;
    int rLength;
    char * readBuffer;

    if (argc == 3 || argc == 4) {
        fileName = argv[1];
        char* suffix = strrchr(fileName, '.');
        if (strcasecmp(suffix + 1, "txt") != 0) {
            printf("The file is not a .txt file.\n");
            exit(-1);
        }

        dest.sin_family = AF_INET; // Use the IPv4 address family
        
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
        /*
        Figures this out later for 2%
        error = getaddrinfo(address,port,&hints,&res);
        if (error != 0) {
            fprintf(stderr, "%s: %s\n", address, gai_strerror(error));
            exit(-1);
        }
        
        if (res->ai_addr->sa_family == AF_INET) {
            struct sockaddr_in *p = (struct sockaddr_in *)res->ai_addr;
            printf("%s\n", inet_ntop(AF_INET, &p->sin_addr, str, sizeof(str)));
        } else if (i->ai_addr->sa_family == AF_INET6) {
            struct sockaddr_in6 *p = (struct sockaddr_in6 *)res->ai_addr;
            printf("%s\n", inet_ntop(AF_INET6, &p->sin6_addr, str, sizeof(str)));
        }
        */
        
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

    int mysocket = socket(dest.sin_family, SOCK_STREAM, 0);

    // Connect to the server
	error = connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));
    if (error != 0) {
        fprintf(stderr,"Error Connecting to Server: %s\n",strerror(errno));
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

    fseek(fptr, 0, SEEK_END);
    int file_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    char * initialMessage = malloc(bufSize*sizeof(char));
    if (bufSize < snprintf(initialMessage, bufSize,"%s %d\n",fileName,file_size)) {
        fprintf(stderr,"Error: File name %s and length too large for buffer length %d\n",fileName,bufSize);
        free (initialMessage);
        fclose(fptr);
        close(mysocket);
        exit(-1);
    }

    // Send initial message
    send(mysocket, initialMessage, strlen(initialMessage), 0); 
    free(initialMessage);

    received = malloc(bufSize*sizeof(char)+1);
    // Confirm message was recieved
	rLength = recv(mysocket, received, bufSize, 0);
    received[rLength] = '\0';

    /* Put error check here*/

    readBuffer = malloc(bufSize*sizeof(char));
    while (fread(readBuffer, bufSize, sizeof(char), fptr) != NULL) {
        send(mysocket, readBuffer, strlen(readBuffer), 0);  
    }


    

    rLength = recv(mysocket, received, bufSize, 0);
    received[rLength] = '\0';

    /* Put error check here*/
    fclose(fptr);
    free(readBuffer);
    close(mysocket);
}