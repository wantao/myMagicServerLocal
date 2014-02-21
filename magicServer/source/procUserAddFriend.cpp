#include "../include/procUserAddFriend.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/common.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"

cProcUserAddFriend g_cProcUserAddFriend;

cProcUserAddFriend::cProcUserAddFriend()
{
	
}
cProcUserAddFriend::~cProcUserAddFriend()
{

}

//判断用户是否存在
bool cProcUserAddFriend::isUserExist(const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select userID from m_gameUserInfo where \
				userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord(" isUserExist m_userFriendInfo not find userId:%d\n",u_userId);
		q.finalize();
		return false;
	}
	q.finalize();
	return true;
}

int cProcUserAddFriend::getUserMaxFriendCount(const unsigned int& u_userId)
{
	int u_userMaxFriendCount = USER_MAX_FRIEND_COUNT;
	int u_addFriendCount = 0;
	char buf[128] = {0};
	sprintf(buf,"select userLevel from m_gameuserinfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserMaxFriendCount error,strSql:%s\n",strSql.c_str());
	}
	else
	{
		u_addFriendCount = q.getIntFileds("userLevel") - 1;
	}
	u_userMaxFriendCount += u_addFriendCount;
	if (u_userMaxFriendCount > USER_FINAL_MAX_FRIEND_COUNT)
	{
		u_userMaxFriendCount = USER_FINAL_MAX_FRIEND_COUNT;
	}
	q.finalize();
	return u_userMaxFriendCount;
}

//判断用户好友是否达到上限
bool cProcUserAddFriend::isUserFriendToMax(const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select count(friendId) from m_userFriendInfo%d where \
				userId = %d;",u_userId%USER_FRIEND_TABLE_COUNT,u_userId);
	string strSql(buf);
	int nRow = 0;
	nRow = g_cMagicServerDbRead.getIntFiled(strSql);
	int u_userMaxFriendCount = getUserMaxFriendCount(u_userId);
	if (nRow >= u_userMaxFriendCount)
	{
		return true;
	}
	return false;
}

//该插入记录是否已经存在
bool cProcUserAddFriend::hasHistoryRecord(const unsigned int& u_userId,const unsigned int& u_friendId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_userFriendReqInfo%d where \
				userId = %d and reqFriendId = %d;",u_friendId%USER_FRIEND_REQ_TABLE_COUNT,u_userId,u_friendId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		q.finalize();
		return false;
	}
	q.finalize();
	return true;

}

//插入记录
bool cProcUserAddFriend::insertRecord(const unsigned int& u_userId,const unsigned int& u_friendId)
{
	char buf[256] = {0};
	sprintf(buf,"insert into m_userFriendReqInfo%d(userId,reqFriendId) \
				values(%d,%d);",u_friendId%USER_FRIEND_REQ_TABLE_COUNT,u_userId,u_friendId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord(" insertRecord error:sql:%s\n",strSql.c_str());
		return false;
	}
	return true;

}

//插入添加好友请求
bool cProcUserAddFriend::insertAddFriendReq(const unsigned int& u_userId,const unsigned int& u_friendId)
{
	if (hasHistoryRecord(u_userId,u_friendId))
	{
		return true;
	}
	else
	{
		if (!insertRecord(u_userId,u_friendId))
		{
			return false;
		}
	}
	return true;
}

//对方是否已经是好友
bool cProcUserAddFriend::isFriend(const unsigned int& u_userId,const unsigned int& u_friendId)
{
	char buf[128] = {0};
	sprintf(buf,"select count(friendId) from m_userFriendInfo%d where \
				userId = %d and friendId = %d;",u_userId%USER_FRIEND_TABLE_COUNT,u_userId,u_friendId);
	string strSql(buf);
	int nRow = 0;
	nRow = g_cMagicServerDbRead.getIntFiled(strSql);
	if (nRow > 0)
	{
		return true;
	}
	return false;
}

//处理用户添加好友请求
int cProcUserAddFriend::procUserAddFriend(PBAddFriendReceive& u_PBAddFriendReceive,const string& u_strToken,\
					   const unsigned int& u_friendId)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord(" procUserAddFriend getUserIdbyToken error:Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	if (u_userId == u_friendId)
	{
		g_cMyGlog.errorlogRecord("cannot add youself as friend\n");
		return K_OPPOSIT_IS_FRIEND;
	}

	if (isUserExist(u_friendId))
	{
		if (isFriend(u_userId,u_friendId))
		{
			return K_OPPOSIT_IS_FRIEND;
		}
		else
		{
			if (isUserFriendToMax(u_friendId))
			{
				return K_OPPOSIT_FRIEND_FULL;
			}
			else if (isUserFriendToMax(u_userId))
			{
				return K_OWN_FRIEND_FULL;
			}
			else
			{
				if (insertAddFriendReq(u_userId,u_friendId))
				{
					u_PBAddFriendReceive.set_addresult(ADD_FRIEND_SUCCESS);
				}
				else
				{
					u_result = -1;
				}
				
			}
		}

	}
	else
	{
		return K_FRIEND_IS_NOT_EXIST;
	}
	return u_result;
	
}