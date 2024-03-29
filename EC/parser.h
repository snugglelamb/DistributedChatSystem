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

		Parser(){};
		static Parser* parser;	

		static ChatNode* cn;
		vector<string> splitstr(string ori, char deli);
		Block_PQ queue;
		Parser& operator=(const Parser&) ; // disable assignment
		Parser(const Parser&);            // disable copying

public:

		static Parser* getInstance();
		void parsePara(string arr);

		string dequeueRequest();
		void processReq(string str);


};

#endif
