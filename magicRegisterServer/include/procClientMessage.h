#ifndef PROCCLIENTMESSAGE_H
#define PROCCLIENTMESSAGE_H

#include "../include/global.h"
#include "../include/structDef.h"
#include "../include/MagicCard.pb.h"

class cProClientMessage
{
public:
	cProClientMessage();
	~cProClientMessage();

	/////////////////////
	//处理资源下载
	//获取系统信息请求处理
	string procSystemInfo(const string& u_strClientData);
	string procResourceDownLoad(const string& u_strClientData);
	string procInvitationCodeAuthentication(const string& u_strClientData);
	string procMainServerRegister(const string& u_strClientData);
	string procMainServerLogin(const string& u_strClientData);
	string createData(const dataPacket& u_dataPacket,const string& u_strBody);
	string procClientReqData(const string& u_strClientData);


protected:
private:
};

extern cProClientMessage g_cProClientMessage;

#endif