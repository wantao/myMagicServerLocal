#ifndef PROCUSERSHOPBUY_H
#define PROCUSERSHOPBUY_H

#include <iostream>
#include "../include/MagicCard.pb.h"
#include "../include/myJsonParser.h"
using namespace std;

//class cMyJsonParser;

class cProcUserShopBuy
{
public:
	cProcUserShopBuy();
	~cProcUserShopBuy();
	//更新用户背包数目
	void updateUserBagCountAndDiamond(const unsigned int& u_userId,const int& u_bagCount,const int& u_diamondNum);
	//获取用户背包和钻石数
	bool getUserBagCountAndDiamond(const unsigned int& u_userId,int& u_bagCount,int& u_diamondNum);
	//背包扩充设置
	int setAddBag(const unsigned int& u_userId);
	//获取体力药水计数
	bool getUserEnergyPotionCountAndDiamond(const unsigned int& u_userId,int& u_diamondNumber,\
		int& u_buyEnergyTime,int& u_lastEnergyTime,int& u_energyPotionCount);
	//更新体力药水计数
	void updateEnergyPotionCountAndDiamond(const unsigned int& u_userId,int& u_diamondNumber,\
		int& u_buyEnergyTime,int& u_lastEnergyTime,int& u_energyPotionCount);
	//购买疲劳请求处理
	int setBuyEnergy(const unsigned int& u_userId);
	//获取用户金币和魔石
	bool getUserGoldAndDiamondNum(const unsigned int& u_userId,int& u_goldNum,int& u_diamondNum);
	//更新用户金币和魔石
	bool updateUserGoldDiamond(const unsigned int& u_userId,const int& u_goldNum,bool u_updateGoldFlag,\
		int& u_diamondNum,bool u_updateDiamondFlag);
	//购买金币处理
	int setBuyGold(const unsigned int& u_userId);
	//获取用户累计充值金额和魔石数目
	bool getUserMoneyPay(const unsigned int& u_userId,int& u_userDiamondNum,int& u_moneyTotal);
	//更新用户累计充值金额和魔石数目
	void updateUserMoneyPayAndDiamondNum(const unsigned int& u_userId,const int& u_moneyTotal,const int& u_userDiamond);
	//获取充值任务条件
	string getPayMissionCondition(const int& u_userMoneyTotal,const int& u_moneyAdd);
	//购买魔石处理
	int setBuyMagicSton(const unsigned int& u_userId,const int& u_buyType);
	//获取用户复活次数
	bool getUserBattleReviveCount(const unsigned int& u_userId,int& u_reviveCount);
	//更新用户复活次数
	void updateUserBattleReviveCount(const unsigned int& u_userId,const int& u_reviveCount);
	//处理复活请求
	int setReviveReq(const unsigned int& u_userId,const int& u_buyType);
	//处理商店购买请求
	int procShopBuy(const unsigned int& u_userId,const int& u_buyType);
	int setPBShopBuySendReceive(PBShopBuySendReceive& u_PBShopBuySendReceive,const string& u_strToken,\
		const PBShopBuySend& u_PBShopBuySend);

	//更新体力和钻石
	void updateEnergyAndDiamond(const unsigned int& u_userId,const int& u_diamondNumber,\
		const int& u_lastEnergyTime,const int& u_energy);
	//获取体力和钻石
	bool getUserEnergyAndDiamond(const unsigned int& u_userId,int& u_diamondNumber,\
		int& u_userLevel,int& u_lastEnergyTime,int& u_energy);
	//复活所需魔石
	int getReviveReqNeedDiamond(const int& reviveCount);

	void update91ShopBuyOrderRocord(const string& u_cooOrderSerial);
	//处理91商店购买请求
	int proc91ShopBuy(const unsigned int& u_userId,const int& u_buyType);
	int getGoodsIdByGoodsOrder(const PBIAPSend& u_PBIAPSend);
	int setPBIAPRecv(PBIAPRecv& u_PBIAPRecv,const string& u_strToken,const PBIAPSend& u_PBIAPSend);
	
	void procNotProccessedOrder();
	int proc91PBIAPRecv(PBIAPRecv& u_PBIAPRecv,const unsigned int& u_userId,const PBIAPSend& u_PBIAPSend);
	int procIOSPBIAPRecv(PBIAPRecv& u_PBIAPRecv,const unsigned int& u_userId,const PBIAPSend& u_PBIAPSend);
	//获取体力恢复倒计时
	int getNextEnergyTime(const int& u_nowStamp,int& u_lastEnergyTimeStamp,\
		int& u_energy,const int& u_userLevel);
	//更新体力和最新获取体力的时间戳
	bool updateUserEnergyAndTime(const unsigned int& u_energy,const unsigned int& u_lastEnergyTime,\
		const unsigned int& u_userId);
	//判断是否有任务完成
	bool hasMissionFinished(const unsigned int& u_userId);
	//获取充值任务条件列表
	string getPayMissionConditionList(const unsigned int& u_userId,const string& u_payMissionCondition);
	//根据任务类型更新任务完成状态
	void updateMissionStatus(const unsigned int& u_userId,const int& u_missionType,\
		const string& u_missionCondition,bool isFinished);
	//根据任务类型判断任务是否存在
	bool isMissionExist(const unsigned int& u_userId,const int& u_missionType,\
		const string& u_missionCondition,int& isFinished);
	//填充用户信息
	bool setPBUser(PBUser* u_PBUser,const unsigned int& u_userId);

	//int curlWriteCallback(char *data, size_t size, size_t nmemb, string *writerData);
protected:
private:
	cMyJsonParser m_cMyJsonParser;
};

extern cProcUserShopBuy g_cProcUserShopBuy;
#endif