#include <iostream>
#include <cstring>
#include "md5.h"
using namespace std;


//#include <openssl/md5.h>

/*
#unsigned char *MD5(const unsigned char *d, unsigned long n,		                         unsigned char *md);

*/
int main(int argc,char *argv[]){
	/*
	const char *p = "Hello world";
	char buf[128] = {};
	MD5((const unsigned char*)p,strlen(p),(unsigned char*)buf);
	cout << buf << endl;
	*/
	char buf[128] = {};
	
	getFileMD5(argv[1], buf); 
	cout << buf << endl;
	return 0;	
}
