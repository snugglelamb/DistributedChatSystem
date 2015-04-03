#include "User.h"

string User::getIP()
{
	return IP;
}

string User::getNickname()
{
	return nickname;
}

int User::getPort()
{
	return port;
}

User::User(string IP, string nickname)
{
	this->IP = IP;
	this->nickname = nickname;
}

User::User(string IP, string nickname, int port)
{
	this->IP = IP;
	this->nickname = nickname;
	this->port = port;
}

User::User()
{

}