#include <iostream>
#include "../include/magicServerDb.h"
#include "../include/procClientMessage.h"
#include "../include/testServer.h"
#include "../include/myGlog.h"
#include "../include/procUserShopBuy.h"


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

		if (u_hour>=3 && u_hour <4)
		{
			g_cProcUserShopBuy.procNotProccessedOrder();
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
	int n_payServerPort = getPayServerPortByPayServerUrl(g_strLocalHost);
	if (n_payServerPort == 0)
	{
		g_cMyGlog.errorlogRecord("getPayServerPortByPayServerUrl error,port:%d\n",n_payServerPort);
		return -1;
	}
	if (g_strLocalHost.size() <= 0)
	{
		g_cMyGlog.errorlogRecord("get g_strLocalHost failure,size:%d\n",g_strLocalHost.size());
		return -1;
	}

	if (!mainMysqlKeepAlive())
	{
		return -1;
	}

	TestServer server(64);    
	server.AddSignalEvent(SIGINT, TestServer::QuitCb);    
	//timeval tv = {10, 0};    
	//server.AddTimerEvent(TestServer::TimeOutCb, tv, false);    
	server.SetPort(n_payServerPort);    
	server.StartRun();    
	printf("done\n");
	g_cMagicServerDb.unInit();
	g_cMagicServerDbRead.unInit();

	return 0;
}
