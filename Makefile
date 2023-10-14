CC = gcc
CCFLAGS = -std=gnu99 -Wpedantic -lm -lpthread -g 

all: server sendFile TCPserver TCPclient

server: server.o protocol.h
	$(CC) $(CCFLAGS) server.o protocol.h -o server

server.o: server.c
	$(CC) $(CCFLAGS) -c server.c -o server.o

sendFile: sendFile.o protocol.h
	$(CC) $(CCFLAGS) sendFile.o protocol.h -o sendFile

sendFile.o: sendFile.c
	$(CC) $(CCFLAGS) -c sendFile.c -o sendFile.o

TCPclient: TCPclient.o
	$(CC) $(CCFLAGS) TCPclient.o -o TCPclient

TCPclient.o: TCPclient.c
	$(CC) $(CCFLAGS) -c TCPclient.c -o TCPclient.o

TCPserver: TCPserver.o
	$(CC) $(CCFLAGS) TCPserver.o  -o TCPserver

TCPserver.o: TCPserver.c
	$(CC) $(CCFLAGS) -c TCPserver.c -o TCPserver.o



clean:
	rm *.o
	rm sendFile
	rm server