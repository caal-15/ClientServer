CC=g++

all: server client slave

server: server.cpp
	$(CC) -std=c++11 -c server.cpp
	$(CC) -std=c++11 -lzmq -lczmq -o server server.o
	
client: client.cpp
	$(CC) -std=c++11 -c client.cpp
	$(CC) -std=c++11 -lzmq	-lczmq -o client client.o
	
slave: slave.cpp
	$(CC) -std=c++11 -c slave.cpp
	$(CC) -std=c++11 -lzmq	-lczmq -o slave slave.o
		
clean:
	rm -f client server slave *.o *~ 
