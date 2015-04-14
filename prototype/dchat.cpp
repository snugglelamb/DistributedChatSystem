#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
// #include "User.h"
// #include "ChatNode.h"
#include "util.h"
#include <string.h>

#include "multicast.h"


using namespace std;

// for test purpose, type msgs are all sent to creator
static char test_port[10];

ChatNode *node = ChatNode::getInstance();

void leaderMsgSend()
{
	while(1)
	{
		node->checkMsgQueue();
	}
}


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
			cout << "Hint: blank messages won't be sent.\n";

		} else {
			strcpy(ip, "127.0.0.1");
			strcpy(port_, test_port);
			// strcpy(port_,"20000");
			// call stub_send
			//stub_send(ip, port_, msg);
			node->sendMsg(string(msg));
		}
	}
}

void check()
{
	// heart beat check alive
}

int main(int argc, char** argv)
{

	if(argc == 2)
	{
		string name(argv[1]);
	//	std::cout << "Create Chat, name: "<< name << endl;
		// create

		string handle = string(stub_create());
		cout<<"create chat:"<<handle<<endl;
		while ( handle.compare("CREATEERROR") == 0 ) 
		{
			// ERROR
			cout << "Met error, could not create chat.\n";
			handle = string(stub_create());
		} 

		size_t pos = handle.find(":");
		if (pos == string::npos)
		{
			cout<< "argc 2 the pos is npos" <<endl;
			cout << "please enter address as 0.0.0.0:1234" << endl;
			exit(1);
		} 

		char *ip = (char *) handle.substr(0, pos).c_str();		
		char *port = (char *) handle.substr(pos+1, -1).c_str();

		int leaderPort = atoi(port); 

		strcpy(test_port, port);
		User user(string(ip), name, leaderPort);
		node->createChat(user);
		cout<<name<<" started a new chat, listening on "<<string(ip)<<":"<<string(port)<<endl;
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
		
		//std::cout << name << " attempts to join chat at "<< addr << endl;
		//char *tip = addr.substr(0, pos).c_str();
		//char *tport = addr.substr(pos+1, -1).c_str();
		

		//strcpy(test_port, tport);
		
		string handle = stub_connect(addr.substr(0, pos).c_str(), addr.substr(pos+1, -1).c_str());
		cout<<"handle:"<<handle<<endl;
		if ( handle.compare("ERROR") == 0 ) 
		{
			//std::cout << "Sorry, no chat is active on "<<string(tip)<<":"<<string(tport)<<", try again later"<<endl;
			exit(1);
		}
		while(handle.compare("CREATEERROR") == 0)
		{
			handle = string(stub_connect(addr.substr(0, pos).c_str(), addr.substr(pos+1, -1).c_str()));
		}
		size_t posTarget = handle.find(":");
		if (pos == string::npos)
		{
			cout << "please enter address as 0.0.0.0:1234" << endl;
			exit(1);
		} 

		//char *sip = (char *) handle.substr(0, posTarget).c_str();
		//char *sport = (char *) handle.substr(posTarget+1, -1).c_str();

		int selfPort = atoi(handle.substr(posTarget+1, -1).c_str());
	
		User user(handle.substr(0, posTarget), name, selfPort);
		node->setMe(user);
		//cout<<name<<" joining a new chat on "<<string(tip)<<":"<<string(tport)<<", listening on"<<endl;
		cout<<handle.substr(0, posTarget).c_str()<<":"<<handle.substr(posTarget+1, -1).c_str()<<endl;
		cout<<"Succeeded, current users:"<<endl;
		node->reqLeader(addr.substr(0, pos), atoi(addr.substr(pos+1, -1).c_str()));

	} else {
		cout << "[./dchat Bob] or [./dchat Alice 192.168.1.101:3000]\n";
		exit(1);
	}
	
	
	// three thread1: receive; thread2: type; thread3: check
	std::thread main_receive(receive);
	std::thread main_type(type);
	std::thread main_check(check);
	std::thread main_leaderMsgSend(leaderMsgSend);

	
	cout << "threads started.\n";

	
	main_receive.join();
	main_type.join();
	main_check.join();
	main_leaderMsgSend.join();
	
	cout << "threads finished.\n";
	
	return 0;
		
}



