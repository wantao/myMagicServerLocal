#ifndef PROCUSERCARDEVOLUTION_H
#define PROCUSERCARDEVOLUTION_H

#include <iostream>
#include "../include/MagicCard.pb.h"
#include "../include/magicServerDb.h"
using namespace std;

typedef struct _ComposeTable
{
	//合成所需的金钱
	int needMoney;
	//材料1的的Id
	string SynthesizeId1;
	//数量
	int SynthesizeNum1;
	//材料2的的Id
	string SynthesizeId2;
	//数量
	int SynthesizeNum2;
	//材料3的的Id
	string SynthesizeId3;
	//数量
	int SynthesizeNum3;
	//材料4的的Id
	string SynthesizeId4;
	//数量
	int SynthesizeNum4;
	//材料5的的Id
	string SynthesizeId5;
	//数量
	int SynthesizeNum5;

	_ComposeTable()
	{
		needMoney = 0;
		SynthesizeId1 = "";
		SynthesizeNum1 = 0;
		SynthesizeId2 = "";
		SynthesizeNum2 = 0;
		SynthesizeId3 = "";
		SynthesizeNum3 = 0;
		SynthesizeId4 = "";
		SynthesizeNum4 = 0;
		SynthesizeId5 = "";
		SynthesizeNum5 = 0;
	}
}ComposeTable;

class cProcUserCardEvolution
{
public:
	cProcUserCardEvolution();
	~cProcUserCardEvolution();

	//获取配方中某种配料的数量
	int getComposeCount(const string& u_cardId,const unsigned int& u_userId);
	//填充合成配方表
	void setComposeTable(ComposeTable& u_ComposeTable,const string& u_ComposeId,const int& u_Num,int u_Type);
	//得到合成卡牌的配方表
	bool getEvolutionCompose(string& u_EvolutionId,const unsigned int& u_userId,ComposeTable& u_ComposeTable);
	//插入合成成功得到的卡牌
	bool insertEvolutionCard(const unsigned int& u_userId,MysqlQuery& q,unsigned int& u_itemId,\
		const int& u_cardPosition,const int& u_cardLevel);
	//获取合成卡牌的初始信息并插入用户卡牌表
	bool getInitEvolutionCardAndInsert(const unsigned int& u_userId,const string& u_strEvolutionId,unsigned int& u_itemId,\
		const int& u_cardPosition,const int& u_cardLevel);
	//获取将要合成得到的卡牌ID
	bool getEvolutionId(const string& u_cardId,string& u_EvolutionId);
	//得到卡牌列表，方便删除
	string getItemIdList(const unsigned int& u_userId,const PBCardEvolutionSend& u_PBCardEvolutionSend);
	//删除被合成的卡牌
	void deleteCard(const unsigned int& u_userId,const PBCardEvolutionSend& u_PBCardEvolutionSend);
	//要合成的卡牌是否存在
	bool isCardExist(const unsigned int& u_itemId,const unsigned int& u_userId,string& u_cardId,int& u_cardPosition);
	//删除装备
	void deleteEquip(const unsigned int& u_userId,const ComposeTable& u_ComposeTable);
	//合成所需的金币是否充足
	bool isEvolutionGoldEnough(const unsigned int& u_userId,const int& u_evolutionNeedGold,int& u_ownGold);
	//插入合成成功的装备
	bool insertEvolutionEquip(const unsigned int& u_userId,const string& u_EvolutionId,const int& u_Num);
	//增加合成成功的装备
	bool AddEvolutionEquip(const unsigned int& u_userId,const string& u_EvolutionId);
	//填充合成信息
	int setPBCardEvolutionRecieve(PBCardEvolutionRecieve& u_PBCardEvolutionRecieve,const string& u_strToken,PBCardEvolutionSend& u_PBCardEvolutionSend);
	//test
	void printComposeTable(const ComposeTable& u_ComposeTable);
	void printPBCardEvolutionSend(const PBCardEvolutionSend& u_PBCardEvolutionSend);
	//test
protected:
private:
};

extern cProcUserCardEvolution g_cProcUserCardEvolution;
#endif