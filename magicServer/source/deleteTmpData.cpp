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
		g_cMyGlog.errorlogRecord("delete m_equiptmpsaveinfo failure,strSql:%s\n",strSql.c_str());
	}
}

void cDeleteTmpData::deleteTmpCardInfo(const unsigned int& u_tableId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_cardtmpsaveinfo%d where isShouldDelete = 1",u_tableId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("delete m_cardtmpsaveinfo failure,strSql:%s\n",strSql.c_str());
	}
}

void cDeleteTmpData::deleteTmpData()
{
	for (unsigned int i=0; i<EQUIP_TMP_TABLE_COUNT; i++)
	{
		deleteTmpEquipInfo(i);
		//sleep(1);
	}
	for (unsigned int i=0; i<CARD_TMP_TABLE_COUNT; i++)
	{
		deleteTmpCardInfo(i);
		//sleep(1);
	}
	int n_noticeTimePoint = getNoticeTimePoint();
	deleteHighStarCardGotNotice(n_noticeTimePoint);
	deleteEvolutionSuccessNotice(n_noticeTimePoint);
	deleteFightCapacityBreakThroughNotice(n_noticeTimePoint);
	deletePassBigLevelNotice();
}


//删除高星级卡牌通知
void cDeleteTmpData::deleteHighStarCardGotNotice(const int& u_noticeTimePoint)
{
	char buf[128] = {0};
	sprintf(buf,"delete from high_star_card_got_notice where createTime < %d;",u_noticeTimePoint);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("deleteHighStarCardGotNotice error,strSql:%s\n",strSql.c_str());
	}
	return;
}

//删除进化成功通知
void cDeleteTmpData::deleteEvolutionSuccessNotice(const int& u_noticeTimePoint)
{
	char buf[128] = {0};
	sprintf(buf,"delete from evolution_success_notice where createTime < %d;",u_noticeTimePoint);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("deleteEvolutionSuccessNotice error,strSql:%s\n",strSql.c_str());
	}
	return;
}

//删除战斗力突破通知
void cDeleteTmpData::deleteFightCapacityBreakThroughNotice(const int& u_noticeTimePoint)
{
	char buf[128] = {0};
	sprintf(buf,"delete from fight_capacity_breakthrough_notice where createTime < %d;",u_noticeTimePoint);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("deleteFightCapacityBreakThroughNotice error,strSql:%s\n",strSql.c_str());
	}
	return;
}


//获取通过该大关的最大排名
int cDeleteTmpData::getMaxLevelRankByLevelCode(const string& u_levelCode)
{
	char buf[128] = {0};
	sprintf(buf,"select max(levelRank) from pass_level_notice where levelCode = '%s';",u_levelCode.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	int n_maxLevelRank = 0;
	if (q.eof())
	{
		q.finalize();
		return n_maxLevelRank;
	}
	n_maxLevelRank = q.getIntFileds("max(levelRank)");
	q.finalize();
	return n_maxLevelRank;
}

//根据大关名字和通过该大关的最大排名，删除通关通知
void cDeleteTmpData::deletePassBigLevelNotice(const string& u_levelCode,const int& u_maxRank)
{
	char buf[256] = {0};
	sprintf(buf,"delete from pass_level_notice where levelCode = '%s' and levelRank < %d;",\
		u_levelCode.c_str(),u_maxRank);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("deletePassBigLevelNotice error,strSql:%s\n",strSql.c_str());
	}
	
}

//删除通关通知记录信息
void cDeleteTmpData::deletePassBigLevelNotice(/*const int& u_noticeTimePoint*/)
{
	char buf[256] = {0};
	sprintf(buf,"select * from big_chapter_table;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	while (!q.eof())
	{
		int n_maxLevelRank = getMaxLevelRankByLevelCode(q.getStringFileds("levelCode"));
		if (n_maxLevelRank > 1)
		{
			deletePassBigLevelNotice(q.getStringFileds("levelCode"),n_maxLevelRank);
		}
		q.nextRow();
	}
	q.finalize();
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