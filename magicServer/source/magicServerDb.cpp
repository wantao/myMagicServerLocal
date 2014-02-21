#include "../include/magicServerDb.h"
#include "../include/structDef.h"
#include "../include/common.h"
#include "../include/define.h"
#include "../include/procUserCardLevelUp.h"
#include "../include/errorCode.h"
#include "../include/procUserLogin.h"
#include "../include/myGlog.h"


cMagicServerDb g_cMagicServerDb;
cMagicServerDb g_cMagicServerDbRead;

cMagicServerDb::cMagicServerDb()
{
	m_bConnect = false;
}
cMagicServerDb::~cMagicServerDb()
{

}

bool cMagicServerDb::Init()
{
	//连接参数
	initCondition(g_strDbHost,g_nDbPort,g_strDatabase,g_strUser,g_strPw);
	printf("g_strDbHost=%s,g_nDbPort=%d,g_strDatabase=%s,g_strUser=%s,g_strPw=%s\r\n",g_strDbHost.c_str(),g_nDbPort,g_strDatabase.c_str(),g_strUser.c_str(),g_strPw.c_str());
	//打开连接DB
	if(connectDB())
	{
		m_bConnect=true;
	}
	else
	{
		m_bConnect=false;
		return false;
	}
	return true;
}

//从数据库实例化
bool cMagicServerDb::DbReadInit()
{
	//连接参数
	initCondition(g_strDbReadHost,g_nDbPort,g_strDatabase,g_strUser,g_strPw);
	printf("g_strDbReadHost=%s,g_nDbPort=%d,g_strDatabase=%s,g_strUser=%s,g_strPw=%s\r\n",g_strDbReadHost.c_str(),g_nDbPort,g_strDatabase.c_str(),g_strUser.c_str(),g_strPw.c_str());
	//打开连接DB
	if(connectDB())
	{
		m_bConnect=true;
	}
	else
	{
		m_bConnect=false;
		return false;
	}
	return true;
}


void cMagicServerDb::unInit()
{
	if (m_bConnect)
	{
		closeDB();
		m_bConnect = false;
	}
}

/*
message PBSystemInfoReceive{
required string affiche = 1;//公告
repeated PBServer allServer = 2;//服务器信息
required string gameVersion = 3;//游戏版本
required string resourceVersion = 4;//资源版本
required string battleVersion = 5;//活动副本版本
}

*/
////////////得到PBSystemInfoReceive所需的东西/////begin

//填充PBSystemInfoReceive
void cMagicServerDb::setPBSystemInfoReceive(PBSystemInfoReceive& u_PBSystemInfoReceive)
{
	PBSystemInfoReceive_setAfficheInfo(u_PBSystemInfoReceive);
	PBSystemInfoReceive_setServerInfo(u_PBSystemInfoReceive);
	PBSystemInfoReceive_setGameVersion(u_PBSystemInfoReceive);
	PBSystemInfoReceive_setResourceVersion(u_PBSystemInfoReceive);
	PBSystemInfoReceive_setBattleVersion(u_PBSystemInfoReceive);
}

//获取公告信息
void cMagicServerDb::PBSystemInfoReceive_setAfficheInfo(PBSystemInfoReceive& u_PBSystemInfoReceive)
{
	string strSql = "select * from afficheInfo order by afficheID desc limit 1";

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	string strAfficheContents = "";
	if(!q.eof())
	{
		strAfficheContents = q.getStringFileds("afficheContents");
		u_PBSystemInfoReceive.set_affiche(strAfficheContents);
		q.nextRow();
	}
	q.finalize();
}
//获取服务器所有分区信息
void cMagicServerDb::PBSystemInfoReceive_setServerInfo(PBSystemInfoReceive& u_PBSystemInfoReceive)
{
	string strSql = "select * from allServerInfo;";
	string strServerInfo = "";
	string serverName = "";
	string serverUrl = "";
	PBServer *u_PBServer;
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	while(!q.eof())
	{
		serverName = q.getStringFileds("serverName");
		serverUrl = q.getStringFileds("serverUrl");
		u_PBServer = u_PBSystemInfoReceive.add_allserver();
		u_PBServer->set_servername(serverName);
		u_PBServer->set_serverurl(serverUrl);
		//暂定60000
		u_PBServer->set_serverport(60000);
		q.nextRow();
	}
	q.finalize();

}


//获取游戏版本信息
void cMagicServerDb::PBSystemInfoReceive_setGameVersion(PBSystemInfoReceive& u_PBSystemInfoReceive)
{
	string strSql = "select gameVersionName from gameVersionInfo order by gameVersionID desc limit 1";
	string strGameVersionName = "";
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		strGameVersionName = q.getStringFileds("gameVersionName");
		u_PBSystemInfoReceive.set_gameversion(strGameVersionName);
		
	}
	q.finalize();
}

//获取资源版本信息
void cMagicServerDb::PBSystemInfoReceive_setResourceVersion(PBSystemInfoReceive& u_PBSystemInfoReceive)
{
	string strSql = "select resourceVersionName from resourceVersionInfo order by resourceVersionID desc limit 1";
	string strResourceVersionName = "";
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		strResourceVersionName = q.getStringFileds("resourceVersionName");
		u_PBSystemInfoReceive.set_resourceversion(strResourceVersionName);
	}
	q.finalize();
}

//获取副本版本信息
void cMagicServerDb::PBSystemInfoReceive_setBattleVersion(PBSystemInfoReceive& u_PBSystemInfoReceive)
{
	string strSql = "select battleVersionName from battleVersionInfo order by battleVersionId desc limit 1";
	string strBattleVersionName = "";
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		strBattleVersionName = q.getStringFileds("battleVersionName");
		u_PBSystemInfoReceive.set_battleversion(strBattleVersionName);
	}
	q.finalize();
}
////////////得到PBSystemInfoReceive所需的东西/////end








//token值和用id插入表中
bool cMagicServerDb::InsertUserTokenInfo(string& userToken,int& userId)
{
	/*char buf[128]={0};
	sprintf(buf,"insert into m_userTokenInfo(userId,tokenValue) values(%d,%s)",userId,userToken.c_str());
	string strSql(buf);
	if (master_slave_exQuery(strSql) != 0)
	{
		cerr<<"InsertUserTokenInfo error:%s"<<strSql<<endl;
		return true;
	}*/
	return false;
}

//根据userToken查找对应的用户ID
int cMagicServerDb::getUserIdByUserToken(string& userToken)
{
	char buf[128] = {0};
	sprintf(buf,"select userId from m_userTokenInfo where tokenValue = '%s'",userToken.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		int userId = q.getIntFileds("tokenValue");
		q.finalize();
		return userId;
	}
	q.finalize();
	return 0;
}

//判断是否有任务完成
bool cMagicServerDb::hasMissionFinished(const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select missionId from m_userMissionsInfo%d where userId = %d and isFinished = 1;",\
		u_userId%USER_MISSION_TABLE_COUNT,u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("no missionFinished:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	q.finalize();
	return true;
}

//插入注册的新用户
int cMagicServerDb::PBLoginReceive_gernerateNewUser(PBUser* u_PBUser,const PBRegisterSend& u_PBRegisterSend,unsigned int& newUserId,const int& u_regType)
{
	char buf[512] = {0}; 
	unsigned int currentTime = GetTimeStamp();
	string strTime = GetTime(currentTime);
	//帐号注册
	if (u_regType == 1)
	{
		sprintf(buf,"insert into m_gameUserInfo(userID,userAccount,userPasswd,userName,macAddress,userLevel,\
					userExp,energy,buyEnergyTime,lastEnergyTime,diamondNumber,\
					friendValue,goldNumber,leaderValue,battleProgress,logTime,userTmpLevel,maxOwnCardCount,maxEnergy,playerProfession,plateform) \
					values(NULL,'%s','%s','%s','%s',%d,%d,%d,%d,\
					%d,%d,%d,%d,%d,'%s','%s',%d,%d,%d,%d,%d)",(u_PBRegisterSend.useraccount()).c_str(),\
					(u_PBRegisterSend.userpasswd()).c_str(),(u_PBRegisterSend.username()).c_str(),(u_PBRegisterSend.useraccount()).c_str(),\
					USERLEVEL,USEREXP,ENERGY,BUYENEYGYTIME,currentTime,DIAMONDNUMBER,
					0/*FRIENDVALUE*/,GOLDNUMBER,LEADERVALUE,INIT_BATTLE_PROGRESS,strTime.c_str(),USERLEVEL,\
					USER_INIT_MAX_OWN_CARDS,ENERGY,u_PBRegisterSend.captainindex(),u_PBRegisterSend.usersourcetype());
	}
	else if (u_regType == 2)//mac地址注册
	{
		if(isRegisterUserMacExist(u_PBRegisterSend.macaddress(),u_PBRegisterSend.usersourcetype()))
		{
			sprintf(buf,"update m_gameUserInfo set userName='%s' where macAddress='%s';",(u_PBRegisterSend.username()).c_str(),(u_PBRegisterSend.macaddress()).c_str());
		}
		else
		{
			sprintf(buf,"insert into m_gameUserInfo(userID,userAccount,userPasswd,userName,macAddress,userLevel,\
						userExp,energy,buyEnergyTime,lastEnergyTime,diamondNumber,\
						friendValue,goldNumber,leaderValue,battleProgress,logTime,userTmpLevel,maxOwnCardCount,maxEnergy,playerProfession,plateform) \
						values(NULL,'%s','%s','%s','%s',%d,%d,%d,%d,\
						%d,%d,%d,%d,%d,'%s','%s',%d,%d,%d,%d,%d)",(u_PBRegisterSend.macaddress()).c_str(),\
						(u_PBRegisterSend.userpasswd()).c_str(),(u_PBRegisterSend.username()).c_str(),(u_PBRegisterSend.macaddress()).c_str(),\
						USERLEVEL,USEREXP,ENERGY,BUYENEYGYTIME,currentTime,DIAMONDNUMBER,
						0/*FRIENDVALUE*/,GOLDNUMBER,LEADERVALUE,INIT_BATTLE_PROGRESS,strTime.c_str(),USERLEVEL,\
						USER_INIT_MAX_OWN_CARDS,ENERGY,u_PBRegisterSend.captainindex(),u_PBRegisterSend.usersourcetype());
		}
		
	}
	string strSql(buf);
	if(g_cMagicServerDb.execSQL(strSql,&newUserId)!=0)
	{
		g_cMyGlog.errorlogRecord("register failure:%s\n",strSql.c_str());
		return PROC_FAILURE;
	}
	u_PBUser->set_userid(newUserId);
	u_PBUser->set_username((u_PBRegisterSend.username()).c_str());
	u_PBUser->set_userlevel(USERLEVEL);
	u_PBUser->set_userexp(USEREXP);
	u_PBUser->set_energy(ENERGY);
	u_PBUser->set_nextenergytime(0);
	//u_PBUser->set_buyenergytime(BUYENEYGYTIME);
	//u_PBUser->set_buyenergytime(q.getIntFileds("buyEnergyTime"));
	newAddSetSomeUserInfo(u_PBUser,newUserId);
	u_PBUser->set_diamondnumber(DIAMONDNUMBER);
	u_PBUser->set_friendvalue(0);
	u_PBUser->set_goldnumber(GOLDNUMBER);
	u_PBUser->set_leadervalue(LEADERVALUE);
	u_PBUser->set_hasmissionfinished(true);
	u_PBUser->set_bagmaxcount(USER_INIT_MAX_OWN_CARDS);
	u_PBUser->set_energyagentiacount(0);
	u_PBUser->set_captainindex(u_PBRegisterSend.captainindex());
	return PROC_SUCCESS;
}


//判断注册用户帐号是否已经存在
bool cMagicServerDb::isRegisterUserAccountExist(const string& u_strUserAccount,const int& u_plateform)
{
	char buf[128] = {0};
	sprintf(buf,"select userID from m_gameUserInfo where userAccount = '%s' and plateform = %d;",u_strUserAccount.c_str(),u_plateform);
	string strSql(buf);

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		q.finalize();
		g_cMyGlog.errorlogRecord("userAccount has been registed,userAccount:%s\n",u_strUserAccount.c_str());
		return true;
	}
	q.finalize();
	return false;
}

//判断mac地址地址注册的用户是否已经存在
bool cMagicServerDb::isRegisterUserMacExist(const string& u_strUserMac,const int& u_plateform)
{
	char buf[128] = {0};
	sprintf(buf,"select userID from m_gameUserInfo where macAddress = '%s' and plateform=%d",u_strUserMac.c_str(),u_plateform);
	string strSql(buf);

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		q.finalize();
		g_cMyGlog.errorlogRecord("macAddress:%s has been registed\n",u_strUserMac.c_str());
		return true;
	}
	q.finalize();
	return false;
}

//判断用户昵称是否存在
bool cMagicServerDb::isRegisterUserNameExist(const string& u_strUserName)
{
	char buf[128] = {0};
	sprintf(buf,"select userName from m_gameUserInfo where userName = '%s'",u_strUserName.c_str());
	string strSql(buf);

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		q.finalize();
		g_cMyGlog.errorlogRecord("userName: %s has been registed\n",u_strUserName.c_str());
		return true;
	}
	q.finalize();
	return false;
}


//根据用户MacAddress和UserName生成新的用户信息 
//0:注册成功，1：此MacAddress已经注册，2：注册失败
int cMagicServerDb::PBLoginReceive_gernerateUserByMacAndUserName(PBUser* u_PBUser,const PBRegisterSend& u_PBRegisterSend,unsigned int& newUserId)
{
	int u_registType = 0;//注册类型，1，帐号注册，2，mac地址注册
	if ((u_PBRegisterSend.useraccount()).size() < 32 && (u_PBRegisterSend.useraccount()).size() > 0)
	{
		if (isRegisterUserAccountExist(u_PBRegisterSend.useraccount(),u_PBRegisterSend.usersourcetype()))
		{
			return K_REGIST_ACCOUNT_REPEAT;
		}
		u_registType = 1;
		if ((u_PBRegisterSend.username()).size() < 32 && (u_PBRegisterSend.username()).size() > 0)
		{
			if(isRegisterUserNameExist(u_PBRegisterSend.username()))
			{
				return K_REGIST_USERNAME_REPEAT;
			}
		}
		else 
		{
			g_cMyGlog.errorlogRecord("u_registType:%d,userName is too Long:%s\n",u_registType,(u_PBRegisterSend.username()).c_str());
			return PROC_FAILURE;
		}
	}
	else if ((u_PBRegisterSend.macaddress()).size() < 32 && (u_PBRegisterSend.macaddress()).size() > 0)
	{
		u_registType = 2;
		if ((u_PBRegisterSend.username()).size() < 32 && (u_PBRegisterSend.username()).size() > 0)
		{
			if(isRegisterUserNameExist(u_PBRegisterSend.username()))
			{
				return K_REGIST_USERNAME_REPEAT;
			}
		}
		else 
		{
			g_cMyGlog.errorlogRecord("u_registType:%d,userName is too Long:%s\n",u_registType,(u_PBRegisterSend.username()).c_str());
			return PROC_FAILURE;
		}
	}
	else
	{
		g_cMyGlog.errorlogRecord("unkown regist type\n");
		return PROC_FAILURE;
	}
	
	//产生一个新用户
	return PBLoginReceive_gernerateNewUser(u_PBUser,u_PBRegisterSend,newUserId,u_registType);

}

//获取活动副本信息
bool cMagicServerDb::PBLoginReceive_getActiveBattleInfo(PBBattle* u_PBBattle,const unsigned int& u_userId)
{
	g_cProcUserLogin.getActiveBattleInfo(u_PBBattle,u_userId);
	return true;
}

//插入初始卡牌信息
bool cMagicServerDb::insertInitCardInfo(PBCard* u_PBCard,const int& u_CardId,unsigned int& nNewUserId,MysqlQuery& q,int battlePosition)
{
	string cardId = q.getStringFileds("cardId");
	int VTLGrow = 0, STRGrow = 0, TGHGrow = 0, QUIGrow = 0, MGCGrow = 0;
	if (battlePosition == CAPITOR_POSITION)
	{
		VTLGrow = getMaxGrow(q.getIntFileds("VTLGrow"));
		STRGrow = getMaxGrow(q.getIntFileds("STRGrow"));
		TGHGrow = getMaxGrow(q.getIntFileds("TGHGrow"));
		QUIGrow = getMaxGrow(q.getIntFileds("QUIGrow"));
		MGCGrow = getMaxGrow(q.getIntFileds("MGCGrow"));
	}
	else
	{
		VTLGrow = getGrow(q.getIntFileds("VTLGrow"));
		STRGrow = getGrow(q.getIntFileds("STRGrow"));
		TGHGrow = getGrow(q.getIntFileds("TGHGrow"));
		QUIGrow = getGrow(q.getIntFileds("QUIGrow"));
		MGCGrow = getGrow(q.getIntFileds("MGCGrow"));
	}
	char buf[512] = {0};
	unsigned int itemId = 0;
	sprintf(buf,"insert into m_userCardInfo%d(itemId,userID,cardId,cardLevel,\
				cardExp,battlePosition,VTLGrow,STRGrow,TGHGrow,\
				QUIGrow,MGCGrow,skillExp,skillLevel,cardStarLevel) \
				values(NULL,%d,'%s',%d,%d,%d,%d,\
				%d,%d,%d,%d,%d,%d,%d)",nNewUserId%USER_CARD_TABLE_COUNT,nNewUserId,cardId.c_str(),1, \
				0,battlePosition,VTLGrow,STRGrow,TGHGrow, \
				QUIGrow,MGCGrow,0,1,q.getIntFileds("CardStarLevel"));
	string strSql(buf);
	if(g_cMagicServerDb.execSQL(strSql,&itemId)!=0)
	{
		g_cMyGlog.errorlogRecord("insertInitCardInfo insert card failure: cardId:%d\n",u_CardId);
		g_cMyGlog.errorlogRecord("insertInitCardInfo insert card failure sql:%s\n",strSql.c_str());
		return false;
	}
	else
	{
		//cerr<<"itemId:"<<itemId<<endl;
		u_PBCard->set_itemid(itemId);
		u_PBCard->set_cardid(cardId);
		u_PBCard->set_cardlevel(1);
		u_PBCard->set_cardexp(0);
		u_PBCard->set_skillexp(0);
		u_PBCard->set_skilllevel(1);
		u_PBCard->set_battleposition(battlePosition);
		u_PBCard->set_vtlgrow(VTLGrow);
		u_PBCard->set_strgrow(STRGrow);
		u_PBCard->set_tghgrow(TGHGrow);
		u_PBCard->set_quigrow(QUIGrow);
		u_PBCard->set_mgcgrow(MGCGrow);
	}
	return true;
}

//获取初始卡牌信息
bool cMagicServerDb::getInitCardInfoByCardId(PBCard* u_PBCard,const int& u_CardId,unsigned int& nNewUserId,int battlePosition)
{
	char bufTmp[5] = {0};
	if(u_CardId < 1000 && u_CardId > 0)
	{
		sprintf(bufTmp,"%03d",u_CardId);
	}
	else
	{
		sprintf(bufTmp,"%d",u_CardId);
	}
	string u_strCardId(bufTmp);
	char buf[128] = {0};
	sprintf(buf,"select * from cardInfo where cardId = '%s';",u_strCardId.c_str());
	string strSql(buf);
	
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("not find CardId:%s in table cardInfo\n",u_strCardId.c_str());
		q.finalize();
		return false;
	}
	else
	{
		if (!insertInitCardInfo(u_PBCard,u_CardId,nNewUserId,q,battlePosition))
		{
			q.finalize();
			return false;
		}
	}
	q.finalize();
	return true;

}

//根据职业编号获取队长卡牌ID
string cMagicServerDb::getCapitorCardByProfession(const int& u_playerProfessionIndex)
{
	string u_capitorCardId = "";
	char buf[128] = {0};
	sprintf(buf,"select roleCardId from reg_role_card_info where Id = %d;",u_playerProfessionIndex);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if(q.eof())
	{
		g_cMyGlog.errorlogRecord("getCapitorCardByProfession:index:%d,strSql:%s\n",u_playerProfessionIndex,strSql.c_str());
	}
	else
	{
		u_capitorCardId = q.getStringFileds("roleCardId");
	}
	q.finalize();
	return u_capitorCardId;
}

//填充PBAllCard
int cMagicServerDb::PBLoginReceive_setAndGetUserCardInfo(PBAllCard* u_PBAllCard,unsigned int& nNewUserId,const int& u_profession)
{
	//随机产生INIT_CARD_COUNT张卡牌
	/*mapRand mapRandObject;
	mapRand::iterator it;
	
	mapRandObject = generateIntFiled();*/
	//int u_cardId = atoi(u_strCardId.c_str());
	int u_result = 0;
	string u_strCardId = getCapitorCardByProfession(u_profession);
	//g_cMyGlog.errorlogRecord("u_profession:%d,u_strCardId:%s\n",u_profession,u_strCardId.c_str());
	if (u_strCardId.size() == 0)
	{
		u_result = -1;
		return u_result;
	}
	int u_cardId = atoi(u_strCardId.c_str());
	//插入玩家设定的队长卡牌
	PBCard* u_PBCard;
	u_PBCard = u_PBAllCard->add_allcard();
	getInitCardInfoByCardId(u_PBCard,u_cardId,nNewUserId,CAPITOR_POSITION);

	int i = 1;
	//for (it = mapRandObject.begin(); it != mapRandObject.end(); it++)
	for(i = 1; i <= 4; i++)
	{
		PBCard* u_PBCard;
		u_PBCard = u_PBAllCard->add_allcard();
		getInitCardInfoByCardId(u_PBCard,i,nNewUserId,i);
	}

	//再增加一张卡牌，卡牌id为‘2004’
	PBCard* u_PBCardAdd;
	u_PBCardAdd = u_PBAllCard->add_allcard();
	getInitCardInfoByCardId(u_PBCardAdd,2004,nNewUserId,-1);

	return u_result;
}

//填充PBAllMission
void cMagicServerDb::PBLoginReceive_setMissionsInfo(PBAllMission* u_PBAllMission,unsigned int& nNewUserId)
{

	char buf[256] = {0};
	//missionId > 7为非登陆任务,(69,70,71,72,73,74,75,76,77,78,79,80)为活动副本任务，活动副本任务循环出现
	sprintf(buf,"select * from missioninfo where Id > 7 and Id not in (69,70,71,72,73,74,75,76,77,78,79,80,91,92,93);");
	string strSql(buf);
	
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("no Data in missionsInfo");
		q.finalize();
		return;
	}
	else
	{
		while (!q.eof())
		{
			PBMission* u_PBMission;
			u_PBMission = u_PBAllMission->add_allmission();
			
			int nMissionId = q.getIntFileds("Id");
			int nMissionType = q.getIntFileds("ConditionType");
			string u_missionCondition = q.getStringFileds("ConditionValue");
			string u_hortationType = q.getStringFileds("HortationType");
			int u_hortationCount = q.getIntFileds("HortationValue");
			bool isFinished = false;
			
			//先插入信息，成功后再填充信息
			char buf[512] = {0};
			sprintf(buf,"insert into m_userMissionsInfo%d(userId,missionId,missinType,missionCondition,hortationType,hortationCount,isFinished) \
						values(%d,%d,%d,'%s','%s',%d,0)",nNewUserId%USER_MISSION_TABLE_COUNT,nNewUserId,\
						nMissionId,nMissionType,u_missionCondition.c_str(),u_hortationType.c_str(),u_hortationCount);
			string strSql(buf);
			unsigned int tmpId = 0;
			if(g_cMagicServerDb.execSQL(strSql,&tmpId)!=0)
			{
				g_cMyGlog.errorlogRecord("PBLoginReceive_setMissionsInfo insert Missions failure: userID:%d,MissionId:%d\n",\
					nNewUserId,nMissionId);
			}
			else
			{
				u_PBMission->set_missionid(nMissionId);
				u_PBMission->set_ismissinfinished(isFinished);
			}
			q.nextRow();
		}
		q.finalize();
		return;
	}

}


//填充战斗卡牌信息
void cMagicServerDb::PBLoginReceive_setBattleCardInfo(PBBattleCard* u_PBBattleCard,unsigned int& nNewUserId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_userCardInfo%d where userID = %d and battlePosition != %d \
				",nNewUserId%USER_CARD_TABLE_COUNT,nNewUserId,NOT_BATTLE_POSITION);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("no Data in m_userCardInfo%d\n",nNewUserId%USER_CARD_TABLE_COUNT);
		q.finalize();
		return;
	}
	else
	{
		while (!q.eof())
		{
			PBCard* u_PBCard;
			u_PBCard = u_PBBattleCard->add_battlecard();
			u_PBCard->set_itemid(q.getLongFileds("itemId"));
			u_PBCard->set_cardid(q.getStringFileds("cardId"));
			u_PBCard->set_cardlevel(q.getIntFileds("cardLevel"));
			u_PBCard->set_cardexp(q.getIntFileds("cardExp"));
			u_PBCard->set_skillexp(q.getIntFileds("skillExp"));
			u_PBCard->set_skilllevel(q.getIntFileds("skillLevel"));
			u_PBCard->set_battleposition(q.getIntFileds("battlePosition"));
			u_PBCard->set_vtlgrow(q.getIntFileds("VTLGrow"));
			u_PBCard->set_strgrow(q.getIntFileds("STRGrow"));
			u_PBCard->set_tghgrow(q.getIntFileds("TGHGrow"));
			u_PBCard->set_quigrow(q.getIntFileds("QUIGrow"));
			u_PBCard->set_mgcgrow(q.getIntFileds("MGCGrow"));
			q.nextRow();
		}
		q.finalize();
		return;
	}
}

bool cMagicServerDb::isExistToken(string& strToken)
{
	char buf[128] = {0};
	//先查找m_userTokenInfo中是否存在该strToken的userId
	sprintf(buf,"select userId from m_userTokenInfo where tokenValue = '%s';",strToken.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		q.finalize();
		return false;
	}
	q.finalize();
	return true;
}

bool cMagicServerDb::isExistUserId(unsigned int& nNewUserId)
{
	char buf[128] = {0};
	memset(buf,0,sizeof(buf));
	//查看有没有nNewUserId这个用户
	sprintf(buf,"select * from m_userTokenInfo where userId = %d;",nNewUserId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		q.finalize();
		return false;
	}
	q.finalize();
	return true;
}

bool cMagicServerDb::insertUserIdToken(string& strToken,unsigned int& nNewUserId)
{
	char buf[128] = {0};
	sprintf(buf,"insert into m_userTokenInfo(userId,tokenValue) values(%d,'%s');",nNewUserId,strToken.c_str());
	string strSql(buf);
	unsigned int u_id = 0;
	if(g_cMagicServerDb.execSQL(strSql,&u_id)!=0)
	{
		g_cMyGlog.errorlogRecord("insertUserIdToken failure: userId:%d,Token:%s\n",nNewUserId,strToken.c_str());
		return false;
	}
	return true;
}

bool cMagicServerDb::updateUserToken(string& strToken,unsigned int& nNewUserId)
{
	char buf[128] = {0};
	sprintf(buf,"update m_userTokenInfo set tokenValue = '%s' where userId = %d;",strToken.c_str(),nNewUserId);
	string strSql(buf);
	if(g_cMagicServerDb.execSQL(strSql)!=0)
	{
		g_cMyGlog.errorlogRecord("updateUserToken failure: userId:%d,Token:%s\n",nNewUserId,strToken.c_str());
		return false;
	}
	return true;
}


//填充token
void cMagicServerDb::PBLoginReceive_setToken(string& strTokenRet,unsigned int& nNewUserId)
{
	string strToken("");
	strToken = generateToken();
	//不存在该token
	if (!isExistToken(strToken))
	{
		//不存在个该userID
		if (!isExistUserId(nNewUserId))
		{
			//插入该记录
			insertUserIdToken(strToken,nNewUserId);
		}
		else//存在该用户id
		{
			//跟新该记录
			updateUserToken(strToken,nNewUserId);
		}
		strTokenRet = strToken;
		return;
	}
	else
	{
		PBLoginReceive_setToken(strTokenRet,nNewUserId);
	}
}

//填充好友信息
void cMagicServerDb::PBLoginReceive_setFriendInfo(PBAllFriend& u_PBAllFriend,unsigned int& nNewUserId)
{

	char buf[128] = {0};
	//在m_userFriendInfo(nNewUserId%20)查找用户nNewUserId的好友信息
	sprintf(buf,"select * from m_userFriendInfo%d where userId=%d",nNewUserId%20,nNewUserId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("no Data in m_userFriendInfo%d in userId=%d\n",nNewUserId%100,nNewUserId);
		q.finalize();
		return;
	}
	else
	{
		while (!q.eof())
		{
			memset(buf,0,sizeof(buf));
			int nFrindId = q.getIntFileds("friendId");
			//根据好友的Id查找好友名字和等级
			sprintf(buf,"select userName,userLevel from m_gameUserInfo where userID=%d",nFrindId);
			string strSql1(buf);
			MysqlQuery q1 = g_cMagicServerDbRead.master_slave_exQuery(strSql1);
			if (q1.eof())
			{
				g_cMyGlog.errorlogRecord("no Data in m_gameUserInfo in userId=%d\n",nFrindId);
				q1.finalize();
				q.finalize();
				return;
			}
			else
			{
				PBFriend* u_PBFriend;
				u_PBFriend = u_PBAllFriend.add_allfriend();
				u_PBFriend->set_friendid(nFrindId);
				u_PBFriend->set_friendname(q1.getStringFileds("userName"));
				u_PBFriend->set_friendlevel(q1.getIntFileds("userLevel"));
				
				memset(buf,0,sizeof(buf));
				//根据好友ID查找好友的队长卡牌信息
				sprintf(buf,"select * from m_userCardInfo%d where userId = %d and battlePosition = %d",\
					nFrindId%USER_CARD_TABLE_COUNT,nFrindId,CAPITOR_POSITION);
				string strSql2(buf);
				MysqlQuery q2 = g_cMagicServerDbRead.master_slave_exQuery(strSql2);
				if (q1.eof())
				{
					g_cMyGlog.errorlogRecord("no Data in m_userCardInfo%d in userId=%d\n",nFrindId%USER_CARD_TABLE_COUNT,nFrindId);
					q2.finalize();
					q1.finalize();
					q.finalize();
					return;
				}
				else
				{
					PBCard* u_PBCard;
					u_PBCard = u_PBFriend->mutable_friendcard();
					u_PBCard->set_itemid(q2.getLongFileds("itemId"));
					
					u_PBCard->set_cardid(q2.getStringFileds("cardId"));
					u_PBCard->set_cardlevel(q2.getIntFileds("cardLevel"));
					u_PBCard->set_cardexp(q2.getIntFileds("cardExp"));
					u_PBCard->set_skillexp(q2.getIntFileds("skillExp"));
					u_PBCard->set_skilllevel(q2.getIntFileds("skillLevel"));
					u_PBCard->set_battleposition(q2.getIntFileds("battlePosition"));
					u_PBCard->set_vtlgrow(q2.getIntFileds("VTLGrow"));
					u_PBCard->set_strgrow(q2.getIntFileds("STRGrow"));
					u_PBCard->set_tghgrow(q2.getIntFileds("TGHGrow"));
					u_PBCard->set_quigrow(q2.getIntFileds("QUIGrow"));
					u_PBCard->set_mgcgrow(q2.getIntFileds("MGCGrow"));

					q2.finalize();
				}
				q1.finalize();
				
			}
		}
	}
	q.finalize();
	return;

}

//数据库连接保活
void cMagicServerDb::mysqlKeepAlive(int u_Type)
{
	char buf[128] = {0};
	if (u_Type == 1)
	{
		sprintf(buf,"select * from m_gameuserinfo where userID = 20;");
		string strSql(buf);

		MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
		q.finalize();
	}
	else if (u_Type == 2)
	{
		sprintf(buf,"delete from mysql_keep_alive_test_table;");
		string strSql(buf);
		g_cMagicServerDb.execSQL(strSql);
	}

}

//删除用户
void cMagicServerDb::deletUserByUserId(int u_userId)
{
	char buf[256] = {0};
	sprintf(buf,"delete from m_gameuserinfo where userID = %d;",u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("deletUserByUserId error,strSql:%s\n",strSql.c_str());
	}
	return;
}

//判断是否需要更新数据
bool cMagicServerDb::isSeverExecuteDataUpdate(const string& u_serverIp)
{
	char buf[128] = {0};
	sprintf(buf,"select isExecuteUpdate from m_updateInfo_table where executeUpdateServer='%s';",u_serverIp.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("isSeverExecuteDataUpdate execQuery failure:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	if (q.getIntFileds("isExecuteUpdate") == 1)
	{
		q.finalize();
		return true;
	}
	q.finalize();
	return false;

}

//防止主从同步延迟造成的从从库取不到数据的情况
MysqlQuery cMagicServerDb::master_slave_exQuery(const string& strSql)
{
	MysqlQuery q = g_cMagicServerDbRead.execQuery(strSql);
	if (!q.eof())
	{
		return q;
	}
	g_cMyGlog.debugLog("get nothing from slaveDb:%s\n",strSql.c_str());
	q.finalize();
	MysqlQuery q1 = g_cMagicServerDb.execQuery(strSql);
	return q1;
}




