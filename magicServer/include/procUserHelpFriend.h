#ifndef PROCUSERHELPFRIEND_H
#define PROCUSERHELPFRIEND_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserHelpFriend
{
public:
	cProcUserHelpFriend();
	~cProcUserHelpFriend();
	int setPBHelpFriend(PBHelpFriend& u_PBHelpFriend,const string& u_strToken);
	//�Ӻ��ѱ����ȡ
	bool getAllHelpFriendByUserId(PBHelpFriend& u_PBHelpFriend,const unsigned& u_userId);
	//���û����л�ȡ��ս����
	bool getHelpFriendFromUserTable(PBHelpFriend& u_PBHelpFriend,const unsigned int& u_userId);

	//�����û�Id��ȡս�����Ƶ�ս������
	int getUserCapacityClassByUserId(const unsigned int& u_userId);
	//����������ݼ��û�ս�����Ƶĵ��λ�ȡ���·�˵ĵ���
	int getRandCapacityClass(const int& u_userClass,const int& u_randData);
	//����·�˵�ս�����Ƶĵ��λ�ȡ·�˵�ID
	int getPlayerIdByRandClass(int u_randClass,const string& u_hasUsedIdList);
	//�����û�ս�����Ƶĵ��λ�ȡ·��Id
	int getPlayerIdByUserCapacityClass(const int& u_userCapacityClass,const string& u_hasUsedIdList);
protected:
private:
};

extern cProcUserHelpFriend g_cProcUserHelpFriend;
#endif