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
		zmsg_addstr(outmsg, "Not supported");
		return;
	}
	sprintf(buffer, "%d", ans);
	zmsg_addstr(outmsg, buffer);
	free(buffer);
	return;
}

void dispatch(zmsg_t* incmsg, zmsg_t* outmsg){
	char* s = zmsg_popstr(incmsg);
	if(strcmp(s, "op") == 0){
		op(incmsg, outmsg);
	}
	else{
		zmsg_addstr(outmsg, "error");		
	}
	return;
}

int main(int argc, char** argv){
	zctx_t* context = zctx_new();
    void* server = zsocket_new(context,ZMQ_DEALER);
    zsocket_connect(server, "tcp://localhost:5556");
    string w_type = "";
    if(argc > 1){
    	w_type = argv[1];
    }
    else w_type = "add";
    string serverans = "";
    zmsg_t* inmsg;
    zmsg_t* outmsg = zmsg_new();
    zmsg_addstr(outmsg, "addworker");
    zmsg_addstr(outmsg, "add");
    while (serverans != "Thanks"){
    	zmsg_print(outmsg);
    	zmsg_send(&outmsg, server);
    	inmsg = zmsg_recv(server);
    	zmsg_print(inmsg);
    	char* s = zmsg_popstr(inmsg);
    	serverans = s;
    	free(s);
    	   	    	
    }
    zmsg_destroy(&inmsg);
	zmsg_destroy(&outmsg);
	outmsg = zmsg_new();
	zframe_t* id;
	zmq_pollitem_t items[] = {{server, 0, ZMQ_POLLIN, 0}};
	
	while(true){
		zmq_poll(items,1,10*ZMQ_POLL_MSEC);
		if(items[0].revents & ZMQ_POLLIN){
			
			inmsg = zmsg_recv(server);
			zmsg_print(inmsg);
			id = zmsg_pop(inmsg);
			zmsg_addstr(outmsg, "send");
			dispatch(inmsg, outmsg);
			zmsg_append(outmsg, &id);
			zmsg_send(&outmsg, server);
			
			zframe_destroy(&id);
			zmsg_destroy(&inmsg);
			
			outmsg = zmsg_new();
			
		}
		
	}
    
    zctx_destroy(&context);
    return 0;
}
