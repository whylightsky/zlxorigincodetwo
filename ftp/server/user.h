#ifndef _USER_H__
#define _USER_H__
#include <iostream>
#include <cstring>
#define NAME_LEN 40
#define PASS_LEN 12
#define HOME_LEN 40
#define PATH_LEN 128
//文件共享软件安装路径
#define ROOT_PATH "/home/ubuntu"
using namespace std;
class User{
public:
	char name[NAME_LEN];
	char pass[PASS_LEN];
	char home[HOME_LEN];
	char currPath[PATH_LEN];//初始化的值为　/
	bool operator==(const User& user){
		return strcmp(name,user.name)==0 && 
			strcmp(pass,user.pass)==0;
	}
	friend ostream& operator<<(ostream& os,const User& user){
		return os << user.name << " " << user.pass << " " << user.home << " " << user.currPath ;
	}
	friend istream& operator>>(istream& is,User& user){
		return is >> user.name >> user.pass >> user.home >> user.currPath;	
	}
};


#endif //_USER_H__
