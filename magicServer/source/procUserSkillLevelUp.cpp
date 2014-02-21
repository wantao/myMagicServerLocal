#include "../include/procUserSkillLevelUp.h"
#include "../include/procUserCardLevelUp.h"
#include "../include/procUserLogin.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"

cProcUserSkillLevelUp g_cProcUserSkillLevelUp;

cProcUserSkillLevelUp::cProcUserSkillLevelUp()
{

}
cProcUserSkillLevelUp::~cProcUserSkillLevelUp()
{

}

bool cProcUserSkillLevelUp::getCardSkillLevelExp(const unsigned int& u_itemId,unsigned int& u_skillLevel,\
						  const unsigned int& u_userId,unsigned int& u_skillExp)
{
	char buf[128] = {0};
	sprintf(buf,"select skillExp,skillLevel from m_userCardInfo%d where itemId = %d;",u_userId%USER_CARD_TABLE_COUNT,u_itemId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getCardSkillLevelExp error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_skillExp = q.getIntFileds("skillExp");
	u_skillLevel = q.getIntFileds("skillLevel");
	q.finalize();
	return true;

}

bool cProcUserSkillLevelUp::updateCardSkillLevelExp(const unsigned int& u_itemId,unsigned int& u_skillLevel,\
						  const unsigned int& u_userId,unsigned int& u_skillExp)
{
	char buf[128] = {0};
	sprintf(buf,"update m_userCardInfo%d set skillExp=%d,skillLevel=%d where itemId = %d; \
				",u_userId%USER_CARD_TABLE_COUNT,u_skillExp,u_skillLevel,u_itemId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateCardSkillLevelExp failure,strSql:%s\n",strSql.c_str());
		return false;
	}
	return true;
}


//技能是否能升级
bool cProcUserSkillLevelUp::isSkillCanLevelUp(unsigned int& u_skillLevel,const unsigned int& u_skillExp)
{
	if (u_skillLevel == 1)
	{
		if (u_skillExp == ONE_LEVEL_MAX_SKILLEXP)
		{
			return true;
		}
	}
	else if (u_skillLevel == 2)
	{
		if (u_skillExp == TWO_LEVEL_MAX_SKILLEXP)
		{
			return true;
		}
	}
	else if (u_skillLevel == 3)
	{
		if (u_skillExp == THREE_LEVEL_MAX_SKILLEXP)
		{
			return true;
		}
	}
	else if (u_skillLevel == 4)
	{
		if (u_skillExp == FOURE_LEVEL_MAX_SKILLEXP)
		{
			return true;
		}
	}
	else if (u_skillLevel == 5)
	{
		if (u_skillExp == FIVE_LEVEL_MAX_SKILLEXP)
		{
			return true;
		}
	}
	else if (u_skillLevel == 6)
	{
		if (u_skillExp == SIX_LEVEL_MAX_SKILLEXP)
		{
			return true;
		}
	}
	else if (u_skillLevel == 7)
	{
		if (u_skillExp == SEVEN_LEVEL_MAX_SKILLEXP)
		{
			return true;
		}
	}
	else if (u_skillLevel == 8)
	{
		if (u_skillExp == EIGHT_LEVEL_MAX_SKILLEXP)
		{
			return true;
		}
	}
	else if (u_skillLevel == 9)
	{
		if (u_skillExp == NINE_LEVEL_MAX_SKILLEXP)
		{
			return true;
		}
	}
	return false;
}

int cProcUserSkillLevelUp::setPBSkillLevelUpReceive(PBSkillLevelUpReceive& u_PBSkillLevelUpReceive,const string& u_strToken,PBSkillLevelUpSend& u_PBSkillLevelUpSend)
{
	unsigned int u_userId = 0;
	unsigned int u_skillLevel = 0;
	unsigned int u_skillExp = 0;
	unsigned int u_itemId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBSkillLevelUpReceive getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	
	u_itemId = u_PBSkillLevelUpSend.upcarditemid();
	if(!getCardSkillLevelExp(u_itemId,u_skillLevel,u_userId,u_skillExp))
	{
		g_cMyGlog.errorlogRecord("setPBSkillLevelUpReceive getCardSkillLevelExp error\n");
		u_result = -1;
		return u_result;
	}
	//TEST begin
//#ifdef DEBUG_MOD
//	if (u_userId == 212)
//	{
//		u_skillLevel = 1;
//		u_skillExp = 9;
//	}
//	
//#endif
	//test end
	if (u_skillLevel == 10)
	{
		//u_PBSkillLevelUpReceive.set_skillupresult(SKILL_LEVEL_MAX);
		return K_SKILL_LEVEL_TO_MAX;
	}
	if (!isSkillCanLevelUp(u_skillLevel,u_skillExp))
	{
		//u_PBSkillLevelUpReceive.set_skillupresult(SKILL_NOT_ENOUGH);
		return K_SKILL_EXP_NOT_ENOUGH;
	}
	else
	{

		u_skillLevel += 1;
		u_skillExp = 0;
		//TEST begin
//#ifdef DEBUG_MOD
//		if (u_userId == 212)
//		{
//			u_skillLevel = 1;
//			u_skillExp = 9;
//		}
//#endif
		//test end
		if(updateCardSkillLevelExp(u_itemId,u_skillLevel,u_userId,u_skillExp))
		{
			u_PBSkillLevelUpReceive.set_skillupresult(SKILL_UP_SUCCESS);
			PBCard* u_PBCard;
			u_PBCard = u_PBSkillLevelUpReceive.mutable_targetcard();

			if (!g_cProcUserCardLevel.getCardInfo(u_PBCard,u_userId,u_itemId))
			{
				g_cMyGlog.errorlogRecord("setPBSkillLevelUpReceive getCardInfo error\n");
			}

			PBAllCard* u_PBAllCard;
			u_PBAllCard = u_PBSkillLevelUpReceive.mutable_cards();
			if (!g_cProcUserLogin.getAllCardsInfo(u_PBAllCard,u_userId))
			{
				g_cMyGlog.errorlogRecord("setPBSkillLevelUpReceive getAllCardsInfo\n");
			}
			
			int u_userFightCapacity = 0;
			int u_userCapitorCardFightCapacity = 0;
			u_userFightCapacity = getUserFightCapacity(u_userId,u_userCapitorCardFightCapacity);
			updateUserFightCapacity(u_userId,u_userFightCapacity,u_userCapitorCardFightCapacity);
			u_PBSkillLevelUpReceive.set_fightcapacity(u_userFightCapacity);

			//记录战斗力突破通知
			procFightCapacityBreakThrough(u_userId,u_userFightCapacity);
		}

	}
	return u_result;

}