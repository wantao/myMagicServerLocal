#ifndef PROCUSERCARDEVOLUTION_H
#define PROCUSERCARDEVOLUTION_H

#include <iostream>
#include "../include/MagicCard.pb.h"
#include "../include/magicServerDb.h"
using namespace std;

typedef struct _ComposeTable
{
	//�ϳ�����Ľ�Ǯ
	int needMoney;
	//����1�ĵ�Id
	string SynthesizeId1;
	//����
	int SynthesizeNum1;
	//����2�ĵ�Id
	string SynthesizeId2;
	//����
	int SynthesizeNum2;
	//����3�ĵ�Id
	string SynthesizeId3;
	//����
	int SynthesizeNum3;
	//����4�ĵ�Id
	string SynthesizeId4;
	//����
	int SynthesizeNum4;
	//����5�ĵ�Id
	string SynthesizeId5;
	//����
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

	//��ȡ�䷽��ĳ�����ϵ�����
	int getComposeCount(const string& u_cardId,const unsigned int& u_userId);
	//���ϳ��䷽��
	void setComposeTable(ComposeTable& u_ComposeTable,const string& u_ComposeId,const int& u_Num,int u_Type);
	//�õ��ϳɿ��Ƶ��䷽��
	bool getEvolutionCompose(string& u_EvolutionId,const unsigned int& u_userId,ComposeTable& u_ComposeTable);
	//����ϳɳɹ��õ��Ŀ���
	bool insertEvolutionCard(const unsigned int& u_userId,MysqlQuery& q,unsigned int& u_itemId,\
		const int& u_cardPosition,const int& u_cardLevel);
	//��ȡ�ϳɿ��Ƶĳ�ʼ��Ϣ�������û����Ʊ�
	bool getInitEvolutionCardAndInsert(const unsigned int& u_userId,const string& u_strEvolutionId,unsigned int& u_itemId,\
		const int& u_cardPosition,const int& u_cardLevel);
	//��ȡ��Ҫ�ϳɵõ��Ŀ���ID
	bool getEvolutionId(const string& u_cardId,string& u_EvolutionId);
	//�õ������б�����ɾ��
	string getItemIdList(const unsigned int& u_userId,const PBCardEvolutionSend& u_PBCardEvolutionSend);
	//ɾ�����ϳɵĿ���
	void deleteCard(const unsigned int& u_userId,const PBCardEvolutionSend& u_PBCardEvolutionSend);
	//Ҫ�ϳɵĿ����Ƿ����
	bool isCardExist(const unsigned int& u_itemId,const unsigned int& u_userId,string& u_cardId,int& u_cardPosition);
	//ɾ��װ��
	void deleteEquip(const unsigned int& u_userId,const ComposeTable& u_ComposeTable);
	//�ϳ�����Ľ���Ƿ����
	bool isEvolutionGoldEnough(const unsigned int& u_userId,const int& u_evolutionNeedGold,int& u_ownGold);
	//����ϳɳɹ���װ��
	bool insertEvolutionEquip(const unsigned int& u_userId,const string& u_EvolutionId,const int& u_Num);
	//���Ӻϳɳɹ���װ��
	bool AddEvolutionEquip(const unsigned int& u_userId,const string& u_EvolutionId);
	//���ϳ���Ϣ
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