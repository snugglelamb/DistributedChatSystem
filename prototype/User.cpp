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

int User::getID()
{
	return this->ID;
}
void User::setID(int ID)
{
	this->ID = ID;
}
int User::getTotal()
{
	return this->total;
}
void User::setTotal(int total)
{
	this->total = total;
}

bool User::getIsLeader()
{
	return this->leader;
}

void User::setIsLeader(bool leader)
{
	this->leader = leader;
}

User::User(string IP, string nickname, int port, int ID, int total, bool leader)
{
	this->IP = IP;
	this->nickname = nickname;
	this->port = port;
	this->ID = ID;
	this->total = total;
	this->leader = leader;
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