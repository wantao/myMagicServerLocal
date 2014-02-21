#include "../include/procUserCardEvolution.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/procUserCardLevelUp.h"
#include "../include/procUserAwardFriend.h"
#include "../include/procUserLogin.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"


cProcUserCardEvolution g_cProcUserCardEvolution;

cProcUserCardEvolution::cProcUserCardEvolution()
{
}

cProcUserCardEvolution::~cProcUserCardEvolution()
{

}

//获取配方中某种配料的数量
int cProcUserCardEvolution::getComposeCount(const string& u_cardId,const unsigned int& u_userId)
{
	char buf[128] = {0};
	int u_cardCount = 0;
	if (atoi(u_cardId.c_str()) >= 1000)
	{
		sprintf(buf,"select count from m_userequipinfo%d where userId=%d and equipMentId='%s';",\
			u_userId%USER_EQUIP_TABLE_COUNT,u_userId,u_cardId.c_str());
		string strSql(buf);
		MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
		if (q.eof())
		{
			g_cMyGlog.errorlogRecord("getComposeCount error ,strSql::%s\n",strSql.c_str());
			q.finalize();
			return u_cardCount;
		}
		u_cardCount = q.getIntFileds("count");
	}
	else
	{
		sprintf(buf,"select count(cardId) from m_userCardInfo%d where userID=%d and cardId = '%s'",\
			u_userId%USER_CARD_TABLE_COUNT,u_userId,u_cardId.c_str());
		string strSql(buf);
		MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
		if (q.eof())
		{
			g_cMyGlog.errorlogRecord("121getComposeCount error ,strSql:%s\n",strSql.c_str());
			q.finalize();
			return u_cardCount;
		}
		u_cardCount = q.getIntFileds("count(cardId)");
		q.finalize();
	}
	return u_cardCount;
}

//填充合成配方表
void cProcUserCardEvolution::setComposeTable(ComposeTable& u_ComposeTable,const string& u_ComposeId,const int& u_Num,int u_Type)
{
	if (u_Type == 1)
	{
		u_ComposeTable.SynthesizeId1 = u_ComposeId;
		u_ComposeTable.SynthesizeNum1 = u_Num;
	}
	else if (u_Type == 2)
	{
		u_ComposeTable.SynthesizeId2 = u_ComposeId;
		u_ComposeTable.SynthesizeNum2 = u_Num;
	}
	else if (u_Type == 3)
	{
		u_ComposeTable.SynthesizeId3 = u_ComposeId;
		u_ComposeTable.SynthesizeNum3 = u_Num;
	}
	else if (u_Type == 4)
	{
		u_ComposeTable.SynthesizeId4 = u_ComposeId;
		u_ComposeTable.SynthesizeNum4 = u_Num;
	}
	else if (u_Type == 5)
	{
		u_ComposeTable.SynthesizeId5 = u_ComposeId;
		u_ComposeTable.SynthesizeNum5 = u_Num;
	}
}
//得到合成卡牌的配方表
bool cProcUserCardEvolution::getEvolutionCompose(string& u_EvolutionId,const unsigned int& u_userId,ComposeTable& u_ComposeTable)
{
	char buf[512] = {0};
	string u_composeId = "";
	int u_composeCount = 0;
	sprintf(buf,"select Money,SynthesizeId1,SynthesizeNum1,\
				SynthesizeId2,SynthesizeNum2,\
				SynthesizeId3,SynthesizeNum3,\
				SynthesizeId4,SynthesizeNum4,\
				SynthesizeId5,SynthesizeNum5 from ResInfo where Id = '%s';",u_EvolutionId.c_str());
	string strSql(buf);

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getEvolutionCompose get nothing,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	if (atoi((q.getStringFileds("SynthesizeId1")).c_str()) != 0)
	{
		u_composeCount = atoi((q.getStringFileds("SynthesizeNum1")).c_str());
		int u_ownCount = getComposeCount(q.getStringFileds("SynthesizeId1"),u_userId);
		if (u_composeCount > 0)
		{
			if (u_composeCount > u_ownCount)
			{
				g_cMyGlog.errorlogRecord("aaaa %s less ComposeThing: %s need:%d,ownNum:%d\n",\
					u_EvolutionId.c_str(),(q.getStringFileds("SynthesizeId1")).c_str(),u_composeCount,u_ownCount);
				q.finalize();
				return false;
			}
			setComposeTable(u_ComposeTable,q.getStringFileds("SynthesizeId1"),u_composeCount,1);
		}
	}
	if (atoi((q.getStringFileds("SynthesizeId2")).c_str()) != 0)
	{
		u_composeCount = atoi((q.getStringFileds("SynthesizeNum2")).c_str());
		int u_ownCount = getComposeCount(q.getStringFileds("SynthesizeId2"),u_userId);
		if (u_composeCount > 0)
		{
			if (u_composeCount > u_ownCount)
			{
				g_cMyGlog.errorlogRecord("bbb %s less ComposeThing: %s need:%d,ownNum:%d\n",\
					u_EvolutionId.c_str(),(q.getStringFileds("SynthesizeId2")).c_str(),u_composeCount,u_ownCount);
				q.finalize();
				return false;
			}
			setComposeTable(u_ComposeTable,q.getStringFileds("SynthesizeId2"),u_composeCount,2);
		}
	}
	if (atoi((q.getStringFileds("SynthesizeId3")).c_str()) != 0)
	{
		u_composeCount = atoi((q.getStringFileds("SynthesizeNum3")).c_str());
		int u_ownCount = getComposeCount(q.getStringFileds("SynthesizeId3"),u_userId);
		if (u_composeCount > 0)
		{
			if (u_composeCount > u_ownCount)
			{
				g_cMyGlog.errorlogRecord("ccc %s less ComposeThing: %s need:%d,ownNum:%d\n",\
					u_EvolutionId.c_str(),(q.getStringFileds("SynthesizeId3")).c_str(),u_composeCount,u_ownCount);
				q.finalize();
				return false;
			}
			setComposeTable(u_ComposeTable,q.getStringFileds("SynthesizeId3"),u_composeCount,3);
		}
	}
	if (atoi((q.getStringFileds("SynthesizeId4")).c_str()) != 0)
	{
		u_composeCount = atoi((q.getStringFileds("SynthesizeNum4")).c_str());
		int u_ownCount = getComposeCount(q.getStringFileds("SynthesizeId4"),u_userId);
		if (u_composeCount > 0)
		{
			if (u_composeCount > u_ownCount)
			{
				g_cMyGlog.errorlogRecord("dddd %s less ComposeThing: %s need:%d,ownNum:%d\n",\
					u_EvolutionId.c_str(),(q.getStringFileds("SynthesizeId4")).c_str(),u_composeCount,u_ownCount);
				q.finalize();
				return false;
			}
			setComposeTable(u_ComposeTable,q.getStringFileds("SynthesizeId4"),u_composeCount,4);
		}
	}
	if (atoi((q.getStringFileds("SynthesizeId5")).c_str()) != 0)
	{
		u_composeCount = atoi((q.getStringFileds("SynthesizeNum5")).c_str());
		int u_ownCount = getComposeCount(q.getStringFileds("SynthesizeId5"),u_userId);
		if (u_composeCount > 0)
		{
			if (u_composeCount > u_ownCount)
			{
				g_cMyGlog.errorlogRecord("eee %s less ComposeThing: %s need:%d,ownNum:%d\n",\
					u_EvolutionId.c_str(),(q.getStringFileds("SynthesizeId5")).c_str(),u_composeCount,u_ownCount);
				q.finalize();
				return false;
			}
			setComposeTable(u_ComposeTable,q.getStringFileds("SynthesizeId5"),u_composeCount,5);
		}
	}

	u_ComposeTable.needMoney = atoi((q.getStringFileds("Money")).c_str());

	q.finalize();
	return true;
}

//插入合成成功得到的卡牌
bool cProcUserCardEvolution::insertEvolutionCard(const unsigned int& u_userId,MysqlQuery& q,unsigned int& u_itemId,\
												 const int& u_cardPosition,const int& u_cardLevel)
{
	string cardId = q.getStringFileds("cardId");
	
	int VTLGrow = getGrow(q.getIntFileds("VTLGrow"));
	int STRGrow = getGrow(q.getIntFileds("STRGrow"));
	int TGHGrow = getGrow(q.getIntFileds("TGHGrow"));
	int QUIGrow = getGrow(q.getIntFileds("QUIGrow"));
	int MGCGrow = getGrow(q.getIntFileds("MGCGrow"));
	int battlePosition = u_cardPosition;

	char bufTmp[512] = {0};
	sprintf(bufTmp,"insert into m_userCardInfo%d(itemId,userID,cardId,cardLevel,\
				cardExp,battlePosition,VTLGrow,STRGrow,TGHGrow,\
				QUIGrow,MGCGrow,skillExp,skillLevel,cardStarLevel) \
				values(NULL,%d,'%s',%d,%d,%d,%d,\
				%d,%d,%d,%d,%d,%d,%d)",u_userId%USER_CARD_TABLE_COUNT,u_userId,cardId.c_str(),u_cardLevel, \
				0,battlePosition,VTLGrow,STRGrow,TGHGrow, \
				QUIGrow,MGCGrow,0,1,q.getIntFileds("CardStarLevel"));
	string strTmpSql(bufTmp);
	if(g_cMagicServerDb.execSQL(strTmpSql,&u_itemId)!=0)
	{
		g_cMyGlog.errorlogRecord("insertEvolutionCard insert card failure,strSql:%s\n",strTmpSql.c_str());
		return false;
	}
	//cerr<<"u_itemId:"<<u_itemId<<endl;
	q.finalize();
	return true;
}

//获取合成卡牌的初始信息并插入用户卡牌表
bool cProcUserCardEvolution::getInitEvolutionCardAndInsert(const unsigned int& u_userId,const string& u_strEvolutionId,unsigned int& u_itemId,\
														   const int& u_cardPosition,const int& u_cardLevel)
{
	char buf[128] = {0};
	sprintf(buf,"select * from cardInfo where cardId = '%s';",u_strEvolutionId.c_str());
	string strSql(buf);

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);

	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getInitEvolutionCardAndInsert not find cardId,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	if (!insertEvolutionCard(u_userId,q,u_itemId,u_cardPosition,u_cardLevel))
	{
		q.finalize();
		return false;
	}
	q.finalize();
	return true;
}

//获取将要合成得到的卡牌ID
bool cProcUserCardEvolution::getEvolutionId(const string& u_cardId,string& u_EvolutionId)
{

	char buf[128] = {0};
	sprintf(buf,"select evolutionId from cardInfo where cardId = '%s';",u_cardId.c_str());
	string strSql(buf);

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);

	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("not find cardId,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_EvolutionId = q.getStringFileds("evolutionId");
	q.finalize();
	return true;
}

//得到卡牌列表，方便删除
string cProcUserCardEvolution::getItemIdList(const unsigned int& u_userId,const PBCardEvolutionSend& u_PBCardEvolutionSend)
{
	string u_strList = "";
	char buf[32] = {0};
	sprintf(buf,"%d",u_PBCardEvolutionSend.targetcarditemid());
	u_strList += buf;
	if (u_PBCardEvolutionSend.materialcarditemids_size() > 0)
	{
		for (int i = 0; i < u_PBCardEvolutionSend.materialcarditemids_size(); i++)
		{
			memset(buf,0,sizeof(buf));
			sprintf(buf,",%d",u_PBCardEvolutionSend.materialcarditemids(i));
			u_strList += buf;
		}
	}
	return u_strList;
}

//删除被合成的卡牌
void cProcUserCardEvolution::deleteCard(const unsigned int& u_userId,const PBCardEvolutionSend& u_PBCardEvolutionSend)
{
	char buf[512] = {0};
	string u_strDeletList = getItemIdList(u_userId,u_PBCardEvolutionSend);
	//cerr<<"u_strDeletList:"<<u_strDeletList.c_str()<<endl;
	sprintf(buf,"delete from m_userCardInfo%d where itemId in (%s)",u_userId%USER_CARD_TABLE_COUNT,u_strDeletList.c_str());
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("deleteCard error,strSql:%s\n",strSql.c_str());
	}
}

//要合成的卡牌是否存在
bool cProcUserCardEvolution::isCardExist(const unsigned int& u_itemId,const unsigned int& u_userId,string& u_cardId,int& u_cardPosition)
{
	char buf[128] = {0};
	sprintf(buf,"select cardId,battlePosition from m_userCardInfo%d where itemId = %d;",u_userId%USER_CARD_TABLE_COUNT,u_itemId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("isCardExist: card is not exist itemID,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_cardId = q.getStringFileds("cardId");
	u_cardPosition  = q.getIntFileds("battlePosition");
	q.finalize();
	return true;
}

//删除装备
void cProcUserCardEvolution::deleteEquip(const unsigned int& u_userId,const ComposeTable& u_ComposeTable)
{
	if (u_ComposeTable.SynthesizeNum1 > 0)
	{
		if (atoi((u_ComposeTable.SynthesizeId1).c_str()) > 1000)
		{
			deleteUserFragmentInfo(u_userId,1,u_ComposeTable.SynthesizeId1,u_ComposeTable.SynthesizeNum1);
		}
		
	}
	if (u_ComposeTable.SynthesizeNum2 > 0)
	{
		if (atoi((u_ComposeTable.SynthesizeId2).c_str()) > 1000)
		{
			deleteUserFragmentInfo(u_userId,1,u_ComposeTable.SynthesizeId2,u_ComposeTable.SynthesizeNum2);
		}

	}
	if (u_ComposeTable.SynthesizeNum3 > 0)
	{
		if (atoi((u_ComposeTable.SynthesizeId3).c_str()) > 1000)
		{
			deleteUserFragmentInfo(u_userId,1,u_ComposeTable.SynthesizeId3,u_ComposeTable.SynthesizeNum3);
		}

	}
	if (u_ComposeTable.SynthesizeNum4 > 0)
	{
		if (atoi((u_ComposeTable.SynthesizeId4).c_str()) > 1000)
		{
			deleteUserFragmentInfo(u_userId,1,u_ComposeTable.SynthesizeId4,u_ComposeTable.SynthesizeNum4);
		}

	}
	if (u_ComposeTable.SynthesizeNum5 > 0)
	{
		if (atoi((u_ComposeTable.SynthesizeId5).c_str()) > 1000)
		{
			deleteUserFragmentInfo(u_userId,1,u_ComposeTable.SynthesizeId5,u_ComposeTable.SynthesizeNum5);
		}

	}
}

//合成所需的金币是否充足
bool cProcUserCardEvolution::isEvolutionGoldEnough(const unsigned int& u_userId,const int& u_evolutionNeedGold,int& u_ownGold)
{
	u_ownGold = g_cProcUserCardLevel.getUserGold(u_userId);
	if (u_ownGold < u_evolutionNeedGold)
	{
		return false;
	}
	return true;
}

//增加合成成功的装备
bool cProcUserCardEvolution::AddEvolutionEquip(const unsigned int& u_userId,const string& u_EvolutionId)
{
	addUserFragmentInfo(u_userId,1,u_EvolutionId,1);
	return true;
}

void cProcUserCardEvolution::printComposeTable(const ComposeTable& u_ComposeTable)
{
	//cerr<<"needMoney:"<<u_ComposeTable.needMoney<<endl;
	//cerr<<"1 Id:"<<u_ComposeTable.SynthesizeId1<<endl;
	//cerr<<"1 Num:"<<u_ComposeTable.SynthesizeNum1<<endl;
	//cerr<<"2 Id:"<<u_ComposeTable.SynthesizeId2<<endl;
	//cerr<<"2 Num:"<<u_ComposeTable.SynthesizeNum2<<endl;
	//cerr<<"3 Id:"<<u_ComposeTable.SynthesizeId3<<endl;
	//cerr<<"3 Num:"<<u_ComposeTable.SynthesizeNum3<<endl;
	//cerr<<"4 Id:"<<u_ComposeTable.SynthesizeId4<<endl;
	//cerr<<"4 Num:"<<u_ComposeTable.SynthesizeNum4<<endl;
	//cerr<<"5 Id:"<<u_ComposeTable.SynthesizeId5<<endl;
	//cerr<<"5 Num:"<<u_ComposeTable.SynthesizeNum5<<endl;

}

//test
void cProcUserCardEvolution::printPBCardEvolutionSend(const PBCardEvolutionSend& u_PBCardEvolutionSend)
{
	//cerr<<"!!!!!!!!!!!!targetCardId:"<<u_PBCardEvolutionSend.targetcarditemid()<<endl;
	//cerr<<"!!!!!!!!!!!!targetGemId:"<<u_PBCardEvolutionSend.targetgemid()<<endl;
	for (int i = 0; i < u_PBCardEvolutionSend.materialcarditemids_size(); i++)
	{
		cerr<<"!!!!!!!!!!!!:"<<u_PBCardEvolutionSend.materialcarditemids(i)<<endl;
	}
	cerr<<"!!!!!!!gemID:"<<u_PBCardEvolutionSend.gemid()<<endl;
	
}
//test

//填充合成信息
int cProcUserCardEvolution::setPBCardEvolutionRecieve(PBCardEvolutionRecieve& u_PBCardEvolutionRecieve,const string& u_strToken,PBCardEvolutionSend& u_PBCardEvolutionSend)
{
	//cerr<<"setPBCardEvolutionRecieve1111:targerId:"<<u_PBCardEvolutionSend.targetcarditemid()<<endl;
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBBattleResultReceive getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	string u_strCardId = "";
	string u_strEvolutionId = "";
	int u_cardPosition = -1;
	if (u_PBCardEvolutionSend.has_targetcarditemid())
	{
		//cerr<<"has_targetcarditemid AAAAAAAA"<<endl;
		if (!isCardExist(u_PBCardEvolutionSend.targetcarditemid(),u_userId,u_strCardId,u_cardPosition))
		{
			//u_PBCardEvolutionRecieve.set_evolutionresult(EVOLUTION_CARD_NOT_EXIST);
			return K_CARD_NOT_EXIST;
		}
	}
	else if(u_PBCardEvolutionSend.has_targetgemid())
	{
		u_strCardId = u_PBCardEvolutionSend.targetgemid();
	}
	else
	{
		u_result = -1;
		return u_result;
	}
	//printPBCardEvolutionSend(u_PBCardEvolutionSend);
	string u_EvolutionId = "";
	ComposeTable u_ComposeTable;
	unsigned int u_itemId = 0;
	int u_ownGold = 0;
	//cerr<<"setPBCardEvolutionRecieve2222"<<endl;
	if (getEvolutionId(u_strCardId,u_EvolutionId))
	{
		//cerr<<"u_strCardId:"<<u_strCardId<<endl;
		//cerr<<"u_EvolutionId:"<<u_EvolutionId<<endl;
		if (atoi(u_EvolutionId.c_str()) == 0)
		{
			g_cMyGlog.errorlogRecord("%s cannot Evolution\n",u_strCardId.c_str());
			return K_CARD_CAN_NOT_EVOLUTION;
		}
		if (getEvolutionCompose(u_EvolutionId,u_userId,u_ComposeTable))
		{
			//test
			//printComposeTable(u_ComposeTable);
			//test
			if (!isEvolutionGoldEnough(u_userId,u_ComposeTable.needMoney,u_ownGold))
			{
				//u_PBCardEvolutionRecieve.set_evolutionresult(EVOLUTION_LESS_GOLD);
				return K_GOLD_NOT_ENOUGH;
			}
			unsigned int u_FinalGold = u_ownGold - u_ComposeTable.needMoney;
			int u_EvolutionId_Number = atoi(u_EvolutionId.c_str());
			if(u_EvolutionId_Number>=1000 && u_EvolutionId_Number < 2000)//1000-2000为碎片
			{
				
				if (!AddEvolutionEquip(u_userId,u_EvolutionId))
				{
					u_result = -1;
					return u_result;
				}
				deleteEquip(u_userId,u_ComposeTable);
				g_cProcUserCardLevel.updateUserGold(u_userId,u_FinalGold);
				u_PBCardEvolutionRecieve.set_evolutionresult(EVOLUTION_SUCCESS);
				
				PBAllGem* u_PBAllGem;
				u_PBAllGem = u_PBCardEvolutionRecieve.mutable_gems();
				g_cProcUserLogin.getUserGemInfo(u_PBAllGem,u_userId);

				u_PBCardEvolutionRecieve.set_allgoldnumber(u_FinalGold);
				//记录进化成功通知
				procEvolutionSuccessNotice(u_userId,u_strCardId,u_EvolutionId);
			}
			else if((u_EvolutionId_Number > 0 && u_EvolutionId_Number < 1000) || u_EvolutionId_Number >= 2000)//0-1000 或者2000以上为卡牌
			{
				int u_cardLevel = getCardLevelById(u_userId,u_PBCardEvolutionSend.targetcarditemid());
				if (u_cardLevel <= 0)
				{
					u_result = -1;
					return u_result;
				}
				if (!getInitEvolutionCardAndInsert(u_userId,u_EvolutionId,u_itemId,u_cardPosition,u_cardLevel))
				{
					return K_CARD_NOT_EXIST;
				}
				deleteCard(u_userId,u_PBCardEvolutionSend);
				deleteEquip(u_userId,u_ComposeTable);
				g_cProcUserCardLevel.updateUserGold(u_userId,u_FinalGold);
				u_PBCardEvolutionRecieve.set_evolutionresult(EVOLUTION_SUCCESS);

				PBCard* u_PBCard;
				u_PBCard = u_PBCardEvolutionRecieve.mutable_targetcard();
				g_cProcUserAwardFriend.getCardInfo(u_PBCard,u_userId,u_itemId);
		
				PBAllCard* u_PBAllCard;
				u_PBAllCard = u_PBCardEvolutionRecieve.mutable_cards();
				g_cProcUserLogin.getAllCardsInfo(u_PBAllCard,u_userId);	

				PBAllGem* u_PBAllGem;
				u_PBAllGem = u_PBCardEvolutionRecieve.mutable_gems();
				g_cProcUserLogin.getUserGemInfo(u_PBAllGem,u_userId);

				u_PBCardEvolutionRecieve.set_allgoldnumber(u_FinalGold);

				//记录进化成功通知
				procEvolutionSuccessNotice(u_userId,u_strCardId,u_EvolutionId);
			}
			else
			{
				u_result = -1;
				return u_result;
			}
			
		}
		else
		{
			//cerr<<"AAAAA"<<endl;
			return K_EVOLUTION_MATERIAL_NOT_ENOUGH;
		}
	}
	else
	{
		u_result = -1;
		return u_result;
	}
	int u_userFightCapacity = 0;
	int u_userCapitorCardFightCapacity = 0;
	u_userFightCapacity = getUserFightCapacity(u_userId,u_userCapitorCardFightCapacity);
	updateUserFightCapacity(u_userId,u_userFightCapacity,u_userCapitorCardFightCapacity);
	u_PBCardEvolutionRecieve.set_fightcapacity(u_userFightCapacity);

	//记录战斗力突破通知
	procFightCapacityBreakThrough(u_userId,u_userFightCapacity);

	return u_result;
}	