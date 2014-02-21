#ifndef PROCUSERALLMISSIONS_H
#define PROCUSERALLMISSIONS_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserAllMissions
{
public:
	cProcUserAllMissions();
	~cProcUserAllMissions();
	//得到用户所有信息
	bool getUserAllMissionsInfo(PBAllMission& u_PBAllMission,const unsigned int& u_userID);
	//根据任务类型判断任务是否存在
	bool isMissionExist(const unsigned int& u_userId,const int& u_missionType,const string& u_missionCondition,int& isFinished);
	//获取充值任务条件列表
	string getPayMissionConditionList(const unsigned int& u_userId,const string& u_payMissionCondition);
	//根据任务类型更新任务完成状态
	void updateMissionStatus(const unsigned int& u_userId,const int& u_missionType,const string& u_missionCondition,bool isFinished);
	//用户获取任务处理
	int procUserAllMission(PBAllMission& u_PBAllMission,const string& u_strToken);
protected:
private:
};

extern cProcUserAllMissions g_cProcUserAllMissions;
#endif