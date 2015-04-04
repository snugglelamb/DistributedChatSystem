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
	bool isLeader;
public:
	string getIP();
	void setIP(string IP);
	string getNickname();
	void setNickName(string nickname);
	int getPort();
	void setPort(int port);
	int getID();
	void setID(int ID);
	int getTotal();
	void setTotal(int total);
	bool isLeader();
	void setIsLeader(bool isLeader);
	User(string IP, string nickname, int port, int ID, int total, bool isLeader);
	User(string IP, string nickname, int port);
	User();
};

#endif