#include "../include/procUserRegistCheck.h"
#include "../include/errorCode.h"
#include "../include/magicServerDb.h"
#include "../include/myGlog.h"

cProcUserRegistCheck g_cProcUserRegistCheck;

typedef enum registCheckEnum
{
	R_NOT_IN_USE = 0,//帐号完全可用
	R_ACCOUNT_IN_USE,//帐号重复
	R_USER_NAME_IN_USE	//昵称重复
}registCheckEnum;

cProcUserRegistCheck::cProcUserRegistCheck()
{

}

cProcUserRegistCheck::~cProcUserRegistCheck()
{

}

//获取注册类型
int cProcUserRegistCheck::getRegistType(const PBRegisterCheckSend& u_PBRegisterCheckSend)
{
	int u_registType = 0;
	//帐号注册
	if ((u_PBRegisterCheckSend.useraccount()).size() < 32 && (u_PBRegisterCheckSend.useraccount()).size() > 0)
	{
		u_registType = 1;
	}
	else if((u_PBRegisterCheckSend.macaddress()).size() < 32 && (u_PBRegisterCheckSend.macaddress()).size() > 0)
	{
		//mac地址注册
		u_registType = 2;
	}
	return u_registType;
}

//检查mac地址是否存在
int cProcUserRegistCheck::isMacAddressExist(const string& u_macAddress,const int& u_plateform)
{
	char buf[256] = {0};
	sprintf(buf,"select macAddress from m_gameUserInfo where macAddress = '%s' and plateform=%d;",u_macAddress.c_str(),u_plateform);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("isMacAddressExist,strSql:%s\n",strSql.c_str());
#endif

	if (q.eof())
	{
		q.finalize();
		return R_NOT_IN_USE;
	}
	q.finalize();
	return R_ACCOUNT_IN_USE;
}

//检查账号是否存在
int cProcUserRegistCheck::isUserAccountExist(const string& u_userAccount,const int& u_plateform)
{
	char buf[256] = {0};
	sprintf(buf,"select userAccount from m_gameUserInfo where userAccount = '%s' and plateform=%d;",u_userAccount.c_str(),u_plateform);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		q.finalize();
		return R_NOT_IN_USE;
	}
	q.finalize();
	return R_ACCOUNT_IN_USE;
}

//检查昵称是否存在
int cProcUserRegistCheck::isUserNameExist(const string& u_userName)
{
	char buf[256] = {0};
	sprintf(buf,"select userName from m_gameUserInfo where userName = '%s';",u_userName.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("isUserNameExist,strSql:%s\n",strSql.c_str());
#endif

	if (q.eof())
	{
		q.finalize();
		return R_NOT_IN_USE;
	}
	q.finalize();
	return R_USER_NAME_IN_USE;
}


int cProcUserRegistCheck::setPBRegisterCheckReceive(PBRegisterCheckReceive& u_PBRegisterCheckReceive,const PBRegisterCheckSend& u_PBRegisterCheckSend)
{
	int u_result = 0;
	int u_registType = 0;
	int nResult = 0;
	u_registType = getRegistType(u_PBRegisterCheckSend);
#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("setPBRegisterCheckReceive u_registType:%d\n",u_registType);
#endif
	//账号注册
	if (u_registType == 1)
	{
		nResult = isUserAccountExist(u_PBRegisterCheckSend.useraccount(),u_PBRegisterCheckSend.usersourcetype());
		if (nResult == R_NOT_IN_USE)
		{
			nResult = isUserNameExist(u_PBRegisterCheckSend.username());
			u_PBRegisterCheckReceive.set_result(nResult);
		}
		else
		{
			u_PBRegisterCheckReceive.set_result(nResult);
		}
	}
	else if (u_registType == 2)//mac地址注册
	{
		nResult = isMacAddressExist(u_PBRegisterCheckSend.macaddress(),u_PBRegisterCheckSend.usersourcetype());
		if (nResult == R_NOT_IN_USE)
		{
			nResult = isUserNameExist(u_PBRegisterCheckSend.username());
			u_PBRegisterCheckReceive.set_result(nResult);
		}
		else
		{
			u_PBRegisterCheckReceive.set_result(nResult);
		}
	}
	else
	{
		g_cMyGlog.errorlogRecord("setPBRegisterCheckReceive unkonw type:%d\n",u_registType);
		u_result = -1;
	}
	return u_result;
}