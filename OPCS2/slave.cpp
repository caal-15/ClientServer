#include <czmq.h>
#include <zmq.h>
#include <bits/stdc++.h>
using namespace std;

void op(zmsg_t* incmsg, zmsg_t* outmsg){
    char* buffer = zmsg_popstr(incmsg);
    char op;
    int x, y, ans;
    sscanf(buffer, "%i %c %i", &x, &op, &y );
    free(buffer);
    if(op == '+'){
        ans = x + y;
    }
    else if(op == '-'){
        ans = x - y;
    }
    else if(op == '*'){
        ans = x*y;
    }
    else if(op == '/'){
        ans = x / y;
    }
    else{
    	//TODO: implement this fucker
       }
}

void dispatch(zmsg_t* incmsg, void* server){
	zframe_t* cid = zmsg_pop(incmsg);
	return;
}



int main(int argc, char** argv){
    zctx_t* context = zctx_new();
    void* server = zsocket_new(context, ZMQ_DEALER);
    zsocket_connect(server, "tcp://localhost:5555");
    string w_type = "";
    if(argc == 2)
        w_type = argv[1];
    else 
        w_type = "add";
    
    string serverans = "";
    
    while (serverans != "Thanks"){
    	zmsg_t* outmsg = zmsg_new();
    	
    	zmsg_addstr(outmsg, "addworker");
    	zmsg_addstr(outmsg, w_type.c_str());
    	
    	zmsg_send(&outmsg, server);
    	
        zmsg_t* incmsg = zmsg_recv(server);
        zmsg_print(incmsg);
        
        char* s = zmsg_popstr(incmsg);
        serverans = s;
        
        free(s);
        zmsg_destroy(&incmsg);
    }

   
    zmq_pollitem_t items[] = {{server, 0, ZMQ_POLLIN, 0}};
    
    while(true){
        zmq_poll(items,1,10*ZMQ_POLL_MSEC);
        if(items[0].revents & ZMQ_POLLIN){
            cout<< "Message received" << endl;
            zmsg_t* incmsg = zmsg_recv(server);
            zmsg_print(incmsg);
            
            dispatch(incmsg, server);
            
        }
        
    }
    
    zctx_destroy(&context);
    return 0;
}
