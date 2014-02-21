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
	//填充用户信息
	bool setPBUser(PBUser* u_PBUser,const unsigned int& u_userId);
	bool getUserFriendValue(const unsigned int& u_userId,unsigned int& u_friendValue);
	bool updataUserFriendValue(const unsigned int& u_userId,unsigned int u_friendValue);
	//插入卡牌信息
	bool insertCardInfo(const unsigned int& u_userId,const unsigned int& u_CardId,unsigned int& u_itemId);
	
	void getCardInfo(PBCard* u_PBCard,const unsigned int& u_userId,const unsigned int& u_itemId);
	//代理填充PBAwardReceive
	int proxySetPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int& u_Type);
	//填充PBAwardReceive
	int setPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const string& u_strToken,const int& u_Type);

	//获取友情抽奖所需的友情值
	unsigned int getNeedFriendValue(const int& u_Type);
	//随机卡牌星级
	int randCardStarLevel();
	//根据星级得到对应星级的卡牌数据库
	string getCardStarLevelTable(const int& u_cardStarLevel);
	//从星级卡牌数据库中随机一个carddigitId
	bool getCardDigitId(const string& u_strCardLevelTable,int& u_digitId);
	//插入和填充友情抽奖卡牌
	void insertAndSetAwardCard(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int& u_Type);

protected:
private:
};

extern cProcUserAwardFriend g_cProcUserAwardFriend;
#endif