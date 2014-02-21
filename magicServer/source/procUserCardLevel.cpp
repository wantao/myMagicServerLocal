#include "../include/procUserCardLevelUp.h"
#include "../include/procUserLogin.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"



cProcUserCardLevel g_cProcUserCardLevel;

cProcUserCardLevel::cProcUserCardLevel()
{

}
cProcUserCardLevel::~cProcUserCardLevel()
{

}

bool cProcUserCardLevel::isCardExist(const unsigned int& u_userId,const unsigned& u_itemId,\
				 unsigned int& u_cardLevel,unsigned int& u_cardExp,int& u_cardStarLevel,string& u_cardId/*int& u_cardId*/)
{
	char buf[128] = {0};
	sprintf(buf,"select cardId,cardLevel,cardExp,cardStarLevel from m_userCardInfo%d where itemId = %d;",u_userId%USER_CARD_TABLE_COUNT,u_itemId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("isCardExist: card is not exist itemID:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}

	//u_cardId = atoi((q.getStringFileds("cardId")).c_str());
	u_cardId = q.getStringFileds("cardId");
	u_cardLevel = q.getIntFileds("cardLevel");
	u_cardExp = q.getIntFileds("cardExp");
	u_cardStarLevel = q.getIntFileds("cardStarLevel");
	q.finalize();
	return true;
}

bool cProcUserCardLevel::getEatedCardExp(const PBCardLevelUpSend &u_PBCardLevelUpSend,unsigned int& u_allCardExp,\
					 const unsigned int& u_userId)
{
	string strTmp = "(";
	char bufTmp[12] = {0};
	for (int i = 0; i < u_PBCardLevelUpSend.eatcarditemids_size();i ++)
	{
		memset(bufTmp,0,sizeof(bufTmp));
		if (i == u_PBCardLevelUpSend.eatcarditemids_size()-1)
		{
			sprintf(bufTmp,"%d)",u_PBCardLevelUpSend.eatcarditemids(i));
			strTmp += bufTmp;
		}
		else
		{
			sprintf(bufTmp,"%d,",u_PBCardLevelUpSend.eatcarditemids(i));
			strTmp += bufTmp;
		}
		
	}
	char buf[128] = {0};
	sprintf(buf,"select cardExp from m_userCardInfo%d where itemId in %s;",u_userId%USER_CARD_TABLE_COUNT,strTmp.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getEatedCardExp: card is not exist:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	while (!q.eof())
	{
		u_allCardExp += q.getIntFileds("cardExp");
		q.nextRow();
	}
	q.finalize();
	return true;
}

unsigned int cProcUserCardLevel::getUserGold(const unsigned int& u_userId)
{
	unsigned int u_goldNumber = 0;
	char buf[128] = {0};
	sprintf(buf,"select goldNumber from m_gameUserInfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserGold failure :%s\n",strSql.c_str());
		q.finalize();
		return u_goldNumber;
	}
	u_goldNumber = q.getIntFileds("goldNumber");
	q.finalize();
	return u_goldNumber;
}

bool cProcUserCardLevel::updateUserGold(const unsigned int& u_userId,unsigned int& u_goldNumber)
{
	char buf[128] = {0};
	sprintf(buf,"update m_gameUserInfo set goldNumber = %d where userID = %d;",u_goldNumber,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUserGold failure:%s\n",strSql.c_str());
		return false;
	}
	return true;
}

bool cProcUserCardLevel::goldEnough(const PBCardLevelUpSend &u_PBCardLevelUpSend,\
				const unsigned int& u_userId,unsigned int& u_goldNumber,int u_NeedMoney)
{
	u_goldNumber = getUserGold(u_userId);
	if (u_goldNumber < u_NeedMoney)
	{
		return false;
	}
	return true;
}

bool cProcUserCardLevel::updateCardInfo(const unsigned int& u_cardLevel,const unsigned int& u_cardExp,\
					const unsigned int& u_userId,const unsigned int& u_itemId)
{
	char buf[256] = {0};
	sprintf(buf,"update m_userCardInfo%d set cardLevel = %d,cardExp = %d \
				where itemId = %d;",u_userId%USER_CARD_TABLE_COUNT,u_cardLevel,u_cardExp,u_itemId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateCardInfo error:%s\n",strSql.c_str());
		return false;
	}
	return true;
}

//根据卡牌星级判断最大卡牌等级
int cProcUserCardLevel::getMaxCardLevelByCardStar(const int& u_cardStar)
{
	int u_maxCardLevel = 1;
	if (u_cardStar == 1)
	{
		u_maxCardLevel = ONE_STAR_MAX_CARD_LEVEL;
	}
	else if (u_cardStar == 2)
	{
		u_maxCardLevel = TWO_STAR_MAX_CARD_LEVEL;
	}
	else if (u_cardStar == 3)
	{
		u_maxCardLevel = THREE_STAR_MAX_CARD_LEVEL;
	}
	else if (u_cardStar == 4)
	{
		u_maxCardLevel = FOUR_STAR_MAX_CARD_LEVEL;
	}
	else if (u_cardStar == 5)
	{
		u_maxCardLevel = FIVE_STAR_MAX_CARD_LEVEL;
	}
	else if (u_cardStar == 6)
	{
		u_maxCardLevel = SIX_STAR_MAX_CARD_LEVEL;
	}
	return u_maxCardLevel;
}

bool cProcUserCardLevel::cardLevelUp(unsigned int& u_cardLevel,const int& u_maxCardLevel/*const int& u_cardStar*/,unsigned int& u_cardExp,\
				 const unsigned int& u_allEatedExp,const unsigned int& u_userId,\
				 const unsigned int& u_itemId)
{
	int u_cardLevelUpNeedExp = CARD_LEVEL_UP_NEED_EXP(u_cardLevel);
#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("111 u_cardLevel:%d,u_cardExp:%d,u_cardLevelUpNeedExp:%d,u_allEatedExp:%d\n",\
		u_cardLevel,u_cardExp,u_cardLevelUpNeedExp,u_allEatedExp);
#endif

	u_cardExp += u_allEatedExp;
	unsigned int u_finalCardExp = 0;
	if (u_cardExp >= u_cardLevelUpNeedExp)
	{
		while (u_cardExp >= u_cardLevelUpNeedExp)
		{
#ifdef DEBUG_MOD
			g_cMyGlog.debugLog("u_cardLevel:%d,u_cardExp:%d,u_cardLevelUpNeedExp:%d,u_allEatedExp:%d\n",\
				u_cardLevel,u_cardExp,u_cardLevelUpNeedExp,u_allEatedExp);
#endif
			u_cardLevel += 1;
			u_cardExp -= u_cardLevelUpNeedExp;
			u_cardLevelUpNeedExp = CARD_LEVEL_UP_NEED_EXP(u_cardLevel);
		}
	}
	u_finalCardExp = u_cardExp;
	//int u_maxCardLevel = getMaxCardLevelByCardStar(u_cardStar);
	//int u_maxCardLevel = getCardMaxLevelByStrCardId(u_strCardId);
	
	if (u_cardLevel >= u_maxCardLevel)
	{
		u_cardLevel = u_maxCardLevel;
		u_finalCardExp = 0;
	}
	if (!updateCardInfo(u_cardLevel,u_finalCardExp,u_userId,u_itemId))
	{
		return false;
	}
	return true;
	
}

bool cProcUserCardLevel::deleteEatedCards(const PBCardLevelUpSend &u_PBCardLevelUpSend,const unsigned int& u_userId)
{
	string strTmp = "(";
	char bufTmp[12] = {0};
	for (int i = 0; i < u_PBCardLevelUpSend.eatcarditemids_size();i ++)
	{
		memset(bufTmp,0,sizeof(bufTmp));
		if (i == u_PBCardLevelUpSend.eatcarditemids_size()-1)
		{
			sprintf(bufTmp,"%d)",u_PBCardLevelUpSend.eatcarditemids(i));
			strTmp += bufTmp;
		}
		else
		{
			sprintf(bufTmp,"%d,",u_PBCardLevelUpSend.eatcarditemids(i));
			strTmp += bufTmp;
		}

	}
	char buf[1024] = {0};
	sprintf(buf,"delete from m_userCardInfo%d where itemId in %s",u_userId%USER_CARD_TABLE_COUNT,strTmp.c_str());
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("deleteEatedCards error:%s\n",strSql.c_str());
		return false;
	}
	return true;
}

bool cProcUserCardLevel::getCardInfo(PBCard* u_PBCard,const unsigned int& u_userId,\
									 const unsigned int& u_itemId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_userCardInfo%d where itemId = %d;",u_userId%USER_CARD_TABLE_COUNT,u_itemId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getCardInfo error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	
	u_PBCard->set_itemid(q.getIntFileds("itemId"));
	u_PBCard->set_cardid(q.getStringFileds("cardId"));
	u_PBCard->set_cardlevel(q.getIntFileds("cardLevel"));
	u_PBCard->set_cardexp(q.getIntFileds("cardExp"));
	u_PBCard->set_skilllevel(q.getIntFileds("skillLevel"));
	u_PBCard->set_skillexp(q.getIntFileds("skillExp"));
	u_PBCard->set_battleposition(q.getIntFileds("battlePosition"));
	u_PBCard->set_vtlgrow(q.getIntFileds("VTLGrow"));
	u_PBCard->set_strgrow(q.getIntFileds("STRGrow"));
	u_PBCard->set_tghgrow(q.getIntFileds("TGHGrow"));
	u_PBCard->set_quigrow(q.getIntFileds("QUIGrow"));
	u_PBCard->set_mgcgrow(q.getIntFileds("MGCGrow"));
		
	q.finalize();
	return true;
}

//吞噬一张卡牌所需的金钱
int cProcUserCardLevel::eatOneCardNeedGold(const int& u_cardLevel,const int& u_cardStarLevel)
{
	int u_needMoney = 0;
	//u_needMoney = (u_cardLevel*(u_cardStarLevel*100))/1.5;
	u_needMoney = 400*u_cardLevel*u_cardStarLevel;
	return u_needMoney;
}

//卡牌能卖出的价钱
int cProcUserCardLevel::valueOfCard(const int& u_cardLevel,const int& u_cardStarLevel)
{
	int u_soldMoney = 0;
	//u_soldMoney = u_cardLevel*(u_cardStarLevel*3)*20;
	u_soldMoney = u_cardLevel*(u_cardStarLevel*5)*40;
	return u_soldMoney;
}

//卡牌的经验值
int cProcUserCardLevel::expOfCard(const int& u_LevelUpcardLevel,const int& u_LevelUpcardStarLevel,\
								  const int& u_cardLevel,const int& u_cardStarLevel)
{
	int u_cardExp = 0;
	//（x/3）400*（卡片星级/1.5)
	//u_cardExp = (u_LevelUpcardLevel*400*u_LevelUpcardStarLevel)/4.5 + ((u_cardLevel - 1)*200*u_cardStarLevel)/4.5;
	u_cardExp = 400*u_cardStarLevel + (u_cardLevel-1)*310;
	return u_cardExp;
}

//吃掉一张卡牌能获得的经验
int cProcUserCardLevel::expOfEatCard(const int& u_cardLevel,const int& u_cardStarLevel)
{
	//【（y-1）/3】200*（卡片星级/1.5)
	int u_eatCardExp = 0;
	u_eatCardExp = ((u_cardLevel - 1)*200*u_cardStarLevel)/4.5;
	return u_eatCardExp;
}

//获取被吃掉卡牌的所有经验
int cProcUserCardLevel::expOfAllEatedCard(const int& u_LevelUpcardLevel,const int& u_LevelUpcardStarLevel,const PBCardLevelUpSend &u_PBCardLevelUpSend,const unsigned int& u_userId,\
					  int& u_NeedMoney)
{
	unsigned int u_cardLevel = 0;
	unsigned int u_cardExp = 0;
	int u_cardStarLevel = 0;
	//int u_cardId = 0;
	string u_strCardId = "";
	int u_allExp = 0;
	for (int i = 0; i < u_PBCardLevelUpSend.eatcarditemids_size(); i++)
	{
		if (isCardExist(u_userId,u_PBCardLevelUpSend.eatcarditemids(i),u_cardLevel,u_cardExp,u_cardStarLevel,u_strCardId))
		{
			//u_allExp += u_cardExp;
			int u_cardId = atoi(u_strCardId.c_str());
			if (u_cardId == 2004)
			{
				u_allExp += EXP_OF_2004;
			}
			else if (u_cardId == 2005)
			{
				u_allExp += EXP_OF_2005;
			}
			else if (u_cardId == 2006)
			{
				u_allExp += EXP_OF_2006;
			}
			else
			{
				u_allExp += expOfCard(u_LevelUpcardLevel,u_LevelUpcardStarLevel,u_cardLevel,u_cardStarLevel);
			}
			u_NeedMoney += eatOneCardNeedGold(u_cardLevel,u_cardStarLevel);
		}
#ifdef DEBUG_MOD
		g_cMyGlog.debugLog("u_allExp:%d\n",u_allExp);
#endif
	}
	return u_allExp;
}

//根据等级和星级判断卡牌是否能升级
bool cProcUserCardLevel::isCardCanLevelUpByCardLevelAndCardStar(const int& u_cardLevel,const int& u_cardStar)
{
	if (u_cardStar == 1)
	{
		if (u_cardLevel < ONE_STAR_MAX_CARD_LEVEL)
		{
			return true;
		}
	}
	else if (u_cardStar == 2)
	{
		if (u_cardLevel < TWO_STAR_MAX_CARD_LEVEL)
		{
			return true;
		}
	}
	else if (u_cardStar == 3)
	{
		if (u_cardLevel < THREE_STAR_MAX_CARD_LEVEL)
		{
			return true;
		}
	}
	else if (u_cardStar == 4)
	{
		if (u_cardLevel < FOUR_STAR_MAX_CARD_LEVEL)
		{
			return true;
		}
	}
	else if (u_cardStar == 5)
	{
		if (u_cardLevel < FIVE_STAR_MAX_CARD_LEVEL)
		{
			return true;
		}
	}
	else if (u_cardStar == 6)
	{
		if (u_cardLevel < SIX_STAR_MAX_CARD_LEVEL)
		{
			return true;
		}
	}
	return false;
}

int cProcUserCardLevel::setPBCardLevelUpReceive(PBCardLevelUpReceive& u_PBCardLevelUpReceive,const string& u_strToken,\
							 const PBCardLevelUpSend &u_PBCardLevelUpSend)
{
	unsigned int u_userId = 0;
	unsigned int u_cardLevel = 0;
	unsigned int u_cardExp = 0;
	unsigned int u_allCardExp = 0;
	unsigned int u_goldNumber = 0;
	int u_NeedMoney = 0;
	int u_cardStarLevel = 0;
	int u_result = 0;
	//cerr<<"setPBCardLevelUpReceive"<<endl;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBCardLevelUpReceive getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	unsigned int u_itemId = u_PBCardLevelUpSend.upcarditemid();
	//int u_cardId = 0;
	string u_cardId = "";
	if (!isCardExist(u_userId,u_itemId,u_cardLevel,u_cardExp,u_cardStarLevel,u_cardId))
	{
		//u_PBCardLevelUpReceive.set_upresult(CARD_LEVEL_UP_NOT_EXIST);
		//cerr<<"setPBCardLevelUpReceive isCardExist false"<<endl;
		return K_CARD_NOT_EXIST;

	}
	if (u_PBCardLevelUpSend.eatcarditemids_size() <= 0)
	{
		g_cMyGlog.errorlogRecord("u_PBCardLevelUpSend.eatcarditemids_size():%d\n",u_PBCardLevelUpSend.eatcarditemids_size());
		u_result = -1;
		return u_result;
	}
	int u_maxCardLevel = getCardMaxLevelByStrCardId(u_cardId);
	if (u_cardLevel >= u_maxCardLevel)
	{
		return K_CARD_LEVEL_HAS_TO_MAX;
	}
	/*if (!isCardCanLevelUpByCardLevelAndCardStar(u_cardLevel,u_cardStarLevel))
	{
		return K_CARD_LEVEL_HAS_TO_MAX;
	}*/
	u_allCardExp = expOfAllEatedCard(u_cardLevel,u_cardStarLevel,u_PBCardLevelUpSend,u_userId,u_NeedMoney);
	if (!goldEnough(u_PBCardLevelUpSend,u_userId,u_goldNumber,u_NeedMoney))
	{
		//u_PBCardLevelUpReceive.set_upresult(CARD_LEVEL_UP_LESS_GOLD);
		//cerr<<"setPBCardLevelUpReceive goldEnough false"<<endl;
		return K_GOLD_NOT_ENOUGH;
	}

	u_PBCardLevelUpReceive.set_upresult(CARD_LEVEL_UP_SUCCESS);
	//getEatedCardExp(u_PBCardLevelUpSend,u_allCardExp,u_userId);
	
	cardLevelUp(u_cardLevel,u_maxCardLevel/*u_cardStarLevel*/,u_cardExp,u_allCardExp,u_userId,u_itemId);
	
	deleteEatedCards(u_PBCardLevelUpSend,u_userId);

	PBAllCard* u_PBAllCard;
	u_PBAllCard = u_PBCardLevelUpReceive.mutable_cards();
	g_cProcUserLogin.getAllCardsInfo(u_PBAllCard,u_userId);

	u_goldNumber -= u_NeedMoney;
	updateUserGold(u_userId,u_goldNumber);
	u_PBCardLevelUpReceive.set_goldnumber(u_goldNumber);

	PBCard* u_PBCard;
	u_PBCard = u_PBCardLevelUpReceive.mutable_targetcard();
	getCardInfo(u_PBCard,u_userId,u_itemId);

	int u_userFightCapacity = 0;
	int u_userCapitorCardFightCapacity = 0;
	u_userFightCapacity = getUserFightCapacity(u_userId,u_userCapitorCardFightCapacity);
	updateUserFightCapacity(u_userId,u_userFightCapacity,u_userCapitorCardFightCapacity);

	u_PBCardLevelUpReceive.set_fightcapacity(u_userFightCapacity);

	//记录战斗力突破通知
	procFightCapacityBreakThrough(u_userId,u_userFightCapacity);
	return u_result;
}