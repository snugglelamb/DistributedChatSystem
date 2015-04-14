#include "multicast.h"
// local ip, port for dchat
#define MAXBUFLEN 4096
#define QUEUESIZE 20
#define SENDMAX 3
#define PREFIXLEN 5 
#define MAXUSER 50

static int sockfd; //used for listener

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

// int recvQ_num = 0;
// struct msgQueue_recv recvQ[QUEUESIZE];
// const struct msgQueue_recv empty_recv = {};



void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

std::string getlocalinfo()
{
	int port;
	char* msg = new char[30];
	char* port_ = new char[10];
	char* ip = new char[20];
	
	// get local ip, port info; ip:port	
	// could only get 0.0.0.0 from listening socket
	// could get real ip from send socket
	struct sockaddr_in localAddress;
	socklen_t addressLength = sizeof(localAddress);
	getsockname(sockfd, (struct sockaddr*)&localAddress, &addressLength);
	
	//get ip
	    struct ifaddrs *ifaddr, *ifa;
	    int family, s;
	    char host[NI_MAXHOST];

	    if (getifaddrs(&ifaddr) == -1)
	    {
	        perror("getifaddrs");
	        return "GETIPERROR";
	    }


	    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	    {
	        if (ifa->ifa_addr == NULL)
	            continue;

	        s = getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

		// en0 on mac, eth0 on linuxs
	        if((strcmp(ifa->ifa_name,"en0")==0)&&(ifa->ifa_addr->sa_family==AF_INET))
	        {
	            if (s != 0)
	            {
	                printf("getnameinfo() failed: %s\n", gai_strerror(s));
	                return "GETIPERROR";
	            }
			sprintf(ip, "%s", host);
	            // printf("\tInterface : <%s>\n",ifa->ifa_name );
	            // printf("\t  Address : <%s>\n", host);
	        }
	    }

	    freeifaddrs(ifaddr);
	
	
	port = (int) ntohs(localAddress.sin_port);
	// strcpy(ip, inet_ntoa(localAddress.sin_addr));
	// printf("local address: %s\n", ip);
	// printf("local port: %d\n", port);
	
	sprintf(port_, "%d", port);	
	strcat(msg,ip);
	strcat(msg,":");
	strcat(msg,port_);
	std::string msg_(msg);
	
	delete[] ip;
	delete[] port_;
	delete[] msg;	
	
	// std::cout << "stub: finish binding. ip:port ->" << msg_ << std::endl;
    // printf("stub: waiting to recvfrom...\n");
	
	return msg_;
}


std::string stub_create()
{
    struct addrinfo hints, *servinfo, *p;
    int rv, port;
	char* port_ = new char[10];
	
	// randomly assign a port number
	port = randomPort();
	// port = 20000;
	sprintf(port_,"%d", port);
	printf("stub: PORT specified: %d\n", port);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM; // use UDP
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, port_, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return "ERROR";
    }
	
    // loop through all the results and bind to the first valid one
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("stub: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("stub: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "stub: failed to bind socket\n");
        return "ERROR";
    }

    freeaddrinfo(servinfo); // done with servinfo
	delete[] port_;
	
	// init send queue
	int i = 0;
	for (i = 0; i < QUEUESIZE; i++) {
		sendID[i] = i;
	}
	return getlocalinfo();
			
}

std::string stub_connect(const char* Tip, const char* Tport)
{

	const char* msg = "00013CONNECT@";	
	if ( stub_send(Tip, Tport, msg, 1).compare("ERROR") == 0 ) return "ERROR";
	
	std::string msg_;
	msg_.assign(stub_create());
	if ( msg_.compare("ERROR") == 0 ) { return "ERROR"; }
	else { return msg_; }
}

std::string stub_receive()
{
	// will only receive normal msg other than OK, RESEND
	struct sockaddr_storage their_addr;
    int numbytes;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];
	char* Sip = new char[30];

    addr_len = sizeof their_addr;
	
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom error");
        return "ERROR";
    }
	// store source ip
	strcpy(Sip, inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
    printf("stub: got packet from %s\n", Sip);
    printf("stub: packet is %d bytes long\n", numbytes);
	delete[] Sip;
	
	// check packet size
	char packet_size[6] = {0};
	char str[20] = {0}; //OK, RESEND
	memcpy(packet_size, &buf[0], 5);
	packet_size[5] = '\0';
	int size = atoi(packet_size);
	
	if (numbytes == size) {
		printf("stub: received size correct.\n");
		// packet intact
	    buf[numbytes] = '\0';
	    printf("stub: packet contains \"%s\"\n", buf);
		
		// send ack back
		strcpy(str, "OK");
		if ( buf[0] == '0') {
			// connect received, send ack back
			if ((numbytes = sendto(sockfd, str, strlen(str), 0, 
				(struct sockaddr *)&their_addr, addr_len)) == -1) {
		        perror("stub: sendto");
		        return "ERROR";
		    }
		
		    printf("stub: send %d bytes to %s\n	msg contains: %s\n\n", numbytes, inet_ntop(their_addr.ss_family,
		            get_in_addr((struct sockaddr *)&their_addr),
		            s, sizeof s), str);
		} 
		
		if (buf[5] != 'C') {
			// ignore first connect msg
			// update monitor
			int seq_num = 0;
			std::string str(buf);
			std::size_t start = str.find("STA");
			std::size_t end = str.find("END");
			std::string key;
			std::string seq;
			if (start!=std::string::npos && end!= std::string::npos ) {
				// capture "IP:PORT" + "STA" + SEQ + "END"
				key.assign(str.substr(5, start - 5));
				seq.assign(str.substr(start + 3, end - start - 3));
				seq_num = stoi(seq);
				std::cout << "stub: monitor key received: " << key << std::endl;
				std::cout << "stub: monitor seq received: " << seq_num << std::endl;
			} else {
				printf("stub: header not intact.\n");
				return "ERROR";
			}
		
			if (monitor_num == 0) {
				// add in monitor
				monitor[monitor_num].label = key;
				if (seq_num == 1) { monitor[monitor_num].latest = 1; }
				else { 
					monitor[monitor_num].latest = 0;
					monitor[monitor_num].holdback.push_back(seq_num); 
				}
				monitor_num++;
				// parse string received
				// parsePara(&buf);
						
			} else {
				// loop to find if a match
				int i = 0;
				for (i = 0; i < monitor_num; i++ ) {
					if ((monitor[i].label).compare(key) == 0) {
						break;
					}
				}
				if (i == monitor_num) {
					// user not exist
					monitor[monitor_num].label = key;
					if (seq_num == 1) { monitor[monitor_num].latest = 1; }
					else { 
						monitor[monitor_num].latest = 0;
						monitor[monitor_num].holdback.push_back(seq_num); 
					}
					monitor_num++;
					// parse string received
					// parsePara(&buf);
				
				} else {
					// user exist 
					int flag = seq_num - monitor[i].latest;
					if ( flag <= 0 ) {
						// duplicate
						printf("stub: duplicate message.\n");
					} else if ( flag == 1 ) {
						// correct
						monitor[i].latest = seq_num;
						// parse string received
						// parsePara(buf);
					
						while (!monitor[i].holdback.empty()) {
							// remove seq_num + 1 one at a time
							printf("stub: monitor check hold back.\n");
							int found = 0;
						    for (std::vector<int>::iterator it = monitor[i].holdback.begin(); it != monitor[i].holdback.end(); ++it) {
						    	if ( *it == monitor[i].latest + 1) {
									monitor[i].latest += 1;
									monitor[i].holdback.erase(it);
									found = 1;
						    		break;
						    	}
						    }
							if (found == 1) {
								// dequeue msg here and send to parser							
							} else {
								// found no adjacent
								printf("stub: monitor still %lu msg in holdback queue.\n", monitor[i].holdback.size());
								break;
							}
						}// end while
					
					} else {
						// some msg hasn't come
						monitor[i].holdback.push_back(seq_num);
						// may need to enqueue msg, but just pass to parser for now
						// parse string received
						// parsePara(buf);				
					}
				}			
			} // end user exist/not exist handling
		} // end check if CONNECT msg
	} // end packet intact 
	
	// packet loss 
	else {
		printf("stub: packet loss occurs, request resend.\n");
		
		strcpy(str, "RESEND");
		if ( buf[0] == '0') {
			// request resend
			if ((numbytes = sendto(sockfd, str, strlen(str), 0, 
				(struct sockaddr *)&their_addr, addr_len)) == -1) {
		        perror("stub: sendto");
		        return "ERROR";
		    }
		
		    printf("stub: send %d bytes to %s\n	msg contains: %s\n\n", numbytes, inet_ntop(their_addr.ss_family,
		            get_in_addr((struct sockaddr *)&their_addr),
		            s, sizeof s), str);
		}
		
	}
	

	return "SUCCESS";
	
}

std::string stub_send(const char* Tip, const char* Tport, const char* msg, int request)
{
    int sockfd_w;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes; 
	
	// receive msg from server, for ack
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];
	
	// customize request sent
	char* fullmsg = new char[MAXBUFLEN];
	// sometimes char* mess up with address in memory
	// printf("Target IP:%s\n", Tip);
	// printf("Target PORT:%s\n", Tport); // check if Tport is shifted to "msg"
	
	// set timeout val
	struct timeval tv;
	tv.tv_sec = 5;  /* 5 Secs Timeout */
	tv.tv_usec = 0;  // Not init'ing this can cause strange errors
	
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(Tip, Tport, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return "ERROR";
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd_w = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("stub: socket error");
            continue;
        }
		
		if (setsockopt(sockfd_w, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval))== -1) {
		            perror("setsockopt");
		            return "ERROR";
		        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "stub: failed to get addr.\n");
        return "ERROR";
    }
	
	// enqueue
	int available_id = 0;
	if (sendQ_num < QUEUESIZE) { 
		printf("stub: send msg enqueue.\n");
		available_id = sendID[sendQ_num];
		sendQ[available_id].id = available_id;
		strcpy(sendQ[available_id].message, msg);
		sendQ_num ++;
	} else {
		printf("stub: msgQue full.\n");
		return "MSGQUEUEFULL";
	}
	// request >= 1 ==> resend msg	
	if (request < 1) {
		int valid_send_num = 0;
		char* ip_ = new char[30];
		char* port_ = new char[10];
		char* taddr = new char[40];
		
		strcpy(ip_, Tip);
		strcpy(port_, Tport);
		strcpy(taddr, ip_);
		strcat(taddr, ":");
		strcat(taddr, port_);
		std::string key(taddr);
		delete[] ip_;
		delete[] port_;
		delete[] taddr;
			
		// check if target is already in the list
		if (clock_num == 0) {
			// no former target
			printf("stub: clock new target.\n");
			vclock[clock_num].label = key;
			vclock[clock_num].num = 1;
			clock_num++;
			valid_send_num = 1; 
			
		} else {
			int i = 0;
			for (i = 0; i < clock_num; i++ ) {
				if ((vclock[i].label).compare(key) == 0) {
					break;
				}
			}
			if (i == clock_num) {
				// not in list, new target
				printf("stub: clock new target, num = 1\n");
				vclock[clock_num].label = key;
				vclock[clock_num].num = 1;
				clock_num++;
				valid_send_num = 1; 
			} else {
				// in list, update num
				vclock[i].num += 1;
				valid_send_num = vclock[i].num;
				printf("stub: clock update target, num = %d.\n", valid_send_num);
			} // end check if in list
		} // end check if new
		
		// add prefix
		// PREFIXLEN is the length of header (data size)
		char* seq_num = new char[10];
		char* seq = new char[100];
		char* info = new char[50];
		std::string info_ = getlocalinfo();
		strcpy(info, info_.c_str());
		printf("SSSSTUB: get info: %s\n", info);
		sprintf(seq_num, "%05d", valid_send_num);
		// SEQ: IP:PORTSTA00000END@
		strcpy(seq, info);
		strcat(seq, "STA");
		strcat(seq, seq_num);
		strcat(seq, "END@");
	
		int size = PREFIXLEN + strlen(seq) + strlen(msg); 
		sprintf(fullmsg, "%05d", size);
		strcat(fullmsg, seq);
		strcat(fullmsg, msg);
		
		delete[] seq_num;
		delete[] seq;
		delete[] info;
		
	} else {
		// resend confirmed
		strcpy(fullmsg, msg);
	}
	
	printf("stub: msg prepared to send: %s\n",fullmsg);
	const char* fullmsg_ = fullmsg;
	delete[] fullmsg; // free buffer
	
    if ((numbytes = sendto(sockfd_w, fullmsg_, strlen(fullmsg_), 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("stub: sendto");
        return "ERROR";
    }
	printf("stub: sent %d bytes to %s\n", numbytes, Tip);
	
	// receive return msg from server *ack
	// need to compare received msg with OK, RESEND
    addr_len = sizeof their_addr;
	
    if ((numbytes = recvfrom(sockfd_w, buf, MAXBUFLEN-1 , 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("stub: recvfrom");
		// not getting ack within 5 sec; time out
        // return "ERROR";
		// dequeue
		sendQ_num--;
		sendID[sendQ_num] = sendQ[available_id].id;
		sendQ[available_id] = empty_send;
		// need to check if target alive
		if (request == 1) {
			// send only once, target host down
			return "ERROR";
		} else if (request == 0) {
			// resend three times
			return stub_send(Tip, Tport, fullmsg_, SENDMAX);
		} else {
			return stub_send(Tip, Tport, fullmsg_, request - 1);
		}
    }
	
    printf("stub: received packet from %s\n",
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s));

    printf("stub: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("stub: packet contains: %s\n", buf);
	
	if ( strcmp(buf, "OK") == 0 ) {
		// dequeue
		printf("stub: send msg dequeue.\n");
		sendQ_num--;
		sendID[sendQ_num] = sendQ[available_id].id;
		sendQ[available_id] = empty_send;
	} else if (strcmp(buf, "RESEND") == 0) {
		// resend, connection correct
		// dequeue
		printf("stub: message resent \n");
		sendQ_num--;
		sendID[sendQ_num] = sendQ[available_id].id;
		sendQ[available_id] = empty_send;
		if (request == 0) {
			return stub_send(Tip, Tport, fullmsg_, SENDMAX);
		} else if (request == 1) {
			printf("STUB: RESEND FAILURE AFTER %d ADDITIONAL TRIALS.\n", SENDMAX);
			return "ERROR";
		} else {
			return stub_send(Tip, Tport, fullmsg_, request - 1);
		}
		
	}

    close(sockfd_w);
	
	return "SUCCESS";
	
}
