#include "multicast.h"
// local ip, port for dchat
#define MAXBUFLEN 9999
static int sockfd; //used for listener

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

string getlocalinfo()
{
	int port;

	char* msg = new char[100];
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
       // perror("getifaddrs");
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
	//printf("local address: %s\n", ip);
	//printf("local port: %d\n", port);
	
	sprintf(port_, "%d", port);
	strcpy(msg,"");
	strcat(msg,ip);
	strcat(msg,":");
	strcat(msg,port_);
	//strcat(msg,'\0');

	delete[] port_;
	delete[] ip;
		
	return string(msg);
}


string stub_create()
{
    struct addrinfo hints, *servinfo, *p;
    int rv, port;

	char* port_ = new char[10];

	
	// randomly assign a port number
	port = randomPort();
	// port = 20000;
	sprintf(port_,"%d", port);
	//printf("stub: PORT specified: %s\n", port_);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM; // use UDP
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, port_, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return "CREATEERROR";
    }

    // loop through all the results and bind to the first valid one
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("stub: socket");
            continue;
        }

        if (::bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("stub: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "stub: failed to bind socket\n");
        return "CREATEERROR";
    }

    freeaddrinfo(servinfo); // done with servinfo
    //printf("%s\n", getlocalinfo());
	delete[] port_;
	
	return getlocalinfo();
			
}

string stub_connect(const char* Tip, const char* Tport)
{
	//printf("tip: %s tport: %s\n",Tip, Tport);
	const char* msg = "CONNECT@";
	if ( (stub_send(Tip, Tport, msg)).compare("ERROR") == 0 ) return "ERROR";
	

	string msg_;
	msg_.assign(stub_create());
	//printf("%s\n", msg);
	if ( msg_.compare("ERROR") == 0 ) { return "ERROR"; }
	else { 
		std::cout << "########connect get msg: "<< msg_ << endl;

		return msg_; 
	}

}

string stub_receive()
{
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
    //printf("stub: got packet from %s\n", Sip);
	
    //printf("stub: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    //printf("stub: packet contains \"%s\"\n", buf);
		
	char* str = new char[20];
	strcpy(str, "OK");
	if ( buf[0] == '0') {
		// connect received, send ack back
		if ((numbytes = sendto(sockfd, str, strlen(str), 0, 
			(struct sockaddr *)&their_addr, addr_len)) == -1) {
	        perror("stub: sendto");
	        return "ERROR";
	    }
		
	}
	
	// parse string received

	if (buf[6] == 'C') {
		//printf("stub: connect received. do nothing\n");
	} else {
		Parser p;
		p.parsePara(buf);
	}

	return "SUCCESS";
}

string stub_send(const char* Tip, const char* Tport, const char* msg)
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
	char* fullmsg = new char[1024];
	// sometimes char* mess up with address in memory
	
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

	// customize request sent
	strcpy(fullmsg,"");
	strcat(fullmsg, "00000@"); // add prefix
	strcat(fullmsg, msg);
	// strcpy(fullmsg, "0ABCDEF"); // test
	//printf("stub: msg prepared to send: %s\n",fullmsg);
    if ((numbytes = sendto(sockfd_w, fullmsg, strlen(fullmsg), 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("stub: sendto");
        return "ERROR";
    }
	//printf("stub: sent %d bytes to %s\n", numbytes, Tip);
	
	// receive return msg from server
	// ack timeout 5s
    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd_w, buf, MAXBUFLEN-1 , 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("stub: recvfrom");
        return "ERROR";
    }
	

    buf[numbytes] = '\0';

    close(sockfd_w);
	
	return "SUCCESS";
	
}
