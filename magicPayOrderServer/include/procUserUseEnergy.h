#ifndef	PROCUSERBUYENERGY_H
#define PROCUSERBUYENERGY_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserUseEnergy
{
public:
	cProcUserUseEnergy();
	~cProcUserUseEnergy();
	//得到用户当前体力，钻石数，购买体力次数，上次恢复体力的时间戳
	bool getUserEnergyAndDiamond(const unsigned int& u_userId,int& u_energy,int& u_diamondNumber,\
		int& u_buyEnergyTime,int& u_lastEnergyTime,int& u_maxEnergy);
	//更新购买体力后的用户信息
	bool updateUserEnergy(const unsigned int& u_userId,int& u_energy,int& u_diamondNumber,\
		int& u_buyEnergyTime,int& u_lastEnergyTime);
	//是否有完成的任务
	bool hasMissionFinished(const unsigned int& u_userId);
	//填充用户信息
	bool setPBUser(PBUser* u_PBUser,const unsigned int& u_userId,\
		const unsigned int& u_nextEnergyTime);
	//获取用户等级和体力
	bool getUserLevelAndEnergyPotionCount(const unsigned int& u_userId,int& u_userLevel,\
		int& u_lastEnergyTime,int& u_energyPotionCount,int& u_maxEnergy);
	//更新用户体力和体力药水个数
	void updateUserEnergyAndEnergyPotion(const unsigned int& u_userId,const int& u_energy,\
		const int& u_lastEnergyTime,const int& u_maxEnergy,const int& u_energyPotionCount);
	//获取体力恢复倒计时
	int getNextEnergyTime(const int& u_nowStamp,int& u_lastEnergyTimeStamp,\
		int& u_energy,const int& u_userLevel);
	//填充购买体力请求的返回结果
	int setPBUseEnergyReceive(PBUseEnergyReceive& u_PBUseEnergyReceive,const string& u_strToken);
	bool getNextEnergyTimeNeedInfo(const unsigned int& u_userId,int& u_lastEnergyTime,int& u_energy,int& u_userLevel);
	int getUserEnergy(PBEnergyReceive& u_PBEnergyReceive,const string& u_strToken);
protected:
private:
};

extern cProcUserUseEnergy g_cProcUserUseEnergy;

#endif