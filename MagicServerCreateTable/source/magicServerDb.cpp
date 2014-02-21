#include "../include/magicServerDb.h"
#include "../include/structDef.h"
#include "../include/define.h"
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>



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
	//���Ӳ���
	initCondition(g_strDbHost,g_nDbPort,g_strDatabase,g_strUser,g_strPw);
	printf("g_strDbHost=%s,g_nDbPort=%d,g_strDatabase=%s,g_strUser=%s,g_strPw=%s\r\n",g_strDbHost.c_str(),g_nDbPort,g_strDatabase.c_str(),g_strUser.c_str(),g_strPw.c_str());
	//������DB
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
bool cMagicServerDb::DbReadInit()
{
	//���Ӳ���
	initCondition(g_strDbReadHost,g_nDbPort,g_strDatabase,g_strUser,g_strPw);
	printf("g_strDbReadHost=%s,g_nDbPort=%d,g_strDatabase=%s,g_strUser=%s,g_strPw=%s\r\n",g_strDbReadHost.c_str(),g_nDbPort,g_strDatabase.c_str(),g_strUser.c_str(),g_strPw.c_str());
	//������DB
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
required string affiche = 1;//����
repeated PBServer allServer = 2;//��������Ϣ
required string gameVersion = 3;//��Ϸ�汾
required string resourceVersion = 4;//��Դ�汾
required string battleVersion = 5;//������汾
}

*/
////////////�õ�PBSystemInfoReceive����Ķ���/////begin

//���PBSystemInfoReceive
void cMagicServerDb::setPBSystemInfoReceive(PBSystemInfoReceive& u_PBSystemInfoReceive)
{
	PBSystemInfoReceive_setAfficheInfo(u_PBSystemInfoReceive);
	PBSystemInfoReceive_setServerInfo(u_PBSystemInfoReceive);
	PBSystemInfoReceive_setGameVersion(u_PBSystemInfoReceive);
	PBSystemInfoReceive_setResourceVersion(u_PBSystemInfoReceive);
	PBSystemInfoReceive_setBattleVersion(u_PBSystemInfoReceive);
}

//��ȡ������Ϣ
void cMagicServerDb::PBSystemInfoReceive_setAfficheInfo(PBSystemInfoReceive& u_PBSystemInfoReceive)
{
	string strSql = "select * from afficheInfo order by afficheID desc limit 1";

	MysqlQuery q = execQuery(strSql);
	string strAfficheContents = "";
	if(!q.eof())
	{
		strAfficheContents = q.getStringFileds("afficheContents");
		u_PBSystemInfoReceive.set_affiche(strAfficheContents);
		q.nextRow();
	}
	q.finalize();
}
//��ȡ���������з�����Ϣ
void cMagicServerDb::PBSystemInfoReceive_setServerInfo(PBSystemInfoReceive& u_PBSystemInfoReceive)
{
	string strSql = "select * from allServerInfo";
	string strServerInfo = "";
	string serverName = "";
	string serverUrl = "";
	PBServer *u_PBServer;
	MysqlQuery q = execQuery(strSql);
	int i = 0;
	while(!q.eof())
	{
		serverName = q.getStringFileds("serverName");
		serverUrl = q.getStringFileds("serverUrl");
		u_PBServer = u_PBSystemInfoReceive.add_allserver();
		u_PBServer->set_servername(serverName);
		u_PBServer->set_serverurl(serverUrl);
		q.nextRow();
	}
	q.finalize();

}


//��ȡ��Ϸ�汾��Ϣ
void cMagicServerDb::PBSystemInfoReceive_setGameVersion(PBSystemInfoReceive& u_PBSystemInfoReceive)
{
	string strSql = "select gameVersionName from gameVersionInfo order by gameVersionID desc limit 1";
	string strGameVersionName = "";
	MysqlQuery q = execQuery(strSql);
	if (!q.eof())
	{
		strGameVersionName = q.getStringFileds("gameVersionName");
		u_PBSystemInfoReceive.set_gameversion(strGameVersionName);
		
	}
	q.finalize();
}

//��ȡ��Դ�汾��Ϣ
void cMagicServerDb::PBSystemInfoReceive_setResourceVersion(PBSystemInfoReceive& u_PBSystemInfoReceive)
{
	string strSql = "select resourceVersionName from resourceVersionInfo order by resourceVersionID desc limit 1";
	string strResourceVersionName = "";
	MysqlQuery q = execQuery(strSql);
	if (!q.eof())
	{
		strResourceVersionName = q.getStringFileds("resourceVersionName");
		u_PBSystemInfoReceive.set_resourceversion(strResourceVersionName);
	}
	q.finalize();
}

//��ȡ�����汾��Ϣ
void cMagicServerDb::PBSystemInfoReceive_setBattleVersion(PBSystemInfoReceive& u_PBSystemInfoReceive)
{
	string strSql = "select battleVersionName from battleVersionInfo order by battleVersionId desc limit 1";
	string strBattleVersionName = "";
	MysqlQuery q = execQuery(strSql);
	if (!q.eof())
	{
		strBattleVersionName = q.getStringFileds("battleVersionName");
		u_PBSystemInfoReceive.set_battleversion(strBattleVersionName);
	}
	q.finalize();
}
////////////�õ�PBSystemInfoReceive����Ķ���/////end








//tokenֵ����id�������
bool cMagicServerDb::InsertUserTokenInfo(string& userToken,int& userId)
{
	/*char buf[128]={0};
	sprintf(buf,"insert into m_userTokenInfo(userId,tokenValue) values(%d,%s)",userId,userToken.c_str());
	string strSql(buf);
	if (execQuery(strSql) != 0)
	{
		cerr<<"InsertUserTokenInfo error:%s"<<strSql<<endl;
		return true;
	}*/
	return false;
}

//����userToken���Ҷ�Ӧ���û�ID
int cMagicServerDb::getUserIdByUserToken(string& userToken)
{
	char buf[128] = {0};
	sprintf(buf,"select userId from m_userTokenInfo where tokenValue = '%s'",userToken.c_str());
	string strSql(buf);
	MysqlQuery q = execQuery(strSql);
	if (!q.eof())
	{
		int userId = q.getIntFileds("tokenValue");
		q.finalize();
		return userId;
	}
	q.finalize();
	return 0;
}

//������ʼ������Ϣ��
bool cMagicServerDb::createInitCardInfo()
{
	char buf[1024] = {0};
	memset(buf,0,sizeof(buf));
	sprintf(buf,"drop table if exists cardInfo");
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"drop cardInfo error"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return false;
	}
	memset(buf,0,sizeof(buf));
	sprintf(buf,"CREATE TABLE IF NOT EXISTS `cardinfo` ( \
				`cardId` varchar(6) NOT NULL COMMENT '����Id', \
				`VTLGrow` tinyint(4) NOT NULL COMMENT '�����ɳ����Ǽ�', \
				`STRGrow` tinyint(4) NOT NULL COMMENT '�����ɳ����Ǽ�', \
				`TGHGrow` tinyint(4) NOT NULL COMMENT 'ǿ�ȳɳ����Ǽ�', \
				`QUIGrow` tinyint(4) NOT NULL COMMENT '�ٶȳɳ����Ǽ�', \
				`MGCGrow` tinyint(4) NOT NULL COMMENT 'ħ���ɳ����Ǽ�', \
				`evolutionId` varchar(6) NOT NULL COMMENT '������Ŀ���Id',\
				`CardStarLevel` int	 NOT NULL COMMENT '�����Ǽ�',\
				`cardDigitId` int NOT NULL COMMENT '��������Id', \
				`maxCardLevel` int NOT NULL COMMENT '�������ȼ�',\
				PRIMARY KEY (`cardId`),\
				KEY (`CardStarLevel`)\
				) ENGINE=MyISAM DEFAULT CHARSET=utf8;");
	string strSqlTmp(buf);
	if (g_cMagicServerDb.execSQL(strSqlTmp) != 0)
	{
		cerr<<"strSql:"<<strSqlTmp<<endl;
		return false;
	}
	return true;
}

bool cMagicServerDb::dropNoUseCard(const int& u_cardId)
{
	if ((u_cardId >= 500 && u_cardId < 1000) ||
		u_cardId == 85 || u_cardId == 86 || u_cardId == 87 || u_cardId == 88)
	{
		return false;
	}
	return true;
}

//����ʼ���ݲ��뵽cardinfo
void cMagicServerDb::insertToCardInfo(MysqlQuery& q)
{
	char buf[256] = {0};
	int u_vtl = 0;
	string u_strVtl = q.getStringFileds("VTL");
	u_vtl = atoi(u_strVtl.c_str());

	int u_str = 0;
	string u_strStr = q.getStringFileds("STR");
	u_str = atoi(u_strStr.c_str());

	int u_tgh = 0;
	string u_strTgh = q.getStringFileds("TGH");
	u_tgh = atoi(u_strTgh.c_str());

	int u_qui = 0;
	string u_strQui = q.getStringFileds("QUI");
	u_qui = atoi(u_strQui.c_str());

	int u_mgc = 0;
	string u_strMgc = q.getStringFileds("MGC");
	u_mgc = atoi(u_strMgc.c_str());
	int u_starLevel = 0;
	string u_strStar = q.getStringFileds("CardStarLevel");
	u_starLevel = atoi(u_strStar.c_str());

	string u_strmaxCardLevel = q.getStringFileds("MaxLevel");
	int u_maxCardLevel = atoi(u_strmaxCardLevel.c_str());

	int u_cardDigitId = atoi((q.getStringFileds("Id")).c_str());
	sprintf(buf,"insert into cardinfo(cardId,VTLGrow,STRGrow,TGHGrow,QUIGrow,\
				MGCGrow,evolutionId,CardStarLevel,cardDigitId,maxCardLevel) values('%s',%d,%d,%d,%d,%d,'%s',%d,%d,%d)",(q.getStringFileds("Id")).c_str(),u_vtl,u_str,u_tgh,u_qui,u_mgc,\
				(q.getStringFileds("EvolutionId")).c_str(),u_starLevel,u_cardDigitId,u_maxCardLevel);
	string strSql(buf);
	if (execSQL(strSql) != 0)
	{
		cerr<<"insert cardId:"<<q.getStringFileds("Id")<<"failure"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
	}
	return;
}

//���Ǽ����뿨����Ϣ 
void cMagicServerDb::insertIntoCardStarTable(MysqlQuery& q,const string& u_strTableName)
{
	char buf[256] = {0};

	string u_strCardId = q.getStringFileds("cardId");

	int u_vtl = q.getIntFileds("VTLGrow");
	

	int u_str = q.getIntFileds("STRGrow");
	

	int u_tgh = q.getIntFileds("TGHGrow");

	int u_qui = q.getIntFileds("QUIGrow");

	int u_mgc = q.getIntFileds("MGCGrow");
	
	string u_strEvolutionId = q.getStringFileds("evolutionId");

	int u_starLevel = q.getIntFileds("CardStarLevel");

	int u_cardDigitId = q.getIntFileds("cardDigitId");

	sprintf(buf,"insert into %s(Id,cardId,VTLGrow,STRGrow,TGHGrow,QUIGrow,\
				MGCGrow,evolutionId,CardStarLevel,cardDigitId) values(NULL,'%s',%d,%d,%d,%d,%d,'%s',%d,%d)",u_strTableName.c_str(),u_strCardId.c_str(),u_vtl,u_str,u_tgh,u_qui,u_mgc,\
				u_strEvolutionId.c_str(),u_starLevel,u_cardDigitId);
	string strSql(buf);
	if (execSQL(strSql) != 0)
	{
		cerr<<"insert into "<<u_strTableName<<"error"<<endl;
		cerr<<"insert cardId:"<<q.getStringFileds("Id")<<"failure"<<endl;
	}
	return;
}

void cMagicServerDb::createStarTable(const string& u_strStarTable)
{
	char buf[2048] = {0};
	sprintf(buf,"drop table if exists %s;",u_strStarTable.c_str());
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"createStarTable error:"<<endl;
		cerr<<strSql<<endl;
		return;
	}
	memset(buf,0,sizeof(buf));
	sprintf(buf,"CREATE TABLE IF NOT EXISTS %s (\
		`Id` int NOT NULL AUTO_INCREMENT COMMENT '����Id',\
		`cardId` varchar(6) NOT NULL COMMENT '����Id',\
		`VTLGrow` tinyint(4) NOT NULL COMMENT '�����ɳ����Ǽ�',\
		`STRGrow` tinyint(4) NOT NULL COMMENT '�����ɳ����Ǽ�',\
		`TGHGrow` tinyint(4) NOT NULL COMMENT 'ǿ�ȳɳ����Ǽ�',\
		`QUIGrow` tinyint(4) NOT NULL COMMENT '�ٶȳɳ����Ǽ�',\
		`MGCGrow` tinyint(4) NOT NULL COMMENT 'ħ���ɳ����Ǽ�',\
		`evolutionId` varchar(6) NOT NULL COMMENT '������Ŀ���Id',\
		`CardStarLevel` int	 NOT NULL COMMENT '�����Ǽ�',\
		`cardDigitId` int NOT NULL COMMENT '��������Id',\
		PRIMARY KEY (`Id`)\
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;",u_strStarTable.c_str());
	strSql.clear();
	strSql += buf;
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"createStarTable error:"<<endl;
		cerr<<strSql<<endl;
		return;
	}

	
}

//���Ǽ���ʼ��������Ϣ
void cMagicServerDb::initCardInfoByCardStar()
{
	string u_strStarTable = "oneStarCardInfo";
	createStarTable(u_strStarTable);
	u_strStarTable.clear();
	u_strStarTable = "twoStarCardInfo";
	createStarTable(u_strStarTable);
	u_strStarTable.clear();
	u_strStarTable = "threeStarCardInfo";
	createStarTable(u_strStarTable);
	u_strStarTable.clear();
	u_strStarTable = "fourStarCardInfo";
	createStarTable(u_strStarTable);
	u_strStarTable.clear();
	u_strStarTable = "fiveStarCardInfo";
	createStarTable(u_strStarTable);
	u_strStarTable.clear();
	u_strStarTable = "sixStarCardInfo";
	createStarTable(u_strStarTable);


	char buf[128] = {0};
	sprintf(buf,"select * from cardinfo;");
	string strSql(buf);
	MysqlQuery q = execQuery(strSql);
	if (q.eof())
	{
		cerr<<"initCardInfoByCardStar error:"<<endl;
		cerr<<strSql<<endl;
		q.finalize();
		return;
	}
	while (!q.eof())
	{
		string u_strTable = "";
		int u_cardStar = q.getIntFileds("CardStarLevel");
		cerr<<"u_cardStar:"<<u_cardStar<<endl;
		if (q.getIntFileds("cardDigitId") < 1000)
		{
			if (!dropNoUseCard(q.getIntFileds("cardDigitId")))
			{
				q.nextRow();
				continue;
			}
			if (u_cardStar == 1)
			{
				u_strTable = "oneStarCardInfo";
				insertIntoCardStarTable(q,u_strTable);
			}
			else if (u_cardStar == 2)
			{
				u_strTable = "twoStarCardInfo";
				insertIntoCardStarTable(q,u_strTable);
			}
			else if (u_cardStar == 3)
			{
				u_strTable = "threeStarCardInfo";
				insertIntoCardStarTable(q,u_strTable);
			}
			else if (u_cardStar == 4)
			{
				u_strTable = "fourStarCardInfo";
				insertIntoCardStarTable(q,u_strTable);
			}
			else if (u_cardStar == 5)
			{
				u_strTable = "fiveStarCardInfo";
				insertIntoCardStarTable(q,u_strTable);
			}
			else if (u_cardStar == 6)
			{
				u_strTable = "sixStarCardInfo";
				insertIntoCardStarTable(q,u_strTable);
			}
		}
		q.nextRow();
	}
	q.finalize();
	return;
}

//��ʼ��cardinfo��
void cMagicServerDb::initCardInfoTable()
{
	char buf[256] = {0};
	sprintf(buf,"select Id,VTL,STR,TGH,QUI,MGC,EvolutionId,CardStarLevel,MaxLevel from cardinfoinput");
	string strSql(buf);
	MysqlQuery q = execQuery(strSql);
	if (q.eof())
	{
		cerr<<"get nothing from cardinfoinput"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		q.finalize();
		return;
	}
	while (!q.eof())
	{
		insertToCardInfo(q);
		q.nextRow();
	}
	q.finalize();
	return;
}


//�õ��ؿ�������
unsigned int cMagicServerDb::getInitLevelMonsterCount(const string& u_LevelId)
{
	unsigned int u_MonsterCount = 0;
	char buf[128] = {0};
	sprintf(buf,"select count(Id) from battleinfoinput where Id like '%s",u_LevelId.c_str());
	string strSql(buf);
	strSql += "%'";
	MysqlQuery q = execQuery(strSql);
	//cerr<<"strSql:"<<strSql<<endl;
	if (q.eof())
	{
		cerr<<"get nothing from battleinfoinput"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		q.finalize();
		return u_MonsterCount;
	}
	u_MonsterCount = q.getIntFileds(0);
	cerr<<"levelId:"<<u_LevelId<<endl;
	cerr<<"monsterId:"<<u_MonsterCount<<endl;
	return u_MonsterCount;
}

unsigned int cMagicServerDb::getBattleCount(const string& u_battleId,const unsigned int& u_maxId)
{
	int u_maxBattleId = 0;
	cerr<<"in getBattleCount:"<<u_battleId<<" "<<u_maxId<<endl;
	for (int i = 1; i <= u_maxId; i++)
	{
		
		for (int j = 0; j<10; j++)
		{
			char u_battleIdBuf[12] = {0};
			sprintf(u_battleIdBuf,"%s%d%d",u_battleId.c_str(),i,j);
			string u_strBattleId(u_battleIdBuf);
			char bufTmp[128] = {0};
			sprintf(bufTmp,"select count(Id) from battleinfoinput where Id like '%s",u_strBattleId.c_str());
			string strSql(bufTmp);
			strSql += "%'";
			MysqlQuery q = execQuery(strSql);
			cerr<<"count(Id)"<<q.getIntFileds("count(Id)")<<endl;
			if (q.getIntFileds("count(Id)") > 0)
			{
				q.finalize();
				//cerr<<"strSql:"<<strSql<<endl;
				continue;
			}
			q.finalize();
			cerr<<"strSql:"<<strSql<<endl;
			char u_maxBattleIdBuf[6] = {0};
			sprintf(u_maxBattleIdBuf,"%d%d",i,j);
			u_maxBattleId = atoi(u_maxBattleIdBuf) - 1;
			cerr<<"u_maxBattleId:"<<u_maxBattleId<<endl;
			return u_maxBattleId;
		}
		
	}
	return u_maxBattleId;
}

//��ȡ�󸱱��µ�С������
int cMagicServerDb::getBattleSummulation(const string& u_battleId)
{
	int u_battleCount = 0;
	char buf[128] = {0};
	sprintf(buf,"select count(battleId) from battleinfo where battleId < '%s",u_battleId.c_str());
	string strSql(buf);
	strSql += "%'";
	MysqlQuery q = execQuery(strSql);
	if (q.eof())
	{
		cerr<<"getBattleSummulation failure:"<<endl;
		cerr<<strSql<<endl;
	}
	else
	{
		u_battleCount = q.getIntFileds("count(battleId)");
	}
	q.finalize();
	return u_battleCount;
}

//���¸����ۼ���Ŀ
void cMagicServerDb::updateBattleSummulation(const string& u_battleTmpId,const int& u_battleSumCount)
{
	char buf[128] = {0};
	sprintf(buf,"update battleinfo set battleSumCount=%d where battleId = '%s';",u_battleSumCount,u_battleTmpId.c_str());
	string strSql(buf);
	if (execSQL(strSql) != 0)
	{
		cerr<<"updateBattleSummulation error"<<endl;
		cerr<<strSql<<endl;
	}
	
}

//��ȡ������Ŀ
unsigned int cMagicServerDb::getInitBattleCount(const string& u_battleId)
{
	unsigned int u_battleCount = 0;
	char buf[128] = {0};
	sprintf(buf,"select max(Id) from battleinfoinput where Id like '%s",u_battleId.c_str());
	string strSql(buf);
	strSql += "%'";
	MysqlQuery q = execQuery(strSql);
	cerr<<"strSql:"<<strSql<<endl;
	if (q.eof())
	{
		cerr<<"get nothing from battleinfoinput"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		q.finalize();
		return u_battleCount;
	}
	string u_strMaxId = "";
	u_strMaxId = q.getStringFileds(0);
	cerr<<"MaxId:"<<u_strMaxId<<endl;
	string u_strMaxId1 = "";
	u_strMaxId1 = q.getStringFileds("max(Id)");
	cerr<<"MaxId1:"<<u_strMaxId1<<endl;
	if (u_strMaxId.size() > 3)
	{
		char* buf = NULL;
		buf = (char*)malloc(u_strMaxId1.size()+1);
		memcpy(buf,u_strMaxId.c_str(),u_strMaxId.size());
		cerr<<"buf:"<<buf<<endl;
		char   *token   =   strtok(buf,"-");
		int i = 0;
		while(token  !=   NULL  && i < 1 )
		{
			i += 1;
			token   =   strtok(NULL,"-");
			if(i == 1)
				printf("%s\n",token);
		}
		u_battleCount = atoi(token);
		
		
	}
	if (u_battleCount > 0 && u_battleCount < 9)
	{
		cerr<<"Id:"<<u_battleId<<endl;
		cerr<<"u_battleCount:"<<u_battleCount<<endl;
		return u_battleCount;
	}
	else
	{
		int battleCountTmp = u_battleCount;
		cerr<<"battleCountTmp:"<<battleCountTmp<<endl;
		u_battleCount = getBattleCount(u_battleId,battleCountTmp);
		cerr<<"Id:"<<u_battleId<<endl;
		cerr<<"u_battleCount:"<<u_battleCount<<endl;
		return u_battleCount;
	}
	
}

//��ȡ�����ؿ�����
unsigned int cMagicServerDb::getInitBattleLevelCount(const string& u_battleId)
{
	unsigned int u_levelCount = 0;
	char buf[128] = {0};
	sprintf(buf,"select max(Id) from battleinfoinput where Id like '%s",u_battleId.c_str());
	string strSql(buf);
	strSql += "%'";
	MysqlQuery q = execQuery(strSql);
	cerr<<"strSql:"<<strSql<<endl;
	if (q.eof())
	{
		cerr<<"get nothing from battleinfoinput"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		q.finalize();
		return u_levelCount;
	}
	string u_strMaxId = "";
	u_strMaxId = q.getStringFileds(0);
	cerr<<"MaxId:"<<u_strMaxId<<endl;
	string u_strMaxId1 = "";
	u_strMaxId1 = q.getStringFileds("max(Id)");
	cerr<<"MaxId1:"<<u_strMaxId1<<endl;
	if (u_strMaxId.size() > 3)
	{
		char* buf = NULL;
		buf = (char*)malloc(u_strMaxId1.size()+1);
		memcpy(buf,u_strMaxId.c_str(),u_strMaxId.size());
		cerr<<"buf:"<<buf<<endl;
		char   *token   =   strtok(buf,"-");
		int i = 0;
		while(token  !=   NULL  && i < 2 )
		{
			i += 1;
			token   =   strtok(NULL,"-");
			if(i == 2)
				printf("%s\n",token);
		}

		u_levelCount = atoi(token);
	}
	if (u_levelCount > 0 && u_levelCount < 9)
	{
		cerr<<"battleId:"<<u_battleId<<endl;
		cerr<<"u_battleLevelCount:"<<u_levelCount<<endl;
		return u_levelCount;
	}
	else
	{
		int u_levelTmpCount = u_levelCount;
		string u_strBattleId = u_battleId;
		u_levelCount = getBattleCount(u_strBattleId,u_levelTmpCount);
		cerr<<"battleId:"<<u_strBattleId<<endl;
		cerr<<"u_battleLevelCount:"<<u_levelCount<<endl;
		return u_levelCount;
	}
}

//������Ը�����Ϣ
void cMagicServerDb::insertInitBattleInfo(const string& u_battleId,const unsigned int& u_levelCount,\
						  const unsigned int& u_energyUse,const unsigned int& u_monsterTotal,\
						  const unsigned int& u_monsterAverageLevel,const int& u_monsterTatalLevel)
{
	char buf[1024] = {0};
	sprintf(buf,"insert into battleinfo(`battleId`,`levelCount`,`energyUse`,`monsterCount`,`averageLevel`,`totalLevel`) \
				values('%s',%d,%d,%d,%d,%d)",u_battleId.c_str(),u_levelCount,u_energyUse,u_monsterTotal,u_monsterAverageLevel,u_monsterTatalLevel);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"insert into battleinfo error"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return;
	}
	return;
}

//����������Ϣ��
bool cMagicServerDb::createInitBattleInfo()
{
	char buf[1024] = {0};
	memset(buf,0,sizeof(buf));
	sprintf(buf,"drop table if exists battleinfo");
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"drop battleinfo error"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return false;
	}
	memset(buf,0,sizeof(buf));
	sprintf(buf,"CREATE TABLE IF NOT EXISTS `battleinfo` (\
		`battleId` varchar(10) NOT NULL COMMENT '����Id',\
		`levelCount` tinyint(3) NOT NULL COMMENT '�����ؿ����ﲨ��',\
		`energyUse` tinyint(2) NOT NULL COMMENT '��������',\
		`monsterCount` int NOT NULL COMMENT '�ø�����������',\
		`averageLevel` int NOT NULL COMMENT '�������й���ƽ���ȼ�',\
		`battleSumCount` int NOT NULL COMMENT '�����ۼ���Ŀ',\
		`totalLevel` int NOT NULL COMMENT '�������й����ܵȼ�',\
		PRIMARY KEY (`battleId`) \
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;");
	string strSqlTmp(buf);
	if (g_cMagicServerDb.execSQL(strSqlTmp) != 0)
	{
		cerr<<"strSql:"<<strSqlTmp<<endl;
		return false;
	}
	return true;
}

//�����ʼ�ؿ���Ϣ
void cMagicServerDb::insertInitBattleLevelInfo(const string& u_levelId,const unsigned int& u_monsterCount)
{
	char buf[1024] = {0};
	sprintf(buf,"insert into battleLevelInfo(`levelId`,`monsterCount`) \
				values('%s',%d)",u_levelId.c_str(),u_monsterCount);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"insert into battleLevelInfo error"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return;
	}
	return;
}

//���������ؿ�����������
bool cMagicServerDb::createInitBattleLevelInfo()
{
	char buf[1024] = {0};
	memset(buf,0,sizeof(buf));
	sprintf(buf,"drop table if exists battleLevelInfo");
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"drop battleLevelInfo error"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return false;
	}
	memset(buf,0,sizeof(buf));

	sprintf(buf,"CREATE TABLE IF NOT EXISTS `battleLevelInfo` (\
		`levelId` varchar(14) NOT NULL COMMENT '�ؿ�Id',\
		`monsterCount` tinyint(3) NOT NULL COMMENT '�ùؿ���������',\
		PRIMARY KEY (`levelId`) \
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;");
	string strSqlTmp(buf);
	if (g_cMagicServerDb.execSQL(strSqlTmp) != 0)
	{
		cerr<<"strSql:"<<strSqlTmp<<endl;
		return false;
	}
	return true;
}

//��ʼ�������ҵ���
void cMagicServerDb::InitMonsterGoldLoseInfo(const string& u_monsterId)
{
	char buf[512] = {0};
	sprintf(buf,"select GoldProbability,GoldNumber from battleinfoinput where Id = '%s'",u_monsterId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql); 
	if (q.eof())
	{
		cerr<<"InitMonsterGoldInfo get nothing"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return;
	}
	insertMonsterGoldLoseInfo(q,u_monsterId);

	q.finalize();
	return;
	
}

//��������ҵ���
void cMagicServerDb::insertMonsterGoldLoseInfo(MysqlQuery& q,const string& u_monsterId)
{
	char buf[1024] = {0};
	string u_strGoldProbability = q.getStringFileds("GoldProbability");
	int u_goldProbability = atoi(u_strGoldProbability.c_str());

	string u_strGoldNumber = q.getStringFileds("GoldNumber");
	int u_goldNumber = atoi(u_strGoldNumber.c_str());
	sprintf(buf,"insert into battleGoldInfo(`monsterId`,`goldProbability`,`goldNumber`) \
				values('%s',%d,%d)",u_monsterId.c_str(),u_goldProbability,u_goldNumber);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"insert into battleGoldInfo error"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return;
	}
	return;
}

//������һ��������Ϣ�����
bool cMagicServerDb::createInitMonsterGoldLoseInfo()
{
	char buf[1024] = {0};
	memset(buf,0,sizeof(buf));
	sprintf(buf,"drop table if exists battleGoldInfo");
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"drop battleGoldInfo error"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return false;
	}
	memset(buf,0,sizeof(buf));
	sprintf(buf,"CREATE TABLE IF NOT EXISTS `battleGoldInfo` (\
		`monsterId` varchar(18) NOT NULL COMMENT '����Id',\
		`goldProbability` int NOT NULL COMMENT '�����ҵ�����',\
		`goldNumber` smallint(6) NOT NULL COMMENT '�������Ľ������',\
		PRIMARY KEY (`monsterId`) \
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;");
	string strSqlTmp(buf);
	if (g_cMagicServerDb.execSQL(strSqlTmp) != 0)
	{
		cerr<<"strSql:"<<strSqlTmp<<endl;
		return false;
	}
	return true;
}

//��ʼ������װ������
void cMagicServerDb::InitMonsterEquipLoseInfo(const string& u_monsterId,const unsigned int& u_nType)
{
	char buf[512] = {0};
	sprintf(buf,"select EquipProbability%d,EquipId%d from battleinfoinput where Id = '%s'",u_nType,u_nType,u_monsterId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql); 
	if (q.eof())
	{
		cerr<<"InitMonsterEquipLoseInfo get nothing"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return;
	}
	insertMonsterEquipLoseInfo(q,u_monsterId,u_nType);
	q.finalize();
	return;

}

//�������װ������
void cMagicServerDb::insertMonsterEquipLoseInfo(MysqlQuery& q,const string& u_monsterId,const unsigned int& u_nType)
{
	char buf[1024] = {0};
	char bufTmp[32] = {0};
	sprintf(bufTmp,"EquipProbability%d",u_nType);
	string u_strBufTmp(bufTmp);

	string u_equipProbalility1 = q.getStringFileds(u_strBufTmp);
	int u_EuipProbability = atoi(u_equipProbalility1.c_str());

	memset(bufTmp,0,sizeof(bufTmp));
	sprintf(bufTmp,"EquipId%d",u_nType);
	string u_strBufTmp1(bufTmp);
	string u_strEquipId = q.getStringFileds(u_strBufTmp1);

	int u_EquipId = atoi(u_strEquipId.c_str());
	sprintf(buf,"insert into battleEquipInfo(monsterId,equipTypeId,equipId,equipProbability) values('%s',%d,'%s',%d);",\
		u_monsterId.c_str(),u_nType,u_strEquipId.c_str(),u_EuipProbability);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"insert into battleEquipInfo error"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return;
	}
	return;
}


//������һ����װ����Ϣ�����
bool cMagicServerDb::createInitEquipLoseInfo()
{
	char buf[1024] = {0};
	memset(buf,0,sizeof(buf));
	sprintf(buf,"drop table if exists battleEquipInfo");
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"drop battleEquipInfo error"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return false;
	}
	memset(buf,0,sizeof(buf));
	sprintf(buf,"CREATE TABLE IF NOT EXISTS `battleEquipInfo` (\
		`monsterId` varchar(18) NOT NULL COMMENT '����Id',\
		`equipTypeId` tinyint(3) NOT NULL COMMENT '��������װ������Id',\
		`equipId` varchar(6) NOT NULL COMMENT '��������ĳ��װ����Id',\
		`equipProbability` int NOT NULL COMMENT '����ĳ��װ����װ��Id������',\
		KEY `monster_equipTypeId` (`monsterId`,`equipTypeId`)\
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;");

	string strSqlTmp(buf);
	if (g_cMagicServerDb.execSQL(strSqlTmp) != 0)
	{
		cerr<<"strSql:"<<strSqlTmp<<endl;
		return false;
	}
	return true;
}


//��ʼ�����￨�Ƶ���
void cMagicServerDb::InitMonsterCardLoseInfo(const string& u_monsterId,int& u_cardLevel)
{
	char buf[512] = {0};
	sprintf(buf,"select CardLevel,CardProbability,GetCardId from battleinfoinput where Id = '%s'",u_monsterId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql); 
	if (q.eof())
	{
		cerr<<"InitMonsterCardLoseInfo get nothing"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return;
	}
	insertMonsterCardLoseInfo(q,u_monsterId);
	u_cardLevel = atoi((q.getStringFileds("CardLevel")).c_str());
	q.finalize();
	return;

}

//������￨�Ƶ���
void cMagicServerDb::insertMonsterCardLoseInfo(MysqlQuery& q,const string& u_monsterId)
{
	char buf[1024] = {0};

	string u_strCardLevel = q.getStringFileds("CardLevel");
	int u_cardLevel = atoi(u_strCardLevel.c_str());

	string u_strCardProbability = q.getStringFileds("CardProbability");
	int u_cardProbability = atoi(u_strCardProbability.c_str());

	string u_strCardId = q.getStringFileds("GetCardId");
	
	sprintf(buf,"insert into battleCardInfo(monsterId,cardId,cardProbability,cardLevel) values('%s','%s',%d,%d);",\
		u_monsterId.c_str(),u_strCardId.c_str(),u_cardProbability,u_cardLevel);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"insert into battleCardInfo error"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return;
	}
	return;
}


//������һ���￨����Ϣ�����
bool cMagicServerDb::createInitCardLoseInfo()
{
	char buf[1024] = {0};
	memset(buf,0,sizeof(buf));
	sprintf(buf,"drop table if exists battleCardInfo");
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"drop battleCardInfo error"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return false;
	}
	memset(buf,0,sizeof(buf));
	sprintf(buf,"CREATE TABLE IF NOT EXISTS `battleCardInfo` (\
		`monsterId` varchar(18) NOT NULL COMMENT '����Id',\
		`cardId` varchar(64) NOT NULL COMMENT '�������Ŀ���Id',\
		`cardProbability` int NOT NULL COMMENT '�������Ŀ��Ƶ�����',\
		`cardLevel`	tinyint(3) NOT NULL	COMMENT	'�������Ŀ��Ƶĵȼ�',\
		PRIMARY KEY (`monsterId`)\
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;");
	string strSqlTmp(buf);
	if (g_cMagicServerDb.execSQL(strSqlTmp) != 0)
	{
		cerr<<"strSql:"<<strSqlTmp<<endl;
		return false;
	}
	return true;
}



//��ʼ�����￨�Ƶ���
void cMagicServerDb::InitMonsterBigCardIdLoseInfo(const string& u_monsterId,int& u_cardLevel)
{
	char buf[512] = {0};
	sprintf(buf,"select CardLevel,EquipProbabulity2,EquipId2 from battleinfoinput where Id = '%s'",u_monsterId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql); 
	if (q.eof())
	{
		cerr<<"InitMonsterCardLoseInfo get nothing"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return;
	}
	insertMonsterBigCardIdLoseInfo(q,u_monsterId);
	u_cardLevel = atoi((q.getStringFileds("CardLevel")).c_str());
	q.finalize();
	return;

}

//������￨�Ƶ���
void cMagicServerDb::insertMonsterBigCardIdLoseInfo(MysqlQuery& q,const string& u_monsterId)
{
	char buf[1024] = {0};

	string u_strCardLevel = q.getStringFileds("CardLevel");
	int u_cardLevel = atoi(u_strCardLevel.c_str());

	string u_strCardProbability = q.getStringFileds("EquipProbabulity2");
	int u_cardProbability = atoi(u_strCardProbability.c_str());

	string u_strCardId = q.getStringFileds("EquipId2");

	sprintf(buf,"insert into battle_big_card_id_info(monsterId,cardId,cardProbability,cardLevel) values('%s','%s',%d,%d);",\
		u_monsterId.c_str(),u_strCardId.c_str(),u_cardProbability,u_cardLevel);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"insert into battleCardInfo error"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return;
	}
	return;
}


////������һ���￨��Id����2000��Ϣ�����
bool cMagicServerDb::createInitBigCardIdLoseInfo()
{
	char buf[1024] = {0};
	memset(buf,0,sizeof(buf));
	sprintf(buf,"drop table if exists battle_big_card_id_info;");
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"drop battle_big_card_id_info error"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return false;
	}
	memset(buf,0,sizeof(buf));
	sprintf(buf,"CREATE TABLE IF NOT EXISTS `battle_big_card_id_info` (\
				`monsterId` varchar(18) NOT NULL COMMENT '����Id',\
				`cardId` varchar(64) NOT NULL COMMENT '�������Ŀ���Id',\
				`cardProbability` int NOT NULL COMMENT '�������Ŀ��Ƶ�����',\
				`cardLevel`	tinyint(3) NOT NULL	COMMENT	'�������Ŀ��Ƶĵȼ�',\
				PRIMARY KEY (`monsterId`)\
				) ENGINE=MyISAM DEFAULT CHARSET=utf8;");
	string strSqlTmp(buf);
	if (g_cMagicServerDb.execSQL(strSqlTmp) != 0)
	{
		cerr<<"strSql:"<<strSqlTmp<<endl;
		return false;
	}
	return true;
}

//����100��m_userCardInfo��
bool cMagicServerDb::createUserCardInfo()
{
	for (int i = 0; i <= 99; i++)
	{
		char buf[2048] = {0};
		memset(buf,0,sizeof(buf));
		sprintf(buf,"drop table if exists m_usercardinfo%d",i);
		string strSqlTmp(buf);
		if (g_cMagicServerDb.execSQL(strSqlTmp) != 0)
		{
			cerr<<"strSql:"<<strSqlTmp<<endl;
			return false;
		}
		memset(buf,0,sizeof(buf));
		sprintf(buf,"CREATE TABLE IF NOT EXISTS `m_usercardinfo%d` ( \
					`itemId` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '������Ʒid', \
					`userID` int(11) NOT NULL COMMENT '�û�ID', \
					`cardId` varchar(6) NOT NULL COMMENT '����Id', \
					`cardLevel` tinyint(3) NOT NULL DEFAULT 1 COMMENT '���Ƶȼ�', \
					`cardExp` int(11) NOT NULL DEFAULT 0 COMMENT '���ƾ���', \
					`battlePosition` tinyint(1) NOT NULL COMMENT 'ս��λ�ñ��', \
					`VTLGrow` tinyint(4) NOT NULL COMMENT '�����ɳ���', \
					`STRGrow` tinyint(4) NOT NULL COMMENT '�����ɳ���', \
					`TGHGrow` tinyint(4) NOT NULL COMMENT 'ǿ�ȳɳ���', \
					`QUIGrow` tinyint(4) NOT NULL COMMENT '�ٶȳɳ���', \
					`MGCGrow` tinyint(4) NOT NULL COMMENT 'ħ���ɳ���', \
					`skillExp` int(11) NOT NULL DEFAULT 0 COMMENT '����������', \
					`skillLevel` tinyint(3) NOT NULL DEFAULT 1 COMMENT '���ܵȼ�', \
					`goldValue` int(11) NOT	NULL COMMENT '�������ļ�Ǯ',\
					PRIMARY KEY (`itemId`), \
					KEY `userID` (`userID`,`battlePosition`),\
					) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;",i);
		string strSqlTmp1(buf);
		if (g_cMagicServerDb.execSQL(strSqlTmp1) != 0)
		{
			cerr<<"strSql:"<<strSqlTmp1<<endl;
			return false;
		}
	}
	return true;
}

//����20��m_userEquipInfo
bool cMagicServerDb::createUserEuipInfo()
{
	for(int i = 0; i < 20; i++)
	{
		char buf[1024] = {0};
		memset(buf,0,sizeof(buf));
		sprintf(buf,"drop table if exists m_userEquipInfo%d",i);
		string strTmpSql(buf);
		if (g_cMagicServerDb.execSQL(strTmpSql) != 0)
		{
			cerr<<"strSql:"<<strTmpSql<<endl;
			return false;
		}

		memset(buf,0,sizeof(buf));
		sprintf(buf,"CREATE TABLE IF NOT EXISTS `m_userequipinfo%d` (\
					`userId` int(11) NOT NULL COMMENT '�û�ID',\
					`equipTypeId` tinyint(3) NOT NULL COMMENT 'װ������Id',\
					`equipMentId` varchar(6) NOT NULL COMMENT 'װ��ID',\
					`count` int(11) NOT NULL DEFAULT '0' COMMENT 'װ������',\
					KEY `user_equipId` (`userId`,`equipMentId`) \
					) ENGINE=MyISAM DEFAULT CHARSET=utf8;",i);
		string strSqlTmp1(buf);
		if (g_cMagicServerDb.execSQL(strSqlTmp1) != 0)
		{
			cerr<<"strSql:"<<strSqlTmp1<<endl;
			return false;
		}
	}
	return true;
}

//�����������
void cMagicServerDb::insertMissionProccess(const int& u_preMaxMissionId)
{
	char buf[128] = {0};
	sprintf(buf,"select userID from m_gameuserinfo;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	if (q.eof())
	{
		cerr<<"insertMissionProccess failure"<<endl;
		cerr<<strSql<<endl;
		q.finalize();
		return;
	}
	while (!q.eof())
	{
		int u_userId = q.getIntFileds("userID");
		/*if (u_userId != 39)
		{*/
			insertMissions(u_userId,u_preMaxMissionId);
		//}
		q.nextRow();
	}
	q.finalize();
	return;
}


bool cMagicServerDb::isUserMissionExist(const unsigned int& u_userId,const int& u_missionId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_userMissionsInfo%d where userId=%d and missionId=%d;",\
		u_userId%100,u_userId,u_missionId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	if (q.eof())
	{
		q.finalize();
		return false;
	}
	q.finalize();
	return true;
}

void cMagicServerDb::deleteUserMissionId(const unsigned int& u_userId,const int& u_missionId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_userMissionsInfo%d where userId=%d and missionId=%d;",\
		u_userId%100,u_userId,u_missionId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"deleteUserMissionId failure,strSql:"<<strSql<<endl;
	}
	return;
}

//��������
void cMagicServerDb::insertMissions(const unsigned int& u_userId,const int& u_preMaxMissionId)
{
	char buf[128] = {0};
	//missionId > 7Ϊ�ǵ�½����
	sprintf(buf,"select * from missioninfo where Id > 7 and Id not in (69,70,71,72,73,74,75,76,77,78,79,80);");
	string strSql(buf);

	MysqlQuery q = execQuery(strSql);
	if (q.eof())
	{
		cerr<<"no Data in missionsInfo"<<endl;
		q.finalize();
		return;
	}
	else
	{
		while (!q.eof())
		{

			int nMissionId = q.getIntFileds("Id");
			int nMissionType = q.getIntFileds("ConditionType");
			string u_missionCondition = q.getStringFileds("ConditionValue");
			string u_hortationType = q.getStringFileds("HortationType");
			int u_hortationCount = q.getIntFileds("HortationValue");
			bool isFinished = false;
			if (isUserMissionExist(u_userId,nMissionId))
			{
				deleteUserMissionId(u_userId,nMissionId);
			}
			else
			{
				if (nMissionId <= u_preMaxMissionId)
				{
					/*if (nMissionId == 8 || nMissionId == 9)
					{
					}
					else
					{*/
						q.nextRow();
						continue;
					//}
					
					
				}
			}
			//�Ȳ�����Ϣ���ɹ����������Ϣ
			char buf[512] = {0};
			sprintf(buf,"insert into m_userMissionsInfo%d(userId,missionId,missinType,missionCondition,hortationType,hortationCount,isFinished) \
						values(%d,%d,%d,'%s','%s',%d,0)",u_userId%100,u_userId,\
						nMissionId,nMissionType,u_missionCondition.c_str(),u_hortationType.c_str(),u_hortationCount);
			string strSql(buf);
			unsigned int tmpId = 0;
			if(execSQL(strSql,&tmpId)!=0)
			{
				cerr<<"insertMissions error!"<<endl;
				cerr<<strSql<<endl;
			}
			q.nextRow();
		}
		q.finalize();
	}
}


//����100��m_userMissionsInfo
bool cMagicServerDb::createUserMissionInfo()
{
	for(int i = 0 ;i < 100; i++)
	{
		char buf[1024] = {0};
		memset(buf,0,sizeof(buf));
		sprintf(buf,"drop table if exists m_userMissionsInfo%d",i);
		string strTmpSql(buf);
		if (g_cMagicServerDb.execSQL(strTmpSql) != 0)
		{
			cerr<<"strSql:"<<strTmpSql<<endl;
			return false;
		}

		memset(buf,0,sizeof(buf));
		sprintf(buf,"CREATE TABLE IF NOT EXISTS `m_usermissionsinfo%d` ( \
					`userId` int(11) NOT NULL COMMENT '�û�ID', \
					`missionId` int(11) NOT NULL COMMENT '����id', \
					`missinType` tinyint(3) NOT NULL  COMMENT '��������', \
					`missionCondition` varchar(20) NOT NULL COMMENT '�����������', \
					`hortationType` varchar(6) NOT NULL COMMENT '��������', \
					`hortationCount` int NOT NULL COMMENT '������Ŀ', \
					`isFinished` tinyint(1) NOT NULL DEFAULT '0' COMMENT '�Ƿ������', \
					UNIQUE	`user_MissionId` (`userId`,`missionId`),\
					KEY `user_Type_Condition` (`userId`,`missinType`,`missionCondition`) \
					) ENGINE=MyISAM DEFAULT CHARSET=utf8;",i);
		string strSqlTmp1(buf);
		if (g_cMagicServerDb.execSQL(strSqlTmp1) != 0)
		{
			cerr<<"strSql:"<<strSqlTmp1<<endl;
			return false;
		}
	}
	return true;
}

//����20��m_userFriendInfo
bool cMagicServerDb::createUserFriendInfo()
{
	for (int i = 0; i < 20 ;i++)
	{
		char buf[1024] = {0};
		memset(buf,0,sizeof(buf));
		sprintf(buf,"drop table if exists m_userFriendInfo%d",i);	
		string strTmpSql(buf);
		if (g_cMagicServerDb.execSQL(strTmpSql) != 0)
		{
			cerr<<"strSql:"<<strTmpSql<<endl;
			return false;
		}

		memset(buf,0,sizeof(buf));
		sprintf(buf,"CREATE TABLE IF NOT EXISTS `m_userfriendinfo%d` ( \
					`userId` int(11) NOT NULL COMMENT '�û�id', \
					`friendId` int(11) NOT NULL COMMENT '����id', \
					KEY `user_friendId` (`userId`,`friendId`)\
					) ENGINE=MyISAM DEFAULT CHARSET=utf8;",i);
		string strSqlTmp1(buf);
		if (g_cMagicServerDb.execSQL(strSqlTmp1) != 0)
		{
			cerr<<"strSql:"<<strSqlTmp1<<endl;
			return false;
		}
	}
	return true;
}

//����20��m_userFriendInfo
bool cMagicServerDb::userCreateUserFriendReqInfo()
{
	for (int i = 0; i < 20 ;i++)
	{
		char buf[1024] = {0};
		memset(buf,0,sizeof(buf));
		sprintf(buf,"drop table if exists m_userfriendreqinfo%d",i);	
		string strTmpSql(buf);
		if (g_cMagicServerDb.execSQL(strTmpSql) != 0)
		{
			cerr<<"strSql:"<<strTmpSql<<endl;
			//return false;
		}

		memset(buf,0,sizeof(buf));
		sprintf(buf,"CREATE TABLE IF NOT EXISTS `m_userfriendreqinfo%d` (\
			`userId` int(11) NOT NULL COMMENT '�û�id',\
			`reqFriendId` int(11) NOT NULL COMMENT '��������û�id',\
			KEY `reqFriend_userId` (`reqFriendId`,`userId`)\
			) ENGINE=MyISAM DEFAULT CHARSET=utf8;",i);
		string strSqlTmp1(buf);
		if (g_cMagicServerDb.execSQL(strSqlTmp1) != 0)
		{
			cerr<<"strSql:"<<strSqlTmp1<<endl;
			return false;
		}
	}
	return true;
}

//����100m_EquipTmpSaveInfo��
bool cMagicServerDb::creatEquipTmpSaveInfo()
{
	for (int i = 0 ;i < 100; i++)
	{
		char buf[1024] = {0};
		memset(buf,0,sizeof(buf));
		sprintf(buf,"drop table if exists m_equiptmpsaveinfo%d",i);
		string strTmpSql(buf);
		if (g_cMagicServerDb.execSQL(strTmpSql) != 0)
		{
			cerr<<"strSql:"<<strTmpSql<<endl;
			return false;
		}

		memset(buf,0,sizeof(buf));
		sprintf(buf,"CREATE TABLE IF NOT EXISTS `m_equiptmpsaveinfo%d` ( \
					`userId` int(11) NOT NULL COMMENT '�û�ID',\
					`equipTypeId` tinyint(3) NOT NULL COMMENT 'װ������Id',\
					`equipMentId` varchar(6) NOT NULL COMMENT 'װ��ID',\
					`count` int(11) NOT NULL DEFAULT '0' COMMENT 'װ������', \
					`isShouldDelete` tinyint(1) NOT NULL DEFAULT '0' COMMENT '�ü�¼�Ƿ�Ӧ�ñ�ɾ��',\
					KEY `user_isShouldDelete` (`userId`,`isShouldDelete`) \
					) ENGINE=MyISAM DEFAULT CHARSET=utf8;",i);
		string strSqlTmp1(buf);
		if (g_cMagicServerDb.execSQL(strSqlTmp1) != 0)
		{
			cerr<<"strSql:"<<strSqlTmp1<<endl;
			return false;
		}

	}
	return true;
}

//����100��m_CardTmpSaveInfo��
bool cMagicServerDb::createCardTmpSaveInfo()
{
	for (int i = 0; i < 100 ;i++)
	{
		char buf[1024] = {0};
		memset(buf,0,sizeof(buf));
		sprintf(buf,"drop table if exists m_cardtmpsaveinfo%d",i);
		string strTmpSql(buf);
		if (g_cMagicServerDb.execSQL(strTmpSql) != 0)
		{
			cerr<<"strSql:"<<strTmpSql<<endl;
			//return false;
		}

		memset(buf,0,sizeof(buf));
		sprintf(buf,"CREATE TABLE IF NOT EXISTS `m_cardtmpsaveinfo%d` ( \
					`userId` int(11) NOT NULL COMMENT '�û�ID', \
					`cardID` varchar(6) NOT NULL COMMENT '����id', \
					`cardLevel` tinyint(3) NOT NULL DEFAULT 1 COMMENT '���Ƶȼ�',\
					`isShouldDelete` tinyint(1) NOT NULL DEFAULT '0' COMMENT '�ü�¼�Ƿ�Ӧ�ñ�ɾ��', \
					KEY `user_isShouldDelete` (`userId`,`isShouldDelete`) \
					) ENGINE=MyISAM DEFAULT CHARSET=utf8;",i);

		string strTmpSql1(buf);
		if (g_cMagicServerDb.execSQL(strTmpSql1) != 0)
		{
			cerr<<"strSql:"<<strTmpSql<<endl;
			return false;
		}
	}
	return true;
}

//������ҵ�����ʱ�洢��
bool cMagicServerDb::createGoldTmpSaveInfo()
{
	char buf[1024] = {0};
	sprintf(buf,"drop table if exists m_goldtmpsaveinfo");
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"strSql:"<<strSql<<endl;
		return false;
	}
	memset(buf,0,sizeof(buf));
	sprintf(buf,"CREATE TABLE IF NOT EXISTS `m_goldtmpsaveinfo` (\
		`userId` int(11) NOT NULL COMMENT '�û�id',\
		`goldNumber` int NOT NULL COMMENT '�������Ľ��',\
		`gotExp` int NOT NULL COMMENT '���ͨ�ػ�õľ���ֵ',\
		PRIMARY KEY (`userId`)\
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;");
	string strTmpSql(buf);
	if (g_cMagicServerDb.execSQL(strTmpSql) != 0)
	{
		cerr<<"strSql:"<<strTmpSql<<endl;
		return false;
	}
	return true;
}

//�û��������
void cMagicServerDb::createUserActiveBattleInfo(const int& u_tableCount)
{
	char buf[1024] = {0};
	for(int i = 0; i < u_tableCount; i++)
	{
		sprintf(buf,"drop table if exists m_useractivebattleinfo%d",i);
		string strSql(buf);
		if (g_cMagicServerDb.execSQL(strSql) != 0)
		{
			cerr<<"createUserActiveBattleInfo error"<<endl;
			cerr<<strSql<<endl;
			return;
		}
		strSql.clear();
		memset(buf,0,1024);
		sprintf(buf,"create table if not exists `m_useractivebattleinfo%d`(\
					`userId` int(11) not null comment '�û�id',\
					`activeBattleId`varchar(32) not null comment '�����id',\
					`activeBattleCount` tinyint(3) NOT NULL DEFAULT 1 comment '�����ʣ�����',\
					UNIQUE `user_activeBattleId` (`userId`,`activeBattleId`)\
					)ENGINE=MyISAM DEFAULT CHARSET=utf8;",i);
		strSql += buf;
		if (g_cMagicServerDb.execSQL(strSql) != 0)
		{
			cerr<<"createUserActiveBattleInfo error"<<endl;
			cerr<<strSql<<endl;
			return;
		}
	}
	
}

//�޸��û�card���ֶ�
void cMagicServerDb::modifyUserCardTableField(int i)
{
	char buf[128] = {0};
	sprintf(buf,"ALTER TABLE `m_usercardinfo%d` DROP `goldValue`;",i);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql))
	{
		cerr<<"strSql:"<<strSql<<endl;
	}

	memset(buf,0,sizeof(buf));
	sprintf(buf,"ALTER TABLE `m_usercardinfo%d` ADD `cardStarLevel` INT NOT NULL COMMENT '�����Ǽ�' \
				AFTER `skillLevel`;",i);
	string strSqlTmp(buf);
	if (g_cMagicServerDb.execSQL(strSqlTmp))
	{
		cerr<<"strSqlTmp:"<<strSqlTmp<<endl;
	}
}

//�����������
void cMagicServerDb::createInvitationCodeTable()
{
	char buf[1024] = {0};
	sprintf(buf,"drop table if exists m_invitation_code_table;");
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"strSql:"<<strSql<<endl;
		return;
	}
	memset(buf,0,sizeof(buf));
	sprintf(buf,"create table if not exists m_invitation_code_table(\
				Id int(11) not null AUTO_INCREMENT comment '������Id',\
				invitationCode varchar(32) not null comment '������',\
				usedCount tinyint not null default 0 comment '����������ʹ�õĴ���',\
				primary key (Id),\
				unique invitationCode (invitationCode))ENGINE=MyISAM DEFAULT CHARSET=utf8;");
	strSql.clear();
	strSql += buf;
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"11strSql:"<<strSql<<endl;
		return;
	}

}

void cMagicServerDb::insertInvitationCodeToTable()
{
	string u_strInvitationCode = "";
	u_strInvitationCode = generateInvitationCode(9);
	char buf[512] = {0};
	sprintf(buf,"insert into m_invitation_code_table(Id,invitationCode,usedCount) \
				values(NULL,'%s',0);",u_strInvitationCode.c_str());
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"insertInvitationCodeToTable error"<<endl;
		cerr<<strSql<<endl;
		return;
	}
}

void cMagicServerDb::insertInvitationCodeToTable(int u_count)
{
	int i=0;
	for (i = 0; i < u_count; i++)
	{
		insertInvitationCodeToTable();
	}
}


void cMagicServerDb::printHasNoCardUser()
{
	char buf[512] = {0};
	sprintf(buf,"select userID from m_gameuserinfo;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	if (q.eof())
	{
		cerr<<"printHasNoCardUser error,strSql:"<<strSql<<endl;
		return;
	}
	while (!q.eof())
	{
		int u_userId = q.getIntFileds("userID");
		memset(buf,0,sizeof(buf));
		sprintf(buf,"select cardId from m_usercardinfo%d;",u_userId%100);
		string strSql1(buf);
		MysqlQuery q1 = g_cMagicServerDb.execQuery(strSql1);
		if (q1.eof())
		{
			cerr<<"userId:"<<u_userId<<" m_usercardinfo"<<u_userId%100<<endl;
		}
		q1.finalize();
		q.nextRow();
	}
	q.finalize();
}

void cMagicServerDb::clearUserCardinfo(const int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_usercardinfo%d where userID = %d;",u_userId%100,u_userId);
	string strSql(buf);
	if(g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"clearUserCardinfo error:strSql:%s"<<strSql<<endl;
	}
}
void cMagicServerDb::clearUserEquipInfo(const int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_userequipinfo%d where userId = %d;",u_userId%20,u_userId);
	string strSql(buf);
	if(g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"clearUserEquipInfo error:strSql:%s"<<strSql<<endl;
	}
}

void cMagicServerDb::clearUserFriendInfo(const int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_userfriendinfo%d where userId = %d;",u_userId%20,u_userId);
	string strSql(buf);
	if(g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"clearUserFriendInfo error:strSql:%s"<<strSql<<endl;
	}
}

void cMagicServerDb::clearUserFriendReqInfo(const int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_userfriendreqinfo%d where reqFriendId = %d;",u_userId%20,u_userId);
	string strSql(buf);
	if(g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"clearUserFriendInfo error:strSql:%s"<<strSql<<endl;
	}
}

void cMagicServerDb::clearUserMissionInfo(const int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_usermissionsinfo%d where userId = %d;",u_userId%100,u_userId);
	string strSql(buf);
	if(g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"clearUserMissionInfo error:strSql:%s"<<strSql<<endl;
	}
}

void cMagicServerDb::clearUserTokenInfo(const int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_usertokeninfo where userId = %d;",u_userId);
	string strSql(buf);
	if(g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"clearUserTokenInfo error:strSql:%s"<<strSql<<endl;
	}
}

void cMagicServerDb::clearUserinfo(const int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_gameuserinfo where userID = %d;",u_userId);
	string strSql(buf);
	if(g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"clearUserinfo error:strSql:%s"<<strSql<<endl;
	}
}

void cMagicServerDb::clearDataBase()
{
	char buf[128] = {0};
	//sprintf(buf,"select * from m_gameuserinfo where userName like 'Name%';");
	sprintf(buf,"select * from m_gameuserinfo where userName like 'Name%';");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	if (q.eof())
	{
		cerr<<"clearDataBase error:strSql:"<<strSql<<endl;
		return;
	}
	while (!q.eof())
	{
		clearUserCardinfo(q.getIntFileds("userID"));
		clearUserEquipInfo(q.getIntFileds("userID"));
		clearUserFriendInfo(q.getIntFileds("userID"));
		clearUserFriendReqInfo(q.getIntFileds("userID"));
		clearUserMissionInfo(q.getIntFileds("userID"));
		clearUserTokenInfo(q.getIntFileds("userID"));
		clearUserinfo(q.getIntFileds("userID"));
		q.nextRow();
	}
	q.finalize();
}

bool cMagicServerDb::isCardIdExist(const string& u_strCardId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from cardinfo where cardId = '%s';",u_strCardId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	if (q.eof())
	{
		cerr<<"not find "<<u_strCardId<<"in cardinfo"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
		return false;
	}
	return true;
}

void cMagicServerDb::deleteUserCard(const unsigned int& u_userId,const string& u_strCardId)
{
	char buf[256] = {0};
	sprintf(buf,"delete from m_usercardinfo%d where userID=%d and cardId='%s';",\
		u_userId%100,u_userId,u_strCardId.c_str());
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"deleteUserCard error,strSql:"<<strSql<<endl;
	}
}

void cMagicServerDb::clearUserNotExistCard(const unsigned int& u_userId)
{
	char buf[256] = {0};
	sprintf(buf,"select cardId from m_usercardinfo%d where userID=%d;",\
		u_userId%100,u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	if (q.eof())
	{
		cerr<<"2222clearUserNotExistCard error,strSql:"<<strSql<<endl;
		return;
	}
	while (!q.eof())
	{
		if (!isCardIdExist(q.getStringFileds("cardId")))
		{
			deleteUserCard(u_userId,q.getStringFileds("cardId"));
		}
		q.nextRow();
	}
	q.finalize();
}

void cMagicServerDb::clearUserNotExistCard()
{
	char buf[128] = {0};
	sprintf(buf,"select userID from m_gameuserinfo;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	if (q.eof())
	{
		cerr<<"1111clearUserNotExistCard error,strSql:"<<strSql<<endl;
		return;
	}
	while (!q.eof())
	{
		clearUserNotExistCard(q.getIntFileds("userID"));
		q.nextRow();
	}
	q.finalize();
	return;
}

void cMagicServerDb::updataUserLeaderValue(const unsigned int& u_userId,const int& u_userLevel)
{
	int u_userLeaderValue = 0;
	u_userLeaderValue = LEADER_VALUE + u_userLevel - 1;
	char buf[256] = {0};
	sprintf(buf,"update m_gameuserinfo set leaderValue=%d where userID=%d;",u_userLeaderValue,u_userId);
	string strSql(buf);
	if(g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"updataUserLeaderValue error,strSql:"<<strSql<<endl;
	}
}

void cMagicServerDb::updataUserLeaderValue()
{
	char buf[128] = {0};
	sprintf(buf,"select userID,userLevel from m_gameuserinfo;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	if (q.eof())
	{
		cerr<<"updataUserLeaderValue error,strSql:"<<strSql<<endl;
		return;
	}
	while (!q.eof())
	{
		updataUserLeaderValue(q.getIntFileds("userID"),q.getIntFileds("userLevel"));
		q.nextRow();
	}
	q.finalize();

}

void cMagicServerDb::alterUserMissionsTableIndex()
{
	for (int i = 0; i < 100; i++)
	{
		char buf[128] = {0};
		sprintf(buf,"alter table m_usermissionsinfo%d drop index user_MissionId;",i);
		string strSql(buf);
		if (g_cMagicServerDb.execSQL(strSql) != 0)
		{
			cerr<<"alterUserMissionsTableIndex drop index error,strSql:"<<strSql<<endl;
			return;
		}
		memset(buf,0,128);
		strSql.clear();
		sprintf(buf,"alter table m_usermissionsinfo%d add index user_MissionId (`userId`,`missionId`);",i);
		strSql += buf;
		
		if (g_cMagicServerDb.execSQL(strSql) != 0)
		{
			cerr<<"alterUserMissionsTableIndex add index error,strSql:"<<strSql<<endl;
			return;
		}
	}
}



void cMagicServerDb::mysqlPressureTest()
{
	int u_choice = 0;
	int u_Numbers = 0;
	cerr<<"1 for insert,2 for update,3 for select,4 for delete"<<endl;
	cerr<<"input you choice:"<<endl;
	
	cin>>u_choice;

	if (u_choice == 1)
	{
		cerr<<"input insert Numbers:"<<endl;
		cin>>u_Numbers;
		struct timeval tvBegin1,tvEnd1;
		gettimeofday(&tvBegin1,NULL);
		for (int i = 1; i <= u_Numbers; i++)
		{
			char buf[128] = {0};
			sprintf(buf,"insert into missionsinfo_test(missionId) values(%d);",i);
			string strSql(buf);
			struct timeval tvBegin,tvEnd;
			gettimeofday(&tvBegin,NULL);
			if (g_cMagicServerDb.execSQL(strSql) != 0)
			{
				cerr<<"mysqlPressureTest insert error,strSql:"<<strSql<<endl;
				return;
			}
			gettimeofday(&tvEnd,NULL);
			int u_useTime = (tvEnd.tv_sec*1000000+tvEnd.tv_usec)-(tvBegin.tv_sec*1000000+tvBegin.tv_usec);
			cerr<<"insert "<<i<<" needTime:"<<u_useTime<<endl;
		}
		gettimeofday(&tvEnd1,NULL);
		int u_useTime1 = (tvEnd1.tv_sec*1000000+tvEnd1.tv_usec)-(tvBegin1.tv_sec*1000000+tvBegin1.tv_usec);
		cerr<<"insert "<<u_Numbers<<" Rows needTime:"<<u_useTime1<<endl;
		
	}
	else if (u_choice == 2)
	{
		cerr<<"input update Numbers:"<<endl;
		cin>>u_Numbers;
		struct timeval tvBegin1,tvEnd1;
		gettimeofday(&tvBegin1,NULL);
		for (int i = 1; i <= u_Numbers; i++)
		{
			char buf[128] = {0};
			sprintf(buf,"update missionsinfo_test set missionId=%d where missionId=%d;",i,i);
			string strSql(buf);
			struct timeval tvBegin,tvEnd;
			gettimeofday(&tvBegin,NULL);
			if (g_cMagicServerDb.execSQL(strSql) != 0)
			{
				cerr<<"mysqlPressureTest update error,strSql:"<<strSql<<endl;
				return;
			}
			gettimeofday(&tvEnd,NULL);
			int u_useTime = (tvEnd.tv_sec*1000000+tvEnd.tv_usec)-(tvBegin.tv_sec*1000000+tvBegin.tv_usec);
			cerr<<"update "<<i<<" needTime:"<<u_useTime<<endl;
		}
		gettimeofday(&tvEnd1,NULL);
		int u_useTime1 = (tvEnd1.tv_sec*1000000+tvEnd1.tv_usec)-(tvBegin1.tv_sec*1000000+tvBegin1.tv_usec);
		cerr<<"update "<<u_Numbers<<" Rows needTime:"<<u_useTime1<<endl;

	}
	else if (u_choice == 3)
	{
		cerr<<"input select Numbers:"<<endl;
		cin>>u_Numbers;
		struct timeval tvBegin1,tvEnd1;
		gettimeofday(&tvBegin1,NULL);
		for (int i = 1; i <= u_Numbers; i++)
		{
			char buf[128] = {0};
			sprintf(buf,"select * from missionsinfo_test where missionId=%d;",i);
			string strSql(buf);
			struct timeval tvBegin,tvEnd;
			gettimeofday(&tvBegin,NULL);
			MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
			if (q.eof())
			{
				cerr<<"mysqlPressureTest select error,strSql:"<<strSql<<endl;
				q.finalize();
				return;
			}
			gettimeofday(&tvEnd,NULL);
			int u_useTime = (tvEnd.tv_sec*1000000+tvEnd.tv_usec)-(tvBegin.tv_sec*1000000+tvBegin.tv_usec);
			cerr<<"select "<<i<<" needTime:"<<u_useTime<<endl;
		}
		gettimeofday(&tvEnd1,NULL);
		int u_useTime1 = (tvEnd1.tv_sec*1000000+tvEnd1.tv_usec)-(tvBegin1.tv_sec*1000000+tvBegin1.tv_usec);
		cerr<<"select "<<u_Numbers<<" Rows needTime:"<<u_useTime1<<endl;

	}
	else if (u_choice == 4)
	{
		cerr<<"input delete Numbers:"<<endl;
		cin>>u_Numbers;
		struct timeval tvBegin1,tvEnd1;
		gettimeofday(&tvBegin1,NULL);
		for (int i = 1; i <= u_Numbers; i++)
		{
			char buf[128] = {0};
			sprintf(buf,"delete from missionsinfo_test where missionId=%d;",i);
			string strSql(buf);
			struct timeval tvBegin,tvEnd;
			gettimeofday(&tvBegin,NULL);
			if (g_cMagicServerDb.execSQL(strSql) != 0)
			{
				cerr<<"mysqlPressureTest delete error,strSql:"<<strSql<<endl;
				return;
			}
			gettimeofday(&tvEnd,NULL);
			int u_useTime = (tvEnd.tv_sec*1000000+tvEnd.tv_usec)-(tvBegin.tv_sec*1000000+tvBegin.tv_usec);
			cerr<<"delete "<<i<<" needTime:"<<u_useTime<<endl;
		}
		gettimeofday(&tvEnd1,NULL);
		int u_useTime1 = (tvEnd1.tv_sec*1000000+tvEnd1.tv_usec)-(tvBegin1.tv_sec*1000000+tvBegin1.tv_usec);
		cerr<<"delete "<<u_Numbers<<" Rows needTime:"<<u_useTime1<<endl;
	}
	
	return;
}

void cMagicServerDb::updateUserCapitorCapacityClass()
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_user_fight_capacity;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	if (q.eof())
	{
		cerr<<"updateUserCapitorCapacityClass failure:"<<strSql<<endl;
		return;
	}
	while (!q.eof())
	{
		if(!updateUserFightCapacity(q.getUnIntFileds("userId"),q.getUnIntFileds("fightCapacity"),q.getIntFileds("capitorFightCapacity")))
		{
			break;
		}
		q.nextRow();
	}
	q.finalize();
	return;
}

bool cMagicServerDb::createChapterInfo()
{
	char buf[1024] = {0};
	sprintf(buf,"drop table if exists m_chapterinfo;");
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"createChapterInfo failure strSql:"<<strSql<<endl;
		return false;
	}
	memset(buf,0,sizeof(buf));
	sprintf(buf,"create table if not exists m_chapterinfo(\
				Id varchar(32) not null comment '�󸱱�Id',\
				ChapterName varchar(32) not null comment '�󸱱�����',\
				LevelLimited int not null default 1 comment '�����ȼ�����',\
				EnergyUse int not null default 0 comment '��Ҫ����',\
				primary key (Id),\
				unique Id (Id))ENGINE=MyISAM DEFAULT CHARSET=utf8;");
	string strSql1(buf);
	if (g_cMagicServerDb.execSQL(strSql1) != 0)
	{
		cerr<<"createChapterInfo failure strSql:"<<strSql<<endl;
		return false;
	}
	return true;
}

bool cMagicServerDb::insertIntoChapterInfo(MysqlQuery& q)
{
	char buf[1024] = {0};
	string u_Id = q.getStringFileds("Id");
	string u_ChapterName = q.getStringFileds("ChapterName");
	int u_LevelLimited = q.getIntFileds("LevelLimited");
	int u_EnergyUse = q.getIntFileds("EnergyUse");
	sprintf(buf,"insert into m_chapterinfo(Id,ChapterName,LevelLimited,EnergyUse) \
				values('%s','%s',%d,%d)",u_Id.c_str(),u_ChapterName.c_str(),u_LevelLimited,u_EnergyUse);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"insertIntoChapterInfo failure:strSql:"<<strSql<<endl;
		return false;
	}
	return true;
}

void cMagicServerDb::insertChapterInfo()
{
	if(createChapterInfo())
	{
		char buf[128] = {0};
		sprintf(buf,"select * from chapterinfo;");
		string strSql(buf);
		MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
		if (q.eof())
		{
			cerr<<"insertChapterInfo failure:"<<strSql<<endl;
			q.finalize();
			return;
		}
		while (!q.eof())
		{
			if (!insertIntoChapterInfo(q))
			{
				q.finalize();
				return;
			}
			q.nextRow();
		}
		q.finalize();
		cerr<<"insertChapterInfo success"<<endl;
		return;
	}
}

void* ThreadSelect(void* pArg)
{
	int u_sleep = *(int*)pArg;
	while(1)
	{
		char buf[128] = {0};
		sprintf(buf,"select * from test_select_table;");
		string strSql(buf);
		MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
		if (q.eof())
		{
			cerr<<"ThreadSelect error:"<<strSql<<endl;
			cerr<<"ThreadSelect strerror:"<<strerror(errno)<<endl;
			//pthread_exit(0);
		}
		usleep(u_sleep);
	}
	pthread_exit(0);
	
}

void* ThreadInsert(void* pArg)
{
	int u_sleep = *(int*)pArg;
	int i = 2;
	while(1)
	{
		char buf[128] = {0};
		sprintf(buf,"insert into test_insert_table(Id) values(%d);",i);
		string strSql(buf);
		if (g_cMagicServerDb.execSQL(strSql) != 0)
		{
			cerr<<"ThreadInsert error:"<<strSql<<endl;
			cerr<<"ThreadInsert strerror:"<<strerror(errno)<<endl;
			//pthread_exit(0);
		}
		usleep(u_sleep);
		i += 1;
	}
	pthread_exit(0);

}


void cMagicServerDb::CreateSelectAndInsertThread()
{
	int u_sleep = 0;
	cout<<"input sleep time:"<<endl;
	cin>>u_sleep;
	//�߳�id
	pthread_t id;
	//�߳�����
	pthread_attr_t   attr;
	//��ʼ��
	pthread_attr_init(&attr);
	//�����߳�
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	//��ѯ�����߳�
	int nret=pthread_create(&id,&attr,ThreadSelect,(void*)&u_sleep);
	//�ɹ�
	if(nret!=0)
	{
		//ʧ��
		printf("pthread ThreadSelect failure:errno:%d,err:%s\n",errno,strerror(errno));
		return;
	}

	//���������߳�
	nret=pthread_create(&id,&attr,ThreadInsert,(void*)&u_sleep);
	//�ɹ�
	if(nret!=0)
	{
		//ʧ��
		printf("pthread ThreadInsert failure:errno:%d,err:%s\n",errno,strerror(errno));
		return;
	}
	pthread_attr_destroy(&attr);
	while(1)
	{
		sleep(2);
	}
}

void cMagicServerDb::mysql_proxy_test()
{
	char buf[128] = {0};
	int i = 19;
	while (1)
	{
		memset(buf,0,sizeof(buf));
		sprintf(buf,"select * from a_table where Id=3;");
		string strSql(buf);
		MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
		if (q.eof())
		{
			cerr<<"getnothing strSql:"<<strSql<<endl;
		}
		q.finalize();
		memset(buf,0,sizeof(buf));
		strSql.clear();
		sprintf(buf,"insert into a_table(Id) values(%d);",i);
		strSql += buf;
		if (g_cMagicServerDb.execSQL(strSql) != 0)
		{
			cerr<<"insert error strSql:"<<strSql<<endl;
		}
		i++;
		sleep(1);
	}
	
}

void cMagicServerDb::mysql_master_slave_test()
{
	char buf[128];
	sprintf(buf,"select * from a_table where Id=3;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.execQuery(strSql);
	if (q.eof())
	{
		cerr<<"get nothing from slaveDbHost:"<<strSql<<endl;
	}
	q.finalize();
	MysqlQuery q1 = g_cMagicServerDb.execQuery(strSql);
	if (q1.eof())
	{
		cerr<<"get nothing from masterDbHost,strSql:"<<strSql<<endl;
	}
	else
	{
		cerr<<"get data from masterDbHost:"<<q1.getIntFileds("Id")<<endl;
	}
	q1.finalize();
}

//Ϊm_userFriendInfo%d���ʹ�ú���ʱ���useFriendStamp
void cMagicServerDb::addFieldForM_userFriendInfo()
{
	int i = 0;
	for (;i<20;i++)
	{
		char buf[128] = {0};
		sprintf(buf,"alter table m_userFriendInfo%d add useFriendStamp int(11) not null default 0;",i%20);
		string strSql(buf);
		if (g_cMagicServerDb.execSQL(strSql) != 0)
		{
			cerr<<"addFieldForM_userFriendInfo error,strSql:"<<strSql<<endl;
		}
	}
	return;
}


//�����Ƭ��Ϣ
void cMagicServerDb::cleanUserFragmentInfo(const unsigned int& u_userId,const int& u_type,const string& u_strId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_userequipinfo%d  where userId = %d and equipMentId = '%s';", \
		u_userId%20,u_userId,u_strId.c_str());
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		printf("cleanUserFragmentInfo failure,strSql:%s\n",strSql.c_str());
	}
	return;
}

//������Ƭ��Ϣ
void cMagicServerDb::insertUserFragment(const unsigned int& u_userId,const int& u_type,\
						const string& u_strId,const int& u_count)
{
	char buf[512] = {0};
	sprintf(buf,"insert into m_userEquipInfo%d(userId,equipTypeId,equipMentId,count) \
				values(%d,%d,'%s',%d);",u_userId%20,u_userId,u_type,\
				u_strId.c_str(),u_count);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		printf("insertUserFragment error,strSql:%s\n",strSql.c_str());
	}
	return;
}


//�ϲ���ͬ����Ƭ
void cMagicServerDb::mergeSameIdFragmentByUserIdAndFragmentId(const unsigned int& u_userId,const string& u_fragmentId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_userequipinfo%d where userId=%d and equipMentId='%s';",\
		u_userId%20,u_userId,u_fragmentId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	int n_fragmentCount = 0;
	int n_type = 0;
	while (!q.eof())
	{
		n_fragmentCount += q.getIntFileds("count");
		n_type = q.getIntFileds("equipTypeId");
		q.nextRow();
	}
	if (n_type != 0)
	{
		cleanUserFragmentInfo(u_userId,n_type,u_fragmentId);
		insertUserFragment(u_userId,n_type,u_fragmentId,n_fragmentCount);
	}
	q.finalize();
	return;
}

//�ϲ���ͬ����Ƭ
void cMagicServerDb::mergeSameIdFragmentByUserId(const unsigned int& n_userId)
{
	mergeSameIdFragmentByUserIdAndFragmentId(n_userId,"1000");
	mergeSameIdFragmentByUserIdAndFragmentId(n_userId,"1001");
	mergeSameIdFragmentByUserIdAndFragmentId(n_userId,"1002");
	mergeSameIdFragmentByUserIdAndFragmentId(n_userId,"1003");
	mergeSameIdFragmentByUserIdAndFragmentId(n_userId,"1004");
	mergeSameIdFragmentByUserIdAndFragmentId(n_userId,"1005");
	mergeSameIdFragmentByUserIdAndFragmentId(n_userId,"1006");
	mergeSameIdFragmentByUserIdAndFragmentId(n_userId,"1007");
	mergeSameIdFragmentByUserIdAndFragmentId(n_userId,"1008");
	mergeSameIdFragmentByUserIdAndFragmentId(n_userId,"1009");
	mergeSameIdFragmentByUserIdAndFragmentId(n_userId,"1010");
	mergeSameIdFragmentByUserIdAndFragmentId(n_userId,"1011");

	return;
}

//�ϲ���Ƭ��Ϣ
void cMagicServerDb::mergeSameIdFragment()
{
	char buf[128] = {0};
	sprintf(buf,"select userID from m_gameuserinfo;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	while (!q.eof())
	{
		unsigned int n_userId = q.getIntFileds("userID");
		mergeSameIdFragmentByUserId(n_userId);
		q.nextRow();
	}
	q.finalize();
	return;
}


//��ȡ��Ҫ�޸ĵ�����Id
MysqlQuery cMagicServerDb::getModifiedMissionId()
{
	char buf[128] = {0};
	sprintf(buf,"select modifiedId from missionIdNeedModified_table;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.execQuery(strSql);
	return q;
}

//��������Id
void cMagicServerDb::insertUserMissionId(const unsigned int& u_userId,const int& u_missionId)
{
	char buf[256] = {0};
	sprintf(buf,"select * from missioninfo where Id=%d;",u_missionId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.execQuery(strSql);
	if (!q.eof())
	{
			int nMissionId = q.getIntFileds("Id");
			int nMissionType = q.getIntFileds("ConditionType");
			string u_missionCondition = q.getStringFileds("ConditionValue");
			string u_hortationType = q.getStringFileds("HortationType");
			int u_hortationCount = q.getIntFileds("HortationValue");
			bool isFinished = false;
			//�Ȳ�����Ϣ���ɹ����������Ϣ
			memset(buf,0,256);
			sprintf(buf,"insert into m_userMissionsInfo%d(userId,missionId,missinType,missionCondition,hortationType,hortationCount,isFinished) \
						values(%d,%d,%d,'%s','%s',%d,0)",u_userId%100,u_userId,\
						nMissionId,nMissionType,u_missionCondition.c_str(),u_hortationType.c_str(),u_hortationCount);
			string strSql(buf);
			if (execSQL(strSql) != 0)
			{
				cerr<<"insertUserMissionId error,strSql:"<<strSql.c_str()<<endl;
			}
	}
	q.finalize();

}

//�޸��û�������Ϣ
void cMagicServerDb::modifyUserMissionInfo()
{
	char buf[128] = {0};
	sprintf(buf,"select userID from m_gameuserinfo;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	while (!q.eof())
	{
		unsigned int n_userId = q.getIntFileds("userID");
		MysqlQuery q1 = getModifiedMissionId();
		if (q1.eof())
		{
			q1.finalize();
			cerr<<"there is no missionId to modify"<<endl;
			break;
		}
		while (!q1.eof())
		{
			if (isUserMissionExist(n_userId,q1.getIntFileds("modifiedId")))
			{
				deleteUserMissionId(n_userId,q1.getIntFileds("modifiedId"));
				insertUserMissionId(n_userId,q1.getIntFileds("modifiedId"));
			}
			q1.nextRow();
		}
		q1.finalize();
		q.nextRow();
	}
	q.finalize();
	cerr<<"finish modifyUserMissionInfo"<<endl;
	return;
}

//��ȡ��Ҫ�����ӵ�����Id
MysqlQuery cMagicServerDb::getNewAddMissionId()
{
	char buf[128] = {0};
	sprintf(buf,"select Id from new_add_mission_id_table;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.execQuery(strSql);
	return q;
}

//�����û�����
void cMagicServerDb::AddUserMissionInfo()
{
	char buf[128] = {0};
	sprintf(buf,"select userID from m_gameuserinfo;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	while (!q.eof())
	{
		unsigned int n_userId = q.getIntFileds("userID");
		MysqlQuery q1 = getNewAddMissionId();
		if (q1.eof())
		{
			q1.finalize();
			cerr<<"there is no missionId to Add"<<endl;
			break;
		}
		while (!q1.eof())
		{
			insertUserMissionId(n_userId,q1.getIntFileds("Id"));
			q1.nextRow();
		}
		q1.finalize();
		q.nextRow();
	}
	q.finalize();
	cerr<<"finish AddUserMissionInfo"<<endl;
	return;
}


///Ϊ��ҿ��Ʊ��ֶ�VTLGrow,STRGrow,TGHGrow,\
QUIGrow,MGCGrow��������tinyint��Ϊsmallint
void cMagicServerDb::modifyUserCardInfoFieldAttribute()
{
	for (int i = 0; i < 100; i++)
	{
		char buf[128] = {0};
		sprintf(buf,"alter table m_usercardinfo%d modify column VTLGrow smallint(6);",i);
		string strSql(buf);
		if (g_cMagicServerDb.execSQL(strSql) != 0)
		{
			cerr<<"modifyUserCardInfoFieldAttribute error,strSql:%s"<<strSql<<endl;
			return;
		}
		
		memset(buf,0,128);
		sprintf(buf,"alter table m_usercardinfo%d modify column STRGrow smallint(6);",i);
		string strSql1(buf);
		if (g_cMagicServerDb.execSQL(strSql1) != 0)
		{
			cerr<<"modifyUserCardInfoFieldAttribute error,strSql:%s"<<strSql1<<endl;
			return;
		}
		memset(buf,0,128);
		sprintf(buf,"alter table m_usercardinfo%d modify column TGHGrow smallint(6);",i);
		string strSql2(buf);
		if (g_cMagicServerDb.execSQL(strSql2) != 0)
		{
			cerr<<"modifyUserCardInfoFieldAttribute error,strSql:%s"<<strSql2<<endl;
			return;
		}
		memset(buf,0,128);
		sprintf(buf,"alter table m_usercardinfo%d modify column QUIGrow smallint(6);",i);
		string strSql3(buf);
		if (g_cMagicServerDb.execSQL(strSql3) != 0)
		{
			cerr<<"modifyUserCardInfoFieldAttribute error,strSql:%s"<<strSql3<<endl;
			return;
		}
		memset(buf,0,128);
		sprintf(buf,"alter table m_usercardinfo%d modify column MGCGrow smallint(6);",i);
		string strSql4(buf);
		if (g_cMagicServerDb.execSQL(strSql4) != 0)
		{
			cerr<<"modifyUserCardInfoFieldAttribute error,strSql:%s"<<strSql4<<endl;
			return;
		}
	}
}
///������¼��ҵ�����ɨ���Ĵ������ѹ�������ҩˮ�����ı�20����
void cMagicServerDb::createTableForRecordUserWipeOutAndBuyEneryCount()
{
	for (int i = 0; i < 20; i++)
	{
		char buf[1024] = {0};
		sprintf(buf,"drop table if exists m_user_wipeout_enery_count%d",i);
		string strTmpSql(buf);
		if (g_cMagicServerDb.execSQL(strTmpSql) != 0)
		{
			cerr<<"createTableForRecordUserWipeOutAndBuyEneryCount strSql:"<<strTmpSql<<endl;
		}
		memset(buf,0,1024);
		sprintf(buf,"CREATE TABLE IF NOT EXISTS `m_user_wipeout_enery_count%d` (\
					`userId` int(11) NOT NULL COMMENT '�û�id',\
					`wipeOutCount` smallint(6) NOT NULL COMMENT '������ɨ���Ĵ���',\
					`buyEnergyCount` smallint(6) NOT NULL COMMENT '�����ѹ�������ҩˮ����',\
					UNIQUE	`userId` (`userId`)\
					) ENGINE=MyISAM DEFAULT CHARSET=utf8;",i);
		string strSqlTmp1(buf);
		if (g_cMagicServerDb.execSQL(strSqlTmp1) != 0)
		{
			cerr<<"createTableForRecordUserWipeOutAndBuyEneryCount strSql:"<<strSqlTmp1<<endl;
		}

	}
	
}

void cMagicServerDb::updateUserEnergyByUserID(const int& u_userId,const int& u_userMaxEnergy)
{
	char buf[128] = {0};
	sprintf(buf,"update m_gameuserinfo set energy=%d,maxEnergy=%d where userID=%d;",\
		u_userMaxEnergy,u_userMaxEnergy,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		cerr<<"updateUserEnergyByUserID error"<<endl;
		cerr<<"strSql:"<<strSql<<endl;
	}
}

void cMagicServerDb::updateUserEnergy()
{
	char buf[128] = {0};
	sprintf(buf,"select userID,userLevel from m_gameuserinfo;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	int u_userMaxEnergy = 0;
	while (!q.eof())
	{
		u_userMaxEnergy = 30+q.getIntFileds("userLevel")-1;
		//u_userMaxEnergy = u_userEnergy;
		updateUserEnergyByUserID(q.getIntFileds("userID"),u_userMaxEnergy);
		q.nextRow();
	}
	q.finalize();
	cerr<<"finish updateUserEnergy"<<endl;
}