#ifndef MAGICSERVERDB_H
#define MAGICSERVERDB_H
#include <iostream>
#include "common.h"
#include "MysqlQuery.h"
#include "MysqlDB.h"
#include "../include/MagicCard.pb.h"
using namespace std;

class cMagicServerDb:public MysqlDB
{
public:
	cMagicServerDb();
	~cMagicServerDb();
	bool Init();
	//从数据库实例化
	bool DbReadInit();
	void unInit();

	////////////得到PBSystemInfoReceive所需的东西/////begin
	//填充PBSystemInfoReceive
	void setPBSystemInfoReceive(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//获取公告信息
	void PBSystemInfoReceive_setAfficheInfo(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//获取服务器所有分区信息
	void PBSystemInfoReceive_setServerInfo(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//获取游戏版本信息
	void PBSystemInfoReceive_setGameVersion(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//获取资源版本信息
	void PBSystemInfoReceive_setResourceVersion(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//获取副本版本信息
	void PBSystemInfoReceive_setBattleVersion(PBSystemInfoReceive& u_PBSystemInfoReceive);
	////////////得到PBSystemInfoReceive所需的东西/////end

	//数据库连接保活
	void mysqlKeepAlive(int u_Type);
	string getNewResourceVersionPath();
	bool getResorceVersionPath(const string& u_resorceVersionName,string& u_resourceVersionPath);
	//防止主从同步延迟造成的从从库取不到数据的情况
	MysqlQuery master_slave_exQuery(const string& strSql);
protected:
private:
	//数据库连接开关
	bool m_bConnect;
};
extern cMagicServerDb g_cMagicServerDb;
extern cMagicServerDb g_cMagicServerDbRead;
#endif



