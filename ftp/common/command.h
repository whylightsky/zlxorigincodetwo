#ifndef _COMMAND_H__
#define _COMMAND_H__

#define ERR_LEN 128
#define MSG_LEN 128
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
	SUCCESS=-999,
	NOTLOGIN,
	FAILED,
	FILENOTEXISTS
};

struct ErrInfo{
	int retCode;
	char errInfo[ERR_LEN];
};

/*
strcut ErrInfo errs[] ={
	{SUCCESS,"指令成功"},
	{},
};
*/
#endif //_COMMAND_H__
