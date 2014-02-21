#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <stdlib.h>
#include "../include/structDef.h"
#include "../include/MagicCard.pb.h"

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

//extern char buf[];

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
//���ݿ����Ǽ���ȡ�����쵼��
int getCardLeaderValueByStarLevel(const int& u_starLevel);

//��ȡBP
float getCardBPByGrowAndRank(int _growNum, int rank);
//ͨ��BP�ɳ��Ϳ�Ƭ�ȼ��õ���Ƭ����
CardAttribute getCardAttributeByCardBaseStatusGrowAndRank(CardBaseStatusGrow _baseStatusGrow, int rank);
//���ݿ������Ի�ȡս����
int getFightCapacityByCardAttribute(const CardAttribute& u_CardAttribute);
//���ݿ������Ի�ȡ����ս����
int getFightCapacityByCardAttribute(const CardAttribute& u_CardAttribute);
//���ݼ��ܱ�źͼ��ܵȼ���ȡ����ս����
int getFightCapacityBySkillCodeAndSkillLevel(const string& u_skillCode,const int& u_skillLevel);
void getSkillCodeByCardId(const string& u_strCardId,string& u_passiveSkillCode,string& u_activeSkillCode);
int getUserFightCapacity(const int& u_userId,int& u_userCapitorFightCapacity);

void insertUserFightCapacity(const unsigned int& u_userId,const int& u_userFigthCapacity,\
							 const int& u_userCapitorCardCapacity);
void updateUserFightCapacity(const unsigned int& u_userId,const int& u_userFigthCapacity,\
							 const int& u_userCapitorCardCapacity);
void getUserFightCapacity(const unsigned int& u_userId,int& u_userFigthCapacity,\
						  int& u_userCapitorCardCapacity);
//��ȡ����ip��ַ
string GetIpAddress(char* eth,int nIndex);
int getDbServerIp();
int getClassByFightCapacity(const int& u_fightCapacity);
int getRandData(int u_randMix,int u_randMax);
int getNoticeTimePoint();
//��ȡ�������
string getUserNameBasedUserId(const unsigned int& u_userId);
//������Ǽ�����֪ͨ
void insertHighStarCardNotice(const unsigned int& u_userId,const string& u_cardId,const int& u_cardStar);

//����ս����ͻ��֪ͨ
int getFightCapacityNoticeCount(const unsigned int& u_userId,const int& n_fightCapacityType);
void insertFightCapacityNoticeCount(const unsigned int& u_userId,const int& u_fightCapacityType,const int& u_noticeCount);
void insertFightCapacityBreakThroughNotice(const unsigned int& u_userId,const int& u_userFightCapacity);
void procFightCapacityBreakThrough(const unsigned int& u_userId,const int& u_userFightCapacity);

//��������ɹ�֪ͨ
void procEvolutionSuccessNotice(const unsigned int& u_userId,const string& u_strCardId,const string& u_EvolutionId);

//����u_battleId���жϸù��Ƿ�Ϊ��ص����һС�أ���������ر�źʹ������
bool isBigChapterLastLevel(const string& u_battleId,string& u_bigChapterCode,string& u_bigChapterName);
//��ȡͨ�ظô�ص�����
int getRankByBigChapterCode(const string& u_bigChapterCode);
//��ȡͨ����ؿ��Ĺ������
int getPassBigLevelNoticeCount(const string& u_bigLevelCode,const unsigned int& u_userId);
//��ȡͨ����ؿ��Ĺ������
int getPassBigLevelNoticeCount(const string& u_bigLevelCode,const unsigned int& u_userId);
void procInsertPassLevelNotice(const string& u_battleId,const unsigned int& u_userId);


///�û���Ƭ��Ϣ��ɾ��///begin
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
///�û���Ƭ��Ϣ��ɾ��///end


unsigned int getUserIdByUserMacAddress(const string& u_userMacAddress);

//��ȡ���Ƶȼ�
int getCardLevelById(const unsigned int& u_userId,const int& u_cardItemId);

int getCardMaxLevelByStrCardId(const string& u_strCardId);

//��ȡ�û���ɨ�����ѹ�������ҩˮ����
void getUserWipeOutAndBuyEnergyCount(const unsigned int& u_userId,int& u_wipeOutCount,int& u_buyEnergyCount);
//��ȡ��ֵ�û����ɨ���͹�������ҩˮ����
void getUserMaxWipeOutAndBuyEnergyCount(const int& u_rechargedDiamond,int& u_maxWipeOutCount,int& u_maxBuyEnergyCount);
//��ȡ�û��ѳ�ֵ��ħʯ����
int getUserRechargedDiamond(const unsigned int& u_userId);
//�����û��ѹ�������ҩˮ����
void updateUserBuyEnergyCount(const unsigned int& u_userId,const int& u_buyEnergyCount);
//�����û���ɨ������
void updateUserWipeOutCount(const unsigned int& u_userId,const int& u_wipeOutCount);
//�ж��û��Ƿ��Ѵﵱ����������������
int isUserToMaxBuyEnergyCount(const unsigned int& u_userId);
//�ж��û��Ƿ��Ѵﵱ��������ɨ������
int isUserToMaxWipeOutCount(const unsigned int& u_userId);
//Ϊ�û���Ϣ�����û��ѳ�ֵ��ħʯ���������տ�ɨ�������������ѹ�������ҩˮ����
void newAddSetSomeUserInfo(PBUser* &u_PBUser,const unsigned int& u_userId);

#endif