#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <stdlib.h>
#include "../include/structDef.h"


using namespace std;

//���ݿ�����
extern string g_strDbHost;
//�����ݿ�����
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

//��Դ�ļ�map
extern resourceDataMap g_resourceDataMap;

double random(double start, double end);

//�������token
string generateToken();

//����ʱ���ַ�����ԭ������
unsigned long MakeTime(const string& strTime);
//ȡʱ���
unsigned int GetTimeStamp();
//����ʱ�����ԭʱ��
string GetTime(long lTime);
string getStringTime(long lTime,int nType);
int getOneRand();

//����һ��1000-10000�������
int monsterGetOneRand();
//����һ��1-5���������
mapRand generateIntFiled();

//��ȡ�漴�ɳ�ֵ
int getGrowValue(int u_min,int u_max);

bool getUserIdbyToken(const string& u_strToken,unsigned int& u_userId);

//��óɳ���
int getGrow(const int& u_value);
//������ɳ��� ע��ʱΪ�ӳ�����ָ�����ɳ���
int getMaxGrow(const int& u_value);
//����ʱ��ȷ�����������ڼ���ʱ���ʽ2013 08 19
int getWeekDayByTime(const string& u_strTime);
//��ȡ�ļ�����
std::string GetFileDataByPath(std::string strResourcePath);
int getDbServerIp();
#endif