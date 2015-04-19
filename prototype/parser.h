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
		bool paserdebug = false;
		static ChatNode  *cn;
		static vector<string> splitstr(string ori, char deli);
		static Block_PQ queue;
public:
		Parser();
		void parsePara(string arr );
		static string dequeueRequest();
		static void processReq(string str);
};

#endif
