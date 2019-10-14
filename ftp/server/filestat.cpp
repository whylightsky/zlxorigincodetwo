#include "filestat.h"


FileStat::FileStat(string filePath){
	this->filePath = filePath;	
}
void FileStat::getStat(char *buf){
	struct stat st = {};
	int ret =  stat(filePath.c_str(),&st);//stat获取软件链接文件所指向的那个文件的信息
	//int ret = lstat(fileName,&st);//lstat获取软链接文件本身的信息
	if(ret == -1){
		return ;
	}
	parsemode(st.st_mode,buf);
	sprintf(buf+10,"  %-10ld  %s  ",st.st_size,ctime(&st.st_ctime));	
}
void FileStat::parsemode(mode_t mode,char *buf){
	if(S_ISDIR(mode)){
		buf[0] = 'd';	
	}else if(S_ISREG(mode)){
		buf[0] = '-';	
	}else if(S_ISLNK(mode)){
		buf[0] = 'l';	
	}else if(S_ISBLK(mode)){
		buf[0] = 'b';	
	}else if(S_ISCHR(mode)){
		buf[0] = 'c';	
	}else if(S_ISSOCK(mode)){
		buf[0] = 's';
	}else if(S_ISFIFO(mode)){
		buf[0] = 'p';	
	}
	
	strcat(buf,mode&S_IRUSR?"r":"-");
	strcat(buf,mode&S_IWUSR?"w":"-");
	strcat(buf,mode&S_IXUSR?"x":"-");

	strcat(buf,mode&S_IRGRP?"r":"-");
	strcat(buf,mode&S_IWGRP?"w":"-");
	strcat(buf,mode&S_IXGRP?"x":"-");

	strcat(buf,mode&S_IROTH?"r":"-");
	strcat(buf,mode&S_IWOTH?"w":"-");
	strcat(buf,mode&S_IXOTH?"x":"-");
	
	if((mode&S_ISUID)&&buf[3]=='x'){
		buf[3] = 's';	
	} 
	if((mode&S_ISGID)&&buf[6]=='x'){
		buf[6] = 's';	
	}
}

