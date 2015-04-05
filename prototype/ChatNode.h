#ifndef CHATNODE_H
#define CHATNODE_H
#include "User.h"
#include <vector>
#include <queue>
using namespace std;

class ChatNode
{
private:
	ChatNode(){};
	static ChatNode* node;
	vector<User> userlist;
	User me;
	int pNum;
	int rNum;
	mutex userlistMutex;
	mutex meMutex;
	mutex pNumMutex;
	mutex rNumMutex;

	struct Compare{
		bool operator() (int a, int b){
			return a > b;
		}
	};
	priority_queue<int, vector<int>, Compare> holdback;

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
	void reqLeader(string Tip, int Tport);
	void sendLeader(string Tip, int Tport);
	void connectLeader(string Tip, int Tport);
	void updateUserlist(vector<User> vector);
	void addUser(string ip, string name, int port);
	void multicastUserlist();

	void sendMsg(string msg);
	void multicastMsg(string msg, int total);
	
};

#endif