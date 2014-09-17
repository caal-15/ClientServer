#include <bits/stdc++.h>
#include <czmq.h>

using namespace std;

unordered_map<string, vector<zframe_t*>> workers;
unordered_map<string, vector<bool>> states;
unordered_map<string, vector<zmsg_t*>> msgs;
vector<string> ops;

void addOp(string op){
	for (int i = 0; i < ops.size(); i++){
		if(op == ops[i])
			return;
	}
	ops.push_back(op);
}

void addWorker(zframe_t* id, zmsg_t* incmsg, void* slaves){
    char* s = zmsg_popstr(incmsg);
    char* w_num = (char*) malloc(sizeof(char)*256);
    string op = s;
    
    workers[op].push_back(zframe_dup(id));
    states[op].push_back(0);
    sprintf(w_num, "%d", workers[op].size() - 1);
    addOp(op);
    
    zmsg_t* outmsg = zmsg_new();
       
    zmsg_append(outmsg, &id);
    zmsg_addstr(outmsg, "Thanks");
    zmsg_addstr(outmsg, w_num);
    zmsg_send(&outmsg, slaves);
    
    free(s);
    free(w_num);
    zmsg_destroy(&incmsg);
}


void dispatchWorker(zmsg_t* incmsg, void* slaves, void* clients){
    zframe_t* id = zmsg_pop(incmsg);
    char* s = zmsg_popstr(incmsg);
    if (strcmp(s, "addworker") == 0) {
        addWorker(id, incmsg, slaves);
        
    } 
    else if (strcmp(s, "send") == 0) {
    	char* s1 = zmsg_popstr(incmsg);
    	char* s2 = zmsg_popstr(incmsg);
    	string w_type = s1;
    	int w_num = atoi(s2);
    	states[w_type][w_num] = 0;
    	zmsg_send(&incmsg, clients);
    	free(s1);
    	free(s2);	        
    }
    else {
    	zmsg_destroy(&incmsg);
    }
    
    free(s);
}

void getWorker(void* slaves){
	for(int i = 0; i < ops.size(); i++){
		if(msgs[ops[i]].size() > 0){
			
			for(int j = 0; j < states[ops[i]].size(); j++){
				
				if(states[ops[i]][j] == 0 && msgs[ops[i]].size() > 0){
					zmsg_t* outmsg = zmsg_dup(msgs[ops[i]].back());
					zframe_t* wid = zframe_dup(workers[ops[i]][j]);
					
					zmsg_prepend(outmsg, &wid);
					
					cout << "Sent to: " << ops[i] << " " << j << endl;
					zmsg_print(outmsg);
					cout << endl;
					
					zmsg_send(&outmsg, slaves);
					
					states[ops[i]][j] = 1;
					msgs[ops[i]].pop_back();
					
					
				}
			}
		}
	}
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
		
		char* s1 = zmsg_popstr(incmsg);
		char* s2 = zmsg_popstr(incmsg);	
		
		zmsg_t* outmsg = zmsg_new();
		
		zmsg_append(outmsg, &cid);
		zmsg_addstr(outmsg, op.c_str());
		zmsg_addstr(outmsg, s1);
		zmsg_addstr(outmsg, s2);
		
		msgs[op].insert(msgs[op].begin(), outmsg);
		
		free(s1);
		free(s2);	
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
        getWorker(slaves);
    }
    
    zctx_destroy(&context);
    return 0;
}
