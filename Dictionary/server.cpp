//  Hello World server
#include <czmq.h>
#include <zmq.h>
#include <bits/stdc++.h>
using namespace std;


typedef unordered_map<string, string> Dict;


Dict dict;

void dispatch(zmsg_t* incmsg, zmsg_t* outmsg){
    string fun = zmsg_popstr(incmsg);
    string word;
    if(fun == "Find"){
        word = zmsg_popstr(incmsg);
        if(dict.count(word) > 0){
            zmsg_addstr(outmsg, dict[word].c_str());
        }
        else{
            zmsg_addstr(outmsg, "Not Found");
        }
    }
    else if (fun == "Add"){
        word = zmsg_popstr(incmsg);
        string means = zmsg_popstr(incmsg);
        dict[word] = means;
        zmsg_addstr(outmsg, "Word Added");
               
    }
    else{
        zmsg_addstr(outmsg, "Not Supported"); 
    }
    
    
}

int main (void)
{
    //  Socket to talk to clients
    zctx_t* context = zctx_new();
    void *responder = zsocket_new(context, ZMQ_REP);
    zsocket_bind(responder, "tcp://*:5555");
    
    
    while (1) {
        zmsg_t* inc = zmsg_recv (responder);
        cout << "Message Received" << endl;
        zmsg_t* resp = zmsg_new();
        dispatch(inc, resp);
        cout << "Sending answer" << endl;
        zmsg_send(&resp, responder);
        
        
     }
    return 0;
}
