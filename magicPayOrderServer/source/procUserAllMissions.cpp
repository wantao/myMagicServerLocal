#include "../include/procUserAllMissions.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/procUserLogin.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"

cProcUserAllMissions g_cProcUserAllMissions;

cProcUserAllMissions::cProcUserAllMissions()
{

}
cProcUserAllMissions::~cProcUserAllMissions()
{

}

//得到用户所有信息
bool cProcUserAllMissions::getUserAllMissionsInfo(PBAllMission& u_PBAllMission,const unsigned int& u_userID)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_userMissionsInfo%d where userId = %d;",u_userID%USER_MISSION_TABLE_COUNT,u_userID);
	string strSql(buf);

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("getUserAllMissionsInfo error:strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	else
	{
		while (!q.eof())
		{
			PBMission* u_PBMission;
			u_PBMission = u_PBAllMission.add_allmission();
			u_PBMission->set_missionid(q.getIntFileds("missionId"));
			int u_isMissionFinished = q.getIntFileds("isMissinFinished");
			u_PBMission->set_ismissinfinished(u_isMissionFinished);
			if (!u_isMissionFinished)
			{
				u_PBMission->set_missonprogress(q.getStringFileds("missionProgress"));
			}
			q.nextRow();
		}
	}
	q.finalize();
	return true;
}

//根据任务类型判断任务是否存在
bool cProcUserAllMissions::isMissionExist(const unsigned int& u_userId,const int& u_missionType,const string& u_missionCondition,int& isFinished)
{
	char buf[256] = {0};
	if (u_missionType == BATTLE_MISSION || u_missionType == LEVEL_UP_MISSION \
		|| u_missionType == ACTIVE_BATTLE_MISSION)
	{
		sprintf(buf,"select isFinished from m_userMissionsInfo%d where userId = %d and missinType = %d and missionCondition = '%s';",\
			u_userId%USER_MISSION_TABLE_COUNT,u_userId,u_missionType,u_missionCondition.c_str());
	}
	else if (u_missionType == PAY_MISSION)
	{
		sprintf(buf,"select isFinished from m_userMissionsInfo%d where userId = %d and missinType = %d and missionCondition in ('%s');",\
			u_userId%USER_MISSION_TABLE_COUNT,u_userId,u_missionType,u_missionCondition.c_str());
	}
	else
	{
		g_cMyGlog.debugLog("unkown  missionType:%d\n",u_missionType);
		return false;
	}
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("isMissionExist get nothing:strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	isFinished = q.getIntFileds("isFinished");
	q.finalize();
	return true;
}


//获取充值任务条件列表
string cProcUserAllMissions::getPayMissionConditionList(const unsigned int& u_userId,const string& u_payMissionCondition)
{
	string u_retList = "";
	int u_condition = atoi(u_payMissionCondition.c_str());
	char buf[64] = {0};
	if (u_condition == M_1_YUAN_CONDITION)
	{
		sprintf(buf,"%d",u_condition);
	}
	else if (u_condition == M_50_YUAN_CONDITION)
	{
		sprintf(buf,"%d,%d",M_1_YUAN_CONDITION,u_condition);
	}
	else if (u_condition == M_100_YUAN_CONDITION)
	{
		sprintf(buf,"%d,%d,%d",M_1_YUAN_CONDITION,M_50_YUAN_CONDITION,u_condition);
	}
	else if (u_condition == M_500_YUAN_CONDITION)
	{
		sprintf(buf,"%d,%d,%d,%d",M_1_YUAN_CONDITION,M_50_YUAN_CONDITION,M_100_YUAN_CONDITION,u_condition);
	}
	else if (u_condition == M_1500_YUAN_CONDITION)
	{
		sprintf(buf,"%d,%d,%d,%d,%d",M_1_YUAN_CONDITION,M_50_YUAN_CONDITION,\
			M_100_YUAN_CONDITION,M_500_YUAN_CONDITION,u_condition);
	}
	else if (u_condition == M_3000_YUAN_CONDITION)
	{
		sprintf(buf,"%d,%d,%d,%d,%d,%d",M_1_YUAN_CONDITION,M_50_YUAN_CONDITION,\
			M_100_YUAN_CONDITION,M_500_YUAN_CONDITION,M_1500_YUAN_CONDITION,u_condition);
	}
	else
	{
		g_cMyGlog.errorlogRecord("unkown payCondition::u_condition:%d\n",u_condition);
		return u_retList;
	}
	u_retList = buf;
	return u_retList;
}

//根据任务类型更新任务完成状态
void cProcUserAllMissions::updateMissionStatus(const unsigned int& u_userId,const int& u_missionType,const string& u_missionCondition,bool isFinished)
{
	char buf[256] = {0};
	if (u_missionType == BATTLE_MISSION || u_missionType == LEVEL_UP_MISSION)
	{
		sprintf(buf,"update m_userMissionsInfo%d set isFinished = %d where userId = %d and missinType = %d and missionCondition = '%s';",\
			u_userId%USER_MISSION_TABLE_COUNT,isFinished,u_userId,u_missionType,u_missionCondition.c_str());
	}
	else if (u_missionType == PAY_MISSION)
	{
		sprintf(buf,"update m_userMissionsInfo%d set isFinished = %d where userId = %d and missinType = %d and missionCondition in ('%s');",\
				u_userId%USER_MISSION_TABLE_COUNT,isFinished,u_userId,u_missionType,u_missionCondition.c_str());
	}
	else
	{
		g_cMyGlog.errorlogRecord("unkown  missionType:%d\n",u_missionType);
		return;
	}
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateMissionStatus error,strSql:%s\n",strSql.c_str());
	}
} 


//用户获取任务处理
int cProcUserAllMissions::procUserAllMission(PBAllMission& u_PBAllMission,const string& u_strToken)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("procUserAllMission getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	bool isLogMissionFinished = false;
	g_cProcUserLogin.procLoginMissions(&u_PBAllMission,u_userId,isLogMissionFinished);
	g_cProcUserLogin.getNotLoginMissions(&u_PBAllMission,u_userId);
	/*if (!getUserAllMissionsInfo(u_PBAllMission,u_userId))
	{
		cerr<<"procUserAllMission getUserAllMissionsInfo"<<endl;
		return false;
	}*/
	return u_result;
}
