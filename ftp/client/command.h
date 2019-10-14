#ifndef _COMMAND_H__
#define _COMMAND_H__
#define USER_LEN 40
#define ERR_LEN 128
#define MSG_LEN 128
#define CMD_LEN 128
#define LINE_LEN 1024
enum CMD{
	LS = 999,
	PWD,
	TOUCH,
	MKDIR,
	LOGIN,
	USER,
	PASS,
	CD,
	GET,
	GETS,
	PUT,
	PUTS,
	QUIT,
	BYE,
	HELP,
	CLEAR,
	RM,
	FIND,
	GREP
};

enum RetCode{
	SUCCESS=200,
	NOTLOGIN,
	FAILED,
	FILENOTEXISTS,
	USERNOTEXISTS,
	PASSWORDERROR,
	INVALIDPATH,
	NOTPATH,
	PATHNOTEXISTS,
	UNKNOWN,
	READY,
	ISDIRBUTFILE,
	OPENFILEFAILED,
	TRANS,
	MDERROR
};

struct ErrInfo{
	int errCode;
	char errInfo[ERR_LEN];
};

const char * getErrInfoByErrorCode(int code);


#endif //_COMMAND_H__
