#include "parser.h"

vector<string> Parser::splitstr(string ori, char deli) {
	vector<string> tmp;
	stringstream ss(ori);
	string tok;
	while (getline(ss, tok, deli)) {
		tmp.push_back(tok);
	}
	return tmp;
}

Parser::Parser() {
	this->cn = ChatNode::getInstance();
}
void Parser::parsePara(char* arr) {
	//cout << "get 44444444params:" << endl;
	//cout << string(arr) << endl;
	string rawStr = string(arr);
	size_t pos0 = rawStr.find("@");
	string str = rawStr.substr(pos0 + 1);

	size_t pos = str.find("#");
	if (pos == string::npos) {
		//cout << "cannot find #"<<endl; return;
	}
	string req = str.substr(0, pos);
	//cout << "requestis " << req<<endl;
	vector<string> params = splitstr(str.substr(pos + 1), '_');
	// for(string s: params){ //      cout<< s<<endl; // }
	if (req.compare("sendLeader") == 0) {
		//	cout<<"sendLeader"<<endl;
		assert(params.size() == 2);
		//cout << "parser: "<<params[0] << "   " <<params[1].c_str()<<endl;
		cn->sendLeader(params[0], atoi(params[1].c_str()));

	} else if (req.compare("connectLeader") == 0) {
		assert(params.size() == 4);
		cn->connectLeader(params[2], atoi(params[3].c_str()));

	} else if (req.compare("updateUserlist") == 0) {
		//cout<<"update user list"<<endl;
		//string IP, string nickname, int port, int ID, inttotal, bool isleader 
		assert(params.size() % 6 == 2);
		vector<User> tmp;
		for (int i = 2; i < params.size(); i += 6) {
			User t;
			t.setIP(params[i]);
			t.setNickname(params[i + 1]);
			t.setPort(atoi(params[i + 2].c_str()));
			t.setID(atoi(params[i + 3].c_str()));
			t.setTotal(atoi(params[i + 4].c_str()));
			t.setIsLeader(params[i + 5] == "1");
			tmp.push_back(t);
		}
		cn->updateUserlist(tmp);

	} else if (req.compare("addUser") == 0) {
		//	cout<<"add user"<<endl;

		assert(params.size() == 5);
		cn->addUser(params[2], params[3], atoi(params[4].c_str()));

	} else if (req.compare("multicastUserlist") == 0) {
		//	cout<<"multicast user list"<<endl;
		assert(params.size() == 2);
		cn->multicastUserlist();
	} else if (req.compare("recMsg") == 0) {
		assert(params.size() == 5);
		cn->recMsg(params[3],atoi(params[2].c_str()) ,params[4]);

	} else if(req.compare("enqueueMsg") == 0){
		assert(params.size() == 4);
		cn->multicastMsg(params[2]+"_"+params[3]);
	}

}
