#include "../include/procUserGoldAward.h"
#include "../include/procUserCardLevelUp.h"
#include "../include/procUserAwardFriend.h"
#include "../include/procUserLogin.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/procUserBattle.h"
#include "../include/myGlog.h"


//随机范围
#define AWARD_GOLD_RAND_MIN	1

#define AWARD_GOLD_RAND_MAX	100000

#define SET_WEEKDAY	2

cProUserGoldAward g_cProUserGoldAward;

cProUserGoldAward::cProUserGoldAward()
{

}
cProUserGoldAward::~cProUserGoldAward()
{

}

//获取金币抽奖所需的金币值
unsigned int cProUserGoldAward::getNeedGold(const int& u_Type)
{
	unsigned int u_needGold = 0;
	//金币单抽
	if (u_Type == 4)
	{
		u_needGold = ONE_GOLD_AWARD_NEED_GOLD;
	}
	//金币十连抽
	else if (u_Type == 5)
	{
		u_needGold = ONE_GOLD_AWARD_NEED_GOLD*10;
	}
	return u_needGold;
}


//判断今天是否为指定的星期天数
//bool TodayIsSetDay(const string& u_strTime,const int& u_setWeekDay)
//{
//	int uWeekDay[] = {0,1,2,3,4,5,6};
//	int u_year = atoi((u_strTime.substr(0,4)).c_str());
//	int u_month = atoi((u_strTime.substr(5,2)).c_str());
//	int u_day = atoi((u_strTime.substr(8,2)));
//	if(u_month == 1|| u_month == 2)
//	{
//		u_month += 12;
//		u_month--;
//	}
//	int u_weedDay = uWeekDay[(u_day+1+2*u_month+3*(u_month+1)/5+u_year+u_year/4-u_year/100+u_year/400)%7];
//	if (u_weedDay == u_setWeekDay)
//	{
//		return true;
//	}
//	return false;
//}


//随机卡牌星级
int cProUserGoldAward::randCardStarLevel()
{
	double random(double,double);
	int randData = 0;
	int u_cardStarLevel = 1;
	struct timeval tv;
	gettimeofday(&tv,NULL);

	srand(tv.tv_usec+tv.tv_sec);
	randData = int(random(AWARD_GOLD_RAND_MIN,AWARD_GOLD_RAND_MAX));
	if (randData > AWARD_GOLD_RAND_MIN && randData <= 70000)
	{
		u_cardStarLevel = 1;
	}
	else if (randData > 70000 && randData <= 95000)
	{
		u_cardStarLevel = 2;
	}
	else if (randData > 95000 && randData <=  99900)
	{
		u_cardStarLevel = 3;
	}
	else if (randData > 99900 && randData <= AWARD_GOLD_RAND_MAX)
	{
		u_cardStarLevel = 4;
	}
	//else if (randData > 79990 && randData <= 80000)
	//{
	//	u_cardStarLevel = 5;
	//}
	//else if (randData > 80000 && randData <= AWARD_GOLD_RAND_MAX)
	//{
	//	/*long u_time = GetTimeStamp();
	//	string u_strTime = getStringTime(u_time);
	//	if (TodayIsSetDay(u_strTime,SET_WEEKDAY))
	//	{*/
	//		//随即碎片
	//		u_cardStarLevel = 7;
	//	//}
	//}
	return u_cardStarLevel;
}

//随机一个宝石ID
bool cProUserGoldAward::getGemId(const string& u_strGemTable,int& u_gemType,string& u_gemId)
{
	char buf[1024] = {0};
	sprintf(buf,"SELECT gemId,gemType FROM `%s` AS t1 JOIN \
				(SELECT ROUND(RAND() * ((SELECT MAX(Id) FROM `%s`) \
				-(SELECT MIN(Id) FROM  `%s`))+(SELECT MIN(Id) FROM \
				`%s`)) AS id) AS t2 WHERE t1.Id >= t2.id ORDER BY t1.Id LIMIT 1;",\
				u_strGemTable.c_str(),u_strGemTable.c_str(),
				u_strGemTable.c_str(),u_strGemTable.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getGemId failure:strSql:%s\n",strSql.c_str());
		q.finalize();
		//取一个一星的卡牌ID
		u_gemId = "1000";
		return false;
	}
	u_gemId = q.getStringFileds("gemId");
	if (u_gemId.size() != 4)
	{
		u_gemId = "1000";
	}
	u_gemType = q.getIntFileds("gemType");
	q.finalize();
	return true;
}
//插入和填充金币抽奖卡牌
void cProUserGoldAward::insertAndSetAwardCard(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int& u_Type)
{
	int u_Count = 0;
	//金币单抽
	if (u_Type == 4)
	{
		u_Count = 1;
	}
	//金币十连抽
	else if (u_Type == 5)
	{
		u_Count = 10;
	}
	for (int i = 0; i < u_Count; i++)
	{
		int u_cardStarLevel = randCardStarLevel();
		int u_digitId = 0;
		string u_strCardLevelTable = "";
		if (u_cardStarLevel != 7)
		{
			u_strCardLevelTable = g_cProcUserAwardFriend.getCardStarLevelTable(u_cardStarLevel);
			g_cProcUserAwardFriend.getCardDigitId(u_strCardLevelTable,u_digitId);
			unsigned int u_itemId = 0;
			g_cProcUserAwardFriend.insertCardInfo(u_userId,u_digitId,u_itemId);

			PBCard* u_PBCard;
			u_PBCard = u_PBAwardReceive.add_awardcard();
			g_cProcUserAwardFriend.getCardInfo(u_PBCard,u_userId,u_itemId);
		}
		else
		{
			//碎片表
			u_strCardLevelTable = "gemsTable";
			int u_gemType = 1;
			string u_gemId = "";
			getGemId(u_strCardLevelTable,u_gemType,u_gemId);
			addUserFragmentInfo(u_userId,u_gemType,u_gemId,1);
			//saveUserGems(u_userId,u_gemType,u_gemId,1);
			PBGem* u_PBGem;
			u_PBGem = u_PBAwardReceive.add_awardgem();
			u_PBGem->set_gemid(u_gemId);
			u_PBGem->set_gemnumber(1);
		}
	}
}


//代理填充PBAwardReceive
int cProUserGoldAward::proxySetPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int& u_Type)
{
	int u_result = 0;
	unsigned int u_userGold = 0;
	unsigned int u_awardNeedGold = 0;
	u_userGold = g_cProcUserCardLevel.getUserGold(u_userId);
	u_awardNeedGold = getNeedGold(u_Type);
	if (u_userGold < u_awardNeedGold)
	{
		return K_GOLD_NOT_ENOUGH;
	}
	else
	{
		u_userGold -= u_awardNeedGold;
		g_cProcUserCardLevel.updateUserGold(u_userId,u_userGold);

		u_PBAwardReceive.set_awardresult(AWARD_FRIEND_SUCCESS);
		PBUser* u_PBUser;
		u_PBUser = u_PBAwardReceive.mutable_user();
		if (!g_cProcUserAwardFriend.setPBUser(u_PBUser,u_userId))
		{
			u_result = -1;
			return u_result;
		}

		insertAndSetAwardCard(u_PBAwardReceive,u_userId,u_Type);
		PBAllCard* u_PBAllCard;
		u_PBAllCard = u_PBAwardReceive.mutable_cards();
		g_cProcUserLogin.getAllCardsInfo(u_PBAllCard,u_userId);

		PBAllGem* u_PBAllGem;
		u_PBAllGem = u_PBAwardReceive.mutable_gems();
		g_cProcUserLogin.getUserGemInfo(u_PBAllGem,u_userId);
	}
	return u_result;
}


//填充PBAwardReceive
int cProUserGoldAward::setPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const string& u_strToken,const int& u_Type)
{
	int u_result = 0;
	unsigned int u_userId = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("231setPBAwardReceive getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	if (g_cProcUserBattle.isCardCountToMax(u_userId))
	{
		return K_BACKPACK_HAS_FULL;
	}
	//金币单抽或十连抽类型
	if (u_Type == 4 || u_Type == 5)
	{
		u_result = proxySetPBAwardReceive(u_PBAwardReceive,u_userId,u_Type);
	}
	else 
	{
		u_result = -1;
	}
	return u_result;
}