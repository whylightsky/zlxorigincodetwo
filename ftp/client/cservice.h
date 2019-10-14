#ifndef _CSERVICE_H__
#define _CSERVICE_H__
#include "csocket.h"
#include <list>
using namespace std;

class CService{
private:
	CSocket cs;
public:
	CService(CSocket cs);
	void run();
	int parse(char *buf,char *& arg);
	void login();
	char *trim(char *buf);
	void lscmd();
	void getcmd(const char *filename);
	void putcmd(const char *filename);
	list<string> getFile(char *arg);
};


#endif //_CSERVICE_H__
