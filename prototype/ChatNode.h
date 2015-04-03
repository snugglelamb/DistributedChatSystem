#ifndef CHATNODE_H
#define CHATNODE_H

#include "User.h"
#include <vector>
using namespace std;

class ChatNode
{
private:
	int sequenceNumber;
	vector<User> userlist;
	User sequencer;
	User currentUser;
public:
	void addUser(User user);
	void deleteUser(User user);

	int getSequenceNumber();
	void setSequenceNumber(int sequenceNumber);

	vector<User> getUserlist();
	void setUserlist(vector<User> list);
	User getSequencer();
	void setSequencer(User sequencer);
	User getCurrentUser();
	void setCurrentUser(User currentUser);

	bool canJoin(string ip, int port);
	void join(User user, string ip, int port);
	bool userEquals(User user1, User user2);


	void electLeader();
	void getInput();
	void requestSequenceNumber();
	void send();
	void receive();


	void controlInitChat(User user);
	void controlJoin(User user, string ip, int port);
	void controlUserlist(vector<User> list);
	void controlNewUser(User user);
	void controlSequencer(User sequencer);
	void controlSequenceNumber(int sequenceNumber);

	void showUserlist();
};

#endif