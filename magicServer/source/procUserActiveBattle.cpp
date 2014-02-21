#include "../include/procUserActiveBattle.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/procUserBattle.h"
#include "../include/procUserLogin.h"
#include "../include/myGlog.h"


cProcUserActiveBattle g_cProcUserActiveBattle;

cProcUserActiveBattle::cProcUserActiveBattle()
{

}

cProcUserActiveBattle::~cProcUserActiveBattle()
{

}

int cProcUserActiveBattle::getUserActiveBattle(PBBattle& u_PBBattle,const string& u_strToken)
{
	int u_result = 0;
	unsigned int u_userId = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("getUserActiveBattle getUserIdbyToken error:Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	string u_strBattleId = "";
	g_cProcUserBattle.getUserBattleProgress(u_userId,u_strBattleId);
	u_PBBattle.set_pveprogress(u_strBattleId);
	if (!g_cProcUserLogin.getActiveBattleInfo(&u_PBBattle,u_userId))
	{
		g_cMyGlog.errorlogRecord("procUserLogin getUserBattleInfo error\n");
	}
	return u_result;
}