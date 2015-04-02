# CIS505
CIS 505 term project<br/>
Team member: Yuanjie Chen, Siyu Qiu, Zhi Li

### Initial Interface Design
1). Datastructure
```C++
  User List (shared by all users within a chat group)
  int id; //ID=0 as leader
  string ip_addr;
  string port;
  
  // static boolean value to identify whether the attendent is the sequencer
  static boolean isSequencer = false;

  //log for 2PC
  //request, commit, accept...

  Chat Log (stored on local machine)
  store in buffer;
  write to local file on exit;
  
```

2). Functions
```C++
  create(string name, string ip_addr, string port);
    /*
      only sequencer
      1. bind a listener socket with port no.
      2. select as central sequencer, update user list
     */
     
  join(string name, string ip_addr, string port);
    /*
      only attendent:
      1. check if group exists
      2. act on ack msg received
        exit on error
        rejoin if duplicate names or not contacting the sequencer
      3. send request to update user list
     */
     
  handle_join(bool isSequencer, string name, string ip_addr, string port);
    /*
      sequencer:
      1. update user list and send ack msg back to attendent
      2. multicast user list to all active attendents
      
      attendent:
      1. send back ack msg(1st part) whether duplicate name exists
      2. send back ack msg(2nd part) ip_addr and port number of current sequencer
     */
     
  send(bool isSequencer, );
     /*
      sequencer:
      1. ** need to maintain total order
      2. ** need to implement 2PC request, commit..
      
      attendent:
      1. send msg to sequencer
      2. receive ack msg
     */
     
  exit_handler(bool isSequencer, SIGINT sig);
     /*
      sequencer:
      1. sequencer itself exits (CTRL+D)
        1. assign a new leader, remove itself from the list
        2. update user list and multicast to all attendents
        3. exit program
        
      2. handle attendent exit
        1. receive request to exit
        2. update user list and multicast to all attendents
        
      attendent:
      1. send exit msg to sequencer
      2. receive ack msg
      3. exit program
     */
     
  check(bool isSequencer);
     /*
      sequencer:
      1. check zombie attent and autoremove from the list
      2. multicast user list
      
      attendent:
      1. check if sequencer is alive
      2. ** request election for new leader
     */
  
```

3). Protocol Design (msg format)
  - should be based on functions


4) Implementation
  - Multithreading
    - th1: send
    - th2: receive, parser
    - th3: check alive
