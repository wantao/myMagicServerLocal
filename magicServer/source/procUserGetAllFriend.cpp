#include "../include/procUserGetAllFriend.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"

cProcUserGetAllFriend g_cProcUserGetAllFriend;
cProcUserGetAllFriend::cProcUserGetAllFriend()
{

}
cProcUserGetAllFriend::~cProcUserGetAllFriend()
{

}

//填充好友队长卡牌信息
bool cProcUserGetAllFriend::getCaptainCardInfo(PBCard* u_PBCard,const unsigned int& u_userID)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_userCardInfo%d where userId = %d and battlePosition = %d;",u_userID%USER_CARD_TABLE_COUNT,u_userID,CAPITOR_POSITION);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getCaptainCardInfo error:strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
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
	return true;
}

//填充好友信息
bool cProcUserGetAllFriend::getFriendInfo(PBFriend* u_PBFriend,const unsigned& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select userName,userLevel from m_gameUserInfo \
				where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getFriendInfo m_gameUserInfo not find userId:strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	else
	{
		u_PBFriend->set_friendid(u_userId);
		u_PBFriend->set_friendname(q.getStringFileds("userName"));
		u_PBFriend->set_friendlevel(q.getIntFileds("userLevel"));
		PBCard* u_PBCard;
		u_PBCard = u_PBFriend->mutable_friendcard();
		getCaptainCardInfo(u_PBCard,u_userId);

		int u_userFightCapacity = 0;
		int u_userCapitorCardFightCapacity = 0;
		getUserFightCapacity(u_userId,u_userFightCapacity,\
			u_userCapitorCardFightCapacity);
		u_PBFriend->set_fightcapacity(u_userFightCapacity);
		u_PBFriend->set_capitorcardfightcapacity(u_userCapitorCardFightCapacity);
	}
	q.finalize();
	return true;
}

//更新用户领取友情值状态
void cProcUserGetAllFriend::updateGetFriendValueFlag(const unsigned int& u_userId,const unsigned int& u_friendId,\
							  bool u_getFlag)
{
	char buf[256] = {0};
	int u_timeStamp = GetTimeStamp();
	sprintf(buf,"update m_userFriendInfo%d set getStamp = %d,getFlag = %d \
				where userId = %d and friendId = %d;",u_userId%USER_FRIEND_TABLE_COUNT,\
				u_timeStamp,u_getFlag,u_userId,u_friendId);
	string strSql(buf);
	if(g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateGetFriendValueFlag error:strSql:%s\n",strSql.c_str());
	}	
}

//获取领取友情值状态
bool cProcUserGetAllFriend::getFriendValueFlag(const unsigned int& u_userId,const unsigned int& u_friendId,\
						int& u_getStamp,int& u_getFriendValueFlag)
{
	char buf[128] = {0};
	sprintf(buf,"select getStamp,getFlag from m_userFriendInfo%d where \
				userId = %d and friendId = %d;",u_userId%USER_FRIEND_TABLE_COUNT,u_userId,u_friendId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getFriendValueFlag,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_getStamp = q.getIntFileds("getStamp");
	u_getFriendValueFlag = q.getIntFileds("getFlag");
#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("u_getFriendValueFlag:%d\n",u_getFriendValueFlag);
#endif
	q.finalize();
	return true;
}

//设置PBFriend中是否已领取友情值标志
void cProcUserGetAllFriend::setPBFriendFlag(PBFriend* &u_PBFriend,const unsigned int& u_userId,\
					 const unsigned int& u_friendId)
{
	int u_getStamp = 0,u_getFriendValueFlag = 0;
	if(getFriendValueFlag(u_userId,u_friendId,u_getStamp,u_getFriendValueFlag))
	{
		int u_timeDiff = 0;
		int u_nowStamp = GetTimeStamp();
		u_timeDiff = (u_nowStamp - u_getStamp)/43200;
		bool u_getFlag = false;
		if (u_timeDiff >= 1)
		{
			u_getFlag = false;
			updateGetFriendValueFlag(u_userId,u_friendId,u_getFlag);
			u_PBFriend->set_isgetfriendvalue(u_getFlag);
		}
		else
		{
			if (u_getFriendValueFlag == 0)
			{
				u_getFlag = false;
			}
			else
			{
				u_getFlag = true;
			}
			u_PBFriend->set_isgetfriendvalue(u_getFlag);
		}
	}
	else
	{
		u_PBFriend->set_isgetfriendvalue(true);
	}
}



//获取特定用户ID的所有好友信息
bool cProcUserGetAllFriend::getAllFriendByUserId(PBAllFriend& u_PBAllFriend,const unsigned& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select friendId,getStamp,getFlag from m_userFriendInfo%d where \
				userId = %d;",u_userId%USER_FRIEND_TABLE_COUNT,u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord(" getAllFriendByUserId m_userFriendInfo not find userId:,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	else
	{
		while (!q.eof())
		{
			PBFriend* u_PBFriend;
			u_PBFriend = u_PBAllFriend.add_allfriend();
			getFriendInfo(u_PBFriend,q.getIntFileds("friendId"));
			setPBFriendFlag(u_PBFriend,u_userId,q.getIntFileds("friendId"));
			q.nextRow();
		}
	}
	q.finalize();
	return true;
}

//获取用户所有好友信息
int cProcUserGetAllFriend::getUserAllFriend(PBAllFriend& u_PBAllFriend,const string& u_strToken)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("getUserAllFriend getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}

	getAllFriendByUserId(u_PBAllFriend,u_userId);
	return u_result;
	
}