#ifndef MULTICAST_H
#define MULTICAST_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
extern "C"{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <errno.h>
	#include <netdb.h>
}
#include "parser.h"
#include "util.h"


char* stub_connect(char* Tip, char* Tport);
char* stub_receive();
char* stub_send(const char* Tip, const char* Tport, const char* msg);
char* stub_create();
char* getlocalinfo();
void* get_in_addr(struct sockaddr *sa);

#endif