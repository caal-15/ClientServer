#include <iostream>
#include <czmq.h>
#include <string.h>


using namespace std;

int main(void) {
  zctx_t* context = zctx_new();
  void* client = zsocket_new(context,ZMQ_DEALER);
  zsocket_connect(client, "tcp://localhost:5555");
  
  string op = "";
  
  zmq_pollitem_t items[] = {{client, 0, ZMQ_POLLIN, 0}};
  
  zmsg_t* incmsg;
  
  while(op!="exit"){
  	zmq_poll(items,1,10*ZMQ_POLL_MSEC);
  	if(items[0].revents & ZMQ_POLLIN){
  		incmsg = zmsg_recv(client);
  		zmsg_print(incmsg);
  		
  		zmsg_destroy(&incmsg);
  	}
  	cin >> op;
  	zmsg_t* outmsg = zmsg_new();
  	zmsg_addstr(outmsg, op.c_str());
  	zmsg_print(outmsg);
  	zmsg_send(&outmsg, client);
  	zmsg_destroy(&outmsg);
  	cout << "message sent" << endl;
  }
  
  return 0;
}
