#ifndef MULTICAST_H
#define MULTICAST_H

#include <string>
#include <iostream>
#include <vector>
#include "parser.h"
extern "C"
{
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <errno.h>
	#include <netdb.h>
	#include "util.h"
	#include <ifaddrs.h>
	#include <net/if.h>
}

extern int sendQ_num;

std::string stub_connect(const char* Tip, const char* Tport);
std::string stub_receive();
std::string stub_send(const char* Tip, const char* Tport, const char* msg, int request);
std::string stub_create();
std::string getlocalinfo();
std::string encrypt(std::string msg, std::string key);
std::string decrypt(std::string msg, std::string key);

void* get_in_addr(struct sockaddr *sa);

#endif