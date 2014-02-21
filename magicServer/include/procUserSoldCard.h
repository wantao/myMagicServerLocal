#ifndef PROCUSERSOLDCARD_H
#define PROCUSERSOLDCARD_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;


class cProcUserSoldCard
{
public:
	cProcUserSoldCard();
	~cProcUserSoldCard();

	//��ȡ���������б�
	string getSoldCardItemList(const PBSoldCardSend& u_PBSoldCardSend);
	//��ȡ�����������õĽ�Ǯ
	int getSoldCardGold(const string& u_soldCardItemList,const unsigned int& u_userId);
	//ɾ�������Ŀ���
	void deleteSoldCard(const string& u_soldCardItemList,const unsigned int& u_userId);
	//�������ƹ��̴���
	int procSoldCard(const PBSoldCardSend& u_PBSoldCardSend,const unsigned int& u_userId,unsigned int& u_userGold);
	//��䷷��������Ϣ�ṹ
	int setPBSoldCardReceive(PBSoldCardReceive &u_PBSoldCardReceive,const string& u_strToken,\
		const PBSoldCardSend& u_PBSoldCardSend);
protected:
private:
};

extern cProcUserSoldCard g_cProcUserSoldCard;

#endif