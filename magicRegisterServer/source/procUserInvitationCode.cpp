#include "../include/procUserInvitationCode.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/myGlog.h"

cProcUserInvitationCode g_cProcUserInvitationCode;

cProcUserInvitationCode::cProcUserInvitationCode()
{

}
cProcUserInvitationCode::~cProcUserInvitationCode()
{

}

int cProcUserInvitationCode::getInvitationCodeUsedCount(const string& u_invitationCode,int& u_usedCount)
{
	char buf[256] = {0};
	sprintf(buf,"select usedCount from m_invitation_code_table where invitationCode = '%s';",u_invitationCode.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		//error invitationCode;
		g_cMyGlog.errorlogRecord("getInvitationCodeUsedCount error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return 1;
	}
	u_usedCount = q.getIntFileds("usedCount");
	if (u_usedCount > 4)
	{
		//error invitationCode;
		q.finalize();
		return 1;
	}
	q.finalize();
	return 0;
}

void cProcUserInvitationCode::updateInvitationCodeCount(const string& u_invitationCode,const int& u_usedCount)
{
	char buf[256] = {0};
	sprintf(buf,"update m_invitation_code_table set usedCount = %d where invitationCode = '%s';",\
		u_usedCount,u_invitationCode.c_str());
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateInvitationCodeCount error:%s\n",strSql.c_str());
		return;
	}
}

void cProcUserInvitationCode::deleteInvitationCode(const string& u_invitationCode)
{
	char buf[256] = {0};
	sprintf(buf,"delete from m_invitation_code_table where invitationCode = '%s';",u_invitationCode.c_str());
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("deleteInvitationCode error:%s\n",strSql.c_str());
		return;
	}
}	

int cProcUserInvitationCode::setPBInvitationCodeReceive(PBInvitationCodeReceive& u_PBInvitationCodeReceive,const PBInvitationCodeSend& u_PBInvitationCodeSend)
{
	string u_invitationCode = u_PBInvitationCodeSend.invitationcode();
	int u_usedCount = 0;
	int u_result = 0;
	if (getInvitationCodeUsedCount(u_invitationCode,u_usedCount) == 0)
	{
		deleteInvitationCode(u_invitationCode);
		/*u_usedCount += 1;
		updateInvitationCodeCount(u_invitationCode,u_usedCount);*/
	}
	else
	{
		u_result = 1;
	}
	u_PBInvitationCodeReceive.set_result(u_result);
	return 0;
}