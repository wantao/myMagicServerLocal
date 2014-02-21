#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <stdlib.h>

using namespace std;

//���ݿ�����
extern string g_strDbHost;
//���ݿ��ӻ�
extern string g_strDbReadHost;
//���ݿ�˿�
extern int g_nDbPort;
//���ݿ�����
extern string g_strDatabase;
//���ݿ��û���
extern string g_strUser;
//���ݿ���������
extern string g_strPw;

extern char buf[];

extern char randBuf[];

double random(double start, double end);

//����ָ�����ȵ�������
string generateInvitationCode(int u_length);
//�������token
string generateToken();
int getPreMaxMissionId();
int getDbServerIp();
int getClassByFightCapacity(const int& u_fightCapacity);
bool updateUserFightCapacity(const unsigned int& u_userId,const int& u_userFigthCapacity,\
							 const int& u_userCapitorCardCapacity);
void getUserFightCapacity(const unsigned int& u_userId,int& u_userFigthCapacity,\
						  int& u_userCapitorCardCapacity);
#endif