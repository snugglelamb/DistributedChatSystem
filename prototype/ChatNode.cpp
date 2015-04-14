#include "ChatNode.h"
#include <iostream>
#include <cstddef>

ChatNode* ChatNode::node = NULL;

ChatNode* ChatNode::getInstance() {
	if (!node)
		node = new ChatNode;
	return node;
}

vector<User> ChatNode::getUserlist() {
	return this->userlist;
}

void ChatNode::setUserlist(vector<User> userlist) {
	this->userlist = userlist;
}

User ChatNode::getMe() {
	return this->me;
}

void ChatNode::setMe(User user) {
	this->me = user;
}

int ChatNode::getPNum() {
	return this->pNum;
}

void ChatNode::setPNum(int number) {
	this->pNum = number;
}

int ChatNode::getRNum() {
	return this->rNum;
}

void ChatNode::setRNum(int number) {
	this->rNum = number;
}

void ChatNode::createChat(User user) {
	user.setTotal(0);
	user.setID(0);
	user.setIsLeader(true);
	userlist.push_back(user);
	setMe(user);
	pNum = 0;
	rNum = 0;
	showCurrentUser();
	cout << "Waiting for others to join ..." << endl;
}

void ChatNode::showCurrentUser() {
	for (User u : this->userlist) {
		bool isLeader = u.getIsLeader();
		string ip = u.getIP();
		string name = u.getNickname();
		int port = u.getPort();
		if (isLeader)
			cout << name << " " << ip << ":" << port << " (Leader)" << endl;
		else
			cout << name << " " << ip << ":" << port << endl;
	}
}

char* ChatNode::str2cstr(string str) {
	char *cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	return cstr;
}

// request leader information from other client using target IP and port.
void ChatNode::reqLeader(string Tip, int Tport) {
	string msg;
	string requestName;
	string content;
	requestName = "sendLeader";
	string SIP = me.getIP();
	string mePortArr = to_string(me.getPort());
	content = SIP + "_" + mePortArr;
	cout << "content" << content << endl;
	msg = requestName + "#" + content;
	stub_send(Tip.c_str(), to_string(Tport).c_str(), msg.c_str());
}

// send leader information back to new added client
void ChatNode::sendLeader(string Tip, int Tport) {
	string msg;
	string requestName;
	string content;

	string leaderIP;
	int leaderPort;
	for (int i = 0; i < userlist.size(); i++) {

		if (userlist[i].getIsLeader()) {
			leaderIP = userlist[i].getIP();
			leaderPort = userlist[i].getPort();
		}
	}
	requestName = "connectLeader";

	// cout<<"SIP"<<SIP<<endl;
	// cout<<"Sport"<<Sport<<endl;

	content = me.getIP() + "_" + to_string(me.getPort()) + "_" + leaderIP + "_"
			+ to_string(leaderPort);
	cout << "!!!!!!!!!!!!! " << content << endl;
	msg = requestName + "#" + content;

	stub_send(Tip.c_str(), to_string(Tport).c_str(), msg.c_str());
}

// connect to leader. add new client to userlist
void ChatNode::connectLeader(string Tip, int Tport) {
	string msg;
	string requestName;
	string content;

	requestName = "addUser";
	content = me.getIP() + "_" + to_string(me.getPort()) + "_" + me.getIP()
			+ "_" + me.getNickname() + "_" + to_string(me.getPort());
	msg = requestName + "#" + content;

	stub_send(Tip.c_str(), to_string(Tport).c_str(), msg.c_str());
}

//update userlist
void ChatNode::updateUserlist(vector<User> newuserlist) {
	//cout<<"in update userlist"<<endl;
	this->userlist = newuserlist;
	for (User u : this->userlist) {
		if (u.getIP() == me.getIP() && u.getPort() == me.getPort())
			me = u;
	}
	for (User u : this->userlist) {
		if (u.getIsLeader()) {
			//	cout<<"is leader"<<endl;
			rNum = u.getTotal();
			pNum = u.getTotal();
		}
	}
	showCurrentUser();
}

//add new user to userlist and then multicast new userlist to other clients
void ChatNode::addUser(string ip, string name, int port) {
	//cout<<"in add user"<<endl;
	User t;
	t.setIP(ip);
	t.setNickname(name);
	t.setPort(port);
	t.setID(this->userlist.size());
	t.setIsLeader(false);
	t.setTotal(me.getTotal());
	this->userlist.push_back(t);
	//showCurrentUser();
	cout << "NOTICE " << name << " joined on " << ip << ":" << port << endl;
	multicastUserlist();
}

//multicast new userlist to other clients
void ChatNode::multicastUserlist() {
	//cout<<"in multicast userlist"<<endl;
	string requestName = "updateUserlist";
	string msg;
	string content = "";
	//string IP, string nickname, int port, int ID, int total, bool isleader
	content = me.getIP() + "_" + to_string(me.getPort()) + "_";
	for (User u : this->userlist) {
		if (u.getIsLeader())
			u.setTotal(me.getTotal());
		content += u.getIP() + "_" + u.getNickname() + "_"
				+ to_string(u.getPort()) + "_" + to_string(u.getID()) + "_"
				+ to_string(u.getTotal()) + "_";
		if (u.getIsLeader())
			content += "1_";
		else
			content += "0_";
	}

	msg = requestName + "#" + content.substr(0, content.size() - 1);
	for (User u : this->userlist) {
		if (u.getIP() == me.getIP() && u.getPort() == me.getPort())
			continue;
		stub_send(u.getIP().c_str(), to_string(u.getPort()).c_str(),
				msg.c_str());

	}
	showCurrentUser();
}

void ChatNode::sendMsg(string message) {
	string requestName = "enqueueMsg";
	string msg;
	string content;

	content = me.getIP() + "_" + to_string(me.getPort()) + "_"
			+ me.getNickname() + "_" + message;
	string Tip;
	int Tport;

	if (me.getIsLeader()) {
		msgQueue.push(me.getNickname() + "_" + message);
	} else {
		for (User u : this->userlist) {
			if (u.getIsLeader()) {
				Tip = u.getIP();
				Tport = u.getPort();
			}
		}
		msg = requestName + "#" + content;

		stub_send(Tip.c_str(), to_string(Tport).c_str(), msg.c_str());
	}
}

// only called by leader object
void ChatNode::enqueueMsg(string msg) {
	msgQueue.push(msg);
}

void ChatNode::checkMsgQueue() {
	string item = (string) msgQueue.pop();
	multicastMsg(item);
}

void ChatNode::multicastMsg(string message) {
	cout<<"multicast msg!!!!!!!!!!!!!!!!!!!!!"<<endl;
	string requestName = "recMsg";
	string msg;
	//totalMutex.lock();
	string content = me.getIP() + "_" + to_string(me.getPort()) + "_"
			+ to_string(me.getTotal()) + "_";
	me.setTotal(me.getTotal() + 1);
	//totalMutex.unlock();
	msg = requestName + "#" + content + message;
	for (User u : userlist) {
		//if()
		stub_send(u.getIP().c_str(), to_string(u.getPort()).c_str(),
				msg.c_str());

	}
}

void ChatNode::recMsg(string name, int total, string msg) {
	if (total == rNum) {
		rNum++;
		showMsg(name, msg);
		while (!holdback.isEmpty()) {
			if (rNum != holdback.peek())
				break;
			else {
				int key = holdback.peek();
				string message = holdback.get(key);
				holdback.dequeue();
				rNum++;
				showMsg(name, message);
			}

		}
	} else {
		holdback.enqueue(total, msg);
	}
}

void ChatNode::showMsg(string name, string msg) {
	cout << name << "::" << msg << endl;
}

void ChatNode::userExit() {
	if (me.getIsLeader()) {
		for (vector<User>::iterator it = userlist.begin(); it != userlist.end();
				it++) {
			if (it->getIP() == me.getIP() && it->getPort() == me.getPort()) {
				userlist.erase(it);
				if (userlist.begin() != userlist.end()) {
					userlist.begin()->setIsLeader(true);
					this->multicastUserlist();
					return;
				}
				return;
			}
		}


	}
	string msg = "deleteUser#" + me.getIP() + "_" + to_string(me.getPort())
			+ "_" + me.getIP() + "_" + to_string(me.getPort());
	string Tip;
	int Tport;
	for (User u : this->userlist) {
		if (u.getIsLeader()) {
			Tip = u.getIP();
			Tport = u.getPort();
		}
	}
	stub_send(Tip.c_str(), to_string(Tport).c_str(), msg.c_str());
}

void ChatNode::deleteUser(string Tip, int Tport) {

	for (vector<User>::iterator it = this->userlist.begin();
			it != this->userlist.end(); it++) {
		if (it->getIP() == Tip && it->getPort() == Tport) {
			this->userlist.erase(it);
			break;
		}
	}
	this->multicastUserlist();
}
