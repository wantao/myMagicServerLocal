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
	//�̵깺������
	string procUserShopBuy(const string& u_strClientData);
	//�̵깺������
	string procUser91ShopBuy(const string& u_strClientData);
	//���տͻ�����Ϣ������Ӧ��Ӧ��Ϣ
	string procClientReqData(const string& u_strClientData);
	string createData(dataPacket& u_dataPacket,const string& u_strBody);
protected:
private:
	
};

extern cProClientMessage g_cProClientMessage;

#endif