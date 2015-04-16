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
	stub_send(Tip.c_str(), to_string(Tport).c_str(), msg.c_str(), 0);
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
	msg = requestName + "#" + content;

	stub_send(Tip.c_str(), to_string(Tport).c_str(), msg.c_str(), 0);
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
	if(stub_send(Tip.c_str(), to_string(Tport).c_str(), msg.c_str(), 0) == "ERROR"){
		cout << "You can't connect with leader" <<endl;
		exit(1);
	}
}

//update userlist
void ChatNode::updateUserlist(vector<User> newuserlist) {
	//cout<<"in update userlist"<<endl;
	participant = false;
	userlistMutex.lock();
	this->userlist = newuserlist;
	userlistMutex.unlock();

	for (int i = 0; i < userlist.size(); i++) {
		if (userlist[i].getIP() == me.getIP()
				&& userlist[i].getPort() == me.getPort()) {
			me = userlist[i];
			me.setID(i);
		}
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
	userlistMutex.lock();
	this->userlist.push_back(t);
	userlistMutex.unlock();
	//showCurrentUser();
	cout << "NOTICE " << name << " joined on " << ip << ":" << port << endl;
	multicastUserlist();
}

//multicast new userlist to other clients
void ChatNode::multicastUserlist() {
	cout << "!!!! in multi cast userlist" << endl;
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
				msg.c_str(), 0);

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

		stub_send(Tip.c_str(), to_string(Tport).c_str(), msg.c_str(), 0);
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
				msg.c_str(), 0);

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
				userlistMutex.lock();
				userlist.erase(it);
				if (userlist.begin() != userlist.end()) {
					userlist.begin()->setIsLeader(true);
					this->multicastUserlist();
				}
				userlistMutex.unlock();
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
	stub_send(Tip.c_str(), to_string(Tport).c_str(), msg.c_str(), 0);
}

void ChatNode::deleteUser(string Tip, int Tport) {

	for (vector<User>::iterator it = this->userlist.begin();
			it != this->userlist.end(); it++) {
		if (it->getIP() == Tip && it->getPort() == Tport) {
			userlistMutex.lock();
			this->userlist.erase(it);
			userlistMutex.unlock();
			break;
		}
	}
	this->multicastUserlist();
}

void ChatNode::leaderElection() {
	string result;
	if (!participant) {
		string cnt = "sendUID#" + me.getIP() + "_" + to_string(me.getPort())
				+ "_" + to_string(me.getID());
		int nextidx = (me.getID() + 1) % userlist.size();
		while (true) {
			if (userlist[nextidx].getIsLeader()) {
				nextidx = (++nextidx) % userlist.size();
			}
			result = stub_send(userlist[nextidx].getIP().c_str(),
					to_string(userlist[nextidx].getPort()).c_str(), "00006C",
					0);
			if (result == "ERROR")
				nextidx = (++nextidx) % userlist.size();
			else
				break;
		}

		stub_send(userlist[nextidx].getIP().c_str(),
				to_string(userlist[nextidx].getPort()).c_str(), cnt.c_str(), 0);
	}

}

void ChatNode::sendUID(int id) {
	cout<<"sendUID called"<<endl;
	int proposeID;
	string result;
	if (participant && id > me.getID()) {
		return;
	}
	if (!participant) {
		participant = true;
		proposeID = id > me.getID() ? me.getID() : id;
		int nextidx = (me.getID() + 1) % userlist.size();
		string cnt = "sendUID#" + me.getIP() + "_" + to_string(me.getPort())
				+ "_" + to_string(proposeID);
		while (true) {
			if (userlist[nextidx].getIsLeader()) {
				nextidx = (++nextidx) % userlist.size();
			}
			result = stub_send(userlist[nextidx].getIP().c_str(),
					to_string(userlist[nextidx].getPort()).c_str(), "00006C",
					0);
			if (result == "ERROR")
				nextidx = (++nextidx) % userlist.size();
			else
				break;
		}
		stub_send(userlist[nextidx].getIP().c_str(),
				to_string(userlist[nextidx].getPort()).c_str(), cnt.c_str(), 0);
	}
	else if (participant && id == me.getID()) {


		userlistMutex.lock();
		int total;
		for(vector<User>::iterator it = userlist.begin(); it != userlist.end(); it++){
			if(it->getIsLeader()) {
				total = it->getTotal();
				userlist.erase(it--);
				continue;
			}
			if(it->getID() == me.getID()){
				it->setTotal(total);
				it->setIsLeader(true);
			}
		}
		userlistMutex.unlock();
		multicastUserlist();
	}

}
void ChatNode::checkAlive() {
	cout<<"check alive called"<<endl;
	bool change = false;
	string result;
	if (me.getIsLeader()) {

		for (vector<User>::iterator it = userlist.begin(); it != userlist.end();
				it++) {
			if (it->getIP() == me.getIP() && it->getPort() == me.getPort())
				continue;


			result = stub_send(it->getIP().c_str(),
					to_string(it->getPort()).c_str(), "00006C", 3);
			cout << "leader result: " << result << endl;
			if (result == "ERROR") {
				userlistMutex.lock();
				cout << "!!!!!inlock!!!!" << endl;
				change = true;
				userlist.erase(it--);
				userlistMutex.unlock();
				cout << "!!! out of lock !!!" << endl;
			}
		}
		if (change)
			multicastUserlist();
	} else {
		string ip = "";
		string port = "";
		for (User u : userlist) {
			if (u.getIsLeader()) {
				ip = u.getIP();
				port = to_string(u.getPort());
				break;
			}
		}

		if (ip.length() != 0 && port.length() != 0) {
			result = stub_send(ip.c_str(), port.c_str(), "00006C", 3);
			cout << "user result:" << result << endl;
			if (result == "ERROR") {
				leaderElection();
			}
		}
	}

}
