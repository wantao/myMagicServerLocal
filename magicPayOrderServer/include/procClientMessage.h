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
	//商店购买请求
	string procUserShopBuy(const string& u_strClientData);
	//商店购买请求
	string procUser91ShopBuy(const string& u_strClientData);
	//接收客户端消息处理并回应对应消息
	string procClientReqData(const string& u_strClientData);
	string createData(dataPacket& u_dataPacket,const string& u_strBody);
protected:
private:
	
};

extern cProClientMessage g_cProClientMessage;

#endif