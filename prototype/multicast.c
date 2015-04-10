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
	
	port = (int) ntohs(localAddress.sin_port);
	strcpy(ip, inet_ntoa(localAddress.sin_addr));
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
	return getlocalinfo();
			
}

char* stub_connect(char* Tip, char* Tport)
{

	char msg[20];	
	strcpy(msg, "connect");
		
	if ( strcmp(stub_send(Tip, Tport, msg), "ERROR") == 0 ) return "ERROR";

	strcpy(msg, stub_create());
	if ( strcmp(msg, "ERROR") == 0 ) { return "ERROR"; 	}
	else { return msg; }

}

char* stub_receive()
{
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
    buf[numbytes] = '\0';
    printf("stub: packet contains \"%s\"\n", buf);
		
	char str[20];
	strcpy(str, "OK:3");
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
	parsePara(buf);
	return "SUCCESS";
}

char* stub_send(const char* Tip, const char* Tport, const char* msg)
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

	// customize request sent
	strcpy(fullmsg,"");
	strcat(fullmsg, "00000"); // add prefix
	strcat(fullmsg, msg);
	// strcpy(fullmsg, "0ABCDEF"); // test
	printf("stub: msg prepared to send: %s\n",fullmsg);
    if ((numbytes = sendto(sockfd_w, fullmsg, strlen(fullmsg), 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("stub: sendto");
        return "ERROR";
    }
	printf("stub: sent %d bytes to %s\n", numbytes, Tip);
	
	// receive return msg from server
	// ack timeout 5s
    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd_w, buf, MAXBUFLEN-1 , 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("stub: recvfrom");
        return "ERROR";
    }
	
    printf("stub: received packet from %s\n",
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s));

    printf("stub: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("stub: packet contains: %s\n", buf);


    close(sockfd_w);
	
	return "SUCCESS";
	
}

