#include "../include/procUserDiamondAward.h"
#include "../include/procUserLogin.h"
#include "../include/procUserAwardFriend.h"
#include "../include/magicServerDb.h"
#include "../include/common.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/procUserMissionAward.h"
#include "../include/procUserCardLevelUp.h"
#include "../include/procUserBattle.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"
#include "../include/procUserUseEnergy.h"

//钻石抽奖随机范围
#define DIAMOND_AWARD_RAND_MIN	1

#define DIAMOND_AWARD_RAND_MAX	10000

cProcUserDiamondAward g_cProcUserDiamondAward;

cProcUserDiamondAward::cProcUserDiamondAward()
{

}
cProcUserDiamondAward::~cProcUserDiamondAward()
{

}

//填充用户信息
bool cProcUserDiamondAward::setPBUser(PBUser* u_PBUser,const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_gameUserInfo where userID = %d;",u_userId);
	string strSql(buf);
	//MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
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

		unsigned int u_mLastEnergyTime = u_lastEnergyTime;
		unsigned int u_muEnergy = u_energy;
		g_cProUserMissionAward.updateUserEnergyAndTime(u_muEnergy,u_mLastEnergyTime,u_userId);
		u_PBUser->set_energy(u_energy);
		u_PBUser->set_nextenergytime(u_NextEnergyTime);

		//u_PBUser->set_buyenergytime(q.getIntFileds("buyEnergyTime"));
		newAddSetSomeUserInfo(u_PBUser,u_userId);
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


bool cProcUserDiamondAward::getUserDiamond(const unsigned int& u_userId,unsigned int& u_diamondNumber)
{
	char buf[128] = {0};
	sprintf(buf,"select diamondNumber from m_gameUserInfo where \
				userID = %d;",u_userId);
	string strSql(buf);
	//MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord(" getUserDiamond m_userFriendInfo not find userId,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_diamondNumber = q.getIntFileds("diamondNumber");
	q.finalize();
	return true;
}

bool cProcUserDiamondAward::updataUserDiamondNumber(const unsigned int& u_userId,unsigned int& u_diamondNumber)
{
	char buf[128] = {0};
	sprintf(buf,"update m_gameUserInfo set diamondNumber = %d where userID = %d;",u_diamondNumber,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updataUserDiamondNumber error,strSql:%s\n",strSql.c_str());
	}
	return true;

}

//插入卡牌信息
bool cProcUserDiamondAward::insertCardInfo(const unsigned int& u_userId,const unsigned int& u_cardId,unsigned int& u_itemId)
{
	char u_buf[5] = {0};
	/*if (u_cardId > 1000)
	{
		sprintf(u_buf,"%d",u_cardId);
	}
	else
	{*/
		sprintf(u_buf,"%03d",u_cardId);
	//}
	string u_strCardId(u_buf);
	char buf[128] = {0};
	sprintf(buf,"select * from cardInfo where cardId = '%s';",u_strCardId.c_str());
	string strSql(buf);

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("not find CardId,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}

	string cardId = q.getStringFileds("cardId");
	
	int VTLGrow = getGrow(q.getIntFileds("VTLGrow"));
	int STRGrow = getGrow(q.getIntFileds("STRGrow"));
	int TGHGrow = getGrow(q.getIntFileds("TGHGrow"));
	int QUIGrow = getGrow(q.getIntFileds("QUIGrow"));
	int MGCGrow = getGrow(q.getIntFileds("MGCGrow"));
	int battlePosition = NOT_BATTLE_POSITION;
#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("u_cardId:%s,v:%d,s:%d,t:%d,q:%d,m:%d\n",cardId.c_str(),VTLGrow,STRGrow,TGHGrow,QUIGrow,MGCGrow);
#endif
	
	char bufTmp[1024] = {0};
	sprintf(bufTmp,"insert into m_userCardInfo%d(itemId,userID,cardId,cardLevel,\
				cardExp,battlePosition,VTLGrow,STRGrow,TGHGrow,\
				QUIGrow,MGCGrow,skillExp,skillLevel,cardStarLevel) \
				values(NULL,%d,'%s',%d,%d,%d,%d,\
				%d,%d,%d,%d,%d,%d,%d)",u_userId%USER_CARD_TABLE_COUNT,u_userId,cardId.c_str(),1, \
				0,battlePosition,VTLGrow,STRGrow,TGHGrow, \
				QUIGrow,MGCGrow,0,1,q.getIntFileds("CardStarLevel"));
	string strTmpSql(bufTmp);
	if(g_cMagicServerDb.execSQL(strTmpSql,&u_itemId)!=0)
	{
		g_cMyGlog.errorlogRecord("insertInitCardInfo insert card failure,strSql:%s\n",strTmpSql.c_str());
		q.finalize();
		return false;
	}
	q.finalize();
	return true;
}

//获取钻石抽奖所需的钻石数量
unsigned int cProcUserDiamondAward::getNeedDiamondNum(const int& u_Type)
{
	unsigned int u_needDiamondNum = 0;
	if(u_Type == 2)
	{
		u_needDiamondNum = AWARD_DIAMOND_NUMBER;
	}
	else if (u_Type == 3)
	{
		u_needDiamondNum = AWARD_DIAMOND_NUMBER*9.6;
	}
	return u_needDiamondNum;
}
//随机卡牌星级
int cProcUserDiamondAward::randCardStarLevel()
{
	double random(double,double);
	int randData = 0;
	int u_cardStarLevel = 3;
	struct timeval tv;
	gettimeofday(&tv,NULL);

	srand(tv.tv_usec+tv.tv_sec);
	randData = int(random(DIAMOND_AWARD_RAND_MIN,DIAMOND_AWARD_RAND_MAX));
	if (randData > DIAMOND_AWARD_RAND_MIN && randData <= 9495)
	{
		u_cardStarLevel = 3;
	}
	else if (randData > 9495 && randData <= 9945)
	{
		u_cardStarLevel = 4;
	}
	else if (randData > 9945 && randData <=  9995)
	{
		u_cardStarLevel = 5;
	}
	else if (randData > 9995 && randData <= DIAMOND_AWARD_RAND_MAX)
	{
		u_cardStarLevel = 6;
	}
	return u_cardStarLevel;
}

//根据星级得到对应星级的卡牌数据库
string cProcUserDiamondAward::getCardStarLevelTable(const int& u_cardStarLevel)
{
	string u_strCardLevelTable = "twostarcardinfo";
	if (u_cardStarLevel == 3)
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
bool cProcUserDiamondAward::getCardDigitId(const string& u_strCardLevelTable,int& u_digitId)
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
		g_cMyGlog.errorlogRecord("getCardDigitId failure,strSql:%s\n",strSql.c_str());
		q.finalize();
		//取一个两星的卡牌ID
		u_digitId = 46;
		return false;
	}
	u_digitId = q.getIntFileds("cardDigitId");
	q.finalize();
	return true;
}


//插入和填充钻石抽奖卡牌
void cProcUserDiamondAward::insertAndSetAwardCard(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int& u_Type)
{
	int u_Count = 0;
	//钻石单抽
	if (u_Type == 2)
	{
		u_Count = 1;
	}
	//钻石十连抽
	else if (u_Type == 3)
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
		////test for 测试数据begin
		/*if (u_userId == 452)
		{
			int u_mod = i%7;
			if (u_mod == 0)
			{
				u_digitId = 163;
			}
			else if(u_mod == 1)
			{
				u_digitId = 707;
			}
			else if (u_mod == 2)
			{
				u_digitId = 703;
			}
			else if (u_mod == 3)
			{
				u_digitId = 704;
			}
			else if (u_mod == 4)
			{
				u_digitId = 705;
			}
			else if (u_mod == 5)
			{
				u_digitId = 706;
			}
			else if (u_mod == 6)
			{
				u_digitId = 41;
			}
		}*/
		////test for 测试数据end
		insertCardInfo(u_userId,u_digitId,u_itemId);
		//5星或6星卡牌需通知
		if (u_cardStarLevel == 5 || u_cardStarLevel == 6)
		{
			char buf[6] = {0};
			sprintf(buf,"%03d",u_digitId);
			string n_cardId(buf);
			//插入高星级卡牌通知
			insertHighStarCardNotice(u_userId,n_cardId,u_cardStarLevel);
		}

		PBCard* u_PBCard;
		u_PBCard = u_PBAwardReceive.add_awardcard();
	    
		g_cProcUserAwardFriend.getCardInfo(u_PBCard,u_userId,u_itemId);
	}
}


//代理填充PBAwardReceive
int cProcUserDiamondAward::proxySetPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const unsigned int& u_userId,const int u_Type)
{
	unsigned int u_diamondNumber = 0;
	unsigned int u_needDiamondNum = 0;
	int u_result = 0;
	if (!getUserDiamond(u_userId,u_diamondNumber))
	{
		g_cMyGlog.errorlogRecord("proxySetPBAwardReceive getUserDiamond error\n");
		u_result = -1;
		return u_result;
	}
	u_needDiamondNum = getNeedDiamondNum(u_Type);
	if (u_diamondNumber < u_needDiamondNum)
	{
		//u_PBAwardReceive.set_awardresult(AWARD_DIAMOND_VALUE_LESS);
		return K_MAGIC_STONE_NOT_ENOUGH;
	}
	else
	{
		u_PBAwardReceive.set_awardresult(AWARD_DIAMOND_SUCCESS);
		u_diamondNumber -= u_needDiamondNum;
		updataUserDiamondNumber(u_userId,u_diamondNumber);
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
int cProcUserDiamondAward::setPBAwardReceive(PBAwardReceive& u_PBAwardReceive,const string& u_strToken,const int& u_Type)
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

	if (u_Type == 2 || u_Type == 3)
	{
		u_result = proxySetPBAwardReceive(u_PBAwardReceive,u_userId,u_Type);
#ifdef TEST
		PBUser u_PBUser = u_PBAwardReceive.user();
		printUserInfo(u_PBUser);
		PBAllCard u_PBAllCard = u_PBAwardReceive.cards();
		printAllCard(u_PBAllCard);
		g_cMyGlog.errorlogRecord("!!!!!!!!!!!!!awardCardSize:%d\n",u_PBAwardReceive.awardcard_size());
#endif

		return u_result;
	}
	else
	{
		u_result = -1;
	}

	return u_result;
}

#ifdef TEST
///////////////////test
void cProcUserDiamondAward::printUserInfo(const PBUser& u_PBUser)
{
	g_cMyGlog.errorlogRecord("!!!!!!!!!!!!!!!!userID:%d,userName:%s,userLevel:%d,diamondNumber:%d,goldNumber:%d\n",\
		u_PBUser.userid(),(u_PBUser.username()).c_str(),u_PBUser.userlevel(),u_PBUser.diamondnumber(),\
		u_PBUser.goldnumber());
	
}

void cProcUserDiamondAward::printAllCard(const PBAllCard& u_PBAllCard)
{
	g_cMyGlog.errorlogRecord("!!!!!!!!!!!!!!!AllCardSize:%d\n",u_PBAllCard.allcard_size());
}
#endif

//////////////////////test