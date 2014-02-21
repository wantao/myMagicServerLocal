#include "../include/mememCacheClient.h"
#include "../include/myGlog.h"
#include "../include/structDef.h"
#include <sys/time.h>

mememCacheClient g_mememCacheClient;

mememCacheClient::mememCacheClient()
{
}

mememCacheClient::~mememCacheClient()
{
	if (memc != NULL)
	{
		memcached_free(memc);
	}
}

bool mememCacheClient::mememCacheClientInit(const string& u_memCacheServer,const int& u_memCacheServerPort)
{
	memcached_return rc;                                                                   
	memcached_server_st *server = NULL;

	memc = memcached_create(NULL);

	server =memcached_server_list_append(server, u_memCacheServer.c_str(), u_memCacheServerPort, &rc);

	rc=memcached_server_push(memc,server);                                         


	if (MEMCACHED_SUCCESS != rc)  
	{
		g_cMyGlog.errorlogRecord("Couldn't add server: %s\n",memcached_strerror(memc, rc));
		memc = NULL;
		return false;
	}

	memcached_server_list_free(server);
	return true;
}

void mememCacheClient::UnInit()
{
	if (memc != NULL)
	{
		memcached_free(memc);
	}
}

string mememCacheClient::formatBattleCardLoseInfoValue(MysqlQuery& q)
{
	string u_retBattleLoseInfoValue = "";
	char buf[512] = {0};
	//value格式: cardsId cardProbability cardLevel equipTypeId equipId equipProbability goldProbability goldNumber
	sprintf(buf,"%s %d %d",(q.getStringFileds("cardId")).c_str(),q.getIntFileds("cardProbability"),\
		q.getIntFileds("cardLevel"));
	u_retBattleLoseInfoValue += buf;
	return u_retBattleLoseInfoValue;
}

string mememCacheClient::formatBattleEquipLoseInfoValue(const string& u_monsterId)
{
	char buf[256] = {0};
	string u_retBattleEquipLoseInfoValue = "";
	sprintf(buf,"select * from battleequipinfo where monsterId='%s';",u_monsterId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("formatBattleEquipLoseInfoValue error:strSql:%s\n",strSql.c_str());
		q.finalize();
		return u_retBattleEquipLoseInfoValue;
	}
	memset(buf,0,sizeof(buf));
	sprintf(buf," %d %s %d",q.getIntFileds("equipTypeId"),(q.getStringFileds("equipId")).c_str(),\
		q.getIntFileds("equipProbability"));
	u_retBattleEquipLoseInfoValue += buf;
	q.finalize();
	return u_retBattleEquipLoseInfoValue;
}

string mememCacheClient::formatBattleGoldLoseInfoValue(const string& u_monsterId)
{
	char buf[256] = {0};
	string u_retBattleGoldLoseInfoValue = "";
	sprintf(buf,"select * from battlegoldinfo where monsterId='%s';",u_monsterId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("formatBattleGoldLoseInfoValue error:strSql:%s\n",strSql.c_str());
		q.finalize();
		return u_retBattleGoldLoseInfoValue;
	}
	memset(buf,0,sizeof(buf));
	sprintf(buf," %d %d",q.getIntFileds("goldProbability"),q.getIntFileds("goldNumber"));
	u_retBattleGoldLoseInfoValue += buf;
	q.finalize();
	return u_retBattleGoldLoseInfoValue;
}

string mememCacheClient::formatBattleBigCardIdLoseInfo(const string& u_monsterId)
{
	string u_retBattleBigCardIdLoseInfoValue = "";
	char buf[256] = {0};
	sprintf(buf,"select * from battle_big_card_id_info where monsterId='%s';",u_monsterId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("formatBattleGoldLoseInfoValue error:strSql:%s\n",strSql.c_str());
		q.finalize();
		return u_retBattleBigCardIdLoseInfoValue;
	}
	memset(buf,0,sizeof(buf));
	//value格式: cardsId cardProbability cardLevel equipTypeId equipId equipProbability goldProbability goldNumber
	sprintf(buf," %d %d",q.getIntFileds("cardId"),\
		q.getIntFileds("cardProbability"));
	u_retBattleBigCardIdLoseInfoValue += buf;
	q.finalize();
	return u_retBattleBigCardIdLoseInfoValue;
}

bool mememCacheClient::LoadBattleLoseInfoToMememCache()
{
	char buf[128] = {0};
	sprintf(buf,"select * from battlecardinfo;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("LoadBattleLoseInfoToMememCache error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	while(!q.eof())
	{
		string u_battleLoseInfoValue = "";
		string u_battleCardLoseInfoValue = formatBattleCardLoseInfoValue(q);
		if (u_battleCardLoseInfoValue.size() == 0)
		{
			g_cMyGlog.errorlogRecord("u_battleCardLoseInfoValue.size() == 0\n");
			q.nextRow();
			continue;
		}

		string u_battleEquipLoseInfo = formatBattleEquipLoseInfoValue(q.getStringFileds("monsterId"));
		if (u_battleEquipLoseInfo.size() == 0)
		{
			g_cMyGlog.errorlogRecord("u_battleEquipLoseInfo.size() == 0\n");
			q.nextRow();
			continue;
		}

		string u_battleGoldLoseInfo = formatBattleGoldLoseInfoValue(q.getStringFileds("monsterId"));
		if (u_battleGoldLoseInfo.size() == 0)
		{
			g_cMyGlog.errorlogRecord("u_battleGoldLoseInfo.size() == 0\n");
			q.nextRow();
			continue;
		}

		string u_battleBigCardIdLoseInfo = formatBattleBigCardIdLoseInfo(q.getStringFileds("monsterId"));
		if (u_battleBigCardIdLoseInfo.size() == 0)
		{
			g_cMyGlog.errorlogRecord("u_battleBigCardIdLoseInfo.size() == 0\n");
			q.nextRow();
			continue;
		}

		u_battleLoseInfoValue += u_battleCardLoseInfoValue;
		u_battleLoseInfoValue += u_battleEquipLoseInfo;
		u_battleLoseInfoValue += u_battleGoldLoseInfo;
		u_battleLoseInfoValue += u_battleBigCardIdLoseInfo;
		if (Insert((q.getStringFileds("monsterId")).c_str(),u_battleLoseInfoValue.c_str(),0) != 1)
		{
			q.finalize();
			return false;
		}
		q.nextRow();
	}
	q.finalize();
	return true;
}

bool mememCacheClient::getMemCacheInfoByIp(const string& u_serverIp,string& u_memCacheIp,int& u_memCachePort,int& u_updateStatus)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_memcache_table where updateMemcacheServer='%s';",u_serverIp.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getMemCacheInfoByIp error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_memCacheIp = q.getStringFileds("memcacheServer");
	u_memCachePort = q.getIntFileds("memcacheSeverPort");
	u_updateStatus = q.getIntFileds("updateStatus");
	q.finalize();
	return true;
}

bool mememCacheClient::Init()
{
	string u_memCacheServer = "";
	int u_memCacheServerPort = 0;
	string u_ipAddress = "";
	int u_updateStatus = 0;
	u_ipAddress = GetIpAddress("eth1",1);
	if (u_ipAddress.size() == 0)
	{
		g_cMyGlog.errorlogRecord("GetIpAddress(eth1,1) error\n");
		return false;
	}
	if (!getMemCacheInfoByIp(u_ipAddress,u_memCacheServer,u_memCacheServerPort,u_updateStatus))
	{
		g_cMyGlog.errorlogRecord("getMemCacheInfoByIp error,u_ipAddress:%s\n",u_ipAddress.c_str());
		return false;
	}
	if (!mememCacheClientInit(u_memCacheServer,u_memCacheServerPort))
	{
		g_cMyGlog.errorlogRecord("mememCacheClientInit failure\n");
		return false;
	}
	if (u_updateStatus == 1)
	{
		struct timeval tvBegin,tvEnd;
		gettimeofday(&tvBegin,NULL);
		g_cMyGlog.errorlogRecord("tvBeginSecond:%d,tvBeginUsec:%d\n",tvBegin.tv_sec,tvBegin.tv_usec);
//#ifndef DEBUG_MOD
		if (!LoadBattleLoseInfoToMememCache())
		{
			g_cMyGlog.errorlogRecord("LoadBattleLoseInfoToMememCache failure\n");
			return false;
		}
//#endif
		
		gettimeofday(&tvEnd,NULL);
		g_cMyGlog.errorlogRecord("tvEndSecond:%d,tvEndUsec:%d\n",tvEnd.tv_sec,tvEnd.tv_usec);
		float u_useTime = ((tvEnd.tv_sec*1000*1000+tvEnd.tv_usec) - (tvBegin.tv_sec*1000*1000+tvBegin.tv_usec))/1000000;
		g_cMyGlog.errorlogRecord("LoadToMememcache Use Time:%f\n",u_useTime);
	}
	return true;
}

int mememCacheClient::Insert(const char* key, const char* value,const time_t& expiration)
{
	if (NULL == key || NULL == value || memc == NULL)
	{  
		return -1;
	}

	uint32_t flags = 0;

	memcached_return rc;

	rc = memcached_set(memc, key, strlen(key),value,strlen(value),expiration, flags);

	// insert ok
	if (MEMCACHED_SUCCESS == rc)
	{
		return 1;
	}
	else
	{
		g_cMyGlog.errorlogRecord("Couldn't store key:%s, error:%s\n",key,memcached_strerror(memc, rc));
		return 0;
	}
}

string mememCacheClient::Get(const char* key)
{
	if (NULL == key && memc == NULL)
	{  
		return "";
	}
	string u_retValue = "";
	uint32_t flags = 0;

	memcached_return rc;

	size_t value_length;
	char* value = memcached_get(memc, key, strlen(key), &value_length, &flags, &rc);

	// get ok
	if(rc == MEMCACHED_SUCCESS)
	{  
		if (value != NULL)
		{
			u_retValue = value;
			free(value);
		}
	}

	return u_retValue;
}

void mememCacheClient::printValueByKey(const char* key)
{
	while (1)
	{
		//cerr<<"key:"<<key<<" value:"<<Get(key)<<endl;
		usleep(1000*300);
	}
	
}