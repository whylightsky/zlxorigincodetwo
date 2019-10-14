#include "fileoper.h"
#include <fstream>
using namespace std;
/*
class FileOper{
private:
	string fileName;
public:
	FileOper(string fileName);
	void load(UserOper& uo);
	void save(UserOper& uo);
};
*/

FileOper::FileOper(string fileName){
	this->fileName = fileName;	
}

void FileOper::load(UserOper& uo){
	ifstream ifs(fileName.c_str());
	while(!ifs.eof()){
		User user = {};	
		ifs >> user;
		if(user.name[0] == '\0'){
			break;	
		}
		uo.addUser(user);
	}
	ifs.close();
}

void FileOper::save(UserOper& uo){
	ofstream ofs(fileName.c_str());
	list<User> users = uo.getUser();
	list<User>::iterator it = users.begin();
	for(;it != users.end(); ++it){
		ofs << *it << " ";	
	}
	ofs.close();
}

