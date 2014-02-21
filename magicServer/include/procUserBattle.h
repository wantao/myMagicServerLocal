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
	//获取用户当前体力
	bool getUserEnergy(const unsigned int& u_userId,unsigned int& u_energy);
	void setPBUser(PBUser* u_PBUser,const unsigned int& u_userId,int u_userEnergy,int u_usedEnergy);
	int calUserBattleExp(const int& u_battleSumCount,const int& u_monsterTotalCount,\
		const int& u_monsterTotalLevel);

	//获取玩家最大能拥有的卡牌数
	bool getUserMaxOwnCardCount(const unsigned int& u_userId,int& u_maxOwnCardCount);

	//获取玩家现在拥有的卡牌数
	bool getUserOwnCardCount(const unsigned int& u_userId,int& u_OwnCardCount);

	//玩家拥有的卡牌是否达到上限
	bool isCardCountToMax(const unsigned int& u_userId);
	//更新用户副本进度
	void updateUserBattleProgress(const unsigned int& u_userId,const string& u_battleId);
	//获取玩家当前副本进度
	bool getUserBattleProgress(const unsigned int& u_userId,string& u_battleId);
	//解析副本Id 3-10
	int analysisBattleId(char* u_battleIdBuf,int position);
	//判断副本进度是否应该被更新
	bool isShouldUpdateBattleProgress(const string& u_newBattleId,const string& u_BattleId);
	//判断副本关卡是否存在
	bool isBattleExist(const string& u_strBattleId,int& u_userLevelLimit,int& u_needEnergy);
	//更新用户战斗状态
	void updateUserFightStatus(const unsigned int& u_userId,bool u_fightStatus);
	//获取用户等级
	bool getUserLevel(const unsigned int& u_userId,int& u_userLevel);
	//从002-003-004中将卡牌分割出来
	void splitCardIdList(const string& u_strCardIdList,const string& u_splitFlag,splitCardIdListMap& u_splitCardIdListMap);
	//随机战斗怪物
	int srandCardPostion(splitCardIdListMap& u_splitCardIdListMap,string& u_strCardId);
	//判断是否为活动副本，1000-1 暂定为u_battleId.size() >= 6,即为活动副本
	bool isActiveBattle(const string& u_battleId);
	//得到通关活动副本的数目
	void getFinishedActiveBattleCount(const unsigned int& u_userId,const string& u_battleId,int& u_count);
	//处理进入活动副本的情况
	int procActiveBattle(const unsigned int& u_userId,const string& u_battleId);
	//副本是否为指定日期的副本
	bool isSetActiveBattle(const string& u_battleId,const unsigned int& u_userId);
	//获取额外奖励
	int getExtraAward(const string& u_battleId,int& u_awardType);
	//存储额外奖励
	void saveTmpExtraAward(const unsigned int& u_userId,const int& u_goldNum,const int& u_exp,\
		const int& u_awardType,const int& u_awardThing);
	int setPBBattleReceive(PBBattleReceive& u_PBBattleReceive,const string& u_strToken,PBBattleSend& u_PBBattleSend);
	//获取活动副本经验
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