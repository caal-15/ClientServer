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
	string amount = "";
	string ans = "";
	bool logged = 0;
    zmsg_t* req = zmsg_new();
    zmsg_t* resp = zmsg_new();
    while(1){
    	if(logged){
			cout << "You are now logged, here are your options:" << endl;
			cout << "1. Enter 'check' to view the amount of funds in account" << endl;
			cout << "2. Enter 'add' and an amount to add funds to your account" << endl;
			cout << "3. Enter 'withdraw' and an amount to remove funds from your account" << endl;
			cout << "4. Enter 'transfer', an amount, and another account to transfer funds" << endl;
			cout << "Enter 'exit' to finish"<< endl;
			cin >> command;
			zmsg_addstr(req, command.c_str());
			if(command == "exit") break;
			else if (command == "check"){
				zmsg_addstr(req, id.c_str());
							
			}
			else if (command == "add" || command == "withdraw"){
				zmsg_addstr(req, id.c_str());
				cin >> amount;
				zmsg_addstr(req, amount.c_str());			
			}
			else if(command == "transfer"){
				zmsg_addstr(req, id.c_str());
				cin >> amount;
				cin >> secondId;
				zmsg_addstr(req, amount.c_str());
				zmsg_addstr(req, secondId.c_str());
			}			
			zmsg_send(&req, requester);
			cout << "Sending message" << endl;
			resp = zmsg_recv(requester);
			ans =  zmsg_popstr(resp);
			cout << "Answer received: " << ans << endl << endl;
			
		}
		
		else{
			cout << "Welcome to InseBank, here are your options:" << endl;
			cout << "1. Enter your id to login, an account will be created if it doesn't exist" << endl;
			cout << "Enter 'exit' to finish"<< endl;
			cin >> id;
			zmsg_addstr(req, "login");
			zmsg_addstr(req, id.c_str());
			zmsg_send(&req, requester);
			cout << "Sending message" << endl;
			resp = zmsg_recv(requester);
			ans =  zmsg_popstr(resp);
			cout << "Answer received: " << ans << endl << endl;
			if(ans == "success") logged = 1;
		}
        zmsg_destroy(&req);    	
        req = zmsg_new();
	
    }
    
    
    zctx_destroy(&context);
    return 0;
}
