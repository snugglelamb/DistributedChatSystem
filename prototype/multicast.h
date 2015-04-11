#ifndef MULTICAST_H
#define MULTICAST_H

#include "parser.h"

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

extern "C" char* stub_connect(char* Tip, char* Tport);
extern "C" char* stub_receive();
extern "C" char* stub_send(const char* Tip, const char* Tport, const char* msg);
extern "C" char* stub_create();
extern "C" char* getlocalinfo();
extern "C" void* get_in_addr(struct sockaddr *sa);

#endif



