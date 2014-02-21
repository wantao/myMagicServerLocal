#ifndef PROCUSERCARDLEVEL_H
#define PROCUSERCARDLEVEL_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserCardLevel
{
public:
	cProcUserCardLevel();
	~cProcUserCardLevel();
	bool isCardExist(const unsigned int& u_userId,const unsigned& u_itemId,\
		unsigned int& u_cardLevel,unsigned int& u_cardExp,int& u_cardStarLevel,string& u_cardId/*int& u_cardId*/);
	bool getEatedCardExp(const PBCardLevelUpSend &u_PBCardLevelUpSend,unsigned int& u_allCardExp,\
		const unsigned int& u_userId);
	unsigned int getUserGold(const unsigned int& u_userId);
	bool updateUserGold(const unsigned int& u_userId,unsigned int& u_goldNumber);
	bool goldEnough(const PBCardLevelUpSend &u_PBCardLevelUpSend,\
		const unsigned int& u_userId,unsigned int& u_goldNumber,int u_NeedMoney);
	bool updateCardInfo(const unsigned int& u_cardLevel,const unsigned int& u_cardExp,\
		const unsigned int& u_userId,const unsigned int& u_itemId);
	bool cardLevelUp(unsigned int& u_cardLevel,const int& u_maxCardLevel/*const int& u_cardStar*/,unsigned int& u_cardExp,\
		const unsigned int& u_allEatedExp,const unsigned int& u_userId,\
		const unsigned int& u_itemId);

	bool deleteEatedCards(const PBCardLevelUpSend &u_PBCardLevelUpSend,const unsigned int& u_userId);

	bool getCardInfo(PBCard* u_PBCard,const unsigned int& u_userId,const unsigned int& u_itemId);

	int setPBCardLevelUpReceive(PBCardLevelUpReceive& u_PBCardLevelUpReceive,const string& u_strToken,\
		const PBCardLevelUpSend &u_PBCardLevelUpSend);

	//吞噬一张卡牌所需的金钱
	int eatOneCardNeedGold(const int& u_cardLevel,const int& u_cardStarLevel);
	//卡牌能卖出的价钱
	int valueOfCard(const int& u_cardLevel,const int& u_cardStarLevel);
	//卡牌的经验值
	int expOfCard(const int& u_LevelUpcardLevel,const int& u_LevelUpcardStarLevel,const int& u_cardLevel,const int& u_cardStarLevel);
	//吃掉一张卡牌能获得的经验
	int expOfEatCard(const int& u_cardLevel,const int& u_cardStarLevel);
	//获取被吃掉卡牌的所有经验
	int expOfAllEatedCard(const int& u_LevelUpcardLevel,const int& u_LevelUpcardStarLevel,const PBCardLevelUpSend &u_PBCardLevelUpSend,const unsigned int& u_userId,\
		int& u_NeedMoney);

	//根据等级和星级判断卡牌是否能升级
	bool isCardCanLevelUpByCardLevelAndCardStar(const int& u_cardLevel,const int& u_cardStar);
	//根据卡牌星级判断最大卡牌等级
	int getMaxCardLevelByCardStar(const int& u_cardStar);
protected:
private:
};
extern cProcUserCardLevel g_cProcUserCardLevel;
#endif