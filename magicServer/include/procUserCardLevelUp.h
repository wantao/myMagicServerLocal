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

	//����һ�ſ�������Ľ�Ǯ
	int eatOneCardNeedGold(const int& u_cardLevel,const int& u_cardStarLevel);
	//�����������ļ�Ǯ
	int valueOfCard(const int& u_cardLevel,const int& u_cardStarLevel);
	//���Ƶľ���ֵ
	int expOfCard(const int& u_LevelUpcardLevel,const int& u_LevelUpcardStarLevel,const int& u_cardLevel,const int& u_cardStarLevel);
	//�Ե�һ�ſ����ܻ�õľ���
	int expOfEatCard(const int& u_cardLevel,const int& u_cardStarLevel);
	//��ȡ���Ե����Ƶ����о���
	int expOfAllEatedCard(const int& u_LevelUpcardLevel,const int& u_LevelUpcardStarLevel,const PBCardLevelUpSend &u_PBCardLevelUpSend,const unsigned int& u_userId,\
		int& u_NeedMoney);

	//���ݵȼ����Ǽ��жϿ����Ƿ�������
	bool isCardCanLevelUpByCardLevelAndCardStar(const int& u_cardLevel,const int& u_cardStar);
	//���ݿ����Ǽ��ж�����Ƶȼ�
	int getMaxCardLevelByCardStar(const int& u_cardStar);
protected:
private:
};
extern cProcUserCardLevel g_cProcUserCardLevel;
#endif