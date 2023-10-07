CC = gcc
CCFLAGS = -std=gnu99 -Wpedantic -lm -lpthread -g 

all: server sendFile

server: server.o protocol.h
	$(CC) $(CCFLAGS) server.o protocol.h -o server

server.o: server.c
	$(CC) $(CCFLAGS) -c server.c -o server.o

sendFile: sendFile.o protocol.h
	$(CC) $(CCFLAGS) sendFile.o protocol.h -o sendFile

sendFile.o: sendFile.c
	$(CC) $(CCFLAGS) -c sendFile.c -o sendFile.o


clean:
	rm *.o
	rm sendFile
	rm server