#include "multicast.h"
// local ip, port for dchat
#define MAXBUFLEN 9999
#define QUEUESIZE 20
#define SENDMAX 3
#define PREFIXLEN 6

static int sockfd; //used for listener

// for fault tolerance	
struct msgQueue_send {
	int id;
	char message[1024]; //msg length arbitrary 1024
};

struct msgQueue_recv {
	int id;
	char message[1024]; //msg length arbitrary 1024
};

int sendID[QUEUESIZE];
int sendQ_num = 0; // record no. of msg remained in send queue
int recvQ_num = 0;
struct msgQueue_send sendQ[QUEUESIZE]; // 10 is the size of msg queue
struct msgQueue_recv recvQ[QUEUESIZE];
const struct msgQueue_send empty_send = {};
const struct msgQueue_recv empty_recv = {};


void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

char* getlocalinfo()
{
	int port;
	char msg[30];
	char port_[10];
	char ip[20];
	
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
	printf("local address: %s\n", ip);
	printf("local port: %d\n", port);
	
	sprintf(port_, "%d", port);	
	strcat(msg,ip);
	strcat(msg,":");
	strcat(msg,port_);
	
	printf("stub: finish binding. ip:port -> %s:%d\n",ip,port);
    printf("stub: waiting to recvfrom...\n");
	
	return msg;
}


char* stub_create()
{
    struct addrinfo hints, *servinfo, *p;
    int rv, port;
	char port_[20];
	
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
	
	// init send queue
	int i = 0;
	for (i = 0; i < QUEUESIZE; i++) {
		sendID[i] = i;
	}
	return getlocalinfo();
			
}

char* stub_connect(const char* Tip, const char* Tport)
{

	char msg[20];	
	strcpy(msg, "connect");
		
	if ( strcmp(stub_send(Tip, Tport, msg, 1), "ERROR") == 0 ) return "ERROR";

	strcpy(msg, stub_create());
	if ( strcmp(msg, "ERROR") == 0 ) { return "ERROR"; 	}
	else { return msg; }

}

char* stub_receive()
{
	// will only receive normal msg other than OK, RESEND
	struct sockaddr_storage their_addr;
    int numbytes;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];
	char Sip[30];

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
	
	// check packet size
	char packet_size[6];
	char str[20]; //OK, RESEND
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
		
	// parse string received
	// parsePara(&buf);
	} 
	
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

char* stub_send(const char* Tip, const char* Tport, const char* msg, int request)
{
	// request > 1 ==> resend msg
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
	char fullmsg[512];
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

	// add prefix
	int size = strlen(msg) + PREFIXLEN; // 6s is the length of header
	sprintf(fullmsg, "%05d@", size);
	strcat(fullmsg, msg);
	printf("stub: msg prepared to send: %s\n",fullmsg);
    if ((numbytes = sendto(sockfd_w, fullmsg, strlen(fullmsg), 0,
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
			return stub_send(Tip, Tport, msg, SENDMAX);
		} else {
			return stub_send(Tip, Tport, msg, request - 1);
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
		return stub_send(Tip, Tport, msg, 0);
	}

    close(sockfd_w);
	
	return "SUCCESS";
	
}

