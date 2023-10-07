#include "protocol.h"

int main(int argc, char *argv[])
{
    int bufSize = DEFAULT_BUFFER;
    char * port;

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