#include "../include/procUserAwardFriend.h"
#include "../include/procUserLogin.h"

#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/procUserMissionAward.h"
#include "../include/procUserCardLevelUp.h"
#include "../include/procUserBattle.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"
#include "../include/procUserUseEnergy.h"

cProcUserAwardFriend g_cProcUserAwardFriend;

//随机范围
#define AWARD_FRIEND_RAND_MIN	1

#define AWARD_FRIEND_RAND_MAX	100000

cProcUserAwardFriend::cProcUserAwardFriend()
{

}
cProcUserAwardFriend::~cProcUserAwardFriend()
{

}

//填充用户信息
bool cProcUserAwardFriend::setPBUser(PBUser* u_PBUser,const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_gameUserInfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("m_gameUserInfo not find userId:%d\n",u_userId);
		q.finalize();
		return false;
	}
	else
	{
		u_PBUser->set_userid(q.getIntFileds("userID"));
		u_PBUser->set_username(q.getStringFileds("userName"));
		u_PBUser->set_userlevel(q.getIntFileds("userLevel"));
		u_PBUser->set_userexp(q.getIntFileds("userExp"));

		int u_energy = q.getIntFileds("energy");

		int u_lastEnergyTime = q.getIntFileds("lastEnergyTime");
		int u_userLevel = q.getIntFileds("userLevel");
		
		int u_nowTimeStamp = 0;
		u_nowTimeStamp = GetTimeStamp();
		int u_NextEnergyTime = g_cProcUserUseEnergy.getNextEnergyTime(u_nowTimeStamp,u_lastEnergyTime,u_energy,u_userLevel);

		unsigned int u_Energy = u_energy;
		g_cProUserMissionAward.updateUserEnergyAndTime(u_Energy,u_lastEnergyTime,u_userId);
		u_PBUser->set_energy(u_energy);
		u_PBUser->set_nextenergytime(u_NextEnergyTime);

		u_PBUser->set_buyenergytime(q.getIntFileds("buyEnergyTime"));
		u_PBUser->set_diamondnumber(q.getIntFileds("diamondNumber"));
		u_PBUser->set_friendvalue(q.getIntFileds("friendValue"));
		u_PBUser->set_goldnumber(q.getIntFileds("goldNumber"));
		u_PBUser->set_leadervalue(q.getIntFileds("leaderValue"));
		if (g_cMagicServerDb.hasMissionFinished(u_userId))
		{
			u_PBUser->set_hasmissionfinished(true);
		}
		else
		{
			u_PBUser->set_hasmissionfinished(false);
		}
		u_PBUser->set_bagmaxcount(q.getIntFileds("maxOwnCardCount"));
		u_PBUser->set_energyagentiacount(q.getIntFileds("energyCount"));
		u_PBUser->set_captainindex(q.getIntFileds("playerProfession"));
		q.finalize();
	}
	return true;
}


bool cProcUserAwardFriend::getUserFriendValue(const unsigned int& u_userId,unsigned int& u_friendValue)
{
	char buf[128] = {0};
	sprintf(buf,"select friendValue from m_gameUserInfo where \
				userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserFriendValue m_userFriendInfo not find userId:%d\n",u_userId);
		q.finalize();
		return false;
	}
	u_friendValue = q.getIntFileds("friendValue");
	q.finalize();
	return true;
}

bool cProcUserAwardFriend::updataUserFriendValue(const unsigned int& u_userId,unsigned int u_friendValue)
{
	char buf[128] = {0};
	sprintf(buf,"update m_gameUserInfo set friendValue = %d where userID = %d;",u_friendValue,u_userId);
	string strSql(buf);
	
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updataUserFriendValue error,strSql:%s\n",strSql.c_str());
	}
	return true;
	
}

//插入卡牌信息
bool cProcUserAwardFriend::insertCardInfo(const unsigned int& u_userId,const unsigned int& u_CardId,unsigned int& u_itemId)
{
	char u_buf[5] = {0};
	if (u_CardId < 1000)
	{
		sprintf(u_buf,"%03d",u_CardId);
	}
	else
	{
		sprintf(u_buf,"%d",u_CardId);
	}
	string u_strCardId(u_buf);
	char buf[128] = {0};
	sprintf(buf,"select * from cardInfo where cardId = '%s';",u_strCardId.c_str());
	string strSql(buf);

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("cProcUserAwardFriend::insertCardInfo not find CardId:%s\n",u_strCardId.c_str());
		g_cMyGlog.errorlogRecord("sql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}

	string cardId = q.getStringFileds("cardId");
	/*
	int cardLevel = q.getIntFileds("cardLevel");
	int cardExp = q.getIntFileds("cardExp");
	int skillExp = q.getIntFileds("skillExp");
	int skillLevel = q.getIntFileds("skillLevel");
	*/
	int VTLGrow = getGrow(q.getIntFileds("VTLGrow"));
	int STRGrow = getGrow(q.getIntFileds("STRGrow"));
	int TGHGrow = getGrow(q.getIntFileds("TGHGrow"));
	int QUIGrow = getGrow(q.getIntFileds("QUIGrow"));
	int MGCGrow = getGrow(q.getIntFileds("MGCGrow"));
	int battlePosition = NOT_BATTLE_POSITION;
	
	char bufTmp[512] = {0};
	sprintf(bufTmp,"insert into m_userCardInfo%d(itemId,userID,cardId,cardLevel,\
				cardExp,battlePosition,VTLGrow,STRGrow,TGHGrow,\
				QUIGrow,MGCGrow,skillExp,skillLevel,cardStarLevel) \
				values(NULL,%d,'%s',%d,%d,%d,%d,\
				%d,%d,%d,%d,%d,%d,%d)",u_userId%USER_CARD_TABLE_COUNT,u_userId,cardId.c_str(),1, \
				0,battlePosition,VTLGrow,STRGrow,TGHGrow, \
				QUIGrow,MGCGrow,0,1,q.getIntFileds("CardStarLevel"));
	string strSqlTmp(bufTmp);
	if(g_cMagicServerDb.execSQL(strSqlTmp,&u_itemId)!=0)
	{
		g_cMyGlog.errorlogRecord("insertInitCardInfo insert card failure: cardId:%d\n",u_CardId);
		g_cMyGlog.errorlogRecord("strSql:%s\n",strSqlTmp.c_str());
		return false;
	}

	q.finalize();
	return true;
}

void cProcUserAwardFriend::getCardInfo(PBCard* u_PBCard,const unsigned int& u_userId,const unsigned int& u_itemId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_userCardInfo%d where itemId = %d;",u_userId%USER_CARD_TABLE_COUNT,u_itemId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getCardInfo error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return;
	}
	else
	{
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
	}
	q.finalize();
	return;
}

//获取友情抽奖所需的友情值
unsigned int cProcUserAwardFriend::getNeedFriendValue(const int& u_Type)
{
	unsigned int u_needFriendValue = 0;
	//友情单抽
	if (u_Type == 0)
	{
		u_needFriendValue = AWARD_FRIEND_VALUE;
	}
	//友情十连抽
	else if (u_Type == 1)
	{
		u_needFriendValue = AWARD_FRIEND_VALUE*10;
	}
	return u_needFriendValue;
}


//随机卡牌星级
int cProcUserAwardFriend::randCardStarLevel()
{
	double random(double,double);
	int randData = 0;
	int u_cardStarLevel = 1;
	struct timeval tv;
	gettimeofday(&tv,NULL);

	srand(tv.tv_usec+tv.tv_sec);
	randData = int(random(AWARD_FRIEND_RAND_MIN,AWARD_FRIEND_RAND_MAX));
	if (randData > AWARD_FRIEND_RAND_MIN && randData <= 80000)
	{
		u_cardStarLevel = 1;
	}
	else if (randData > 70000 && randData <= 99990)
	{
		u_cardStarLevel = 2;
	}
	else if (randData > 99990 && randData <=  AWARD_FRIEND_RAND_MAX)
	{
		u_cardStarLevel = 3;
	}
	return u_cardStarLevel;
}

//根据星级得到对应星级的卡牌数据库
string cProcUserAwardFriend::getCardStarLevelTable(const int& u_cardStarLevel)
{
	string u_strCardLevelTable = "onestarcardinfo";
	if (u_cardStarLevel == 2)
	{
		u_strCardLevelTable.clear();
		u_strCardLevelTable = "twostarcardinfo";
	}
	else if (u_cardStarLevel == 3)
	{
		u_strCardLevelTable.clear();
		u_strCardLevelTable = "threestarcardinfo";
	}
	else if (u_cardStarLevel == 4)
	{
		u_strCardLevelTable.clear();
		u_strCardLevelTable = "fourstarcardinfo";
	}
	else if (u_cardStarLevel == 5)
	{
		u_strCardLevelTable.clear();
		u_strCardLevelTable = "fivestarcardinfo";
	}
	else if (u_cardStarLevel == 6)
	{
		u_strCardLevelTable.clear();
		u_strCardLevelTable = "sixstarcardinfo";
	}
	return u_strCardLevelTable;
}

//从星级卡牌数据库中随机一个carddigitId
bool cProcUserAwardFriend::getCardDigitId(const string& u_strCardLevelTable,int& u_digitId)
{
	char buf[1024] = {0};
	sprintf(buf,"SELECT cardDigitId FROM `%s` AS t1 JOIN \
				(SELECT ROUND(RAND() * ((SELECT MAX(Id) FROM `%s`) \
				-(SELECT MIN(Id) FROM  `%s`))+(SELECT MIN(Id) FROM \
				`%s`)) AS id) AS t2 WHERE t1.Id >= t2.id ORDER BY t1.Id LIMIT 1;",\
				u_strCardLevelTable.c_str(),u_strCardLevelTable.c_str(),
				u_strCardLevelTable.c_str(),u_strCardLevelTable.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getCardDigitId failure:,strSql:%s\n",strSql.c_str());
		q.finalize();
		//取一个一星的卡牌ID
		u_digitId = 14;
		return false;
	}
	u_digitId = q.getIntFileds("cardDigitId");
	return true;
}

//插入和填充友情抽奖卡牌
void cProcUserAwardFriend::insertAndSetAwardCard(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int& u_Type)
{
	int u_Count = 0;
	//友情单抽
	if (u_Type == 0)
	{
		u_Count = 1;
	}
	//友情十连抽
	else if (u_Type == 1)
	{
		u_Count = 10;
	}
	for (int i = 0; i < u_Count; i++)
	{
		int u_cardStarLevel = randCardStarLevel();
		int u_digitId = 0;
		string u_strCardLevelTable = getCardStarLevelTable(u_cardStarLevel);
		getCardDigitId(u_strCardLevelTable,u_digitId);
		unsigned int u_itemId = 0;
		insertCardInfo(u_userId,u_digitId,u_itemId);
		
		PBCard* u_PBCard;
		u_PBCard = u_PBAwardReceive.add_awardcard();
		getCardInfo(u_PBCard,u_userId,u_itemId);
	}
}

//代理填充PBAwardReceive
int cProcUserAwardFriend::proxySetPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int& u_Type)
{
	unsigned int u_friendValue = 0;
	unsigned int u_needFriendValue = 0;
	int u_result = 0;
	if (!getUserFriendValue(u_userId,u_friendValue))
	{
		g_cMyGlog.errorlogRecord("proxySetPBAwardReceive getUserFriendValue error\n");
		u_result = -1;
		return u_result;
	}
	u_needFriendValue = getNeedFriendValue(u_Type);
	if (u_friendValue < u_needFriendValue)
	{
		//u_PBAwardReceive.set_awardresult(AWARD_FRIEND_VALUE_LESS);
		return K_FRIEND_VALUE_NOT_ENOUGH;
	}
	else
	{
		u_friendValue -= u_needFriendValue;
		updataUserFriendValue(u_userId,u_friendValue);
	
		u_PBAwardReceive.set_awardresult(AWARD_FRIEND_SUCCESS);
		PBUser* u_PBUser;
		u_PBUser = u_PBAwardReceive.mutable_user();
		if (!setPBUser(u_PBUser,u_userId))
		{
			g_cMyGlog.errorlogRecord("proxySetPBAwardReceive setPBUser failure\n");
			u_result = -1;
			return u_result;
		}
		
		insertAndSetAwardCard(u_PBAwardReceive,u_userId,u_Type);
		PBAllCard* u_PBAllCard;
		u_PBAllCard = u_PBAwardReceive.mutable_cards();
		g_cProcUserLogin.getAllCardsInfo(u_PBAllCard,u_userId);

		PBGem* u_PBGem;
		u_PBGem = u_PBAwardReceive.add_awardgem();
	}
	return u_result;
}

//填充PBAwardReceive
int cProcUserAwardFriend::setPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const string& u_strToken,const int& u_Type)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("getUserAllFriend getUserIdbyToken error:Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	if (g_cProcUserBattle.isCardCountToMax(u_userId))
	{
		//u_PBAwardReceive.set_awardresult(USER_CARD_TO_MAX);
		return K_BACKPACK_HAS_FULL;
	}
	//友情单抽或十连抽类型
	if (u_Type == 0 || u_Type == 1)
	{
		u_result = proxySetPBAwardReceive(u_PBAwardReceive,u_userId,u_Type);
		return u_result;
	}
	else
	{
		u_result = -1;
	}
	return u_result;
}