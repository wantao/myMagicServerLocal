#include "../include/procMainServerLogin.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/myGlog.h"

cProcMainServerLogin g_cProcMainServerLogin;

cProcMainServerLogin::cProcMainServerLogin()
{

}
cProcMainServerLogin::~cProcMainServerLogin()
{

}

bool cProcMainServerLogin::isUserAccountRight(const PBMainServerLoginSend& u_PBMainServerLoginSend)
{
	char buf[128] = {0};
	sprintf(buf,"select userAccount from m_main_server_user where userAccount='%s' and plateform=%d;",\
		u_PBMainServerLoginSend.useraccount().c_str(),u_PBMainServerLoginSend.usersourcetype());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		q.finalize();
		return true;
	}
	g_cMyGlog.debugLog("isUserAccountRight not find userAccount:%s,sourcType:%d\n",\
		u_PBMainServerLoginSend.useraccount().c_str(),u_PBMainServerLoginSend.usersourcetype());
	q.finalize();
	return false;
}

bool cProcMainServerLogin::isUserPasswordRight(const PBMainServerLoginSend& u_PBMainServerLoginSend)
{
	char buf[128] = {0};
	sprintf(buf,"select userAccount from m_main_server_user where userAccount='%s' and plateform=%d and userPassword='%s';",\
		u_PBMainServerLoginSend.useraccount().c_str(),u_PBMainServerLoginSend.usersourcetype(),u_PBMainServerLoginSend.userpasswd().c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		q.finalize();
		return true;
	}
	g_cMyGlog.debugLog("isUserPasswordRight,not find userAccount:%s,password:%s,sourcType:%d\n",\
		u_PBMainServerLoginSend.useraccount().c_str(),u_PBMainServerLoginSend.userpasswd().c_str(),u_PBMainServerLoginSend.usersourcetype());
	q.finalize();
	return false;
}

int cProcMainServerLogin::setPBMainServerLoginReceive(PBMainServerLoginReceive& u_PBMainServerLoginReceive,\
								const PBMainServerLoginSend& u_PBMainServerLoginSend)
{
	int u_result = 0;
	if (u_PBMainServerLoginSend.useraccount().size() <= 0)
	{
		//账号名不存在
		u_PBMainServerLoginReceive.set_loginresult(1);
	}
	else
	{
		//ios平台
		if (u_PBMainServerLoginSend.usersourcetype() == P_IOS)
		{
			if (u_PBMainServerLoginSend.userpasswd().size() <= 0)
			{
				//密码错误
				u_PBMainServerLoginReceive.set_loginresult(2);
				return u_result;
			}
			if (!isUserAccountRight(u_PBMainServerLoginSend))
			{
				//账号名不存在
				u_PBMainServerLoginReceive.set_loginresult(1);
				return u_result;
			}
			if (!isUserPasswordRight(u_PBMainServerLoginSend))
			{
				//密码错误
				u_PBMainServerLoginReceive.set_loginresult(2);
				return u_result;
			}
		}
		//91平台
		else if (u_PBMainServerLoginSend.usersourcetype() == P_91)
		{
			if (!isUserAccountRight(u_PBMainServerLoginSend))
			{
				//账号名不存在
				u_PBMainServerLoginReceive.set_loginresult(1);
				return u_result;
			}
		}

	}
	return u_result;
}