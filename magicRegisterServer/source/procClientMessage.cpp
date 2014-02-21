#include "../include/procClientMessage.h"
#include "../include/define.h"
#include "../include/CSocketBase.h"
#include "../include/magicServerDb.h"
#include "../include/procUserResourceDownLoad.h"
#include "../include/myGlog.h"
#include "../include/procUserInvitationCode.h"
#include "../include/procMainServerRegister.h"
#include "../include/procMainServerLogin.h"


cProClientMessage g_cProClientMessage;

cProClientMessage::cProClientMessage()
{
}

cProClientMessage::~cProClientMessage()
{
}


//获取系统信息请求处理
string cProClientMessage::procSystemInfo(const string& u_strClientData)
{
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	//包格式 dataPacket +　protobufData;
	PBSystemInfoReceive u_PBSystemInfoReceive;
	string strReturnData = "";
	string strProtobufData = "";

	g_cMagicServerDb.setPBSystemInfoReceive(u_PBSystemInfoReceive);
	u_PBSystemInfoReceive.SerializeToString(&strProtobufData);

	nDataPacket.protobufLength = strProtobufData.size();

	strReturnData.append((char*)&nDataPacket,sizeof(nDataPacket));
	strReturnData.append(strProtobufData.c_str(),strProtobufData.size());

	return strReturnData;
}

//处理资源下载
string cProClientMessage::procResourceDownLoad(const string& u_strClientData)
{
	string u_retData = "";
	string strProtobufData = "";
	dataPacket u_dataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&u_dataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),u_dataPacket.protobufLength);
	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procResourceDownLoad error\n");
		return u_retData;
	}

	PBDownloadSend u_PBDownloadSend;
	if (!u_PBDownloadSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procResourceDownLoad ParseFromString error\n");
		return u_retData;
	}
	//resourceDataMap::iterator it = g_resourceDataMap.find(u_PBDownloadSend.resourceversion());
	string u_strBody = "";
	/*if (it != g_resourceDataMap.end())
	{
		int u_size = (it->second).size();
		int u_pointMark = u_PBDownloadSend.pointmark();
		u_strBody = (it->second).substr(u_pointMark,u_size - u_pointMark);
	}*/
	g_cMagicServerDb.getResorceVersionPath(u_PBDownloadSend.resourceversion(),u_strBody);
	u_dataPacket.protobufLength = u_strBody.size();
	//test
	u_retData = createData(u_dataPacket,u_strBody);
	//cerr<<"66666"<<endl;
	return u_retData;
}

string cProClientMessage::procInvitationCodeAuthentication(const string& u_strClientData)
{
	//cerr<<"procInvitationCodeAuthentication"<<endl;
	string u_retData = "";
	string strProtobufData = "";
	dataPacket u_dataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&u_dataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),u_dataPacket.protobufLength);
	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procInvitationCodeAuthentication error\n");
		return u_retData;
	}

	PBInvitationCodeSend u_PBInvitationCodeSend;
	if (!u_PBInvitationCodeSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procInvitationCodeAuthentication ParseFromString error\n");
		return u_retData;
	}
	PBInvitationCodeReceive u_PBInvitationCodeReceive;
	g_cProcUserInvitationCode.setPBInvitationCodeReceive(u_PBInvitationCodeReceive,u_PBInvitationCodeSend);
	string u_strBody = "";
	u_PBInvitationCodeReceive.SerializeToString(&u_strBody);
	u_dataPacket.protobufLength = u_strBody.size();
	u_retData = createData(u_dataPacket,u_strBody);
	return u_retData;
}

string cProClientMessage::procMainServerRegister(const string& u_strClientData)
{
	g_cMyGlog.debugLog("in procMainServerRegister\n");
	string u_retData = "";
	string strProtobufData = "";
	dataPacket u_dataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&u_dataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),u_dataPacket.protobufLength);
	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procMainServerRegister error\n");
		return u_retData;
	}

	PBMainServerRegisterSend u_PBMainServerRegisterSend;
	if (!u_PBMainServerRegisterSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procMainServerRegister ParseFromString error\n");
		return u_retData;
	}
	PBMainServerRegisterReceive u_PBMainServerRegisterReceive;
	g_cProcMainServerRegister.setPBMainServerRegisterReceive(u_PBMainServerRegisterReceive,u_PBMainServerRegisterSend);
	string u_strBody = "";
	u_PBMainServerRegisterReceive.SerializeToString(&u_strBody);
	u_dataPacket.protobufLength = u_strBody.size();
	u_retData = createData(u_dataPacket,u_strBody);
	return u_retData;
}

string cProClientMessage::procMainServerLogin(const string& u_strClientData)
{
	g_cMyGlog.debugLog("in procMainServerLogin\n");
	string u_retData = "";
	string strProtobufData = "";
	dataPacket u_dataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&u_dataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),u_dataPacket.protobufLength);
	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procMainServerLogin error\n");
		return u_retData;
	}

	PBMainServerLoginSend u_PBMainServerLoginSend;
	if (!u_PBMainServerLoginSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procMainServerLogin ParseFromString error\n");
		return u_retData;
	}
	PBMainServerLoginReceive u_PBMainServerLoginReceive;
	g_cProcMainServerLogin.setPBMainServerLoginReceive(u_PBMainServerLoginReceive,u_PBMainServerLoginSend);
	string u_strBody = "";
	u_PBMainServerLoginReceive.SerializeToString(&u_strBody);
	u_dataPacket.protobufLength = u_strBody.size();
	u_retData = createData(u_dataPacket,u_strBody);
	return u_retData;
}

string cProClientMessage::createData(const dataPacket& u_dataPacket,const string& u_strBody)
{
	string u_retData = "";
	u_retData.append((char*)&u_dataPacket,sizeof(dataPacket));
	u_retData.append(u_strBody.c_str(),u_strBody.size());

	return u_retData;
}

string cProClientMessage::procClientReqData(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	g_cMyGlog.debugLog("nType:%d\n",nDataPacket.nType);
	switch(nDataPacket.nType)
	{
		case SYSTEMINFO:
			u_retData = procSystemInfo(u_strClientData);
			break;
		case RESOURCE_DOWNLOAD_REQ:
			u_retData = procResourceDownLoad(u_strClientData);
			break;
		case INVITATION_CODE_AUTHENTICATION:
			u_retData = procInvitationCodeAuthentication(u_strClientData);
			break;
		case MAIN_SERVER_REGIST_REQ:
			u_retData = procMainServerRegister(u_strClientData);
			break;
		case MAIN_SERVER_LOGIN_REQ:
			u_retData = procMainServerLogin(u_strClientData);
			break;
		default:
			g_cMyGlog.errorlogRecord("recvClientMessageAndSend unkown nType:%d\n",nDataPacket.nType);
	}
	return u_retData;
}
