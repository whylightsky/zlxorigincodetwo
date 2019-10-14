#ifndef _IP_TO_INT_H__
#define _IP_TO_INT_H__

class Ip2Int{
private:
	char ip[32];
public:
	Ip2Int();
	Ip2Int(const char *ip);
	void setIp(const char* ip);
	void getIp(int* arr);
	int getIp();
	char* getIp(int num);
};


#endif //_IP_TO_INT_H__
