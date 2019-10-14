#ifndef _CLIENT_H__
#define _CLIENT_H__
#include "csocket.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;
class Client{
private:
	string ip;
	short int port;
	int sockfd;
	//CSocket cs;
public:
	Client(string ip,short int port);
	int connect();
	void run();
	int getFd();
};


#endif //_CLIENT_H__
