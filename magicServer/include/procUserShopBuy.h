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
	//�����û�������Ŀ
	void updateUserBagCountAndDiamond(const unsigned int& u_userId,const int& u_bagCount,const int& u_diamondNum);
	//��ȡ�û���������ʯ��
	bool getUserBagCountAndDiamond(const unsigned int& u_userId,int& u_bagCount,int& u_diamondNum);
	//������������
	int setAddBag(const unsigned int& u_userId);
	//��ȡ����ҩˮ����
	bool getUserEnergyPotionCountAndDiamond(const unsigned int& u_userId,int& u_diamondNumber,\
		int& u_buyEnergyTime,int& u_lastEnergyTime,int& u_energyPotionCount);
	//��������ҩˮ����
	void updateEnergyPotionCountAndDiamond(const unsigned int& u_userId,int& u_diamondNumber,\
		int& u_buyEnergyTime,int& u_lastEnergyTime,int& u_energyPotionCount);
	//����ƣ��������
	int setBuyEnergy(const unsigned int& u_userId);
	//��ȡ�û���Һ�ħʯ
	bool getUserGoldAndDiamondNum(const unsigned int& u_userId,int& u_goldNum,int& u_diamondNum);
	//�����û���Һ�ħʯ
	bool updateUserGoldDiamond(const unsigned int& u_userId,const int& u_goldNum,bool u_updateGoldFlag,\
		int& u_diamondNum,bool u_updateDiamondFlag);
	//�����Ҵ���
	int setBuyGold(const unsigned int& u_userId);
	//��ȡ�û��ۼƳ�ֵ����ħʯ��Ŀ
	bool getUserMoneyPay(const unsigned int& u_userId,int& u_userDiamondNum,int& u_moneyTotal);
	//�����û��ۼƳ�ֵ����ħʯ��Ŀ
	void updateUserMoneyPayAndDiamondNum(const unsigned int& u_userId,const int& u_moneyTotal,const int& u_userDiamond);
	//��ȡ��ֵ��������
	string getPayMissionCondition(const int& u_userMoneyTotal,const int& u_moneyAdd);
	//����ħʯ����
	int setBuyMagicSton(const unsigned int& u_userId,const int& u_buyType);
	//��ȡ�û��������
	bool getUserBattleReviveCount(const unsigned int& u_userId,int& u_reviveCount);
	//�����û��������
	void updateUserBattleReviveCount(const unsigned int& u_userId,const int& u_reviveCount);
	//����������
	int setReviveReq(const unsigned int& u_userId,const int& u_buyType);
	//�����̵깺������
	int procShopBuy(const unsigned int& u_userId,const int& u_buyType);
	int setPBShopBuySendReceive(PBShopBuySendReceive& u_PBShopBuySendReceive,const string& u_strToken,\
		const PBShopBuySend& u_PBShopBuySend);

	//������������ʯ
	void updateEnergyAndDiamond(const unsigned int& u_userId,const int& u_diamondNumber,\
		const int& u_lastEnergyTime,const int& u_energy);
	//��ȡ��������ʯ
	bool getUserEnergyAndDiamond(const unsigned int& u_userId,int& u_diamondNumber,\
		int& u_userLevel,int& u_lastEnergyTime,int& u_energy);
	//��������ħʯ
	int getReviveReqNeedDiamond(const int& reviveCount);

	void update91ShopBuyOrderRocord(const string& u_cooOrderSerial);
	//����91�̵깺������
	int proc91ShopBuy(const unsigned int& u_userId,const int& u_buyType);
	int getGoodsIdByGoodsOrder(const PBIAPSend& u_PBIAPSend);
	int setPBIAPRecv(PBIAPRecv& u_PBIAPRecv,const string& u_strToken,const PBIAPSend& u_PBIAPSend);
	
	void procNotProccessedOrder();
	int proc91PBIAPRecv(PBIAPRecv& u_PBIAPRecv,const unsigned int& u_userId,const PBIAPSend& u_PBIAPSend);
	int procIOSPBIAPRecv(PBIAPRecv& u_PBIAPRecv,const unsigned int& u_userId,const PBIAPSend& u_PBIAPSend);
	//��ȡ�����ָ�����ʱ
	int getNextEnergyTime(const int& u_nowStamp,int& u_lastEnergyTimeStamp,\
		int& u_energy,const int& u_userLevel);
	//�������������»�ȡ������ʱ���
	bool updateUserEnergyAndTime(const unsigned int& u_energy,const unsigned int& u_lastEnergyTime,\
		const unsigned int& u_userId);
	//�ж��Ƿ����������
	bool hasMissionFinished(const unsigned int& u_userId);
	//��ȡ��ֵ���������б�
	string getPayMissionConditionList(const unsigned int& u_userId,const string& u_payMissionCondition);
	//�����������͸����������״̬
	void updateMissionStatus(const unsigned int& u_userId,const int& u_missionType,\
		const string& u_missionCondition,bool isFinished);
	//�������������ж������Ƿ����
	bool isMissionExist(const unsigned int& u_userId,const int& u_missionType,\
		const string& u_missionCondition,int& isFinished);
	//����û���Ϣ
	bool setPBUser(PBUser* u_PBUser,const unsigned int& u_userId);

	//int curlWriteCallback(char *data, size_t size, size_t nmemb, string *writerData);
protected:
private:
	cMyJsonParser m_cMyJsonParser;
};

extern cProcUserShopBuy g_cProcUserShopBuy;
#endif