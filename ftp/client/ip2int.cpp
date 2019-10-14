#include "ip2int.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
/*
private:
	char ip[32];
public:
	Ip2Int(const char *ip);
	void setIp(const char* ip);
	void getIp(int* arr);
};
*/
Ip2Int::Ip2Int(){}
Ip2Int::Ip2Int(const char *ip){
	strcpy(this->ip,ip);	
}
void Ip2Int::setIp(const char *ip){
	strcpy(this->ip,ip);	
}
void Ip2Int::getIp(int* arr){
	size_t len = strlen(ip);
	int cnt = 0;
	char *beg = ip;
	char *ip = this->ip;
	for(;cnt<4;cnt++){
		while(*ip != '.' && *ip != '\0'){
			++ip;	
		}
		*ip = '\0';
		arr[cnt] = atoi(beg);
		beg = ip+1;
		++ip;
	}
}

int Ip2Int::getIp(){
	int arr[4] = {};
	getIp(arr);
	int num = 0;
	for(int i=0;i<4;i++){
		num = (num << 8) | arr[i];	
	}
	return num;
}
char* Ip2Int::getIp(int num){
	int arr[4] = {};
	for(int i=0;i<4;i++){
		arr[3-i] = (num >> 8*i)&0xff;	
	}
	memset(ip,0,32);
	sprintf(ip,"%d.%d.%d.%d",arr[0],arr[1],arr[2],arr[3]);
	return ip;
}
