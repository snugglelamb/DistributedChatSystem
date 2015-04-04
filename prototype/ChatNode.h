#ifndef CHATNODE_H
#define CHATNODE_H

#include "User.h"
#include <vector>
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
public:
	static ChatNode* getInstance();
	vector<User> getUserlist();
	void setUserlist(Vector<User> vector);
	User getMe();
	void setMe(User user);
	int getProposedNumber();
	void setProposedNumber(int number);
	int getReceivedNumber();
	void setReceivedNumber(int number);

	void createChat(User user);
	void reqLeader(int Tip, int Tport);
	void connectLeader(int Tip, int port);
	void updateUserlist(vector<User> vector);
	void addUser(string ip, string name, int port);
	void multicastUserlist();

};

#endif