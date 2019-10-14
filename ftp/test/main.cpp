#include <iostream>
using namespace std;
#include "filestat.h"
#include "ip2int.h"

int main(int argc,char *argv[]){
	/*FileStat fs(argv[1]);
	char buf[128] = {};
	fs.getStat(buf);
	cout << buf << endl;
	*/
	Ip2Int ip(argv[1]);
	int num = ip.getIp();
	cout << num << endl;
	cout << ip.getIp(num) << endl;
	return 0;	
}
