#ifndef STRUCTDEF_H
#define STRUCTDEF_H
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <map>
using namespace std;

//�ͻ��˷��͹��������ݰ�ͷ��ʽ����=dataPacket+protobuf��
typedef struct _dataPacket
{
	int nType;//��������
	int protobufLength;
	char tokenValue[33];//32λtokenֵ
	_dataPacket()
	{
		nType = -1;
		protobufLength = 0;
		memset(tokenValue,0,sizeof(tokenValue));
	}
}dataPacket;

struct CardBaseStatusGrow//����BP�ɳ���
{
	int VTLGrow;//�����ɳ���
	int STRGrow;//�����ɳ���
	int TGHGrow;//ǿ�ȳɳ���
	int QUIGrow;//�ٶȳɳ���
	int MGCGrow;//ħ���ɳ���
	CardBaseStatusGrow() { VTLGrow = 0; STRGrow = 0; TGHGrow = 0; QUIGrow = 0; MGCGrow = 0; }
};

struct CardBaseStatus//���ƻ�������BP
{
	float VTL;//����
	float STR;//����
	float TGH;//ǿ��
	float QUI;//�ٶ�
	float MGC;//ħ��
	CardBaseStatus() { VTL = 0; STR = 0; TGH = 0; QUI = 0; MGC = 0; }
};

struct CardAttribute//��������
{
	int LP;   //����
	int FP;   //ħ��
	int ATK;  //����
	int DEF;  //����
	int AGL;  //����
	int MND;  //����
	int VCY;  //�ظ�
	CardAttribute() { LP = 0; FP = 0; ATK = 0; DEF = 0; AGL = 0; MND = 0; VCY = 0; }
};

typedef struct _CardInfoAuth
{
	string u_cardId;//��ƬId
	string u_cardSkillA;//��Ƭ��������
	string u_cardSkillP;//��Ƭ��������
	int VTLStar;//�����Ǽ�
	int STRStar;//�����Ǽ�
	int TGHStar;//ǿ���Ǽ�
	int QUIStar;//�ٶ��Ǽ�
	int MGCStar;//ħ���Ǽ�
}CardInfoAuth;

typedef map<string,CardInfoAuth> cardInfoAuthMap;

typedef map<int,int> mapRand;


typedef struct _battleLoseInfo
{
	string u_monsterId;//����Id
	string u_loseCardId;//����cardId
	int u_loseCardProbability;//����cardId�ĸ���
	int u_monsterCardLevel;//����ĵȼ�
	int u_equipTypeId;//�����װ������
	string u_equipId;//�����װ��Id
	int u_equipProbability;//װ���ĵ���
	int u_goldProbability;//��ҵ���
	int u_goldNumber;//��ҵ�����
	_battleLoseInfo()
	{
		u_monsterId = "";
		u_loseCardId = "";
		u_loseCardProbability = 0;
		u_monsterCardLevel = 1;
		u_equipTypeId = 1;
		u_equipId = "";
		u_equipProbability = 0;
		u_goldProbability = 0;
		u_goldNumber = 0;
	}
}battleLoseInfo;

#endif