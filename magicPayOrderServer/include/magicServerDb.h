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

	//数据库连接保活
	void mysqlKeepAlive(int u_Type);
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



