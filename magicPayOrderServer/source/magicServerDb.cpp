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




