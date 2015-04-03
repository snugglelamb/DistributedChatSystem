#include "multicast.h"

char* onReceive(int port)
{    
	int sockfd, length;
	struct sockaddr_in server_addr, client_addr;
	socklen_t len;
	//char readline[100];
	char* readline;
	char* sendline;

	readline = (char*) malloc(500); 

	char command[10];
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);	
	bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	len = sizeof(client_addr);
	length = recvfrom(sockfd, readline, 1000, 0, (struct sockaddr *)&client_addr, &len);
	//readline[length]=0;
	*(readline+length)=0;
	//printf("%s \n",readline);
	return readline;
}

int multiJoin(char* ip, int port, char* userIP, char* userName, int userPort)
{
	printf("ip=%s; port=%d \n", ip, port);
	int sockfd,n;
	struct sockaddr_in server_addr,client_addr;
	char recvline[500];

	socklen_t len;

	int portnumber;
	portnumber= port;

	sockfd=socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(ip);
	server_addr.sin_port=htons(portnumber);

	char portStr[20];
    sprintf(portStr,"%d",userPort);

	char* send;
	char* command = "JOIN";
	send = (char*) malloc(strlen(command)+1+strlen(userIP)+1+strlen(userName)+1+strlen(portStr)); 
	strcpy(send, command);
	strcat(send, " ");
 	strcat(send, userIP);
 	strcat(send, " ");  
    strcat(send, userName);
    strcat(send, " ");  
    strcat(send, portStr);
	int ret=sendto(sockfd,send,strlen(send),0,(struct sockaddr *)&server_addr,sizeof(server_addr));	
	return 1;
}

void sendUserlist(char* ip, int port, char* userlist)
{
	printf("ip=%s; userlist=%s \n", ip, userlist);
	int sockfd,n;
	struct sockaddr_in server_addr,client_addr;
	char recvline[500];

	socklen_t len;

	//int portnumber;
	//portnumber=20000;

	sockfd=socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(ip);
	server_addr.sin_port=htons(port);

	char* send;
	char* command = "USERLIST";
	send=(char*) malloc(strlen(command)+1+strlen(userlist)); 
	strcpy(send, command);
	strcat(send, " ");
    strcat(send,userlist);
	int ret=sendto(sockfd,send,strlen(send),0,(struct sockaddr *)&server_addr,sizeof(server_addr));	
	//n=recvfrom(sockfd,recvline,500,0,NULL,NULL);
	printf("ret=%d\n",ret);
}


void sendNewUser(char* ip, int port, char* newUser)
{
	//printf("ip=%s; userlist=%s \n", ip, userlist);
	int sockfd,n;
	struct sockaddr_in server_addr,client_addr;
	char recvline[500];

	socklen_t len;

	//int portnumber;
	//portnumber=20000;

	sockfd=socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(ip);
	server_addr.sin_port=htons(port);

	char* send;
	char* command = "NEWUSER";
	send=(char*) malloc(strlen(command)+1+strlen(newUser)); 
	strcpy(send, command);
	strcat(send, " ");
    strcat(send,newUser);
	int ret=sendto(sockfd,send,strlen(send),0,(struct sockaddr *)&server_addr,sizeof(server_addr));	
	//n=recvfrom(sockfd,recvline,500,0,NULL,NULL);
	printf("ret=%d\n",ret);
}

void sendSequencer(char* ip, int port, char* sequencer)
{
	printf("ip=%s; sequencer=%s \n", ip, sequencer);
	int sockfd,n;
	struct sockaddr_in server_addr,client_addr;
	char recvline[500];
	socklen_t len;

	//int portnumber;
	//portnumber=20000;

	sockfd=socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(ip);
	server_addr.sin_port=htons(port);

	char* send;
	char* command = "SEQUENCER";
	send=(char*) malloc(strlen(command)+1+strlen(sequencer)); 
	strcpy(send, command);
 	strcat(send," ");  
    strcat(send,sequencer);	
	int ret=sendto(sockfd,send,strlen(send),0,(struct sockaddr *)&server_addr,sizeof(server_addr));	
	//n=recvfrom(sockfd,recvline,500,0,NULL,NULL);
	printf("ret=%d\n",ret);
}

void sendSequenceNumber(char* ip, int port, int sequenceNumber)
{
	printf("ip=%s; sequenceNumber=%d \n", ip, sequenceNumber);
	int sockfd,n;
	struct sockaddr_in server_addr,client_addr;
	char recvline[500];

	socklen_t len;

	//int portnumber;
	//portnumber=20000;

	sockfd=socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(ip);
	server_addr.sin_port=htons(port);

	char number[20];
    //itoa(sequenceNumber,number,10); 
    sprintf(number,"%d",sequenceNumber);

	char* send;
	char* command = "SEQUENCENUMBER";
	send=(char*) malloc(strlen(command)+1+strlen(ip)+1+strlen(number)); 
	strcpy(send, command);
 	strcat(send," ");  
    strcat(send,number);
	int ret=sendto(sockfd,send,strlen(send),0,(struct sockaddr *)&server_addr,sizeof(server_addr));	
	//n=recvfrom(sockfd,recvline,500,0,NULL,NULL);
	printf("ret=%d\n",ret);
}



