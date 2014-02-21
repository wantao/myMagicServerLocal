#include "../include/deleteTmpData.h"
#include "../include/define.h"
#include "../include/magicServerDb.h"
#include "../include/myGlog.h"
#include <errno.h>

cDeleteTmpData g_cDeleteTmpData;

cDeleteTmpData::cDeleteTmpData()
{

}
cDeleteTmpData::~cDeleteTmpData()
{

}


void cDeleteTmpData::deleteTmpEquipInfo(const unsigned int& u_tableId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_equiptmpsaveinfo%d where isShouldDelete = 1",u_tableId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("delete m_equiptmpsaveinfo failure strSql:%s\n",strSql.c_str());
	}
}

void cDeleteTmpData::deleteTmpCardInfo(const unsigned int& u_tableId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_cardtmpsaveinfo%d where isShouldDelete = 1",u_tableId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("delete m_cardtmpsaveinfo failure strSql:%s\n",strSql.c_str());
	}
}

void cDeleteTmpData::deleteTmpData()
{
	for (unsigned int i=0; i<EQUIP_TMP_TABLE_COUNT; i++)
	{
		deleteTmpEquipInfo(i);
		sleep(1);
	}
	for (unsigned int i=0; i<CARD_TMP_TABLE_COUNT; i++)
	{
		deleteTmpCardInfo(i);
		sleep(1);
	}
}

void cDeleteTmpData::deleteUserActiveBattleInfoById(const int& u_tableId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_useractivebattleinfo%d;",u_tableId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("delete deleteUserActiveBattleInfoById failure strSql:%s\n",strSql.c_str());
	}
}

void cDeleteTmpData::deleteUserActiveBattleInfo()
{
	for (int i = 0; i < ACTIVE_BATTLE_COUNT; i++)
	{
		deleteUserActiveBattleInfoById(i);
	}
}

void* ThreadDeleteTmpData(void* pArg)
{
	g_cDeleteTmpData.deleteTmpData();
	pthread_exit((void*)0);
}

bool cDeleteTmpData::InitDeleteTmpData()
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
	int nret=pthread_create(&id,&attr,ThreadDeleteTmpData,NULL);
	//成功
	if(nret!=0)
	{
		//失败
		g_cMyGlog.errorlogRecord("pthread ThreadDeleteTmpData failure:errno:%d,err:%s\n",errno,strerror(errno));
		return false;
	}
	pthread_attr_destroy(&attr);
	return true;
}