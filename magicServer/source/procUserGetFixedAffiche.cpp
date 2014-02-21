#include "../include/procUserGetFixedAffiche.h"
#include "../include/magicServerDb.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"

cProcUserGetFixedAffiche g_cProcUserGetFixedAffiche;

cProcUserGetFixedAffiche::cProcUserGetFixedAffiche()
{

}
cProcUserGetFixedAffiche::~cProcUserGetFixedAffiche()
{

}

int cProcUserGetFixedAffiche::setPBSystemInfoReceive(PBSystemInfoReceive& u_PBSystemInfoReceive,const string& u_strToken)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("getUserAllFriend getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	g_cMagicServerDb.setPBSystemInfoReceive(u_PBSystemInfoReceive);
	return u_result;
}