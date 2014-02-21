#include "../include/updateUserBaseInfo.h"
#include "../include/magicServerDb.h"
#include "../include/common.h"
#include "../include/myGlog.h"

cUpdateUserBaseInfo g_cUpdateUserBaseInfo;

cUpdateUserBaseInfo::cUpdateUserBaseInfo()
{

}
cUpdateUserBaseInfo::~cUpdateUserBaseInfo()
{

}

bool cUpdateUserBaseInfo::getActiveBattleMapByWeekDay(const int& u_weekDay,activeBattleMap& u_activeBattleMap)
{
	u_activeBattleMap.clear();
	if (u_weekDay == L_MONDAY)
	{
		u_activeBattleMap.insert(make_pair("1000-1",1));
		u_activeBattleMap.insert(make_pair("1001-1",1));
		u_activeBattleMap.insert(make_pair("1002-1",1));
	}
	else if (u_weekDay == L_TUESDAY)
	{
		//抽奖活动副本ID暂定为10000
		u_activeBattleMap.insert(make_pair("10000",1));
	}
	else if (u_weekDay == L_WEDNESDAY)
	{
		u_activeBattleMap.insert(make_pair("1003-1",1));
		u_activeBattleMap.insert(make_pair("1004-1",1));
		u_activeBattleMap.insert(make_pair("1005-1",1));
	}
	else if (u_weekDay == L_THURSDAY)
	{
		u_activeBattleMap.insert(make_pair("1006-1",1));
		u_activeBattleMap.insert(make_pair("1007-1",1));
		u_activeBattleMap.insert(make_pair("1008-1",1));
	}
	else if (u_weekDay == L_FRIDAY)
	{
		u_activeBattleMap.insert(make_pair("1009-1",1));
		u_activeBattleMap.insert(make_pair("1010-1",1));
		u_activeBattleMap.insert(make_pair("1011-1",1));
	}
	else if (u_weekDay == L_SATURDAY || u_weekDay == L_SUNDAY)
	{
		u_activeBattleMap.insert(make_pair("1012-1",1));
		u_activeBattleMap.insert(make_pair("1013-1",1));
		u_activeBattleMap.insert(make_pair("1014-1",1));
	}
	else
	{
		return false;
	}
	return true;
}

string cUpdateUserBaseInfo::getActiveBattleInsertList(const int& u_userId,activeBattleMap u_activeBattleMap)
{
	string retStrListTmp = "values";
	activeBattleMap::iterator it = u_activeBattleMap.begin();
	while (it != u_activeBattleMap.end())
	{
		char buf[32] = {0};
		sprintf(buf,"(%d,'%s',%d),",u_userId,(it->first).c_str(),it->second);
		retStrListTmp += buf;
		it++;
	}
	string retStrList = retStrListTmp.substr(0,retStrListTmp.size() - 1);
	return retStrList;
}

void cUpdateUserBaseInfo::insertUserActiveBattleInfo(const int& u_userId,const activeBattleMap& u_activeBattleMap)
{
	string u_insertList = getActiveBattleInsertList(u_userId,u_activeBattleMap);
	if (u_insertList.size() > 7)
	{
		char buf[512] = {0};
		sprintf(buf,"insert into m_useractivebattleinfo%d(userId,activeBattleId,activeBattleCount) %s;",u_userId%ACTIVE_BATTLE_COUNT,u_insertList.c_str());
		string strSql(buf);
		if (g_cMagicServerDb.execSQL(strSql) != 0)
		{
			g_cMyGlog.errorlogRecord("insertUserActiveBattleInfo error:%s\n",strSql.c_str());
		}
	}
}

void cUpdateUserBaseInfo::updateUserActiveBattleInfoByUserID(const int& u_userId)
{
	long u_timeStamp = GetTimeStamp();
	string u_strTime = getStringTime(u_timeStamp,0);
	int u_weekDay = getWeekDayByTime(u_strTime);
	activeBattleMap u_activeBattleMap;
	if (!getActiveBattleMapByWeekDay(u_weekDay,u_activeBattleMap))
	{
		return;
	}
	insertUserActiveBattleInfo(u_userId,u_activeBattleMap);
}

void cUpdateUserBaseInfo::updateUserActiveBattleInfo()
{
	char buf[128] = {0};
	sprintf(buf,"select userID from m_gameuserinfo;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("updateUserActiveBattleInfo select error:%s\n",strSql.c_str());
		q.finalize();
		return;
	}
	while (!q.eof())
	{
		updateUserActiveBattleInfoByUserID(q.getIntFileds("userID"));
		q.nextRow();
	}
	q.finalize();
	return;
}


//初始化领取友情值次数
void cUpdateUserBaseInfo::InitUserGetFriendValueCount()
{
	char buf[128] = {0};
	sprintf(buf,"update m_gameuserinfo set getFriendValueCount = 0;");
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("InitUserGetFriendValueCount error:%s\n",strSql.c_str());
	}
	return;
}

//初始化领取友情值的状态
void cUpdateUserBaseInfo::InitUserGetFriendValueFlag()
{
	char buf[256] = {0};
	int u_timeStamp = GetTimeStamp();
	for (int i = 0; i < USER_FRIEND_TABLE_COUNT; i++)
	{
		sprintf(buf,"update m_userFriendInfo%d set getStamp = %d,getFlag = %d,useFriendCount = 0;",i,u_timeStamp,0);
		string strSql(buf);
		if(g_cMagicServerDb.execSQL(strSql) != 0)
		{
			g_cMyGlog.errorlogRecord("updateGetFriendValueFlag error:%s\n",strSql.c_str());
		}
	}
	return;
}