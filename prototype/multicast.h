#ifndef MULTICAST_H
#define MULTICAST_H

#include "parser.h"
#include <string>
extern "C"{
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
	#include <ifaddrs.h>
	#include "util.h"
};

string stub_connect(const char* Tip, const char* Tport);
string stub_receive();
string stub_send(const char* Tip, const char* Tport, const char* msg);
string stub_create();
string getlocalinfo();
void* get_in_addr(struct sockaddr *sa);

#endif



