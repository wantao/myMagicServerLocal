#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <stdlib.h>
#include "../include/structDef.h"
#include "../include/MagicCard.pb.h"

using namespace std;

//本机IP
extern string g_strLocalHost;
//数据库主机
extern string g_strDbHost;
//从数据库主机
extern string g_strDbReadHost;
//数据库端口
extern int g_nDbPort;
//数据库名称
extern string g_strDatabase;
//数据库用户名
extern string g_strUser;
//数据库连接密码
extern string g_strPw;

//卡牌信息
extern cardInfoAuthMap g_cardInfoAuthMap;

//extern char buf[];

double random(double start, double end);

//产生随机token
string generateToken();

//根据时间字符串还原成秒数
unsigned long MakeTime(const string& strTime);
//取时间戳
unsigned int GetTimeStamp();
//根据时间戳还原时间
string GetTime(long lTime);
string getStringTime(long lTime,int nType);
int getOneRand();

//产生一个1000-10000的随机数
int monsterGetOneRand();
//产生一个1-5的随机整数
mapRand generateIntFiled();

//获取随即成长值
int getGrowValue(int u_min,int u_max);

bool getUserIdbyToken(const string& u_strToken,unsigned int& u_userId);

//获得成长档
int getGrow(const int& u_value);
//获得最大成长档 注册时为队长卡牌指定最大成长档
int getMaxGrow(const int& u_value);
//根据卡牌星级获取卡牌领导力
int getCardLeaderValueByStarLevel(const int& u_starLevel);

//获取BP
float getCardBPByGrowAndRank(int _growNum, int rank);
//通过BP成长和卡片等级得到卡片属性
CardAttribute getCardAttributeByCardBaseStatusGrowAndRank(CardBaseStatusGrow _baseStatusGrow, int rank);
//根据卡牌属性获取战斗力
int getFightCapacityByCardAttribute(const CardAttribute& u_CardAttribute);
//根据卡牌属性获取卡牌战斗力
int getFightCapacityByCardAttribute(const CardAttribute& u_CardAttribute);
//根据技能编号和技能等级获取卡牌战斗力
int getFightCapacityBySkillCodeAndSkillLevel(const string& u_skillCode,const int& u_skillLevel);
void getSkillCodeByCardId(const string& u_strCardId,string& u_passiveSkillCode,string& u_activeSkillCode);
int getUserFightCapacity(const int& u_userId,int& u_userCapitorFightCapacity);

void insertUserFightCapacity(const unsigned int& u_userId,const int& u_userFigthCapacity,\
							 const int& u_userCapitorCardCapacity);
void updateUserFightCapacity(const unsigned int& u_userId,const int& u_userFigthCapacity,\
							 const int& u_userCapitorCardCapacity);
void getUserFightCapacity(const unsigned int& u_userId,int& u_userFigthCapacity,\
						  int& u_userCapitorCardCapacity);
//获取网卡ip地址
string GetIpAddress(char* eth,int nIndex);
int getDbServerIp();
int getClassByFightCapacity(const int& u_fightCapacity);
int getRandData(int u_randMix,int u_randMax);
int getNoticeTimePoint();
//获取玩家名字
string getUserNameBasedUserId(const unsigned int& u_userId);
//插入高星级卡牌通知
void insertHighStarCardNotice(const unsigned int& u_userId,const string& u_cardId,const int& u_cardStar);

//插入战斗力突破通知
int getFightCapacityNoticeCount(const unsigned int& u_userId,const int& n_fightCapacityType);
void insertFightCapacityNoticeCount(const unsigned int& u_userId,const int& u_fightCapacityType,const int& u_noticeCount);
void insertFightCapacityBreakThroughNotice(const unsigned int& u_userId,const int& u_userFightCapacity);
void procFightCapacityBreakThrough(const unsigned int& u_userId,const int& u_userFightCapacity);

//插入进化成功通知
void procEvolutionSuccessNotice(const unsigned int& u_userId,const string& u_strCardId,const string& u_EvolutionId);

//根据u_battleId，判断该关是否为大关的最后一小关，并带出大关编号和大关名字
bool isBigChapterLastLevel(const string& u_battleId,string& u_bigChapterCode,string& u_bigChapterName);
//获取通关该大关的名次
int getRankByBigChapterCode(const string& u_bigChapterCode);
//获取通过大关卡的公告次数
int getPassBigLevelNoticeCount(const string& u_bigLevelCode,const unsigned int& u_userId);
//获取通过大关卡的公告次数
int getPassBigLevelNoticeCount(const string& u_bigLevelCode,const unsigned int& u_userId);
void procInsertPassLevelNotice(const string& u_battleId,const unsigned int& u_userId);


///用户碎片信息增删查///begin
int getUserFragmentCount(const unsigned int& u_userId,const int& u_type,const string& u_strId);
void insertUserFragment(const unsigned int& u_userId,const int& u_type,\
						const string& u_strId,const int& u_count);
void updateUserFragment(const unsigned int& u_userId,const int& u_type,\
						const string& u_strId,const int& u_count);
void addUserFragmentInfo(const unsigned int& u_userId,const int& u_type,\
						  const string& u_strId,const int& u_addCount);
void cleanUserFragmentInfo(const unsigned int& u_userId,const int& u_type,const string& u_strId);
void deleteUserFragmentInfo(const unsigned int& u_userId,const int& u_type,\
							const string& u_strId,const int& u_deleteCount);
///用户碎片信息增删查///end


unsigned int getUserIdByUserMacAddress(const string& u_userMacAddress);

//获取卡牌等级
int getCardLevelById(const unsigned int& u_userId,const int& u_cardItemId);

int getCardMaxLevelByStrCardId(const string& u_strCardId);

//获取用户已扫荡和已购买体力药水次数
void getUserWipeOutAndBuyEnergyCount(const unsigned int& u_userId,int& u_wipeOutCount,int& u_buyEnergyCount);
//获取充值用户最大扫荡和购买体力药水次数
void getUserMaxWipeOutAndBuyEnergyCount(const int& u_rechargedDiamond,int& u_maxWipeOutCount,int& u_maxBuyEnergyCount);
//获取用户已充值的魔石数量
int getUserRechargedDiamond(const unsigned int& u_userId);
//更新用户已购买体力药水次数
void updateUserBuyEnergyCount(const unsigned int& u_userId,const int& u_buyEnergyCount);
//更新用户已扫荡次数
void updateUserWipeOutCount(const unsigned int& u_userId,const int& u_wipeOutCount);
//判断用户是否已达当日最大体力购买次数
int isUserToMaxBuyEnergyCount(const unsigned int& u_userId);
//判断用户是否已达当日最大最大扫荡次数
int isUserToMaxWipeOutCount(const unsigned int& u_userId);
//为用户信息新增用户已充值的魔石数量，当日可扫荡次数，当日已购买体力药水次数
void newAddSetSomeUserInfo(PBUser* &u_PBUser,const unsigned int& u_userId);

#endif