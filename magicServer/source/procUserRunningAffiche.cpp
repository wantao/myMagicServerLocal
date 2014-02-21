#include "../include/procUserRunningAffiche.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"

cProcUserRunningAffiche g_cProcUserRunningAffiche;

cProcUserRunningAffiche::cProcUserRunningAffiche()
{

}
cProcUserRunningAffiche::~cProcUserRunningAffiche()
{

}

bool cProcUserRunningAffiche::proxySetRunningAffiche(PBRunningAfficheReceive& u_PBRunningAfficheReceive)
{
	char buf[128] = {0};
	sprintf(buf,"select AfficheContents from m_RunningAfficheInfo order by AfficheTime desc limit %d;",RUNNING_AFFICHE_COUNTS);
	string strSql(buf);

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("proxySetRunningAffiche error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}

	while (!q.eof())
	{
		u_PBRunningAfficheReceive.add_affiches(q.getStringFileds("AfficheContents"));
		q.nextRow();
	}

	q.finalize();
	return true;
}

int cProcUserRunningAffiche::setPBRunningAfficheReceive(PBRunningAfficheReceive& u_PBRunningAfficheReceive,const string& u_strToken)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("getUserAllFriend getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	proxySetRunningAffiche(u_PBRunningAfficheReceive);
	return u_result;
}