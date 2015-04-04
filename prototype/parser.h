#ifndef PARSER_H
#define PARSER_H
#include "ChatNode.h"
#include "User.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <cstdlib>
using namespace std;
class Parser{
private:
		ChatNode *cn;
		vector<string> splitstr(string ori, char deli);
public:
		Parser();
		void parsePara(char* arr );

};

#endif