#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <stdlib.h>
#include "../include/MagicCard.pb.h"
#include "../include/structDef.h"

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

double random(double start, double end);


//根据时间字符串还原成秒数
unsigned long MakeTime(const string& strTime);
//取时间戳
unsigned int GetTimeStamp();
//根据时间戳还原时间
string GetTime(long lTime);
string getStringTime(long lTime,int nType);

bool getUserIdbyToken(const string& u_strToken,unsigned int& u_userId);

//获取网卡ip地址
string GetIpAddress(char* eth,int nIndex);
int getDbServerIp();

unsigned int getUserIdByUserMacAddress(const string& u_userMacAddress);
/************************************************************************/
/* get payServerPort                                                                     */
/************************************************************************/
int getPayServerPortByPayServerUrl(const string& u_payServerUrl);

//获取用户已充值的魔石数量
int getUserRechargedDiamond(const unsigned int& u_userId);
//更新用户已充值的魔石数量
void updateUserReChargedDiamondNum(const unsigned int& u_userId,const int& u_addDiamonds);
//获取用户已扫荡和已购买体力药水次数
void getUserWipeOutAndBuyEnergyCount(const unsigned int& u_userId,int& u_wipeOutCount,int& u_buyEnergyCount);
//为用户信息新增用户已充值的魔石数量，当日可扫荡次数，当日已购买体力药水次数
void newAddSetSomeUserInfo(PBUser* &u_PBUser,const unsigned int& u_userId);
#endif