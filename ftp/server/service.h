#ifndef _SERVICE_H__
#define _SERVICE_H__

#include "csocket.h"
#include "user.h"
#include "useroper.h"
//#define MSG_LEN 1024


class Service{
private:
	CSocket cs;
	User user;
	bool isLogin;
public:
	Service(CSocket cs);
	void run();
	void cdcmd();
	void lscmd();
	void getcmd();
	void putcmd();
};


#endif //_SERVICE_H__
