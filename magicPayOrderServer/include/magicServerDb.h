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
	//�����ݿ�ʵ����
	bool DbReadInit();
	void unInit();

	//���ݿ����ӱ���
	void mysqlKeepAlive(int u_Type);
	//��ֹ����ͬ���ӳ���ɵĴӴӿ�ȡ�������ݵ����
	MysqlQuery master_slave_exQuery(const string& strSql);
	
	
protected:
private:
	//���ݿ����ӿ���
	bool m_bConnect;
};
extern cMagicServerDb g_cMagicServerDb;
extern cMagicServerDb g_cMagicServerDbRead;
#endif



