#include "../include/procUserCardForm.h"
#include "../include/procUserLogin.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"

cProcUserCardForm g_cProcUserCardForm;

cProcUserCardForm::cProcUserCardForm()
{

}

cProcUserCardForm::~cProcUserCardForm()
{

}

bool cProcUserCardForm::updateCardPosition(const unsigned int& u_userId,const unsigned int& u_itemId,\
						int u_battlePosition,bool isBattleStatus)
{
	char buf[128] = {0};
	if(isBattleStatus)
	{
		sprintf(buf,"update m_userCardInfo%d set battlePosition=%d where itemId = %d; \
					",u_userId%USER_CARD_TABLE_COUNT,u_battlePosition,u_itemId);
		string strSql(buf);
		if (g_cMagicServerDb.execSQL(strSql) != 0)
		{
			g_cMyGlog.errorlogRecord("updateCardPosition failure,strSql:%s\n",strSql.c_str());
			return false;
		}
	}
	else
	{
		sprintf(buf,"update m_userCardInfo%d set battlePosition=%d where userId =%d and \
					battlePosition = %d;",u_userId%USER_CARD_TABLE_COUNT,NOT_BATTLE_POSITION,u_userId,u_battlePosition);
		string strSql(buf);
		if (g_cMagicServerDb.execSQL(strSql) != 0)
		{
			g_cMyGlog.errorlogRecord("updateCardPosition failure,strSql:%s\n",strSql.c_str());
			return false;
		}
	}
	return true;
}

int cProcUserCardForm::getUserLeaderValue(const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select leaderValue from m_gameuserinfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserLeaderValue error:%s\n",strSql.c_str());
	}
	int u_retLeaderValue = q.getIntFileds("leaderValue");
	q.finalize();
	return u_retLeaderValue;
}

bool cProcUserCardForm::getCardStarLevel(const unsigned int& u_userId,const int& u_cardItemId,int& u_cardLeaderValue)
{
	char buf[256] = {0};
	sprintf(buf,"select cardStarLevel from m_usercardinfo%d where itemId = %d;",\
		u_userId%USER_CARD_TABLE_COUNT,u_cardItemId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getCardStarLevel error:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	int u_cardStarLevel = q.getIntFileds("cardStarLevel");
	u_cardLeaderValue = getCardLeaderValueByStarLevel(u_cardStarLevel);
	q.finalize();
	return true;
}

bool cProcUserCardForm::getFormCardLeaderValue(const unsigned int& u_userId,PBCardFormSend& u_PBCardFormSend,int& u_formCardLeaderValue)
{
	int u_tmpCardLeaderValue = 0;
	if (u_PBCardFormSend.has_captaincardid())
	{
		if (getCardStarLevel(u_userId,u_PBCardFormSend.captaincardid(),u_tmpCardLeaderValue))
		{
			u_formCardLeaderValue += u_tmpCardLeaderValue;
		}
		else
		{
			return false;
		}
	}
	if (u_PBCardFormSend.has_cardoneid())
	{
		if (getCardStarLevel(u_userId,u_PBCardFormSend.cardoneid(),u_tmpCardLeaderValue))
		{
			u_formCardLeaderValue += u_tmpCardLeaderValue;
		}
		else
		{
			return false;
		}
	}
	if (u_PBCardFormSend.has_cardtwoid())
	{
		if (getCardStarLevel(u_userId,u_PBCardFormSend.cardtwoid(),u_tmpCardLeaderValue))
		{
			u_formCardLeaderValue += u_tmpCardLeaderValue;
		}
		else
		{
			return false;
		}
	}
	if (u_PBCardFormSend.has_cardthreeid())
	{
		if (getCardStarLevel(u_userId,u_PBCardFormSend.cardthreeid(),u_tmpCardLeaderValue))
		{
			u_formCardLeaderValue += u_tmpCardLeaderValue;
		}
		else
		{
			return false;
		}
	}
	if (u_PBCardFormSend.has_cardfourid())
	{
		if (getCardStarLevel(u_userId,u_PBCardFormSend.cardfourid(),u_tmpCardLeaderValue))
		{
			u_formCardLeaderValue += u_tmpCardLeaderValue;
		}
		else
		{
			return false;
		}
	}
	return true;
}

int cProcUserCardForm::isLeaderValueEnough(const unsigned int& u_userId,PBCardFormSend& u_PBCardFormSend)
{
	int u_cardFormNeedLeaderValue = 0;
	int u_result = 0;
	if (!getFormCardLeaderValue(u_userId,u_PBCardFormSend,u_cardFormNeedLeaderValue))
	{
		u_result = -1;
	}
	int u_userLeaderValue = getUserLeaderValue(u_userId);
	if (u_cardFormNeedLeaderValue > u_userLeaderValue)
	{
		return K_LESS_LEADER_VALUE;
	}
	return u_result;
}

int cProcUserCardForm::setPBCardFormReceive(PBCardFormReceive& u_PBCardFormReceive,const string& u_strToken,PBCardFormSend& u_PBCardFormSend)
{
	unsigned int u_userId = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBCardFormReceive getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	int u_result = 0;
	u_result = isLeaderValueEnough(u_userId,u_PBCardFormSend);
	if (u_result == -1)
	{
		return u_result;
	}
	if (u_result == 0)
	{
		if (u_PBCardFormSend.has_captaincardid())
		{
			if (!(updateCardPosition(u_userId,u_PBCardFormSend.captaincardid(),CAPITOR_POSITION,false) && \
				updateCardPosition(u_userId,u_PBCardFormSend.captaincardid(),CAPITOR_POSITION,true)))
			{
				u_result = -1;
				return u_result;
			}
		}
		else
		{
			return K_CAPITOR_NOT_EXIST;
		}
		if (u_PBCardFormSend.has_cardoneid())
		{
			if (!(updateCardPosition(u_userId,u_PBCardFormSend.cardoneid(),1,false) && \
				updateCardPosition(u_userId,u_PBCardFormSend.cardoneid(),1,true)))
			{
				u_result = -1;
				return u_result;
			}
		}
		else
		{
			if(!updateCardPosition(u_userId,u_PBCardFormSend.cardoneid(),1,false))
			{
				u_result = -1;
				return u_result;
			}
		}
		if (u_PBCardFormSend.has_cardtwoid())
		{
			if (!(updateCardPosition(u_userId,u_PBCardFormSend.cardtwoid(),2,false) && \
				updateCardPosition(u_userId,u_PBCardFormSend.cardtwoid(),2,true)))
			{
				u_result = -1;
				return u_result;
			}
		}
		else
		{
			if(!updateCardPosition(u_userId,u_PBCardFormSend.captaincardid(),2,false))
			{
				u_result = -1;
				return u_result;
			}
		}
		if (u_PBCardFormSend.has_cardthreeid())
		{
			if (!(updateCardPosition(u_userId,u_PBCardFormSend.cardthreeid(),3,false) && \
				updateCardPosition(u_userId,u_PBCardFormSend.cardthreeid(),3,true)))
			{
				u_result = -1;
				return u_result;
			}
		}
		else
		{
			if(!updateCardPosition(u_userId,u_PBCardFormSend.captaincardid(),3,false))
			{
				u_result = -1;
				return u_result;
			}
		}
		if (u_PBCardFormSend.has_cardfourid())
		{
			if (!(updateCardPosition(u_userId,u_PBCardFormSend.cardfourid(),4,false) && \
				updateCardPosition(u_userId,u_PBCardFormSend.cardfourid(),4,true)))
			{
				u_result = -1;
				return u_result;
			}
		}
		else
		{
			if(!updateCardPosition(u_userId,u_PBCardFormSend.captaincardid(),4,false))
			{
				u_result = -1;
				return u_result;
			}
		}
	}
	
	if (u_result == K_LESS_LEADER_VALUE)
	{
		u_result = 0;
		u_PBCardFormReceive.set_formresult(2);
	}
	else
	{
		u_PBCardFormReceive.set_formresult(CARD_FORM_SUCCESS);
	}
	PBAllCard* u_PBAllCard;
	u_PBAllCard = u_PBCardFormReceive.mutable_cards();
	if (!g_cProcUserLogin.getAllCardsInfo(u_PBAllCard,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBCardFormReceive getAllCardsInfo\n");
		u_result = -1;
	}

	int u_userFightCapacity = 0;
	int u_userCapitorCardFightCapacity = 0;
	u_userFightCapacity = getUserFightCapacity(u_userId,u_userCapitorCardFightCapacity);
	updateUserFightCapacity(u_userId,u_userFightCapacity,u_userCapitorCardFightCapacity);

	u_PBCardFormReceive.set_fightcapacity(u_userFightCapacity);
	return u_result;
}