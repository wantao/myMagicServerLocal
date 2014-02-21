#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <stdlib.h>
#include "../include/structDef.h"


using namespace std;

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

extern char buf[];

//资源文件map
extern resourceDataMap g_resourceDataMap;

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
//根据时间确定今天是星期几，时间格式2013 08 19
int getWeekDayByTime(const string& u_strTime);
//获取文件内荣
std::string GetFileDataByPath(std::string strResourcePath);
int getDbServerIp();
#endif