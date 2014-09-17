#include <iostream>
#include <czmq.h>
#include <string.h>


using namespace std;

int main(void) {
  zctx_t* context = zctx_new();
  void* client = zsocket_new(context, ZMQ_DEALER);
  zsocket_connect(client, "tcp://localhost:5556");
  
  string op = "";
  string x = "";
  string y = "";
    
  while(op!="exit"){
  	cin >> op;
  	cin >> x;
  	cin >> y;
  	
  	zmsg_t* outmsg = zmsg_new();
  	zmsg_addstr(outmsg, op.c_str());
  	zmsg_addstr(outmsg, x.c_str());
  	zmsg_addstr(outmsg, y.c_str());
  	
  	zmsg_send(&outmsg, client);  	
  	cout << "message sent" << endl;
  	
  	zmsg_t* incmsg = zmsg_recv(client);
	zmsg_print(incmsg);
	
	zmsg_destroy(&incmsg);
  	
  	
  }
  
  zctx_destroy(&context);
  return 0;
}
