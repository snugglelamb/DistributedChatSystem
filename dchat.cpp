#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "User.h"
#include "ChatNode.h"
#include "util.h"

extern "C"
{
	#include "multicast.h"
}

using namespace std;

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

	for(;;)
	{
		User user = node.getCurrentUser();
		int port = user.getPort();
		cout<<"listening on port:"<<port<<endl;
		string response(onReceive(port));
		//cout<<"response:"<<response<<endl;

    	string buf;
    	stringstream ss(response); 
    	vector<string> tokens; 
		while (ss >> buf)
        	tokens.push_back(buf);

        if(tokens[0].compare("JOIN")==0)
        {
        	cout<<"join msg"<<endl;
        	string ip(tokens[1]);
        	string name(tokens[2]);
        	string portStr(tokens[3]);
    		int port = atoi(portStr.c_str()); 
        	User user(ip,name,port);
        	node.controlJoin(user, ip, port);
        }else if(tokens[0].compare("NEWUSER"))
        {
        	cout<<"newuser msg"<<endl;
    		string ip(tokens[1]);
    		string name(tokens[2]);
    		string portStr(tokens[3]);
    		int port = atoi(portStr.c_str()); 
    		User newUser(ip,name,port);
        	node.controlNewUser(newUser);
        }else if(tokens[0].compare("USERLIST"))
        {
        	cout<<"userlist msg"<<endl;
        	vector<User> list;
        	for(int j=1; j<tokens.size();j=j+3)
        	{
        		string ip(tokens[j]);
        		string name(tokens[j+1]);
        		string portStr(tokens[j+2]);
        		int port = atoi(portStr.c_str());
        		User newUser(ip,name,port);
        		list.push_back(newUser);
        	}
        	node.controlUserlist(list);
        }else if(tokens[0].compare("SEQUENCER"))
        {
        	cout<<"sequencer msg"<<endl;
        	string ip(tokens[1]);
        	string name(tokens[2]);
        	string portStr(tokens[3]);
    		int port = atoi(portStr.c_str()); 
        	User sequencer(ip,name,port);
        	node.controlSequencer(sequencer);
        }else if(tokens[0].compare("SEQUENCENUMBER"))
        {
        	cout<<"sequence number msg"<<endl;
        	string numStr(tokens[2]);
        	int sequenceNumber = atoi(numStr.c_str()); 
        	node.controlSequenceNumber(sequenceNumber);
        }

    }
}



