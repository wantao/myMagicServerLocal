#ifndef PROCUSERGETALLFRIEND_H
#define PROCUSERGETALLFRIEND_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserGetAllFriend
{
public:
	cProcUserGetAllFriend();
	~cProcUserGetAllFriend();
	//�����Ѷӳ�������Ϣ
	bool getCaptainCardInfo(PBCard* u_PBCard,const unsigned int& u_userID);
	//��������Ϣ
	bool getFriendInfo(PBFriend* u_PBFriend,const unsigned& u_userId);
	//�����û���ȡ����ֵ״̬
	void updateGetFriendValueFlag(const unsigned int& u_userId,const unsigned int& u_friendId,\
		bool u_getFlag);
	//��ȡ��ȡ����ֵ״̬
	bool getFriendValueFlag(const unsigned int& u_userId,const unsigned int& u_friendId,\
		int& u_getStamp,int& u_getFriendValueFlag);
	//����PBFriend���Ƿ�����ȡ����ֵ��־
	void setPBFriendFlag(PBFriend* &u_PBFriend,const unsigned int& u_userId,\
		const unsigned int& u_friendId);
	//��ȡ�ض��û�ID�����к�����Ϣ
	bool getAllFriendByUserId(PBAllFriend& u_PBAllFriend,const unsigned& u_userId);
	//��ȡ�û����к�����Ϣ
	int getUserAllFriend(PBAllFriend& u_PBAllFriend,const string& u_strToken);
protected:
private:
};

extern cProcUserGetAllFriend g_cProcUserGetAllFriend;
#endif