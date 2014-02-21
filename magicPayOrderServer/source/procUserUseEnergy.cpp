#include "../include/procUserUseEnergy.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/procUserLogin.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"
#include "../include/procUserMissionAward.h"

cProcUserUseEnergy g_cProcUserUseEnergy;

cProcUserUseEnergy::cProcUserUseEnergy()
{

}
cProcUserUseEnergy::~cProcUserUseEnergy()
{
	
}

//得到用户当前体力，钻石数，购买体力次数，上次恢复体力的时间戳
bool cProcUserUseEnergy::getUserEnergyAndDiamond(const unsigned int& u_userId,int& u_energy,int& u_diamondNumber,\
							 int& u_buyEnergyTime,int& u_lastEnergyTime,int& u_maxEnergy)
{
	char buf[128] = {0};
	sprintf(buf,"select energy,diamondNumber,buyEnergyTime,lastEnergyTime,maxEnergy from m_gameUserInfo where \
				userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserEnergyAndDiamond m_gameUserInfo not find userId,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_energy = q.getIntFileds("energy");
	u_diamondNumber = q.getIntFileds("diamondNumber");
	u_buyEnergyTime = q.getIntFileds("buyEnergyTime");
	u_lastEnergyTime = q.getIntFileds("lastEnergyTime");
	u_maxEnergy = q.getIntFileds("maxEnergy");
	q.finalize();
	return true;
}

//更新购买体力后的用户信息
bool cProcUserUseEnergy::updateUserEnergy(const unsigned int& u_userId,int& u_energy,int& u_diamondNumber,\
					  int& u_buyEnergyTime,int& u_lastEnergyTime)
{
	char buf[256] = {0};
	sprintf(buf,"update m_gameUserInfo set energy=%d,diamondNumber=%d, \
				buyEnergyTime=%d,lastEnergyTime=%d where userID = %d;",\
				u_energy,u_diamondNumber,u_buyEnergyTime,u_lastEnergyTime,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUserEnergy error,strSql:%s\n",strSql.c_str());
	}
	return true;
}

//是否有完成的任务
bool cProcUserUseEnergy::hasMissionFinished(const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_userMissionsInfo%d where userId = %d and \
				isMissinFinished = 1;",u_userId%USER_MISSION_TABLE_COUNT,u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("no mission finished,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	q.finalize();
	return true;
}

//填充用户信息
bool cProcUserUseEnergy::setPBUser(PBUser* u_PBUser,const unsigned int& u_userId,\
				const unsigned int& u_nextEnergyTime)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_gameUserInfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("m_gameUserInfo not find userId:,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	else
	{
		u_PBUser->set_userid(q.getIntFileds("userID"));
		u_PBUser->set_username(q.getStringFileds("userName"));
		u_PBUser->set_userlevel(q.getIntFileds("userLevel"));
		u_PBUser->set_userexp(q.getIntFileds("userExp"));
		u_PBUser->set_energy(q.getIntFileds("energy"));
		
		u_PBUser->set_nextenergytime(u_nextEnergyTime);
		u_PBUser->set_buyenergytime(q.getIntFileds("buyEnergyTime"));
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

//获取用户等级和体力
bool cProcUserUseEnergy::getUserLevelAndEnergyPotionCount(const unsigned int& u_userId,int& u_userLevel,\
						   int& u_lastEnergyTime,int& u_energyPotionCount,int& u_maxEnergy)
{
	char buf[128] = {0};
	sprintf(buf,"select userLevel,lastEnergyTime,maxEnergy,energyCount from m_gameuserinfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserLevelAndEnergy error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_userLevel = q.getIntFileds("userLevel");
	u_lastEnergyTime = q.getIntFileds("lastEnergyTime");
	u_maxEnergy = q.getIntFileds("maxEnergy");
	u_energyPotionCount = q.getIntFileds("energyCount");
	q.finalize();
	return true;
}
//更新用户体力和体力药水个数
void cProcUserUseEnergy::updateUserEnergyAndEnergyPotion(const unsigned int& u_userId,const int& u_energy,\
									 const int& u_lastEnergyTime,const int& u_maxEnergy,const int& u_energyPotionCount)
{
	char buf[256] = {0};
	sprintf(buf,"update m_gameuserinfo set energy = %d,lastEnergyTime = %d,maxEnergy = %d,energyCount = %d where userID = %d;",\
		u_energy,u_lastEnergyTime,u_maxEnergy,u_energyPotionCount,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUserEnergyAndEnergyPotion error,strSql:%s\n",strSql.c_str());
	}
}

//获取体力恢复倒计时
int cProcUserUseEnergy::getNextEnergyTime(const int& u_nowStamp,int& u_lastEnergyTimeStamp,\
					  int& u_energy,const int& u_userLevel)
{
	int u_timeDif = u_nowStamp - u_lastEnergyTimeStamp;
	int u_maxEnergy = ENERGY + (u_userLevel-1)/2;
	int u_nextEnergyTime = 0;
	if (u_timeDif < ONE_ENERGY_NEED_TIME)
	{
		if (u_energy >= u_maxEnergy)
		{
			u_energy = u_maxEnergy;
			u_nextEnergyTime = 0;
		}
		else
		{
			u_nextEnergyTime = ONE_ENERGY_NEED_TIME - u_timeDif;
		}
	}
	else
	{
		if (u_energy >= u_maxEnergy)
		{
			u_energy = u_maxEnergy;
			u_nextEnergyTime = 0;
		}
		else
		{
			u_energy += u_timeDif/ONE_ENERGY_NEED_TIME;
			if (u_energy >= u_maxEnergy)
			{
				u_energy = u_maxEnergy;
				u_nextEnergyTime = 0;
			}
			else
			{
				u_nextEnergyTime = ONE_ENERGY_NEED_TIME - u_timeDif%ONE_ENERGY_NEED_TIME;
			}
			
		}
		u_lastEnergyTimeStamp = u_nowStamp;
	}
	return u_nextEnergyTime;
}

//填充购买体力请求的返回结果
int cProcUserUseEnergy::setPBUseEnergyReceive(PBUseEnergyReceive& u_PBUseEnergyReceive,const string& u_strToken)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("getUserAllFriend getUserIdbyToken error:Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	int u_energy=0,u_lastEnergyTime=0,u_maxEnergy = 0;
	int u_energyPotionCount = 0,u_userLevel = 0;
	if (!getUserLevelAndEnergyPotionCount(u_userId,u_userLevel,\
		u_lastEnergyTime,u_energyPotionCount,u_maxEnergy))
	{
		u_result = -1;
		return u_result;
	}
	else
	{	
			u_energy = ENERGY + (u_userLevel-1)/2;
			u_maxEnergy = u_energy;
			u_energyPotionCount -= 1;
			//int u_nextEnergyTime = (GetTimeStamp()-u_lastEnergyTime)%ONE_ENERGY_NEED_TIME;
			int u_nextEnergyTime = 0;
			int u_nowTimeStamp = 0;
			u_nowTimeStamp = GetTimeStamp();
			u_nextEnergyTime = getNextEnergyTime(u_nowTimeStamp,u_lastEnergyTime,u_energy,u_userLevel);
			updateUserEnergyAndEnergyPotion(u_userId,u_energy,u_lastEnergyTime,u_maxEnergy,u_energyPotionCount);
			PBUser* u_PBUser;
			u_PBUser = u_PBUseEnergyReceive.mutable_user();
			if (!setPBUser(u_PBUser,u_userId,u_nextEnergyTime))
			{
				g_cMyGlog.errorlogRecord("setPBBuyEnergyReceive failure\n");
				u_result = -1;
				return u_result;
			}
	}
	return u_result;
}


bool cProcUserUseEnergy::getNextEnergyTimeNeedInfo(const unsigned int& u_userId,int& u_lastEnergyTime,int& u_energy,int& u_userLevel)
{
	char buf[256] = {0};
	sprintf(buf,"select userLevel,energy,lastEnergyTime from m_gameUserInfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getNextEnergyTimeNeedInfo error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_lastEnergyTime = q.getIntFileds("lastEnergyTime");
	u_energy = q.getIntFileds("energy");
	u_userLevel = q.getIntFileds("userLevel");
	q.finalize();
	return true;

}

int cProcUserUseEnergy::getUserEnergy(PBEnergyReceive& u_PBEnergyReceive,const string& u_strToken)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	int u_lastEnergyTime = 0,u_energy = 0,u_userLevel = 0;
	int u_NextEnergyTime = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("getUserEnergy getUserIdbyToken error:Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	if (getNextEnergyTimeNeedInfo(u_userId,u_lastEnergyTime,u_energy,u_userLevel))
	{
		int u_nowTimeStamp = 0;
		u_nowTimeStamp = GetTimeStamp();
		u_NextEnergyTime = getNextEnergyTime(u_nowTimeStamp,u_lastEnergyTime,u_energy,u_userLevel);

		unsigned int u_Energy = u_energy;
		g_cProUserMissionAward.updateUserEnergyAndTime(u_Energy,u_lastEnergyTime,u_userId);
	}
	u_PBEnergyReceive.set_energy(u_energy);
	u_PBEnergyReceive.set_nextenergytime(u_NextEnergyTime);
	return u_result;
}