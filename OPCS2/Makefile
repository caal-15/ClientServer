CC=g++

all: server slave

server: server.cpp
	$(CC) -std=c++11 -c server.cpp
	$(CC) -std=c++11 -lzmq -lczmq -o server server.o
	

slave: slave.cpp
	$(CC) -std=c++11 -c slave.cpp
	$(CC) -std=c++11 -lzmq	-lczmq -o slave slave.o
		
clean:
	rm -f server slave *.o *~ 
