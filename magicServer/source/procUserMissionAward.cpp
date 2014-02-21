#include "../include/procUserMissionAward.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/procUserLogin.h"
#include "../include/loginMission.h"
#include "../include/procUserCardLevelUp.h"
#include "../include/procUserAwardFriend.h"
#include "../include/procUserBattle.h"
#include "../include/procUserDiamondAward.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"
#include "../include/procUserUseEnergy.h"

cProUserMissionAward g_cProUserMissionAward;

cProUserMissionAward::cProUserMissionAward()
{

}
cProUserMissionAward::~cProUserMissionAward()
{

}

//获取连续登陆天数
int cProUserMissionAward::getLoginDays(const unsigned int& u_userId,int& u_isLoginAwarded)
{
	int u_loginDays = 0;
	char buf[128] = {0};
	sprintf(buf,"select loginDays,isLoginAwarded from m_gameuserinfo where userID=%d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getLoginDays error,strSql:%s\n",strSql.c_str());
	}
	else
	{
		u_loginDays = q.getIntFileds("loginDays");
		u_isLoginAwarded = q.getIntFileds("isLoginAwarded");
	}
	q.finalize();
	return u_loginDays;
}


//该登陆任务是否应该被奖励
int cProUserMissionAward::isLoginMissionShouldAward(const unsigned int& u_userId,const unsigned int& u_missionId)
{
	int u_loginDays = 0;
	int u_isLoginAwarded = 0;
	u_loginDays = getLoginDays(u_userId,u_isLoginAwarded);
	if (u_loginDays == 0 || u_missionId != u_loginDays)
	{
		return K_MISSION_NOT_FINISHED;
	}
	if (u_isLoginAwarded == 1)
	{
		return K_AWARD_HAS_GOT;
	}
	return 0;
}


//更新登录奖励领取标志
void cProUserMissionAward::updateLoginAwardStatus(const unsigned int& u_userId,bool isLoginAwarded)
{
	char buf[128] = {0};

	sprintf(buf,"update m_gameUserInfo set isLoginAwarded = %d where userID = %d;",isLoginAwarded,u_userId);
	string strSql(buf);

	if(g_cMagicServerDb.execSQL(strSql)!=0)
	{
		g_cMyGlog.errorlogRecord("updateLoginAwardStatus failure,strSql:%s\n",strSql.c_str());
	}
}

//更新用户体力
void cProUserMissionAward::updateUserEnergy(const unsigned int& u_userId,const int& u_userEnergy)
{
	char buf[128] = {0};

	sprintf(buf,"update m_gameUserInfo set energy = %d where userID = %d;",u_userEnergy,u_userId);
	string strSql(buf);

	if(g_cMagicServerDb.execSQL(strSql)!=0)
	{
		g_cMyGlog.errorlogRecord("updateUserEnergy failure,strSql:%s\n",strSql.c_str());
	}
}

//获取用户体力和最大体力值
bool cProUserMissionAward::getUserEnergyAndMaxEnergy(const unsigned int& u_userId,int& u_userEnergy,int& u_maxEnergy)
{
	char buf[128] = {0};
	sprintf(buf,"select energy,userLevel from m_gameUserInfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserEnergy m_gameUserInfo not find userId,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_userEnergy = q.getIntFileds("energy");
	u_maxEnergy = USER_MAX_ENERGY(q.getIntFileds("userLevel"));//(ENERGY + q.getIntFileds("userLevel") - 1);
	q.finalize();
	return true;
}

bool cProUserMissionAward::getAwardTypeAndAwardNumByLogMissionId(const unsigned int& u_missionId,string& u_hortationType,int& u_hortationValue)
{
	char buf[256] = {0};
	sprintf(buf,"select HortationType,HortationValue from missioninfo where Id=%d;",u_missionId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getAwardTypeAndAwardNumByLogMissionId error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_hortationType = q.getStringFileds("HortationType");
	u_hortationValue = q.getIntFileds("HortationValue");
	q.finalize();
	return true;
}

//根据任务ID获取对应的奖励
int cProUserMissionAward::getAwardByMissionId(const unsigned int& u_userId,const unsigned int& u_missionId)
{
	int u_result = -1;
	string u_hortationType = "";
	int u_hortationValue = 0;
	if (!getAwardTypeAndAwardNumByLogMissionId(u_missionId,u_hortationType,u_hortationValue))
	{
		return u_result;
	}
	int u_hortationTypeNum = atoi(u_hortationType.c_str());
	if (u_hortationTypeNum == AWARD_MONEY)
	{
		unsigned int u_userGold = 0;
		u_userGold = g_cProcUserCardLevel.getUserGold(u_userId);
		u_userGold += u_hortationValue;
		if (g_cProcUserCardLevel.updateUserGold(u_userId,u_userGold))
		{
			u_result = 0;
		}
		return u_result;
	}
	else if (u_hortationTypeNum == FRIEND_VALUE_AWARD)
	{
		unsigned int u_userFriendValue = 0;
		if (g_cProcUserAwardFriend.getUserFriendValue(u_userId,u_userFriendValue))
		{
			if (u_userFriendValue < MAX_FRIEND_VALUE)
			{
				u_userFriendValue += u_hortationValue;
				if (u_userFriendValue > MAX_FRIEND_VALUE)
				{
					u_userFriendValue = MAX_FRIEND_VALUE;
				}
				if (g_cProcUserAwardFriend.updataUserFriendValue(u_userId,u_userFriendValue))
				{
					u_result = 0;
				}
			}
			else
			{
				u_result = K_TO_MAX_FRIEND_VALUE;
			}
		}
		return u_result;
	}
	else if (u_hortationTypeNum == AWARD_ENERGY)
	{
		/*int u_uu_hortationTypeNum == AWARD_ENERGYserEnergyPotionCount = 0;
		if (getUserEnergyPotionCount(u_userId,u_userEnergyPotionCount))
		{
			u_userEnergyPotionCount += 1;
			updateUserEnergyPotionCount(u_userId,u_userEnergyPotionCount);
		}
		u_result = 0;*/
		int u_energy = 0;
		int u_maxEnergy = 0;
		if (getUserEnergyAndMaxEnergy(u_userId,u_energy,u_maxEnergy))
		{
			u_energy = u_maxEnergy;
			updateUserEnergy(u_userId,u_energy);
			u_result = 0;
		}
		return u_result;
	}
	else if (u_hortationTypeNum == AWARD_MAGIC_STON)
	{
		unsigned int u_userDiamond = 0;
		if (g_cProcUserDiamondAward.getUserDiamond(u_userId,u_userDiamond))
		{
			u_userDiamond += u_hortationValue;
			g_cProcUserDiamondAward.updataUserDiamondNumber(u_userId,u_userDiamond);
			u_result = 0;
		}
		return u_result;
	}
	else if (u_hortationTypeNum > 2000)
	{
		unsigned int u_cardId = u_hortationTypeNum;
		unsigned int u_itemId = 0;
		if (g_cProcUserBattle.isCardCountToMax(u_userId))
		{
			u_result = K_BACKPACK_HAS_FULL;
		}
		else
		{
			while (u_hortationValue > 0)
			{
				if (g_cProcUserAwardFriend.insertCardInfo(u_userId,u_cardId,u_itemId))
				{
					u_result = 0;
				}
				u_hortationValue -= 1;
				u_itemId = 0;
			}
		}
	}
	return u_result;
}

//获取用户体力药水个数
bool cProUserMissionAward::getUserEnergyPotionCount(const unsigned int& u_userId,int& u_energyPotionCount)
{
	char buf[128] = {0};
	sprintf(buf,"select energyCount from m_gameuserinfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserEnergyPotionCount error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_energyPotionCount = q.getIntFileds("energyCount");
	q.finalize();
	return true;
}

//更新体力药水个数
void cProUserMissionAward::updateUserEnergyPotionCount(const unsigned int& u_userId,const int& u_energyPotionCount)
{
	char buf[128] = {0};
	sprintf(buf,"update m_gameuserinfo set energyCount = %d where userID = %d;",u_energyPotionCount,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUserEnergyPotionCount error,strSql:%s\n",strSql.c_str());
	}
}

//处理登陆任务奖励
int cProUserMissionAward::procLoginMissionAward(const unsigned int& u_userId,const unsigned int& u_missionId)
{
	int u_result = 0;
	u_result = isLoginMissionShouldAward(u_userId,u_missionId);
	if (u_result != 0)
	{
		g_cMyGlog.errorlogRecord("can not award ID:%d\n",u_missionId);
		return u_result;
	}
	u_result = getAwardByMissionId(u_userId,u_missionId);
	if (u_result == 0)
	{
		updateLoginAwardStatus(u_userId,true);
	}
	return u_result;
}

//判断该任务是否应该有奖励
bool cProUserMissionAward::isShouldAward(const unsigned int& u_userId,\
				   const unsigned int& u_missionId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_userMissionsInfo%d where userId = %d and \
				isMissinFinished = 1 and missionId = %d;",u_userId%USER_MISSION_TABLE_COUNT,\
				u_userId,u_missionId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("userId:%d,missionId:%d cannot award\n",u_userId,u_missionId);
		g_cMyGlog.errorlogRecord("strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	q.finalize();
	return true;
}

//获取用户金币数
bool cProUserMissionAward::getUserGoldNumber(const unsigned int& u_userId,unsigned int& u_golderNumber)
{
	char buf[128] = {0};
	sprintf(buf,"select goldNumber from m_gameUserInfo where userID = %d",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserGoldNumber not find userId,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	else
	{
		u_golderNumber = q.getIntFileds("goldNumber");
	}
	q.finalize();
	return true;
}

//更新用户金币数
bool cProUserMissionAward::updateUserGoldNumber(const unsigned int& u_userId,\
												const unsigned int& u_awardGoldNumber)
{
	char buf[128] = {0};
	unsigned int u_totalGoldNumber = 0;
	if (!getUserGoldNumber(u_userId,u_totalGoldNumber))
	{
		return false;
	}
	u_totalGoldNumber += u_awardGoldNumber;
	sprintf(buf,"update m_gameUserInfo set goldNumber = %d \
				where userID = %d",u_totalGoldNumber,u_userId);
	string strSql(buf);
	if(g_cMagicServerDb.execSQL(strSql)!=0)
	{
		g_cMyGlog.errorlogRecord("update goldNumber failure,strSql:%s\n",strSql.c_str());
		return false;
	}
	return true;
}

//更新登录任务状态
bool cProUserMissionAward::updateUserMissionStatus(const unsigned int& u_userId,\
							 const unsigned int& u_missionId)
{
	char buf[128] = {0};
	sprintf(buf,"update m_userMissionsInfo%d set isMissinFinished = 0 where \
				userId = %d and isMissinFinished = 1 and missionId = %d;",\
				u_userId%USER_MISSION_TABLE_COUNT,u_userId,u_missionId);
	string strSql(buf);
	if(g_cMagicServerDb.execSQL(strSql)!=0)
	{
		g_cMyGlog.errorlogRecord("update MissionStatus failure,strSql:%s\n",strSql.c_str());
		return false;
	}
	return true;
}

//删除任务
bool cProUserMissionAward::deleteUserMissionId(const unsigned int& u_userId,\
											   const unsigned int& u_missionId)
{
	if (u_missionId > 7)
	{
		char buf[128] = {0};
		sprintf(buf,"delete from m_userMissionsInfo%d where userId = %d and missionId = %d limit 1;",\
			u_userId%USER_MISSION_TABLE_COUNT,u_userId,u_missionId);
		string strSql(buf);
		if (g_cMagicServerDb.execSQL(strSql) != 0)
		{
			g_cMyGlog.errorlogRecord("deleteUserMissionId error,strSql:%s\n",strSql.c_str());
			return false;
		}
	}
	else
	{
		g_cMyGlog.errorlogRecord("unkown missionID,u_missionId:%d\n",u_missionId);
	}
	return true;
}
//获取奖励
int cProUserMissionAward::getMissionAward(const unsigned int& u_userId,const string& u_hortationType,const int& u_hortationCount)
{
	int u_result = 0;
	if (u_hortationType.size() < 3 && u_hortationType.size() > 0)
	{
		int u_Type = atoi(u_hortationType.c_str());
		if (u_Type == AWARD_MONEY)
		{
			updateUserGoldNumber(u_userId,u_hortationCount);
		}
		else if (u_Type == FRIEND_VALUE_AWARD)
		{
			unsigned int u_friendValue = 0;
			g_cProcUserAwardFriend.getUserFriendValue(u_userId,u_friendValue);
			if (u_friendValue >= MAX_FRIEND_VALUE)
			{
				return K_TO_MAX_FRIEND_VALUE;
			}
			else
			{
				u_friendValue += u_hortationCount;
				if (u_friendValue > K_TO_MAX_FRIEND_VALUE)
				{
					u_friendValue = MAX_FRIEND_VALUE;
				}
				g_cProcUserAwardFriend.updataUserFriendValue(u_userId,u_friendValue);
			}
		}
		else if (u_Type == AWARD_MAGIC_STON)
		{
			unsigned int u_userDiamond = 0;
			if (g_cProcUserDiamondAward.getUserDiamond(u_userId,u_userDiamond))
			{
				u_userDiamond += u_hortationCount;
				g_cProcUserDiamondAward.updataUserDiamondNumber(u_userId,u_userDiamond);
			}
		}
		else if (u_Type == AWARD_ENERGY)
		{
			/*int u_userEnergyPotionCount = 0;
			if (getUserEnergyPotionCount(u_userId,u_userEnergyPotionCount))
			{
				u_userEnergyPotionCount += 1;
				if (u_userEnergyPotionCount > 1)
				{
					u_userEnergyPotionCount = 1;
				}
				updateUserEnergyPotionCount(u_userId,u_userEnergyPotionCount);
			}*/
			int u_userEnergy = 0,u_maxEnergy = 0;
			getUserEnergyAndMaxEnergy(u_userId,u_userEnergy,u_maxEnergy);
			u_userEnergy = u_maxEnergy;
			updateUserEnergy(u_userId,u_userEnergy);

		}
	}
	else if (u_hortationType.size() == 3)
	{
		unsigned int u_itemId = 0;
		unsigned int u_cardId = atoi(u_hortationType.c_str());
		if (g_cProcUserBattle.isCardCountToMax(u_userId))
		{
			return K_BACKPACK_HAS_FULL;
		}
		else
		{
			for (int i = 0; i < u_hortationCount; i++)
			{
				g_cProcUserAwardFriend.insertCardInfo(u_userId,u_cardId,u_itemId);
			}
		}
	}
	else if (u_hortationType.size() == 4)
	{
		unsigned int u_cardId = atoi(u_hortationType.c_str());
		if (u_cardId >= 2000)
		{
			if (g_cProcUserBattle.isCardCountToMax(u_userId))
			{
				return K_BACKPACK_HAS_FULL;
			}
			else
			{
				for (int i = 0; i < u_hortationCount; i++)
				{
					unsigned int u_itemId = 0;
					g_cProcUserAwardFriend.insertCardInfo(u_userId,u_cardId,u_itemId);
				}
			}
		}
		else
		{
			addUserFragmentInfo(u_userId,1,u_hortationType,u_hortationCount);
		}
	}
	else
	{
		g_cMyGlog.errorlogRecord("unkonw hortationType:%s\n",u_hortationType.c_str());
		u_result = -1;
	}
	return u_result;
}

//获取奖励及删除任务
int cProUserMissionAward::getAwardAndDeleteMission(const unsigned int& u_userId,const unsigned int& u_missionId,\
							 const int& u_missionType,const string& u_hortationType,const int& u_hortationCount)
{
	int u_result = 0;
	u_result = getMissionAward(u_userId,u_hortationType,u_hortationCount);
	if (u_result != 0)
	{
		return u_result;
	}
	if(u_missionType == BATTLE_MISSION || u_missionType == LEVEL_UP_MISSION || u_missionType == PAY_MISSION \
		|| u_missionType == ACTIVE_BATTLE_MISSION)
	{
		deleteUserMissionId(u_userId,u_missionId);
	}
	return u_result;
}

//处理非登陆任务奖励
int cProUserMissionAward::procNotLoginMissionAward(const unsigned int& u_userId,const unsigned int& u_missionId)
{
	int u_result = 0;
	char buf[512] = {0};
	sprintf(buf,"select * from m_userMissionsInfo%d where userId = %d and missionId = %d limit 1;",\
		u_userId%USER_MISSION_TABLE_COUNT,u_userId,u_missionId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("procNotLoginMissionAward get nothing,strSql:%s\n",strSql.c_str());
		q.finalize();
		return -1;
	}
	int u_isFinished = q.getIntFileds("isFinished");
	if (u_isFinished == 0)
	{
		q.finalize();
		return K_MISSION_NOT_FINISHED;
	}
	int u_missionType = q.getIntFileds("missinType");
	string u_hortationType = q.getStringFileds("hortationType");
	int u_hortationCount = q.getIntFileds("hortationCount");
	u_result = getAwardAndDeleteMission(u_userId,u_missionId,u_missionType,u_hortationType,u_hortationCount);
	q.finalize();
	return u_result;
}
//根据任务Id给用户对应奖励
int cProUserMissionAward::updateUserInfoByMissionId(const unsigned int& u_userId,\
													 const unsigned int& u_missionId)
{

	int n_result = -1;
	if (u_missionId >= FIRST_LOGIN_MISSION_ID && u_missionId <= SEVENTH_LOGIN_MISSION_ID)
	{
		n_result = procLoginMissionAward(u_userId,u_missionId);
	}
	else if (u_missionId > SEVENTH_LOGIN_MISSION_ID)
	{
		n_result = procNotLoginMissionAward(u_userId,u_missionId);
	}
	return n_result;
}

//更新体力和最新获取体力的时间戳
bool cProUserMissionAward::updateUserEnergyAndTime(const unsigned int& u_energy,const unsigned int& u_lastEnergyTime,const unsigned int& u_userId)
{
	char buf[128] = {0};

	sprintf(buf,"update m_gameUserInfo set energy = %d,lastEnergyTime=%d where userID = %d;",u_energy,u_lastEnergyTime,u_userId);
	string strSql(buf);

	if(g_cMagicServerDb.execSQL(strSql)!=0)
	{
		g_cMyGlog.errorlogRecord("updateUserEnergyAndTime failure,strSql:%s\n",strSql.c_str());
		return false;
	}
	return true;
}

//获取用户信息
bool cProUserMissionAward::getUserInfo(PBUser* u_PBUser,const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_gameUserInfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("m_gameUserInfo not find userId,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	else
	{
		u_PBUser->set_userid(q.getIntFileds("userID"));
		u_PBUser->set_username(q.getStringFileds("userName"));
		u_PBUser->set_userlevel(q.getIntFileds("userLevel"));
		u_PBUser->set_userexp(q.getIntFileds("userExp"));

		int u_energy = q.getIntFileds("energy");
		int u_lastEnergyTime = q.getIntFileds("lastEnergyTime");
		int u_userLevel = q.getIntFileds("userLevel");

		/*int u_energyPotionCount = q.getIntFileds("energyCount");
		if (u_energyPotionCount >= 1)
		{
			u_energy = USER_MAX_ENERGY(u_userLevel);//ENERGY + (u_userLevel-1)/2;
		}*/
		int u_nowTimeStamp = 0;
		u_nowTimeStamp = GetTimeStamp();
		int u_NextEnergyTime = g_cProcUserUseEnergy.getNextEnergyTime(u_nowTimeStamp,u_lastEnergyTime,u_energy,u_userLevel);

		unsigned int u_mEnergy = u_energy;
		unsigned int u_mLastEnergyTime = u_lastEnergyTime;

		updateUserEnergyAndTime(u_mEnergy,u_mLastEnergyTime,u_userId);
		u_PBUser->set_energy(u_energy);
		//暂时为0
		u_PBUser->set_nextenergytime(u_NextEnergyTime);
		//u_PBUser->set_buyenergytime(q.getIntFileds("buyEnergyTime"));
		newAddSetSomeUserInfo(u_PBUser,u_userId);
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

//插入活动副本任务
bool cProUserMissionAward::insertActiveBattleMissionAward(const unsigned int& u_userId,const string& u_activeBattleId)
{
	char buf[512] = {0};
	sprintf(buf,"select * from missioninfo where ConditionValue='%s';",u_activeBattleId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("insertActiveBattleMissionAward error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	while(!q.eof())
	{
		memset(buf,0,sizeof(buf));
		int nMissionId = q.getIntFileds("Id");
		int nMissionType = q.getIntFileds("ConditionType");
		string u_missionCondition = q.getStringFileds("ConditionValue");
		string u_hortationType = q.getStringFileds("HortationType");
		int u_hortationCount = q.getIntFileds("HortationValue");
		//bool isFinished = false;

		sprintf(buf,"insert into m_userMissionsInfo%d(userId,missionId,missinType,missionCondition,hortationType,hortationCount,isFinished) \
					values(%d,%d,%d,'%s','%s',%d,1)",u_userId%USER_MISSION_TABLE_COUNT,u_userId,\
					nMissionId,nMissionType,u_missionCondition.c_str(),u_hortationType.c_str(),u_hortationCount);
		string strSqlTmp(buf);
		unsigned int tmpId = 0;
		if(g_cMagicServerDb.execSQL(strSqlTmp)!=0)
		{
			g_cMyGlog.errorlogRecord("insertActiveBattleMissionAward insert Missions failure: userID:%d,MissionId:%d\n",\
				u_userId,nMissionId);
			q.finalize();
			return false;
		}
		q.nextRow();
	}
	q.finalize();
	return true;
}

//处理用户任务奖励请求
int cProUserMissionAward::procUserMissionAward(PBMissionAwardReceive& u_PBMissionAwardReceive,const string& u_strToken,\
												const PBMissionAwardSend& u_PBMissionAwardSend)
{
	unsigned int u_userId = 0;
	int u_result = -1;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("procUserMissionAward getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	int n_missionCount = u_PBMissionAwardSend.missionid_size();
	for (int i = 0; i < n_missionCount; i++)
	{
		u_result = updateUserInfoByMissionId(u_userId,u_PBMissionAwardSend.missionid(i));
		if (u_result != 0)
		{
			g_cMyGlog.errorlogRecord("updateUserInfoByMissionId error,u_userId:%d,missionId:%d\n",u_userId,u_PBMissionAwardSend.missionid(i));
			return u_result;
		}
	}
	PBUser* u_PBUser;
	u_PBUser = u_PBMissionAwardReceive.mutable_user();
	getUserInfo(u_PBUser,u_userId);

	PBAllCard* u_PBAllCard;
	u_PBAllCard = u_PBMissionAwardReceive.mutable_cards();
	g_cProcUserLogin.getAllCardsInfo(u_PBAllCard,u_userId);

	PBAllGem* u_PBAllGem;
	u_PBAllGem = u_PBMissionAwardReceive.mutable_gems();
	g_cProcUserLogin.getUserGemInfo(u_PBAllGem,u_userId);
	return u_result;
}