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
	bool DbReadInit();
	bool Init();
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
	

	//tokenֵ����id�������
	bool InsertUserTokenInfo(string& userToken,int& userId);
	//����userToken���Ҷ�Ӧ���û�ID
	int getUserIdByUserToken(string& userToken);

	bool dropNoUseCard(const int& u_cardId);

	//����ʼ���ݲ��뵽cardinfo
	void insertToCardInfo(MysqlQuery& q);
	//���Ǽ����뿨����Ϣ 
	void insertIntoCardStarTable(MysqlQuery& q,const string& u_strTableName);
	void createStarTable(const string& u_strStarTable);
	//���Ǽ���ʼ��������Ϣ
	void initCardInfoByCardStar();
	//��ʼ��cardinfo��
	void initCardInfoTable();
	//������ʼ������Ϣ��
	bool createInitCardInfo();

	//�õ��ؿ�������
	unsigned int getInitLevelMonsterCount(const string& u_LevelId);
	//��ȡ�����ؿ�����
	unsigned int getInitBattleLevelCount(const string& u_battleId);

	
	//����������Ϣ��
	bool createInitBattleInfo();
	//������Ը�����Ϣ
	void insertInitBattleInfo(const string& u_battleId,const unsigned int& u_levelCount,\
		const unsigned int& u_energyUse,const unsigned int& u_monsterTotal,\
		const unsigned int& u_monsterAverageLevel,const int& u_monsterTatalLevel);

	unsigned int getBattleCount(const string& u_battleId,const unsigned int& u_maxId);
	//��ȡ�󸱱��µ�С������
	int getBattleSummulation(const string& u_battleId);
	//���¸����ۼ���Ŀ
	void updateBattleSummulation(const string& u_battleTmpId,const int& u_battleSumCount);
	//��ȡ������Ŀ
	unsigned int getInitBattleCount(const string& u_battleId);
	//���������ؿ�����������
	bool createInitBattleLevelInfo();
	//�����ʼ�ؿ���Ϣ
	void insertInitBattleLevelInfo(const string& u_levelId,const unsigned int& u_monsterCount);

	//������һ��������Ϣ�����
	bool createInitMonsterGoldLoseInfo();
	//��ʼ�������ҵ���
	void InitMonsterGoldLoseInfo(const string& u_monsterId);
	//��������ҵ���
	void insertMonsterGoldLoseInfo(MysqlQuery& q,const string& u_monsterId);

	//������һ����װ����Ϣ�����
	bool createInitEquipLoseInfo();
	//��ʼ������װ������
	void InitMonsterEquipLoseInfo(const string& u_monsterId,const unsigned int& u_nType);
	//�������װ������
	void insertMonsterEquipLoseInfo(MysqlQuery& q,const string& u_monsterId,const unsigned int& u_nType);
	
	//��ʼ�����￨�Ƶ���
	void InitMonsterCardLoseInfo(const string& u_monsterId,int& u_cardLevel);
	//������￨�Ƶ���
	void insertMonsterCardLoseInfo(MysqlQuery& q,const string& u_monsterId);
	//������һ���￨����Ϣ�����
	bool createInitCardLoseInfo();
	

	//��ʼ�����￨�Ƶ���
	void InitMonsterBigCardIdLoseInfo(const string& u_monsterId,int& u_cardLevel);
	//������￨�Ƶ���
	void insertMonsterBigCardIdLoseInfo(MysqlQuery& q,const string& u_monsterId);

	////������һ���￨��Id����2000��Ϣ�����
	bool createInitBigCardIdLoseInfo();

	//����100��m_userCardInfo��
	bool createUserCardInfo();
	//����20��m_userEquipInfo
	bool createUserEuipInfo();
	//�����������
	void insertMissionProccess(const int& u_preMaxMissionId);
	//��������
	void insertMissions(const unsigned int& u_userId,const int& u_preMaxMissionId);
	bool isUserMissionExist(const unsigned int& u_userId,const int& u_missionId);
	void deleteUserMissionId(const unsigned int& u_userId,const int& u_missionId);
	//����20��m_userMissionsInfo
	bool createUserMissionInfo();
	//����20��m_userFriendInfo
	bool createUserFriendInfo();
	//����20��m_userFriendReqInfo
	bool userCreateUserFriendReqInfo();
	//����100m_EquipTmpSaveInfo��
	bool creatEquipTmpSaveInfo();
	//����100��m_CardTmpSaveInfo��
	bool createCardTmpSaveInfo();
	//������ҵ�����ʱ�洢��
	bool createGoldTmpSaveInfo();
	//�û��������
	void createUserActiveBattleInfo(const int& u_tableCount);

	//�޸��û�card���ֶ�
	void modifyUserCardTableField(int i);
	//�����������
	void createInvitationCodeTable();
	void insertInvitationCodeToTable();
	void insertInvitationCodeToTable(int u_count);
	void printHasNoCardUser();

	void clearUserCardinfo(const int& u_userId);
	void clearUserEquipInfo(const int& u_userId);
	void clearUserinfo(const int& u_userId);
	void clearDataBase();
	void clearUserFriendInfo(const int& u_userId);
	void clearUserFriendReqInfo(const int& u_userId);
	void clearUserMissionInfo(const int& u_userId);
	void clearUserTokenInfo(const int& u_userId);

	bool isCardIdExist(const string& u_strCardId);
	void deleteUserCard(const unsigned int& u_userId,const string& u_strCardId);
	void clearUserNotExistCard(const unsigned int& u_userId);
	void clearUserNotExistCard();

	void updataUserLeaderValue(const unsigned int& u_userId,const int& u_userLevel);
	void updataUserLeaderValue();
	void alterUserMissionsTableIndex();

	void mysqlPressureTest();


	bool createChapterInfo();
	bool insertIntoChapterInfo(MysqlQuery& q);
	void insertChapterInfo();
	void updateUserCapitorCapacityClass();


	void CreateSelectAndInsertThread();
	void mysql_proxy_test();
	void mysql_master_slave_test();
	//Ϊm_userFriendInfo%d���ʹ�ú���ʱ���useFriendStamp
	void addFieldForM_userFriendInfo();
	

	//�����Ƭ��Ϣ
	void cleanUserFragmentInfo(const unsigned int& u_userId,const int& u_type,\
		const string& u_strId);
	//������Ƭ��Ϣ
	void insertUserFragment(const unsigned int& u_userId,const int& u_type,\
		const string& u_strId,const int& u_count);
	//�ϲ���ͬ����Ƭ
	void mergeSameIdFragmentByUserIdAndFragmentId(const unsigned int& u_userId,const string& u_fragmentId);
	//�ϲ���ͬ����Ƭ
	void mergeSameIdFragmentByUserId(const unsigned int& n_userId);
	//�ϲ���Ƭ��Ϣ
	void mergeSameIdFragment();
	//��ȡ��Ҫ�޸ĵ�����Id
	MysqlQuery getModifiedMissionId();
	//��������Id
	void insertUserMissionId(const unsigned int& u_userId,const int& u_missionId);
	//�޸��û�������Ϣ
	void modifyUserMissionInfo();
	//��ȡ��Ҫ�����ӵ�����Id
	MysqlQuery getNewAddMissionId();
	//�����û�����
	void AddUserMissionInfo();
	///Ϊ��ҿ��Ʊ��ֶ�VTLGrow,STRGrow,TGHGrow,\
	QUIGrow,MGCGrow��������tinyint��Ϊsmallint
	void modifyUserCardInfoFieldAttribute();
	///������¼��ҵ�����ɨ���Ĵ������ѹ�������ҩˮ�����ı�20����
	void createTableForRecordUserWipeOutAndBuyEneryCount();
	void updateUserEnergyByUserID(const int& u_userId,const int& u_userMaxEnergy);
	void updateUserEnergy();

protected:
private:
	//���ݿ����ӿ���
	bool m_bConnect;
};
extern cMagicServerDb g_cMagicServerDb;
extern cMagicServerDb g_cMagicServerDbRead;
#endif



