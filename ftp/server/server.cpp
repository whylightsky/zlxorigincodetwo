#include "service.h"
#include "server.h"
#include "csocket.h"

Server::Server(string ip,short int port){
	this->ip = ip;
	this->port = port;
	sockfd = 0;
}
Server::Server(){
	ip = "";
	port = 0;
	sockfd = 0;
}

void Server::setPort(short int port){
	this->port = port;	
}
int Server::init(){
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		return sockfd;	
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	socklen_t len = sizeof(addr);
	int ret = bind(sockfd,(const struct sockaddr*)(&addr),len);
	if(ret == -1){
		return ret;	
	}
	ret = listen(sockfd,LISTENBACKLOG);
	if(ret == -1){
		return ret;	
	}
}

void * Server::pthread_run(void *arg){
	CSocket* pcs = (CSocket*)arg;
	Service s(*pcs);
	s.run();
	delete pcs;
}
int Server::accept(){
	while(true){
		struct sockaddr_in addr;
		socklen_t len = sizeof(addr);
		cout << "wait client to connect..." << endl;
		int clientfd = ::accept(sockfd,(struct sockaddr*)(&addr),&len);
		cout << clientfd << " connect to server" << endl;
		CSocket* pcs = new CSocket(clientfd,addr);
		pthread_t ids;
		pthread_create(&ids,NULL,pthread_run,(void*)pcs);
		pthread_detach(ids);
	}
}

void Server::accept(int& fd){	
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	cout << "wait client connect to datacommand..." << endl;
	fd = ::accept(sockfd,(struct sockaddr*)(&addr),&len);
}

void Server::close(){
	::close(sockfd);	
}
