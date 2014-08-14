//  Hello World client
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int main (void)
{
    cout << "Connecting to opserver..." << endl;
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");
    cout << "Success" << endl << endl;
    char buffer [256];
    while(1){
        cout << "Please enter a basic operation, enter 'exit' to finish: ";
        scanf("%[^\n]s", buffer);
        getchar();
        if(string(buffer)=="exit") break;
        zmq_send (requester, buffer, 256, 0);
        cout << "Sending Operation..." << endl;
        zmq_recv (requester, buffer, 256, 0);
        cout << "Received an answer, the Result is: " << string(buffer) << endl << endl;
    }
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
