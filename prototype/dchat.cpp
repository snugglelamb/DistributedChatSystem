#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include "User.h"
#include "ChatNode.h"
#include "util.h"

extern "C"
{
	#include "multicast.h"
}

using namespace std;

void receive()
{
	// call stub_receive
	stub_receive();
}

void type()
{
	// call ChatNode to parse down msg
	
	// call stub_send
	stub_send(string Tip, string Tport, string msg);
}

void check()
{
	// heart beat check alive
}

int main(int argc, char** argv)
{
	ChatNode node;
	if(argc == 2)
	{
		string name(argv[1]);
		string ip = "127.0.0.1";
		int port = randomPort();
		User user(ip,name,port);
		node.controlInitChat(user);	
	}else if(argc == 3)
	{
		string name(argv[1]);
		string address(argv[2]);
		vector<string> strings;
		istringstream f(address);
		string ownIP = "127.0.0.1";
		string ip;
		string portStr; 
    	getline(f, ip, ':');
    	getline(f, portStr, ':');
		int port = atoi(portStr.c_str()); 
		int ownPort = randomPort();
		User user(ownIP, name, ownPort);
        node.setCurrentUser(user);
		node.join(user, ip, port);
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



