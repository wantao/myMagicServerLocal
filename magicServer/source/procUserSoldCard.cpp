#include "../include/procUserSoldCard.h"
#include "../include/procUserCardLevelUp.h"
#include "../include/procUserLogin.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"

cProcUserSoldCard g_cProcUserSoldCard;

cProcUserSoldCard::cProcUserSoldCard()
{

}
cProcUserSoldCard::~cProcUserSoldCard()
{

}

//获取贩卖卡牌列表
string cProcUserSoldCard::getSoldCardItemList(const PBSoldCardSend& u_PBSoldCardSend)
{
	string u_soldCardItemList = "";

	for (int i = 0; i < u_PBSoldCardSend.carditemid_size(); i++)
	{
		char buf[32] = {0};
		if (i == u_PBSoldCardSend.carditemid_size() - 1)
		{
			sprintf(buf,"%d",u_PBSoldCardSend.carditemid(i));
		}
		else
		{
			sprintf(buf,"%d,",u_PBSoldCardSend.carditemid(i));
		}
		u_soldCardItemList += buf;
	}
	return u_soldCardItemList;
}

//获取贩卖卡牌所得的金钱
int cProcUserSoldCard::getSoldCardGold(const string& u_soldCardItemList,const unsigned int& u_userId)
{
	int u_goldNumber = 0;
	int u_cardLevel = 0;
	int u_cardStarLevel = 0;
	int u_cardId = 0;
	char buf[1024] = {0};
	sprintf(buf,"select cardId,cardLevel,cardStarLevel from m_userCardInfo%d where itemId in (%s);",\
		u_userId%USER_CARD_TABLE_COUNT,u_soldCardItemList.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getSoldCardGold error,strSql:%s\n",strSql.c_str());
	}
	else
	{
		while (!q.eof())
		{
			u_cardLevel = q.getIntFileds("cardLevel");
			u_cardStarLevel = q.getIntFileds("cardStarLevel");
			u_cardId = atoi((q.getStringFileds("cardId")).c_str());
			if (u_cardId == 2001)
			{
				u_goldNumber += MONEY_OF_2001;
			}
			else if (u_cardId == 2002)
			{
				u_goldNumber += MONEY_OF_2002;
			}
			else if (u_cardId == 2003)
			{
				u_goldNumber += MONEY_OF_2003;
			}
			else
			{
				u_goldNumber += g_cProcUserCardLevel.valueOfCard(u_cardLevel,u_cardStarLevel);
			}
			q.nextRow();
		}
	}
	q.finalize();
	return u_goldNumber;
}

//删除卖掉的卡牌
void cProcUserSoldCard::deleteSoldCard(const string& u_soldCardItemList,const unsigned int& u_userId)
{
	char buf[1024] = {0};
	sprintf(buf,"delete from m_userCardInfo%d where itemId in (%s)",\
		u_userId%USER_CARD_TABLE_COUNT,u_soldCardItemList.c_str());
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("deleteSoldCard error,strSql:%s\n",strSql.c_str());
	}
}

//贩卖卡牌过程处理
int cProcUserSoldCard::procSoldCard(const PBSoldCardSend& u_PBSoldCardSend,const unsigned int& u_userId,unsigned int& u_userGold)
{
	string u_strSoldCardItemList = "";
	int u_soldGold = 0;
	int u_result = 0;
	u_strSoldCardItemList = getSoldCardItemList(u_PBSoldCardSend);
	//g_cMyGlog.errorlogRecord("procSoldCard u_strSoldCardItemList:%s\n",u_strSoldCardItemList.c_str());
	if (u_strSoldCardItemList.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procSoldCard nothing to be sold\n");
		return K_CARD_NOT_EXIST;
	}
	u_soldGold = getSoldCardGold(u_strSoldCardItemList,u_userId);

	u_userGold = g_cProcUserCardLevel.getUserGold(u_userId);

	u_userGold += u_soldGold;
	g_cProcUserCardLevel.updateUserGold(u_userId,u_userGold);

	deleteSoldCard(u_strSoldCardItemList,u_userId);
	return u_result;
}

//填充贩卖卡牌信息结构
int cProcUserSoldCard::setPBSoldCardReceive(PBSoldCardReceive &u_PBSoldCardReceive,const string& u_strToken,\
						  const PBSoldCardSend& u_PBSoldCardSend)
{
	unsigned int u_userId = 0;
	unsigned int u_goldNumber = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBSoldCardReceive getUserIdbyToken error:Token,\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	
	u_result = procSoldCard(u_PBSoldCardSend,u_userId,u_goldNumber);
	if (u_result != 0)
	{
		return u_result;
	}
	else 
	{
		u_PBSoldCardReceive.set_soldresult(SOLD_CARD_SUCCESS);
		PBAllCard* u_PBAllCard;
		u_PBAllCard = u_PBSoldCardReceive.mutable_cards();
		g_cProcUserLogin.getAllCardsInfo(u_PBAllCard,u_userId);

		u_PBSoldCardReceive.set_goldnumber(u_goldNumber);
	}
	return u_result;
}
