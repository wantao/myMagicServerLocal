#ifndef PROCUSERBATTLERESULT
#define PROCUSERBATTLERESULT

#include <iostream>
#include "../include/MagicCard.pb.h"
#include "../include/magicServerDb.h"
using namespace std;

class cProUserBattleResult
{
public:
	cProUserBattleResult();
	~cProUserBattleResult();
	bool deleteGoldTmpInfo(const unsigned int& u_userId);
	bool getUserGoldAndExp(const unsigned int& u_userId,int& u_userGold,int& u_userExp,\
		int& u_userLevel,int& u_userTmpLevel,int& u_userEnergy,int& u_userLeaderValue,int& u_userMaxEnergy,\
		int& u_diamondNum,int& u_energyCount);
	bool updateUserGoldAndExp(const unsigned int& u_userId,int u_userGold,int u_userExp,\
		int u_userLevel,int u_userTmpLevel,int u_userEnergy,int u_userLeaderValue,int u_userMaxEnergy,\
		int u_diamNum,int u_energyCount);
	bool getTmpGold(const unsigned int& u_userId,int& u_goldNumber,int& u_tmpUserExp,\
		int& u_tmpDiamondNum,int& u_tmpEnergyCount);
	bool getTmpEquip(const unsigned int& u_userId);
	bool deleteEquipTmpInfo(const unsigned int& u_userId);
	bool insertCardInfo(const unsigned int& u_userId,MysqlQuery& q,const unsigned int& u_cardLevel);
	bool getFixCardInfoAndInsert(const unsigned int& u_userId,const string& u_cardId,const unsigned int& u_cardLevel);
	bool getTmpCardInfo(const unsigned int& u_userId);
	bool deleteCardTmpInfo(const unsigned int& u_userId);
	bool deleteTmpData(const unsigned int& u_userId);
	//ʹ�����ȼ���Ӧ�ļ��������Ȳ�ҪԽ��
	void makeCardExpNotOverFlow(const unsigned int& u_skillLevel,unsigned int& u_skillExp);
	bool updateSkillExp(const unsigned int& u_userId,const unsigned int& u_itemId,const int& u_mSkillExp);
	//�����������
	void addUserLevel(int& u_userExp,int& u_userLevel);
	bool getTmpData(const unsigned int& u_userId,bool& hasMissionFinished);
	//��ȡ�û�ս��״̬
	bool getUserFightStatus(const unsigned int& u_userId,int& u_fightStatus);
	//�жϸ����Ƿ����
	bool isBattleExist(const string& u_strBattleId);
	//���½�����������
	void updateEnterActiveBattleCount(const unsigned int& u_userId,const string& u_battleId,const int& u_count);
	int setPBBattleResultReceive(PBBattleResultReceive& u_PBBattleResultReceive,const string& u_strToken,PBBattleResultSend& u_PBBattleResultSend);
	void printUserAllCardInfo(const PBAllCard& u_PBAllCard);
	void printCardInfo(const PBCard& u_PBCard);

	//��ȡ���ѱ�����ս���Ĵ���
	int getUseFriendCount(const unsigned int& u_userId,const int& u_friendId);
	void updateUseFriendCount(const unsigned int& u_userId,const int& u_friendId,const int& u_useFriendCount);

	//���ؿ�����Ϣ�ڴ�
	bool loadCardInfoToMem();
	bool authCardDbInfo(const PBBattleResultSend& u_PBBattleResultSend);
	bool authCardDbInfo(const CardInfoAuth& u_CardInfoAuth,const PBCardDBInfo& u_PBCardDBInfo);

	//����ʹ�ú���ʱ���
	void updateUseFriendStamp(const unsigned int& u_userId,const int& u_friendId,const int& u_useFriendStamp);
protected:
private:
};

extern cProUserBattleResult g_cProUserBattleResult;
#endif