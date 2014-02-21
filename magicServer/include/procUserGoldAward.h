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
	//获取金币抽奖所需的金币值
	unsigned int getNeedGold(const int& u_Type);
	//随机卡牌星级
	int randCardStarLevel();
	//随机一个宝石ID
	bool getGemId(const string& u_strGemTable,int& u_gemType,string& u_gemId);
	//插入和填充金币抽奖卡牌
	void insertAndSetAwardCard(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int& u_Type);
	//代理填充PBAwardReceive
	int proxySetPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int& u_Type);
	//填充PBAwardReceive
	int setPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const string& u_strToken,const int& u_Type);
protected:
private:
};

extern cProUserGoldAward g_cProUserGoldAward;

#endif