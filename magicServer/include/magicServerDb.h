#ifndef MAGICSERVERDB_H
#define MAGICSERVERDB_H
#include <iostream>
#include "common.h"
#include "MysqlQuery.h"
#include "MysqlDB.h"
#include "../include/MagicCard.pb.h"
using namespace std;

class cMagicServerDb:public MysqlDB
{
public:
	cMagicServerDb();
	~cMagicServerDb();
	bool Init();
	//�����ݿ�ʵ����
	bool DbReadInit();
	void unInit();

	////////////�õ�PBSystemInfoReceive����Ķ���/////begin
	//���PBSystemInfoReceive
	void setPBSystemInfoReceive(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//��ȡ������Ϣ
	void PBSystemInfoReceive_setAfficheInfo(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//��ȡ���������з�����Ϣ
	void PBSystemInfoReceive_setServerInfo(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//��ȡ��Ϸ�汾��Ϣ
	void PBSystemInfoReceive_setGameVersion(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//��ȡ��Դ�汾��Ϣ
	void PBSystemInfoReceive_setResourceVersion(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//��ȡ�����汾��Ϣ
	void PBSystemInfoReceive_setBattleVersion(PBSystemInfoReceive& u_PBSystemInfoReceive);
	////////////�õ�PBSystemInfoReceive����Ķ���/////end
	

	////////////////////////ע�ᴦ�� �õ�PBLoginReceive�������Ϣ/////begin

	//�ж��Ƿ����������
	bool hasMissionFinished(const unsigned int& u_userId);
	//�������û�
	int PBLoginReceive_gernerateNewUser(PBUser* u_PBUser,const PBRegisterSend& u_PBRegisterSend,unsigned int& newUserId,const int& u_regType);
	//�ж�ע���û��ʺ��Ƿ��Ѿ�����
	bool isRegisterUserAccountExist(const string& u_strUserAccount,const int& u_plateform);
	//�ж�mac��ַ��ַע����û��Ƿ��Ѿ�����
	bool isRegisterUserMacExist(const string& u_strUserMac,const int& u_plateform);
	//�ж��û��ǳ��Ƿ����
	bool isRegisterUserNameExist(const string& u_strUserName);
	//�����û�MacAddress��UserName�����µ��û���Ϣ 
	//0:ע��ɹ���1����MacAddress�Ѿ�ע�ᣬ2��ע��ʧ��
	int PBLoginReceive_gernerateUserByMacAndUserName(PBUser* u_PBUser,const PBRegisterSend& u_PBRegisterSend,unsigned int& newUserId);
	//��ȡ�������Ϣ
	bool PBLoginReceive_getActiveBattleInfo(PBBattle* u_PBBattle,const unsigned int& u_userId);

	
	//�����ʼ������Ϣ
	bool insertInitCardInfo(PBCard* u_PBCard,const int& u_CardId,unsigned int& nNewUserId,MysqlQuery& q,int battlePosition);
	//��ȡ��ʼ������Ϣ
	bool getInitCardInfoByCardId(PBCard* u_PBCard,const int& u_CardId,unsigned int& nNewUserId,int battlePosition);
	//����ְҵ��Ż�ȡ�ӳ�����ID
	string getCapitorCardByProfession(const int& u_playerProfessionIndex);
	//���PBAllCard
	int PBLoginReceive_setAndGetUserCardInfo(PBAllCard* u_PBAllCard,unsigned int& nNewUserId,const int& u_profession);

	//���PBAllMission
	void PBLoginReceive_setMissionsInfo(PBAllMission* u_PBAllMission,unsigned int& nNewUserId);

	//���ս��������Ϣ
	void PBLoginReceive_setBattleCardInfo(PBBattleCard* u_PBBattleCard,unsigned int& nNewUserId);

	bool isExistToken(string& strToken);

	bool isExistUserId(unsigned int& nNewUserId);

	bool insertUserIdToken(string& strToken,unsigned int& nNewUserId);

	bool updateUserToken(string& strToken,unsigned int& nNewUserId);

	void PBLoginReceive_setToken(string& strTokenRet,unsigned int& nNewUserId);

	////////////////////////ע�ᴦ�� �õ�PBLoginReceive�������Ϣ/////end


	///////////////////��¼���� �õ�PBLoginReceive�������Ϣ/////begin
	////////////////////��¼���� �õ�PBLoginReceive�������Ϣ/////end

	//��������Ϣ
	void PBLoginReceive_setFriendInfo(PBAllFriend& u_PBAllFriend,unsigned int& nNewUserId);


	//����userToken���Ҷ�Ӧ���û�ID
	int getUserIdByUserToken(string& userToken);
	//tokenֵ����id�������
	bool InsertUserTokenInfo(string& userToken,int& userId);

	//���ݿ����ӱ���
	void mysqlKeepAlive(int u_Type);
	//ɾ���û�
	void deletUserByUserId(int u_userId);
	//�ж��Ƿ���Ҫ��������
	bool isSeverExecuteDataUpdate(const string& u_serverIp);
	//��ֹ����ͬ���ӳ���ɵĴӴӿ�ȡ�������ݵ����
	MysqlQuery master_slave_exQuery(const string& strSql);
	
	
protected:
private:
	//���ݿ����ӿ���
	bool m_bConnect;
};
extern cMagicServerDb g_cMagicServerDb;
extern cMagicServerDb g_cMagicServerDbRead;
#endif



