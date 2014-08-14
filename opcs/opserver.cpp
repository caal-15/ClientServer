//  Hello World server
#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

void op(char *buffer){
	int x, y, ans;
	char op;
	sscanf(buffer, "%i %c %i", &x, &op, &y );
	switch(op){
		case '+':
			ans = x + y;
			break;
		case '-':
		    ans = x - y;
		    break;
		case '*':
		    ans = x * y;
		    break;
	    case '/':
		    ans = x / y;
		    break;
	    default:
	        sprintf(buffer, "%s", "Operation not supported");
	        return;
	}
	sprintf(buffer, "%d", ans);
}

int main (void)
{
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:5555");
    assert (rc == 0);

    while (1) {
        char buffer [256];
        zmq_recv (responder, buffer, 256, 0);
        cout << "Received Operation: " << string(buffer) << endl; 
        op(buffer);
        cout << "Returning answer" << endl; 
        zmq_send (responder, buffer, 256, 0);
        
        
      }
    return 0;
}
