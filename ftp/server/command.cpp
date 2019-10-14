#include "command.h"

struct ErrInfo errinfos[] = {
	{SUCCESS,"指令执行成功!"},
	{NOTLOGIN,"没有登录!"},
	{FAILED,"指令执行失败!"},
	{FILENOTEXISTS,"文件不存在!"},
	{USERNOTEXISTS,"用户不存在!"},
	{PASSWORDERROR,"密码错误!"},
	{INVALIDPATH,"无效的路径!"},
	{NOTPATH,"不是路径!"},
	{PATHNOTEXISTS,"路径不存在!"},
	{UNKNOWN,"未知错误!"},
	{READY,"已准备好数据通道!"},
	{ISDIRBUTFILE,"是目录而非文件!"},
	{OPENFILEFAILED,"打开文件失败!"},
	{TRANS,"准备开始传输数据!"},
	{MDERROR,"文件MD5检验失败!"}
};
const char * getErrInfoByErrorCode(int code){
	for(int i=0;i<sizeof(errinfos)/sizeof(errinfos[0]);i++){
		if(errinfos[i].errCode == code){
			return errinfos[i].errInfo;	
		}	
	}
	return "未知错误";
}
