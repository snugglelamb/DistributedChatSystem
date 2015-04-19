#ifndef PARSER_H
#define PARSER_H
#include "ChatNode.h"
#include "User.h"
#include "blocking_priority.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <stdlib.h>


using namespace std;

class ChatNode;

class Parser{
private:
		static Parser* parser;	
		static ChatNode  *cn;
		vector<string> splitstr(string ori, char deli);
		Block_PQ queue;
public:
		Parser();
		static Parser* getInstance();
		void parsePara(string arr );
		string dequeueRequest();
		void processReq(string str);
};

#endif
