//  Hello World server
#include <zmq.h>
#include <bits/stdc++.h>
using namespace std;


typedef unordered_map<string, string> Dict;


Dict dict;

void dispatch(msg incmsg, msg outmsg){
    
}

int main (void)
{
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:5555");
    assert (rc == 0);

    while (1) {
        msg inc;
        msg resp;
        zmq_recv (responder, inc);
        dispatch (inc, resp);
        zmq_send (responder, resp);
        
        
     }
    return 0;
}
