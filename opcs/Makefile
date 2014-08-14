CC=g++

all: server client

server: opserver.cpp
	$(CC) -c opserver.cpp
	$(CC) -lzmq -o server opserver.o
	
client: opclient.cpp
	$(CC) -c opclient.cpp
	$(CC) -lzmq	-o client opclient.o
		
clean:
	rm -f client server opclient.o opserver.o *~ 
