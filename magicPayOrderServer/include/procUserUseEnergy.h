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
	//�õ��û���ǰ��������ʯ�������������������ϴλָ�������ʱ���
	bool getUserEnergyAndDiamond(const unsigned int& u_userId,int& u_energy,int& u_diamondNumber,\
		int& u_buyEnergyTime,int& u_lastEnergyTime,int& u_maxEnergy);
	//���¹�����������û���Ϣ
	bool updateUserEnergy(const unsigned int& u_userId,int& u_energy,int& u_diamondNumber,\
		int& u_buyEnergyTime,int& u_lastEnergyTime);
	//�Ƿ�����ɵ�����
	bool hasMissionFinished(const unsigned int& u_userId);
	//����û���Ϣ
	bool setPBUser(PBUser* u_PBUser,const unsigned int& u_userId,\
		const unsigned int& u_nextEnergyTime);
	//��ȡ�û��ȼ�������
	bool getUserLevelAndEnergyPotionCount(const unsigned int& u_userId,int& u_userLevel,\
		int& u_lastEnergyTime,int& u_energyPotionCount,int& u_maxEnergy);
	//�����û�����������ҩˮ����
	void updateUserEnergyAndEnergyPotion(const unsigned int& u_userId,const int& u_energy,\
		const int& u_lastEnergyTime,const int& u_maxEnergy,const int& u_energyPotionCount);
	//��ȡ�����ָ�����ʱ
	int getNextEnergyTime(const int& u_nowStamp,int& u_lastEnergyTimeStamp,\
		int& u_energy,const int& u_userLevel);
	//��乺����������ķ��ؽ��
	int setPBUseEnergyReceive(PBUseEnergyReceive& u_PBUseEnergyReceive,const string& u_strToken);
	bool getNextEnergyTimeNeedInfo(const unsigned int& u_userId,int& u_lastEnergyTime,int& u_energy,int& u_userLevel);
	int getUserEnergy(PBEnergyReceive& u_PBEnergyReceive,const string& u_strToken);
protected:
private:
};

extern cProcUserUseEnergy g_cProcUserUseEnergy;

#endif