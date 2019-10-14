#include "service.h"
#include <fstream>
#include "command.h"
#include "user.h"
#include <limits.h>
#include <unistd.h>
#include <cstdlib>
#include <errno.h>
#include <sys/stat.h>
#include "ip2int.h"
#include "filestat.h"
#include <dirent.h>
#include "server.h"
#include "md5.h"
using namespace std;
extern const char *ip;
extern int port;
extern UserOper uo;
Service::Service(CSocket cs){
	this->cs = cs;	
	isLogin = false;
}

void Service::cdcmd(){	
	char path[CMD_LEN] = {};
	cs.recv(path,CMD_LEN);
	int ret = 0;
	cout << "arg:" << path << endl;
	if(strcmp(path," ")==0){
		strcpy(user.currPath,"/");
		ret = SUCCESS;
		cs.send(ret);
		return;
	}
	string wpath = string(ROOT_PATH)+(string(user.currPath)==string("/")?"/":(string(user.currPath)+string("/")))+string(path);	
	cout << "想要切换到的路径:" << wpath << endl;
	string hpath = string(ROOT_PATH)+(string(user.home)==string("")?string(""):string("/")+string(user.home));
	cout << "用户目录:" << hpath << endl;
	char realp[CMD_LEN] = {};
	realpath(wpath.c_str(),realp);
	cout << "绝对路径:" << realp << endl;
	if(strlen(realp) < strlen(hpath.c_str()) || strncmp(realp,hpath.c_str(),strlen(hpath.c_str())) != 0){
		ret = INVALIDPATH;
		cout << "无效路径" << endl;
		cs.send(ret);
	}else{
		struct stat info;
		int r=stat(realp, &info);
		if(r==0){
			if(!S_ISDIR(info.st_mode)) 
				ret = NOTPATH;
			else{
				strcpy(user.currPath,realp+strlen(hpath.c_str()));
				if(user.currPath[0] == '\0'){
					strcpy(user.currPath,"/");	
				}
				ret = SUCCESS;
			}
		}else{
			if( errno==ENOENT){
				ret = PATHNOTEXISTS;
			}else
				ret = UNKNOWN;
		}
		cs.send(ret);
	}
}
void Service::lscmd(){
	string wpath = string(ROOT_PATH)+(string(user.currPath)==string("/")?"/":(string(user.currPath)+string("/")));
	int i = 2;
	Server s(ip,port+i);
	while(s.init() == -1){//bind地址不可以用　port被占用
		++i;
		s.setPort(port+i);	
	}
	int ret = READY;
	cs.send(ret);//告诉客户端服务器的数据通道已经准备好了
	Ip2Int ipc(ip);
	int ipnum = ipc.getIp();//192.168.4.197
	cs.send(ipnum);//发送ip
	cs.send(port+i);//发送端口
	int fd = 0;
	s.accept(fd);
	int size = 0;
	DIR *dir = opendir(wpath.c_str());
	char buf[MSG_LEN] = {};
	struct dirent *d = NULL;
	while( (d = readdir(dir)) != NULL){
		if(d->d_name[0] == '.'){
			continue;	
		}
		memset(buf,0,MSG_LEN);
		FileStat fs(wpath+string("/")+string(d->d_name));
		fs.getStat(buf);
		for(int i=0;i<strlen(buf);i++){
			if(buf[i] == '\n')buf[i]='\0';	
		}	
		strcat(buf," ");
		strcat(buf,d->d_name);
		strcat(buf,"\r\n");//\r\n  跨平台问题　
		cout << buf ;
		size += ::send(fd,buf,strlen(buf),0);
	}
	::closedir(dir);
	cout << "size:" << size << endl;
	cs.send(size);//命令通道发送　数据通道总共发送的字节数
	ret = SUCCESS;
	cs.send(ret);//命令执行成功
	::close(fd);
}

void Service::getcmd(){
	char buf[MSG_LEN] = {};
	cs.recv(buf,MSG_LEN);
	int ret = 0;
	string wpath = string(ROOT_PATH)+(string(user.currPath)==string("/")?"/":(string(user.currPath)+string("/")));
	string filename = wpath + string(buf);
	cout << filename << endl;
	if(access(filename.c_str(),F_OK)!=0){
		ret = FILENOTEXISTS;
		cs.send(ret);
		return;
	}
	struct stat info;
	int r=stat(filename.c_str(), &info);
	if(r==0){
		if(S_ISDIR(info.st_mode)){
			ret = ISDIRBUTFILE;
			cs.send(ret);
			return;
		}
	}else{
		ret = UNKNOWN;
		cs.send(ret);
		return;
	}
	int i = 2;
	Server s(ip,port+i);
	while(s.init() == -1){//bind地址不可以用　port被占用
		++i;
		s.setPort(port+i);	
	}
	ret = READY;
	cs.send(ret);//告诉客户端服务器的数据通道已经准备好了
	Ip2Int ipc(ip);
	int ipnum = ipc.getIp();//192.168.4.197
	cs.send(ipnum);//发送ip
	cs.send(port+i);//发送端口
	int fd = 0;
	s.accept(fd);
	ifstream ifs(filename.c_str());
	if(ifs.fail()){
		ret = OPENFILEFAILED;
	}else{
		ret = TRANS;
	}
	cs.send(ret);
	size_t size = 0;
	while(true){
		char buf[LINE_LEN] = {};
		ifs.getline(buf,LINE_LEN);
		if(ifs.eof())break;
		strcat(buf,"\n");
		size += ::send(fd,buf,strlen(buf),0);
	}
	ifs.close();
	::close(fd);
	cs.send(size);
	char getmd5[MSG_LEN] = {};
	cs.recv(getmd5,MSG_LEN);
	char md5[MSG_LEN] = {};
	getFileMD5(filename.c_str(),md5);
	if(strcmp(md5,getmd5)==0){
		ret = SUCCESS;
	}else{
		ret = MDERROR;	
	}
	cs.send(ret);
}
void Service::putcmd(){
	char filename[MSG_LEN] = {};
	cs.recv(filename,MSG_LEN);
	//按理来说应该判断在服务器上该文件是否存在！
	string wpath = string(ROOT_PATH)+(string(user.currPath)==string("/")?"/":(string(user.currPath)+string("/")));
	int i = 2;
	Server s(ip,port+i);
	while(s.init() == -1){//bind地址不可以用　port被占用
		++i;
		s.setPort(port+i);	
	}
	int ret = READY;
	cs.send(ret);//告诉客户端服务器的数据通道已经准备好了
	Ip2Int ipc(ip);
	int ipnum = ipc.getIp();//192.168.4.197
	cs.send(ipnum);//发送ip
	cs.send(port+i);//发送端口
	int fd = 0;
	s.accept(fd);
	int size = 0;
	cs.recv(size);
	int cnt = 0;
	ofstream ofs((wpath+string(filename)).c_str(),ios::trunc);
	while(cnt < size){
		char buf[MSG_LEN] = {};
		cnt += ::recv(fd,buf,MSG_LEN-1,0);
		cout << buf ;
		ofs << buf;
	}
	ofs.close();
	::close(fd);
	ret = SUCCESS;
	cs.send(ret);
}
void Service::run(){
	//作为成员
	while(true){
		int cmd = 0;
		int ret = 0;
		ret = cs.recv(cmd);
		if(ret == 0){
			return;	
		}
		cout << "server recv:" << cmd << endl;
		if(cmd == USER){
			cs.recv(user.name,NAME_LEN);
			cout << user.name << endl;
			isLogin = false;
		}else if(cmd == PASS){
			char pass[PASS_LEN] = {};
			cs.recv(pass,PASS_LEN);
			cout << pass << endl;
			if(uo.setUser(user)){
				if(strcmp(pass,user.pass)==0){
					isLogin = true;
					ret = SUCCESS;
				}else{
					ret = PASSWORDERROR;	
				}
			}else{
				ret = USERNOTEXISTS;	
			}
			cs.send(ret);
		}else{
			if(!isLogin){
				ret = NOTLOGIN;
				cs.send(ret);
			}else{
				switch(cmd){
				case LS:
					lscmd();
					break;
				case PWD:
					ret = SUCCESS;
					cs.send(ret);
					cs.send(user.currPath);
					break;
				case CD:
					cdcmd();
					break;
				case BYE:
					return;
				case QUIT:
					return;	
				case GET:
					getcmd();
					break;
				case PUT:
					putcmd();
					break;
				default:
					break;
				}
			}
		}
	}	
}


