#include <iostream>
#include "../include/magicServerDb.h"
#include "../include/listenClientConnect.h"
#include "../include/procClientMessage.h"
#include "../include/deleteTmpData.h"
#include "../include/testServer.h"
//#include "../include/procUserAwardFriend.h"
#include "../include/myGlog.h"
#include "../include/procUserBattleResult.h"
#include "../include/mememCacheClient.h"
#include "../include/procUserShopBuy.h"
#include "../include/updateUserBaseInfo.h"

using namespace std;


void* Thread_mainMysqlKeepAlive(void* pArg)
{
	while(1)
	{
		g_cMagicServerDb.mysqlKeepAlive(2);
		g_cMagicServerDbRead.mysqlKeepAlive(1);
		

		unsigned int u_timeStamp = GetTimeStamp();
		//格式: 2008-05-14 15:12:38
		string strTime = GetTime(u_timeStamp);

		string strHour = strTime.substr(11,2);
		unsigned int u_hour = atoi(strHour.c_str());
		g_cMyGlog.errorlogRecord("u_hour:%d\n",u_hour);

#ifdef DEBUG_MOD
		if (g_cMagicServerDbRead.isSeverExecuteDataUpdate(g_strLocalHost))
		{
			g_cMyGlog.debugLog("ExecuteDataUpdateServerIp:%s\n",g_strLocalHost.c_str());
		}
#endif
		if (u_hour >= 0 && u_hour < 1)
		{
			if (g_cMagicServerDbRead.isSeverExecuteDataUpdate(g_strLocalHost))
			{
				g_cMyGlog.errorlogRecord("updating ActiveBattleInfo Data\n");
				g_cDeleteTmpData.deleteUserActiveBattleInfo();
				g_cUpdateUserBaseInfo.updateUserActiveBattleInfo();
				g_cUpdateUserBaseInfo.clearUserWipeOutAndBuyEnergyCount();
			}
		}
		if (u_hour>=3 && u_hour <4)
		{
			g_cProcUserShopBuy.procNotProccessedOrder();
			if (g_cMagicServerDbRead.isSeverExecuteDataUpdate(g_strLocalHost))
			{
				g_cMyGlog.errorlogRecord("updating Data\n");
				g_cDeleteTmpData.InitDeleteTmpData();
				/*g_cDeleteTmpData.deleteUserActiveBattleInfo();
				g_cUpdateUserBaseInfo.updateUserActiveBattleInfo();*/
				g_cUpdateUserBaseInfo.InitUserGetFriendValueCount();
				g_cUpdateUserBaseInfo.InitUserGetFriendValueFlag();
			}
		}
		sleep(3600);

	}
	pthread_exit((void*)0);
}

bool mainMysqlKeepAlive()
{
	//线程id
	pthread_t id;
	//线程属性
	pthread_attr_t   attr;
	//初始化
	pthread_attr_init(&attr);
	//分离线程
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	//删除临时数据线程
	int nret=pthread_create(&id,&attr,Thread_mainMysqlKeepAlive,NULL);
	//成功
	if(nret!=0)
	{
		//失败
		g_cMyGlog.errorlogRecord("pthread Thread_mainMysqlKeepAlive failure,errno:%d,err:%s\n",\
			errno,strerror(errno));
		return false;
	}
	pthread_attr_destroy(&attr);
	return true;
}

void initNoticeTimePoint()
{
	int u_timeStamp = GetTimeStamp();
	char buf[128] = {0};
	sprintf(buf,"select * from notice_time_point;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		memset(buf,0,sizeof(buf));
		sprintf(buf,"insert into notice_time_point(noticeTimePoint) values(%d);",u_timeStamp);
		string strInsertSql(buf);
		if (g_cMagicServerDb.execSQL(strInsertSql) != 0)
		{
			g_cMyGlog.errorlogRecord("initNoticeTimePoint insert error,strSql:%s\n",strInsertSql.c_str());
		}
	}
	else
	{
		memset(buf,0,sizeof(buf));
		sprintf(buf,"update notice_time_point set noticeTimePoint=%d;",u_timeStamp);
		string strUpdateSql(buf);
		if (g_cMagicServerDb.execSQL(strUpdateSql) != 0)
		{
			g_cMyGlog.errorlogRecord("initNoticeTimePoint update error,strSql:%s\n",strUpdateSql.c_str());
		}
	}	
	q.finalize();
	return;
}

void* Thread_noticeTimePointUpdate(void* pArg)
{
	initNoticeTimePoint();
	while (1)
	{
		int u_timeStamp = GetTimeStamp();
		char buf[128] = {0};
		sprintf(buf,"update notice_time_point set noticeTimePoint=%d;",u_timeStamp);
		string strSql(buf);
		if (g_cMagicServerDb.execSQL(strSql) != 0)
		{
			g_cMyGlog.errorlogRecord("Thread_noticeTimePointUpdate error,strSql:%s\n",strSql.c_str());
		}
#ifdef DEBUG_MOD
		sleep(600);
#else
		sleep(30);
#endif
		
	}
	pthread_exit((void*)0);
}

bool noticeTimePointUpdate()
{
	//线程id
	pthread_t id;
	//线程属性
	pthread_attr_t   attr;
	//初始化
	pthread_attr_init(&attr);
	//分离线程
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	//删除临时数据线程
	int nret=pthread_create(&id,&attr,Thread_noticeTimePointUpdate,NULL);
	//成功
	if(nret!=0)
	{
		//失败
		g_cMyGlog.errorlogRecord("pthread Thread_noticeTimePointUpdate failure,errno:%d,err:%s\n",\
			errno,strerror(errno));
		return false;
	}
	pthread_attr_destroy(&attr);
	return true;
}

int main(int argc,char* argv[])
{
	//数据库连接
	//......
	g_cMyGlog.Init(argv);
	if (getDbServerIp() == -1)
	{
		g_cMyGlog.errorlogRecord("getDbServerIp error\n");
		return -1;
	}
	if (!g_cMagicServerDb.Init())
	{
		g_cMyGlog.errorlogRecord("g_cMagicServerDb.Init() error");
		return -1;
	}
	//从数据连接
	if (!g_cMagicServerDbRead.DbReadInit())
	{
		g_cMyGlog.errorlogRecord("g_cMagicServerDb.DbReadInit() error\n");
		return -1;
	}
	g_strLocalHost = GetIpAddress("eth1",1);
	if (g_strLocalHost.size() <= 0)
	{
		g_cMyGlog.errorlogRecord("get g_strLocalHost failure,size:%d\n",g_strLocalHost.size());
		return -1;
	}

	if (!mainMysqlKeepAlive())
	{
		return -1;
	}
	if (g_cMagicServerDbRead.isSeverExecuteDataUpdate(g_strLocalHost))
	{
		if (!noticeTimePointUpdate())
		{
			return -1;
		}
	}

	if (!g_cProUserBattleResult.loadCardInfoToMem())
	{
		return -1;
	}
	if (!g_mememCacheClient.Init())
	{
		g_mememCacheClient.UnInit();
		g_cMyGlog.errorlogRecord("g_mememCacheClient.Init() failure\n");
	}

	TestServer server(64);    
	server.AddSignalEvent(SIGINT, TestServer::QuitCb);    
	//timeval tv = {10, 0};    
	//server.AddTimerEvent(TestServer::TimeOutCb, tv, false);    
	server.SetPort(60000);    
	server.StartRun();    
	printf("done\n");
	
	g_cMagicServerDb.unInit();
	g_cMagicServerDbRead.unInit();
	g_cMyGlog.UnInit();
	//g_cListenClientConnect.unInit();
	return 0;
}
