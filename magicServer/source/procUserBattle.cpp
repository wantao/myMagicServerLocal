#include "../include/procUserBattle.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/procUserBattleResult.h"
#include "../include/procUserLogin.h"
#include "../include/procUserMissionAward.h"
#include "../include/errorCode.h"
#include "../include/procUserShopBuy.h"
#include "../include/myGlog.h"
#include "../include/procUserUseEnergy.h"
#include "../include/mememCacheClient.h"

cProcUserBattle g_cProcUserBattle;

cProcUserBattle::cProcUserBattle()
{

}
cProcUserBattle::~cProcUserBattle()
{

}

bool cProcUserBattle::getLevelCount(const string& u_battleId,int& u_levelCount,int& u_monsterTotalLevel,int& u_battleSumCount)
{
	char buf[256] = {0};
	sprintf(buf,"select levelCount,battleSumCount,totalLevel from battleInfo where battleId = '%s';",u_battleId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getLevelCount error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_levelCount = q.getIntFileds("levelCount");
	u_battleSumCount = q.getIntFileds("battleSumCount");
	u_monsterTotalLevel = q.getIntFileds("totalLevel");
	q.finalize();
	return true;
}

bool cProcUserBattle::getMonsterCount(const string& u_levelId,int& u_monsterCount)
{
	char buf[128] = {0};
	sprintf(buf,"select monsterCount from battleLevelInfo where levelId = '%s';",u_levelId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getMonsterCount error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_monsterCount = q.getIntFileds("monsterCount");
	q.finalize();
	return true;
}

bool cProcUserBattle::getGoldLose(const string& u_monsterId,int& u_goldLose,int& u_goldLoseNumber)
{
	if (getGoldLoseInfoByKey(u_monsterId,u_goldLose,u_goldLoseNumber))
	{
		//cerr<<"getGoldLoseInfoByKey,success u_monsterId:"<<u_monsterId<<" u_goldLose:"<< \
			u_goldLose<<" u_goldLoseNumber"<<u_goldLoseNumber<<endl;
		return true;
	}
	else
	{
		char buf[128] = {0};
		sprintf(buf,"select goldProbability,goldNumber from battleGoldInfo where monsterId = '%s';",u_monsterId.c_str());
		string strSql(buf);
		MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
		if (q.eof())
		{
			g_cMyGlog.errorlogRecord("getGoldLose error,strSql:%s\n",strSql.c_str());
			q.finalize();
			return false;
		}
		u_goldLose = q.getIntFileds("goldProbability");
		u_goldLoseNumber = q.getIntFileds("goldNumber");
		q.finalize();
		return true;
	}
	return false;
}

bool cProcUserBattle::getEquipLose(const string& u_monsterId,int u_equipType,int& u_equipLose,string& u_quipId)
{
	if (getEquipLoseInfoByKey(u_monsterId,u_equipType,u_equipLose,u_quipId))
	{
		return true;
	}
	else
	{
		char buf[128] = {0};
		sprintf(buf,"select equipId,equipProbability from battleEquipInfo where monsterId = '%s' and equipTypeId = %d;",u_monsterId.c_str(),u_equipType);
		string strSql(buf);
		MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
		if (q.eof())
		{
			g_cMyGlog.errorlogRecord("getEquipLose error,strSql:%s\n",strSql.c_str());
			q.finalize();
			return false;
		}
		u_quipId = q.getStringFileds("equipId");
		u_equipLose = q.getIntFileds("equipProbability");
		q.finalize();
		return true;
	}
	return false;
}

bool cProcUserBattle::getCardLoseInfoByKey(const string& u_monsterId,int& u_cardLose,string& u_cardId,int& u_cardLevel)
{
	string u_battleLoseInfo = "";
	int u_spacePosition[9]={0};
	u_battleLoseInfo = g_mememCacheClient.Get(u_monsterId.c_str());
	if (u_battleLoseInfo.size() == 0)
	{
		g_cMyGlog.errorlogRecord("getCardLoseInfoByKey error,Key:%s\n",u_monsterId.c_str());
		return false;
	}
	string flag=" ";
	int position=0;
	int i=0;
	while((position=u_battleLoseInfo.find_first_of(flag,position))!=string::npos)
	{
		//position=s.find_first_of(flag,position);  
		u_spacePosition[i] = position;
		position++;
		i++;
	}
	u_cardId = u_battleLoseInfo.substr(0,u_spacePosition[0]);
	string u_strCardLose = u_battleLoseInfo.substr(u_spacePosition[0]+1,u_spacePosition[1]-u_spacePosition[0]-1);
	u_cardLose = atoi(u_strCardLose.c_str());

	string u_strBattleLose =  u_battleLoseInfo.substr(u_spacePosition[1]+1,u_spacePosition[2]-u_spacePosition[1]-1);
	u_cardLevel = atoi(u_strBattleLose.c_str());

#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("u_monsterId:%s,u_cardId:%s,u_cardLose:%d\n",u_monsterId.c_str(),u_cardId.c_str(),u_cardLose);
#endif

	return true;
}

bool cProcUserBattle::getEquipLoseInfoByKey(const string& u_monsterId,int& u_equipType,int& u_equipLose,string& u_quipId)
{
	string u_battleLoseInfo = "";
	int u_spacePosition[9]={0};
	u_battleLoseInfo = g_mememCacheClient.Get(u_monsterId.c_str());
	if (u_battleLoseInfo.size() == 0)
	{
		g_cMyGlog.errorlogRecord("getEquipLoseInfoByKey error,Key:%s\n",u_monsterId.c_str());
		return false;
	}
	string flag=" ";
	int position=0;
	int i=0;
	while((position=u_battleLoseInfo.find_first_of(flag,position))!=string::npos)
	{
		//position=s.find_first_of(flag,position);  
		u_spacePosition[i] = position;
		position++;
		i++;
	}
	string u_strEquipType = u_battleLoseInfo.substr(u_spacePosition[2]+1,u_spacePosition[3]-u_spacePosition[2]-1);
	u_equipType = atoi(u_strEquipType.c_str());

	u_quipId = u_battleLoseInfo.substr(u_spacePosition[3]+1,u_spacePosition[4]-u_spacePosition[3]-1);
	
	string u_strEquipLose = u_battleLoseInfo.substr(u_spacePosition[4]+1,u_spacePosition[5]-u_spacePosition[4]-1);
	u_equipLose = atoi(u_strEquipLose.c_str());

#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("u_monsterId:%s,u_quipId:%s,u_strEquipLose:%d\n",u_monsterId.c_str(),u_quipId.c_str(),u_equipLose);
#endif

	return true;
}

bool cProcUserBattle::getGoldLoseInfoByKey(const string& u_monsterId,int& u_goldLose,int& u_goldLoseNumber)
{
	string u_battleLoseInfo = "";
	int u_spacePosition[9]={0};
	u_battleLoseInfo = g_mememCacheClient.Get(u_monsterId.c_str());
	if (u_battleLoseInfo.size() == 0)
	{
		g_cMyGlog.errorlogRecord("getGoldLoseInfoByKey error,Key:%s\n",u_monsterId.c_str());
		return false;
	}
	string flag=" ";
	int position=0;
	int i=0;
	while((position=u_battleLoseInfo.find_first_of(flag,position))!=string::npos)
	{
		//position=s.find_first_of(flag,position);  
		u_spacePosition[i] = position;
		position++;
		i++;
	}

	string u_strGoldLose = u_battleLoseInfo.substr(u_spacePosition[5]+1,u_spacePosition[6]-u_spacePosition[5]-1);
	u_goldLose = atoi(u_strGoldLose.c_str());

	string u_strGoldNumber = u_battleLoseInfo.substr(u_spacePosition[6]+1,u_spacePosition[7]-u_spacePosition[6]-1);
	//string u_strGoldNumber = u_battleLoseInfo.substr(u_spacePosition[6]+1,u_battleLoseInfo.size()-u_spacePosition[5]-1);
	u_goldLoseNumber = atoi(u_strGoldNumber.c_str());

#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("u_monsterId:%s,u_strGoleLose:%d,u_strGoldNumber:%d\n",u_monsterId.c_str(),u_goldLose,u_goldLoseNumber);
#endif

	return true;
}

bool cProcUserBattle::getBigCardIdLoseInfoByKey(const string& u_monsterId,int& u_cardLose,string& u_cardId,int& u_cardLevel)
{
	string u_battleLoseInfo = "";
	int u_spacePosition[9]={0};
	u_battleLoseInfo = g_mememCacheClient.Get(u_monsterId.c_str());
	if (u_battleLoseInfo.size() == 0)
	{
		g_cMyGlog.errorlogRecord("getGoldLoseInfoByKey error,Key:%s\n",u_monsterId.c_str());
		return false;
	}
	string flag=" ";
	int position=0;
	int i=0;
	while((position=u_battleLoseInfo.find_first_of(flag,position))!=string::npos)
	{
		//position=s.find_first_of(flag,position);  
		u_spacePosition[i] = position;
		position++;
		i++;
	}

	u_cardId = u_battleLoseInfo.substr(u_spacePosition[7]+1,u_spacePosition[8]-u_spacePosition[7]-1);

	string n_strBigCardLose = u_battleLoseInfo.substr(u_spacePosition[8]+1,u_battleLoseInfo.size()-u_spacePosition[7]-1);
	u_cardLose = atoi(n_strBigCardLose.c_str());

#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("u_monsterId:%s,n_strBigCardId:%s,n_strBigCardLose:%d\n",u_monsterId.c_str(),u_cardId.c_str(),u_cardLose);
#endif
	u_cardLevel = 1;
	return true;
}


bool cProcUserBattle::getCardLose(const string& u_monsterId,int& u_cardLose,string& u_cardId,int& u_cardLevel)
{
	if (getCardLoseInfoByKey(u_monsterId,u_cardLose,u_cardId,u_cardLevel))
	{
		return true;
	}
	else
	{
		char buf[128] = {0};
		sprintf(buf,"select cardId,cardProbability,cardLevel from battleCardInfo where monsterId = '%s' ;",u_monsterId.c_str());
		string strSql(buf);
		MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
		if (q.eof())
		{
			g_cMyGlog.errorlogRecord("getCardLose error,strSql:%s\n",strSql.c_str());
			q.finalize();
			return false;
		}
		u_cardId = q.getStringFileds("cardId");
		u_cardLose = q.getIntFileds("cardProbability");
		u_cardLevel = q.getIntFileds("cardLevel");
		q.finalize();
		return true;
	}
	return false;
}

bool cProcUserBattle::getBigCardIdLose(const string& u_monsterId,int& u_cardLose,string& u_cardId,int& u_cardLevel)
{
	if (getBigCardIdLoseInfoByKey(u_monsterId,u_cardLose,u_cardId,u_cardLevel))
	{
		return true;
	}
	else
	{
		g_cMyGlog.errorlogRecord("getBigCardIdLoseInfoByKey from memcache error,turn mysql\n");
		char buf[128] = {0};
		sprintf(buf,"select cardId,cardProbability,cardLevel from battle_big_card_id_info where monsterId = '%s' ;",u_monsterId.c_str());
		string strSql(buf);
		MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
		if (q.eof())
		{
			g_cMyGlog.errorlogRecord("getBigCardIdLose error,strSql:%s\n",strSql.c_str());
			q.finalize();
			return false;
		}
		u_cardId = q.getStringFileds("cardId");
		u_cardLose = q.getIntFileds("cardProbability");
		u_cardLevel = q.getIntFileds("cardLevel");
		q.finalize();
		return true;
	}
	return false;
}

bool cProcUserBattle::randGold(const string& u_monsterId,int& u_goldLoseNumber)
{
	int u_randData = 0;
	int u_goldLose = 0;
	if (!getGoldLose(u_monsterId,u_goldLose,u_goldLoseNumber))
	{
		return false;
	}
	u_randData = monsterGetOneRand();
	
	if ((LOSE_MAX-u_goldLose)<u_randData && u_randData<=LOSE_MAX)
	{
		//cerr<<"gold u_monsterId:"<<u_monsterId<<" RandData:"<<u_randData<<endl;
		return true;
	}
	return false;
}

bool cProcUserBattle::randEquip(const string& u_monsterId,int& u_equipType,string& u_equipId)
{
	int u_randData = 0;
	int u_equipLose = 0;
	if (!getEquipLose(u_monsterId,u_equipType,u_equipLose,u_equipId))
	{
		return false;
	}
	u_randData = monsterGetOneRand();
	
	if ((LOSE_MAX-u_equipLose)<u_randData && u_randData<=LOSE_MAX)
	{
		//cerr<<"Equip u_monsterId:"<<u_monsterId<<" RandData:"<<u_randData<<endl;
		return true;
	}
	return false;
}

bool cProcUserBattle::randCard(const string& u_monsterId,string& u_cardId,int& u_cardLevel)
{
	int u_randData = 0;
	int u_cardLose = 0;
	if (!getCardLose(u_monsterId,u_cardLose,u_cardId,u_cardLevel))
	{
		return false;
	}
	u_randData = monsterGetOneRand();
	
	if ((LOSE_MAX-u_cardLose)<u_randData && u_randData<=LOSE_MAX)
	{
		//cerr<<"Card u_monsterId:"<<u_monsterId<<" RandData:"<<u_randData<<endl;
		return true;
	}
	return false;
	
}


bool cProcUserBattle::randBigCardId(const string& u_monsterId,string& u_cardId,int& u_cardLevel)
{
	int u_randData = 0;
	int u_cardLose = 0;
	if (!getBigCardIdLose(u_monsterId,u_cardLose,u_cardId,u_cardLevel))
	{
		return false;
	}
	u_randData = monsterGetOneRand();
//#ifdef DEBUG_MOD
//	u_randData = LOSE_MAX - 1;
//#endif

	if ((LOSE_MAX-u_cardLose)<u_randData && u_randData<=LOSE_MAX)
	{
		//cerr<<"Card u_monsterId:"<<u_monsterId<<" RandData:"<<u_randData<<endl;
		return true;
	}
	return false;

}


bool cProcUserBattle::isExistEquip(const unsigned int& u_userId,unsigned int u_equipType,
				  string u_equipId,int& u_equipCount)
{
	char buf[256] = {0};
	sprintf(buf,"select count from m_EquipTmpSaveInfo%d  where userId = %d and \
				equipTypeId = %d and equipMentId  = '%s' and isShouldDelete = 0;",\
				u_userId%EQUIP_TMP_TABLE_COUNT,u_userId,u_equipType,u_equipId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		/*cerr<<"isExistEquip error:"<<endl;
		cerr<<strSql<<endl;*/
		q.finalize();
		return false;
	}
	u_equipCount = q.getIntFileds("count");
	q.finalize();
	return true;
}

bool cProcUserBattle::updateTmpEquipInfo(const unsigned int& u_userId,unsigned int u_equipType,
						string u_equipId,int& u_equipCount)
{
	char buf[512] = {0};
	sprintf(buf,"update m_EquipTmpSaveInfo%d set count = %d where userId = %d and \
				equipTypeId = %d and equipMentId = '%s' and isShouldDelete = 0;",\
				u_userId%EQUIP_TMP_TABLE_COUNT,u_equipCount,u_userId,u_equipType,u_equipId.c_str());
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateTmpEquipInfo error,strSql:%s\n",strSql.c_str());
	}
	
	return true;
}

bool cProcUserBattle::saveTmpEquipInfo(const unsigned int& u_userId,unsigned int u_equipType,
					  string u_equipId)
{
	int u_equipCount = 0;
	if (isExistEquip(u_userId,u_equipType,u_equipId,u_equipCount))
	{
		u_equipCount += 1;
		updateTmpEquipInfo(u_userId,u_equipType,u_equipId,u_equipCount);
	}
	else
	{
		u_equipCount += 1;
		char buf[256] = {0};
		sprintf(buf,"insert into m_EquipTmpSaveInfo%d(userId,equipTypeId,\
					equipMentId,count,isShouldDelete) values(%d,%d,'%s',%d,%d);",\
					u_userId%EQUIP_TMP_TABLE_COUNT,u_userId,u_equipType,u_equipId.c_str(),u_equipCount,0);
		string strSql(buf);
		if (g_cMagicServerDb.execSQL(strSql) != 0)
		{
			g_cMyGlog.errorlogRecord("saveTmpEquipInfo error,strSql:%s\n",strSql.c_str());
		}

	}
	return true;
}

bool cProcUserBattle::saveTmpCardInfo(const unsigned int& u_userId,string u_cardId,int u_cardLevel)
{
	char buf[128] = {0};
	sprintf(buf,"insert into m_CardTmpSaveInfo%d(userId,cardID,cardLevel,isShouldDelete) values(%d,'%s',%d,%d);",\
				u_userId%CARD_TMP_TABLE_COUNT,u_userId,u_cardId.c_str(),u_cardLevel,0);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("saveTmpCardInfo error,strSql:%s\n",strSql.c_str());
	}
	return true;
}

bool cProcUserBattle::deleteTmpGoldInfo(const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_GoldTmpSaveInfo where userId = %d;",u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("deleteTmpGoldInfo error,strSql:%s\n",strSql.c_str());
	}
	return true;
}

bool cProcUserBattle::saveTmpGoldInfo(const unsigned int& u_userId,int u_totalGoldNumber,int u_userBattleExp)
{
	deleteTmpGoldInfo(u_userId);

	char buf[256] = {0};
	sprintf(buf,"insert into m_GoldTmpSaveInfo(userId,goldNumber,gotExp) values(%d,%d,%d);",\
			u_userId,u_totalGoldNumber,u_userBattleExp);
	
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("saveTmpGoldInfo error,strSql:%s\n",strSql.c_str());
	}
	return true;
}

bool cProcUserBattle::saveTmpGoldInfo(const unsigned int& u_userId,int u_totalGoldNumber,int u_userBattleExp,\
									  bool isInsertDiamond,int u_diamondNum,bool isInsertEnergy,int u_energyCount)
{
	deleteTmpGoldInfo(u_userId);

	char buf[256] = {0};
	if (isInsertDiamond)
	{
		sprintf(buf,"insert into m_GoldTmpSaveInfo(userId,goldNumber,gotExp,awardDiamondNum) values(%d,%d,%d,%d);",\
			u_userId,u_totalGoldNumber,u_userBattleExp,u_diamondNum);
	}
	else if (isInsertEnergy)
	{
		sprintf(buf,"insert into m_GoldTmpSaveInfo(userId,goldNumber,gotExp,awardEnergyCount) values(%d,%d,%d,%d);",\
			u_userId,u_totalGoldNumber,u_userBattleExp,u_energyCount);
	}
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("saveTmpGoldInfo error,strSql:%s\n",strSql.c_str());
	}
	return true;
}

void cProcUserBattle::updatUserEnergy(const unsigned int& u_userId,const unsigned int& u_userEnergy)
{
	char buf[128] = {0};
	sprintf(buf,"update m_gameUserInfo set energy = %d where userID = %d;",u_userEnergy,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updatUserEnergy error,strSql:%s\n",strSql.c_str());
	}
}

//获取用户当前体力
bool cProcUserBattle::getUserEnergy(const unsigned int& u_userId,unsigned int& u_energy)
{
	char buf[128] = {0};
	sprintf(buf,"select energy from m_gameUserInfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserEnergy m_gameUserInfo not find userId:%d\n",u_userId);
		q.finalize();
		return false;
	}
	u_energy = q.getIntFileds("energy");
	q.finalize();
	return true;
}

void cProcUserBattle::setPBUser(PBUser* u_PBUser,const unsigned int& u_userId,int u_userEnergy,int u_usedEnergy)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_gameUserInfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("m_gameUserInfo not find userId:%d\n",u_userId);
		q.finalize();
	}
	//int u_userEnergy = q.getIntFileds("energy");
		
	u_PBUser->set_userid(q.getIntFileds("userID"));
	u_PBUser->set_username(q.getStringFileds("userName"));
	u_PBUser->set_userlevel(q.getIntFileds("userLevel"));
	u_PBUser->set_userexp(q.getIntFileds("userExp"));

	int u_lastEnergyTime = q.getIntFileds("lastEnergyTime");
	int u_userLevel = q.getIntFileds("userLevel");
	int u_nowTimeStamp = 0;
	u_nowTimeStamp = GetTimeStamp();
	int u_NextEnergyTime = g_cProcUserUseEnergy.getNextEnergyTime(u_nowTimeStamp,u_lastEnergyTime,\
		u_userEnergy,u_userLevel);

	unsigned int u_mLastEnergyTime = u_lastEnergyTime;
	unsigned int u_muserEenergy = u_userEnergy;
	g_cProUserMissionAward.updateUserEnergyAndTime(u_muserEenergy,u_mLastEnergyTime,u_userId);

	//int u_maxEnergy = ENERGY + (u_userLevel-1)/2;
	int u_maxEnergy = USER_MAX_ENERGY(u_userLevel);
	if (u_userEnergy == u_maxEnergy)
	{
		u_NextEnergyTime = ONE_ENERGY_NEED_TIME;
	}

	int u_finalEnergy = u_userEnergy - u_usedEnergy;
	u_PBUser->set_energy(u_finalEnergy);
	u_PBUser->set_nextenergytime(u_NextEnergyTime);
	//u_PBUser->set_buyenergytime(q.getIntFileds("buyEnergyTime"));
	newAddSetSomeUserInfo(u_PBUser,u_userId);
	u_PBUser->set_diamondnumber(q.getIntFileds("diamondNumber"));
	u_PBUser->set_friendvalue(q.getIntFileds("friendValue"));
	u_PBUser->set_goldnumber(q.getIntFileds("goldNumber"));
	u_PBUser->set_leadervalue(q.getIntFileds("leaderValue"));
	if (g_cMagicServerDb.hasMissionFinished(u_userId))
	{
		u_PBUser->set_hasmissionfinished(true);
	}
	else
	{
		u_PBUser->set_hasmissionfinished(false);
	}
	u_PBUser->set_bagmaxcount(q.getIntFileds("maxOwnCardCount"));
	u_PBUser->set_energyagentiacount(q.getIntFileds("energyCount"));
	u_PBUser->set_captainindex(q.getIntFileds("playerProfession"));
	q.finalize();
	
}


void cProcUserBattle::proxySetPBBattleReceive(PBBattleAward* u_PBBattleAward,const string& u_monsterId,\
						const int& u_Type,const int& u_Number)
{
	u_PBBattleAward->set_awardid(u_monsterId);
	u_PBBattleAward->set_awardtype(u_Type);
	u_PBBattleAward->set_awardnumber(u_Number);
}

int cProcUserBattle::calUserBattleExp(const int& u_battleSumCount,const int& u_monsterTotalCount,\
					 const int& u_monsterTotalLevel)
{
	int u_userBattleExp = 0;
	u_userBattleExp = (2 + u_battleSumCount*0.1)*u_monsterTotalLevel;
	return u_userBattleExp;
}

//获取玩家最大能拥有的卡牌数
bool cProcUserBattle::getUserMaxOwnCardCount(const unsigned int& u_userId,int& u_maxOwnCardCount)
{
	char buf[128] = {0};
	sprintf(buf,"select maxOwnCardCount from m_gameUserInfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserMaxOwnCardCount failure,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_maxOwnCardCount = q.getIntFileds("maxOwnCardCount");
	q.finalize();
	return true;
}

//获取玩家现在拥有的卡牌数
bool cProcUserBattle::getUserOwnCardCount(const unsigned int& u_userId,int& u_OwnCardCount)
{
	char buf[128] = {0};
	sprintf(buf,"select count(userID) from m_usercardinfo%d where userID = %d;",u_userId%USER_CARD_TABLE_COUNT,u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserOwnCardCount failure,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_OwnCardCount = q.getIntFileds("count(userID)");
	q.finalize();
	return true;
}

//玩家拥有的卡牌是否达到上限
bool cProcUserBattle::isCardCountToMax(const unsigned int& u_userId)
{
	int u_maxOwnCardCount = 0;
	int u_OwnCardCount = 0;
	getUserMaxOwnCardCount(u_userId,u_maxOwnCardCount);
	getUserOwnCardCount(u_userId,u_OwnCardCount);

	if (u_maxOwnCardCount <= u_OwnCardCount)
	{
		return true;
	}
	return false;
}

//更新用户副本进度
void cProcUserBattle::updateUserBattleProgress(const unsigned int& u_userId,const string& u_battleId)
{
	char buf[128] = {0};
	sprintf(buf,"update m_gameuserinfo set battleProgress = '%s' where userID = %d;",u_battleId.c_str(),u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUserBattleProgress failure,strSql:%s\n",strSql.c_str());
	}
}

//获取玩家当前副本进度
bool cProcUserBattle::getUserBattleProgress(const unsigned int& u_userId,string& u_battleId)
{
	char buf[128] = {0};
	sprintf(buf,"select battleProgress from m_gameuserinfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserBattleProgress failure,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_battleId = q.getStringFileds("battleProgress");
	q.finalize();
	return true;
}

//解析副本Id 3-10
int cProcUserBattle::analysisBattleId(char* u_battleIdBuf,int position)
{
	int u_Id = 0;
	char *token  = NULL;
	token  =  strtok(u_battleIdBuf,"-");
	//解析3-10中的3
	if (position == 1)
	{
		u_Id = atoi(token);
	}
	else if (position == 2)
	{
		token = strtok(NULL,"-");
		u_Id = atoi(token);
		
	}
	return u_Id;
}


//判断副本进度是否应该被更新
bool cProcUserBattle::isShouldUpdateBattleProgress(const string& u_newBattleId,const string& u_BattleId)
{
	char *newBuf = NULL;
	char *buf = NULL;
	newBuf = (char*)malloc(sizeof(char)*(u_newBattleId.size()+1));
	buf = (char*)malloc(sizeof(char)*(u_newBattleId.size()+1));
	if (newBuf != NULL && buf != NULL)
	{
		memcpy(newBuf,u_newBattleId.c_str(),u_newBattleId.size());
		memcpy(buf,u_BattleId.c_str(),u_BattleId.size());
		int u_newId1 = analysisBattleId(newBuf,1);
		int u_Id1 = analysisBattleId(buf,1);

		if (u_Id1 == 0 || u_newId1 == 0)
		{
			return false;
		}
		if (u_newId1 > u_Id1)
		{
			if (newBuf != NULL)
			{
				free(newBuf);
			}
			if (buf != NULL)
			{
				free(buf);
			}
			return true;
		}
		else if (u_newId1 == u_Id1)
		{
			memset(newBuf,0,sizeof(newBuf));
			memset(buf,0,sizeof(buf));
			memcpy(newBuf,u_newBattleId.c_str(),u_newBattleId.size());
			memcpy(buf,u_BattleId.c_str(),u_BattleId.size());
			int u_newId2 = analysisBattleId(newBuf,2);
			int u_Id2 = analysisBattleId(buf,2);
			if (u_newId2 > u_Id2)
			{
				if (newBuf != NULL)
				{
					free(newBuf);
				}
				if (buf != NULL)
				{
					free(buf);
				}
				return true;
			}
		}
	}
	if (newBuf != NULL)
	{
		free(newBuf);
	}
	if (buf != NULL)
	{
		free(buf);
	}
	return false;
}

//判断副本关卡是否存在
bool cProcUserBattle::isBattleExist(const string& u_strBattleId,int& u_userLevelLimit,int& u_needEnergy)
{
	char buf[128] = {0};
	sprintf(buf,"select LevelLimited,EnergyUse from chapterinfo where Id = '%s';",u_strBattleId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("not find battleId,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_userLevelLimit = q.getIntFileds("LevelLimited");
	u_needEnergy = q.getIntFileds("EnergyUse");
	q.finalize();
	return true;
} 

//更新用户战斗状态
void cProcUserBattle::updateUserFightStatus(const unsigned int& u_userId,bool u_fightStatus)
{
	char buf[128] = {0};
	sprintf(buf,"update m_gameuserinfo set isInFight = %d where userID = %d;",u_fightStatus,u_userId);
	string strSql(buf);

	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUserFightStatus failure!!!,strSql:%s\n",strSql.c_str());
	}
}

//获取用户等级
bool cProcUserBattle::getUserLevel(const unsigned int& u_userId,int& u_userLevel)
{
	char buf[128] = {0};
	sprintf(buf,"select userLevel from m_gameuserinfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserLevel failure!!!,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_userLevel = q.getIntFileds("userLevel");
	q.finalize();
	return true;
}

//从002-003-004中将卡牌分割出来
void cProcUserBattle::splitCardIdList(const string& u_strCardIdList,const string& u_splitFlag,splitCardIdListMap& u_splitCardIdListMap)
{
	int position=0;
	int prePosition = 0;
	int i=1;
	string u_str = "";
	while((position=u_strCardIdList.find_first_of(u_splitFlag,position))!=string::npos)
	{
		if(prePosition == 0)
		{
			u_str = u_strCardIdList.substr(prePosition,position-prePosition);
		}
		else
		{
			u_str = u_strCardIdList.substr(prePosition+1,position-prePosition-1);
		}
		u_splitCardIdListMap.insert(make_pair(i,u_str));
		prePosition = position;
		position++;
		i++;
	}
	if (i > 1)
	{
		u_str = u_strCardIdList.substr(prePosition+1,position-prePosition-1);
	}
	else
	{
		u_str = u_strCardIdList.substr(0,u_strCardIdList.size());
	}
	u_splitCardIdListMap.insert(make_pair(i,u_str));
}

//随机战斗怪物
int cProcUserBattle::srandCardPostion(splitCardIdListMap& u_splitCardIdListMap,string& u_strCardId)
{
	int u_position = -1;
	if (u_splitCardIdListMap.size() > 0)
	{
		struct timeval tv;
		gettimeofday(&tv,NULL);
		srand(tv.tv_usec+tv.tv_sec);
		u_position = int(random(1,u_splitCardIdListMap.size()+1));
		if (u_position == u_splitCardIdListMap.size()+1)
		{
			u_position = u_splitCardIdListMap.size();
		}
		splitCardIdListMap::iterator it = u_splitCardIdListMap.find(u_position);
		u_strCardId = it->second;
	}
	return u_position;
}

//判断是否为活动副本，1000-1 暂定为u_battleId.size() >= 6,即为活动副本
bool cProcUserBattle::isActiveBattle(const string& u_battleId)
{
	if (u_battleId.size() >= 6)
	{
		return true;
	}
	return false;
}

//得到通关活动副本的数目
void cProcUserBattle::getFinishedActiveBattleCount(const unsigned int& u_userId,const string& u_battleId,int& u_count)
{
	char buf[256] = {0};
	sprintf(buf,"select activeBattleCount from m_useractivebattleinfo%d where userId = %d and activeBattleId = '%s';",\
		u_userId%ACTIVE_BATTLE_COUNT,u_userId,u_battleId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		u_count = 0;
	}
	else
	{
		u_count = q.getIntFileds("activeBattleCount");
	}
	q.finalize();
}
//处理进入活动副本的情况
int cProcUserBattle::procActiveBattle(const unsigned int& u_userId,const string& u_battleId)
{
	int u_activeBattleCount = 0,u_result = 0;
	getFinishedActiveBattleCount(u_userId,u_battleId,u_activeBattleCount);
	if (u_activeBattleCount <= 0)
	{
		return K_TO_MAX_ENTER_ACTIVE_COUNT;
	}
	return u_result;
}

//副本是否为指定日期的副本
bool cProcUserBattle::isSetActiveBattle(const string& u_battleId,const unsigned int& u_userId)
{
	/*long u_timeStamp = GetTimeStamp();
	string u_strTime = getStringTime(u_timeStamp,0);
	int u_weekDay = g_cProcUserLogin.getWeekDayByTime(u_strTime);
	weekDayMissionList u_weekDayMissionList;
	g_cProcUserLogin.getBattleIdListByWeekDay(u_weekDay,u_weekDayMissionList);
	if (u_weekDayMissionList.size() > 0)
	{
		weekDayMissionList::iterator it;
		for (it = u_weekDayMissionList.begin(); it != u_weekDayMissionList.end(); it++)
		{
			if (strcmp(u_battleId.c_str(),(*it).c_str()) == 0)
			{
				return true;
			}
		}
		return false;
	}
	return false;*/
	char buf[256] = {0}; 
	sprintf(buf,"select userId from m_useractivebattleinfo%d where userId = %d and activeBattleId = '%s';",\
		u_userId%ACTIVE_BATTLE_COUNT,u_userId,u_battleId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("isSetActiveBattle,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	q.finalize();
	return true;
}

//获取额外奖励
int cProcUserBattle::getExtraAward(const string& u_battleId,int& u_awardType)
{
	int u_awardThing = 0;
	/*if (strcmp(u_battleId.c_str(),"1000-1") == 0)
	{
		u_awardType = AWARD_CARD;
		u_awardThing = 2001;
	}
	else if (strcmp(u_battleId.c_str(),"1001-1") == 0)
	{
		u_awardType = AWARD_CARD;
		u_awardThing = 2002;
	}
	else if (strcmp(u_battleId.c_str(),"1002-1") == 0)
	{
		u_awardType = AWARD_CARD;
		u_awardThing = 2003;
	}*/
	if (strcmp(u_battleId.c_str(),"1003-1") == 0)
	{
		u_awardType = AWARD_USER_EXP;
		u_awardThing = 4000;
	}
	else if (strcmp(u_battleId.c_str(),"1004-1") == 0)
	{
		u_awardType = AWARD_USER_EXP;
		u_awardThing = 6000;
	}
	else if (strcmp(u_battleId.c_str(),"1005-1") == 0)
	{
		u_awardType = AWARD_USER_EXP;
		u_awardThing = 10000;
	}
	/*else if (strcmp(u_battleId.c_str(),"1006-1") == 0)
	{
		u_awardType = AWARD_MAGIC_STON;
		u_awardThing = 10;
	}
	else if (strcmp(u_battleId.c_str(),"1007-1") == 0)
	{
		u_awardType = AWARD_MAGIC_STON;
		u_awardThing = 20;
	}
	else if (strcmp(u_battleId.c_str(),"1008-1") == 0)
	{
		u_awardType = AWARD_MAGIC_STON;
		u_awardThing = 30;
	}
	else if (strcmp(u_battleId.c_str(),"1009-1") == 0 
		|| strcmp(u_battleId.c_str(),"1010-1") == 0
		|| strcmp(u_battleId.c_str(),"1011-1") == 0)
	{
		u_awardType = AWARD_ENERGY;
		u_awardThing = 1;
	}
	else if (strcmp(u_battleId.c_str(),"1012-1") == 0)
	{
		u_awardType = AWARD_CARD;
		u_awardThing = 2004;
	}
	else if (strcmp(u_battleId.c_str(),"1013-1") == 0)
	{
		u_awardType = AWARD_CARD;
		u_awardThing = 2005;
	}
	else if (strcmp(u_battleId.c_str(),"1014-1") == 0)
	{
		u_awardType = AWARD_CARD;
		u_awardThing = 2006;
	}*/
	return u_awardThing;
}

//存储额外奖励
void cProcUserBattle::saveTmpExtraAward(const unsigned int& u_userId,const int& u_goldNum,const int& u_exp,\
					   const int& u_awardType,const int& u_awardThing)
{
	if (u_awardType == AWARD_CARD)
	{
		string u_strCardId = "";
		char buf[10] = {0};
		sprintf(buf,"%d",u_awardThing);
		u_strCardId = buf;
		int u_cardLevel = 1;
		saveTmpCardInfo(u_userId,u_strCardId,u_cardLevel);
	}
	else if (u_awardType == AWARD_USER_EXP)
	{
		saveTmpGoldInfo(u_userId,u_goldNum,u_awardThing);
	}
	else if (u_awardType == AWARD_ENERGY)
	{
		int u_diamondNum = 0;
		saveTmpGoldInfo(u_userId,u_goldNum,u_exp,false,u_diamondNum,true,u_awardThing);
	}
	else if (u_awardType == AWARD_MAGIC_STON)
	{
		int u_energyCount = 0;
		saveTmpGoldInfo(u_userId,u_goldNum,u_exp,true,u_awardThing,false,u_energyCount);
	}
	/*else 
	{*/
		saveTmpGoldInfo(u_userId,u_goldNum,u_exp);
	//}
}

//获取活动副本经验
int cProcUserBattle::getActiveBattleExp(const string& u_activeBattleId)
{
	int u_activeBattleGetExp = 0;
	char buf[128] = {0};
	sprintf(buf,"select getExp from activeBattle_Exp where activeBattleId = '%s';",u_activeBattleId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getActiveBattleExp error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return u_activeBattleGetExp;
	}
	u_activeBattleGetExp = q.getIntFileds("getExp");
	q.finalize();
	return u_activeBattleGetExp;
}


int cProcUserBattle::setPBBattleReceive(PBBattleReceive& u_PBBattleReceive,const string& u_strToken,PBBattleSend& u_PBBattleSend)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBBattleReceive getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	//g_cMyGlog.errorlogRecord("setPBBattleReceive,userId:%d\n",u_userId);

	string u_batteId = "";
	u_batteId = u_PBBattleSend.battleid();
	
	int u_usedEnergy = 0;
	int u_userLevelLimit = 0;
	int u_userLevel = 0;

	if (!isBattleExist(u_batteId,u_userLevelLimit,u_usedEnergy))
	{
		return K_BATTLE_ID_NOT_EXIST;
	}
	

	if (u_PBBattleSend.has_iswipeout())
	{
		//1,是扫荡
		if (u_PBBattleSend.iswipeout() == 1)
		{
			u_result = isUserToMaxWipeOutCount(u_userId);
			if (u_result != 0)
			{
				return u_result;
			}
		}
	}
	
	getUserLevel(u_userId,u_userLevel);
	if (u_userLevel < u_userLevelLimit)
	{
		return K_LEVEL_OF_ENTER_BATTLE_NOT_ENOUGH;
	}
	if (isCardCountToMax(u_userId))
	{
		return K_BACKPACK_HAS_FULL;
	}
	bool u_isActiveBattle = isActiveBattle(u_batteId);
	if (u_isActiveBattle)
	{
		u_result = procActiveBattle(u_userId,u_batteId);
		if (u_result != 0)
		{
			return u_result;
		}
		if (!isSetActiveBattle(u_batteId,u_userId))
		{
			return -1;
		}
	}
	updateUserFightStatus(u_userId,true);
	//g_cProcUserShopBuy.updateUserBattleReviveCount(u_userId,1);
	g_cProUserBattleResult.deleteTmpData(u_userId);
		//bool u_loseFlag = false;
		int u_levelCount = 0;
		int u_monsterCount = 0;
		int u_monsterTotalCount = 0;
		
		//int u_monsterAverageLevel = 0;
		int u_monsterTotalLevel = 0;
		int u_battleSumCount = 0;
		u_batteId += "-";
		getLevelCount(u_batteId,u_levelCount,u_monsterTotalLevel,u_battleSumCount);
		unsigned int u_energy = 0;
		if (u_usedEnergy > 0)
		{
			if (getUserEnergy(u_userId,u_energy))
			{
				PBUser* u_PBUser;
				u_PBUser = u_PBBattleReceive.mutable_user();
				setPBUser(u_PBUser,u_userId,u_energy,u_usedEnergy);

				if (u_usedEnergy > u_energy)
				{
					return K_ENERGY_NOT_ENOUGH;
				}

				u_energy -= u_usedEnergy;
				updatUserEnergy(u_userId,u_energy);
			}
		}
		if(u_levelCount <= 0)
		{
			g_cMyGlog.errorlogRecord("u_battleId:%s,u_battleId:%d\n",u_batteId.c_str(),u_levelCount);
			u_result = -1;
			return u_result;
		}
		int u_totalGoldNumber = 0;
		for (int i = 1; i <= u_levelCount; i++)
		{
			char buf[12] = {0};
			sprintf(buf,"%s%d",u_batteId.c_str(),i);
			string u_levelId(buf);
			u_levelId += "-";
			getMonsterCount(u_levelId,u_monsterCount);
			if (u_monsterCount <= 0)
			{
				g_cMyGlog.errorlogRecord("u_levelId:%s,u_monsterCount error:%d\n",u_levelId.c_str(),u_monsterCount);
				u_result = -1;
				return u_result;
			}
			u_monsterTotalCount += u_monsterCount;
			for (int j = 1; j <= u_monsterCount; j++)
			{
				char bufTmp[12] = {0};
				sprintf(bufTmp,"%s%d",u_levelId.c_str(),j);
				string u_monsterId(bufTmp);
				int u_goldLoseNumber = 0;
				string u_cardId = "";
				int u_cardLevel = 1;
				if(randBigCardId(u_monsterId,u_cardId,u_cardLevel))
				{
					PBBattleAward* u_PBBattleAward;
					u_PBBattleAward = u_PBBattleReceive.add_allbattleawards();

					//test 类型为3
					proxySetPBBattleReceive(u_PBBattleAward,u_monsterId,3,1);

					u_cardLevel = 1;
					saveTmpCardInfo(u_userId,u_cardId,u_cardLevel);
					continue;
				}
				else if (randGold(u_monsterId,u_goldLoseNumber))
				{
					//u_loseFlag = true;
					u_totalGoldNumber += u_goldLoseNumber;
					PBBattleAward* u_PBBattleAward;
					u_PBBattleAward = u_PBBattleReceive.add_allbattleawards();
					proxySetPBBattleReceive(u_PBBattleAward,u_monsterId,0,u_goldLoseNumber);
					continue; 
				}
				else
				{
					bool hasGenerateEquip = false;
					for (int m = 1; m <= MONSTER_EQUIP_TYPE_COUNT; m++)
					{
						string u_equipId = "";
						
						if (randEquip(u_monsterId,m,u_equipId))
						{
							//u_loseFlag = true;
							saveTmpEquipInfo(u_userId,m,u_equipId);
							PBBattleAward* u_PBBattleAward;
							u_PBBattleAward = u_PBBattleReceive.add_allbattleawards();
							proxySetPBBattleReceive(u_PBBattleAward,u_monsterId,2,1);

							hasGenerateEquip = true;

							//cerr<<"monsterId:"<<u_monsterId<<" getEquip Type:"<<m<<" equipId:"<<u_equipId<<endl;
							break;
						}
						
					}
					//cerr<<"AAAAA"<<endl;
					if (hasGenerateEquip)
					{
						//cerr<<"BBBBB"<<endl;
						continue;
					}
					else
					{
						
						//cerr<<"randCard"<<endl;
						if (randCard(u_monsterId,u_cardId,u_cardLevel))
						{
							//u_loseFlag = true;
							PBBattleAward* u_PBBattleAward;
							u_PBBattleAward = u_PBBattleReceive.add_allbattleawards();

							splitCardIdListMap u_splitCardIdListMap;
							u_splitCardIdListMap.clear();
							splitCardIdList(u_cardId,"-",u_splitCardIdListMap);

							string u_strRandCardId = "";
							int u_cardPosition = srandCardPostion(u_splitCardIdListMap,u_strRandCardId);
							if (u_cardPosition == -1)
							{
								g_cMyGlog.errorlogRecord("!!!!!!!!randCardPosition error!!!!!!\n");
								return -1;
							}
							u_cardPosition += 9;
							proxySetPBBattleReceive(u_PBBattleAward,u_monsterId,u_cardPosition,1);
						
							u_cardLevel = 1;
							saveTmpCardInfo(u_userId,u_strRandCardId,u_cardLevel);
							continue;
						}
					/*	else
						{
							if(randBigCardId(u_monsterId,u_cardId,u_cardLevel))
							{
								PBBattleAward* u_PBBattleAward;
								u_PBBattleAward = u_PBBattleReceive.add_allbattleawards();
								
								proxySetPBBattleReceive(u_PBBattleAward,u_monsterId,19,1);

								u_cardLevel = 1;
								saveTmpCardInfo(u_userId,u_cardId,u_cardLevel);
								continue;
							}
							
						}*/

					}
				}
				
			}
		}
		int u_userBattleExp = 0;
		if (!u_isActiveBattle)
		{
			u_userBattleExp = calUserBattleExp(u_battleSumCount,u_monsterTotalCount,u_monsterTotalLevel);
			saveTmpGoldInfo(u_userId,u_totalGoldNumber,u_userBattleExp);
		}
		else 
		{
			int u_awardType = 0,u_awardThing = 0;
			u_userBattleExp = getActiveBattleExp(u_PBBattleSend.battleid());
			u_awardThing = getExtraAward(u_PBBattleSend.battleid(),u_awardType);
			if (u_awardType == AWARD_USER_EXP)
			{
				//u_awardThing += u_userBattleExp;
				u_userBattleExp += u_awardThing;
			}
			//saveTmpExtraAward(u_userId,u_totalGoldNumber,u_userBattleExp,\
			u_awardType,u_awardThing);
			saveTmpGoldInfo(u_userId,u_totalGoldNumber,u_userBattleExp);
		}
		return u_result;
}