//  Hello World client
#include <zmq.h>
#include <czmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int main (int argc, char** argv){

    cout << "Connecting to Dictionary server..." << endl;
    zctx_t* context = zctx_new();
    void *requester = zsocket_new(context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");
    
    
    zmsg_t* req = zmsg_new();
    
    if(strcmp("find", argv[1]) == 0){
        zmsg_addstr(req, "Find");
        zmsg_addstr(req, argv[2]);
        
    }
    
    else if (strcmp("add", argv[1]) == 0){
        zmsg_addstr(req, "Add");
        zmsg_addstr(req, argv[2]);
        zmsg_addstr(req, argv[3]);
        
    }
    
    else {
        cout << "error" << endl;
    }
    
    zmsg_send(&req, requester);
    cout << "Sending commands" << endl;
    zmsg_t* resp = zmsg_recv(requester);
    cout << "Answer received: " << zmsg_popstr(resp) << endl;    
    
    zctx_destroy(&context);
    return 0;
}
