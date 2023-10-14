#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <math.h> 

#define DEFAULT_BUFFER 4096
#define CONFIRMATION_MSG "@"
#define MAX_FILE_LENGTH 256
#define MAX_PORT 6
#define MAX_ADDRESS 50

#endif
