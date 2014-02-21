#include "../include/procUserSearchFriend.h"
#include "../include/magicServerDb.h"
#include "../include/procUserGetAllFriend.h"
#include "../include/errorCode.h"
#include "../include/common.h"
#include "../include/define.h"
#include "../include/myGlog.h"

cProcUserSearchFriend g_cProcUserSearchFriend;

#define SHOW_SEARCH_PLAY_COUNT	10

cProcUserSearchFriend::cProcUserSearchFriend()
{

}

cProcUserSearchFriend::~cProcUserSearchFriend()
{

}

//获取好友ID列表
string cProcUserSearchFriend::getUserFriendIdList(const unsigned int& u_userId)
{
	string u_friendIdList = "";
	string u_tmpFriendIdList = "";
	char buf[128] = {0};
	sprintf(buf,"select friendId from m_userFriendInfo%d where \
				userId = %d;",u_userId%USER_FRIEND_TABLE_COUNT,u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("getUserFriendIdList get nothing!,strSql:%s\n",strSql.c_str());
		q.finalize();
		return u_friendIdList;
	}
	while (!q.eof())
	{
		char buf[12] = {0};
		sprintf(buf,"%d,",q.getIntFileds("friendId"));
		u_tmpFriendIdList += buf;
		q.nextRow();
	}
	if (u_tmpFriendIdList.size() > 0)
	{
		u_friendIdList = u_tmpFriendIdList.substr(0,u_tmpFriendIdList.size()-1);
	}
	q.finalize();
	return u_friendIdList;
}

//代理填充SetPBSearchFriendReceive
int cProcUserSearchFriend::proxySetPBSearchFriendReceive(PBSearchFriendReceive& u_PBSearchFriendReceive,const unsigned int& u_userId,\
								   PBSearchFriendSend& u_PBSearchFriendSend)
{
	string u_searchKey = u_PBSearchFriendSend.searchkey();
	string u_friendIdList = getUserFriendIdList(u_userId);
	char buf[1024] = {0};
	int u_result = 0;
	if (u_friendIdList.size() > 0)
	{
		sprintf(buf,"select userID from m_gameuserinfo where userName like '\%\%%s\%%' and userID not in (%s) and userID != %d order by userName asc limit %d;",\
			u_searchKey.c_str(),u_friendIdList.c_str(),u_userId,SHOW_SEARCH_PLAY_COUNT);
	}
	else
	{
		sprintf(buf,"select userID from m_gameuserinfo where userName like '\%\%%s\%%' and userID != %d order by userName asc limit %d",\
			u_searchKey.c_str(),u_userId,SHOW_SEARCH_PLAY_COUNT);
	}
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("proxySetPBSearchFriendReceive getNothing!,strSql:%s\n",strSql.c_str());
		q.finalize();
		return u_result;
	}
	while (!q.eof())
	{
		PBFriend* u_PBFriend;
		u_PBFriend = u_PBSearchFriendReceive.add_friends();
		g_cProcUserGetAllFriend.getFriendInfo(u_PBFriend,q.getIntFileds("userID"));
		q.nextRow();
	}
	q.finalize();
	return u_result;
}

int cProcUserSearchFriend::setPBSearchFriendReceive(PBSearchFriendReceive& u_PBSearchFriendReceive,const string& u_strToken,\
						 PBSearchFriendSend& u_PBSearchFriendSend)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBSearchFriendReceive getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	u_result = proxySetPBSearchFriendReceive(u_PBSearchFriendReceive,u_userId,u_PBSearchFriendSend);
	return u_result;
}