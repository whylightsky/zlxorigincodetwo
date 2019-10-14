#ifndef _SERVER_H__
#define _SERVER_H__
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

#define LISTENBACKLOG 100

class Server{
private:
	string ip;       //服务器的ip
	short int port;  //端口
	int sockfd;
public:
	Server(string ip,short int port);
	Server();
	void setPort(short int port);
	static void *pthread_run(void *arg);
	int init();
	int accept();
	void accept(int& fd);
	void close();
};


#endif //_SERVER_H__
