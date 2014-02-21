#ifndef PROCUSERHELPFRIEND_H
#define PROCUSERHELPFRIEND_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserHelpFriend
{
public:
	cProcUserHelpFriend();
	~cProcUserHelpFriend();
	int setPBHelpFriend(PBHelpFriend& u_PBHelpFriend,const string& u_strToken);
	//从好友表里边取
	bool getAllHelpFriendByUserId(PBHelpFriend& u_PBHelpFriend,const unsigned& u_userId);
	//从用户表中获取助战基友
	bool getHelpFriendFromUserTable(PBHelpFriend& u_PBHelpFriend,const unsigned int& u_userId);

	//根据用户Id获取战斗卡牌的战斗档次
	int getUserCapacityClassByUserId(const unsigned int& u_userId);
	//根据随机数据及用户战斗卡牌的档次获取随机路人的档次
	int getRandCapacityClass(const int& u_userClass,const int& u_randData);
	//根据路人的战斗卡牌的档次获取路人的ID
	int getPlayerIdByRandClass(int u_randClass,const string& u_hasUsedIdList);
	//根据用户战斗卡牌的档次获取路人Id
	int getPlayerIdByUserCapacityClass(const int& u_userCapacityClass,const string& u_hasUsedIdList);
protected:
private:
};

extern cProcUserHelpFriend g_cProcUserHelpFriend;
#endif