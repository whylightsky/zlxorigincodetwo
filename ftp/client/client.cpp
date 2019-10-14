#include "client.h"
#include "cservice.h"
/*
class Client{
private:
	string ip;
	short int port;
	int sockfd;
	//CSocket cs;
public:
	Client(string ip,short int port);
	int connect();
};
*/

Client::Client(string ip,short int port){
	this->ip = ip;
	this->port = port;
}

int Client::connect(){
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		return sockfd;	
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	int ret = ::connect(sockfd,(struct sockaddr*)(&addr),sizeof(addr));
	if(ret == -1){
		return ret;	
	}
}



void Client::run(){	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	CSocket cs(sockfd,addr);
	CService s(cs);
	s.run();
}
int Client::getFd(){
	return sockfd;	
}



