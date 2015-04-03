#ifndef MULTICAST_H
#define MULTICAST_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* onReceive(int port);

int multiJoin(char* destIP, int port, char* destUserIP, char* destUserName, int userPort);

void sendUserlist(char* ip, int port, char* userlist);

void sendNewUser(char* ip, int port, char* newUser);

void sendSequencer(char* ip, int port, char* sequencer);

void sendSequenceNumber(char* ip, int port, int sequenceNumber);

#endif