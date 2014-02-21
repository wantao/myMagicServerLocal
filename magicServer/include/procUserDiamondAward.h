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
	//填充用户信息
	bool setPBUser(PBUser* u_PBUser,const unsigned int& u_userId);
	bool getUserDiamond(const unsigned int& u_userId,unsigned int& u_diamondNumber);
	bool updataUserDiamondNumber(const unsigned int& u_userId,unsigned int& u_diamondNumber);
	//插入卡牌信息
	bool insertCardInfo(const unsigned int& u_userId,const unsigned int& u_cardId,unsigned int& u_itemId);
	//代理填充PBAwardReceive
	int proxySetPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int u_Type);
	//填充PBAwardReceive
	int setPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const string& u_strToken,const int& u_Type);

	//获取钻石抽奖所需的钻石数量
	unsigned int getNeedDiamondNum(const int& u_Type);
	//随机卡牌星级
	int randCardStarLevel();
	//根据星级得到对应星级的卡牌数据库
	string getCardStarLevelTable(const int& u_cardStarLevel);
	//从星级卡牌数据库中随机一个carddigitId
	bool getCardDigitId(const string& u_strCardLevelTable,int& u_digitId);
	//插入和填充钻石抽奖卡牌
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