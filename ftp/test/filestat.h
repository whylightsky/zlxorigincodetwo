#ifndef _FILE_STAT_H__
#define _FILE_STAT_H__

#include <sys/stat.h>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <cstring>
#include <string>
using namespace std;
class FileStat{
private:
	string filePath;
public:
	FileStat(string filePath);
	void getStat(char *buf);
	void parsemode(mode_t mode,char *buf);
};

#endif //_FILE_STAT_H__
