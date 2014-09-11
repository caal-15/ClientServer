#include <bits/stdc++.h>
#include <czmq.h>


using namespace std;

unordered_map<string, vector<zframe_t*>> workers;

void addWorker(zframe_t* id, zmsg_t* incmsg, zmsg_t* outmsg){
	char* s = zmsg_popstr(incmsg);
	string op = s;
	free(s);
	workers[op].push_back(zframe_dup(id));
	zmsg_append(outmsg, &id);
	zmsg_addstr(outmsg, "Thanks");
	
}

void dispatchWorker(zmsg_t* incmsg, zmsg_t* outmsg, void* client, void* server){
	zframe_t* id = zmsg_pop(incmsg);
	char* s = zmsg_popstr(incmsg);
	string op = s;
	free(s);
	if(op == "addworker"){
		addWorker(id, incmsg, outmsg);
		zmsg_send(&outmsg, client);
		zframe_destroy(&id);
		return;		
	}
	else return;
	zframe_destroy(&id);
	
}

int main(void) {
	zctx_t* context = zctx_new();
	void* server = zsocket_new(context,ZMQ_ROUTER);
    zsocket_bind(server, "tcp://*:5555");
    void* client = zsocket_new(context,ZMQ_ROUTER);
	zsocket_bind(client, "tcp://*:5556");
	
	zmsg_t* incmsg;
	
	
	zmq_pollitem_t items[] = {{client, 0, ZMQ_POLLIN, 0}, {server, 0, ZMQ_POLLIN, 0}};
	cout << "before while" << endl;
	while(true){
		zmq_poll(items,2,10*ZMQ_POLL_MSEC);
    	if(items[0].revents & ZMQ_POLLIN){
    		zmsg_t* outmsg = zmsg_new();
    		
    		incmsg = zmsg_recv(client);
    		zmsg_print(incmsg);
    		dispatchWorker(incmsg, outmsg, client, server);
    		
    		cout << "Dafuq" << endl;
    		
    		zmsg_destroy(&incmsg);
    		
			
			
    		
    	}
    	else if (workers.size() > 0){
    		this_thread::sleep_for(chrono::seconds(1));
    		zmsg_t* outmsg = zmsg_new();
    		zframe_t* w_id = zframe_dup(workers["add"][0]);
    		zmsg_append(outmsg, &w_id);
    		zmsg_addstr(outmsg, "Ficty");
    		zmsg_addstr(outmsg, "op");
    		zmsg_addstr(outmsg, "3 + 3");
    		zmsg_print(outmsg);
    		cout <<"Here's the fault but i don't know why!"<<endl;
    		zmsg_send(&outmsg, client);		
    		
    		
    		
    	}
    	
    	
    }
    
    
	zctx_destroy(&context);
    return 0;
}
