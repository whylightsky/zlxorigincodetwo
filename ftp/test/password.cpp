#include "password.h"
#include <iostream>
using namespace std;

int getch(){
	int c=0;
	struct termios org_opts, new_opts;
    int res=0;
    //-----  store old settings -----------
	res=tcgetattr(STDIN_FILENO, &org_opts);
	assert(res==0);
	//---- set new terminal parms --------
	memcpy(&new_opts, &org_opts, sizeof(new_opts));
	new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
	c=getchar();
	//------  restore old settings ---------
	res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
	assert(res==0);
	return c;
}
void inputPassword(char *pass){
	int i;
	char a;
	for(i=0;;i++){
		if(i<0) i = 0;
		pass[i]=getch();
		//cout << "input:" << pass[i] << endl;
		if(pass[i]=='\n'){
			pass[i]='\0';
			break;
		}
		if(pass[i]==127){
			if(i>0){
				//printf("\b \b");
				cout << "\b \b" << flush;
			}
			i=i-2;
		}else{
			//printf("*");
			cout << "*" << flush;
		}
		if(i<0)
			pass[0]='\0';
	}
}

