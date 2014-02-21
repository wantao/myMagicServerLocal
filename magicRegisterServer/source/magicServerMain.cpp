#include <iostream>
#include "../include/magicServerDb.h"
#include "../include/listenClientConnect.h"
#include "../include/procClientMessage.h"
#include "../include/deleteTmpData.h"
#include "../include/updateUserBaseInfo.h"
#include "../include/procUserResourceDownLoad.h"
#include "../include/testServer.h"
#include "../include/myGlog.h"

using namespace std;

void* Thread_mainMysqlKeepAlive(void* pArg)
{
	while(1)
	{
		g_cMagicServerDb.mysqlKeepAlive(2);
		g_cMagicServerDbRead.mysqlKeepAlive(1);
		/*sleep(3600);*/
		unsigned int u_timeStamp = GetTimeStamp();
		//��ʽ: 2008-05-14 15:12:38
		string strTime = GetTime(u_timeStamp);

		string strHour = strTime.substr(11,2);
		unsigned int u_hour = atoi(strHour.c_str());
		g_cMyGlog.errorlogRecord("u_hour:%d\n",u_hour);
		if (u_hour > 2 && u_hour < 4)
		{
			g_cMyGlog.errorlogRecord("AAAAAA\n");
			/*g_cDeleteTmpData.InitDeleteTmpData();
			g_cDeleteTmpData.deleteUserActiveBattleInfo();
			g_cUpdateUserBaseInfo.updateUserActiveBattleInfo();
			g_cUpdateUserBaseInfo.InitUserGetFriendValueCount();
			g_cUpdateUserBaseInfo.InitUserGetFriendValueFlag();*/
		}
		sleep(3600);
	}
	pthread_exit((void*)0);
}

bool mainMysqlKeepAlive()
{
	//�߳�id
	pthread_t id;
	//�߳�����
	pthread_attr_t   attr;
	//��ʼ��
	pthread_attr_init(&attr);
	//�����߳�
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	//ɾ����ʱ�����߳�
	int nret=pthread_create(&id,&attr,Thread_mainMysqlKeepAlive,NULL);
	//�ɹ�
	if(nret!=0)
	{
		//ʧ��
		g_cMyGlog.errorlogRecord("pthread Thread_mainMysqlKeepAlive failure:errno:%d,err:%s\n",errno,strerror(errno));
		return false;
	}
	pthread_attr_destroy(&attr);
	return true;
}


int main(int argc,char* argv[])
{
	//���ݿ�����
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
	//����������
	if (!g_cMagicServerDbRead.DbReadInit())
	{
		g_cMyGlog.errorlogRecord("g_cMagicServerDb.DbReadInit() error");
		return -1;
	}
	//������Դ��ʼ����
	//g_cProcUserResourceDownLoad.InitLoadResourceData();
	//cerr<<"AAAAA"<<endl;
	//��ؿͻ�������ģ��
	//...
	/*if (!g_cListenClientConnect.Init())
	{
		cerr<<"g_cListenClientConnect.Init() failure"<<endl;
		return -1;
	}*/
	if(!mainMysqlKeepAlive())
	{
		return -1;
	}
	//test
	TestServer server(64);    
	server.AddSignalEvent(SIGINT, TestServer::QuitCb);    
	//timeval tv = {10, 0};    
	//server.AddTimerEvent(TestServer::TimeOutCb, tv, false);    
	server.SetPort(60000);    
	server.StartRun();    
	printf("done\n");
	
	//g_cProClientMessage.unInit();
	g_cMagicServerDb.unInit();
	g_cMagicServerDbRead.unInit();
	//g_cListenClientConnect.unInit();
	return 0;
}
