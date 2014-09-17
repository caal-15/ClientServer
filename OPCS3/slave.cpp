#include <czmq.h>
#include <zmq.h>
#include <bits/stdc++.h>
using namespace std;

int operate(int x, int y, string op){
    
    if(op == "add"){
        return x + y;
    }
    else if(op == "sub"){
        return x - y;
    }
    else if(op == "mul"){
        return x*y;
    }
    else if(op == "div"){
        return x / y;
    }
    
}

void dispatch(zmsg_t* incmsg, void* server, string w_type, string w_num){
	zframe_t* cid = zmsg_pop(incmsg);
	int x, y;
		
	char* s = zmsg_popstr(incmsg);
	string op = s;
	
	char* s1 = zmsg_popstr(incmsg);
	x = atoi(s1);
	
	char* s2 = zmsg_popstr(incmsg);
	y = atoi(s2);
	
	zmsg_t* outmsg = zmsg_new();
	zmsg_addstr(outmsg, "send");
	zmsg_addstr(outmsg, w_type.c_str());
	zmsg_addstr(outmsg, w_num.c_str());
	zmsg_append(outmsg, &cid);
	
	if(op != w_type){
		zmsg_addstr(outmsg, "Not supported");
	}
	else{
		char* ans = (char*) malloc(sizeof(char)* 256);
		sprintf(ans, "%d", operate(x, y, op));
		zmsg_addstr(outmsg, ans);
		free(ans);
	}
	
	zmsg_send(&outmsg, server);
	
	zmsg_destroy(&incmsg);
	free(s);
	free(s1);
	free(s2);
}



int main(int argc, char** argv){
    zctx_t* context = zctx_new();
    void* server = zsocket_new(context, ZMQ_DEALER);
    zsocket_connect(server, "tcp://localhost:5555");
    
    string w_type = "";
    string w_num = "";
    int sleep = 0;
    
    if(argc == 2)
        w_type = argv[1];
    else if (argc == 3){
    	w_type = argv[1];
    	sleep = atoi(argv[2]);
    }
    else 
        w_type = "add";
    
    string serverans = "";
    
    while (true){
    	zmsg_t* outmsg = zmsg_new();
    	
    	zmsg_addstr(outmsg, "addworker");
    	zmsg_addstr(outmsg, w_type.c_str());
    	
    	zmsg_send(&outmsg, server);
    	
        zmsg_t* incmsg = zmsg_recv(server);
        zmsg_print(incmsg);
        
        char* s = zmsg_popstr(incmsg);
        serverans = s;
        
        if(serverans == "Thanks"){
        	char* s1 = zmsg_popstr(incmsg);
        	w_num = s1;
        	free(s1);
        	free(s);
       		zmsg_destroy(&incmsg);
       		break;
        }
        
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
            cout << "waiting for: " << sleep << " seconds" << endl;
            this_thread::sleep_for (chrono::seconds(sleep));
            dispatch(incmsg, server, w_type, w_num);
            
        }
        
    }
    
    zctx_destroy(&context);
    return 0;
}
