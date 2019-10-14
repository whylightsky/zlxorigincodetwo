#include "client.h"
#include <cstdlib>
#include <cstring>
#include <errno.h>

int main(int argc,char *argv[]){
	Client c(argv[1],atoi(argv[2]));
	int ret = c.connect();
	cout << strerror(errno) << endl;
	c.run();
	return 0;	
}
