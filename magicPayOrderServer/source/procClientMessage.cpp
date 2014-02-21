#include "../include/procClientMessage.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/magicServerDb.h"
#include "../include/procUserShopBuy.h"
#include "../include/myGlog.h"




cProClientMessage g_cProClientMessage;

cProClientMessage::cProClientMessage()
{
}

cProClientMessage::~cProClientMessage()
{
}

string cProClientMessage::createData(dataPacket& u_dataPacket,const string& u_strBody)
{
	string u_retData = "";
	u_dataPacket.protobufLength = u_strBody.size();
	u_retData.append((char*)&u_dataPacket,sizeof(dataPacket));
	u_retData.append(u_strBody.c_str(),u_strBody.size());

	return u_retData;
}

//商店购买请求
string cProClientMessage::procUserShopBuy(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procUserShopBuy error\n");
		return u_retData;
	}

	PBShopBuySend u_PBShopBuySend;
	if (!u_PBShopBuySend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procUserShopBuy ParseFromString error\n");
		return u_retData;
	}

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBShopBuySendReceive u_PBShopBuySendReceive;
	int u_result = -1;
	u_result = g_cProcUserShopBuy.setPBShopBuySendReceive(u_PBShopBuySendReceive,u_strToken,u_PBShopBuySend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procUserShopBuy cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBShopBuySendReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//用户查询购买结果请求
string cProClientMessage::procUser91ShopBuy(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procUser91ShopBuy error\n");
		return u_retData;
	}

	PBIAPSend u_PBIAPSend;
	if (!u_PBIAPSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procUser91ShopBuy ParseFromString error\n");
		return u_retData;
	}

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBIAPRecv u_PBIAPRecv;
	int u_result = -1;
	u_result = g_cProcUserShopBuy.setPBIAPRecv(u_PBIAPRecv,u_strToken,u_PBIAPSend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procUser91ShopBuy cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0 || u_result == 1)
	{
		u_PBIAPRecv.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//接收客户端消息处理并回应对应消息
string cProClientMessage::procClientReqData(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());

#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("procClientReqData:nType:%d\n",nDataPacket.nType);
#endif

	switch(nDataPacket.nType)
	{
		case SHOP_BUY:
			u_retData = procUserShopBuy(u_strClientData);
			break;
		case USER_BUY_RESULT_REQ:
			u_retData = procUser91ShopBuy(u_strClientData);
			break;
		default:
			g_cMyGlog.errorlogRecord("recvClientMessageAndSend unkown nType:%d\n",nDataPacket.nType);

	}
	
	return u_retData;
}


