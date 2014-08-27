CC=g++

all: server client

server: server.cpp
	$(CC) -std=c++11 -c server.cpp
	$(CC) -std=c++11 -lzmq -lczmq -o server server.o
	
client: client.cpp
	$(CC) -std=c++11 -c client.cpp
	$(CC) -std=c++11 -lzmq	-lczmq -o client client.o
		
clean:
	rm -f client server *.o *~ 
