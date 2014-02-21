#ifndef PROCUSERMISSIONAWARD_H
#define PROCUSERMISSIONAWARD_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProUserMissionAward
{
public:
	cProUserMissionAward();
	~cProUserMissionAward();

	//判断该任务是否应该有奖励
	bool isShouldAward(const unsigned int& u_userId,\
		const unsigned int& u_missionId);

	//获取用户金币数
	bool getUserGoldNumber(const unsigned int& u_userId,unsigned int& u_golderNumber);

	//更新用户金币数
	bool updateUserGoldNumber(const unsigned int& u_userId,\
		const unsigned int& u_awardGoldNumber);

	//更新登录任务状态
	bool updateUserMissionStatus(const unsigned int& u_userId,\
		const unsigned int& u_missionId);

	//删除任务
	bool deleteUserMissionId(const unsigned int& u_userId,\
		const unsigned int& u_missionId);

	//获取奖励及删除任务
	int getAwardAndDeleteMission(const unsigned int& u_userId,const unsigned int& u_missionId,\
		const int& u_missionType,const string& u_hortationType,const int& u_hortationCount);
	//处理非登陆任务奖励
	int procNotLoginMissionAward(const unsigned int& u_userId,const unsigned int& u_missionId);
	//获取奖励
	int getMissionAward(const unsigned int& u_userId,const string& u_hortationType,const int& u_hortationCount);
	//根据任务Id给用户对应奖励
	int updateUserInfoByMissionId(const unsigned int& u_userId,\
		const unsigned int& u_missionId);

	//更新体力和最新获取体力的时间戳
	bool updateUserEnergyAndTime(const unsigned int& u_energy,const unsigned int& u_lastEnergyTime,const unsigned int& u_userId);
	//获取用户信息
	bool getUserInfo(PBUser* u_PBUser,const unsigned int& u_userId);

	//处理用户任务奖励请求
	int procUserMissionAward(PBMissionAwardReceive& u_PBMissionAwardReceive,const string& u_strToken,\
		const PBMissionAwardSend& u_PBMissionAwardSend);

	//获取连续登陆天数
	int getLoginDays(const unsigned int& u_userId,int& u_isLoginAwarded);
	//该登陆任务是否应该被奖励
	int isLoginMissionShouldAward(const unsigned int& u_userId,const unsigned int& u_missionId);
	//更新登录奖励领取标志
	void updateLoginAwardStatus(const unsigned int& u_userId,bool isLoginAwarded);
	//更新用户体力
	void updateUserEnergy(const unsigned int& u_userId,const int& u_userEnergy);
	//获取用户体力和最大体力值
	bool getUserEnergyAndMaxEnergy(const unsigned int& u_userId,int& u_userEnergy,int& u_maxEnergy);
	//根据任务ID获取对应的奖励
	int getAwardByMissionId(const unsigned int& u_userId,const unsigned int& u_missionId);
	//获取用户体力药水个数
	bool getUserEnergyPotionCount(const unsigned int& u_userId,int& u_energyPotionCount);
	//更新体力药水个数
	void updateUserEnergyPotionCount(const unsigned int& u_userId,const int& u_energyPotionCount);
	//处理登陆任务奖励
	int procLoginMissionAward(const unsigned int& u_userId,const unsigned int& u_missionId);

	bool getAwardTypeAndAwardNumByLogMissionId(const unsigned int& u_missionId,string& u_hortationType,int& u_hortationValue);

	bool insertActiveBattleMissionAward(const unsigned int& u_userId,const string& u_activeBattleId);
protected:
private:
};

extern cProUserMissionAward g_cProUserMissionAward;
#endif