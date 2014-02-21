#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <stdlib.h>

using namespace std;

//数据库主机
extern string g_strDbHost;
//数据库库从机
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

extern char randBuf[];

double random(double start, double end);

//产生指定长度的邀请码
string generateInvitationCode(int u_length);
//产生随机token
string generateToken();
int getPreMaxMissionId();
int getDbServerIp();
int getClassByFightCapacity(const int& u_fightCapacity);
bool updateUserFightCapacity(const unsigned int& u_userId,const int& u_userFigthCapacity,\
							 const int& u_userCapitorCardCapacity);
void getUserFightCapacity(const unsigned int& u_userId,int& u_userFigthCapacity,\
						  int& u_userCapitorCardCapacity);
#endif