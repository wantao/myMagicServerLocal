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

	//�жϸ������Ƿ�Ӧ���н���
	bool isShouldAward(const unsigned int& u_userId,\
		const unsigned int& u_missionId);

	//��ȡ�û������
	bool getUserGoldNumber(const unsigned int& u_userId,unsigned int& u_golderNumber);

	//�����û������
	bool updateUserGoldNumber(const unsigned int& u_userId,\
		const unsigned int& u_awardGoldNumber);

	//���µ�¼����״̬
	bool updateUserMissionStatus(const unsigned int& u_userId,\
		const unsigned int& u_missionId);

	//ɾ������
	bool deleteUserMissionId(const unsigned int& u_userId,\
		const unsigned int& u_missionId);

	//��ȡ������ɾ������
	int getAwardAndDeleteMission(const unsigned int& u_userId,const unsigned int& u_missionId,\
		const int& u_missionType,const string& u_hortationType,const int& u_hortationCount);
	//����ǵ�½������
	int procNotLoginMissionAward(const unsigned int& u_userId,const unsigned int& u_missionId);
	//��ȡ����
	int getMissionAward(const unsigned int& u_userId,const string& u_hortationType,const int& u_hortationCount);
	//��������Id���û���Ӧ����
	int updateUserInfoByMissionId(const unsigned int& u_userId,\
		const unsigned int& u_missionId);

	//�������������»�ȡ������ʱ���
	bool updateUserEnergyAndTime(const unsigned int& u_energy,const unsigned int& u_lastEnergyTime,const unsigned int& u_userId);
	//��ȡ�û���Ϣ
	bool getUserInfo(PBUser* u_PBUser,const unsigned int& u_userId);

	//�����û�����������
	int procUserMissionAward(PBMissionAwardReceive& u_PBMissionAwardReceive,const string& u_strToken,\
		const PBMissionAwardSend& u_PBMissionAwardSend);

	//��ȡ������½����
	int getLoginDays(const unsigned int& u_userId,int& u_isLoginAwarded);
	//�õ�½�����Ƿ�Ӧ�ñ�����
	int isLoginMissionShouldAward(const unsigned int& u_userId,const unsigned int& u_missionId);
	//���µ�¼������ȡ��־
	void updateLoginAwardStatus(const unsigned int& u_userId,bool isLoginAwarded);
	//�����û�����
	void updateUserEnergy(const unsigned int& u_userId,const int& u_userEnergy);
	//��ȡ�û��������������ֵ
	bool getUserEnergyAndMaxEnergy(const unsigned int& u_userId,int& u_userEnergy,int& u_maxEnergy);
	//��������ID��ȡ��Ӧ�Ľ���
	int getAwardByMissionId(const unsigned int& u_userId,const unsigned int& u_missionId);
	//��ȡ�û�����ҩˮ����
	bool getUserEnergyPotionCount(const unsigned int& u_userId,int& u_energyPotionCount);
	//��������ҩˮ����
	void updateUserEnergyPotionCount(const unsigned int& u_userId,const int& u_energyPotionCount);
	//�����½������
	int procLoginMissionAward(const unsigned int& u_userId,const unsigned int& u_missionId);

	bool getAwardTypeAndAwardNumByLogMissionId(const unsigned int& u_missionId,string& u_hortationType,int& u_hortationValue);

	bool insertActiveBattleMissionAward(const unsigned int& u_userId,const string& u_activeBattleId);
protected:
private:
};

extern cProUserMissionAward g_cProUserMissionAward;
#endif