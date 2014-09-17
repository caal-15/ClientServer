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
        
    zmsg_send(&outmsg, slaves);
    
    free(s);
    zmsg_destroy(&incmsg);
}


void dispatchWorker(zmsg_t* incmsg, void* slaves, void* clients){
    zframe_t* id = zmsg_pop(incmsg);
    char* s = zmsg_popstr(incmsg);
    if (strcmp(s, "addworker") == 0) {
        addWorker(id, incmsg, slaves);
        
    } 
    else if (strcmp(s, "send") == 0) {
    	zmsg_send(&incmsg, clients);	        
    }
    
    free(s);
}

void getWorker(zframe_t* cid, string op, zmsg_t* incmsg, void* slaves){
	zframe_t* wid = zframe_dup(workers[op].back()); 
	zframe_t* temp = zframe_dup(workers[op].back());
	workers[op].pop_back();
	workers[op].insert(workers[op].begin(), temp);
	
	char* s1 = zmsg_popstr(incmsg);
	char* s2 = zmsg_popstr(incmsg);
	
	zmsg_t* outmsg = zmsg_new();
	zmsg_append(outmsg, &wid);
	zmsg_append(outmsg, &cid);
	zmsg_addstr(outmsg, op.c_str());
	zmsg_addstr(outmsg, s1);
	zmsg_addstr(outmsg, s2);
	
	cout << "Sending to:" << endl;
	zmsg_print(outmsg);
	cout << endl;
	
	zmsg_send(&outmsg, slaves);
	
	free(s1);
	free(s2);
}

void dispatchClient(zmsg_t* incmsg, void* slaves, void* clients){
	zframe_t* cid = zmsg_pop(incmsg);
	char* s = zmsg_popstr(incmsg);
	string op = s;
		
	if(workers[op].size() == 0){
		zmsg_t* outmsg = zmsg_new();
		
		zmsg_append(outmsg, &cid);
		zmsg_addstr(outmsg, "No workers exist for that op");
		
		zmsg_send(&outmsg, clients);
	}
	else{
		getWorker(cid, op, incmsg, slaves);		
	}
	zmsg_destroy(&incmsg);
	free(s);
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
            cout << "I'm gonna kill myself (Workers)" << endl;
            
            zmsg_t* incmsg = zmsg_recv(slaves);
            zmsg_print(incmsg);
            cout << endl;
            
            dispatchWorker(incmsg, slaves, clients);
                  
            
        }
        if (items[1].revents & ZMQ_POLLIN){ // Client code
            cout << "I'm gonna kill myself too (Clients)" << endl;
            
            zmsg_t* incmsg = zmsg_recv(clients);
            zmsg_print(incmsg);
            cout << endl;
            
            dispatchClient(incmsg, slaves, clients);
        }
    }
    
    zctx_destroy(&context);
    return 0;
}
