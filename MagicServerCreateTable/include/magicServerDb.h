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

	////////////得到PBSystemInfoReceive所需的东西/////begin
	//填充PBSystemInfoReceive
	void setPBSystemInfoReceive(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//获取公告信息
	void PBSystemInfoReceive_setAfficheInfo(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//获取服务器所有分区信息
	void PBSystemInfoReceive_setServerInfo(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//获取游戏版本信息
	void PBSystemInfoReceive_setGameVersion(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//获取资源版本信息
	void PBSystemInfoReceive_setResourceVersion(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//获取副本版本信息
	void PBSystemInfoReceive_setBattleVersion(PBSystemInfoReceive& u_PBSystemInfoReceive);
	////////////得到PBSystemInfoReceive所需的东西/////end
	

	//token值和用id插入表中
	bool InsertUserTokenInfo(string& userToken,int& userId);
	//根据userToken查找对应的用户ID
	int getUserIdByUserToken(string& userToken);

	bool dropNoUseCard(const int& u_cardId);

	//将初始数据插入到cardinfo
	void insertToCardInfo(MysqlQuery& q);
	//按星级插入卡牌信息 
	void insertIntoCardStarTable(MysqlQuery& q,const string& u_strTableName);
	void createStarTable(const string& u_strStarTable);
	//按星级初始化卡牌信息
	void initCardInfoByCardStar();
	//初始化cardinfo表
	void initCardInfoTable();
	//创建初始卡牌信息表
	bool createInitCardInfo();

	//得到关卡怪物数
	unsigned int getInitLevelMonsterCount(const string& u_LevelId);
	//获取副本关卡数；
	unsigned int getInitBattleLevelCount(const string& u_battleId);

	
	//创建副本信息表
	bool createInitBattleInfo();
	//插入初试副本信息
	void insertInitBattleInfo(const string& u_battleId,const unsigned int& u_levelCount,\
		const unsigned int& u_energyUse,const unsigned int& u_monsterTotal,\
		const unsigned int& u_monsterAverageLevel,const int& u_monsterTatalLevel);

	unsigned int getBattleCount(const string& u_battleId,const unsigned int& u_maxId);
	//获取大副本下的小副本数
	int getBattleSummulation(const string& u_battleId);
	//更新副本累加数目
	void updateBattleSummulation(const string& u_battleTmpId,const int& u_battleSumCount);
	//获取副本数目
	unsigned int getInitBattleCount(const string& u_battleId);
	//创建副本关卡怪物数量表
	bool createInitBattleLevelInfo();
	//插入初始关卡信息
	void insertInitBattleLevelInfo(const string& u_levelId,const unsigned int& u_monsterCount);

	//副本单一怪物金币信息掉落表
	bool createInitMonsterGoldLoseInfo();
	//初始化怪物金币掉落
	void InitMonsterGoldLoseInfo(const string& u_monsterId);
	//插入怪物金币掉落
	void insertMonsterGoldLoseInfo(MysqlQuery& q,const string& u_monsterId);

	//副本单一怪物装备信息掉落表
	bool createInitEquipLoseInfo();
	//初始化怪物装备掉落
	void InitMonsterEquipLoseInfo(const string& u_monsterId,const unsigned int& u_nType);
	//插入怪物装备掉落
	void insertMonsterEquipLoseInfo(MysqlQuery& q,const string& u_monsterId,const unsigned int& u_nType);
	
	//初始化怪物卡牌掉落
	void InitMonsterCardLoseInfo(const string& u_monsterId,int& u_cardLevel);
	//插入怪物卡牌掉落
	void insertMonsterCardLoseInfo(MysqlQuery& q,const string& u_monsterId);
	//副本单一怪物卡牌信息掉落表
	bool createInitCardLoseInfo();
	

	//初始化怪物卡牌掉落
	void InitMonsterBigCardIdLoseInfo(const string& u_monsterId,int& u_cardLevel);
	//插入怪物卡牌掉落
	void insertMonsterBigCardIdLoseInfo(MysqlQuery& q,const string& u_monsterId);

	////副本单一怪物卡牌Id大于2000信息掉落表
	bool createInitBigCardIdLoseInfo();

	//创建100个m_userCardInfo表
	bool createUserCardInfo();
	//创建20个m_userEquipInfo
	bool createUserEuipInfo();
	//插入任务过程
	void insertMissionProccess(const int& u_preMaxMissionId);
	//插入任务
	void insertMissions(const unsigned int& u_userId,const int& u_preMaxMissionId);
	bool isUserMissionExist(const unsigned int& u_userId,const int& u_missionId);
	void deleteUserMissionId(const unsigned int& u_userId,const int& u_missionId);
	//创建20个m_userMissionsInfo
	bool createUserMissionInfo();
	//创建20个m_userFriendInfo
	bool createUserFriendInfo();
	//创建20个m_userFriendReqInfo
	bool userCreateUserFriendReqInfo();
	//创建100m_EquipTmpSaveInfo表
	bool creatEquipTmpSaveInfo();
	//创建100个m_CardTmpSaveInfo表
	bool createCardTmpSaveInfo();
	//创建金币掉落临时存储表
	bool createGoldTmpSaveInfo();
	//用户活动副本表
	void createUserActiveBattleInfo(const int& u_tableCount);

	//修改用户card表字段
	void modifyUserCardTableField(int i);
	//创建邀请码表
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
	//为m_userFriendInfo%d添加使用好友时间戳useFriendStamp
	void addFieldForM_userFriendInfo();
	

	//清空碎片信息
	void cleanUserFragmentInfo(const unsigned int& u_userId,const int& u_type,\
		const string& u_strId);
	//插入碎片信息
	void insertUserFragment(const unsigned int& u_userId,const int& u_type,\
		const string& u_strId,const int& u_count);
	//合并相同的碎片
	void mergeSameIdFragmentByUserIdAndFragmentId(const unsigned int& u_userId,const string& u_fragmentId);
	//合并相同的碎片
	void mergeSameIdFragmentByUserId(const unsigned int& n_userId);
	//合并碎片信息
	void mergeSameIdFragment();
	//获取需要修改的任务Id
	MysqlQuery getModifiedMissionId();
	//插入任务Id
	void insertUserMissionId(const unsigned int& u_userId,const int& u_missionId);
	//修改用户任务信息
	void modifyUserMissionInfo();
	//获取需要新增加的任务Id
	MysqlQuery getNewAddMissionId();
	//增加用户任务
	void AddUserMissionInfo();
	///为玩家卡牌表将字段VTLGrow,STRGrow,TGHGrow,\
	QUIGrow,MGCGrow的属性由tinyint改为smallint
	void modifyUserCardInfoFieldAttribute();
	///创建记录玩家当日已扫荡的次数和已购买体力药水次数的表（20个）
	void createTableForRecordUserWipeOutAndBuyEneryCount();
	void updateUserEnergyByUserID(const int& u_userId,const int& u_userMaxEnergy);
	void updateUserEnergy();

protected:
private:
	//数据库连接开关
	bool m_bConnect;
};
extern cMagicServerDb g_cMagicServerDb;
extern cMagicServerDb g_cMagicServerDbRead;
#endif



