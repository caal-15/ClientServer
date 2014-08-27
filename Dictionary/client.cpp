//  Hello World client
#include <zmq.h>
#include <czmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int main (){

    cout << "Connecting to Dictionary server..." << endl;
    zctx_t* context = zctx_new();
    void *requester = zsocket_new(context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");
    

    string command = "";
   	string word = "";
	string meaning = "";
    zmsg_t* req = zmsg_new();
    
    while(command != "exit"){
    	cout << "Please enter 'find' and the word to find or" << endl << "'add' with the word and the meaning" << endl; 
		cout << "enter 'exit' to finish"<< endl;
        cin >> command;
        if(command == "find"){
        	cin >> word;
        	cout << word << endl << endl;
        	zmsg_addstr(req, "Find");
        	zmsg_addstr(req, word.c_str());
        	zmsg_send(&req, requester);
    		cout << "Searching..." << endl << endl;
    		zmsg_t* resp = zmsg_recv(requester);
    		cout << "Answer received: " << zmsg_popstr(resp) << endl << endl;
        }
        else if (command == "add"){
        	cin >> word;
        	getline (cin, meaning, '\n');
        	zmsg_addstr(req, "Add");
        	zmsg_addstr(req, word.c_str());
        	zmsg_addstr(req, meaning.c_str());
        	zmsg_send(&req, requester);
        	cout << "Adding..." << endl << endl;
    		zmsg_t* resp = zmsg_recv(requester);
    		cout << "Answer received: " << zmsg_popstr(resp) << endl << endl; 
        }
        else if (command != "exit") cout << "error" << endl << endl;
        zmsg_destroy(&req);
        req = zmsg_new();	    	
        
	
    }
    
       
    
    zctx_destroy(&context);
    return 0;
}
