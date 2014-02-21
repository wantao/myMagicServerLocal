#ifndef PROCUSERADDFRIEND_H
#define PROCUSERADDFRIEND_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserAddFriend
{
public:
	cProcUserAddFriend();
	~cProcUserAddFriend();
	//�ж��û��Ƿ����
	bool isUserExist(const unsigned int& u_userId);
	//�ж��û������Ƿ�ﵽ����
	bool isUserFriendToMax(const unsigned int& u_userId);
	//�ò����¼�Ƿ��Ѿ�����
	bool hasHistoryRecord(const unsigned int& u_userId,const unsigned int& u_friendId);
	//�����¼
	bool insertRecord(const unsigned int& u_userId,const unsigned int& u_friendId);
	//������Ӻ�������
	bool insertAddFriendReq(const unsigned int& u_userId,const unsigned int& u_friendId);
	//�Է��Ƿ��Ѿ��Ǻ���
	bool isFriend(const unsigned int& u_userId,const unsigned int& u_friendId);
	//�����û���Ӻ�������
	int procUserAddFriend(PBAddFriendReceive& u_PBAddFriendReceive,const string& u_strToken,\
		const unsigned int& u_friendId);
	int getUserMaxFriendCount(const unsigned int& u_userId);
protected:
private:
};

extern cProcUserAddFriend g_cProcUserAddFriend;

#endif