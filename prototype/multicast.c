#include "multicast.h"
// local ip, port for dchat
#define MAXBUFLEN 9999;
static int port;
static char* ip;
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
	char* msg, port_s;
	// get local ip, port info; ip:port	
	itoa(port,port_s,10);
	strcat(msg,ip);
	strcat(msg,":");
	strcat(msg,port_s);
	return msg;
}


char* stub_create()
{
    struct addrinfo hints, *servinfo, *p;
    int rv;
	// set timeout val
	struct timeval tv;
	tv.tv_sec = 5;  /* 5 Secs Timeout */
	tv.tv_usec = 0;  // Not init'ing this can cause strange errors
	
	// randomly assign a port number
	port = randomPort();

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM; // use UDP
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
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
		
		// set timeout
		if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval))== -1) {
		            perror("setsockopt");
		            return "ERROR";
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
	
	// update current ip
	struct sockaddr_in localAddress;
	socklen_t addressLength = sizeof(localAddress);
	getsockname(sockfd, (struct sockaddr*)&localAddress, &addressLength);
	printf("local address: %s\n", inet_ntoa(localAddress.sin_addr));
	printf("local port: %d\n", (int) ntohs(localAddress.sin_port));
	
	strcpy(ip, inet_ntoa(localAddress.sin_addr));
	printf("stub: finish binding. ip:port -> %s:%d\n",ip,port);
    printf("stub: waiting to recvfrom...\n");
	return getlocalinfo();
			
}

char* stub_connect(char* Tip, char* Tport)
{
	// used for join, sendto
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
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
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("stub: socket error");
            continue;
        }
		// set timeout
		if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval))== -1) {
		            perror("setsockopt");
		            return "ERROR";
		        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "stub: failed to get addr\n");
        return "ERROR";
    }
    freeaddrinfo(servinfo);
	
	// send connect msg
	char *str;
	strcpy(str,"connect");
    if ((numbytes = sendto(sockfd, str, strlen(str), 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("stub: connect");
        return "ERROR";
    }
	printf("stub: sent %d bytes to %s\n", numbytes, Tip);

	// update IP, port
	struct sockaddr_in localAddress;
	socklen_t addressLength = sizeof(localAddress);
	getsockname(sockfd, (struct sockaddr*)&localAddress, &addressLength);
	printf("local address: %s\n", inet_ntoa(localAddress.sin_addr));
	printf("local port: %d\n", (int) ntohs(localAddress.sin_port));
	
	strcpy(ip, inet_ntoa(localAddress.sin_addr));
	port = (int) ntohs(localAddress.sin_port);
	
	printf("stub: finish binding. ip:port -> %s:%d\n",ip,port);
    printf("stub: waiting to recvfrom...\n");	
	
	return getlocalinfo();
}

char* stub_receive()
{
	struct sockaddr_storage their_addr;
    int numbytes;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];
	char* Sip;

    addr_len = sizeof their_addr;
	
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        return "ERROR";
    }
	
	// store source ip
	Sip = inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
    printf("stub: got packet from %s\n", Sip);
	
	// printf("from port: %hu\n", *get_in_port((struct sockaddr *)&their_addr));
    printf("stub: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("stub: packet contains \"%s\"\n", buf);

	// parse string received
	parsePara(&buf);
}

char* stub_send(char* Tip, char* Tport, char* msg)
{
    int sockfd_w;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes; 
	
	// // receive msg from server, for ack
	//     struct sockaddr_storage their_addr;
	//     char buf[MAXBUFLEN];
	//     socklen_t addr_len;
	//     char s[INET6_ADDRSTRLEN];
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
	printf("stub: msg prepared to send: %s\n",msg);
    if ((numbytes = sendto(sockfd_w, msg, strlen(msg), 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("stub: sendto");
        return "ERROR";
    }
	printf("stub: sent %d bytes to %s\n", numbytes, Tip);
	
}

