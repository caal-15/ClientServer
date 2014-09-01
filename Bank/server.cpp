//  Hello World server
#include <czmq.h>
#include <zmq.h>
#include <bits/stdc++.h>
using namespace std;


unordered_map<string, int> accounts;

void login(zmsg_t* incmsg, zmsg_t* outmsg){
	char* s = zmsg_popstr(incmsg);
	string id = s;
	free(s);
	if(accounts.count(id) > 0){
		zmsg_addstr(outmsg, "success");
	}
	else{
		accounts[id] = 0;
		zmsg_addstr(outmsg, "success");		
	}
		
}

void check(zmsg_t* incmsg, zmsg_t* outmsg){
	char* s = zmsg_popstr(incmsg);
	string id = s;
	free(s);
	if(accounts.count(id)>0){
		zmsg_addstr(outmsg, to_string(accounts[id]).c_str());
	}
	else{
		zmsg_addstr(outmsg, "error");
	}
}

void add(zmsg_t* incmsg, zmsg_t* outmsg){
	char* s = zmsg_popstr(incmsg);
	string id = s;
	free(s);
	if(accounts.count(id)>0){
		s = zmsg_popstr(incmsg);
		accounts[id] += atoi(s);
		free(s);
		zmsg_addstr(outmsg, "success");
	}
	else{
		zmsg_addstr(outmsg, "error");
	}
}

void withdraw(zmsg_t* incmsg, zmsg_t* outmsg){
	char* s = zmsg_popstr(incmsg);
	string id = s;
	free(s);
	if(accounts.count(id) > 0){
		s = zmsg_popstr(incmsg);
		int ans = accounts[id] - atoi(s);
		free(s);
		if (ans < 0) zmsg_addstr(outmsg, "Not enough Funds, Operation Canceled");
		else{
			accounts[id] = ans;
			zmsg_addstr(outmsg, "success");
		}
	}
	else{
		zmsg_addstr(outmsg, "error");
	}
}

void transfer(zmsg_t* incmsg, zmsg_t* outmsg){
	char* s = zmsg_popstr(incmsg);
	string id = s;
	free(s);
	s = zmsg_popstr(incmsg);
	int amount = atoi(s);
	free(s);
	s = zmsg_popstr(incmsg);
	string secondId = s;
	free(s);
	if(accounts.count(id) > 0){
		if(accounts.count(secondId) > 0){
			int ans = accounts[id] - amount;
			if (ans < 0) zmsg_addstr(outmsg, "Not Enough Funds, Operation Canceled");
			else{
				accounts[id] = ans;
				cout << accounts[secondId] << endl;
				accounts[secondId] += amount;
				cout << accounts[secondId] << endl;
				zmsg_addstr(outmsg, "success"); 
			}
		}
		else zmsg_addstr(outmsg, "Account doesn't exist");
	}	
	else{
		zmsg_addstr(outmsg, "error");
	}
}

void dispatch(zmsg_t* incmsg, zmsg_t* outmsg){
	string command = zmsg_popstr(incmsg);
	if(command == "login") login(incmsg, outmsg);
	else if (command == "check") check(incmsg, outmsg);
	else if (command == "add") add(incmsg, outmsg);
	else if (command == "withdraw") withdraw(incmsg, outmsg);
	else if (command == "transfer") transfer(incmsg, outmsg);
	else zmsg_addstr(outmsg, "Not supported");
    
    
    
}

/*word = zmsg_popstr(incmsg);
string means = zmsg_popstr(incmsg);
dict[word] = means;
zmsg_addstr(outmsg, "Word Added");*/

int main (void)
{
    //  Socket to talk to clients
    zctx_t* context = zctx_new();
    void *responder = zsocket_new(context, ZMQ_REP);
    zsocket_bind(responder, "tcp://*:5555");
    
    
    while (1) {
        zmsg_t* inc = zmsg_recv (responder);
        cout << "Message Received" << endl;
        zmsg_print(inc);
        zmsg_t* resp = zmsg_new();
        dispatch(inc, resp);
        cout << "Sending answer" << endl;
        zmsg_print(resp);
        zmsg_send(&resp, responder);

        
        
     }
    return 0;
}
