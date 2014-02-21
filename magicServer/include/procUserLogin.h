#ifndef PROCUSERLOGIN_H
#define PROCUSERLOGIN_H

#include <iostream>
#include <list>
#include "../include/MagicCard.pb.h"
using namespace std;


typedef struct _LoginMissinInfo 
{
	//连续登录一天对应的任务ID，是否完成情况，进度
	int missionId1;
	bool missionFinished1;
	string missionProgress1;

	//连续登录两天对应的任务ID，是否完成情况，进度
	int missionId2;
	bool missionFinished2;
	string missionProgress2;

	//连续登录三天对应的任务ID，是否完成情况，进度
	int missionId3;
	bool missionFinished3;
	string missionProgress3;

	//连续登录四天对应的任务ID，是否完成情况，进度
	int missionId4;
	bool missionFinished4;
	string missionProgress4;

	//连续登录五天对应的任务ID，是否完成情况，进度
	int missionId5;
	bool missionFinished5;
	string missionProgress5;

	//连续登录六天对应的任务ID，是否完成情况，进度
	int missionId6;
	bool missionFinished6;
	string missionProgress6;

	//连续登录七天对应的任务ID，是否完成情况，进度
	int missionId7;
	bool missionFinished7;
	string missionProgress7;

	_LoginMissinInfo()
	{
		missionId1 = 0;
		missionFinished1 = false;
		missionProgress1 = "";

		missionId2 = 0;
		missionFinished2 = false;
		missionProgress2 = "";

		missionId3 = 0;
		missionFinished3 = false;
		missionProgress3 = "";

		missionId4 = 0;
		missionFinished4 = false;
		missionProgress4 = "";

		missionId5 = 0;
		missionFinished5 = false;
		missionProgress5 = "";

		missionId6 = 0;
		missionFinished6 = false;
		missionProgress6 = "";

		missionId7 = 0;
		missionFinished7 = false;
		missionProgress7 = "";
	}

}LoginMissinInfo;

typedef list<string> weekDayMissionList;

class cProcUserLogin
{
public:
		cProcUserLogin();
		~cProcUserLogin();
		//获取最大体力
		bool getMaxEnergy(const int& u_userId,unsigned int& u_MaxEnergy);
		//获取下一点体力恢复的时间（s）
		//输出：当前体力，当前体力恢复时间戳
		unsigned int getNextEnergyTime(const unsigned int& u_userId,unsigned int& energy,unsigned int& lastEnergyTimeStamp);
		//更新用户登录时间
		bool updateUserLogtime(unsigned int u_LogTimeStamp,const unsigned int& u_userId);
		//通过mac地址获取用户信息
		bool getUserInfoByMacAddress(PBUser* u_PBUser,const PBLoginSend& u_PBLoginSend,string& u_battleProgress,unsigned int& u_userID);
		//根据时间确定今天是星期几，时间格式2013 08 19
		int getWeekDayByTime(const string& u_strTime);
		//根据今天是星期几分配任务
		void getBattleIdListByWeekDay(const int& u_weekDay,weekDayMissionList& u_weekDayMissionList);
		//获取活动副本信息
		bool getActiveBattleInfo(PBBattle* u_PBBattle,const unsigned int& u_userId);
		//获取用户所有卡牌信息
		bool getAllCardsInfo(PBAllCard* u_PBAllCard,const unsigned int& u_userID);
		//获取用户所有任务信息
		bool getUserAllMissionsInfo(PBAllMission* u_PBAllMission,const unsigned int& u_userID);
		//获取用户战斗卡牌信息
		bool getUserBattleCardInfo(PBBattleCard* u_PBBattleCard,const unsigned int& u_userID);
		//获取用户装备信息
		void getUserGemInfo(PBAllGem* u_PBAllGem,const unsigned int& u_userID);
		//获取登录类型 1,帐号登录，2，mac地址登录
		int getLoginType(const PBLoginSend& u_PBLoginSend);
		//获得上一次的登录时间和连续登录天数
		bool getPreLoginTimeAndDays(const unsigned int& u_userId,string& u_loginTime,int& u_loginDays);
		//设置连续登录天数
		bool setLoginDays(const string& u_strPreLoginTime,const unsigned long& u_nowStamp,int& u_LoginDays);
		//更新登录时间，连续登录天数，奖励状态
		void updateLoginDaysAndAwardStatus(const unsigned int& u_userId,const string& u_strLoginTime,\
			const int& u_logDays,bool isAwarded);
		//设置登录任务奖励相关
		void setLoginAward(const unsigned int& u_userId);
		//根据连续登录天数，初始化登录任务信息
		bool getLoginMissionInfo(const int& u_logDays,LoginMissinInfo& u_LoginMissinInfo,const int& isAwarded);
		//获取玩家连续登录天数
		bool getLoginDays(const unsigned int& u_userId,int& u_logDays,int& isAwarded);
		//填充登录任务信息
		void fillPBAllMission(PBAllMission* u_PBAllMission,const LoginMissinInfo& u_LoginMissinInfo);
		//处理登录任务
		bool procLoginMissions(PBAllMission* u_PBAllMission,const unsigned int& u_userId,bool& isLogMissionFinished);
		//获取非登陆任务信息
		void getNotLoginMissions(PBAllMission* u_PBAllMission,const unsigned int& u_userId);
		//用户登录处理
		int procUserLogin(PBLoginReceive& u_PBLoginReceive,string& strProtobufData);
		//更新体力和最新获取体力的时间戳
		//bool updateUserEnergyAndTime(const unsigned int& u_energy,const unsigned int& u_lastEnergyTime,const string& strMac);
protected:
private:
};

extern cProcUserLogin g_cProcUserLogin;
#endif