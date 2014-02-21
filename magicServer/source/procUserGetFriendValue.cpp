#include "../include/procUserGetFriendValue.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/procUserAgreeFriend.h"
#include "../include/magicServerDb.h"
#include "../include/procUserAwardFriend.h"
#include "../include/procUserGetAllFriend.h"
#include "../include/errorCode.h"
#include "../include/define.h"
#include "../include/myGlog.h"

cProcUserGetFriendValue g_cProcUserGetFriendValue;


cProcUserGetFriendValue::cProcUserGetFriendValue()
{

}
cProcUserGetFriendValue::~cProcUserGetFriendValue()
{

}

//获取用户已领取友情值次数
bool cProcUserGetFriendValue::getFriendValueCount(const unsigned int& u_userId,int& u_getFriendValueCount)
{
	char buf[128] = {0};
	sprintf(buf,"select getFriendValueCount from m_gameuserinfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getFriendValueCount error!,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_getFriendValueCount = q.getIntFileds("getFriendValueCount");
	q.finalize();
	return true;
}

//更新已领取友情值次数
bool cProcUserGetFriendValue::updateGetFriendValueCount(const unsigned int& u_userId,const int& u_getFriendValueCount)
{
	char buf[128] = {0};
	sprintf(buf,"update m_gameuserinfo set getFriendValueCount = %d where userID = %d;",u_getFriendValueCount,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateGetFriendValueCount error,strSql:%s\n",strSql.c_str());
		return false;
	}
	return true;
}

int cProcUserGetFriendValue::setPBGetFriendValueReceive(PBGetFriendValueReceive& u_PBGetFriendValueReceive,const string& u_strToken,const PBFriendSend& u_PBFriendSend)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	int u_getFriendValueCount = 0;
	unsigned int u_friendValue = 0;

	int u_getFriendValueFlag = 0;
	int u_timeStamp = 0;
	int u_friendId = u_PBFriendSend.friendid();
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBGetFriendValueReceive getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	if (!g_cProcUserAgreeFriend.isUserExist(u_friendId) || \
		!g_cProcUserAgreeFriend.hasHistoryRecord(u_userId,u_friendId))
	{
		return K_FRIEND_IS_NOT_EXIST;
	}
	if (!getFriendValueCount(u_userId,u_getFriendValueCount))
	{
		u_result = -1;
		return u_result;
	}
	if (u_getFriendValueCount >= GET_FRIEND_VALUE_MAX_COUNT)
	{
		return K_TO_MAX_GET_FRIEND_VALUE_COUNT;
	}
	if(!g_cProcUserGetAllFriend.getFriendValueFlag(u_userId,u_friendId,u_timeStamp,u_getFriendValueFlag))
	{
		u_result = -1;
		return u_result;
	}
	if (u_getFriendValueFlag == 1)
	{
		return K_FRIEND_VALUE_HAS_GOT;
	}
	g_cProcUserAwardFriend.getUserFriendValue(u_userId,u_friendValue);
	if (u_friendValue >= MAX_FRIEND_VALUE)
	{
		return K_TO_MAX_FRIEND_VALUE;
		//u_friendValue = MAX_FRIEND_VALUE;
	}
	else
	{
		u_friendValue += FRIEND_ADD_VALUE;
		if (u_friendValue >= MAX_FRIEND_VALUE)
		{
			u_friendValue = MAX_FRIEND_VALUE;
		}
		g_cProcUserAwardFriend.updataUserFriendValue(u_userId,u_friendValue);
	}
	u_getFriendValueCount += 1;
	if (!updateGetFriendValueCount(u_userId,u_getFriendValueCount))
	{
		u_result = -1;
		return u_result;
	}
	g_cProcUserGetAllFriend.updateGetFriendValueFlag(u_userId,u_friendId,true);

	PBAllFriend* u_PBAllFriend;
	u_PBAllFriend = u_PBGetFriendValueReceive.mutable_friends();
	g_cProcUserGetAllFriend.getAllFriendByUserId(*u_PBAllFriend,u_userId);
	u_PBGetFriendValueReceive.set_friendvalue(u_friendValue);

	return u_result;
	
}