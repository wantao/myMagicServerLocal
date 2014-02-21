#ifndef STRUCTDEF_H
#define STRUCTDEF_H
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <map>
using namespace std;

//客户端发送过来的数据包头格式（包=dataPacket+protobuf）
typedef struct _dataPacket
{
	int nType;//请求类型
	int protobufLength;
	char tokenValue[33];//32位token值
	_dataPacket()
	{
		nType = -1;
		protobufLength = 0;
		memset(tokenValue,0,sizeof(tokenValue));
	}
}dataPacket;

struct CardBaseStatusGrow//卡牌BP成长档
{
	int VTLGrow;//体力成长档
	int STRGrow;//力量成长档
	int TGHGrow;//强度成长档
	int QUIGrow;//速度成长档
	int MGCGrow;//魔法成长档
	CardBaseStatusGrow() { VTLGrow = 0; STRGrow = 0; TGHGrow = 0; QUIGrow = 0; MGCGrow = 0; }
};

struct CardBaseStatus//卡牌基础属性BP
{
	float VTL;//体力
	float STR;//力量
	float TGH;//强度
	float QUI;//速度
	float MGC;//魔法
	CardBaseStatus() { VTL = 0; STR = 0; TGH = 0; QUI = 0; MGC = 0; }
};

struct CardAttribute//卡牌属性
{
	int LP;   //生命
	int FP;   //魔力
	int ATK;  //攻击
	int DEF;  //防御
	int AGL;  //敏捷
	int MND;  //精神
	int VCY;  //回复
	CardAttribute() { LP = 0; FP = 0; ATK = 0; DEF = 0; AGL = 0; MND = 0; VCY = 0; }
};

typedef struct _CardInfoAuth
{
	string u_cardId;//卡片Id
	string u_cardSkillA;//卡片主动技能
	string u_cardSkillP;//卡片被动技能
	int VTLStar;//体力星级
	int STRStar;//力量星级
	int TGHStar;//强度星级
	int QUIStar;//速度星级
	int MGCStar;//魔法星级
}CardInfoAuth;

typedef map<string,CardInfoAuth> cardInfoAuthMap;

typedef map<int,int> mapRand;


typedef struct _battleLoseInfo
{
	string u_monsterId;//怪物Id
	string u_loseCardId;//掉落cardId
	int u_loseCardProbability;//掉落cardId的概率
	int u_monsterCardLevel;//怪物的等级
	int u_equipTypeId;//掉落的装备类型
	string u_equipId;//掉落的装备Id
	int u_equipProbability;//装备的掉率
	int u_goldProbability;//金币掉率
	int u_goldNumber;//金币掉落数
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