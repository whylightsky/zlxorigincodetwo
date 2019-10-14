#ifndef _FILE_OPER_H__
#define _FILE_OPER_H__

#include "useroper.h"

class FileOper{
private:
	string fileName;
public:
	FileOper(string fileName);
	void load(UserOper& uo);
	void save(UserOper& uo);
};



#endif //_FILE_OPER_H__
