#include "User.h"

string User::getIP()
{
	return this->IP;
}

void User::setIP(string IP)
{
	this->IP = IP;
}

string User::getNickname()
{
	return this->nickname;
}

void User::setNickname(string nickname)
{
	this->nickname = nickname;
}

int User::getPort()
{
	return this->port;
}

void User::setPort(int port)
{
	this->port = port;
}

int getID()
{
	return this->ID;
}
void setID(int ID)
{
	this->ID = ID;
}
int getTotal()
{
	return this->total;
}
void setTotal(int total)
{
	this->total = total;
}

User::User(string IP, string nickname, int port, int ID, int total)
{
	this->IP = IP;
	this->nickname = nickname;
	this->port = port;
	this->ID = ID;
	this->total = total;
}

User::User()
{

}