#include "../include/procUserWaitingFriend.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"

cProcUserWaitingFriend g_cProcUserWaitingFriend;

cProcUserWaitingFriend::cProcUserWaitingFriend()
{

}
cProcUserWaitingFriend::~cProcUserWaitingFriend()
{

}
//填充等待确认好友的队长卡牌信息
bool cProcUserWaitingFriend::getCaptainCardInfo(PBCard* u_PBCard,const unsigned int& u_userID)
{
	char buf[256] = {0};
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
//填充等待确认好友的信息
bool cProcUserWaitingFriend::getFriendInfo(PBFriend* u_PBFriend,const unsigned& u_userId)
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


//根据用户ID得到等待确认好友ID，并填充等待确认好友信息
bool cProcUserWaitingFriend::getUserWaitingFriend(PBWaitingFriend& u_PBWaitingFriend,const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select userId from m_userFriendReqInfo%d where \
				reqFriendId = %d;",u_userId%USER_FRIEND_REQ_TABLE_COUNT,u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("getUserWaitingFriend m_userFriendReqInfo not find userId:strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	else
	{
		while (!q.eof())
		{
			PBFriend* u_PBFriend;
			u_PBFriend = u_PBWaitingFriend.add_waitingfriend();
			getFriendInfo(u_PBFriend,q.getIntFileds("userId"));
			q.nextRow();
		}
	}
	q.finalize();
	return true;
}

//处理等待确认好友请求
int cProcUserWaitingFriend::procUserWaitingFriend(PBWaitingFriend& u_PBWaitingFriend,const string& u_strToken)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("procUserWaitingFriend getUserIdbyToken error:Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	getUserWaitingFriend(u_PBWaitingFriend,u_userId);
	return u_result;

}