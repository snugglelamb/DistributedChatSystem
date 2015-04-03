#include "ChatNode.h"
#include <iostream>

extern "C"
{
	#include "multicast.h"
}
void ChatNode::addUser(User user)
{
	this->userlist.push_back(user);
}
void ChatNode::deleteUser(User user)
{

}
int ChatNode::getSequenceNumber()
{
	return 1;
}
void ChatNode::setSequenceNumber(int sequenceNumber)
{
	this->sequenceNumber = sequenceNumber;
}
vector<User> ChatNode::getUserlist()
{
	return userlist;
}
void ChatNode::setUserlist(vector<User> list)
{
	this->userlist = list;
}
User ChatNode::getSequencer()
{
	return this->sequencer;
}
void ChatNode::setSequencer(User sequencer)
{
	this->sequencer = sequencer;
}
User ChatNode::getCurrentUser()
{
	return this->currentUser; 
}
void ChatNode::setCurrentUser(User currentUser)
{
	this->currentUser = currentUser;
}

bool ChatNode::canJoin(string ip, int port)
{
	/*
	for(vector<User>::iterator it=this->userlist.begin();it!= userlist.end();it++)
	{
		User curUser = *it;
		//bool equals = userEquals(curUser, user);
		string curIP = it->getIP();
		int curPort = it->getPort();
		if(curIP.compare(ip)==0 && port==curPort)
			return true;
	}
    return false;
    */
    return true;
}

void ChatNode::join(User user, string ip, int port)
{
	cout<<"ip:"<<ip<<";port="<<port<<endl;
	char* destIP = new char[ip.length() + 1]; 
	copy(ip.begin(), ip.end(), destIP);
	string userIP = user.getIP();
	string name = user.getNickname();
	int userPort = user.getPort();
	char* destUserIP = new char[userIP.length()+1];
	copy(userIP.begin(), userIP.end(), destUserIP);
	char* destUserName = new char[name.length()+1];
	copy(name.begin(), name.end(), destUserName);
	int ret = multiJoin(destIP, port, destUserIP, destUserName, userPort);
	/*
	if(ret==-1)
	{
		cout<<"Sorry, no chat is active on "<<ip<<":"<<port<<" , try again later."<<endl;
		cout<<"Bye."<<endl;
	}
	*/

}
bool ChatNode::userEquals(User user1, User user2)
{
	string IP1= user1.getIP();
	string IP2= user2.getIP();
	string name1 = user1.getNickname();
	string name2 = user2.getNickname();
	if(IP1.compare(IP2)==0 && name1.compare(name2)==0)
		return true;
	else
		return false;
}


void ChatNode::electLeader()
{

}
void ChatNode::getInput()
{

}
void ChatNode::requestSequenceNumber()
{

}
void ChatNode::send()
{

}
void ChatNode::receive()
{

}

void ChatNode::controlInitChat(User user)
{
	setSequencer(user);
	addUser(user);	
	setCurrentUser(user);
	string name = user.getNickname();
	string ip = user.getIP();
	int port = user.getPort();
	cout<<name<<" started a new chat, listening on "<<ip<<":"<<port<<endl;
	cout<<"Succeed, current users:"<<endl;
	showUserlist();
	cout<<"Waiting for others to join..."<<endl;
}

void ChatNode::controlJoin(User user, string ip, int port)
{
	cout<<"control join called. IP="<<ip<<";port="<<port<<endl;
	bool groupExist = canJoin(ip, port);
	if(groupExist==true)
	{
		userlist.push_back(user);
		for(vector<User>::iterator it=this->userlist.begin();it!= userlist.end();it++)
		{
			if(userEquals(currentUser,*it)!=true && userEquals(user,*it)!=true)
			{
				/*
				string curIp = it->getIP();
				char* destIP = new char[curIp.length() + 1]; 
				copy(curIp.begin(), curIp.end(), destIP);
				int curPort = it->getPort();
				vector<User> list = getUserlist();
				string listStr;
				for(vector<User>::iterator j=this->userlist.begin();j!= userlist.end();j++)
				{
					listStr = listStr + j->getIP();
					listStr = listStr + " ";
					listStr = listStr + j->getNickname();
					listStr = listStr + " ";
					char portArr[8];
    				sprintf(portArr,"%d",j->getPort());
    				string portStr(portArr);
					listStr = listStr + portStr;
				}
				char* destListStr = new char[listStr.length() + 1]; 
				copy(listStr.begin(), listStr.end(), destListStr);
				sendUserlist(destIP, curPort, destListStr);		
				*/
				string curIp = it->getIP();
				char* destIP = new char[curIp.length() + 1]; 
				copy(curIp.begin(), curIp.end(), destIP);
				int curPort = it->getPort();
				string newUser;
				newUser = newUser + user.getIP();
				newUser = newUser + " ";
				newUser = newUser + user.getNickname();
				newUser = newUser + " ";
				char portArr[8];
    			sprintf(portArr,"%d",it->getPort());
    			string portStr(portArr);
				newUser = newUser + portStr;
				char* destNewUser = new char[newUser.length() + 1]; 
				copy(newUser.begin(), newUser.end(), destNewUser);
				sendNewUser(destIP, curPort, destNewUser);		
			}
			if(userEquals(user, *it)==true)
			{
				string curIp = it->getIP();
				char* destIP = new char[curIp.length() + 1]; 
				copy(curIp.begin(), curIp.end(), destIP);
				int curPort = it->getPort();
				string sequencerStr;
				sequencerStr = sequencerStr + sequencer.getIP();
				sequencerStr = sequencerStr + " ";
				sequencerStr = sequencerStr + sequencer.getNickname();	
				sequencerStr = sequencerStr + " ";
				char portArr[8];
    			sprintf(portArr,"%d",it->getPort());
    			string portStr(portArr);
				sequencerStr = sequencerStr + portStr;
				//char* destNewUser = new char[sequencerStr.length() + 1]; 
				char* destSequencerStr= new char[sequencerStr.length() + 1]; 
				copy(sequencerStr.begin(), sequencerStr.end(), destSequencerStr);	

				vector<User> list = getUserlist();
				string listStr;
				for(vector<User>::iterator j=this->userlist.begin();j!= userlist.end();j++)
				{
					listStr = listStr + j->getIP();
					listStr = listStr + " ";
					listStr = listStr + j->getNickname();
					listStr = listStr + " ";
					char listArr[8];
    				sprintf(listArr,"%d",j->getPort());
    				string listStr(listArr);
					sequencerStr = sequencerStr + listStr;
				}
				char* destListStr = new char[listStr.length() + 1]; 
				copy(listStr.begin(), listStr.end(), destListStr);

				sendUserlist(destIP, curPort, destListStr);		
				sendSequencer(destIP, curPort, destSequencerStr);
				sendSequenceNumber(destIP, curPort, sequenceNumber);
			}
		}	
		string curIp = user.getIP();
		string name = user.getNickname();
		int port = user.getPort();
		cout<<name<<" joining a new chat on "<<endl;	
	}else
	{
		//sendNoGroup();
	}
}

void ChatNode::controlUserlist(vector<User> list)
{
	setUserlist(list);
}

void ChatNode::controlNewUser(User user)
{
	addUser(user);
	string ip = user.getIP();
	string name = user.getNickname();
	int port = user.getPort();
	cout<<"Notice "<<name<<" joined on "<<ip<<":"<<port<<endl;
}

void ChatNode::controlSequencer(User sequencer)
{
	setSequencer(sequencer);
}

void ChatNode::controlSequenceNumber(int sequenceNumber)
{
	setSequenceNumber(sequenceNumber);
}

void ChatNode::showUserlist()
{
	for(int i=0;i<userlist.size();i++)
	{
		if(userEquals(userlist[i],sequencer)==true)
			cout<<userlist[i].getNickname()<<" "<<userlist[i].getIP()<<":"<<userlist[i].getPort()<<" (Leader)"<<endl;
		else
			cout<<userlist[i].getNickname()<<" "<<userlist[i].getIP()<<":"<<userlist[i].getPort()<<endl;
	}
}

