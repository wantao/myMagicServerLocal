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
	//�õ��û�������Ϣ
	bool getUserAllMissionsInfo(PBAllMission& u_PBAllMission,const unsigned int& u_userID);
	//�������������ж������Ƿ����
	bool isMissionExist(const unsigned int& u_userId,const int& u_missionType,const string& u_missionCondition,int& isFinished);
	//��ȡ��ֵ���������б�
	string getPayMissionConditionList(const unsigned int& u_userId,const string& u_payMissionCondition);
	//�����������͸����������״̬
	void updateMissionStatus(const unsigned int& u_userId,const int& u_missionType,const string& u_missionCondition,bool isFinished);
	//�û���ȡ������
	int procUserAllMission(PBAllMission& u_PBAllMission,const string& u_strToken);
protected:
private:
};

extern cProcUserAllMissions g_cProcUserAllMissions;
#endif