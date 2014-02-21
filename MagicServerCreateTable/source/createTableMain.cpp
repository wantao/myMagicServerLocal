#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "../include/common.h"
#include "../include/magicServerDb.h"

using namespace std;

#define MIN_ENERGY_USE	3
#define EQUIP_TYPE_COUNT 2

#define  USER_CARD_TABLE_COUNT 100

#define USER_ACTIVE_BATTLE_COUNT 20

void initBattleLoseInfo(int u_maxChapter)
{
	int i = 0,j = 0;
	if (u_maxChapter < 1000)
	{
		j = 1;
	}
	else
	{
		j = 1000;
	}
	for (i = j; i <= u_maxChapter; i++)
	{
		int u_battleSummulation = 0;//副本数目累加
		char buf[12] = {0};
		sprintf(buf,"%d-",i);
		string u_battleId(buf);
		int u_battleCount = g_cMagicServerDb.getInitBattleCount(u_battleId);
		if (u_battleCount > 0)
		{
			for (int j = 1; j <= u_battleCount; j++)
			{
				string u_battleTmpId = u_battleId;
				char bufTmp[12] = {0};
				sprintf(bufTmp,"%d-",j);
				u_battleTmpId += bufTmp;
				unsigned int u_levelCount = 0;
				unsigned int u_energyUse = 0;
				unsigned int u_monsterTotal = 0;
				int u_monsterTotalLevel = 0;
				int u_monsterAverageLevel = 0;
				u_levelCount = g_cMagicServerDb.getInitBattleLevelCount(u_battleTmpId);
				if (u_levelCount > 0)
				{
					if (u_levelCount > 3)
					{
						u_energyUse = MIN_ENERGY_USE + u_levelCount%MIN_ENERGY_USE;
						//g_cMagicServerDb.insertInitBattleInfo(u_battleTmpId,u_levelCount,MIN_ENERGY_USE);
					}
					else
					{
						u_energyUse = MIN_ENERGY_USE;
						//g_cMagicServerDb.insertInitBattleInfo(u_battleTmpId,u_levelCount,MIN_ENERGY_USE);
					}

				}
				for (int m = 1; m <= u_levelCount; m++)
				{
					string u_LevelTmpId = u_battleTmpId;
					char buf3[12] = {0};
					sprintf(buf3,"%d-",m);
					u_LevelTmpId += buf3;
					unsigned int u_monsterCount = 0;
					u_monsterCount = g_cMagicServerDb.getInitLevelMonsterCount(u_LevelTmpId);
					u_monsterTotal += u_monsterCount;
					if (u_monsterCount > 0)
					{
						g_cMagicServerDb.insertInitBattleLevelInfo(u_LevelTmpId,u_monsterCount);
						for (int n = 1; n <= u_monsterCount; n++)
						{
							string u_monterTmpId = u_LevelTmpId;
							char buf4[12] = {0};
							sprintf(buf4,"%d",n);
							u_monterTmpId += buf4;
							g_cMagicServerDb.InitMonsterGoldLoseInfo(u_monterTmpId);
							for(int k = 1; k < EQUIP_TYPE_COUNT; k++)
							{
								g_cMagicServerDb.InitMonsterEquipLoseInfo(u_monterTmpId,k);
							}
							int tmpCardTmpLevel = 0;
							g_cMagicServerDb.InitMonsterCardLoseInfo(u_monterTmpId,tmpCardTmpLevel);
							u_monsterTotalLevel += tmpCardTmpLevel;
							
							g_cMagicServerDb.InitMonsterBigCardIdLoseInfo(u_monterTmpId,tmpCardTmpLevel);

						}
					}


				}
				u_monsterAverageLevel = u_monsterTotalLevel/u_monsterTotal;
				g_cMagicServerDb.insertInitBattleInfo(u_battleTmpId,u_levelCount,u_energyUse,\
					u_monsterTotal,u_monsterAverageLevel,u_monsterTotalLevel);
				u_battleSummulation = g_cMagicServerDb.getBattleSummulation(u_battleId);
				u_battleSummulation += j;
				g_cMagicServerDb.updateBattleSummulation(u_battleTmpId,u_battleSummulation);
			}

		}

	}
}

int main(void)
{
	//char buf[1024] = {0};
	//连接数据库
	if (getDbServerIp() == -1)
	{
		printf("getDbServerIp error\n");
		return -1;
	}
	if (!g_cMagicServerDb.Init())
	{
		//cerr<<"g_cMagicServerDb.Init() error"<<endl;
		cerr<<"g_cMagicServerDb.Init() error:"<<g_cMagicServerDb.getMysqlError()<<endl;
		return -1;
	}
	//从数据连接
	if (!g_cMagicServerDbRead.DbReadInit())
	{
		cerr<<"g_cMagicServerDbRead.Init() error:"<<g_cMagicServerDb.getMysqlError()<<endl;
		return -1;
	}
	////mysql-proxy test////begin
	//g_cMagicServerDb.mysql_proxy_test();
	////mysql-proxy test////end

	/////mysql-master-slave////test begin
	//g_cMagicServerDb.mysql_master_slave_test();
	/////mysql-master-slave////test end

	////////////////////insertChapterInfo///////////////begin
	//g_cMagicServerDb.insertChapterInfo();
	////////////////////insertChapterInfo///////////////end

	////////////根据用户队长卡牌战斗力区分档次//begin
	//g_cMagicServerDb.updateUserCapitorCapacityClass();
	////////////根据用户队长卡牌战斗力区分档次//end

	///////////////////mysql pressure test begin
	//g_cMagicServerDb.mysqlPressureTest();
	///////////////////mysql pressure test end

	///多线程操作同一mysql连接句柄测试begin
	//g_cMagicServerDb.CreateSelectAndInsertThread();
	///多线程操作同一mysql连接句柄测试end

	////////////需改任务列表的唯一索引为普通索引begin
	//g_cMagicServerDb.alterUserMissionsTableIndex();
	////////////需改任务列表的唯一索引为普通索引end


	////////////////更新玩家领导力begin////////////////////
	//g_cMagicServerDb.updataUserLeaderValue();
	////////////////更新玩家领导力end////////////////////

	//////为玩家清空已经不存在的卡牌begin//////////
	//g_cMagicServerDb.clearUserNotExistCard();
	//////为玩家清空已经不存在的卡牌end//////////

	///////////////////清空用户数据begin/////////////
	//g_cMagicServerDb.clearDataBase();
	///////////////////清空用户数据end/////////////
	////////////////邀请码表begin/////////////
	/*g_cMagicServerDb.createInvitationCodeTable();
	g_cMagicServerDb.insertInvitationCodeToTable(2000);*/
	////////////////邀请码表end/////////////

	cerr<<"m_userCardInfo begin"<<endl;
	/////////////printHasNoCardUser///////begin
	//g_cMagicServerDb.printHasNoCardUser();
	/////////////printHasNoCardUser///////end
	//g_cMagicServerDb.createUserActiveBattleInfo(USER_ACTIVE_BATTLE_COUNT);

	////卡牌信息begin////////////////////////
	//创建卡牌表cardInfo
	//g_cMagicServerDb.createInitCardInfo();

	//g_cMagicServerDb.initCardInfoTable();

	//////按星级插入卡牌信息
	//g_cMagicServerDb.initCardInfoByCardStar();
	////卡牌信息end////////////////////////

	//////创建副本信息表begin///////////////////
	//g_cMagicServerDb.createInitBattleInfo();
	//
	////创建副本关卡怪物数量表
	//g_cMagicServerDb.createInitBattleLevelInfo();
	////副本单一怪物金币信息掉落表
	//g_cMagicServerDb.createInitMonsterGoldLoseInfo();
	////副本单一怪物装备信息掉落表
	//g_cMagicServerDb.createInitEquipLoseInfo();
	////副本单一怪物卡牌信息掉落表
	//g_cMagicServerDb.createInitCardLoseInfo();
	////副本单一怪物卡牌Id大于2000信息掉落表
	//g_cMagicServerDb.createInitBigCardIdLoseInfo();
	//
	//int u_maxChapter = 12;
	//initBattleLoseInfo(u_maxChapter);
	//u_maxChapter = 1014;
	//initBattleLoseInfo(u_maxChapter);
	////////创建副本信息表end///////////////////

	////创建100个m_userCardInfo表
	//g_cMagicServerDb.createUserCardInfo();

	//cerr<<"m_userEquipInfo begin"<<endl;
	////创建20个m_userEquipInfo
	//g_cMagicServerDb.createUserEuipInfo();

	//cerr<<"m_userMissionsInfo begin"<<endl;
	////创建20个m_userMissionsInfo
	//g_cMagicServerDb.createUserMissionInfo();
	
	//为之前未插入任务的用户插入任务begin
	/*int u_preMaxMissionId = getPreMaxMissionId();
	cerr<<"u_preMaxMissionId:"<<u_preMaxMissionId<<endl;
	if (u_preMaxMissionId > 0)
	{
		g_cMagicServerDb.insertMissionProccess(u_preMaxMissionId);
	}*/
	//为之前未插入任务的用户插入任务end

	//////为m_userFriendInfo%d添加使用好友时间戳useFriendStamp/////begin
	//g_cMagicServerDb.addFieldForM_userFriendInfo();
	//////为m_userFriendInfo%d添加使用好友时间戳useFriendStamp/////end

	///合并相同的碎片信息//begin
	//g_cMagicServerDb.mergeSameIdFragment();
	///合并相同的碎片信息//end


	//cerr<<"m_userFriendInfo begin"<<endl;
	////创建20个m_userFriendReqInfo
	//g_cMagicServerDb.userCreateUserFriendReqInfo();
	////创建20个m_userFriendInfo
	//g_cMagicServerDb.createUserFriendInfo();
	//cerr<<"m_EquipTmpSaveInfo begin"<<endl;
	////创建100m_EquipTmpSaveInfo表
	//g_cMagicServerDb.creatEquipTmpSaveInfo();
	//cerr<<"m_CardTmpSaveInfo begin"<<endl;
	////创建100个m_CardTmpSaveInfo表
	//g_cMagicServerDb.createCardTmpSaveInfo();

	//创建金币掉落临时存储表
	//g_cMagicServerDb.createGoldTmpSaveInfo();
	/*for (int i = 0; i < USER_CARD_TABLE_COUNT; i++)
	{
		g_cMagicServerDb.modifyUserCardTableField(i);
	}*/
	
	//创建玩家信息表

	///////////////修改玩家任务信息begin
	//g_cMagicServerDb.modifyUserMissionInfo();
	///////////////修改玩家任务信息end
	
	/////////为玩家新增任务Id
	//g_cMagicServerDb.AddUserMissionInfo();
	/////////为玩家新增任务Id

	///为玩家卡牌表将字段VTLGrow,STRGrow,TGHGrow,\
	QUIGrow,MGCGrow的属性由tinyint改为smallint ///begin

	//g_cMagicServerDb.modifyUserCardInfoFieldAttribute();
	///为玩家卡牌表将字段VTLGrow,STRGrow,TGHGrow,\
	QUIGrow,MGCGrow的属性由tinyint改为smallintint ///end


	///创建记录玩家当日已扫荡的次数和已购买体力药水次数的表（20个）begin
	//g_cMagicServerDb.createTableForRecordUserWipeOutAndBuyEneryCount();
	///创建记录玩家当日已扫荡的次数和已购买体力药水次数的表（20个）end

	///更新玩家的体力，体力初始值为30点，每升一级增加一点体力//begin
	g_cMagicServerDb.updateUserEnergy();
	///更新玩家的体力，体力初始值为30点，每升一级增加一点体力//end
	return 0;
	
}