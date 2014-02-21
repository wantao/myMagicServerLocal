#ifndef MEMEMCACHECLIENT_H
#define MEMEMCACHECLIENT_H

#include <libmemcached/memcached.h>
#include <iostream>
#include <string>
#include <time.h>
#include "../include/structDef.h"
#include "../include/magicServerDb.h"

using namespace std;

class mememCacheClient
{
public:
	mememCacheClient();
	~mememCacheClient();
	bool mememCacheClientInit(const string& u_memCacheServer,const int& u_memCacheServerPort);
	void UnInit();
	string formatBattleCardLoseInfoValue(MysqlQuery& q);
	string formatBattleEquipLoseInfoValue(const string& u_monsterId);
	string formatBattleGoldLoseInfoValue(const string& u_monsterId);
	string formatBattleBigCardIdLoseInfo(const string& u_monsterId);
	bool LoadBattleLoseInfoToMememCache();
	bool Init();
	int Insert(const char* key, const char* value,const time_t& expiration);
	string Get(const char* key);
	bool getMemCacheInfoByIp(const string& u_serverIp,string& u_memCacheIp,int& u_memCachePort,int& u_updateStatus);

	void printValueByKey(const char* key);
private:
	memcached_st* memc;                                                                 
};


extern mememCacheClient g_mememCacheClient;

#endif