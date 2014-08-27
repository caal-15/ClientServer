//  Hello Bank client
#include <zmq.h>
#include <czmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int main (){

    cout << "Connecting to Bank server..." << endl;
    zctx_t* context = zctx_new();
    void *requester = zsocket_new(context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");
    

    string command = "";
   	string id = "";
	string secondId = "";
	string ammount = "";
	string ans = "";
	bool logged = 0;
    zmsg_t* req = zmsg_new();
    
    while(1){
    	if(logged){
			cout << "Logged in" << endl << endl;
			break;
		}
		
		else{
			cout << "Welcome to InseBank, here are your options:" << endl;
			cout << "1. Enter 'create' and your id to create an account" << endl;
			cout << "2. Enter 'login' and your id to gain access to your account" << endl;  
			cout << "Enter 'exit' to finish"<< endl;
			cin >> command;
			if (command == "exit") break;
			else if(command == "create"){
				cin >> id;
				zmsg_addstr(req, "create");
				zmsg_addstr(req, id.c_str());
				
			}
			else if(command == "login"){
				cin >> id;
				cout << id << endl << endl;
			}
			zmsg_send(&req, requester);
			cout << "Sending message" << endl;
			zmsg_t* resp = zmsg_recv(requester);
			ans =  zmsg_popstr(resp);
			cout << "Answer received: " << ans << endl << endl;
			if(ans == "success") logged = 1;
		}
        	    	
        
	
    }
    
    
    zctx_destroy(&context);
    return 0;
}
