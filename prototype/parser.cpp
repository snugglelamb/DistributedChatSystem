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

void Parser::parsePara(string rawStr) {
	size_t pos0 = rawStr.find("@");
	string str = rawStr.substr(pos0 + 1);
	size_t pos = str.find("#");
	if (pos == string::npos) {
		if(this->paserdebug)cout << "cannot find #"<<endl; return;
	}
	string req = str.substr(0, pos);

	int comparator = -1;

	if(req.compare("sendUID") == 0){
		comparator = 1;
	}else if(req.compare("multicastUserlist")==0){
		comparator = 2;
	}else if(req.compare("updateUserlist")==0){
		comparator = 2;
	}else if(req.compare("deleteUser")==0){
		comparator = 3;
	}else if(req.compare("addUser")==0){
		comparator = 4;
	}else if(req.compare("sendLeader")==0){
		comparator = 4;
	}else if(req.compare("connectLeader")==0){
		comparator = 4;
	}else if(req.compare("recMsg")==0){
		comparator = 5;
	}else if(req.compare("enqueueMsg")==0){
		comparator = 5;
	}else if(req.compare("newUser")==0){
		comparator = 6;
	}else if(req.compare("exitNotice")==0){
		comparator = 6;
	}
	Req reqObj;
	reqObj.comparator = comparator;
	reqObj.request = str;
	queue.enqueue(reqObj);
}

string Parser::dequeueRequest(){
	return queue.pop();
}

void Parser::processReq(string str) {
	if(this->paserdebug)cout << "parser: get params:" << endl;
	if(this->paserdebug)cout << string(arr) << endl;
	//string rawStr = string(arr);
	//size_t pos0 = rawStr.find("@");
	//string str = rawStr.substr(pos0 + 1);

	size_t pos = str.find("#");
	if (pos == string::npos) {
		if(this->paserdebug)cout << "cannot find #"<<endl; return;
	}
	string req = str.substr(0, pos);
	if(this->paserdebug)cout << "requestis " << req<<endl;
	vector<string> params = splitstr(str.substr(pos + 1), '_');
	// for(string s: params){ //      cout<< s<<endl; // }
	if (req.compare("sendLeader") == 0) {
		if(this->paserdebug)cout<<"sendLeader"<<endl;
		assert(params.size() == 2);
		if(this->paserdebug)cout << "parser: "<<params[0] << "   " <<params[1].c_str()<<endl;
		cn->sendLeader(params[0], atoi(params[1].c_str()));

	} else if (req.compare("connectLeader") == 0) {
		assert(params.size() == 4);
		cn->connectLeader(params[2], atoi(params[3].c_str()));

	} else if (req.compare("updateUserlist") == 0) {
		if(this->paserdebug)cout<<"update user list"<<endl;
		//string IP, string nickname, int port, int ID, int total, int nextid, bool isleader
		assert(params.size() % 7 == 2);
		vector<User> tmp;
		for (int i = 2; i < params.size(); i += 7) {
			User t;
			t.setIP(params[i]);
			t.setNickname(params[i + 1]);
			t.setPort(atoi(params[i + 2].c_str()));
			t.setID(atoi(params[i + 3].c_str()));
			t.setTotal(atoi(params[i + 4].c_str()));
			t.setNextID(atoi(params[i+5].c_str()));
			t.setIsLeader(params[i+6] == "1");
			tmp.push_back(t);
		}
		cn->updateUserlist(tmp);
	} else if (req.compare("addUser") == 0) {
		if(this->paserdebug)cout<<"add user"<<endl;

		assert(params.size() == 5);
		cn->addUser(params[2], params[3], atoi(params[4].c_str()));

	} else if (req.compare("multicastUserlist") == 0) {
		if(this->paserdebug)cout<<"multicast user list"<<endl;
		assert(params.size() == 2);
		cn->multicastUserlist();
	} else if (req.compare("recMsg") == 0) {
		assert(params.size() == 5);
		cn->recMsg(params[3],atoi(params[2].c_str()) ,params[4]);

	} else if(req.compare("enqueueMsg") == 0){
		assert(params.size() == 4);
		cn->enqueueMsg(params[2]+"_"+params[3]);
	}else if(req.compare("deleteUser") == 0){
		if(this->paserdebug) cout << "parser: in delete user"<<endl;
		if(this->paserdebug)for(string s : params){
			cout << s <<" ";
		}
		if(this->paserdebug)  cout <<endl;
		assert(params.size() == 4);
		cn->deleteUser(params[2], stoi(params[3]));
	}else if(req.compare("sendUID") == 0){
		assert(params.size() == 3);
		if(this->paserdebug)cout<<"send uid :" << params[2] <<endl;
		cn->sendUID(stoi(params[2]));
	}else if(req.compare("newUser") == 0){
		assert(params.size() == 5);
		//if(this->paserdebug)cout<<"uid :" << params[2] <<endl;
		//cn->sendUID(stoi(params[2]));
		cout<<"NOTICE "<<params[2]<<" joined on "<<params[3]<<":"<<params[4]<<endl;
	} else if(req.compare("exitNotice") == 0){
		assert(params.size() == 3);
		//if(this->paserdebug)cout<<"uid :" << params[2] <<endl;
		//cn->sendUID(stoi(params[2]));
		cout<<"NOTICE "<<params[2]<<"  EXIT"<<endl;
	} 
}

