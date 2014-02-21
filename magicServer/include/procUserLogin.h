#ifndef PROCUSERLOGIN_H
#define PROCUSERLOGIN_H

#include <iostream>
#include <list>
#include "../include/MagicCard.pb.h"
using namespace std;


typedef struct _LoginMissinInfo 
{
	//������¼һ���Ӧ������ID���Ƿ�������������
	int missionId1;
	bool missionFinished1;
	string missionProgress1;

	//������¼�����Ӧ������ID���Ƿ�������������
	int missionId2;
	bool missionFinished2;
	string missionProgress2;

	//������¼�����Ӧ������ID���Ƿ�������������
	int missionId3;
	bool missionFinished3;
	string missionProgress3;

	//������¼�����Ӧ������ID���Ƿ�������������
	int missionId4;
	bool missionFinished4;
	string missionProgress4;

	//������¼�����Ӧ������ID���Ƿ�������������
	int missionId5;
	bool missionFinished5;
	string missionProgress5;

	//������¼�����Ӧ������ID���Ƿ�������������
	int missionId6;
	bool missionFinished6;
	string missionProgress6;

	//������¼�����Ӧ������ID���Ƿ�������������
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
		//��ȡ�������
		bool getMaxEnergy(const int& u_userId,unsigned int& u_MaxEnergy);
		//��ȡ��һ�������ָ���ʱ�䣨s��
		//�������ǰ��������ǰ�����ָ�ʱ���
		unsigned int getNextEnergyTime(const unsigned int& u_userId,unsigned int& energy,unsigned int& lastEnergyTimeStamp);
		//�����û���¼ʱ��
		bool updateUserLogtime(unsigned int u_LogTimeStamp,const unsigned int& u_userId);
		//ͨ��mac��ַ��ȡ�û���Ϣ
		bool getUserInfoByMacAddress(PBUser* u_PBUser,const PBLoginSend& u_PBLoginSend,string& u_battleProgress,unsigned int& u_userID);
		//����ʱ��ȷ�����������ڼ���ʱ���ʽ2013 08 19
		int getWeekDayByTime(const string& u_strTime);
		//���ݽ��������ڼ���������
		void getBattleIdListByWeekDay(const int& u_weekDay,weekDayMissionList& u_weekDayMissionList);
		//��ȡ�������Ϣ
		bool getActiveBattleInfo(PBBattle* u_PBBattle,const unsigned int& u_userId);
		//��ȡ�û����п�����Ϣ
		bool getAllCardsInfo(PBAllCard* u_PBAllCard,const unsigned int& u_userID);
		//��ȡ�û�����������Ϣ
		bool getUserAllMissionsInfo(PBAllMission* u_PBAllMission,const unsigned int& u_userID);
		//��ȡ�û�ս��������Ϣ
		bool getUserBattleCardInfo(PBBattleCard* u_PBBattleCard,const unsigned int& u_userID);
		//��ȡ�û�װ����Ϣ
		void getUserGemInfo(PBAllGem* u_PBAllGem,const unsigned int& u_userID);
		//��ȡ��¼���� 1,�ʺŵ�¼��2��mac��ַ��¼
		int getLoginType(const PBLoginSend& u_PBLoginSend);
		//�����һ�εĵ�¼ʱ���������¼����
		bool getPreLoginTimeAndDays(const unsigned int& u_userId,string& u_loginTime,int& u_loginDays);
		//����������¼����
		bool setLoginDays(const string& u_strPreLoginTime,const unsigned long& u_nowStamp,int& u_LoginDays);
		//���µ�¼ʱ�䣬������¼����������״̬
		void updateLoginDaysAndAwardStatus(const unsigned int& u_userId,const string& u_strLoginTime,\
			const int& u_logDays,bool isAwarded);
		//���õ�¼���������
		void setLoginAward(const unsigned int& u_userId);
		//����������¼��������ʼ����¼������Ϣ
		bool getLoginMissionInfo(const int& u_logDays,LoginMissinInfo& u_LoginMissinInfo,const int& isAwarded);
		//��ȡ���������¼����
		bool getLoginDays(const unsigned int& u_userId,int& u_logDays,int& isAwarded);
		//����¼������Ϣ
		void fillPBAllMission(PBAllMission* u_PBAllMission,const LoginMissinInfo& u_LoginMissinInfo);
		//�����¼����
		bool procLoginMissions(PBAllMission* u_PBAllMission,const unsigned int& u_userId,bool& isLogMissionFinished);
		//��ȡ�ǵ�½������Ϣ
		void getNotLoginMissions(PBAllMission* u_PBAllMission,const unsigned int& u_userId);
		//�û���¼����
		int procUserLogin(PBLoginReceive& u_PBLoginReceive,string& strProtobufData);
		//�������������»�ȡ������ʱ���
		//bool updateUserEnergyAndTime(const unsigned int& u_energy,const unsigned int& u_lastEnergyTime,const string& strMac);
protected:
private:
};

extern cProcUserLogin g_cProcUserLogin;
#endif