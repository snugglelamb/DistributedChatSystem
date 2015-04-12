#ifndef MULTICAST_H
#define MULTICAST_H

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
// #include "parser.h"

extern int sendQ_num;

extern "C" char* stub_connect(const char* Tip, const char* Tport);
extern "C" char* stub_receive();
extern "C" char* stub_send(const char* Tip, const char* Tport, const char* msg, int request);
extern "C" char* stub_create();
extern "C" char* getlocalinfo();
extern "C" void* get_in_addr(struct sockaddr *sa);

#endif