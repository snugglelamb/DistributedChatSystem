#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
// #include "User.h"
// #include "ChatNode.h"
#include "multicast.h"

extern "C"
{
	#include "util.h"
	#include <string.h>
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
		char* msg = new char[512];
		cin.getline (msg, 512);
		if (strlen(msg) == 0)
		{
			// won't send blank message
			cout << "Hint: blank messages won't be sent.\n";

		} else {
			const char* msg_ = msg;
			const char ip[] = "127.0.0.1";
			const char* port_ = test_port;
			// strcpy(ip, "127.0.0.1");
			// strcpy(port_, test_port);
			// strcpy(port_,"20000");
			// call stub_send
			stub_send(ip, port_, msg_, 0);
			printf("main: %d messages left in send queue.\n", sendQ_num);
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
		cout << "Create Chat, name: "<< name << endl;
		// create
		string handle = stub_create();
		if ( handle.compare("ERROR") == 0 ) 
		{
			// ERROR
			cout << "Met error, could not create chat.\n";
			exit(1);
		} else {
			size_t pos = handle.find(":");
			if (pos == string::npos)
			{
				cout << "please enter address as 0.0.0.0:1234" << endl;
				exit(1);
			} 

			string ip = handle.substr(0, pos);
			string port = handle.substr(pos+1, -1);
			strcpy(test_port, port.c_str());
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
		
		cout << name << " attempts to join chat at "<< addr << endl;
		const char* ip =  addr.substr(0, pos).c_str();		
		const char* port = addr.substr(pos+1, -1).c_str();
		
		strcpy(test_port, port);
		
		string handle = stub_connect(ip, port);
		if ( handle.compare("ERROR") == 0 ) 
		{
			// ERROR
			cout << "Met error, could not join chat.\n";
			exit(1);
		}
		
	} else {
		cout << "[./dchat Bob] or [./dchat Alice 192.168.1.101:3000]\n";
		exit(1);
	}
	
	
	// three thread1: receive; thread2: type; thread3: check
	std::thread main_receive(receive);
	std::thread main_type(type);
	std::thread main_check(check);
	
	cout << "threads started.\n";
	
	main_receive.join();
	main_type.join();
	main_check.join();
	
	std:cout << "threads finished.\n";
	
	return 0;
		
}



