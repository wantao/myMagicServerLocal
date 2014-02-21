#include "../include/procUserRankReq.h"
#include "../include/procUserBattle.h"
#include "../include/myGlog.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/procUserWaitingFriend.h"

cProcUserRankReq g_cProcUserRankReq;

cProcUserRankReq::cProcUserRankReq()
{

}
cProcUserRankReq::~cProcUserRankReq()
{

}

string cProcUserRankReq::getUserNameById(const unsigned int& u_userId)
{
	string n_retUserName = "";
	char buf[128] = {0};
	sprintf(buf,"select userName from m_gameuserinfo where userID=%d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("getUserNameById error11,strSql:%s\n",strSql.c_str());
	}
	n_retUserName = q.getStringFileds("userName");
	q.finalize();
	return n_retUserName;
}

string cProcUserRankReq::getPlayerRankSelectSql(const unsigned int& u_userId,const int& u_rankWays)
{
	char buf[512] = {0};
	if (u_rankWays == 1)
	{
		sprintf(buf,"select * from (Select *,(@rowNum:=@rowNum+1) as rowNo  \
					From m_user_fight_capacity, (Select (@rowNum :=0) ) b order by \
					m_user_fight_capacity.fightCapacity desc) AS a where userId=%d;",u_userId);
	}
	else if (u_rankWays == 2)
	{
		sprintf(buf,"select * from (Select *,(@rowNum:=@rowNum+1) as rowNo  \
					From m_user_fight_capacity, (Select (@rowNum :=0) ) b order by \
					m_user_fight_capacity.capitorFightCapacity desc) AS a where userId=%d;",u_userId);
	}
	else if (u_rankWays == 3)
	{
		sprintf(buf,"select * from (Select userID,userName,userLevel,(@rowNum:=@rowNum+1) as rowNo \
					From  m_gameuserinfo,(Select (@rowNum :=0) ) b order by \
					m_gameuserinfo.userLevel desc) AS a where userId=%d;",u_userId);
	}
	string n_PlayerRankSelectSql(buf);
	return n_PlayerRankSelectSql;
}

int cProcUserRankReq::getPlayerRank(const unsigned int& u_userId,const int& u_rankWays)
{
	int n_playerRank = 0;
	string n_strSql = getPlayerRankSelectSql(u_userId,u_rankWays);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(n_strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getPlayerRank error,strSql:%s\n",n_strSql.c_str());
		q.finalize();
		return n_playerRank;
	}
	n_playerRank = q.getIntFileds("rowNo");
	q.finalize();
	g_cMyGlog.debugLog("u_userId:%d,u_rankWays:%d,rank:%d\n",u_userId,u_rankWays,n_playerRank);
	return n_playerRank;

}

string cProcUserRankReq::getLastPlayerRankSelectSql(const unsigned int& u_userId,const int& u_rankWays)
{
	char buf[512] = {0};
	if (u_rankWays == 1)
	{
		sprintf(buf,"select max(rowNo) from (Select *,(@rowNum:=@rowNum+1) as rowNo  \
					From m_user_fight_capacity, (Select (@rowNum :=0) ) b order by \
					m_user_fight_capacity.fightCapacity desc) AS a;");
	}
	else if (u_rankWays == 2)
	{
		sprintf(buf,"select max(rowNo) from (Select *,(@rowNum:=@rowNum+1) as rowNo  \
					From m_user_fight_capacity, (Select (@rowNum :=0) ) b order by \
					m_user_fight_capacity.capitorFightCapacity desc) AS a;");
	}
	else if (u_rankWays == 3)
	{
		sprintf(buf,"select max(rowNo) from (Select userID,userName,userLevel,(@rowNum:=@rowNum+1) as rowNo \
					From  m_gameuserinfo,(Select (@rowNum :=0) ) b order by \
					m_gameuserinfo.userLevel desc) AS a;");
	}
	string n_lastPlayerRankSelectSql(buf);
	return n_lastPlayerRankSelectSql;
}

int cProcUserRankReq::getLastPlayerRank(const unsigned int& u_userId,const int& u_rankWays)
{
	int n_lastPlayerRank = 0;
	string n_strSql = getLastPlayerRankSelectSql(u_userId,u_rankWays);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(n_strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getLastPlayerRank error,strSql:%s\n",n_strSql.c_str());
		q.finalize();
		return n_lastPlayerRank;
	}
	n_lastPlayerRank = q.getIntFileds("max(rowNo)");
	q.finalize();
	g_cMyGlog.debugLog("u_userId:%d,u_rankWays:%d,lastRank:%d\n",u_userId,u_rankWays,n_lastPlayerRank);
	return n_lastPlayerRank;

}

int cProcUserRankReq::getSelectBaseCount(const unsigned int& u_userId,const int& u_rankWays,\
					   const int& u_reqWays,const int& u_count,int& u_rankStatus)
{
	int u_baseCount = 0;
	int n_playerRank = 0;
	if (u_reqWays == 1)
	{
		u_baseCount = 0;
	}
	else
	{
		int n_lastPlayerRank = getLastPlayerRank(u_userId,u_rankWays);
		if (u_count >= 0)
		{
			n_playerRank = getPlayerRank(u_userId,u_rankWays);
			if (n_playerRank > 0 && n_playerRank <= 15)
			{
				if (u_reqWays == 4 || u_reqWays == 2)
				{
					u_baseCount = 0;
					u_rankStatus = 1;
				}
				else if (u_reqWays == 3)
				{
					if (n_lastPlayerRank <= 30)
					{
						u_baseCount = 0;
						u_rankStatus = 2;
					}
					else
					{
						u_baseCount = u_count*30;
						if (n_lastPlayerRank-u_baseCount <= 30)
						{
							u_baseCount = n_lastPlayerRank - 30;
							if (u_baseCount < 0)
							{
								u_baseCount = 0;
							}
							u_rankStatus = 2;
						}
					}
				}

			}
			else if(n_playerRank > 15 && n_playerRank < (n_lastPlayerRank - 15))
			{
				if (u_reqWays == 4)
				{
					u_baseCount = n_playerRank - 15;
				}
				else if (u_reqWays == 2)
				{
					u_baseCount = n_playerRank - 15 - u_count*30;
					if (u_baseCount < 0)
					{
						u_baseCount = 0;
						u_rankStatus = 1;
					}
				}
				else if (u_reqWays == 3)
				{
					u_baseCount = n_playerRank + 15 + (u_count-1)*30;
					if (u_baseCount + 30 > n_lastPlayerRank)
					{
						//u_baseCount = n_lastPlayerRank - (30-(n_lastPlayerRank-u_baseCount));
						u_baseCount = n_lastPlayerRank - 30;
						u_rankStatus = 2;
					}
				}

			}
			else 
			{
				if (u_reqWays == 4 || u_reqWays == 3)
				{
					u_baseCount = n_lastPlayerRank - 30;
					if (u_baseCount < 0)
					{
						u_baseCount = 0;
					}
					u_rankStatus = 2;
				}
				else if (u_reqWays == 2)
				{
					u_baseCount = n_lastPlayerRank - (u_count+1)*30;
					if (u_baseCount < 0)
					{
						u_baseCount = 0;
						u_rankStatus = 1;
					}
				}
			}
		}
	}
	return u_baseCount;
}

string cProcUserRankReq::getSelectSql(const unsigned int& u_userId,const int& u_rankWays,\
					const int& u_reqWays,const int& u_count,int& u_rankStatus)
{
	//int u_userFightCapacity = 0,u_userCapitorCapacity = 0,u_userLevel = 0;
	char buf[1024] = {0};
	int u_baseCount = getSelectBaseCount(u_userId,u_rankWays,u_reqWays,u_count,u_rankStatus);
	if (u_rankWays == 1)
	{
		sprintf(buf,"select * from (Select m.*,n.userName,n.userLevel,(@rowNum:=@rowNum+1) as rowNo \
					From m_user_fight_capacity as m left join m_gameuserinfo as n \
					on m.userId = n.userID,(Select (@rowNum :=0) ) b order by \
					m.fightCapacity desc) AS a limit %d,30;",u_baseCount);
	}
	else if (u_rankWays == 2)
	{
		sprintf(buf,"select * from (Select m.*,n.userName,n.userLevel,(@rowNum:=@rowNum+1) as rowNo \
					From m_user_fight_capacity as m left join m_gameuserinfo as n \
					on m.userId = n.userID,(Select (@rowNum :=0) ) b order by \
					m.capitorFightCapacity desc) AS a limit %d,30;",u_baseCount);
	}
	else if (u_rankWays == 3)
	{
		sprintf(buf,"select * from (Select n.userName,n.userLevel,m.*,(@rowNum:=@rowNum+1) as rowNo \
					From m_gameuserinfo as n left join m_user_fight_capacity as m on n.userId = m.userID,\
					(Select (@rowNum :=0) ) b order by  n.userLevel desc) AS a limit %d,30",u_baseCount);
	}
	string strSql(buf);
	return strSql;
}

void cProcUserRankReq::fillPBRankRecv(PBRankRecv& u_PBRankRecv,MysqlQuery& q)
{
	while (!q.eof())
	{
		PBRankPlayerInfo* u_PBRankPlayerInfo;
		u_PBRankPlayerInfo = u_PBRankRecv.add_randplayerinfo();
		u_PBRankPlayerInfo->set_username(q.getStringFileds("userName"));
		u_PBRankPlayerInfo->set_userlevel(q.getIntFileds("userLevel"));
		u_PBRankPlayerInfo->set_fightcapacity(q.getIntFileds("fightCapacity"));
		u_PBRankPlayerInfo->set_capitorcardfightcapacity(q.getIntFileds("capitorFightCapacity"));
		u_PBRankPlayerInfo->set_playerrank(q.getIntFileds("rowNo"));
		PBCard* u_PBCard;
		u_PBCard = u_PBRankPlayerInfo->mutable_usercard();
		g_cProcUserWaitingFriend.getCaptainCardInfo(u_PBCard,q.getIntFileds("userId"));
		q.nextRow();

	}
	q.finalize();
}


int cProcUserRankReq::proxysetPBRankRecv(PBRankRecv& u_PBRankRecv,const unsigned int& u_userId,const PBRankSend& u_PBRankSend)
{
	int u_result = 0;
	//排名方式 1，按小队战斗力 2，按队长战斗力 3，玩家等级
	int u_rankWays = u_PBRankSend.rankways();
	//1,查看全区前30位玩家排名，2，查看前30位玩家排名，3，查看后30位玩家排名
	int u_reqWays = u_PBRankSend.reqways();
	//reqWays为2时表示查看前30位玩家排名的次数，为3表示查看后30位玩家排名的次数
	int u_count = u_PBRankSend.count();
	g_cMyGlog.debugLog("u_userId:%d,u_rankWays:%d,u_reqWays:%d,u_count:%d\n",u_userId,u_rankWays,u_reqWays,u_count);
	int u_rankStatus = 0;
	string strSql = getSelectSql(u_userId,u_rankWays,u_reqWays,u_count,u_rankStatus);
	g_cMyGlog.debugLog("getSelectSql:%s\n",strSql.c_str());
	if (strSql.size() <= 0)
	{
		u_result = -1;
		return u_result;
	}
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		fillPBRankRecv(u_PBRankRecv,q);
	}
	u_PBRankRecv.set_resultstatus(u_rankStatus);
	g_cMyGlog.debugLog("u_rankStatus:%d\n",u_PBRankRecv.resultstatus());
	q.finalize();
	return u_result;
	
}


int cProcUserRankReq::setPBRankRecv(PBRankRecv& u_PBRankRecv,const string& u_strToken,const PBRankSend& u_PBRankSend)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBRankRecv getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	if (u_PBRankSend.rankways() > 3)
	{
		g_cMyGlog.errorlogRecord("rankWays error:%d\n",u_PBRankSend.rankways());
		return -1;
	}
	u_result = proxysetPBRankRecv(u_PBRankRecv,u_userId,u_PBRankSend);
	return u_result;
}