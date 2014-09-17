CC=g++

all: server slave client

server: server.cpp
	$(CC) -std=c++11 -c server.cpp
	$(CC) -std=c++11 -lzmq -lczmq -o server server.o
	

slave: slave.cpp
	$(CC) -std=c++11 -c slave.cpp
	$(CC) -std=c++11 -lzmq	-lczmq -o slave slave.o
	
client: client.cpp
	$(CC) -std=c++11 -c client.cpp
	$(CC) -std=c++11 -lzmq	-lczmq -o client client.o
		
clean:
	rm -f server slave client *.o *~ *.swp
