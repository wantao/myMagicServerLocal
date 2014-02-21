#ifndef PROCUSERBATTLE_H
#define PROCUSERBATTLE_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

typedef map<int,string> splitCardIdListMap;
class cProcUserBattle
{
public:
	cProcUserBattle();
	~cProcUserBattle();
	bool getLevelCount(const string& u_battleId,int& u_levelCount,int& u_monsterTotalLevel,int& u_battleSumCount);
	bool getMonsterCount(const string& u_levelId,int& u_monsterCount);
	bool getGoldLose(const string& u_monsterId,int& u_goldLose,int& u_goldLoseNumber);
	bool getEquipLose(const string& u_monsterId,int u_equipType,int& u_equipLose,string& u_quipId);
	bool getCardLose(const string& u_monsterId,int& u_cardLose,string& u_cardId,int& u_cardLevel);
	bool getBigCardIdLose(const string& u_monsterId,int& u_cardLose,string& u_cardId,int& u_cardLevel);
	bool randGold(const string& u_monsterId,int& u_goldLoseNumber);
	bool randEquip(const string& u_monsterId,int& u_equipType,string& u_equipId);
	bool randCard(const string& u_monsterId,string& u_cardId,int& u_cardLevel);
	bool isExistEquip(const unsigned int& u_userId,unsigned int u_equipType,
		string u_equipId,int& u_equipCount);
	bool randBigCardId(const string& u_monsterId,string& u_cardId,int& u_cardLevel);
	bool updateTmpEquipInfo(const unsigned int& u_userId,unsigned int u_equipType,
		string u_equipId,int& u_equipCount);
	bool saveTmpEquipInfo(const unsigned int& u_userId,unsigned int u_equipType,
		string u_equipId);
	bool saveTmpCardInfo(const unsigned int& u_userId,string u_cardId,int u_cardLevel);
	bool deleteTmpGoldInfo(const unsigned int& u_userId);
	bool saveTmpGoldInfo(const unsigned int& u_userId,int u_totalGoldNumber,int u_userBattleExp);
	bool saveTmpGoldInfo(const unsigned int& u_userId,int u_totalGoldNumber,int u_userBattleExp,\
		bool isInsertDiamond,int u_diamondNum,bool isInsertEnergy,int u_energyCount);
	void proxySetPBBattleReceive(PBBattleAward* u_PBBattleAward,const string& u_monsterId,\
		const int& u_Type,const int& u_Number);
	void updatUserEnergy(const unsigned int& u_userId,const unsigned int& u_userEnergy);
	//��ȡ�û���ǰ����
	bool getUserEnergy(const unsigned int& u_userId,unsigned int& u_energy);
	void setPBUser(PBUser* u_PBUser,const unsigned int& u_userId,int u_userEnergy,int u_usedEnergy);
	int calUserBattleExp(const int& u_battleSumCount,const int& u_monsterTotalCount,\
		const int& u_monsterTotalLevel);

	//��ȡ��������ӵ�еĿ�����
	bool getUserMaxOwnCardCount(const unsigned int& u_userId,int& u_maxOwnCardCount);

	//��ȡ�������ӵ�еĿ�����
	bool getUserOwnCardCount(const unsigned int& u_userId,int& u_OwnCardCount);

	//���ӵ�еĿ����Ƿ�ﵽ����
	bool isCardCountToMax(const unsigned int& u_userId);
	//�����û���������
	void updateUserBattleProgress(const unsigned int& u_userId,const string& u_battleId);
	//��ȡ��ҵ�ǰ��������
	bool getUserBattleProgress(const unsigned int& u_userId,string& u_battleId);
	//��������Id 3-10
	int analysisBattleId(char* u_battleIdBuf,int position);
	//�жϸ��������Ƿ�Ӧ�ñ�����
	bool isShouldUpdateBattleProgress(const string& u_newBattleId,const string& u_BattleId);
	//�жϸ����ؿ��Ƿ����
	bool isBattleExist(const string& u_strBattleId,int& u_userLevelLimit,int& u_needEnergy);
	//�����û�ս��״̬
	void updateUserFightStatus(const unsigned int& u_userId,bool u_fightStatus);
	//��ȡ�û��ȼ�
	bool getUserLevel(const unsigned int& u_userId,int& u_userLevel);
	//��002-003-004�н����Ʒָ����
	void splitCardIdList(const string& u_strCardIdList,const string& u_splitFlag,splitCardIdListMap& u_splitCardIdListMap);
	//���ս������
	int srandCardPostion(splitCardIdListMap& u_splitCardIdListMap,string& u_strCardId);
	//�ж��Ƿ�Ϊ�������1000-1 �ݶ�Ϊu_battleId.size() >= 6,��Ϊ�����
	bool isActiveBattle(const string& u_battleId);
	//�õ�ͨ�ػ��������Ŀ
	void getFinishedActiveBattleCount(const unsigned int& u_userId,const string& u_battleId,int& u_count);
	//����������������
	int procActiveBattle(const unsigned int& u_userId,const string& u_battleId);
	//�����Ƿ�Ϊָ�����ڵĸ���
	bool isSetActiveBattle(const string& u_battleId,const unsigned int& u_userId);
	//��ȡ���⽱��
	int getExtraAward(const string& u_battleId,int& u_awardType);
	//�洢���⽱��
	void saveTmpExtraAward(const unsigned int& u_userId,const int& u_goldNum,const int& u_exp,\
		const int& u_awardType,const int& u_awardThing);
	int setPBBattleReceive(PBBattleReceive& u_PBBattleReceive,const string& u_strToken,PBBattleSend& u_PBBattleSend);
	//��ȡ���������
	int getActiveBattleExp(const string& u_activeBattleId);


	bool getCardLoseInfoByKey(const string& u_monsterId,int& u_cardLose,string& u_cardId,int& u_cardLevel);
	bool getEquipLoseInfoByKey(const string& u_monsterId,int& u_equipType,int& u_equipLose,string& u_quipId);
	bool getGoldLoseInfoByKey(const string& u_monsterId,int& u_goldLose,int& u_goldLoseNumber);
	bool getBigCardIdLoseInfoByKey(const string& u_monsterId,int& u_cardLose,string& u_cardId,int& u_cardLevel);
	
protected:
private:
};

extern cProcUserBattle g_cProcUserBattle;

#endif