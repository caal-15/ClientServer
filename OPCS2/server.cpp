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
	
	
	
	
	zmq_pollitem_t items[] = {{client, 0, ZMQ_POLLIN, 0}, {server, 0, ZMQ_POLLIN, 0}};
	
	while(true){
		zmq_poll (items, 2, 10*ZMQ_POLL_MSEC);
    	if(items[0].revents & ZMQ_POLLIN){
    		cout << "I'm gonna kill myself" << endl;
    		zmsg_t* outmsg = zmsg_new();
    		zmsg_t* incmsg = zmsg_recv(client);
    		zmsg_print(incmsg);
    		dispatchWorker(incmsg, outmsg, client, server);
    		//cout << "here!" << endl;
    				
    		zmsg_destroy(&incmsg);		
    		
    	}
    	if (items[1].revents & ZMQ_POLLIN){
    		cout << "I'm gonna kill myself 2" << endl;
    		zmsg_t* incmsg = zmsg_recv(server);
    		zmsg_print(incmsg);
    		zframe_t* cid = zmsg_pop(incmsg);
    		//SI SE AÑADE EL ID DEL CLIENTE AL MENSAJE A ENVIAR SE PARA EL HILO DE EJECUCIÓN
    		//COMENTAR PARA NO DESTRUIR ID INMEDIATAMENTE
    		zframe_destroy(&cid); 
    		char* s = zmsg_popstr(incmsg);
    		
    		zmsg_t* outmsg = zmsg_new();
    		zmsg_append(outmsg, &workers["add"][0]);
    		zmsg_addstr(outmsg, "op");
    		zmsg_addstr(outmsg, s);
    		//DESCOMENTAR PARA AÑADIR EL ID DEL CLIENTE AL MENSAJE
    		//zmsg_append(outmsg, &cid);
    		zmsg_print(outmsg);
    		   		
    		
    		zmsg_send(&outmsg, client);
    		
    		free(s);
    		//DESCOMENTAR PARA DESTRUIR EL FRAME UNA VEZ ENVIADO EL MENSAJE
    		//zframe_destroy(&cid);
    		zmsg_destroy(&incmsg);
    		
    		
    		
    		
    	}
    	
    	
    }
    
    
	zctx_destroy(&context);
    return 0;
}
