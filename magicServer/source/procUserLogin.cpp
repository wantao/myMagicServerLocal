#include "../include/procUserLogin.h"
#include "../include/magicServerDb.h"
#include "../include/define.h"
#include "../include/procUserMissionAward.h"
#include "../include/loginMission.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"
#include "../include/procUserUseEnergy.h"

cProcUserLogin g_cProcUserLogin;


cProcUserLogin::cProcUserLogin()
{

}
cProcUserLogin::~cProcUserLogin()
{

}

//获取最大体力
bool cProcUserLogin::getMaxEnergy(const int& u_userId,unsigned int& u_MaxEnergy)
{
	char buf[128] = {0};
	sprintf(buf,"select maxEnergy from m_gameUserInfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getMaxEnergy failure, strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_MaxEnergy = q.getIntFileds("maxEnergy");
	q.finalize();
	return true;
}
//获取下一点体力恢复的时间（s）
//输出：当前体力，当前体力恢复时间戳
unsigned int cProcUserLogin::getNextEnergyTime(const unsigned int& u_userId,unsigned int& energy,unsigned int& lastEnergyTimeStamp)
{
	unsigned int u_NextEnergyTime = 0;
	unsigned int u_MaxEnergy = 0;
	getMaxEnergy(u_userId,u_MaxEnergy);
	if (energy >= u_MaxEnergy)
	{
		lastEnergyTimeStamp = GetTimeStamp();
		energy = u_MaxEnergy;
		return u_NextEnergyTime;
	}
	else
	{
		unsigned int u_diffTime = GetTimeStamp() - lastEnergyTimeStamp;
		energy += u_diffTime/ONE_ENERGY_NEED_TIME;
		if (energy >= u_MaxEnergy)
		{
			energy = u_MaxEnergy;
			lastEnergyTimeStamp = GetTimeStamp();
			u_NextEnergyTime = 0;
		}
		else
		{
			lastEnergyTimeStamp = GetTimeStamp();
			if (u_diffTime%ONE_ENERGY_NEED_TIME == 0)
			{
				u_NextEnergyTime = ONE_ENERGY_NEED_TIME;
			}
			else
			{
				u_NextEnergyTime = u_diffTime%ONE_ENERGY_NEED_TIME;
			}
			
		}
	}
	return u_NextEnergyTime;
}

//更新用户登录时间
bool cProcUserLogin::updateUserLogtime(unsigned int u_LogTimeStamp,const unsigned int& u_userId)
{
	string strLogTime("");
	strLogTime = GetTime(u_LogTimeStamp);
	char buf[128] = {0};

	sprintf(buf,"update m_gameUserInfo set logTime = '%s' where userID = %d;",strLogTime.c_str(),u_userId);
	string strSql(buf);

	if(g_cMagicServerDb.execSQL(strSql)!=0)
	{
		g_cMyGlog.errorlogRecord("update failure,strSql:%s\n",strSql.c_str());
		return false;
	}
	return true;
	
}

//通过mac地址获取用户信息
bool cProcUserLogin::getUserInfoByMacAddress(PBUser* u_PBUser,const PBLoginSend& u_PBLoginSend,string& u_battleProgress,unsigned int& u_userID)
{
	int u_logType = 0;
	u_logType = getLoginType(u_PBLoginSend);
	if (u_logType == 0)
	{
		return false;
	}
	char buf[256] = {0};
	//用户帐号登录
	if (u_logType == 1)
	{
		sprintf(buf,"select * from m_gameUserInfo where userAccount = '%s' and plateform=%d;",(u_PBLoginSend.useraccount()).c_str(),u_PBLoginSend.usersourcetype());
	}
	//用户mac地址登录
	else
	{
		sprintf(buf,"select * from m_gameUserInfo where macAddress = '%s' and plateform=%d;",(u_PBLoginSend.macaddress()).c_str(),u_PBLoginSend.usersourcetype());
	}
	
	string strSql(buf);

#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("getUserInfoByMacAddress:strSql:%s\n",strSql.c_str());
#endif

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		//cerr<<"not find "<<strMac.c_str()<<" in m_gameUserInfo"<<endl;
		g_cMyGlog.debugLog("getUserInfoByMacAddress failure:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	else
	{
		//用户帐号登录，验证密码
		if (u_logType == 1)
		{
			string u_strPasswd = q.getStringFileds("userPasswd");
			if (u_strPasswd.size() > 0)
			{
				if (strcmp(u_strPasswd.c_str(),(u_PBLoginSend.userpasswd()).c_str()) != 0)
				{
					q.finalize();
					return false;
				}
			}
		}
		u_userID = q.getIntFileds("userID");
		u_PBUser->set_userid(u_userID);
		u_PBUser->set_username(q.getStringFileds("userName"));
		u_PBUser->set_userlevel(q.getIntFileds("userLevel"));
		u_PBUser->set_userexp(q.getIntFileds("userExp"));

		int u_energy = q.getIntFileds("energy");
		int u_lastEnergyTime = q.getIntFileds("lastEnergyTime");
		int u_userLevel = q.getIntFileds("userLevel");

		int u_nowTimeStamp = 0;
		u_nowTimeStamp = GetTimeStamp();
		int u_NextEnergyTime = g_cProcUserUseEnergy.getNextEnergyTime(u_nowTimeStamp,u_lastEnergyTime,u_energy,u_userLevel);

		unsigned int u_mEnergy = u_energy;
		unsigned int u_mLastEnergyTime = u_lastEnergyTime;

		g_cProUserMissionAward.updateUserEnergyAndTime(u_mEnergy,u_mLastEnergyTime,u_userID);

		u_PBUser->set_energy(u_energy);
		u_PBUser->set_nextenergytime(u_NextEnergyTime);
		//u_PBUser->set_buyenergytime(q.getIntFileds("buyEnergyTime"));
		newAddSetSomeUserInfo(u_PBUser,u_userID);
		u_PBUser->set_diamondnumber(q.getIntFileds("diamondNumber"));
		u_PBUser->set_friendvalue(q.getIntFileds("friendValue"));
		u_PBUser->set_goldnumber(q.getIntFileds("goldNumber"));
		u_PBUser->set_leadervalue(q.getIntFileds("leaderValue"));
		if (g_cMagicServerDb.hasMissionFinished(u_userID))
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
		u_battleProgress = q.getStringFileds("battleProgress");
		q.finalize();
	}
	
	return true;
}

////更新体力和最新获取体力的时间戳
//bool cProcUserLogin::updateUserEnergyAndTime(const unsigned int& u_energy,const unsigned int& u_lastEnergyTime,const string& strMac)
//{
//	char buf[128] = {0};
//
//	sprintf(buf,"update m_gameUserInfo set energy = %d,lastEnergyTime=%d where macAddress = '%s';",u_energy,u_lastEnergyTime,strMac.c_str());
//	string strSql(buf);
//
//	if(g_cMagicServerDb.execSQL(strSql)!=0)
//	{
//		cerr<<"update failure:"<<strSql<<endl;
//		return false;
//	}
//	return true;
//}

//根据时间确定今天是星期几，时间格式2013 08 19
int cProcUserLogin::getWeekDayByTime(const string& u_strTime)
{
	int u_finalWeedDay = 0;
	//int uWeekDay[] = {L_SUNDAY,L_MONDAY,L_TUESDAY,L_WEDNESDAY,L_THURSDAY,L_FRIDAY,L_SATURDAY};
	int u_year = atoi((u_strTime.substr(0,4)).c_str());
	int u_month = atoi((u_strTime.substr(5,2)).c_str());
	int u_day = atoi((u_strTime.substr(8,2)).c_str());
	if(u_month == 1|| u_month == 2)
	{
		u_month += 12;
		u_year = u_year - 1;
	}
	//基姆拉尔森计算公式计算今天是星期几（0-6代表星期一到星期日）
	u_finalWeedDay = (u_day+2*u_month+3*(u_month+1)/5+u_year+u_year/4-u_year/100+u_year/400)%7;
	//做一次转换（0-6代表星期日到星期一）
	u_finalWeedDay += 1;
	if(u_finalWeedDay == 7)
	{
		u_finalWeedDay = 0;
	}
	return u_finalWeedDay;
}

//根据今天是星期几分配任务
void cProcUserLogin::getBattleIdListByWeekDay(const int& u_weekDay,weekDayMissionList& u_weekDayMissionList)
{
	u_weekDayMissionList.clear();
	if (u_weekDay == L_MONDAY)
	{
		u_weekDayMissionList.push_back("1000-1");
		u_weekDayMissionList.push_back("1001-1");
		u_weekDayMissionList.push_back("1002-1");
	}
	else if (u_weekDay == L_TUESDAY)
	{
		//抽奖活动副本ID暂定为10000
		u_weekDayMissionList.push_back("10000");
	}
	else if (u_weekDay == L_WEDNESDAY)
	{
		u_weekDayMissionList.push_back("1003-1");
		u_weekDayMissionList.push_back("1004-1");
		u_weekDayMissionList.push_back("1005-1");
	}
	else if (u_weekDay == L_THURSDAY)
	{
		u_weekDayMissionList.push_back("1006-1");
		u_weekDayMissionList.push_back("1007-1");
		u_weekDayMissionList.push_back("1008-1");
	}
	else if (u_weekDay == L_FRIDAY)
	{
		u_weekDayMissionList.push_back("1009-1");
		u_weekDayMissionList.push_back("1010-1");
		u_weekDayMissionList.push_back("1011-1");
	}
	else if (u_weekDay == L_SATURDAY || u_weekDay == L_SUNDAY)
	{
		u_weekDayMissionList.push_back("1012-1");
		u_weekDayMissionList.push_back("1013-1");
		u_weekDayMissionList.push_back("1014-1");
	}
}

//获取活动副本信息
bool cProcUserLogin::getActiveBattleInfo(PBBattle* u_PBBattle,const unsigned int& u_userId)
{
	/*long u_timeStamp = GetTimeStamp();
	string u_strTime = getStringTime(u_timeStamp,0);
	int u_weekDay = getWeekDayByTime(u_strTime);*/
	//weekDayMissionList u_weekDayMissionList;
	//getBattleIdListByWeekDay(u_weekDay,u_weekDayMissionList);
	/*if (u_weekDayMissionList.size() > 0)
	{
		weekDayMissionList::iterator it = u_weekDayMissionList.begin();
		int i = 0;
		while (it != u_weekDayMissionList.end())
		{
			u_PBBattle->add_allactivitypveid(*it);
			u_PBBattle->set_allactivitypveid(i,*it);
			it++;
		}
	}
	return true;*/
	char buf[256] = {0};
	sprintf(buf,"select * from m_useractivebattleinfo%d where userId = %d;",u_userId%ACTIVE_BATTLE_COUNT,u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getActiveBattleInfo errror,strSql:%s\n",strSql.c_str());
		q.finalize();
		//当没有活动副本时，加上下面这句，程序不会崩
		PBActivityBattle* u_PBActivityBattle;
		u_PBActivityBattle = u_PBBattle->add_allactivitybattle();
		return false;
	}
	while(!q.eof())
	{
		PBActivityBattle* u_PBActivityBattle;
		u_PBActivityBattle = u_PBBattle->add_allactivitybattle();
		u_PBActivityBattle->set_activitybattleid(q.getStringFileds("activeBattleId"));
		u_PBActivityBattle->set_fightcount(q.getIntFileds("activeBattleCount"));
		q.nextRow();
	}
	q.finalize();
	return true;
}

//获取用户所有卡牌信息
bool cProcUserLogin::getAllCardsInfo(PBAllCard* u_PBAllCard,const unsigned int& u_userID)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_userCardInfo%d where userId = %d;",u_userID%USER_CARD_TABLE_COUNT,u_userID);
	string strSql(buf);
	//MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	MysqlQuery q = g_cMagicServerDb.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getAllCardsInfo errror,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	else
	{
		while(!q.eof())
		{
			PBCard* u_PBCard;
			u_PBCard = u_PBAllCard->add_allcard();
			u_PBCard->set_itemid(q.getIntFileds("itemId"));
			
			u_PBCard->set_cardid(q.getStringFileds("cardId"));
			u_PBCard->set_cardlevel(q.getIntFileds("cardLevel"));
			u_PBCard->set_cardexp(q.getIntFileds("cardExp"));
			u_PBCard->set_skilllevel(q.getIntFileds("skillLevel"));
			u_PBCard->set_skillexp(q.getIntFileds("skillExp"));
			u_PBCard->set_battleposition(q.getIntFileds("battlePosition"));
			u_PBCard->set_vtlgrow(q.getIntFileds("VTLGrow"));
			u_PBCard->set_strgrow(q.getIntFileds("STRGrow"));
			u_PBCard->set_tghgrow(q.getIntFileds("TGHGrow"));
			u_PBCard->set_quigrow(q.getIntFileds("QUIGrow"));
			u_PBCard->set_mgcgrow(q.getIntFileds("MGCGrow"));
			q.nextRow();
		}
	}
	q.finalize();
	return true;
}

//获取用户所有任务信息
bool cProcUserLogin::getUserAllMissionsInfo(PBAllMission* u_PBAllMission,const unsigned int& u_userID)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_userMissionsInfo%d where userId = %d;",u_userID%USER_MISSION_TABLE_COUNT,u_userID);
	string strSql(buf);

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserAllMissionsInfo errror,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	else
	{
		while (!q.eof())
		{
			PBMission* u_PBMission;
			u_PBMission = u_PBAllMission->add_allmission();
			u_PBMission->set_missionid(q.getIntFileds("missionId"));
			int u_isMissionFinished = q.getIntFileds("isMissinFinished");
			u_PBMission->set_ismissinfinished(u_isMissionFinished);
			if (!u_isMissionFinished)
			{
				u_PBMission->set_missonprogress(q.getStringFileds("missionProgress"));
			}
			q.nextRow();
		}
	}
	q.finalize();
	return true;
}

//获取用户战斗卡牌信息
bool cProcUserLogin::getUserBattleCardInfo(PBBattleCard* u_PBBattleCard,const unsigned int& u_userID)
{
	//cerr<<"DDDDDD"<<endl;
	char buf[128] = {0};
	sprintf(buf,"select * from m_userCardInfo%d where userId = %d and battlePosition != %d;",u_userID%USER_CARD_TABLE_COUNT,u_userID,NOT_BATTLE_POSITION);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getBattleCardsInfo errror,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	else
	{
		while(!q.eof())
		{
			PBCard* u_PBCard;
			u_PBCard = u_PBBattleCard->add_battlecard();
			u_PBCard->set_itemid(q.getIntFileds("itemId"));
			u_PBCard->set_cardid(q.getStringFileds("cardId"));
			u_PBCard->set_cardlevel(q.getIntFileds("cardLevel"));
			u_PBCard->set_cardexp(q.getIntFileds("cardExp"));
			u_PBCard->set_skilllevel(q.getIntFileds("skillLevel"));
			u_PBCard->set_skillexp(q.getIntFileds("skillExp"));
			u_PBCard->set_battleposition(q.getIntFileds("battlePosition"));
			u_PBCard->set_vtlgrow(q.getIntFileds("VTLGrow"));
			u_PBCard->set_strgrow(q.getIntFileds("STRGrow"));
			u_PBCard->set_tghgrow(q.getIntFileds("TGHGrow"));
			u_PBCard->set_quigrow(q.getIntFileds("QUIGrow"));
			u_PBCard->set_mgcgrow(q.getIntFileds("MGCGrow"));
	
			q.nextRow();
		}
	}
	q.finalize();
	return true;
}

//获取用户装备信息
void cProcUserLogin::getUserGemInfo(PBAllGem* u_PBAllGem,const unsigned int& u_userID)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_userequipinfo%d where userId = %d;",u_userID%USER_EQUIP_TABLE_COUNT,u_userID);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("getUserGemInfo getNothing,strSql:%s\n",strSql.c_str());
		q.finalize();
		return;
	}
	else
	{
		while (!q.eof())
		{
			PBGem* u_PBGem;
			u_PBGem = u_PBAllGem->add_allgem();
			u_PBGem->set_gemid(q.getStringFileds("equipMentId"));
			u_PBGem->set_gemnumber(q.getIntFileds("count"));
			q.nextRow();
		}
	}
	q.finalize();
	return;
}

//获取登录类型 1,帐号登录，2，mac地址登录
int cProcUserLogin::getLoginType(const PBLoginSend& u_PBLoginSend)
{
	int u_loginType = 0;
	if ((u_PBLoginSend.useraccount()).size() < 32 && (u_PBLoginSend.useraccount()).size() > 0)
	{
		u_loginType = 1;
	}
	else if((u_PBLoginSend.macaddress()).size() < 32 && (u_PBLoginSend.macaddress()).size() > 0)
	{
		u_loginType = 2;
	}
	return u_loginType;
}

//获得上一次的登录时间和连续登录天数
bool cProcUserLogin::getPreLoginTimeAndDays(const unsigned int& u_userId,string& u_loginTime,int& u_loginDays)
{
	char buf[128] = {0};
	sprintf(buf,"select logTime,loginDays from m_gameuserinfo where userID=%d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getLoginTimeAndDays error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_loginTime = q.getStringFileds("logTime");
	u_loginDays = q.getIntFileds("loginDays");
	q.finalize();
	return true;
}

//设置连续登录天数
bool cProcUserLogin::setLoginDays(const string& u_strPreLoginTime,const unsigned long& u_nowStamp,int& u_LoginDays)
{
	unsigned long u_preLoginStamp = MakeTime(u_strPreLoginTime);
	int u_loginDiff = (u_nowStamp - u_preLoginStamp);
	if (u_loginDiff > 172800)
	{
		u_LoginDays = 1;
	}
	else
	{
		int n_preWeekDay = 0,n_nowWeekDay = 0;
		string n_strPreTime = getStringTime(u_preLoginStamp,0);
		n_preWeekDay = getWeekDayByTime(n_strPreTime);

		string n_strNowTime = getStringTime(u_nowStamp,0);
		n_nowWeekDay = getWeekDayByTime(n_strNowTime);
		g_cMyGlog.debugLog("n_preWeekDay:%d,n_nowWeekDay:%d,nLoginDays:%d\n",n_preWeekDay,n_nowWeekDay,u_LoginDays);
		if (n_nowWeekDay == 0)
		{
			if (n_preWeekDay == 6)
			{
				u_LoginDays += 1;
			}
			else if (n_preWeekDay == 0)
			{
				u_LoginDays = 1;
				return false;
			}
			else
			{
				u_LoginDays = 1;
			}
		}
		else if (n_nowWeekDay == 1)
		{
			if (n_preWeekDay == 6)
			{
				u_LoginDays = 1;
			}
			else if (n_preWeekDay == 1)
			{
				u_LoginDays = 1;
				return false;
			}
			else
			{
				u_LoginDays += 1;
			}
		}
		else
		{
			int n_weekDayDiff = n_nowWeekDay - n_preWeekDay;

			if (n_weekDayDiff == 0)
			{
				u_LoginDays = 1;
				return false;
			}
			else if (n_weekDayDiff == 1)
			{
				u_LoginDays += 1;
			}
			else if (n_weekDayDiff >= 2)
			{
				u_LoginDays = 1;
			}
		}
		
	}
	g_cMyGlog.debugLog("u_logDays:%d\n",u_LoginDays);
	if (u_LoginDays > LOGIN_MISSION_MAX_ID)
	{
		u_LoginDays = 1;
	}
	g_cMyGlog.debugLog("u_logDays:%d\n",u_LoginDays);
	return true;
	////一天之内登录的情况
	//if (u_loginDiff == 0)
	//{
	//	if (u_LoginDays == 0)
	//	{
	//		u_LoginDays = 1;
	//		return true;
	//	}
	//	return false;
	//}
	////连续两天登录的情况
	//else if (u_loginDiff == 1)
	//{
	//	u_LoginDays += 1;
	//	if (u_LoginDays > LOGIN_MISSION_MAX_ID)
	//	{
	//		u_LoginDays = 1;
	//	}
	//	return true;
	//}
	////非连续两天登录的情况
	//else if (u_loginDiff >= 2)
	//{
	//	u_LoginDays = 1;
	//	return true;
	//}
	//return false;
}

//更新登录时间，连续登录天数，奖励状态
void cProcUserLogin::updateLoginDaysAndAwardStatus(const unsigned int& u_userId,const string& u_strLoginTime,\
								   const int& u_logDays,bool isAwarded)
{
	char buf[256] = {0};
	sprintf(buf,"update m_gameuserinfo set logTime = '%s',loginDays = %d,isLoginAwarded = %d\
				where userID=%d;",u_strLoginTime.c_str(),u_logDays,isAwarded,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateLoginDaysAndAwardStatus error,strSql:%s\n",strSql.c_str());
	}
}

//设置登录任务奖励相关
void cProcUserLogin::setLoginAward(const unsigned int& u_userId)
{
	string u_PreLoginTime = "";
	int u_loginDays = 0;
	//cerr<<"AAAAA"<<endl;
	if (getPreLoginTimeAndDays(u_userId,u_PreLoginTime,u_loginDays))
	{
		unsigned long u_nowStamp = GetTimeStamp();
		//cerr<<"BBBBBB"<<endl;
		if (setLoginDays(u_PreLoginTime,u_nowStamp,u_loginDays))
		{
			//cerr<<"CCCCC loginDays:"<<u_loginDays<<endl;
			string u_loginTime = GetTime(u_nowStamp);
			updateLoginDaysAndAwardStatus(u_userId,u_loginTime,u_loginDays,false);
		}
	}
}

//根据连续登录天数，初始化登录任务信息
bool cProcUserLogin::getLoginMissionInfo(const int& u_logDays,LoginMissinInfo& u_LoginMissinInfo,const int& isAwarded)
{
	char buf[12] = {0};
	if (u_logDays == FIRST_LOGIN_MISSION_ID)
	{
		if (isAwarded == 0)
		{
			sprintf(buf,"%d/%d",FIRST_LOGIN_MISSION_ID,FIRST_LOGIN_MISSION_ID);
			u_LoginMissinInfo.missionId1 = FIRST_LOGIN_MISSION_ID;
			u_LoginMissinInfo.missionFinished1 = true;
			u_LoginMissinInfo.missionProgress1 += buf;
		}

		/*memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FIRST_LOGIN_MISSION_ID,SECOND_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId2 = SECOND_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished2 = false;
		u_LoginMissinInfo.missionProgress2 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FIRST_LOGIN_MISSION_ID,THIRD_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId3 = THIRD_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished3 = false;
		u_LoginMissinInfo.missionProgress3 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FIRST_LOGIN_MISSION_ID,FOURTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId4 = FOURTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished4 = false;
		u_LoginMissinInfo.missionProgress4 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FIRST_LOGIN_MISSION_ID,FIFTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId5 = FIFTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished5 = false;
		u_LoginMissinInfo.missionProgress5 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FIRST_LOGIN_MISSION_ID,SIXTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId6 = SIXTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished6 = false;
		u_LoginMissinInfo.missionProgress6 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FIRST_LOGIN_MISSION_ID,SEVENTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId7 = SEVENTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished7 = false;
		u_LoginMissinInfo.missionProgress7 += buf;*/

		return true;
	}
	else if (u_logDays == SECOND_LOGIN_MISSION_ID)
	{
		/*sprintf(buf,"%d/%d",FIRST_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId1 = FIRST_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished1 = false;
		u_LoginMissinInfo.missionProgress1 += buf;*/

		if (isAwarded == 0)
		{
			memset(buf,0,sizeof(buf));
			sprintf(buf,"%d/%d",SECOND_LOGIN_MISSION_ID,SECOND_LOGIN_MISSION_ID);
			u_LoginMissinInfo.missionId2 = SECOND_LOGIN_MISSION_ID;
			u_LoginMissinInfo.missionFinished2 = true;
			u_LoginMissinInfo.missionProgress2 += buf;
		}
		

		/*memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",SECOND_LOGIN_MISSION_ID,THIRD_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId3 = THIRD_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished3 = false;
		u_LoginMissinInfo.missionProgress3 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",SECOND_LOGIN_MISSION_ID,FOURTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId4 = FOURTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished4 = false;
		u_LoginMissinInfo.missionProgress4 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",SECOND_LOGIN_MISSION_ID,FIFTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId5 = FIFTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished5 = false;
		u_LoginMissinInfo.missionProgress5 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",SECOND_LOGIN_MISSION_ID,SIXTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId6 = SIXTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished6 = false;
		u_LoginMissinInfo.missionProgress6 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",SECOND_LOGIN_MISSION_ID,SEVENTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId7 = SEVENTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished7 = false;
		u_LoginMissinInfo.missionProgress7 += buf;*/

		return true;
	}
	else if (u_logDays == THIRD_LOGIN_MISSION_ID)
	{
		/*sprintf(buf,"%d/%d",SECOND_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId1 = FIRST_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished1 = false;
		u_LoginMissinInfo.missionProgress1 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FIRST_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId2 = SECOND_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished2 = false;
		u_LoginMissinInfo.missionProgress2 += buf;*/

		if (isAwarded == 0)
		{
			memset(buf,0,sizeof(buf));
			sprintf(buf,"%d/%d",THIRD_LOGIN_MISSION_ID,THIRD_LOGIN_MISSION_ID);
			u_LoginMissinInfo.missionId3 = THIRD_LOGIN_MISSION_ID;
			u_LoginMissinInfo.missionFinished3 = true;
			u_LoginMissinInfo.missionProgress3 += buf;
		}

		/*memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",THIRD_LOGIN_MISSION_ID,FOURTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId4 = FOURTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished4 = false;
		u_LoginMissinInfo.missionProgress4 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",THIRD_LOGIN_MISSION_ID,FIFTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId5 = FIFTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished5 = false;
		u_LoginMissinInfo.missionProgress5 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",THIRD_LOGIN_MISSION_ID,SIXTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId6 = SIXTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished6 = false;
		u_LoginMissinInfo.missionProgress6 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",THIRD_LOGIN_MISSION_ID,SEVENTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId7 = SEVENTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished7 = false;
		u_LoginMissinInfo.missionProgress7 += buf;*/

		return true;
	}
	else if (u_logDays == FOURTH_LOGIN_MISSION_ID)
	{
		/*sprintf(buf,"%d/%d",THIRD_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId1 = FIRST_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished1 = false;
		u_LoginMissinInfo.missionProgress1 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",SECOND_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId2 = SECOND_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished2 = false;
		u_LoginMissinInfo.missionProgress2 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FIRST_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId3 = THIRD_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished3 = false;
		u_LoginMissinInfo.missionProgress3 += buf;*/

		if (isAwarded == 0)
		{
			memset(buf,0,sizeof(buf));
			sprintf(buf,"%d/%d",FOURTH_LOGIN_MISSION_ID,FOURTH_LOGIN_MISSION_ID);
			u_LoginMissinInfo.missionId4 = FOURTH_LOGIN_MISSION_ID;
			u_LoginMissinInfo.missionFinished4 = true;
			u_LoginMissinInfo.missionProgress4 += buf;
		}

		/*memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FOURTH_LOGIN_MISSION_ID,FIFTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId5 = FIFTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished5 = false;
		u_LoginMissinInfo.missionProgress5 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FOURTH_LOGIN_MISSION_ID,SIXTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId6 = SIXTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished6 = false;
		u_LoginMissinInfo.missionProgress6 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FOURTH_LOGIN_MISSION_ID,SEVENTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId7 = SEVENTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished7 = false;
		u_LoginMissinInfo.missionProgress7 += buf;*/

		return true;
	}
	else if (u_logDays == FIFTH_LOGIN_MISSION_ID)
	{
		/*sprintf(buf,"%d/%d",FOURTH_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId1 = FIRST_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished1 = false;
		u_LoginMissinInfo.missionProgress1 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",THIRD_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId2 = SECOND_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished2 = false;
		u_LoginMissinInfo.missionProgress2 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",SECOND_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId3 = THIRD_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished3 = false;
		u_LoginMissinInfo.missionProgress3 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FIRST_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId4 = FOURTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished4 = false;
		u_LoginMissinInfo.missionProgress4 += buf;*/

		if (isAwarded == 0)
		{
			memset(buf,0,sizeof(buf));
			sprintf(buf,"%d/%d",FIFTH_LOGIN_MISSION_ID,FIFTH_LOGIN_MISSION_ID);
			u_LoginMissinInfo.missionId5 = FIFTH_LOGIN_MISSION_ID;
			u_LoginMissinInfo.missionFinished5 = true;
			u_LoginMissinInfo.missionProgress5 += buf;
		}

		/*memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FIFTH_LOGIN_MISSION_ID,SIXTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId6 = SIXTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished6 = false;
		u_LoginMissinInfo.missionProgress6 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FIFTH_LOGIN_MISSION_ID,SEVENTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId7 = SEVENTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished7 = false;
		u_LoginMissinInfo.missionProgress7 += buf;*/

		return true;
	}
	else if (u_logDays == SIXTH_LOGIN_MISSION_ID)
	{
		/*sprintf(buf,"%d/%d",FIFTH_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId1 = FIRST_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished1 = false;
		u_LoginMissinInfo.missionProgress1 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FOURTH_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId2 = SECOND_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished2 = false;
		u_LoginMissinInfo.missionProgress2 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",THIRD_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId3 = THIRD_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished3 = false;
		u_LoginMissinInfo.missionProgress3 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",SECOND_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId4 = FOURTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished4 = false;
		u_LoginMissinInfo.missionProgress4 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FIRST_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId5 = FIFTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished5 = false;
		u_LoginMissinInfo.missionProgress5 += buf;*/

		if (isAwarded == 0)
		{
			memset(buf,0,sizeof(buf));
			sprintf(buf,"%d/%d",SIXTH_LOGIN_MISSION_ID,SIXTH_LOGIN_MISSION_ID);
			u_LoginMissinInfo.missionId6 = SIXTH_LOGIN_MISSION_ID;
			u_LoginMissinInfo.missionFinished6 = true;
			u_LoginMissinInfo.missionProgress6 += buf;
		}

		/*memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",SIXTH_LOGIN_MISSION_ID,SEVENTH_LOGIN_MISSION_ID);
		u_LoginMissinInfo.missionId7 = SEVENTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished7 = false;
		u_LoginMissinInfo.missionProgress7 += buf;*/

		return true;
	}
	else if (u_logDays == SEVENTH_LOGIN_MISSION_ID)
	{
		/*sprintf(buf,"%d/%d",SIXTH_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId1 = FIRST_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished1 = false;
		u_LoginMissinInfo.missionProgress1 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FIFTH_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId2 = SECOND_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished2 = false;
		u_LoginMissinInfo.missionProgress2 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FIFTH_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId3 = THIRD_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished3 = false;
		u_LoginMissinInfo.missionProgress3 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",THIRD_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId4 = FOURTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished4 = false;
		u_LoginMissinInfo.missionProgress4 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",SECOND_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId5 = FIFTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished5 = false;
		u_LoginMissinInfo.missionProgress5 += buf;

		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",FIRST_LOGIN_MISSION_ID,LOGIN_MISSION_MAX_ID);
		u_LoginMissinInfo.missionId6 = SIXTH_LOGIN_MISSION_ID;
		u_LoginMissinInfo.missionFinished6 = false;
		u_LoginMissinInfo.missionProgress6 += buf;*/

		if (isAwarded == 0)
		{
			memset(buf,0,sizeof(buf));
			sprintf(buf,"%d/%d",SEVENTH_LOGIN_MISSION_ID,SEVENTH_LOGIN_MISSION_ID);
			u_LoginMissinInfo.missionId7 = SEVENTH_LOGIN_MISSION_ID;
			u_LoginMissinInfo.missionFinished7 = true;
			u_LoginMissinInfo.missionProgress7 += buf;
		}

		return true;
	}
	return false;
}

//获取玩家连续登录天数
bool cProcUserLogin::getLoginDays(const unsigned int& u_userId,int& u_logDays,int& isAwarded)
{
	char buf[128] = {0};
	sprintf(buf,"select loginDays,isLoginAwarded from m_gameuserinfo where userID=%d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getLoginDays111 error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	else
	{
		u_logDays = q.getIntFileds("loginDays");
		isAwarded = q.getIntFileds("isLoginAwarded");
	}
	q.finalize();
	return true;
}

//填充登录任务信息
void cProcUserLogin::fillPBAllMission(PBAllMission* u_PBAllMission,const LoginMissinInfo& u_LoginMissinInfo)
{
	PBMission* u_PBMission;
	if (u_LoginMissinInfo.missionId1 >= LOGIN_MISSION_MIN_ID && u_LoginMissinInfo.missionId1 <= LOGIN_MISSION_MAX_ID)
	{
		u_PBMission = u_PBAllMission->add_allmission();
		u_PBMission->set_missionid(u_LoginMissinInfo.missionId1);
		u_PBMission->set_missonprogress(u_LoginMissinInfo.missionProgress1);
		u_PBMission->set_ismissinfinished(u_LoginMissinInfo.missionFinished1);
	}

	if (u_LoginMissinInfo.missionId2 >= LOGIN_MISSION_MIN_ID && u_LoginMissinInfo.missionId2 <= LOGIN_MISSION_MAX_ID)
	{
		u_PBMission = u_PBAllMission->add_allmission();
		u_PBMission->set_missionid(u_LoginMissinInfo.missionId2);
		u_PBMission->set_missonprogress(u_LoginMissinInfo.missionProgress2);
		u_PBMission->set_ismissinfinished(u_LoginMissinInfo.missionFinished2);
	}
	
	if (u_LoginMissinInfo.missionId3 >= LOGIN_MISSION_MIN_ID && u_LoginMissinInfo.missionId3 <= LOGIN_MISSION_MAX_ID)
	{
		u_PBMission = u_PBAllMission->add_allmission();
		u_PBMission->set_missionid(u_LoginMissinInfo.missionId3);
		u_PBMission->set_missonprogress(u_LoginMissinInfo.missionProgress3);
		u_PBMission->set_ismissinfinished(u_LoginMissinInfo.missionFinished3);
	}

	if (u_LoginMissinInfo.missionId4 >= LOGIN_MISSION_MIN_ID && u_LoginMissinInfo.missionId4 <= LOGIN_MISSION_MAX_ID)
	{
		u_PBMission = u_PBAllMission->add_allmission();
		u_PBMission->set_missionid(u_LoginMissinInfo.missionId4);
		u_PBMission->set_missonprogress(u_LoginMissinInfo.missionProgress4);
		u_PBMission->set_ismissinfinished(u_LoginMissinInfo.missionFinished4);
	}

	if (u_LoginMissinInfo.missionId5 >= LOGIN_MISSION_MIN_ID && u_LoginMissinInfo.missionId5 <= LOGIN_MISSION_MAX_ID)
	{
		u_PBMission = u_PBAllMission->add_allmission();
		u_PBMission->set_missionid(u_LoginMissinInfo.missionId5);
		u_PBMission->set_missonprogress(u_LoginMissinInfo.missionProgress5);
		u_PBMission->set_ismissinfinished(u_LoginMissinInfo.missionFinished5);
	}
	if (u_LoginMissinInfo.missionId6 >= LOGIN_MISSION_MIN_ID && u_LoginMissinInfo.missionId6 <= LOGIN_MISSION_MAX_ID)
	{
		u_PBMission = u_PBAllMission->add_allmission();
		u_PBMission->set_missionid(u_LoginMissinInfo.missionId6);
		u_PBMission->set_missonprogress(u_LoginMissinInfo.missionProgress6);
		u_PBMission->set_ismissinfinished(u_LoginMissinInfo.missionFinished6);
	}
	if (u_LoginMissinInfo.missionId7 >= LOGIN_MISSION_MIN_ID && u_LoginMissinInfo.missionId7 <= LOGIN_MISSION_MAX_ID)
	{
		u_PBMission = u_PBAllMission->add_allmission();
		u_PBMission->set_missionid(u_LoginMissinInfo.missionId7);
		u_PBMission->set_missonprogress(u_LoginMissinInfo.missionProgress7);
		u_PBMission->set_ismissinfinished(u_LoginMissinInfo.missionFinished7);
	}
}

//处理登录任务
bool cProcUserLogin::procLoginMissions(PBAllMission* u_PBAllMission,const unsigned int& u_userId,bool& isLogMissionFinished)
{
	int u_logDays = 0;
	int u_isAwarded = 0;
	setLoginAward(u_userId);
	//cerr<<"1111"<<endl;
	if (getLoginDays(u_userId,u_logDays,u_isAwarded))
	{
		if (u_isAwarded == 1)
		{
			return true;
		}
		//cerr<<"2222 u_logDays:"<<u_logDays<<endl;
		LoginMissinInfo u_LoginMissinInfo;
		if (getLoginMissionInfo(u_logDays,u_LoginMissinInfo,u_isAwarded))
		{
			//cerr<<"33333"<<endl;
			fillPBAllMission(u_PBAllMission,u_LoginMissinInfo);
			isLogMissionFinished = true;
			return true;
		}
	}
	return false;
}

//获取非登陆任务信息
void cProcUserLogin::getNotLoginMissions(PBAllMission* u_PBAllMission,const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_usermissionsinfo%d where userId = %d;",u_userId%USER_MISSION_TABLE_COUNT,u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getNotLoginMissions getnothing!!!,strSql:%s\n",strSql.c_str());
		q.finalize();
		return;
	}
	while (!q.eof())
	{
		PBMission* u_PBMission;
		u_PBMission = u_PBAllMission->add_allmission();
		u_PBMission->set_missionid(q.getIntFileds("missionId"));
		if (q.getIntFileds("isFinished") == 1)
		{
			u_PBMission->set_ismissinfinished(true);
		}
		else
		{
			u_PBMission->set_ismissinfinished(false);
		}
		q.nextRow();
	}
	q.finalize();
	return;
}

//用户登录处理
int cProcUserLogin::procUserLogin(PBLoginReceive& u_PBLoginReceive,string& strProtobufData)
{
	PBLoginSend u_PBLoginSend;
	//cerr<<"size:"<<strProtobufData.size()<<endl;
	//cerr<<strProtobufData<<endl;
	if (!u_PBLoginSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procUserLogin ParseFromString error\n");
		return false;
	}
	string u_strBattleProgresss("");
	unsigned int u_userID = 0;
	//string strMacAddress = u_PBLoginSend.macaddress();

	//添加用户信息
	PBUser* u_PBUser;
	u_PBUser = u_PBLoginReceive.mutable_user();

	if (!getUserInfoByMacAddress(u_PBUser,u_PBLoginSend,u_strBattleProgresss,u_userID))
	{
		g_cMyGlog.debugLog("procUserLogin getUserInfo error\n");
		//用户名或密码不存在
		u_PBLoginReceive.set_result(LOGIN_USERORPASSWD_NOT_EXIST);
		return PROC_SUCCESS;
	}
	//登录成功
	u_PBLoginReceive.set_result(LOGIN_SUCCESS);
	//添加副本信息
	PBBattle* u_PBBattle;
	u_PBBattle = u_PBLoginReceive.mutable_battle();

	u_PBBattle->set_pveprogress(u_strBattleProgresss);
	if (!getActiveBattleInfo(u_PBBattle,u_userID))
	{
		g_cMyGlog.errorlogRecord("procUserLogin getUserBattleInfo error\n");
	}

	//添加所有卡牌信息
	PBAllCard* u_PBAllCard;
	u_PBAllCard = u_PBLoginReceive.mutable_cards();

	if (!getAllCardsInfo(u_PBAllCard,u_userID))
	{
		g_cMyGlog.errorlogRecord("procUserLogin getAllCardsInfo error\n");
	}
	

	//添加所有任务信息
	PBAllMission* u_PBAllMission;
	u_PBAllMission = u_PBLoginReceive.mutable_missions();
	bool isLogMissionFinished = false;
	if (!procLoginMissions(u_PBAllMission,u_userID,isLogMissionFinished))
	{
		g_cMyGlog.errorlogRecord("procUserLogin procLoginMissions error\n");
	}
	if (isLogMissionFinished)
	{
		u_PBUser->set_hasmissionfinished(isLogMissionFinished);
	}
	getNotLoginMissions(u_PBAllMission,u_userID);
	/*if (!getUserAllMissionsInfo(u_PBAllMission,u_userID))
	{
		cerr<<"procUserLogin getUserAllMissionsInfo error"<<endl;
	}*/

	//添加战斗卡牌信息
	PBBattleCard* u_PBBattleCard;
	u_PBBattleCard = u_PBLoginReceive.mutable_battlecards();
	//cerr<<"FFFFFFFFFF"<<endl;
	if (!getUserBattleCardInfo(u_PBBattleCard,u_userID))
	{
		g_cMyGlog.errorlogRecord("procUserLogin getUserBattleCardInfo error\n");
	}

	//添加装备信息
	PBAllGem* u_PBAllGem;
	u_PBAllGem = u_PBLoginReceive.mutable_gems();
	getUserGemInfo(u_PBAllGem,u_userID);
	

	//添加token信息
	string u_strToken("");
	g_cMagicServerDb.PBLoginReceive_setToken(u_strToken,u_userID);

	if (u_strToken.size() <= 0)
	{
		g_cMyGlog.errorlogRecord("procUserLogin generateToken error\n");
		return PROC_FAILURE;
	}
	u_PBLoginReceive.set_token(u_strToken);

	int u_userFightCapacity = 0;
	int u_userCapitorCardFightCapacity = 0;
	getUserFightCapacity(u_userID,u_userFightCapacity,\
		u_userCapitorCardFightCapacity);
	u_PBLoginReceive.set_fightcapacity(u_userFightCapacity);

	/*if (!updateUserLogtime(GetTimeStamp(),u_userID))
	{
		cerr<<"updateUserLogtime error"<<endl;
	}
	*/
	return PROC_SUCCESS;
	
}