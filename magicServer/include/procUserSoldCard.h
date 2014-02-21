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

	//获取贩卖卡牌列表
	string getSoldCardItemList(const PBSoldCardSend& u_PBSoldCardSend);
	//获取贩卖卡牌所得的金钱
	int getSoldCardGold(const string& u_soldCardItemList,const unsigned int& u_userId);
	//删除卖掉的卡牌
	void deleteSoldCard(const string& u_soldCardItemList,const unsigned int& u_userId);
	//贩卖卡牌过程处理
	int procSoldCard(const PBSoldCardSend& u_PBSoldCardSend,const unsigned int& u_userId,unsigned int& u_userGold);
	//填充贩卖卡牌信息结构
	int setPBSoldCardReceive(PBSoldCardReceive &u_PBSoldCardReceive,const string& u_strToken,\
		const PBSoldCardSend& u_PBSoldCardSend);
protected:
private:
};

extern cProcUserSoldCard g_cProcUserSoldCard;

#endif