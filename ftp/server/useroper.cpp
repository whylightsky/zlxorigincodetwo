#include "useroper.h"

bool UserOper::addUser(User& user){
	map<string,User>::iterator it = users.find(string(user.name));
	if(it != users.end()){
		//已有同名的用户，不能添加
		return false;	
	}
	users[string(user.name)] = user;
	return true;
}

bool UserOper::delUser(User& user){
	map<string,User>::iterator it = users.find(string(user.name));
	if(it != users.end()){
		users.erase(it);
		return true;
	}
	return false;
}

bool UserOper::setUser(User& user){
	map<string,User>::iterator it = users.find(string(user.name));
	if(it != users.end()){
		user = it->second;
		return true;
	}
	return false;
}


list<User> UserOper::getUser(){
	map<string,User>::iterator it = users.begin();
	list<User> listUsers;
	while(it != users.end()){
		listUsers.push_back(it->second);
		++it;
	}
	return listUsers;
}

