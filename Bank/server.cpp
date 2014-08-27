//  Hello World server
#include <czmq.h>
#include <zmq.h>
#include <bits/stdc++.h>
using namespace std;


unordered_map<string, int> accounts;

void createAccount(zmsg_t* incmsg, zmsg_t* outmsg){
	string id = zmsg_popstr(incmsg);
	if(accounts.count(id) > 0){
		zmsg_addstr(outmsg, "Account already exists!");
	}
	else{
		accounts[id] = 0;
		zmsg_addstr(outmsg, "success");
		zmsg_print(outmsg);
		cout << "here" << endl;
	}	
}


void dispatch(zmsg_t* incmsg, zmsg_t* outmsg){
	string command = zmsg_popstr(incmsg);
	if(command == "create") createAccount(incmsg, outmsg);
	else zmsg_addstr(outmsg, "Not supported");
    
    
    
}

/*word = zmsg_popstr(incmsg);
string means = zmsg_popstr(incmsg);
dict[word] = means;
zmsg_addstr(outmsg, "Word Added");*/

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
        zmsg_print(resp);
        zmsg_send(&resp, responder);

        
        
     }
    return 0;
}
