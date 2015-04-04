//#include "ChatNode.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;
class Parser{
	//ChatNode cn = ChatNode.getInstance();

	vector<string> splitstr(string ori, char deli){
		vector<string> tmp;
		stringstream ss(ori);
		string tok;
		while(getline(ss, tok, deli)){
			tmp.push_back(tok);
		}
		return tmp;
	}

	public: 
		void parsePara(char* arr ){
		string str = string(arr);
		size_t pos = str.find("#");
		if (pos == string::npos){
			cout << "cannot find #"<<endl;
			return;
		}
		string req = str.substr(0, pos);
		cout << "request is " << req<<endl;
    	vector<string>params = splitstr(str.substr(pos+1), '_');
    	for(string s: params){
    		cout<< s<<endl;
    	}
		 
	}
};
int main(){
	Parser p;
	string tmp = "join#a_1.2.2.0_455";
	char* arr = new char[tmp.length() + 1];
	strcpy(arr, tmp.c_str());
	p.parsePara(arr);
	return 0;
}




