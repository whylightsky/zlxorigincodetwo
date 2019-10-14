#include <iostream>
using namespace std;
#include "server.h"
#include <cstdlib>
#include "user.h"
#include "fileoper.h"
#include "useroper.h"
int port = 0;
const char * ip = NULL;
UserOper uo;
User u = {"admin","123456","","/"};
int main(int argc,char *argv[]){
	uo.addUser(u);
	FileOper fo("user.dat");
	fo.load(uo);
	Server s(argv[1],atoi(argv[2]));
	ip = argv[1];
	port = atoi(argv[2]);
	int ret = s.init();
	if(ret == -1){
		cout << "init error" << endl;
		return -1;
	}
	s.accept();
	return 0;
}

