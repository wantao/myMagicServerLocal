#ifndef PROCUSERAWARDFRIEND_H
#define PROCUSERAWARDFRIEND_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserAwardFriend
{
public:
	cProcUserAwardFriend();
	~cProcUserAwardFriend();
	//����û���Ϣ
	bool setPBUser(PBUser* u_PBUser,const unsigned int& u_userId);
	bool getUserFriendValue(const unsigned int& u_userId,unsigned int& u_friendValue);
	bool updataUserFriendValue(const unsigned int& u_userId,unsigned int u_friendValue);
	//���뿨����Ϣ
	bool insertCardInfo(const unsigned int& u_userId,const unsigned int& u_CardId,unsigned int& u_itemId);
	
	void getCardInfo(PBCard* u_PBCard,const unsigned int& u_userId,const unsigned int& u_itemId);
	//�������PBAwardReceive
	int proxySetPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int& u_Type);
	//���PBAwardReceive
	int setPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const string& u_strToken,const int& u_Type);

	//��ȡ����齱���������ֵ
	unsigned int getNeedFriendValue(const int& u_Type);
	//��������Ǽ�
	int randCardStarLevel();
	//�����Ǽ��õ���Ӧ�Ǽ��Ŀ������ݿ�
	string getCardStarLevelTable(const int& u_cardStarLevel);
	//���Ǽ��������ݿ������һ��carddigitId
	bool getCardDigitId(const string& u_strCardLevelTable,int& u_digitId);
	//������������齱����
	void insertAndSetAwardCard(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int& u_Type);

protected:
private:
};

extern cProcUserAwardFriend g_cProcUserAwardFriend;
#endif