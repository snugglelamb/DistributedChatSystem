//#include "ChatNode.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <assert.h>
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
		//cout << "request is " << req<<endl;
    	vector<string>params = splitstr(str.substr(pos+1), '_');
    	// for(string s: params){
    	// 	cout<< s<<endl;
    	// }
    	if(req.compare("reqLeader") == 0){
    		assert(params.size() == 2);
    		cn.reqLeader(params[0], atoi(params[1].c_str()));
    	}else if(req.compare("connectLeader")){
    		assert(params.size() == 2);
    		cn.connectLeader(params[0], atoi(params[1].c_str()));
    	}else if(req.compare("updateUserlist") == 0){
    		//string IP, string nickname, int port, int ID, int total, bool isleader
    		assert(params.size()/6 ==0);
    		vector<User> tmp;
    		for(int i = 0; i < params.size()/6; i += 6){
    			User t;
    			t.setIP(params[i]);
    			t.setNickname(params[i+1]);
    			t.setPort(atoi(params[i+2].c_str()));
    			t.setID(atoi(params[i+3].c_str()));
    			t.setTotal(atoi(params[i+4].c_str()));
    			t.setIsleader(params[i+5] == "1");
    			tmp.push_back(t);
    		}
    		cn.updateUserlist(tmp);

    	}else if(req.compare("addUser") ==0){
    		assert(params.size() == 3);
    		cn.addUser(params[0], params[1], atoi(params[2]).c_str();
    	}else if(req.compare("multicastUserlist") == 0){
    		assert(params.size() == 0);
    		cn.multicastUserlist();
    	}
		 
	}
};
// int main(){
// 	Parser p;
// 	string tmp = "multicastUserlist#";
// 	char* arr = new char[tmp.length() + 1];
// 	strcpy(arr, tmp.c_str());
// 	p.parsePara(arr);
// 	return 0;
// }




