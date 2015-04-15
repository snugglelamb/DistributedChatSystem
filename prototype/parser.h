#ifndef PARSER_H
#define PARSER_H
#include "ChatNode.h"
#include "User.h"
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
		ChatNode  *cn;
		vector<string> splitstr(string ori, char deli);
public:
		Parser();
		void parsePara(string arr );

};

#endif
