#ifndef PROCUSERAGREEFRIEND_H
#define PROCUSERAGREEFRIEND_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserAgreeFriend
{
public:
		cProcUserAgreeFriend();
		~cProcUserAgreeFriend();
		//�ж��û��Ƿ����
		bool isUserExist(const unsigned int& u_userId);
		//��¼�Ƿ��Ѿ�����
		bool hasHistoryRecord(const unsigned int& u_userId,const unsigned int& u_friendId);
		//�����¼
		bool insertRecord(const unsigned int& u_userId,const unsigned int& u_friendId);
		//ɾ����¼
		bool deleteRecord(const unsigned int& u_userId,const unsigned int& u_friendId);
		//���PBFriendAgreeReceive
		int setPBFriendAgreeReceive(PBFriendAgreeReceive& u_PBFriendAgreeReceive,const string& u_strToken,\
			const unsigned int& u_friendId);
protected:
private:
};
extern cProcUserAgreeFriend g_cProcUserAgreeFriend;
#endif