#ifndef CHATNODE_H
#define CHATNODE_H
#include "User.h"
#include <vector>
#include <algorithm>
#include <mutex>
#include "multicast.h"
#include "HoldbackQueue.h"

using namespace std;

class ChatNode
{
private:
	ChatNode(){};
	static ChatNode* node;
	vector<User> userlist;
	User me;
	int rNum;
	int pNum;
	mutex userlistMutex;
	mutex meMutex;
	mutex rNumMutex;
	mutex totalMutex;
	
	HoldbackQueue holdback;

	char* str2cstr(string ori);
public:
	static ChatNode* getInstance();
	vector<User> getUserlist();
	void setUserlist(vector<User> userlist);
	User getMe();
	void setMe(User user);
	int getPNum();
	void setPNum(int number);
	int getRNum();
	void setRNum(int number);

	void createChat(User user);
	void showCurrentUser();
	void reqLeader(string Tip, int Tport);
	void sendLeader(string Tip, int Tport);
	void connectLeader(string Tip, int Tport);
	void updateUserlist(vector<User> vector);
	void addUser(string ip, string name, int port);
	void multicastUserlist();

	void sendMsg(string msg);
	void multicastMsg(string msg);
	
};

#endif
