#ifndef PROCUSERGOLDAWARD_H
#define PROCUSERGOLDAWARD_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProUserGoldAward
{
public:
	cProUserGoldAward();
	~cProUserGoldAward();
	//��ȡ��ҳ齱����Ľ��ֵ
	unsigned int getNeedGold(const int& u_Type);
	//��������Ǽ�
	int randCardStarLevel();
	//���һ����ʯID
	bool getGemId(const string& u_strGemTable,int& u_gemType,string& u_gemId);
	//���������ҳ齱����
	void insertAndSetAwardCard(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int& u_Type);
	//�������PBAwardReceive
	int proxySetPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int& u_Type);
	//���PBAwardReceive
	int setPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const string& u_strToken,const int& u_Type);
protected:
private:
};

extern cProUserGoldAward g_cProUserGoldAward;

#endif