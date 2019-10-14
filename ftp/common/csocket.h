#ifndef _CSOCKET_H__
#define _CSOCKET_H__
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
using namespace std;
class CSocket{
private:
	int  fd;
	struct sockaddr_in addr;
public:
	CSocket(int fd,struct sockaddr_in addr);
	//static void *pthread_run(void *arg);
	//int init();
	int getFd();
	void setFd(int fd);
	int send(const char *buf);
	int send(int cmd);
	int recv(char *buf,size_t len);
	int recv(int& cmd);
};


#endif //_CSOKCET_H__
