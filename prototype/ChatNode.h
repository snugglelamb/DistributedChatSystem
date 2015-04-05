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
	int proposedNumber;
	int receivedNumber;
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
	int getProposedNumber();
	void setProposedNumber(int number);
	int getReceivedNumber();
	void setReceivedNumber(int number);

	void createChat(User user);
	void reqLeader(string Tip, int Tport);
	void sendLeader(string Tip, int Tport);
	void connectLeader(string Tip, int Tport);
	void updateUserlist(vector<User> vector);
	void addUser(string ip, string name, int port);
	void multicastUserlist();

	
};

#endif