#ifndef USER_H
#define USER_H

#include <string>

using namespace std;

class User
{
private:
	string IP; 
	string nickname; 
	int port;
	int ID;
	int total;
	int nextID;
	bool leader;
public:
	string getIP();
	void setIP(string IP);
	string getNickname();
	void setNickname(string nickname);
	int getPort();
	void setPort(int port);
	int getID();
	void setID(int ID);
	int getTotal();
	int getNextID();
	void setTotal(int total);
	void setNextID(int next);
	bool getIsLeader();
	void setIsLeader(bool isLeader);
	User(string IP, string nickname, int port, int ID, int total, bool isLeader);
	User(string IP, string nickname, int port);
	User();
};

#endif
