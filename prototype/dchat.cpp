#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
// #include "User.h"
// #include "ChatNode.h"
#include "util.h"
#include <string.h>
#include <chrono>

#include "multicast.h"


using namespace std;

ChatNode *node = ChatNode::getInstance();

void leaderMsgSend()
{
	while(1)
	{
		//this_thread::sleep_for(chrono::seconds(5));
		//cout<<"hello"<<endl;
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

void checkParser()
{
	while(1)
	{
		String request = Parser.dequeueRequest();
		Parser.processReq(request);
	}
}


void type()
{
	while(1)
	{
		// call ChatNode to parse down msg
		  if (cin.eof()){ //checks for Control-D / EOF
			  // node method
			  node->userExit();
			  exit(1);
		  }

		char msg[512];

		std::cin.getline (msg, 512);
		if (strlen(msg) == 0)
		{
			// won't send blank message
			cout << "Hint: blank messages won't be sent.\n";

		} else {

			node->sendMsg(string(msg));
		}
	}
}

void check()
{
	// heart beat check alive
	
	while(1){
		this_thread::sleep_for (chrono::seconds(10));
		node->checkAlive();

	}

}

int main(int argc, char** argv)
{

	if(argc == 2)
	{
		string name(argv[1]);
	//	std::cout << "Create Chat, name: "<< name << endl;
		// create

		string handle = stub_create();
		cout<<"create chat:"<<handle<<endl;
		while ( handle.compare("CREATEERROR") == 0 ) 
		{
			// ERROR
			cout << "Met error, could not create chat.\n";
			handle = stub_create();
		} 

		size_t pos = handle.find(":");
		if (pos == string::npos)
		{
			cout<< "argc 2 the pos is npos" <<endl;
			cout << "please enter address as 0.0.0.0:1234" << endl;
			exit(1);
		} 

		string ip = handle.substr(0, pos);		
		string port = handle.substr(pos+1, -1);

		int leaderPort = stoi(port); 
		User user(ip, name, leaderPort);
		node->createChat(user);
		cout<<name<<" started a new chat, listening on "<<ip<<":"<<port<<endl;
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
		
		string handle = stub_connect(addr.substr(0, pos).c_str(), addr.substr(pos+1, -1).c_str());
		//cout<<"handle:"<<handle<<endl;
		if ( handle.compare("ERROR") == 0 ) 
		{
			std::cout << "Sorry, no chat is active on "<<addr<<", try again later"<<endl;
			exit(1);
		}
		while(handle.compare("CREATEERROR") == 0)
		{

			handle = stub_connect(addr.substr(0, pos).c_str(), addr.substr(pos+1, -1).c_str());
			if(handle == "ERROR")
				exit(1);
		}
		size_t posTarget = handle.find(":");
		if (pos == string::npos)
		{
			cout << "please enter address as 0.0.0.0:1234" << endl;
			exit(1);
		} 

		//char *sip = (char *) handle.substr(0, posTarget).c_str();
		//char *sport = (char *) handle.substr(posTarget+1, -1).c_str();

		int selfPort = stoi(handle.substr(posTarget+1, -1));
	
		User user(handle.substr(0, posTarget), name, selfPort);
		node->setMe(user);

		//cout<<name<<" joining a new chat on "<<string(tip)<<":"<<string(tport)<<", listening on"<<endl;
	//	cout<<handle.substr(0, posTarget)<<":"<<handle.substr(posTarget+1, -1)<<endl;
		cout<<name<<" joining a new chat on "<<addr<<", listening on \n"
			<<handle.substr(0, posTarget)<<":"<<handle.substr(posTarget+1, -1)<<endl;
		//cout<<handle.substr(0, posTarget).c_str()<<":"<<handle.substr(posTarget+1, -1).c_str()<<endl;

		cout<<"Succeeded, current users:"<<endl;
		node->reqLeader(addr.substr(0, pos), stoi(addr.substr(pos+1, -1)));

	} else {
		cout << "[./dchat Bob] or [./dchat Alice 192.168.1.101:3000]\n";
		exit(1);
	}
	
	
	// three thread1: receive; thread2: type; thread3: check
	std::thread main_receive(receive);
	std::thread main_type(type);
	std::thread main_check(check);
	std::thread main_leaderMsgSend(leaderMsgSend);
	std::thread main_checkParser(checkParser);

	
	//cout << "threads started.\n";

	
	main_receive.join();
	main_type.join();
	main_check.join();
	main_leaderMsgSend.join();
	main_checkParser.join();
	
	//cout << "threads finished.\n";
	
	return 0;
		
}



