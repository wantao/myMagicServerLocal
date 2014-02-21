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
		int u_battleSummulation = 0;//������Ŀ�ۼ�
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
	//�������ݿ�
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
	//����������
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

	////////////�����û��ӳ�����ս�������ֵ���//begin
	//g_cMagicServerDb.updateUserCapitorCapacityClass();
	////////////�����û��ӳ�����ս�������ֵ���//end

	///////////////////mysql pressure test begin
	//g_cMagicServerDb.mysqlPressureTest();
	///////////////////mysql pressure test end

	///���̲߳���ͬһmysql���Ӿ������begin
	//g_cMagicServerDb.CreateSelectAndInsertThread();
	///���̲߳���ͬһmysql���Ӿ������end

	////////////��������б��Ψһ����Ϊ��ͨ����begin
	//g_cMagicServerDb.alterUserMissionsTableIndex();
	////////////��������б��Ψһ����Ϊ��ͨ����end


	////////////////��������쵼��begin////////////////////
	//g_cMagicServerDb.updataUserLeaderValue();
	////////////////��������쵼��end////////////////////

	//////Ϊ�������Ѿ������ڵĿ���begin//////////
	//g_cMagicServerDb.clearUserNotExistCard();
	//////Ϊ�������Ѿ������ڵĿ���end//////////

	///////////////////����û�����begin/////////////
	//g_cMagicServerDb.clearDataBase();
	///////////////////����û�����end/////////////
	////////////////�������begin/////////////
	/*g_cMagicServerDb.createInvitationCodeTable();
	g_cMagicServerDb.insertInvitationCodeToTable(2000);*/
	////////////////�������end/////////////

	cerr<<"m_userCardInfo begin"<<endl;
	/////////////printHasNoCardUser///////begin
	//g_cMagicServerDb.printHasNoCardUser();
	/////////////printHasNoCardUser///////end
	//g_cMagicServerDb.createUserActiveBattleInfo(USER_ACTIVE_BATTLE_COUNT);

	////������Ϣbegin////////////////////////
	//�������Ʊ�cardInfo
	//g_cMagicServerDb.createInitCardInfo();

	//g_cMagicServerDb.initCardInfoTable();

	//////���Ǽ����뿨����Ϣ
	//g_cMagicServerDb.initCardInfoByCardStar();
	////������Ϣend////////////////////////

	//////����������Ϣ��begin///////////////////
	//g_cMagicServerDb.createInitBattleInfo();
	//
	////���������ؿ�����������
	//g_cMagicServerDb.createInitBattleLevelInfo();
	////������һ��������Ϣ�����
	//g_cMagicServerDb.createInitMonsterGoldLoseInfo();
	////������һ����װ����Ϣ�����
	//g_cMagicServerDb.createInitEquipLoseInfo();
	////������һ���￨����Ϣ�����
	//g_cMagicServerDb.createInitCardLoseInfo();
	////������һ���￨��Id����2000��Ϣ�����
	//g_cMagicServerDb.createInitBigCardIdLoseInfo();
	//
	//int u_maxChapter = 12;
	//initBattleLoseInfo(u_maxChapter);
	//u_maxChapter = 1014;
	//initBattleLoseInfo(u_maxChapter);
	////////����������Ϣ��end///////////////////

	////����100��m_userCardInfo��
	//g_cMagicServerDb.createUserCardInfo();

	//cerr<<"m_userEquipInfo begin"<<endl;
	////����20��m_userEquipInfo
	//g_cMagicServerDb.createUserEuipInfo();

	//cerr<<"m_userMissionsInfo begin"<<endl;
	////����20��m_userMissionsInfo
	//g_cMagicServerDb.createUserMissionInfo();
	
	//Ϊ֮ǰδ����������û���������begin
	/*int u_preMaxMissionId = getPreMaxMissionId();
	cerr<<"u_preMaxMissionId:"<<u_preMaxMissionId<<endl;
	if (u_preMaxMissionId > 0)
	{
		g_cMagicServerDb.insertMissionProccess(u_preMaxMissionId);
	}*/
	//Ϊ֮ǰδ����������û���������end

	//////Ϊm_userFriendInfo%d���ʹ�ú���ʱ���useFriendStamp/////begin
	//g_cMagicServerDb.addFieldForM_userFriendInfo();
	//////Ϊm_userFriendInfo%d���ʹ�ú���ʱ���useFriendStamp/////end

	///�ϲ���ͬ����Ƭ��Ϣ//begin
	//g_cMagicServerDb.mergeSameIdFragment();
	///�ϲ���ͬ����Ƭ��Ϣ//end


	//cerr<<"m_userFriendInfo begin"<<endl;
	////����20��m_userFriendReqInfo
	//g_cMagicServerDb.userCreateUserFriendReqInfo();
	////����20��m_userFriendInfo
	//g_cMagicServerDb.createUserFriendInfo();
	//cerr<<"m_EquipTmpSaveInfo begin"<<endl;
	////����100m_EquipTmpSaveInfo��
	//g_cMagicServerDb.creatEquipTmpSaveInfo();
	//cerr<<"m_CardTmpSaveInfo begin"<<endl;
	////����100��m_CardTmpSaveInfo��
	//g_cMagicServerDb.createCardTmpSaveInfo();

	//������ҵ�����ʱ�洢��
	//g_cMagicServerDb.createGoldTmpSaveInfo();
	/*for (int i = 0; i < USER_CARD_TABLE_COUNT; i++)
	{
		g_cMagicServerDb.modifyUserCardTableField(i);
	}*/
	
	//���������Ϣ��

	///////////////�޸����������Ϣbegin
	//g_cMagicServerDb.modifyUserMissionInfo();
	///////////////�޸����������Ϣend
	
	/////////Ϊ�����������Id
	//g_cMagicServerDb.AddUserMissionInfo();
	/////////Ϊ�����������Id

	///Ϊ��ҿ��Ʊ��ֶ�VTLGrow,STRGrow,TGHGrow,\
	QUIGrow,MGCGrow��������tinyint��Ϊsmallint ///begin

	//g_cMagicServerDb.modifyUserCardInfoFieldAttribute();
	///Ϊ��ҿ��Ʊ��ֶ�VTLGrow,STRGrow,TGHGrow,\
	QUIGrow,MGCGrow��������tinyint��Ϊsmallintint ///end


	///������¼��ҵ�����ɨ���Ĵ������ѹ�������ҩˮ�����ı�20����begin
	//g_cMagicServerDb.createTableForRecordUserWipeOutAndBuyEneryCount();
	///������¼��ҵ�����ɨ���Ĵ������ѹ�������ҩˮ�����ı�20����end

	///������ҵ�������������ʼֵΪ30�㣬ÿ��һ������һ������//begin
	g_cMagicServerDb.updateUserEnergy();
	///������ҵ�������������ʼֵΪ30�㣬ÿ��һ������һ������//end
	return 0;
	
}