#include <bits/stdc++.h>
#include <czmq.h>

using namespace std;

unordered_map<string, vector<zframe_t*>> workers;

void addWorker(zframe_t* id, zmsg_t* incmsg, void* slaves){
    char* s = zmsg_popstr(incmsg);
    string op(s);
    workers[op].push_back(zframe_dup(id));
    
    zmsg_t* outmsg = zmsg_new();
    
    zmsg_append(outmsg, &id);
    zmsg_addstr(outmsg, "Thanks");
    zmsg_print(outmsg);
    zmsg_send(&outmsg, slaves);
    
    free(s);
    zmsg_destroy(&incmsg);
}


/**
*  outmsg must to be freed in this operation
*/
void dispatchWorker(zmsg_t* incmsg, void* slaves, void* clients){
    zframe_t* id = zmsg_pop(incmsg);
    char* s = zmsg_popstr(incmsg);
    if (strcmp(s, "addworker") == 0) {
        addWorker(id, incmsg, slaves);
        
    } else {
        cout << "TODO: send operations " << endl;  
    }
    
   
}

int main(void) {
    zctx_t* context = zctx_new();
    void* slaves = zsocket_new(context,ZMQ_ROUTER);
    zsocket_bind(slaves, "tcp://*:5555");
    void* clients = zsocket_new(context,ZMQ_ROUTER);
    zsocket_bind(clients, "tcp://*:5556");
    
    
    zmq_pollitem_t items[] = {{slaves, 0, ZMQ_POLLIN, 0}, {clients, 0, ZMQ_POLLIN, 0}};
    
    while(true){
        zmq_poll (items, 2, 10*ZMQ_POLL_MSEC);
        if(items[0].revents & ZMQ_POLLIN){  // Slave code
            cout << "I'm gonna kill myself" << endl;
            zmsg_t* incmsg = zmsg_recv(slaves);
            zmsg_print(incmsg);
            dispatchWorker(incmsg, slaves, clients);
                  
            
        }
        if (items[1].revents & ZMQ_POLLIN){ // Client code
            cout << "I'm gonna kill myself 2" << endl;
            zmsg_t* incmsg = zmsg_recv(clients);
            zmsg_print(incmsg);
            
            zframe_t* cid = zmsg_pop(incmsg);
            char* s = zmsg_popstr(incmsg);
	    	zframe_t* wid = zframe_dup(workers["add"][0]);
            
            zmsg_t* outmsg = zmsg_new();
            zmsg_append(outmsg, &wid);
            zmsg_append(outmsg, &cid);
            zmsg_addstr(outmsg, s);
              
            zmsg_send(&outmsg, slaves);
            
            
            zmsg_destroy(&incmsg);
            free(s);
        }
    }
    
    zctx_destroy(&context);
    return 0;
}
