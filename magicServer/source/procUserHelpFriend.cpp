#include "../include/procUserHelpFriend.h"
#include "../include/procUserGetAllFriend.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"
#include "../include/procUserSearchFriend.h"


cProcUserHelpFriend g_cProcUserHelpFriend;

#define HELP_FRIEND_NEED_COUNT	10

#define HELP_FRIEND_RAND_MIX	1

#define HELP_FRIEND_RAND_MAX	10000

cProcUserHelpFriend::cProcUserHelpFriend()
{

}
cProcUserHelpFriend::~cProcUserHelpFriend()
{

}

////从好友表里边取
bool cProcUserHelpFriend::getAllHelpFriendByUserId(PBHelpFriend& u_PBHelpFriend,const unsigned& u_userId)
{
	char buf[512] = {0};
	/*sprintf(buf,"select friendId from m_userFriendInfo%d where userId = %d and \
				friendId >= (select floor(RAND() * (select MAX(friendId) from `m_userFriendInfo%d` ) ) ) \
				limit %d;",u_userId%USER_FRIEND_TABLE_COUNT,u_userId,\
				u_userId%USER_FRIEND_TABLE_COUNT,FRIEND_HELP_FRIEND_COUNT);*/
	/*sprintf(buf,"select friendId,useFriendCount from m_userFriendInfo%d where userId = %d and \
				friendId >= (select floor(RAND() * (select MAX(friendId) from `m_userFriendInfo%d` ) ) )\
				and useFriendCount < %d limit %d;",\
		u_userId%USER_FRIEND_TABLE_COUNT,u_userId,u_userId%USER_FRIEND_TABLE_COUNT,USE_FRIEND_MAX_COUNT_ONE_DAY,FRIEND_HELP_FRIEND_COUNT);*/

	int u_timeStamp = GetTimeStamp();
	int u_timePoint = u_timeStamp - 7200;
	sprintf(buf,"select friendId,useFriendCount from m_userFriendInfo%d where userId = %d and \
				friendId >= (select floor(RAND() * (select MAX(friendId) from `m_userFriendInfo%d` ) ) )\
				and useFriendStamp <= %d limit %d;",\
				u_userId%USER_FRIEND_TABLE_COUNT,u_userId,u_userId%USER_FRIEND_TABLE_COUNT,u_timePoint,FRIEND_HELP_FRIEND_COUNT);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("getAllHelpFriendByUserId m_userFriendInfo not find userId,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	else
	{
		while (!q.eof())
		{
			PBFriend* u_PBFriend;
			u_PBFriend = u_PBHelpFriend.add_helpfriend();
			g_cProcUserGetAllFriend.getFriendInfo(u_PBFriend,q.getIntFileds("friendId"));
			u_PBFriend->set_isfriend(true);
			q.nextRow();
		}
	}
	q.finalize();
	return true;
}

//根据用户Id获取战斗卡牌的战斗档次
int cProcUserHelpFriend::getUserCapacityClassByUserId(const unsigned int& u_userId)
{
	int u_retClass = 1;
	char buf[128] = {0};
	sprintf(buf,"select class from m_user_fight_capacity where userId = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		q.finalize();
		g_cMyGlog.errorlogRecord("getUserClassByUserId failure,strSql:%s\n",strSql.c_str());
		return u_retClass;
	}
	u_retClass = q.getIntFileds("class");
	q.finalize();
	return u_retClass;
}

//根据随机数据及用户战斗卡牌的档次获取随机路人的档次
int cProcUserHelpFriend::getRandCapacityClass(const int& u_userClass,const int& u_randData)
{
	int u_randCapacityClass = u_userClass;
	if (u_randData > 0 && u_randData <= 100)
	{
		u_randCapacityClass += 3;
	}
	else if (u_randData > 100 && u_randData <= 400)
	{
		u_randCapacityClass += 2;
	}
	else if (u_randData > 400  && u_randData <= 900)
	{
		u_randCapacityClass += 1;
	}
	else if (u_randData > 900 && u_randData <= 9400)
	{
	}
	else if (u_randData > 9400 && u_randData <= 10000)
	{
		if (u_randCapacityClass > 1)
		{
			u_randCapacityClass -= 1;
		}
	}
	return u_randCapacityClass;
}

//根据路人的战斗卡牌的档次获取路人的ID
int cProcUserHelpFriend::getPlayerIdByRandClass(int u_randClass,const string& u_hasUsedIdList)
{
	int u_retPlayerId = 0;
	char buf[512] = {0};
	sprintf(buf,"select userId from m_user_fight_capacity where class = %d and userId >= (select floor(RAND() * (select MAX(userId) from m_user_fight_capacity ) ) ) \
				and userId not in(%s) limit 1;",u_randClass,u_hasUsedIdList.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("getPlayerIdByUserCapacityClass error,u_randClass:%d,strSql:%s\n",u_randClass,strSql.c_str());
		q.finalize();
		if (u_randClass > 1)
		{
			u_randClass -= 1;
			u_retPlayerId = getPlayerIdByRandClass(u_randClass,u_hasUsedIdList);
		}
	}
	else
	{
		u_retPlayerId = q.getIntFileds("userId");
		q.finalize();
	}
	return u_retPlayerId;
}

//根据用户战斗卡牌的档次获取路人Id
int cProcUserHelpFriend::getPlayerIdByUserCapacityClass(const int& u_userCapacityClass,const string& u_hasUsedIdList)
{
	int u_retPlayerId = 0;
	int u_randData = getRandData(HELP_FRIEND_RAND_MIX,HELP_FRIEND_RAND_MAX);
	int u_randClass = getRandCapacityClass(u_userCapacityClass,u_randData);
	//档次最高为10档
	if (u_randClass > 10)
	{
		u_randClass = 10;
	}
	u_retPlayerId = getPlayerIdByRandClass(u_randClass,u_hasUsedIdList);
	return u_retPlayerId;
}

//从用户表中获取助战基友
bool cProcUserHelpFriend::getHelpFriendFromUserTable(PBHelpFriend& u_PBHelpFriend,const unsigned int& u_userId)
{
	//char buf[1024] = {0};
	string u_strFriend = "";
	int u_friendSize = u_PBHelpFriend.helpfriend_size();
	int u_notFriendCount = HELP_FRIEND_NEED_COUNT - u_friendSize;
	u_strFriend = g_cProcUserSearchFriend.getUserFriendIdList(u_userId);
	char bufTmp[12] = {0};
	if (u_strFriend.size() > 0)
	{
		sprintf(bufTmp,",%d",u_userId);
	}
	else
	{
		sprintf(bufTmp,"%d",u_userId);
	}
	u_strFriend += bufTmp;
	int u_userCapacityClass = getUserCapacityClassByUserId(u_userId);
	while (u_notFriendCount > 0)
	{
		int u_playerId = 0;
		u_playerId = getPlayerIdByUserCapacityClass(u_userCapacityClass,u_strFriend);
		if (u_playerId > 0)
		{
			memset(bufTmp,0,sizeof(bufTmp));
			sprintf(bufTmp,",%d",u_playerId);
			u_strFriend += bufTmp;
			PBFriend* u_PBFriend;
			u_PBFriend = u_PBHelpFriend.add_helpfriend();
			g_cProcUserGetAllFriend.getFriendInfo(u_PBFriend,u_playerId);
		}
		u_notFriendCount -= 1;
	}
	return true;
}

int cProcUserHelpFriend::setPBHelpFriend(PBHelpFriend& u_PBHelpFriend,const string& u_strToken)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBHelpFriend getUserIdbyToken error, Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	//g_cMyGlog.errorlogRecord("setPBHelpFriend,userId:%d\n",u_userId);
	getAllHelpFriendByUserId(u_PBHelpFriend,u_userId);
	getHelpFriendFromUserTable(u_PBHelpFriend,u_userId);
	
	return u_result;
}