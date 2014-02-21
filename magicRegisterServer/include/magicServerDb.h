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

	////////////�õ�PBSystemInfoReceive����Ķ���/////begin
	//���PBSystemInfoReceive
	void setPBSystemInfoReceive(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//��ȡ������Ϣ
	void PBSystemInfoReceive_setAfficheInfo(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//��ȡ���������з�����Ϣ
	void PBSystemInfoReceive_setServerInfo(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//��ȡ��Ϸ�汾��Ϣ
	void PBSystemInfoReceive_setGameVersion(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//��ȡ��Դ�汾��Ϣ
	void PBSystemInfoReceive_setResourceVersion(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//��ȡ�����汾��Ϣ
	void PBSystemInfoReceive_setBattleVersion(PBSystemInfoReceive& u_PBSystemInfoReceive);
	////////////�õ�PBSystemInfoReceive����Ķ���/////end

	//���ݿ����ӱ���
	void mysqlKeepAlive(int u_Type);
	string getNewResourceVersionPath();
	bool getResorceVersionPath(const string& u_resorceVersionName,string& u_resourceVersionPath);
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



