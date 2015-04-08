#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
// #include "User.h"
// #include "ChatNode.h"
#include "util.h"
#include <string.h>

extern "C"
{
	#include "multicast.h"
}

using namespace std;

// for test purpose, type msgs are all sent to creator
static char test_port[10];

void receive()
{
	// call stub_receive
	while(1)
	{
		stub_receive();
	}
}

void type()
{
	while(1)
	{
		// call ChatNode to parse down msg
		char msg[512];
		char ip[20], port_[20];
		std::cin.getline (msg, 512);
		if (strlen(msg) == 0)
		{
			// won't send blank message
			std::cout << "Hint: blank messages won't be sent.\n";

		} else {
			strcpy(ip, "127.0.0.1");
			strcpy(port_, test_port);
			// strcpy(port_,"20000");
			// call stub_send
			stub_send(ip, port_, msg);
		}
	}
}

void check()
{
	// heart beat check alive
}

int main(int argc, char** argv)
{
	// ChatNode node;
	if(argc == 2)
	{
		string name(argv[1]);
		std::cout << "Create Chat, name: "<< name << endl;
		// create
		string handle = string(stub_create());
		if ( handle.compare("ERROR") == 0 ) 
		{
			// ERROR
			std::cout << "Met error, could not create chat.\n";
			exit(1);
		} else {
			size_t pos = handle.find(":");
			if (pos == string::npos)
			{
				cout << "please enter address as 0.0.0.0:1234" << endl;
				exit(1);
			} 

			char *ip = (char *) handle.substr(0, pos).c_str();		
			char *port = (char *) handle.substr(pos+1, -1).c_str();
			strcpy(test_port, port);
		}
		
	}else if(argc == 3)
	{
		string name(argv[1]);
		string addr(argv[2]);
		// string ip, port;
		size_t pos = addr.find(":");
		if (pos == string::npos)
		{
			cout << "please enter address as 0.0.0.0:1234" << endl;
			exit(1);
		} 
		
		std::cout << name << " attempts to join chat at "<< addr << endl;
		char *ip = (char *) addr.substr(0, pos).c_str();		
		char *port = (char *) addr.substr(pos+1, -1).c_str();
		
		strcpy(test_port, port);
		
		string handle = string(stub_connect(ip, port));
		if ( handle.compare("ERROR") == 0 ) 
		{
			// ERROR
			std::cout << "Met error, could not join chat.\n";
			exit(1);
		}
		
	} else {
		std::cout << "[./dchat Bob] or [./dchat Alice 192.168.1.101:3000]\n";
		exit(1);
	}
	
	
	// three thread1: receive; thread2: type; thread3: check
	std::thread main_receive(receive);
	std::thread main_type(type);
	std::thread main_check(check);
	
	std::cout << "threads started.\n";
	
	main_receive.join();
	main_type.join();
	main_check.join();
	
	std:cout << "threads finished.\n";
	
	return 0;
		
}


