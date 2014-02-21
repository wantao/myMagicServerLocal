#ifndef PROCUSERRANKREQ_H
#define PROCUSERRANKREQ_H

#include <iostream>
#include "../include/MagicCard.pb.h"
#include "MysqlQuery.h"
#include "MysqlDB.h"
using namespace std;

class cProcUserRankReq
{
public:
	cProcUserRankReq();
	~cProcUserRankReq();
	string getUserNameById(const unsigned int& u_userId);
	string getPlayerRankSelectSql(const unsigned int& u_userId,const int& u_rankWays);
	int getPlayerRank(const unsigned int& u_userId,const int& u_rankWays);
	string getLastPlayerRankSelectSql(const unsigned int& u_userId,const int& u_rankWays);
	int getLastPlayerRank(const unsigned int& u_userId,const int& u_rankWays);
	int getSelectBaseCount(const unsigned int& u_userId,const int& u_rankWays,\
		const int& u_reqWays,const int& u_count,int& u_rankStatus);
	string getSelectSql(const unsigned int& u_userId,const int& u_rankWays,\
		const int& u_reqWays,const int& u_count,int& u_rankStatus);
	void fillPBRankRecv(PBRankRecv& u_PBRankRecv,MysqlQuery& q);
	int proxysetPBRankRecv(PBRankRecv& u_PBRankRecv,const unsigned int& u_userId,const PBRankSend& u_PBRankSend);
	int setPBRankRecv(PBRankRecv& u_PBRankRecv,const string& u_strToken,const PBRankSend& u_PBRankSend);
protected:
private:
};

extern cProcUserRankReq g_cProcUserRankReq;

#endif