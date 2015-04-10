#include "ChatNode.h"
#include <iostream>
#include <cstddef>

extern "C"
{
	#include "multicast.h"
}

ChatNode* ChatNode::node = NULL;

ChatNode* ChatNode::getInstance()
{
	if(!node)
		node = new ChatNode;
	return node;
}

vector<User> ChatNode::getUserlist()
{
	return this->userlist;
}

void ChatNode::setUserlist(vector<User> userlist)
{
	this->userlist = userlist;
}

User ChatNode::getMe()
{
	return this->me;
}

void ChatNode::setMe(User user)
{
	this->me = user;
}

int ChatNode::getPNum()
{
	return this->pNum;
}

void ChatNode::setPNum(int number)
{
	this->pNum = number;
}

int ChatNode::getRNum()
{
	return this->rNum;
}

void ChatNode::setRNum(int number)
{
	this->rNum = number;
}

void ChatNode::createChat(User user)
{
	userlist.push_back(user);
	setMe(user);
	pNum = 0;
	rNum = 0;
}


// request leader information from other client using target IP and port.
void ChatNode::reqLeader(string Tip, int Tport)
{
	string msg;
	string requestName;
	string content;
	requestName = "sendLeader";
	msg= requestName + "#" +content;
	stub_send(Tip.c_str(), to_string(Tport).c_str(), msg.c_str());
}

// send leader information back to new added client
void ChatNode::sendLeader(string Tip, int Tport)
{
	string msg;
	string requestName;
	string content;

	string leaderIP;
	int leaderPort;
	for(int i=0;i<userlist.size();i++)
	{
		User user = userlist[i];
		if(user.getIsLeader())
		{
			leaderIP = user.getIP();
			leaderPort = user.getPort();
		}
	}
	requestName = "connectLeader";

	char leaderPortArr[5];
    sprintf(leaderPortArr,"%d",leaderPort);
    string portStr(leaderPortArr);

    string SIP = me.getIP();
    int Sport = me.getPort();

    char mePortArr[5];
    sprintf(mePortArr,"%d",Sport);
    string mePortStr(mePortArr);

    content = SIP + "_" + mePortArr + "_" +leaderIP + "_" + leaderPortArr;
	msg = requestName + "#" + content;
	stub_send(Tip.c_str(), to_String(Tport).c_str(), msg.c_str());
}

// connect to leader. add new client to userlist
void ChatNode::connectLeader(string Tip, int Tport)
{
	string msg;
	string requestName;
	string content;

	string SIP = me.getIP();
	string Sname = me.getNickname();
	int Sport = me.getPort();

	char mePortArr[5];
    sprintf(mePortArr,"%d",Sport);
    string mePortStr(mePortArr);

    requestName = "addUser";	
    content = SIP + "_" + mePortStr + "_" + SIP + "_" + Sname + "_"+ mePortStr ;
    msg = requestName + "#" + content;
    stub_send(Tip.c_tr(), to_string(Tport).c_str(), msg.c_str());
}

//update userlist
void ChatNode::updateUserlist(vector<User> newuserlist)
{
	this->userlist = newuserlist;
	for(User u: this->userlist){
		if(u.getIP() == me.getIP() && u.getPort() == me.getPort())
			me = u;
	}
	int proposedNumber;
	int receivedNumber;
	for(User u: this->userlist)
	{
		if(u.getIsLeader()){
			proposedNumber = u.getTotal();
			receivedNumber = u.getTotal();
		}
	}
}

//add new user to userlist and then multicast new userlist to other clients
void ChatNode::addUser(string ip, string name, int port)
{
	User t;
	t.setIP(ip);
	t.setNickname(name);
	t.setPort(port);
	t.setID(this->userlist.size());
	t.setIsLeader(false);
	t.setTotal(me.getTotal());
	this->userlist.push_back(t);
	multicastUserlist();
}


//multicast new userlist to other clients
void ChatNode::multicastUserlist()
{
	string requestName ="updateUserlist";
	string msg;
	string content = "";
	//string IP, string nickname, int port, int ID, int total, bool isleader
	content = me.getIP()+"_"+to_string(me.getPort())+"_";
	for(User u: this->userlist){
		content += u.getIP()+"_"+u.getNickname()+"_"+to_string(u.getPort())+"_"+to_string(u.getID())+"_"+to_string(u.getTotal())+"_";
		if(u.getIsLeader()) content += "1";
		else content += "0";
	}
	msg = requestName + "#" + content;
	for(User u: this->userlist){
		if(me.getIsLeader()) continue;
		stub_send(u.getIP().c_str(), to_String(u.getPort()).c_str(), msg.c_str());
	}
}

void ChatNode::sendMsg(string message)
{
	string requestName = "multicastMsg";
	string msg;
	string content;
	content = me.getIP() + "_" + to_string(me.getPort()) + "_" + message;

	string Tip;
	int Tport;
  
  	if(me.getIsLeader())
  	{
  		multicastMsg(message);
  	}else
  	{
  		for(User u: this->userlist){
			if(u.getIsLeader())
			{
				Tip = u.getIP();
				Tport = u.getPort();
			}
		}
		msg = requestName + "#" + content;
    	stub_send(Tip.c_str(), to_string(Tport).c_str(), msg.c_str());
  	}
}

void ChatNode::multicastMsg(string message){
	string requestName = "recMsg";
	string msg;
	totalMutex.lock();
	string content = me.getIP()+"_"+to_string(me.getPort())+"_" + to_string(me.getTotal())+"_";
	me.setTotal(me.getTotal()+1)
	totalMutex.unlock();
	msg = requestName+"#"+content+message;
	for(User u: userlist){
		stub_send(u.getIP().c_str(), to_string(u.getPort()).c_str(), msg.c_str());
	}
	

}




/*
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
    return true;
}

void ChatNode::join(User user, string ip, int port)
{
	// send request to <ip port>
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
*/
