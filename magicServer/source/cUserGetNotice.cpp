#include "../include/cUserGetNotice.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/myGlog.h"
#include "../include/errorCode.h"

cUserGetNotice g_cUserGetNotice;


cUserGetNotice::cUserGetNotice()
{

}

cUserGetNotice::~cUserGetNotice()
{

}

int cUserGetNotice::setPBFiveSixStarCard(PBGameNotice& u_PBGameNotice,const int& u_noticeTimePoint)
{
	char buf[128] = {0};
	sprintf(buf,"select * from high_star_card_got_notice where createTime > %d;",u_noticeTimePoint);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	while (!q.eof())
	{
		PBFiveSixStarCard* n_PBFiveSixStarCard;
		n_PBFiveSixStarCard = u_PBGameNotice.add_fivesixstarcardnotice();
		n_PBFiveSixStarCard->set_username(q.getStringFileds("userName"));
		n_PBFiveSixStarCard->set_cardid(q.getStringFileds("cardId"));
		n_PBFiveSixStarCard->set_cardstar(q.getIntFileds("cardStar"));
		q.nextRow();
	}
	q.finalize();
	return 0;
}

int cUserGetNotice::setPBEvolutionSuccess(PBGameNotice& u_PBGameNotice,const int& u_noticeTimePoint)
{
	char buf[128] = {0};
	sprintf(buf,"select * from evolution_success_notice where createTime > %d;",u_noticeTimePoint);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	while (!q.eof())
	{
		PBEvolutionSuccess* n_PBEvolutionSuccess;
		n_PBEvolutionSuccess = u_PBGameNotice.add_evolutionsuccessnotice();
		n_PBEvolutionSuccess->set_username(q.getStringFileds("userName"));
		n_PBEvolutionSuccess->set_precardid(q.getStringFileds("preCardId"));
		n_PBEvolutionSuccess->set_cardid(q.getStringFileds("cardId"));
		q.nextRow();
	}
	q.finalize();
	return 0;
}

int cUserGetNotice::setPBPassLevelPlayerInfo(PBGameNotice& u_PBGameNotice,const int& u_noticeTimePoint)
{
	char buf[128] = {0};
	sprintf(buf,"select * from pass_level_notice where createTime > %d;",u_noticeTimePoint);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	while (!q.eof())
	{
		PBPassLevelPlayerInfo* n_PBPassLevelPlayerInfo;
		n_PBPassLevelPlayerInfo = u_PBGameNotice.add_passlevelnotice();
		n_PBPassLevelPlayerInfo->set_username(q.getStringFileds("userName"));
		n_PBPassLevelPlayerInfo->set_rank(q.getIntFileds("levelRank"));
		n_PBPassLevelPlayerInfo->set_levelname(q.getStringFileds("levelName"));
		q.nextRow();
	}
	q.finalize();
	return 0;
}

int cUserGetNotice::setPBCapacity(PBGameNotice& u_PBGameNotice,const int& u_noticeTimePoint)
{
	char buf[128] = {0};
	sprintf(buf,"select * from fight_capacity_breakthrough_notice where createTime > %d;",u_noticeTimePoint);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	while (!q.eof())
	{
		PBCapacity* n_PBCapacity;
		n_PBCapacity = u_PBGameNotice.add_capacitynotice();
		n_PBCapacity->set_username(q.getStringFileds("userName"));
		int n_fightCapacity = q.getIntFileds("fightCapacity");
		if (n_fightCapacity >= 100000 && n_fightCapacity < 200000)
		{
			n_PBCapacity->set_capacitytype(1);
		}
		else if (n_fightCapacity >= 200000 && n_fightCapacity < 300000)
		{
			n_PBCapacity->set_capacitytype(2);
		}
		else if (n_fightCapacity >= 300000)
		{
			n_PBCapacity->set_capacitytype(3);
		}
		n_PBCapacity->set_capacity(n_fightCapacity);
		q.nextRow();
	}
	q.finalize();
	return 0;
}

int cUserGetNotice::setGameActiveInfo(PBGameNotice& u_PBGameNotice)
{
	char buf[128] = {0};
	sprintf(buf,"select * from game_active_info_notice;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	while (!q.eof())
	{
		string* n_gameActiveInfo = u_PBGameNotice.add_gameactiveinfo();
		///////´ýÐÞ¸Ä
		string n_gameActiveInfoTmp = q.getStringFileds("activeInfo");
		n_gameActiveInfo->append(n_gameActiveInfoTmp.c_str(),n_gameActiveInfoTmp.size());
		q.nextRow();
	}
	q.finalize();
	return 0;
}

int cUserGetNotice::procSetPBGameNotice(PBGameNotice& u_PBGameNotice,const int& u_noticeTimePoint)
{
	setPBFiveSixStarCard(u_PBGameNotice,u_noticeTimePoint);
	setPBEvolutionSuccess(u_PBGameNotice,u_noticeTimePoint);
	setPBPassLevelPlayerInfo(u_PBGameNotice,u_noticeTimePoint);
	setPBCapacity(u_PBGameNotice,u_noticeTimePoint);
	setGameActiveInfo(u_PBGameNotice);
	return 0;
}

int cUserGetNotice::setPBGameNotice(PBGameNotice& u_PBGameNotice,const string& u_strToken)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBGameNotice getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	int u_noticeTimePoint = getNoticeTimePoint();
	u_result = procSetPBGameNotice(u_PBGameNotice,u_noticeTimePoint);
	return u_result;
}