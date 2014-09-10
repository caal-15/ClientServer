CC=g++
ZMQ=/home/utp/cs/zmq
ZMQ_LIBS=$(ZMQ)/lib
ZMQ_HDRS=$(ZMQ)/include


all: client server

client: client.o
	$(CC) -L$(ZMQ_LIBS) -o client client.o -lzmq -lczmq

client.o: client.cc 
	$(CC) -I$(ZMQ_HDRS) -c client.cc 

server: server.o
	$(CC) -L$(ZMQ_LIBS) -o server server.o -lzmq -lczmq

server.o: server.cc 
	$(CC) -I$(ZMQ_HDRS)  -c server.cc 

clean:
	rm -rf client client.o server server.o *~
