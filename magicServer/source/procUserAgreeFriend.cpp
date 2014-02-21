#include "../include/procUserAgreeFriend.h"
#include "../include/procUserWaitingFriend.h"
#include "../include/procUserGetAllFriend.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/common.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"

cProcUserAgreeFriend g_cProcUserAgreeFriend;

cProcUserAgreeFriend::cProcUserAgreeFriend()
{

}
cProcUserAgreeFriend::~cProcUserAgreeFriend()
{

}

//判断用户是否存在
bool cProcUserAgreeFriend::isUserExist(const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select userID from m_gameUserInfo where \
				userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("isUserExist m_userFriendInfo not find userId:%d\n",u_userId);
		q.finalize();
		return false;
	}
	q.finalize();
	return true;
}

//记录是否已经存在
bool cProcUserAgreeFriend::hasHistoryRecord(const unsigned int& u_userId,const unsigned int& u_friendId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_userFriendInfo%d where \
				userId = %d and friendId = %d;",u_userId%USER_FRIEND_TABLE_COUNT,\
				u_userId,u_friendId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		q.finalize();
		return true;
	}
	g_cMyGlog.errorlogRecord("in hasHistoryRecord error,strSql:%s\n",strSql.c_str());
	q.finalize();
	return false;
}

//插入记录
bool cProcUserAgreeFriend::insertRecord(const unsigned int& u_userId,const unsigned int& u_friendId)
{
	char buf[128] = {0};
	sprintf(buf,"insert into m_userFriendInfo%d(userId,friendId) \
				values(%d,%d)",u_userId%USER_FRIEND_TABLE_COUNT,\
				u_userId,u_friendId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("11111111insert Record error,strSql:%s\n",strSql.c_str());
		return false;
	}
	return true;
}

//删除记录
bool cProcUserAgreeFriend::deleteRecord(const unsigned int& u_userId,const unsigned int& u_friendId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_userFriendReqInfo%d where reqFriendId=%d and\
				userId = %d;",u_userId%USER_FRIEND_REQ_TABLE_COUNT,u_userId,u_friendId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("AgreeFriend deleteRecord faliure,strSql:%s\n",strSql.c_str());
		return false;
	}
	return true;
}

//填充PBFriendAgreeReceive
int cProcUserAgreeFriend::setPBFriendAgreeReceive(PBFriendAgreeReceive& u_PBFriendAgreeReceive,const string& u_strToken,\
							 const unsigned int& u_friendId)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("procUserAddFriend getUserIdbyToken error:Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	if (!isUserExist(u_friendId))
	{
		deleteRecord(u_userId,u_friendId);
		return K_FRIEND_IS_NOT_EXIST;
	}
	if (hasHistoryRecord(u_userId,u_friendId))
	{
		deleteRecord(u_userId,u_friendId);
		return K_OPPOSIT_IS_FRIEND;
	}
	else
	{
		if (!insertRecord(u_userId,u_friendId) || !insertRecord(u_friendId,u_userId))
		{
			u_result = -1;
		}
		else
		{
			deleteRecord(u_userId,u_friendId);
			u_PBFriendAgreeReceive.set_agreeresult(ADD_FRIEND_SUCCESS);
			PBWaitingFriend* u_PBWaitingFriend;
			u_PBWaitingFriend = u_PBFriendAgreeReceive.mutable_waitings();
			g_cProcUserWaitingFriend.getUserWaitingFriend(*u_PBWaitingFriend,u_userId);

			PBAllFriend* u_PBAllFriend;
			u_PBAllFriend = u_PBFriendAgreeReceive.mutable_friends();
			g_cProcUserGetAllFriend.getAllFriendByUserId(*u_PBAllFriend,u_userId);

		}
	}
	return u_result;
	
}