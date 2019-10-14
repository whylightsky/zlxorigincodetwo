#ifndef _USER_OPER_H__
#define _USER_OPER_H__

#include "user.h"
#include <map>
#include <list>
#include <string>
using namespace std;

class UserOper{
private:
	map<string,User> users;
public:
	bool addUser(User& user);
	bool delUser(User& user);
	//根据用户名查找用户，把该用户的信息设置到引用参数中
	bool setUser(User& user);
	list<User> getUser();
};


#endif //_USER_OPER_H__
