#include "cservice.h"
#include "client.h"
#include "ip2int.h"
#include "command.h"
#include <fstream>
//#include "password.h"
#include "md5.h"
#include <list>
#include <sys/stat.h>

using namespace std;

CService::CService(CSocket cs):cs(cs){
	
}

void CService::login(){
	cout << "user:" ;
	char buf[USER_LEN] = {};
	cin >> buf;
	//多余的空格
	int cmd = USER;
	cs.send(cmd);
	cs.send(buf);
	memset(buf,0,USER_LEN);
	cout << "pass:";
	//inputPassword(buf);
	cin >> buf;
	cmd = 0;
	cmd = PASS;
	cs.send(cmd);
	cs.send(buf);
	int ret = 0;
	cs.recv(ret);	
	cout << getErrInfoByErrorCode(ret) << endl;
	cin.getline(buf,100,'\n');
}
char *CService::trim(char *buf){
	while(*buf == ' '|| *buf == '\t' || *buf == '\r' || *buf=='\n'){
		++buf;	
	}
	while(buf[strlen(buf)-1] == ' ' || buf[strlen(buf)-1] == '\t' || buf[strlen(buf)-1] == '\r' || buf[strlen(buf)-1] == '\n'){
		buf[strlen(buf)-1] = '\0';	
	}
	return buf;
}
int CService::parse(char *buf,char* &arg){
	if(buf == NULL){
		return -1;	
	}
	buf = trim(buf);
	if(strncasecmp(buf,"ls",2)==0){
		return LS;	
	}
	if(strncasecmp(buf,"pwd",3)==0){
		return PWD;	
	}
	if(strncasecmp(buf,"get ",4) == 0){
		arg = trim(buf+4);
		return GET;
	}
	if(strncasecmp(buf,"gets ",5)==0){
		arg = trim(buf+5);
		return GETS;	
	}
	if(strncasecmp(buf,"put ",4) == 0){
		arg = trim(buf+4);
		return PUT;
	}
	if(strncasecmp(buf,"puts ",5)==0){
		arg = trim(buf+5);
		return PUTS;
	}
	if(strncasecmp(buf,"cd ",3)==0){
		arg = trim(buf+2);
		return CD;	
	}
	if(strcasecmp(buf,"login")==0){
		return LOGIN;	
	}
	if(strcasecmp(buf,"quit")==0){
		return QUIT;	
	}
	if(strcasecmp(buf,"bye")==0){
		return BYE;	
	}
	return -1;
}

list<string> CService::getFile(char *arg){
	list<string> files;
	char *p = arg;
	while(arg != NULL && *arg != '\0'){
		if(*arg == ' '){
			*arg = '\0';
			if(p != NULL && strlen(p) != 0){
				files.push_back(string(p));	
			}
			arg = trim(arg+1);
			p = arg;
		}else{
			++arg;	
		}
	}	
	if(p != NULL && strlen(p) != 0){
		files.push_back(string(p));	
	}
	return files;
}

void CService::run(){
	login();
	while(true){
		cout << "ftp> ";
		char buf[CMD_LEN] = {};
		int ret = 0;
		//cin >> buf;
		cin.getline(buf,CMD_LEN,'\n');
		trim(buf);
		if(string(buf)==string("")){
			cout << endl;
			continue;	
		}
		char *arg = NULL;
		int cmd = parse(buf,arg);
		if(cmd == -1){
			cout << "无效的指令!" << endl;
			continue;
		}
		if(cmd==PUT){
			while(arg==NULL||strlen(arg)==0){
				cin.getline(buf,CMD_LEN,'\n');
				arg = trim(buf);
			}
			if(access(arg,F_OK)!=0){
				cout << arg << "文件不存在!" << endl;
				continue;
			}
			putcmd(arg);
			continue;
		}
		if(cmd==PUTS){
			while(arg==NULL||strlen(arg)==0){
				cin.getline(buf,CMD_LEN,'\n');
				arg = trim(buf);
			}
			cout << arg << endl;
			list<string> files = getFile(arg);
			list<string>::iterator it = files.begin();
			for(;it != files.end();++it){
				putcmd((*it).c_str());
			}
			continue;
		}
		cs.send(cmd);
		switch(cmd){
			case PWD:
				cs.recv(ret);
				if(ret == SUCCESS){
					char path[CMD_LEN] = {};
					cs.recv(path,CMD_LEN);
					cout << path << endl;
				}else{
					cout << getErrInfoByErrorCode(ret) << endl;	
				}
				break;
			case CD:
				cout << "arg:" <<arg << endl;
				if(strlen(arg)==0){//不带参数
					char s[2] = " ";
					cs.send(s);
				}else{
					cs.send(arg);//发送参数
				}
				cs.recv(ret);
				cout << getErrInfoByErrorCode(ret) << endl;
				break;
			case LS:
				lscmd();				
				break;
			case LOGIN:
				login();
				break;
			case BYE:
				return;
			case QUIT:
				return;
			case GET:
				while(arg==NULL||strlen(arg)==0){
					cin.getline(buf,CMD_LEN,'\n');
					arg = trim(buf);
				}
				cs.send(arg);
				getcmd(arg);
				break;
			case GETS:
				while(arg==NULL||strlen(arg)==0){
					cin.getline(buf,CMD_LEN,'\n');
					arg = trim(buf);
				}
				cout << arg << endl;
				list<string> files = getFile(arg);
				list<string>::iterator it = files.begin();
				for(;it != files.end();++it){
					cs.send(GET);
					cs.send((*it).c_str());
					getcmd((*it).c_str());
				}
			break;
		}
	}	
}
void CService::putcmd(const char *filename){
	if(filename==NULL){
		return;	
	}
	if(access(filename,F_OK)!=0){
		cout << filename << " 不存在！" << endl;	
		return;
	}	
	struct stat info;
	int r = stat(filename,&info);
	if(r==0 && S_ISDIR(info.st_mode)){
		cout << filename << "是一个目录!" << endl;
		return;
	}
	ifstream ifs(filename);
	if(ifs.fail()){
		cout << "打开" << filename << "失败!" << endl;
		return;
	}
	cs.send(PUT);
	cs.send(filename);
	int ret = 0;
	cs.recv(ret);
	if(ret == READY){
		int ipnum = 0, port = 0;
		cs.recv(ipnum);
		cs.recv(port);
		Ip2Int ipc;
		char ips[32] = {};
		strcpy(ips,ipc.getIp(ipnum));
		cout << "recv ip:" << ips << endl;
		cout << "recv port:" << port << endl;
		Client c(ips,port);
		c.connect();
		int fd = c.getFd();
		int size = 0;
		while(true){
			char buf[LINE_LEN] = {};
			ifs.getline(buf,LINE_LEN,'\n');
			if(ifs.eof())break;
			strcat(buf,"\n");
			size += ::send(fd,buf,strlen(buf),0);
		}
		ifs.close();
		cs.send(size);
		::close(fd);
		cs.recv(ret);
		cout << getErrInfoByErrorCode(ret) << endl;	
	}else{
		cout << getErrInfoByErrorCode(ret) << endl;	
	}
}
void CService::getcmd(const char *filename){
	int ret = 0;
	cs.recv(ret);
	if(ret == READY){
		int ipnum = 0, port = 0;
		cs.recv(ipnum);
		cs.recv(port);
		Ip2Int ipc;
		char ips[32] = {};
		strcpy(ips,ipc.getIp(ipnum));
		cout << "recv ip:" << ips << endl;
		cout << "recv port:" << port << endl;
		Client c(ips,port);
		c.connect();
		int fd = c.getFd();
		int size = 0;
		cs.recv(size);
		//下载应该判断本地是否有同名的文件　　是否需要替换
		ofstream ofs(filename);
		if(ofs.fail()){
			cout << "open " << filename << " failed!" << endl;	
		}
		cs.recv(size);
		int r = 0;
		while(r < size){
			char buf[LINE_LEN] = {};
			r += ::recv(fd,buf,LINE_LEN-1,0);
			ofs<<buf;
		}
		ofs.close();
		::close(fd);
		char md5[MSG_LEN] = {};
		getFileMD5(filename, md5);
		cs.send(md5);
		cs.recv(ret);
		cout << "get " << filename << getErrInfoByErrorCode(ret) << endl;

	}else{
		cout << getErrInfoByErrorCode(ret) << endl;
	}
}
void CService::lscmd(){
	int ret = 0;
	cs.recv(ret);
	cout << getErrInfoByErrorCode(ret) << endl;
	if(ret == READY){
		//接收ip和port
		int ipnum = 0, port = 0;
		cs.recv(ipnum);
		cs.recv(port);
		Ip2Int ipc;
		char ips[32] = {};
		strcpy(ips,ipc.getIp(ipnum));
		cout << "recv ip:" << ips << endl;
		cout << "recv port:" << port << endl;
		Client c(ips,port);
		c.connect();
		int fd = c.getFd();
		int size = 0;
		cs.recv(size);
		cout << "recv size:" << size << endl;
		char buf[MSG_LEN] = {};
		int s = 0;
		while(s < size){
			memset(buf,0,MSG_LEN);
			s += ::recv(fd,buf,MSG_LEN-1,0);
			cout << buf;
		}
		close(fd);
		cs.recv(ret);
		cout << endl;
		cout << getErrInfoByErrorCode(ret) << endl;
	}
}



