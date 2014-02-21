#include "../include/magicServerDb.h"
#include "../include/structDef.h"
#include "../include/common.h"
#include "../include/define.h"
#include "../include/errorCode.h"
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
	string strSql = "select * from afficheInfo order by afficheID desc limit 1;";

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	string strAfficheContents = "";
	if(!q.eof())
	{
		strAfficheContents = q.getStringFileds("afficheContents");
		u_PBSystemInfoReceive.set_affiche(strAfficheContents);
		q.nextRow();
	}
	else
	{
		g_cMyGlog.errorlogRecord("PBSystemInfoReceive_setAfficheInfo error,strSql:%s\n",strSql.c_str());
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
	int serverPort = 0;
	int serverStatus = 0;
	string payServerUrl = "";
	int payServerPort = 0;
	PBServer *u_PBServer;
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("PBSystemInfoReceive_setServerInfo error,strSql:%s\n",strSql.c_str());
	}
	while(!q.eof())
	{
		serverName = q.getStringFileds("serverName");
		serverUrl = q.getStringFileds("serverUrl");
		serverPort = q.getIntFileds("serverPort");
		serverStatus = q.getIntFileds("serverStatus");
		payServerUrl = q.getStringFileds("payServerUrl");
		payServerPort = q.getIntFileds("payServerPort");
		u_PBServer = u_PBSystemInfoReceive.add_allserver();
		u_PBServer->set_servername(serverName);
		u_PBServer->set_serverurl(serverUrl);
		//暂定60000
		u_PBServer->set_serverport(serverPort);
		u_PBServer->set_serverstatus(serverStatus);
		if (!payServerUrl.empty())
		{
			u_PBServer->set_payserverurl(payServerUrl);
		}
		if (payServerPort != 0)
		{
			u_PBServer->set_payserverport(payServerPort);
		}
#ifdef DEBUG_MOD
		g_cMyGlog.debugLog("serverName:%s,serverUrl:%s",serverName.c_str(),serverUrl.c_str());
		g_cMyGlog.debugLog("payServerUrl:%s,payServerPort:%d\n",payServerUrl.c_str(),payServerPort);
#endif
		q.nextRow();
	}
	q.finalize();

}


//获取游戏版本信息
void cMagicServerDb::PBSystemInfoReceive_setGameVersion(PBSystemInfoReceive& u_PBSystemInfoReceive)
{
	string strSql = "select gameVersionName from gameVersionInfo order by gameVersionID desc limit 1;";
	string strGameVersionName = "";
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		strGameVersionName = q.getStringFileds("gameVersionName");
		u_PBSystemInfoReceive.set_gameversion(strGameVersionName);
		
	}
	else
	{
		g_cMyGlog.errorlogRecord("PBSystemInfoReceive_setGameVersion error,strSql:%s\n",strSql.c_str());
	}
	q.finalize();
#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("strGameVersionName:%s\n",strGameVersionName.c_str());
	g_cMyGlog.debugLog("u_PBSystemInfoReceive.gameversion:%s\n",u_PBSystemInfoReceive.gameversion().c_str());
#endif
}

//获取资源版本信息
void cMagicServerDb::PBSystemInfoReceive_setResourceVersion(PBSystemInfoReceive& u_PBSystemInfoReceive)
{
	string strSql = "select resourceVersionName from resourceVersionInfo order by resourceVersionID desc limit 1;";
	string strResourceVersionName = "";
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		strResourceVersionName = q.getStringFileds("resourceVersionName");
		u_PBSystemInfoReceive.set_resourceversion(strResourceVersionName);
	}
	else
	{
		g_cMyGlog.errorlogRecord("PBSystemInfoReceive_setResourceVersion error,strSql:%s\n",strSql.c_str());
	}
	q.finalize();
}
//获取副本版本信息
void cMagicServerDb::PBSystemInfoReceive_setBattleVersion(PBSystemInfoReceive& u_PBSystemInfoReceive)
{
	string strSql = "select battleVersionName from battleVersionInfo order by battleVersionId desc limit 1;";
	string strBattleVersionName = "";
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		strBattleVersionName = q.getStringFileds("battleVersionName");
		u_PBSystemInfoReceive.set_battleversion(strBattleVersionName);
	}
	else
	{
		g_cMyGlog.errorlogRecord("PBSystemInfoReceive_setBattleVersion error,strSql:%s\n",strSql.c_str());
	}
	q.finalize();
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

string cMagicServerDb::getNewResourceVersionPath()
{
	char buf[256] = {0};
	sprintf(buf,"select resourcePath from resourceversioninfo order by resourceVersionID desc limit 1;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	string u_retPath = "";
	if (!q.eof())
	{
		u_retPath = q.getStringFileds("resourcePath");
	}
	q.finalize();
	return u_retPath;
}

bool cMagicServerDb::getResorceVersionPath(const string& u_resorceVersionName,string& u_resourceVersionPath)
{
	char buf[256] = {0};
	sprintf(buf,"select resourceVersionName,resourcePath from resourceversioninfo where resourceVersionName='%s';",u_resorceVersionName.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getResorceVersionPath failure,strSql:%s\n",strSql.c_str());
		u_resourceVersionPath.clear();
		u_resourceVersionPath = getNewResourceVersionPath();
	}
	else
	{
		u_resourceVersionPath.clear();
		u_resourceVersionPath = q.getStringFileds("resourcePath");
	}
	
	q.finalize();
	return true;

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




