#include "csocket.h"
/*
class CSocket{
private:
	int  fd;
public:
	CSocket(int fd,struct sockaddr_in addr);
	int init();
	int getFd();
	void setFd(int fd);
	int send(const char *buf);
	int recv(char *buf,size_t len);
};
*/
CSocket::CSocket(){}

CSocket::CSocket(int fd,struct sockaddr_in addr){
	this->fd = fd;
	this->addr = addr;
}

int CSocket::getFd(){
	return fd;	
}

void CSocket::setFd(int fd){
	this->fd = fd;	
}
/*
int CSocket::init(){
	pthread_t ids;
	int ret = pthread_create(&ids,NULL,pthread_run,(void*)this);
	return ret; 
}

void *CSocket::pthread_run(void *arg){
	CSocket *pcs = (CSocket*)(arg);	
	while(true){
		//pcs->fd;	  //交互
		char buf[1024] = {};
		pcs->recv(buf,1024);
		cout << buf << endl;
		memset(buf,0,1024);
		cin >> buf;
		pcs->send(buf);
	}
}
*/
int  CSocket::send(const char *buf){
	return ::send(fd,buf,strlen(buf),0);	
}

int CSocket::recv(char *buf,size_t len){
	return ::recv(fd,buf,len,0);	
}

int CSocket::send(int cmd){
	cmd = htonl(cmd);
	return ::send(fd,&cmd,4,0);
}
int CSocket::recv(int& cmd){
	int r = ::recv(fd,&cmd,4,0);
	cmd = ntohl(cmd);
	return r;
}

