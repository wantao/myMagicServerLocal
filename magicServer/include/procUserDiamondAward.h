#ifndef PROCUSERRUNNINGAFFICHE_H
#define PROCUSERRUNNINGAFFICHE_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserDiamondAward
{
public:
	cProcUserDiamondAward();
	~cProcUserDiamondAward();
	//����û���Ϣ
	bool setPBUser(PBUser* u_PBUser,const unsigned int& u_userId);
	bool getUserDiamond(const unsigned int& u_userId,unsigned int& u_diamondNumber);
	bool updataUserDiamondNumber(const unsigned int& u_userId,unsigned int& u_diamondNumber);
	//���뿨����Ϣ
	bool insertCardInfo(const unsigned int& u_userId,const unsigned int& u_cardId,unsigned int& u_itemId);
	//�������PBAwardReceive
	int proxySetPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int u_Type);
	//���PBAwardReceive
	int setPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const string& u_strToken,const int& u_Type);

	//��ȡ��ʯ�齱�������ʯ����
	unsigned int getNeedDiamondNum(const int& u_Type);
	//��������Ǽ�
	int randCardStarLevel();
	//�����Ǽ��õ���Ӧ�Ǽ��Ŀ������ݿ�
	string getCardStarLevelTable(const int& u_cardStarLevel);
	//���Ǽ��������ݿ������һ��carddigitId
	bool getCardDigitId(const string& u_strCardLevelTable,int& u_digitId);
	//����������ʯ�齱����
	void insertAndSetAwardCard(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int& u_Type);

	#ifdef TEST
	void printUserInfo(const PBUser& u_PBUser);
	void printAllCard(const PBAllCard& u_PBAllCard);
	#endif

protected:
private:
};

extern cProcUserDiamondAward g_cProcUserDiamondAward;

#endif