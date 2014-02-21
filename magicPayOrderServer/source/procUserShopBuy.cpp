#include "../include/procUserShopBuy.h"
#include "../include/magicServerDb.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"
#include </usr/include/curl/curl.h>



cProcUserShopBuy g_cProcUserShopBuy;

typedef enum shopBuyEnum
{
	ENERGY_REGAIN = 1,//疲劳值恢复
	ADD_BAG = 1001,//背包扩充
	//BUY_GOLD,//换取金币
	BUY_6_YUAN_MAGIC_STON,//购买六块钱魔石
	BUY_36_YUAN_MAGIC_STON,//购买36块钱魔石
	BUY_98_YUAN_MAGIC_STON,//购买98块钱魔石
	BUY_388_YUAN_MAGIC_STON,//购买388块钱魔石
	BUY_648_YUAN_MAGIC_STON,	//购买648块钱魔石
	REVIVE_REQ = 8	//复活请求
}shopBuyEnum;

//背包扩充一次能增加的背包数目
#define ONE_TIME_ADD_BAG_COUNT	20

//背包扩充一次所需魔石
#define ADD_BAG_NEED_MAGIC_STON	0

//购买一次金币所需花费的魔石数
#define BUY_GOLD_NEED_MAGIC_STON	88
//购买一次金币能得到的金币数目
#define BUY_GOLD_GET_GOLD	10000

//复活所需魔石
//#define REVIVE_NEED_MAGIC_STON(reviveCount)	(reviveCount+2)*(reviveCount+2)*10


int curlWriteCallback(char *data, size_t size, size_t nmemb, string *writerData)
{
	if (writerData == NULL)
		return 0;
	int len = size*nmemb;
	writerData->append(data, len);

	return len;
}

cProcUserShopBuy::cProcUserShopBuy()
{

}
cProcUserShopBuy::~cProcUserShopBuy()
{

}


//复活所需魔石
int cProcUserShopBuy::getReviveReqNeedDiamond(const int& reviveCount)
{
	int u_retDiamond = 60;
	if (reviveCount > 0)
	{
		for (int i = 1; i < reviveCount; i++)
		{
			u_retDiamond *= 2;
		}
	}
	return u_retDiamond;
}

//更新用户背包数目
void cProcUserShopBuy::updateUserBagCountAndDiamond(const unsigned int& u_userId,const int& u_bagCount,const int& u_diamondNum)
{
	char buf[128] = {0};
	sprintf(buf,"update m_gameUserInfo set diamondNumber = %d,maxOwnCardCount = %d where userID = %d;",u_diamondNum,u_bagCount,u_userId);
	string strSql(buf);
	g_cMagicServerDb.execSQL(strSql);
}

//获取用户背包和钻石数
bool cProcUserShopBuy::getUserBagCountAndDiamond(const unsigned int& u_userId,int& u_bagCount,int& u_diamondNum)
{
	char buf[128] = {0};
	sprintf(buf,"select diamondNumber,maxOwnCardCount from m_gameUserInfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserBagCountAndDiamond failure,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_diamondNum = q.getIntFileds("diamondNumber");
	u_bagCount = q.getIntFileds("maxOwnCardCount");
	q.finalize();
	return true;
}

//背包扩充设置
int cProcUserShopBuy::setAddBag(const unsigned int& u_userId,const int& u_addNum,const int& u_needMoney)
{
	int u_result = 0;
	int u_bagCount = 0,u_diamondNum = 0;
	if (!getUserBagCountAndDiamond(u_userId,u_bagCount,u_diamondNum))
	{
		u_result = -1;
		return u_result;
	}
	if (u_diamondNum < ADD_BAG_NEED_MAGIC_STON)
	{
		return K_MAGIC_STONE_NOT_ENOUGH;
	}
	u_bagCount += u_addNum;
	u_diamondNum -= ADD_BAG_NEED_MAGIC_STON;
	updateUserBagCountAndDiamond(u_userId,u_bagCount,u_diamondNum);
	return u_result;
	
}

//获取体力药水计数
bool cProcUserShopBuy::getUserEnergyPotionCountAndDiamond(const unsigned int& u_userId,int& u_diamondNumber,\
										int& u_buyEnergyTime,int& u_lastEnergyTime,int& u_energyPotionCount)
{
	char buf[128] = {0};
	sprintf(buf,"select diamondNumber,buyEnergyTime,lastEnergyTime,energyCount from m_gameUserInfo where \
				userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserEnergyPotionCountAndDiamond m_gameUserInfo not find userId,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_diamondNumber = q.getIntFileds("diamondNumber");
	u_buyEnergyTime = q.getIntFileds("buyEnergyTime");
	u_lastEnergyTime = q.getIntFileds("lastEnergyTime");
	u_energyPotionCount = q.getIntFileds("energyCount");
	q.finalize();
	return true;
}

//获取体力和钻石
bool cProcUserShopBuy::getUserEnergyAndDiamond(const unsigned int& u_userId,int& u_diamondNumber,\
														  int& u_userLevel,int& u_lastEnergyTime,int& u_energy)
{
	char buf[128] = {0};
	sprintf(buf,"select userLevel,energy,diamondNumber,lastEnergyTime from m_gameUserInfo where \
				userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserEnergyAndDiamond m_gameUserInfo not find userId,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_userLevel = q.getIntFileds("userLevel");
	u_energy = q.getIntFileds("energy");
	u_diamondNumber = q.getIntFileds("diamondNumber");
	u_lastEnergyTime = q.getIntFileds("lastEnergyTime");
	q.finalize();
	return true;
}

//更新体力和钻石
void cProcUserShopBuy::updateEnergyAndDiamond(const unsigned int& u_userId,const int& u_diamondNumber,\
											const int& u_lastEnergyTime,const int& u_energy)
{
	char buf[256] = {0};
	sprintf(buf,"update m_gameUserInfo set energy=%d,diamondNumber=%d, \
			lastEnergyTime=%d where userID = %d;",\
				u_energy,u_diamondNumber,u_lastEnergyTime,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateEnergyAndDiamond error,strSql:%s\n",strSql.c_str());
	}
}

//更新体力药水计数
void cProcUserShopBuy::updateEnergyPotionCountAndDiamond(const unsigned int& u_userId,int& u_diamondNumber,\
									   int& u_buyEnergyTime,int& u_lastEnergyTime,int& u_energyPotionCount)
{
	char buf[256] = {0};
	sprintf(buf,"update m_gameUserInfo set diamondNumber=%d, \
				buyEnergyTime=%d,lastEnergyTime=%d,energyCount=%d where userID = %d;",\
				u_diamondNumber,u_buyEnergyTime,u_lastEnergyTime,u_energyPotionCount,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateEnergyPotionCountAndDiamond error,strSql:%s\n",strSql.c_str());
	}
}

//购买疲劳请求处理
int cProcUserShopBuy::setBuyEnergy(const unsigned int& u_userId)
{
	int u_result = 0;
	int u_energy=0,u_diamondNumber=0,u_userLevel=0,u_lastEnergyTime=0;

	if(!getUserEnergyAndDiamond(u_userId,u_diamondNumber,\
		u_userLevel,u_lastEnergyTime,u_energy))
	{
		u_result = -1;
		return u_result;
	}
	if (u_diamondNumber < ENERGY_DIAMOND_NEED)
	{
		return K_MAGIC_STONE_NOT_ENOUGH;
	}
	else
	{
		u_lastEnergyTime = GetTimeStamp();
		u_energy = ENERGY + (u_userLevel-1)/2;

		u_diamondNumber -= ENERGY_DIAMOND_NEED;

		updateEnergyAndDiamond(u_userId,u_diamondNumber,\
			u_lastEnergyTime,u_energy);
	}
	return u_result;
}

//获取用户金币和魔石
bool cProcUserShopBuy::getUserGoldAndDiamondNum(const unsigned int& u_userId,int& u_goldNum,int& u_diamondNum)
{
	char buf[128] = {0};
	sprintf(buf,"select goldNumber,diamondNumber from m_gameuserinfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserGoldAndDiamondNum error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_goldNum = q.getIntFileds("goldNumber");
	u_diamondNum = q.getIntFileds("diamondNumber");
	q.finalize();
	return true;
}

//更新用户金币和魔石
bool cProcUserShopBuy::updateUserGoldDiamond(const unsigned int& u_userId,const int& u_goldNum,bool u_updateGoldFlag,\
						   int& u_diamondNum,bool u_updateDiamondFlag)
{
	char buf[256] = {0};
	if (u_updateGoldFlag && u_updateDiamondFlag)
	{
		sprintf(buf,"update m_gameuserinfo set goldNumber = %d,diamondNumber = %d where userID = %d;",\
			u_goldNum,u_diamondNum,u_userId);
	}
	else if (u_updateGoldFlag && !u_updateDiamondFlag)
	{
		sprintf(buf,"update m_gameuserinfo set goldNumber = %d where userID = %d;",\
			u_goldNum,u_userId);
	}
	else if (!u_updateGoldFlag && u_updateDiamondFlag)
	{
		sprintf(buf,"update m_gameuserinfo set diamondNumber = %d where userID = %d;",\
			u_diamondNum,u_userId);
	}
	else
	{
		return false;
	}
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUserGoldDiamond error!,strSql:%s\n",strSql.c_str());
		return false;
	}
	return true;
}

//购买金币处理
int cProcUserShopBuy::setBuyGold(const unsigned int& u_userId)
{
	int u_result = 0;
	int u_goldNum = 0,u_diamondNum = 0;
	if (!getUserGoldAndDiamondNum(u_userId,u_goldNum,u_diamondNum))
	{
		u_result = -1;
		return u_result;
	}
	if (u_diamondNum < BUY_GOLD_NEED_MAGIC_STON)
	{
		return K_MAGIC_STONE_NOT_ENOUGH;
	}
	u_diamondNum -= BUY_GOLD_NEED_MAGIC_STON;
	u_goldNum += BUY_GOLD_GET_GOLD;
	if (!updateUserGoldDiamond(u_userId,u_goldNum,true,u_diamondNum,true))
	{
		u_result = -1;
		return u_result;
	}
	return u_result;
}

//获取用户累计充值金额和魔石数目
bool cProcUserShopBuy::getUserMoneyPay(const unsigned int& u_userId,int& u_userDiamondNum,int& u_moneyTotal)
{
	char buf[128] = {0};
	sprintf(buf,"select diamondNumber,moneyPay from m_gameuserinfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserMoneyTotal error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_userDiamondNum = q.getIntFileds("diamondNumber");
	u_moneyTotal = q.getIntFileds("moneyPay");
	q.finalize();
	return true;
}

//更新用户累计充值金额和魔石数目
void cProcUserShopBuy::updateUserMoneyPayAndDiamondNum(const unsigned int& u_userId,const int& u_moneyTotal,const int& u_userDiamond)
{
	char buf[128] = {0};
	sprintf(buf,"update m_gameuserinfo set diamondNumber = %d,moneyPay = %d where userID = %d;",u_userDiamond,u_moneyTotal,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUserMoneyTotal error,strSql:%s\n",strSql.c_str());
	}
}

//获取充值任务条件
string cProcUserShopBuy::getPayMissionCondition(const int& u_userMoneyTotal,const int& u_moneyAdd)
{
	string u_missionCondition = "";
	char buf[20] = {0};
	int u_condition = 0;
	int u_totalMoney = u_userMoneyTotal + u_moneyAdd;

	if (u_totalMoney >= M_1_YUAN_CONDITION && u_totalMoney < M_50_YUAN_CONDITION)
	{
		u_condition = M_1_YUAN_CONDITION;
	}
	else if (u_totalMoney >= M_50_YUAN_CONDITION && u_totalMoney < M_100_YUAN_CONDITION)
	{
		u_condition = M_50_YUAN_CONDITION;
	}
	else if (u_totalMoney >= M_100_YUAN_CONDITION && u_totalMoney < M_500_YUAN_CONDITION)
	{
		u_condition = M_100_YUAN_CONDITION;
	}
	else if (u_totalMoney >= M_500_YUAN_CONDITION && u_totalMoney < M_1500_YUAN_CONDITION)
	{
		u_condition = M_500_YUAN_CONDITION;
	}
	else if (u_totalMoney >= M_1500_YUAN_CONDITION && u_totalMoney < M_2000_YUAN_CONDITION)
	{
		u_condition = M_1500_YUAN_CONDITION;
	}
	else if (u_totalMoney >= M_2000_YUAN_CONDITION && u_totalMoney < M_3000_YUAN_CONDITION)
	{
		u_condition = M_2000_YUAN_CONDITION;
	}
	else if (u_totalMoney >= M_3000_YUAN_CONDITION && u_totalMoney < M_5000_YUAN_CONDITION)
	{
		u_condition = M_3000_YUAN_CONDITION;
	}
	else if (u_totalMoney >= M_5000_YUAN_CONDITION && u_totalMoney < 6000)
	{
		u_condition = M_5000_YUAN_CONDITION;
	}
	else 
	{
		g_cMyGlog.errorlogRecord("unkown award:%d\n",u_totalMoney);
		return u_missionCondition;
	}
	sprintf(buf,"%d",u_condition);
	u_missionCondition = buf;
	return u_missionCondition;

}

//获取充值任务条件列表
string cProcUserShopBuy::getPayMissionConditionList(const unsigned int& u_userId,const string& u_payMissionCondition)
{
	string u_retList = "";
	int u_condition = atoi(u_payMissionCondition.c_str());
	char buf[64] = {0};
	if (u_condition == M_1_YUAN_CONDITION)
	{
		sprintf(buf,"%d",u_condition);
	}
	else if (u_condition == M_50_YUAN_CONDITION)
	{
		sprintf(buf,"%d,%d",M_1_YUAN_CONDITION,u_condition);
	}
	else if (u_condition == M_100_YUAN_CONDITION)
	{
		sprintf(buf,"%d,%d,%d",M_1_YUAN_CONDITION,M_50_YUAN_CONDITION,u_condition);
	}
	else if (u_condition == M_500_YUAN_CONDITION)
	{
		sprintf(buf,"%d,%d,%d,%d",M_1_YUAN_CONDITION,M_50_YUAN_CONDITION,M_100_YUAN_CONDITION,u_condition);
	}
	else if (u_condition == M_1500_YUAN_CONDITION)
	{
		sprintf(buf,"%d,%d,%d,%d,%d",M_1_YUAN_CONDITION,M_50_YUAN_CONDITION,\
			M_100_YUAN_CONDITION,M_500_YUAN_CONDITION,u_condition);
	}
	else if (u_condition == M_2000_YUAN_CONDITION)
	{
		sprintf(buf,"%d,%d,%d,%d,%d,%d",M_1_YUAN_CONDITION,M_50_YUAN_CONDITION,\
			M_100_YUAN_CONDITION,M_500_YUAN_CONDITION,M_1500_YUAN_CONDITION,u_condition);
	}
	else if (u_condition == M_3000_YUAN_CONDITION)
	{
		sprintf(buf,"%d,%d,%d,%d,%d,%d,%d",M_1_YUAN_CONDITION,M_50_YUAN_CONDITION,\
			M_100_YUAN_CONDITION,M_500_YUAN_CONDITION,M_1500_YUAN_CONDITION,M_2000_YUAN_CONDITION,u_condition);
	}
	else if (u_condition == M_5000_YUAN_CONDITION)
	{
		sprintf(buf,"%d,%d,%d,%d,%d,%d,%d,%d",M_1_YUAN_CONDITION,M_50_YUAN_CONDITION,\
			M_100_YUAN_CONDITION,M_500_YUAN_CONDITION,M_1500_YUAN_CONDITION,\
			M_2000_YUAN_CONDITION,M_3000_YUAN_CONDITION,u_condition);
	}
	else
	{
		g_cMyGlog.errorlogRecord("unkown payCondition::u_condition:%d\n",u_condition);
		return u_retList;
	}
	u_retList = buf;
	return u_retList;
}

//根据任务类型更新任务完成状态
void cProcUserShopBuy::updateMissionStatus(const unsigned int& u_userId,const int& u_missionType,const string& u_missionCondition,bool isFinished)
{
	char buf[256] = {0};
	if (u_missionType == BATTLE_MISSION || u_missionType == LEVEL_UP_MISSION)
	{
		sprintf(buf,"update m_userMissionsInfo%d set isFinished = %d where userId = %d and missinType = %d and missionCondition = '%s';",\
			u_userId%USER_MISSION_TABLE_COUNT,isFinished,u_userId,u_missionType,u_missionCondition.c_str());
	}
	else if (u_missionType == PAY_MISSION)
	{
		sprintf(buf,"update m_userMissionsInfo%d set isFinished = %d where userId = %d and missinType = %d and missionCondition in (%s);",\
			u_userId%USER_MISSION_TABLE_COUNT,isFinished,u_userId,u_missionType,u_missionCondition.c_str());
	}
	else
	{
		g_cMyGlog.errorlogRecord("unkown  missionType:%d\n",u_missionType);
		return;
	}
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateMissionStatus error,strSql:%s\n",strSql.c_str());
	}
}

//根据任务类型判断任务是否存在
bool cProcUserShopBuy::isMissionExist(const unsigned int& u_userId,const int& u_missionType,const string& u_missionCondition,int& isFinished)
{
	char buf[256] = {0};
	if (u_missionType == PAY_MISSION)
	{
		sprintf(buf,"select isFinished from m_userMissionsInfo%d where userId = %d and missinType = %d and missionCondition in (%s);",\
			u_userId%USER_MISSION_TABLE_COUNT,u_userId,u_missionType,u_missionCondition.c_str());
	}
	else
	{
		g_cMyGlog.errorlogRecord("unkown  missionType:%d\n",u_missionType);
		return false;
	}
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("isMissionExist get nothing:strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	isFinished = q.getIntFileds("isFinished");
	q.finalize();
	return true;
}


//购买魔石处理
int cProcUserShopBuy::setBuyMagicSton(const unsigned int& u_userId,const int& u_buyType,const int& u_addDiamonds,const int& u_needMoney)
{
	int u_result = 0;
	int u_MoneyPay = 0,u_diamondNum = 0,u_MoneyPayAdd = 0;
	if (!getUserMoneyPay(u_userId,u_diamondNum,u_MoneyPay))
	{
		u_result = -1;
		return u_result;
	}
	switch(u_buyType)
	{
		case BUY_6_YUAN_MAGIC_STON:
			{
				u_diamondNum += u_addDiamonds;
				u_MoneyPayAdd = u_needMoney;
				break;
			}
		case BUY_36_YUAN_MAGIC_STON:
			{
				u_diamondNum += u_addDiamonds;
				u_MoneyPayAdd = u_needMoney;
				break;
			}
		case BUY_98_YUAN_MAGIC_STON:
			{
			u_diamondNum += u_addDiamonds;
			u_MoneyPayAdd = u_needMoney;
			break;
			}
		case BUY_388_YUAN_MAGIC_STON:
			{
			u_diamondNum += u_addDiamonds;
			u_MoneyPayAdd = u_needMoney;
			break;
			}
		case BUY_648_YUAN_MAGIC_STON:
			{
				u_diamondNum += u_addDiamonds;
				u_MoneyPayAdd = u_needMoney;
				break;
			}
		default:
			{
				u_result = -1;
				return u_result;
			}
	}
	if (u_result != -1)
	{
		int u_moneyPayTotal = u_MoneyPay + u_MoneyPayAdd;
		updateUserMoneyPayAndDiamondNum(u_userId,u_moneyPayTotal,u_diamondNum);
		string u_missionCondition = getPayMissionCondition(u_MoneyPay,u_MoneyPayAdd);
		if (u_missionCondition.size() > 0)
		{
			string u_missionConditionList = "";
			u_missionConditionList = getPayMissionConditionList(u_userId,u_missionCondition);
			if (u_missionConditionList.size() > 0)
			{
				int u_missionIsFinished = 0;
				if (isMissionExist(u_userId,PAY_MISSION,u_missionConditionList,u_missionIsFinished))
				{
					updateMissionStatus(u_userId,PAY_MISSION,u_missionConditionList,true);
				}
			}
		}
		updateUserReChargedDiamondNum(u_userId,u_addDiamonds);
	}
	return u_result;
}

//获取用户复活次数
bool cProcUserShopBuy::getUserBattleReviveCount(const unsigned int& u_userId,int& u_reviveCount)
{
	char buf[128] = {0};
	sprintf(buf,"select reviveCount from m_goldtmpsaveinfo where userId = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserBattleReviveCount error!,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	u_reviveCount = q.getIntFileds("reviveCount");
	q.finalize();
	return true;
}

//更新用户复活次数
void cProcUserShopBuy::updateUserBattleReviveCount(const unsigned int& u_userId,const int& u_reviveCount)
{
	char buf[128] = {0};
	sprintf(buf,"update m_goldtmpsaveinfo set reviveCount = %d where userId = %d;",u_reviveCount,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUserBattleReviveCount error,strSql:%s\n",strSql.c_str());

	}
	
}

//处理复活请求
int cProcUserShopBuy::setReviveReq(const unsigned int& u_userId,const int& u_buyType)
{
	int u_result = 0,u_reviveCount = 0,u_diamondNum = 0,u_goldNum = 0;
	if (!getUserBattleReviveCount(u_userId,u_reviveCount))
	{
		u_result = -1;
		return u_result;
	}
	if (!getUserGoldAndDiamondNum(u_userId,u_goldNum,u_diamondNum))
	{
		u_result = -1;
		return u_result;
	}
	int u_reviveNeedDiamond = getReviveReqNeedDiamond(u_reviveCount);
	if(u_diamondNum < u_reviveNeedDiamond)
	{
		g_cMyGlog.errorlogRecord("setReviveReq less Diamond,userId:%d,ownDiamond:%d,ReviveReq Diamond:%d\n",\
			u_userId,u_diamondNum,u_reviveNeedDiamond/*REVIVE_NEED_MAGIC_STON(u_reviveCount)*/);
		return K_MAGIC_STONE_NOT_ENOUGH;
	}
	u_diamondNum -= u_reviveNeedDiamond;//REVIVE_NEED_MAGIC_STON(u_reviveCount);
	u_reviveCount += 1;
	updateUserBattleReviveCount(u_userId,u_reviveCount);
	if (!updateUserGoldDiamond(u_userId,u_goldNum,false,u_diamondNum,true))
	{
		u_result = -1;
		return u_result;
	}
	return u_result;
}



//处理商店购买请求
int cProcUserShopBuy::procShopBuy(const unsigned int& u_userId,const int& u_buyType)
{
	int u_result = 0;
	switch(u_buyType)
	{
		case ENERGY_REGAIN:
			u_result = setBuyEnergy(u_userId);
			break;
		case REVIVE_REQ:
			u_result = setReviveReq(u_userId,REVIVE_REQ);
			break;
		default:
			u_result = -1;
			break;
	}
	
	return u_result;
}


int cProcUserShopBuy::setPBShopBuySendReceive(PBShopBuySendReceive& u_PBShopBuySendReceive,const string& u_strToken,\
							const PBShopBuySend& u_PBShopBuySend)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBShopBuySendReceive getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	int u_buyType = u_PBShopBuySend.buytype();
	u_result = procShopBuy(u_userId,u_buyType);
	if (u_result != 0)
	{
		return u_result;
	}

	PBUser* u_PBUser;
	u_PBUser = u_PBShopBuySendReceive.mutable_user();
	setPBUser(u_PBUser,u_userId);
	return u_result;
}

void cProcUserShopBuy::update91ShopBuyOrderRocord(const string& u_cooOrderSerial)
{
	char buf[128] = {0};
	sprintf(buf,"update m_order_table set isProcessed=1 where cooOrderSerial='%s';",u_cooOrderSerial.c_str());
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("update91ShopBuyOrderRocord failure,strSql:%s\n",strSql.c_str());
		return;
	}
}

int cProcUserShopBuy::getAddNumByBuyType(const int& u_buyType,int& u_needMoney)
{
	int u_retAddNum = 0;
	char buf[128] = {0};
	sprintf(buf,"select addNum,needMoney from m_product_price where buyType=%d;",u_buyType);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.execQuery(strSql);
	if (!q.eof())
	{
		u_retAddNum = q.getIntFileds("addNum");
		u_needMoney = q.getIntFileds("needMoney");
	}
	q.finalize();
	return u_retAddNum;
}

//处理91商店购买请求
int cProcUserShopBuy::proc91ShopBuy(const unsigned int& u_userId,const int& u_buyType)
{
	int u_result = 0;
	int u_addNum = 0;
	int u_needMoney = 0;
	u_addNum = getAddNumByBuyType(u_buyType,u_needMoney);
	switch(u_buyType)
	{
		g_cMyGlog.debugLog("proc91ShopBuy:Type :%d\n",u_buyType);
		case ADD_BAG:
			u_result = setAddBag(u_userId,u_addNum,u_needMoney);
			break;
		case BUY_6_YUAN_MAGIC_STON:
			u_result = setBuyMagicSton(u_userId,BUY_6_YUAN_MAGIC_STON,u_addNum,u_needMoney);
			break;
		case BUY_36_YUAN_MAGIC_STON:
			u_result = setBuyMagicSton(u_userId,BUY_36_YUAN_MAGIC_STON,u_addNum,u_needMoney);
			break;
		case BUY_98_YUAN_MAGIC_STON:
			u_result = setBuyMagicSton(u_userId,BUY_98_YUAN_MAGIC_STON,u_addNum,u_needMoney);
			break;
		case BUY_388_YUAN_MAGIC_STON:
			u_result = setBuyMagicSton(u_userId,BUY_388_YUAN_MAGIC_STON,u_addNum,u_needMoney);
			break;
		case BUY_648_YUAN_MAGIC_STON:
			u_result = setBuyMagicSton(u_userId,BUY_648_YUAN_MAGIC_STON,u_addNum,u_needMoney);
			break;
		default:
			u_result = -1;
			break;

	}
	
	return u_result;
}

int cProcUserShopBuy::getGoodsIdByGoodsOrder(const PBIAPSend& u_PBIAPSend,int& u_isOrderProceed)
{
	int u_goodsId = 0;
	char buf[128] = {0};
	sprintf(buf,"select goodsId,isProcessed from m_order_table where cooOrderSerial='%s';",(u_PBIAPSend.orderid()).c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("getGoodsIdByGoodsOrder error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return u_goodsId;
	}
	u_goodsId = atoi((q.getStringFileds("goodsId")).c_str());
	u_isOrderProceed = q.getIntFileds("isProcessed");
	q.finalize();
	return u_goodsId;

}

void cProcUserShopBuy::procNotProccessedOrder()
{
	char buf[128] = {0};
	sprintf(buf,"select cooOrderSerial,Uin,goodsId from m_order_table where isProcessed=0;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("procNotProccessedOrder getnothing:strSql:%s\n",strSql.c_str());
		q.finalize();
		return;
	}
	while (!q.eof())
	{
		unsigned int u_userId = getUserIdByUserMacAddress(q.getStringFileds("Uin"));
		int u_goodsId = atoi((q.getStringFileds("goodsId")).c_str());
		if (u_userId > 0 && u_goodsId > 0)
		{
			proc91ShopBuy(u_userId,u_goodsId);
			update91ShopBuyOrderRocord(q.getStringFileds("cooOrderSerial"));
		}
		q.nextRow();
	}
	q.finalize();
	return;
}

//获取支付服务器的url
string cProcUserShopBuy::getPayServerUrlByGameServerUrl(const string& u_gameServerUrl)
{
	char buf[128] = {0};
	sprintf(buf,"select * from game_db_serverurl where gameServerUrl='%s';",u_gameServerUrl.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	string n_retPayServerUrl = "";
#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("getPayServerUrlByGameServerUrl error,strSql:%s\n",strSql.c_str());
#endif
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getPayServerUrlByGameServerUrl error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return n_retPayServerUrl;
	}
	n_retPayServerUrl = q.getStringFileds("payServerUrl");
	q.finalize();
	return n_retPayServerUrl;
}

int cProcUserShopBuy::proc91PBIAPRecv(PBIAPRecv& u_PBIAPRecv,const unsigned int& u_userId,const PBIAPSend& u_PBIAPSend)
{
	int u_result = 0;
	if ((u_PBIAPSend.orderid()).size() <= 0 )
	{
		u_result = -1;
		return u_result;
	}
#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("orderid:%s\n",u_PBIAPSend.orderid().c_str());
#endif
	int n_isOrderProceed = 0;
	int u_goodsId = getGoodsIdByGoodsOrder(u_PBIAPSend,n_isOrderProceed);
	//91没有及时把订单通知过来
	if (u_goodsId == 0)
	{
		//添加根据订单号向91查询该订单的支付结果
		CURL *curl = curl_easy_init();
		if(curl)
		{

			string n_payServerUrl = getPayServerUrlByGameServerUrl(u_PBIAPSend.serverurl());
			//cerr<<"n_payServerUrl:"<<n_payServerUrl<<endl;
			if (n_payServerUrl.empty())
			{
				u_result = 1;
			}
			else
			{
				string u_QueryIAPURL="http://";
				
				u_QueryIAPURL += n_payServerUrl;
				u_QueryIAPURL += "/QueryOrderResultDemo.php";
				//u_QueryIAPURL += u_PBIAPSend.orderid();
				string u_sendJsonData = u_PBIAPSend.orderid();
				CURLcode res;
				//g_cMyGlog.debugLog("2222\n");
				//res = curl_easy_setopt(curl, CURLOPT_RETURNTRANSFER, true);   
				res = curl_easy_setopt(curl, CURLOPT_URL, u_QueryIAPURL.c_str());    
				res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, u_sendJsonData.c_str());
				res = curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE,u_sendJsonData.size());
				res = curl_easy_setopt(curl, CURLOPT_POST, true); 
				//res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);  //这两行一定要加，不加会报SSL 错误  
				//res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
				//char u_stream[10240] = {0};
				string u_stream = "";
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);
				res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &u_stream);

				res = curl_easy_perform(curl);

				//g_cMyGlog.debugLog("u_QueryIAPURL:%s\n",u_QueryIAPURL.c_str());
				if(CURLE_OK == res) 
				{
					/*cerr<<"u_stream.size:"<<u_stream.size()<<endl;
					cerr<<"u_stream:"<<u_stream<<endl;*/
					u_goodsId = getGoodsIdByGoodsOrder(u_PBIAPSend,n_isOrderProceed);
					if (u_goodsId == 0)
					{
						u_result = 1;
					}
					else
					{
						if (n_isOrderProceed == 0)
						{
							int u_ret = proc91ShopBuy(u_userId,u_goodsId);
							if (u_ret == 0)
							{
								update91ShopBuyOrderRocord(u_PBIAPSend.orderid());
							}
						}
						u_result = 0;
					}
					//g_cMyGlog.debugLog("u_result:%d\n",u_result);
				}
			}
		}
		else
		{
			u_result = 1;
		}
	}
	else
	{
		u_PBIAPRecv.set_result(0);
#ifdef DEBUG_MOD
		g_cMyGlog.debugLog("u_userId:%d,u_goodsId:%d,n_isOrderProceed:%d\n",u_userId,u_goodsId,n_isOrderProceed);
#endif
		if (n_isOrderProceed == 0)
		{
			int u_ret = proc91ShopBuy(u_userId,u_goodsId);
			if (u_ret == 0)
			{
				update91ShopBuyOrderRocord(u_PBIAPSend.orderid());
			}
		}

	}
	return u_result;
}


bool cProcUserShopBuy::isIosOrderProcced(const string& u_transaction_id)
{
	char buf[128] = {0};
	bool retFlag = false;
	sprintf(buf,"select isProcced from m_ios_order_table where transaction_id='%s';",u_transaction_id.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("isIosOrderProcced error,strSql:%s\n",strSql.c_str());
	}
	else
	{
		int n_procced = q.getIntFileds("isProcced");
		if (n_procced == 1)
		{
			retFlag = true;
		}
	}
	q.finalize();
	return retFlag;
}

int cProcUserShopBuy::procIOSPBIAPRecv(PBIAPRecv& u_PBIAPRecv,const unsigned int& u_userId,const PBIAPSend& u_PBIAPSend)
{
	int u_result = 0;
	if (isIosOrderProcced(u_PBIAPSend.orderid()))
	{
		return u_result;
	}
	if (u_PBIAPSend.authorizationdata().size() <= 0)
	{
		g_cMyGlog.errorlogRecord("procIOSPBIAPRecv has no authorizationdata\n");
		u_result = -1;
		return u_result;
	}
	CURL *curl = curl_easy_init();
	if(curl) {
		//正式地址
		//string u_appleIAPURL = "https://buy.itunes.apple.com/verifyReceipt";
		//测试地址
		string u_appleIAPURL="";
#ifdef DEBUG_MOD
		u_appleIAPURL = "https://sandbox.itunes.apple.com/verifyReceipt";
#else
		u_appleIAPURL = "https://buy.itunes.apple.com/verifyReceipt";
#endif
		g_cMyGlog.debugLog("u_appleIAPURL:%s\n",u_appleIAPURL.c_str());

		string u_sendJsonData= m_cMyJsonParser.MyWriteJson(u_PBIAPSend.authorizationdata());
		if (u_sendJsonData.size() <= 0)
		{
			u_result = 1;
			g_cMyGlog.errorlogRecord("u_sendJsonData get nothing\n");
			return u_result;
		}
		CURLcode res;
		//g_cMyGlog.debugLog("2222\n");
		//res = curl_easy_setopt(curl, CURLOPT_RETURNTRANSFER, true);   
		res = curl_easy_setopt(curl, CURLOPT_URL, u_appleIAPURL.c_str());    
		res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, u_sendJsonData.c_str());
		res = curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE,u_sendJsonData.size());
		res = curl_easy_setopt(curl, CURLOPT_POST, true); 
		res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);  //这两行一定要加，不加会报SSL 错误  
		res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		//char u_stream[10240] = {0};
		string u_stream = "";
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);
		res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &u_stream);

		res = curl_easy_perform(curl);

		//g_cMyGlog.debugLog("u_stream:%s\n",u_stream.c_str());
		//g_cMyGlog.debugLog("recvd:%s\n",u_stream);

		if(CURLE_OK == res) {
			//char *ct;
			/* ask for the content-type */
			/* http://curl.haxx.se/libcurl/c/curl_easy_getinfo.html */
			u_result = m_cMyJsonParser.procJsonData(u_userId,u_PBIAPSend.macaddress(),u_stream);
			if (u_result == -1)
			{
				g_cMyGlog.errorlogRecord("procJsonData error,u_stream:%s\n",u_stream.c_str());
				u_result = 1;
			}
			//g_cMyGlog.debugLog("u_result:%d\n",u_result);
		}
		else
		{
			g_cMyGlog.errorlogRecord("not CURLE_OK!!!!\n");
			u_result = 1;
		}
		//g_cMyGlog.debugLog("5555\n");
		/* always cleanup */
		curl_easy_cleanup(curl);
		//g_cMyGlog.debugLog("6666\n");
	}
	else
	{
		g_cMyGlog.errorlogRecord("curl_easy_init error\n");
		u_result = 1;
	}
	return u_result;
}

//获取体力恢复倒计时
int cProcUserShopBuy::getNextEnergyTime(const int& u_nowStamp,int& u_lastEnergyTimeStamp,\
										  int& u_energy,const int& u_userLevel)
{
	int u_timeDif = u_nowStamp - u_lastEnergyTimeStamp;
	int u_maxEnergy = ENERGY + (u_userLevel-1)/2;
	int u_nextEnergyTime = 0;
	if (u_timeDif < ONE_ENERGY_NEED_TIME)
	{
		if (u_energy >= u_maxEnergy)
		{
			u_energy = u_maxEnergy;
			u_nextEnergyTime = 0;
		}
		else
		{
			u_nextEnergyTime = ONE_ENERGY_NEED_TIME - u_timeDif;
		}
	}
	else
	{
		if (u_energy >= u_maxEnergy)
		{
			u_energy = u_maxEnergy;
			u_nextEnergyTime = 0;
		}
		else
		{
			u_energy += u_timeDif/ONE_ENERGY_NEED_TIME;
			if (u_energy >= u_maxEnergy)
			{
				u_energy = u_maxEnergy;
				u_nextEnergyTime = 0;
			}
			else
			{
				u_nextEnergyTime = ONE_ENERGY_NEED_TIME - u_timeDif%ONE_ENERGY_NEED_TIME;
			}

		}
		u_lastEnergyTimeStamp = u_nowStamp;
	}
	return u_nextEnergyTime;
}

//更新体力和最新获取体力的时间戳
bool cProcUserShopBuy::updateUserEnergyAndTime(const unsigned int& u_energy,const unsigned int& u_lastEnergyTime,const unsigned int& u_userId)
{
	char buf[128] = {0};

	sprintf(buf,"update m_gameUserInfo set energy = %d,lastEnergyTime=%d where userID = %d;",u_energy,u_lastEnergyTime,u_userId);
	string strSql(buf);

	if(g_cMagicServerDb.execSQL(strSql)!=0)
	{
		g_cMyGlog.errorlogRecord("updateUserEnergyAndTime failure,strSql:%s\n",strSql.c_str());
		return false;
	}
	return true;
}

//判断是否有任务完成
bool cProcUserShopBuy::hasMissionFinished(const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select missionId from m_userMissionsInfo%d where userId = %d and isFinished = 1;",\
		u_userId%USER_MISSION_TABLE_COUNT,u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("no missionFinished:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	q.finalize();
	return true;
}

//填充用户信息
bool cProcUserShopBuy::setPBUser(PBUser* u_PBUser,const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select * from m_gameUserInfo where userID = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("m_gameUserInfo not find userId:%d\n",u_userId);
		q.finalize();
		return false;
	}
	else
	{
		u_PBUser->set_userid(q.getIntFileds("userID"));
		u_PBUser->set_username(q.getStringFileds("userName"));
		u_PBUser->set_userlevel(q.getIntFileds("userLevel"));
		u_PBUser->set_userexp(q.getIntFileds("userExp"));

		int u_energy = q.getIntFileds("energy");

		int u_lastEnergyTime = q.getIntFileds("lastEnergyTime");
		int u_userLevel = q.getIntFileds("userLevel");

		int u_nowTimeStamp = 0;
		u_nowTimeStamp = GetTimeStamp();
		int u_NextEnergyTime = getNextEnergyTime(u_nowTimeStamp,u_lastEnergyTime,u_energy,u_userLevel);

		unsigned int u_Energy = u_energy;
		updateUserEnergyAndTime(u_Energy,u_lastEnergyTime,u_userId);
		u_PBUser->set_energy(u_energy);
		u_PBUser->set_nextenergytime(u_NextEnergyTime);

		//u_PBUser->set_buyenergytime(q.getIntFileds("buyEnergyTime"));
		newAddSetSomeUserInfo(u_PBUser,u_userId);
		u_PBUser->set_diamondnumber(q.getIntFileds("diamondNumber"));
		u_PBUser->set_friendvalue(q.getIntFileds("friendValue"));
		u_PBUser->set_goldnumber(q.getIntFileds("goldNumber"));
		u_PBUser->set_leadervalue(q.getIntFileds("leaderValue"));
		if (hasMissionFinished(u_userId))
		{
			u_PBUser->set_hasmissionfinished(true);
		}
		else
		{
			u_PBUser->set_hasmissionfinished(false);
		}
		u_PBUser->set_bagmaxcount(q.getIntFileds("maxOwnCardCount"));
		u_PBUser->set_energyagentiacount(q.getIntFileds("energyCount"));
		u_PBUser->set_captainindex(q.getIntFileds("playerProfession"));
		q.finalize();
	}
	return true;
}

int cProcUserShopBuy::setPBIAPRecv(PBIAPRecv& u_PBIAPRecv,const string& u_strToken,const PBIAPSend& u_PBIAPSend)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBShopBuySendReceive getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	u_userId = getUserIdByUserMacAddress(u_PBIAPSend.macaddress());
	if (u_userId <= 0)
	{
		return -1;
	}
	if (u_PBIAPSend.usersourcetype() == P_91)
	{
		u_result = proc91PBIAPRecv(u_PBIAPRecv,u_userId,u_PBIAPSend);
	}
	else if (u_PBIAPSend.usersourcetype() == P_IOS)
	{
		u_result = procIOSPBIAPRecv(u_PBIAPRecv,u_userId,u_PBIAPSend);
	}
	else
	{
		u_result = -1;
		return u_result;
	}

	if (u_result == 0)
	{
		PBUser* u_PBUser;
		u_PBUser = u_PBIAPRecv.mutable_user();
		setPBUser(u_PBUser,u_userId);
	}
	u_PBIAPRecv.set_result(u_result);
	if (!u_PBIAPSend.orderid().empty())
	{
		u_PBIAPRecv.set_orderid(u_PBIAPSend.orderid());
	}
	g_cMyGlog.debugLog("u_result:%d\n",u_result);
	return u_result;

}

