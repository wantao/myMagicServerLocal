#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <stdlib.h>
#include "../include/MagicCard.pb.h"
#include "../include/structDef.h"

using namespace std;

//����IP
extern string g_strLocalHost;
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

//������Ϣ
extern cardInfoAuthMap g_cardInfoAuthMap;

double random(double start, double end);


//����ʱ���ַ�����ԭ������
unsigned long MakeTime(const string& strTime);
//ȡʱ���
unsigned int GetTimeStamp();
//����ʱ�����ԭʱ��
string GetTime(long lTime);
string getStringTime(long lTime,int nType);

bool getUserIdbyToken(const string& u_strToken,unsigned int& u_userId);

//��ȡ����ip��ַ
string GetIpAddress(char* eth,int nIndex);
int getDbServerIp();

unsigned int getUserIdByUserMacAddress(const string& u_userMacAddress);
/************************************************************************/
/* get payServerPort                                                                     */
/************************************************************************/
int getPayServerPortByPayServerUrl(const string& u_payServerUrl);

//��ȡ�û��ѳ�ֵ��ħʯ����
int getUserRechargedDiamond(const unsigned int& u_userId);
//�����û��ѳ�ֵ��ħʯ����
void updateUserReChargedDiamondNum(const unsigned int& u_userId,const int& u_addDiamonds);
//��ȡ�û���ɨ�����ѹ�������ҩˮ����
void getUserWipeOutAndBuyEnergyCount(const unsigned int& u_userId,int& u_wipeOutCount,int& u_buyEnergyCount);
//Ϊ�û���Ϣ�����û��ѳ�ֵ��ħʯ���������տ�ɨ�������������ѹ�������ҩˮ����
void newAddSetSomeUserInfo(PBUser* &u_PBUser,const unsigned int& u_userId);
#endif