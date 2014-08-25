//  Hello World client
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int main (int argc, char** argv){

    cout << "Connecting to Dictionary server..." << endl;
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");
    cout << "Success" << endl << endl;
    
    msg req;
    
    if(argv[1]=="find"){
        req[0] =  "Find";
        req[1] = argv[2];
    }
    
    else if (argv[1] == "add"){
        req[1] = "Add";
        for(int i = 2; i < argc; i++){
            req[i-1] = argv[i];
        }
    }
    
    else {
        cout << "error" << endl;
    }
    
    zmq_send(requester, req);
    msg resp;
    zmq_recv(requester, resp);
        
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
