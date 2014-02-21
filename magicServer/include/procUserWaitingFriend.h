#ifndef PROCUSERWAITINGFRIEND_H
#define PROCUSERWAITINGFRIEND_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;


class cProcUserWaitingFriend
{
public:
	cProcUserWaitingFriend();
	~cProcUserWaitingFriend();
	//���ȴ�ȷ�Ϻ��ѵĶӳ�������Ϣ
	bool getCaptainCardInfo(PBCard* u_PBCard,const unsigned int& u_userID);
	//���ȴ�ȷ�Ϻ��ѵ���Ϣ
	bool getFriendInfo(PBFriend* u_PBFriend,const unsigned& u_userId);
	//�����û�ID�õ��ȴ�ȷ�Ϻ���ID�������ȴ�ȷ�Ϻ�����Ϣ
	bool getUserWaitingFriend(PBWaitingFriend& u_PBWaitingFriend,const unsigned int& u_userId);
	//����ȴ�ȷ�Ϻ�������
	int procUserWaitingFriend(PBWaitingFriend& u_PBWaitingFriend,const string& u_strToken);
protected:
private:
};
extern cProcUserWaitingFriend g_cProcUserWaitingFriend;
#endif