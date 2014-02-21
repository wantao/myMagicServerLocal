#include "../include/procMainServerRegister.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/myGlog.h"

cProcMainServerRegister g_cProcMainServerRegister;


cProcMainServerRegister::cProcMainServerRegister()
{
	
}
cProcMainServerRegister::~cProcMainServerRegister()
{

}

bool cProcMainServerRegister::isUserAccountExist(const PBMainServerRegisterSend& u_PBMainServerRegisterSend)
{
	char buf[128] = {0};
	sprintf(buf,"select userAccount from m_main_server_user where userAccount='%s' and plateform=%d;",\
		u_PBMainServerRegisterSend.useraccount().c_str(),u_PBMainServerRegisterSend.usersourcetype());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		g_cMyGlog.debugLog("isUserAccountExist,userAccount:%s,sourcType:%d exist\n",\
			u_PBMainServerRegisterSend.useraccount().c_str(),u_PBMainServerRegisterSend.usersourcetype());
		q.finalize();
		return true;
	}
	q.finalize();
	return false;
}

bool cProcMainServerRegister::insertUserAccount(const PBMainServerRegisterSend& u_PBMainServerRegisterSend)
{
	char buf[256] = {0};
	sprintf(buf,"insert into m_main_server_user(Id,userAccount,userPassword,plateform) values(NULL,'%s','%s',%d);",\
		u_PBMainServerRegisterSend.useraccount().c_str(),u_PBMainServerRegisterSend.userpasswd().c_str(),\
		u_PBMainServerRegisterSend.usersourcetype());
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("insertUserAccount error,strSql:%s\n",strSql.c_str());
		return false;
	}
	return true;
}

int cProcMainServerRegister::setPBMainServerRegisterReceive(PBMainServerRegisterReceive& u_PBMainServerRegisterReceive,\
								   const PBMainServerRegisterSend& u_PBMainServerRegisterSend)
{
	int u_result = 0;
	if (u_PBMainServerRegisterSend.useraccount().size() <= 0)
	{
		//账号密码不能为空
		u_PBMainServerRegisterReceive.set_registerresult(2);
	}
	else
	{
		//ios平台
		if (u_PBMainServerRegisterSend.usersourcetype() == P_IOS)
		{
			if (u_PBMainServerRegisterSend.userpasswd().size() <= 0)
			{
				//账号密码不能为空
				u_PBMainServerRegisterReceive.set_registerresult(2);
				return u_result;
			}
		}
		if (isUserAccountExist(u_PBMainServerRegisterSend))
		{
			//账号重复
			u_PBMainServerRegisterReceive.set_registerresult(1);
		}
		else
		{
			if (insertUserAccount(u_PBMainServerRegisterSend))
			{
				//成功
				u_PBMainServerRegisterReceive.set_registerresult(0);
			}
			else
			{
				//账号重复
				u_PBMainServerRegisterReceive.set_registerresult(1);
			}
		}
	}
	return u_result;
}