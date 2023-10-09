#include "protocol.h"

#define PORTNUM 50940

int mysocket;            // socket used to listen for incoming connections
int consocket;

static void sigintCatcher(int signal,  siginfo_t* si, void *arg)
{
    printf("\n\n************** Caught SIG_INT: shutting down the server ********************\n");

	close(consocket);
	close(mysocket);
	exit(0);
}

int main(int argc, char *argv[])
{
    int bufSize = DEFAULT_BUFFER;
    char * port;

    struct sockaddr_in dest; // socket info about the machine connecting to us
	struct sockaddr_in serv; // socket info about our server
    struct sigaction signaler;


    socklen_t socksize = sizeof(struct sockaddr_in);

	memset(&serv, 0, sizeof(serv));           // zero the struct before filling the fields
	
	serv.sin_family = AF_INET;                // Use the IPv4 address family
	serv.sin_addr.s_addr = htonl(INADDR_ANY); // Set our address to any interface
    serv.sin_port = htons(atoi(port));        // Set the server port number 
    mysocket = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr)) != 0){
		printf("Unable to open TCP socket on localhost:%d\n", PORTNUM);

	/* 	strerror() returns a string representation of the system variable "errno"
	 	errno is the integer code of the error that occured during the last system call from this process
	 	need to include errno.h to use this function
		*/
		printf("%s\n", strerror(errno));
		close(mysocket);
		return 0;
	}

    listen(mysocket, 0);

    if (argc == 2 || argc == 3) {
        port = argv[2];
        if (argc == 3) {
            bufSize = atoi(argv[2]);
        }
    } else {
        printf("Invalid argument format. Input should be formatted as  $./server port-number bufSize \n");
        exit(0);
    }
}