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
public:
	string getIP();
	string getNickname();
	int getPort();
	User(string IP, string nickname);
	User(string IP, string nickname, int port);
	User();
};

#endif