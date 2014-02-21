#include "../include/procUserBattleResult.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/procUserCardLevelUp.h"
#include "../include/procUserSkillLevelUp.h"
#include "../include/procUserLogin.h"
#include "../include/procUserDiamondAward.h"
#include "../include/procUserAwardFriend.h"
#include "../include/procUserCardLevelUp.h"
#include "../include/errorCode.h"
#include "../include/procUserBattle.h"
#include "../include/procUserAllMissions.h"
#include "../include/myGlog.h"
#include "../include/procUserMissionAward.h"

cProUserBattleResult g_cProUserBattleResult;

cProUserBattleResult::cProUserBattleResult()
{

}

cProUserBattleResult::~cProUserBattleResult()
{

}

bool cProUserBattleResult::deleteGoldTmpInfo(const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_GoldTmpSaveInfo where userId = %d;",u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("deleteGoldTmpInfo error,strSql:%s\n",strSql.c_str());
	}
	return true;
}

bool cProUserBattleResult::getTmpGold(const unsigned int& u_userId,int& u_goldNumber,int& u_tmpUserExp,\
									  int& u_tmpDiamondNum,int& u_tmpEnergyCount)
{
	char buf[128] = {0};
	sprintf(buf,"select goldNumber,gotExp,awardDiamondNum,awardEnergyCount from m_GoldTmpSaveInfo where userId = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getTmpGold error:,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_goldNumber = q.getIntFileds("goldNumber");
	u_tmpUserExp = q.getIntFileds("gotExp");
	u_tmpDiamondNum = q.getIntFileds("awardDiamondNum");
	u_tmpEnergyCount = q.getIntFileds("awardEnergyCount");
	q.finalize();
	return true;
}

bool cProUserBattleResult::getTmpEquip(const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_EquipTmpSaveInfo%d where userId = %d and isShouldDelete = 0;",u_userId%EQUIP_TMP_TABLE_COUNT,u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("getTmpEquip error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	while (!q.eof())
	{
		string n_strFragmentId = q.getStringFileds("equipMentId");
		int n_fragMentType = q.getIntFileds("equipTypeId");
		int n_fragMentCount = q.getIntFileds("count");
		addUserFragmentInfo(u_userId,n_fragMentType,n_strFragmentId,n_fragMentCount);
		q.nextRow();
	}
	q.finalize();
	return true;
}



bool cProUserBattleResult::deleteEquipTmpInfo(const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"update m_EquipTmpSaveInfo%d set isShouldDelete = 1 where userId = %d and isShouldDelete = 0;",u_userId%EQUIP_TMP_TABLE_COUNT,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("deleteEquipTmpInfo errorr,strSql:%s\n",strSql.c_str());
	}
	return true;
}

bool cProUserBattleResult::insertCardInfo(const unsigned int& u_userId,MysqlQuery& q,const unsigned int& u_cardLevel)
{
	string cardId = q.getStringFileds("cardId");

	int VTLGrow = getGrow(q.getIntFileds("VTLGrow"));
	int STRGrow = getGrow(q.getIntFileds("STRGrow"));
	int TGHGrow = getGrow(q.getIntFileds("TGHGrow"));
	int QUIGrow = getGrow(q.getIntFileds("QUIGrow"));
	int MGCGrow = getGrow(q.getIntFileds("MGCGrow"));
	
	char buf[512] = {0};
	sprintf(buf,"insert into m_userCardInfo%d(itemId,userID,cardId,cardLevel,\
				cardExp,battlePosition,VTLGrow,STRGrow,TGHGrow,\
				QUIGrow,MGCGrow,skillExp,skillLevel,cardStarLevel) \
				values(NULL,%d,'%s',%d,%d,%d,%d,\
				%d,%d,%d,%d,%d,%d,%d)",u_userId%USER_CARD_TABLE_COUNT,u_userId,cardId.c_str(),u_cardLevel, \
				0,NOT_BATTLE_POSITION,VTLGrow,STRGrow,TGHGrow, \
				QUIGrow,MGCGrow,0,1,q.getIntFileds("CardStarLevel"));
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("insertCardInfo error,strSql:%s\n",strSql.c_str());
	}
	return true;
}

bool cProUserBattleResult::getFixCardInfoAndInsert(const unsigned int& u_userId,const string& u_cardId,const unsigned int& u_cardLevel)
{
	char buf[128] = {0};
	sprintf(buf,"select * from cardInfo where cardId = '%s';",u_cardId.c_str());
	string strSql(buf);

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);

	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getFixCardInfoAndInsert error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	insertCardInfo(u_userId,q,u_cardLevel);
	q.finalize();
	return true;
}

bool cProUserBattleResult::getTmpCardInfo(const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select cardID,cardLevel from m_CardTmpSaveInfo%d where userId = %d and isShouldDelete = 0;",u_userId%CARD_TMP_TABLE_COUNT,u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		//g_cMyGlog.errorlogRecord("getTmpCardInfo error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	while (!q.eof())
	{
		getFixCardInfoAndInsert(u_userId,q.getStringFileds("cardID"),q.getIntFileds("cardLevel"));
		q.nextRow();
	}
	q.finalize();
	return true;
}

bool cProUserBattleResult::deleteCardTmpInfo(const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"update m_CardTmpSaveInfo%d set isShouldDelete = 1 where userId = %d and isShouldDelete = 0;",u_userId%CARD_TMP_TABLE_COUNT,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("deleteCardTmpInfo error,strSql:%s\n",strSql.c_str());
	}
	return true;
}

bool cProUserBattleResult::deleteTmpData(const unsigned int& u_userId)
{
	deleteGoldTmpInfo(u_userId);
	deleteEquipTmpInfo(u_userId);
	deleteCardTmpInfo(u_userId);
	return true;
}

//使各个等级对应的技能熟练度不要越界
void cProUserBattleResult::makeCardExpNotOverFlow(const unsigned int& u_skillLevel,unsigned int& u_skillExp)
{
	if (u_skillLevel == 1)
	{
		if (u_skillExp > ONE_LEVEL_MAX_SKILLEXP)
		{
			u_skillExp = ONE_LEVEL_MAX_SKILLEXP;
		}
	}
	else if (u_skillLevel == 2)
	{
		if (u_skillExp > TWO_LEVEL_MAX_SKILLEXP)
		{
			u_skillExp = TWO_LEVEL_MAX_SKILLEXP;
		}
	}
	else if (u_skillLevel == 3)
	{
		if (u_skillExp > THREE_LEVEL_MAX_SKILLEXP)
		{
			u_skillExp = THREE_LEVEL_MAX_SKILLEXP;
		}
	}
	else if (u_skillLevel == 4)
	{
		if (u_skillExp > FOURE_LEVEL_MAX_SKILLEXP)
		{
			u_skillExp = FOURE_LEVEL_MAX_SKILLEXP;
		}
	}
	else if (u_skillLevel == 5)
	{
		if (u_skillExp > FIVE_LEVEL_MAX_SKILLEXP)
		{
			u_skillExp = FIVE_LEVEL_MAX_SKILLEXP;
		}
	}
	else if (u_skillLevel == 6)
	{
		if (u_skillExp > SIX_LEVEL_MAX_SKILLEXP)
		{
			u_skillExp = SIX_LEVEL_MAX_SKILLEXP;
		}
	}
	else if (u_skillLevel == 7)
	{
		if (u_skillExp > SEVEN_LEVEL_MAX_SKILLEXP)
		{
			u_skillExp = SEVEN_LEVEL_MAX_SKILLEXP;
		}
	}
	else if (u_skillLevel == 8)
	{
		if (u_skillExp > EIGHT_LEVEL_MAX_SKILLEXP)
		{
			u_skillExp = EIGHT_LEVEL_MAX_SKILLEXP;
		}
	}
	else if (u_skillLevel == 9 || u_skillLevel == 10)
	{
		if (u_skillExp > NINE_LEVEL_MAX_SKILLEXP)
		{
			u_skillExp = NINE_LEVEL_MAX_SKILLEXP;
		}
	}
}

bool cProUserBattleResult::updateSkillExp(const unsigned int& u_userId,const unsigned int& u_itemId,const int& u_mSkillExp)
{
	unsigned int u_skillLevel = 0;
	unsigned int u_skillExp = 0;

	if (g_cProcUserSkillLevelUp.getCardSkillLevelExp(u_itemId,u_skillLevel,u_userId,u_skillExp))
	{
		if (!g_cProcUserSkillLevelUp.isSkillCanLevelUp(u_skillLevel,u_skillExp))
		{
			u_skillExp += u_mSkillExp;
			makeCardExpNotOverFlow(u_skillLevel,u_skillExp);
			g_cProcUserSkillLevelUp.updateCardSkillLevelExp(u_itemId,u_skillLevel,u_userId,u_skillExp);
		}
	}
	return true;
}

bool cProUserBattleResult::getUserGoldAndExp(const unsigned int& u_userId,int& u_userGold,int& u_userExp,\
											 int& u_userLevel,int& u_userTmpLevel,int& u_userEnergy,int& u_userLeaderValue,int& u_userMaxEnergy,\
											 int& u_diamondNum,int& u_energyCount)
{
	char buf[256] = {0};
	sprintf(buf,"select userLevel,userExp,energy,diamondNumber,goldNumber,leaderValue,userTmpLevel,maxEnergy,energyCount from m_gameUserInfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserGoldAndExp failure,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_userLevel = q.getIntFileds("userLevel");
	u_userExp = q.getIntFileds("userExp");
	u_userEnergy = q.getIntFileds("energy");
	u_diamondNum = q.getIntFileds("diamondNumber");
	u_userGold = q.getIntFileds("goldNumber");
	u_userLeaderValue = q.getIntFileds("leaderValue");
	u_userTmpLevel = q.getIntFileds("userTmpLevel");
	u_userMaxEnergy = q.getIntFileds("maxEnergy");
	u_energyCount = q.getIntFileds("energyCount");
	q.finalize();
	return true;
}

bool cProUserBattleResult::updateUserGoldAndExp(const unsigned int& u_userId,int u_userGold,int u_userExp,\
												int u_userLevel,int u_userTmpLevel,int u_userEnergy,\
												int u_userLeaderValue,int u_userMaxEnergy,int u_diamNum,int u_energyCount)
{
	char buf[256] = {0};
	sprintf(buf,"update m_gameUserInfo set goldNumber = %d,userExp=%d,userLevel = %d,\
				userTmpLevel = %d,energy = %d,leaderValue = %d,maxEnergy = %d,diamondNumber =%d,energyCount = %d where userID = %d;",\
				u_userGold,u_userExp,u_userLevel,u_userTmpLevel,u_userEnergy,u_userLeaderValue,u_userMaxEnergy,u_diamNum,u_energyCount,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUserGoldAndExp failure,strSql:%s\n",strSql.c_str());
		return false;
	}
	return true;
}

//玩家升级计算
void cProUserBattleResult::addUserLevel(int& u_userExp,int& u_userLevel)
{
	while (u_userExp > USER_LEVEL_UP_NEED_EXP(u_userLevel))
	{
		u_userExp -= USER_LEVEL_UP_NEED_EXP(u_userLevel);
		u_userLevel += 1;
	}
}

bool cProUserBattleResult::getTmpData(const unsigned int& u_userId,bool& hasMissionFinished)
{
	int u_tmpGoldNumber = 0;
	int u_tmpUserExp = 0;
	int u_userCurrentGold = 0;
	int u_userCurrentExp = 0;
	int u_tmpDiamondNum = 0;
	int u_tmpEnergyCount = 0;
	if (getTmpGold(u_userId,u_tmpGoldNumber,u_tmpUserExp,u_tmpDiamondNum,u_tmpEnergyCount))
	{
		int u_userGold = 0;
		int u_userExp = 0;
		int u_userLevel = 0;
		int u_userTmpLevel = 0;
		int u_userEnergy = 0;
		int u_userLeaderValue = 0;
		int u_userMaxEnergy = 0;
		int u_diamondNum = 0,u_energyCount = 0;
		if (getUserGoldAndExp(u_userId,u_userCurrentGold,u_userCurrentExp,\
			u_userLevel,u_userTmpLevel,u_userEnergy,u_userLeaderValue,u_userMaxEnergy,u_diamondNum,u_energyCount))
		{
			
			if (u_userLevel < USER_MAX_LEVEL)
			{
				u_userGold = u_userCurrentGold + u_tmpGoldNumber;
				u_userExp = u_userCurrentExp + u_tmpUserExp;
				if (u_userExp >= USER_LEVEL_UP_NEED_EXP(u_userLevel))
				{
					//addUserLevel(u_userExp,u_userLevel);
					while (u_userExp > USER_LEVEL_UP_NEED_EXP(u_userLevel))
					{
						u_userExp -= USER_LEVEL_UP_NEED_EXP(u_userLevel);
						u_userLevel += 1;
						u_userMaxEnergy = USER_MAX_ENERGY(u_userLevel);//ENERGY + (u_userLevel-1)/2;
						//u_userEnergy = u_userMaxEnergy;
						//升一级所需增加的疲劳为u_userLevel+4
						int u_needAddEnergy = u_userLevel + 4;
						u_userEnergy += u_needAddEnergy;
						//u_userLeaderValue = LEADERVALUE + (u_userLevel-1)/2;
						u_userLeaderValue = LEADERVALUE + (u_userLevel-1);
						u_userTmpLevel = u_userLevel;
						char buf[6] = {0};
						sprintf(buf,"%d",u_userLevel);
						string strLevel(buf);
						int u_missionIsFinished = 0;
						if (g_cProcUserAllMissions.isMissionExist(u_userId,LEVEL_UP_MISSION,strLevel,u_missionIsFinished))
						{
							if (u_missionIsFinished == 0)
							{
								g_cProcUserAllMissions.updateMissionStatus(u_userId,LEVEL_UP_MISSION,strLevel,true);
								hasMissionFinished = true;
							}
						}
						
						/*if (u_userLevel - u_userTmpLevel == 2)
						{
							u_userLeaderValue += 1;
							u_userTmpLevel = u_userLevel;
							u_userMaxEnergy += 1;
						}*/
					}
					
				}
				u_diamondNum += u_tmpDiamondNum;
				u_energyCount += u_tmpEnergyCount;
				updateUserGoldAndExp(u_userId,u_userGold,u_userExp,\
					u_userLevel,u_userTmpLevel,u_userEnergy,\
					u_userLeaderValue,u_userMaxEnergy,u_diamondNum,u_energyCount);
			}
			
			deleteGoldTmpInfo(u_userId);
		}
	
	}
	if (getTmpEquip(u_userId))
	{
		deleteEquipTmpInfo(u_userId);
	}
	if (getTmpCardInfo(u_userId))
	{
		deleteCardTmpInfo(u_userId);
	}
	return true;
}


//获取用户战斗状态
bool cProUserBattleResult::getUserFightStatus(const unsigned int& u_userId,int& u_fightStatus)
{
	char buf[128] = {0};
	sprintf(buf,"select isInFight from m_gameuserinfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);

	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserFightStatus error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_fightStatus = q.getIntFileds("isInFight");
	q.finalize();
	return true;
}

//判断副本是否存在
bool cProUserBattleResult::isBattleExist(const string& u_strBattleId)
{
	char buf[128] = {0};
	sprintf(buf,"select Id from chapterinfo where Id = '%s';",u_strBattleId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("isBattleExist not find battleId,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	q.finalize();
	return true;
}

//更新进入活动副本次数
void cProUserBattleResult::updateEnterActiveBattleCount(const unsigned int& u_userId,const string& u_battleId,const int& u_count)
{
	char buf[256] = {0};
	sprintf(buf,"update m_useractivebattleinfo%d set activeBattleCount = %d where userId = %d and activeBattleId = '%s';",\
		u_userId%ACTIVE_BATTLE_COUNT,u_count,u_userId,u_battleId.c_str());
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateEnterActiveBattleCount error,strSql:%s\n",strSql.c_str());
	}
}

void cProUserBattleResult::updateUseFriendCount(const unsigned int& u_userId,const int& u_friendId,const int& u_useFriendCount)
{
	char buf[256] = {0};
	sprintf(buf,"update m_userFriendInfo%d set useFriendCount = %d where userId = %d and friendId = %d;",\
		u_userId%USER_FRIEND_TABLE_COUNT,u_useFriendCount,u_userId,u_friendId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUseFriendCount error,strSql:%s\n",strSql.c_str());
	}
}

//更新使用好友时间戳
void cProUserBattleResult::updateUseFriendStamp(const unsigned int& u_userId,const int& u_friendId,const int& u_useFriendStamp)
{
	char buf[256] = {0};
	sprintf(buf,"update m_userFriendInfo%d set useFriendStamp = %d where userId = %d and friendId = %d;",\
		u_userId%USER_FRIEND_TABLE_COUNT,u_useFriendStamp,u_userId,u_friendId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUseFriendStamp error,strSql:%s\n",strSql.c_str());
	}
}

//获取好友被用于战斗的次数
int cProUserBattleResult::getUseFriendCount(const unsigned int& u_userId,const int& u_friendId)
{
	int u_UseFriendCount = 0;
	char buf[128] = {0};
	sprintf(buf,"select useFriendCount from m_userFriendInfo%d where userId = %d and friendId = %d;",\
		u_userId%USER_FRIEND_TABLE_COUNT,u_userId,u_friendId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUseFriendCount error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return u_UseFriendCount;
	}
	u_UseFriendCount = q.getIntFileds("useFriendCount");
	q.finalize();
	return u_UseFriendCount;
}

bool cProUserBattleResult::authCardDbInfo(const CardInfoAuth& u_CardInfoAuth,const PBCardDBInfo& u_PBCardDBInfo)
{
	if (strcmp((u_CardInfoAuth.u_cardId).c_str(),(u_PBCardDBInfo.cardid()).c_str()) != 0)
	{
		g_cMyGlog.errorlogRecord("authCardDbInfo failure,cardDbId:%s,recvCardId:%s\n",\
			(u_CardInfoAuth.u_cardId).c_str(),(u_PBCardDBInfo.cardid()).c_str());
		return false;
	}
	if (strcmp((u_CardInfoAuth.u_cardSkillA).c_str(),(u_PBCardDBInfo.cardskilla()).c_str()) != 0)
	{
		g_cMyGlog.errorlogRecord("authCardDbInfo failure,cardDbId:%s,recvCardId:%s\n",\
			(u_CardInfoAuth.u_cardId).c_str(),(u_PBCardDBInfo.cardid()).c_str());
		g_cMyGlog.errorlogRecord("cardDb SkillA:%s,recvCard SkillA:%s\n",\
			(u_CardInfoAuth.u_cardSkillA).c_str(),(u_PBCardDBInfo.cardskilla()).c_str());
		return false;
	}
	if (strcmp((u_CardInfoAuth.u_cardSkillP).c_str(),(u_PBCardDBInfo.cardskillp()).c_str()) != 0)
	{
		g_cMyGlog.errorlogRecord("authCardDbInfo failure,cardDbId:%s,recvCardId:%s\n",\
			(u_CardInfoAuth.u_cardId).c_str(),(u_PBCardDBInfo.cardid()).c_str());
		g_cMyGlog.errorlogRecord("cardDb u_cardSkillP:%s,recvCard cardskillp:%s\n",\
			(u_CardInfoAuth.u_cardSkillP).c_str(),(u_PBCardDBInfo.cardskillp()).c_str());
		return false;
	}
	if (u_CardInfoAuth.VTLStar != u_PBCardDBInfo.vtlstar())
	{
		g_cMyGlog.errorlogRecord("authCardDbInfo failure,cardDbId:%s,recvCardId:%s\n",\
			(u_CardInfoAuth.u_cardId).c_str(),(u_PBCardDBInfo.cardid()).c_str());
		g_cMyGlog.errorlogRecord("cardDb VTLStar:%d,recvCard VTLStar:%d\n",\
			u_CardInfoAuth.VTLStar,u_PBCardDBInfo.vtlstar());
		return false;
	}
	if (u_CardInfoAuth.STRStar != u_PBCardDBInfo.strstar())
	{
		g_cMyGlog.errorlogRecord("authCardDbInfo failure,cardDbId:%s,recvCardId:%s\n",\
			(u_CardInfoAuth.u_cardId).c_str(),(u_PBCardDBInfo.cardid()).c_str());
		g_cMyGlog.errorlogRecord("cardDb STRStar:%d,recvCard STRStar:%d\n",\
			u_CardInfoAuth.STRStar,u_PBCardDBInfo.strstar());
		return false;
	}
	if (u_CardInfoAuth.TGHStar != u_PBCardDBInfo.tghstar())
	{
		g_cMyGlog.errorlogRecord("authCardDbInfo failure,cardDbId:%s,recvCardId:%s\n",\
			(u_CardInfoAuth.u_cardId).c_str(),(u_PBCardDBInfo.cardid()).c_str());
		g_cMyGlog.errorlogRecord("cardDb TGHStar:%d,recvCard TGHStar:%d\n",\
			u_CardInfoAuth.TGHStar,u_PBCardDBInfo.tghstar());
		return false;
	}
	if (u_CardInfoAuth.QUIStar != u_PBCardDBInfo.quistar())
	{
		g_cMyGlog.errorlogRecord("authCardDbInfo failure,cardDbId:%s,recvCardId:%s\n",\
			(u_CardInfoAuth.u_cardId).c_str(),(u_PBCardDBInfo.cardid()).c_str());
		g_cMyGlog.errorlogRecord("cardDb QUIStar:%d,recvCard QUIStar:%d\n",\
			u_CardInfoAuth.QUIStar,u_PBCardDBInfo.quistar());
		return false;
	}
	if (u_CardInfoAuth.MGCStar != u_PBCardDBInfo.mgcstar())
	{
		g_cMyGlog.errorlogRecord("authCardDbInfo failure,cardDbId:%s,recvCardId:%s\n",\
			(u_CardInfoAuth.u_cardId).c_str(),(u_PBCardDBInfo.cardid()).c_str());
		g_cMyGlog.errorlogRecord("cardDb MGCStar:%d,recvCard MGCStar:%d\n",\
			u_CardInfoAuth.MGCStar,u_PBCardDBInfo.mgcstar());
		return false;
	}
	return true;
}

bool cProUserBattleResult::authCardDbInfo(const PBBattleResultSend& u_PBBattleResultSend)
{
	int i = 0;
	while (i < u_PBBattleResultSend.cardinfo_size())
	{
		cardInfoAuthMap::iterator it = g_cardInfoAuthMap.find((u_PBBattleResultSend.cardinfo(i)).cardid());
		if (it == g_cardInfoAuthMap.end())
		{
			g_cMyGlog.errorlogRecord("not find u_PBBattleResultSend.cardinfo(%d),cardId:%s\n",\
				i,(u_PBBattleResultSend.cardinfo(i)).cardid().c_str());
			return false;
		}
		if (!authCardDbInfo(it->second,u_PBBattleResultSend.cardinfo(i)))
		{
			return false;
		}
		i++;
	}
	return true;
}

int cProUserBattleResult::setPBBattleResultReceive(PBBattleResultReceive& u_PBBattleResultReceive,const string& u_strToken,PBBattleResultSend& u_PBBattleResultSend)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBBattleResultReceive getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	//g_cMyGlog.errorlogRecord("setPBBattleResultReceive:%d\n",u_userId);
	int u_fightStatus = 0;
	if (!getUserFightStatus(u_userId,u_fightStatus))
	{
		u_result = -1;
		return u_result;
	}
	if (u_fightStatus == 0)
	{
		return K_NOT_ENTER_BATTLE;
	}
	g_cProcUserBattle.updateUserFightStatus(u_userId,false);
	int u_battleResult = 0;
	string u_strBattleId = "";
	string u_battleId = u_PBBattleResultSend.battleprogress();
	bool hasMissionFinished = false;
	bool is_dataUnNormal = false;
	if (!isBattleExist(u_battleId))
	{
		return K_BATTLE_ID_NOT_EXIST;
	}
	g_cProcUserBattle.getUserBattleProgress(u_userId,u_strBattleId);
	
	u_battleResult = u_PBBattleResultSend.battleresult();
	if (u_battleResult == BATTLE_FINAL_LOSE)
	{
		deleteTmpData(u_userId);
		is_dataUnNormal = authCardDbInfo(u_PBBattleResultSend);
		if (is_dataUnNormal)
		{
			for (int i = 0; i < u_PBBattleResultSend.skillexpadd_size(); i++)
			{
				updateSkillExp(u_userId,u_PBBattleResultSend.skillexpadd(i).carditemid(),u_PBBattleResultSend.skillexpadd(i).skillexp());
			}
		}
	}
	else if(u_battleResult == BATTLE_WIN)
	{
		is_dataUnNormal = authCardDbInfo(u_PBBattleResultSend);
		if (is_dataUnNormal)
		{
			unsigned int u_friendValue = 0;
			g_cProcUserAwardFriend.getUserFriendValue(u_userId,u_friendValue);
			if (u_PBBattleResultSend.isfriend())
			{
				u_friendValue += FRIEND_ADD_VALUE;
				int u_friendId = u_PBBattleResultSend.friendid();
				/*int u_useFriendCount = getUseFriendCount(u_userId,u_friendId);
				if (u_useFriendCount < USE_FRIEND_MAX_COUNT_ONE_DAY)
				{
					u_useFriendCount += 1;
					updateUseFriendCount(u_userId,u_friendId,u_useFriendCount);
				}*/
				int n_useFriendStamp = GetTimeStamp();
				updateUseFriendStamp(u_userId,u_friendId,n_useFriendStamp);
			}
			else
			{
				u_friendValue += NOT_FRIEND_ADD_VALUE;
			}
			if (u_friendValue > MAX_FRIEND_VALUE)
			{
				u_friendValue = MAX_FRIEND_VALUE;
			}
			g_cProcUserAwardFriend.updataUserFriendValue(u_userId,u_friendValue);
			getTmpData(u_userId,hasMissionFinished);
			//cerr<<"1111Size:"<<u_PBBattleResultSend.skillexpadd_size()<<endl;
			for (int i = 0; i < u_PBBattleResultSend.skillexpadd_size(); i++)
			{
				updateSkillExp(u_userId,u_PBBattleResultSend.skillexpadd(i).carditemid(),u_PBBattleResultSend.skillexpadd(i).skillexp());
			}
			deleteTmpData(u_userId);

			//cerr<<"11111"<<endl;
			if (!g_cProcUserBattle.isActiveBattle(u_battleId))
			{
				if (g_cProcUserBattle.isShouldUpdateBattleProgress(u_battleId,u_strBattleId))
				{
					//cerr<<"22222"<<endl;
					g_cProcUserBattle.updateUserBattleProgress(u_userId,u_battleId);
					u_strBattleId.clear();
					u_strBattleId = u_battleId;
				}
				int u_missionIsFinished = 0;
				if (g_cProcUserAllMissions.isMissionExist(u_userId,BATTLE_MISSION,u_battleId,u_missionIsFinished))
				{
					if (u_missionIsFinished == 0)
					{
						g_cProcUserAllMissions.updateMissionStatus(u_userId,BATTLE_MISSION,u_battleId,true);
						hasMissionFinished = true;
					}
				}

				//如果是通关大关卡，还需通知
				procInsertPassLevelNotice(u_battleId,u_userId);
			}
			else
			{
				int u_count = 0;
				g_cProcUserBattle.getFinishedActiveBattleCount(u_userId,u_battleId,u_count);
				if (u_count > 0)
				{
					u_count -= 1;
					updateEnterActiveBattleCount(u_userId,u_battleId,u_count);
				}
				
				if (g_cProUserMissionAward.insertActiveBattleMissionAward(u_userId,u_battleId))
				{
					hasMissionFinished = true;
				}


			}
		}
	}
	else
	{
		u_result = -1;
		return u_result;
	}
	PBAllCard* u_PBAllCard;
	u_PBAllCard = u_PBBattleResultReceive.mutable_cards();
	g_cProcUserLogin.getAllCardsInfo(u_PBAllCard,u_userId);
	
	///////////////test///
	//printUserAllCardInfo(*u_PBAllCard);
	//////////////test////

	PBUser* u_PBUser;
	u_PBUser = u_PBBattleResultReceive.mutable_user();
	g_cProcUserDiamondAward.setPBUser(u_PBUser,u_userId);

	PBAllGem* u_PBAllGem;
	u_PBAllGem = u_PBBattleResultReceive.mutable_gems();
	g_cProcUserLogin.getUserGemInfo(u_PBAllGem,u_userId);
	
	//u_PBBattleResultReceive.set_battleprogress(u_strBattleId);
	//添加副本信息
	PBBattle* u_PBBattle;
	u_PBBattle = u_PBBattleResultReceive.mutable_battle();
	u_PBBattle->set_pveprogress(u_strBattleId);
	if (!g_cProcUserLogin.getActiveBattleInfo(u_PBBattle,u_userId))
	{
		//cerr<<"123 getUserBattleInfo error"<<endl;
	}

	if (hasMissionFinished)
	{
		u_PBBattleResultReceive.set_hasfinishedtask(true);
	}
	else
	{
		u_PBBattleResultReceive.set_hasfinishedtask(false);
	}
	if (is_dataUnNormal)
	{
		u_PBBattleResultReceive.set_dbinforesult(0);
	}
	else
	{
		u_PBBattleResultReceive.set_dbinforesult(1);
	}
	return u_result;
}

void cProUserBattleResult::printUserAllCardInfo(const PBAllCard& u_PBAllCard)
{
	g_cMyGlog.debugLog("UserAllCard: size:%d\n",u_PBAllCard.allcard_size());
	for (int i = 0; i < u_PBAllCard.allcard_size(); i++)
	{
		g_cMyGlog.debugLog("i:%d\n",i);
		printCardInfo(u_PBAllCard.allcard(i));
	}
}

void cProUserBattleResult::printCardInfo(const PBCard& u_PBCard)
{
	g_cMyGlog.debugLog("CardInfo:itemId:%d,cardId:%s,battlePosition:%d\n",
		u_PBCard.itemid(),(u_PBCard.cardid()).c_str(),u_PBCard.battleposition());
	cerr<<"CardInfo:"<<endl;
	cerr<<"itemId:"<<u_PBCard.itemid()<<endl;
	cerr<<"cardId:"<<u_PBCard.cardid()<<endl;
	cerr<<"cardLevel"<<u_PBCard.cardlevel()<<endl;
	cerr<<"cardExp"<<u_PBCard.cardexp()<<endl;
	cerr<<"skillExp:"<<u_PBCard.skillexp()<<endl;
	cerr<<"skillLevel:"<<u_PBCard.skilllevel()<<endl;
	cerr<<"battlePosition:"<<u_PBCard.battleposition()<<endl;
	cerr<<"VTLGrow:"<<u_PBCard.vtlgrow()<<endl;
	cerr<<"STRGrow:"<<u_PBCard.strgrow()<<endl;
	cerr<<"TGHGrow:"<<u_PBCard.tghgrow()<<endl;
	cerr<<"QUIGrow:"<<u_PBCard.quigrow()<<endl;
	cerr<<"MGCGrow:"<<u_PBCard.mgcgrow()<<endl;
}

//加载卡牌信息内存
bool cProUserBattleResult::loadCardInfoToMem()
{
	char buf[128] = {0};
	sprintf(buf,"select * from cardinfoinput;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("loadCardInfoToMem error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	while (!q.eof())
	{
		CardInfoAuth u_CardInfoAuth;
		u_CardInfoAuth.u_cardId = q.getStringFileds("Id");
		u_CardInfoAuth.u_cardSkillA = q.getStringFileds("ActiveSkill");
		u_CardInfoAuth.u_cardSkillP = q.getStringFileds("PassiveSkill");
		u_CardInfoAuth.VTLStar = atoi((q.getStringFileds("VTL")).c_str());
		u_CardInfoAuth.STRStar = atoi((q.getStringFileds("STR")).c_str());
		u_CardInfoAuth.TGHStar = atoi((q.getStringFileds("TGH")).c_str());
		u_CardInfoAuth.QUIStar = atoi((q.getStringFileds("QUI")).c_str());
		u_CardInfoAuth.MGCStar = atoi((q.getStringFileds("MGC")).c_str());
		g_cardInfoAuthMap.insert(make_pair(u_CardInfoAuth.u_cardId,u_CardInfoAuth));
		q.nextRow();
	}
	g_cMyGlog.errorlogRecord("loadCardInfoToMem success\n");
	q.finalize();
	return true;
}