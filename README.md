# Distributed ChatSystem
CIS 505 term project<br/>
Team member: Yuanjie Chen, Siyu Qiu, Zhi Li

###System workflow
- thread 1: Receive
    - Receive message, call parser method from parser.h.
    - Then parse the request. 
    - According to request name, call corresponding method in ChatNode.h

- thread 2: TypeMsg
    - listen to the keyboard
    - call stub_send
    - user send msg to leader	
    - leader multicast msg

- thread 3: CheckQueue
    - Check whether message queue is empty or not. If it is empty, the leader could exit.
    - If it is not empty, dequeue the message and multicast it to all users in the group and then exit.

- thread 4: HeartBeat
    - Leader: check if current users are alive, if not then update userlist and multicast. 
    - User: check if leader is alive, if not then raise leader election.


###Low-level Communication Design
```C
// multicast.h
string stub_connect(const char* Tip, const char* Tport);  
// use stub_send to send msg to potential node to test if ip:port is valid, then use stub_create to bind a port for listener
string stub_receive();
// listen on socket binded; send ack msg back to sender; forward msg received to parser
string stub_send(const char* Tip, const char* Tport, const char* msg, int request);
// send msg to destination ip:port; timeout if not receiving any ack msg back after 5s(arbitrary)
// request == 0 -> normal; request == 1 -> send only once; request > 1 -> send that many times
string stub_create(); // create a chat and bind chosen port (random) for listener
string getlocalinfo(); // get current IP and port
void* get_in_addr(struct sockaddr *sa); // get information from socket addr
```

####General Design
In our design, low-level communication is separate from high-level communication (message generation, total ordering, leader election etc.). Its major responsibility is to send given message to the specific destination given IP and port, as well as listen on specific port and forward messages received to high-level parser, which will then call high-level functions to handle different requests/scenarios. The interaction between low-level communication and high-level message processing is similar to stub in RPC. Here in our implementation, multicast.c acts as the stub, which is in charge of message sending and receiving, while other cpp files are in charge of high-level functionalities.  

####Fault Tolerance
Since we need to use UDP for message transferring, we have to design a small application-level protocol that would handle possible packet loss and reordering. The steps are as follows.


example msg:  RequestName#selfip_selfport_content


- Datagrams less than MTU (plus IP and UDP headers) in size (say 1024 bytes) to avoid IP fragmentation.
- Fixed-length header for each datagram that includes data length and a sequence number, so we can stitch data back together, and detect missed, duplicate, and re-ordered parts.
- Acknowledgements from the receiving side of what has been successfully received and put together.
- Timeout and retransmission on the sending side when these acks don't come within appropriate time. Current implementation is to first check if destination node is alive after the first timeout, then react correspondingly.

####Extra
Encryption will be done through low-level communication. The idea is to hash the message (after adding header mentioned above) before sending the message, then decrypt on receiving the message. 


###High level Communication Design

####Message Protocol
When sending messages, user send the message to the leader and then leader multicast the message to all users in the chat group. Each user use a hold-back queue implemented by min_heap. And when user receive the message from the leader, it compares the total number from the receiving message and its local receivedNumber for total ordering.
- format: RequestName#selfip_selfport_content

####Data Structure 
```C++
/*
  User Class: 
  variable:
  IP: ip address of a user
  nickname: nickname of a user
  ID: it is used for leader election module, which choose the highest ID as the new elected leader
  total: it records the current number of messages the leader has received. 
  When leader multicast messages, the total will also be sent to each client. It is used for total ordering.
  leader: bool variable to check whether this user is leader or not.

  method:
    getter and setter for each variable
    multiple constructors 
*/

class User
{
private:
	string IP; 
	string nickname; 
	int port;
	int ID;
	int total;
	bool leader;
public:
	string getIP();
	void setIP(string IP);
	string getNickname();
	void setNickname(string nickname);
	int getPort();
	void setPort(int port);
	int getID();
	void setID(int ID);
	int getTotal();
	void setTotal(int total);
	bool getIsLeader();
	void setIsLeader(bool isLeader);
	User(string IP, string nickname, int port, int ID, int total, bool isLeader);
	User(string IP, string nickname, int port);
	User();
};

/*
  variable:
  userlist: a list of current alive users
  me: local user
  rNum: number of msg received
  holdback:  small end message queue
  BlockingQueue msgQueue;
*/

// method:
createChat(User user);
// A new user create a chat. Then the chatNode object add the user to the userlist and set this user to be the leader.

reqLeader(string Tip, int Tport);
// Client request leader information from other client using target IP and port. 

sendLeader(string Tip, int Tport);
// Send leader information back to new added client

connectLeader(string Tip, int Tport);
// Connect to leader. add new client to userlist

updateUserlist(vector<User> vector);
// Each client received the new userlist from the leader and then update the new userlist.

addUser(string ip, string name, int port);
// Add new user to userlist and then multicast new userlist to other clients

void deleteUser(string ip, int port);
// Delete user from userlist

multicastUserlist();
// Multicast new userlist to other clients

sendMsg(string msg);
// send message to leader

multicastMsg(string msg);
// leader multicast message to all users

electLeader();
// send its own ID to neighbor user. and use chang robert ring algorithm to elect new leader

multicastLeader();
// send leader information to all user in the chat group
 
exitChat();
// if leader, delete myself from userlist, set a new leader, multicast new userlist it to all users. Then check message queue; if it is empty, exit. If it is not empty, wait until all messages has been multicasted to all users
// if I’m not a leader, request deleteUser method from leader, leader will multicast userlist. 

ChatNode Class:
class ChatNode
{
private:
	ChatNode(){};
	static ChatNode* node;
	vector<User> userlist;
	User me;
	int rNum;
BlockingQueue msgQueue;
	mutex userlistMutex;
	mutex meMutex;
	mutex rNumMutex;
	mutex totalMutex;

	struct Compare{
		bool operator() (int a, int b){
			return a > b;
		}
	};
	priority_queue<int, vector<int>, Compare> holdback;

public:
	static ChatNode* getInstance();
	vector<User> getUserlist();
	void setUserlist(vector<User> userlist);
	User getMe();
	void setMe(User user);
	int getPNum();
	void setPNum(int number);
	int getRNum();
	void setRNum(int number);

	void createChat(User user);
	void reqLeader(string Tip, int Tport);
	void sendLeader(string Tip, int Tport);
	void connectLeader(string Tip, int Tport);
	void updateUserlist(vector<User> vector);
	void addUser(string ip, string name, int port);
void deleteUser(string ip, int port);
	void multicastUserlist();

	void sendMsg(string msg);
	void multicastMsg(string msg);
electLeader();
multicastLeader();

exitChat();
	
};
```
#### Fault Tolerance Update 
  - 04/12/15
    - update stub_send
      - now stub_send could choose to send infinite times, send at most once and at least once
      - enable msg send queue by first enqueue msg before sending, and dequeue on receiving "OK" ack back
      - if not receiving "OK" (timeout), then will continue to resend SENDMAX(3) times iteratively and return "ERROR"
      - if receiving "RESEND" then keep sending until receiving "OK"
      - every time stub_send tries to resend, it first dequeue the current msg then enqueue a second time on reinitializing the same msg, however the id of the same msg may change.
    - update stub_receive
      - stub_receive could now check the size of packet received and compare with packet header
      - send "RESEND" request on receiving unintact msg
      - send "OK" ack on receiving full & intact msg
    - update stub_connect
      - now stub_connect use stub_send(Tip, Tport, msg, request = 1) to check if destination available by sending msg only once
      - will receive "ERROR" on not being able to connect
    - update stub_create
      - initialize msg send queue in stub_create
       
      
    - further update
      - handle duplicate msg
        - might need to maintain vector clock? 
      - enforce msg ordering on receiver side (possible?)
        - don't care what sequence the sender send (2,3,1 is OK), but receiver needs to get (1,2,3) then call high-level parser. 
  - 04/14/15
  - finished fault tolerance
    - cleanup the transmission between char*, const char* and string
    - datastructure change
      - add in struct clock_send
        - use targetIP:targetPORT as key to store how many valid messages have been sent to the target   
        - num will only update on sending non-resend messages(resend < 1)
        - use value of num(different among targets) as sequence number as part of msg header
      - add in struct msg_monitor
        - use receivedIP:receivedPORT as key mapping to sequence number received
        - a vector<int> holdback is used to store those sequence num that are way ahead of it's supposed to receive
        - latest is used as a classifier that seq received below latest will be defined as duplicate
        - latest only updates on receiving the msg with correct seq number(current latest+1)
        - on receving msg with correct seq number, it will then try to clear the member in holdback vector one by one incrementaly (e.g. if 3,4 are in holdback vector, now 2 arrives, it will then trigger the erase of 3 and 4, and update value of "latest" before erase), keep latest always the seq of latest valid msg received.
      -  update stub_receive to adapt the msg_monitor datastructure
        -  first check if data is intact
          -  if intact: reply "OK", then if sender already in monitor list, update seq num and holdback vector accordingly.
          -  if not intact: then simply reply "RESEND" as ack.
      -  update stub_send to adapt the clock_send datastructure
        -  first check if it's a resend request
        -  (either triggered by not intact resend request "RESEND" or resource temporarily unavailable)
          -  if resend (request >= 1) use same msg without adding new header to it
          -  if not resend (request == 0) 
            -  update sequence number accordingly, use the same sequence number for sending that msg in the header
            -  adding new header to it, format:   data size(fix length == 5) + IP:PORT + "STA" + sequence number(target specific) + "END@" (@ is for parser). Overall something like: 00045192.168.1.100:26411STA00002END@what's up
    - fix RESEND bug
      - used to send infinite times when data is not intact
      - now just send additional three times and return "ERROR" on the last run
      - Then there won't be any BUS ERROR

```C++
// Data Structure designed for UDP fault tolerance
	// for send msg storage
	struct msgQueue_send {
		int id;
		char message[MAXBUFLEN]; //msg length arbitrary 1024
	};
	
	// for send_seq_num management
	struct clock_send {
		std::string label; // IP:PORT of target
		int num; // valid msg sent to specific target
	};
	
	int clock_num = 0;
	struct clock_send vclock[MAXUSER];
	
	// hash table similar struct
	struct msg_monitor{
		std::string label;
		std::vector<int> holdback;
		int latest;
	};
	
	int monitor_num = 0;
	struct msg_monitor monitor[MAXUSER];
	// struct msgQueue_recv {
	// 	int id;
	// 	char message[512]; //msg length arbitrary 1024
	// };
	
	// send Queue cache
	int sendID[QUEUESIZE];
	int sendQ_num = 0; // record no. of msg remained in send queue
	struct msgQueue_send sendQ[QUEUESIZE]; // 10 is the size of msg queue
	const struct msgQueue_send empty_send = {};	
```


### Reference Links
[CIS 505](http://www.cis.upenn.edu/~boonloo/cis505-sp15/)
