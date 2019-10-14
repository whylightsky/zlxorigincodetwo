#include<iostream>
#include <limits.h>
#include <cstdlib>
using namespace std;
int main(){
	char path[128] = {};
	readlink("/home/ubuntu/code",path,128);
	cout << path << endl;
	realpath("/home/ubuntu/code",path);
	cout << path << endl;
	return 0;	
}
