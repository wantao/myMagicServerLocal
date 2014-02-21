#ifndef PROCUSERGETFRIENDVALUE_H
#define PROCUSERGETFRIENDVALUE_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserGetFriendValue
{
public:
	cProcUserGetFriendValue();
	~cProcUserGetFriendValue();
	//��ȡ�û�����ȡ����ֵ����
	bool getFriendValueCount(const unsigned int& u_userId,int& u_getFriendValueCount);
	//��������ȡ����ֵ����
	bool updateGetFriendValueCount(const unsigned int& u_userId,const int& u_getFriendValueCount);
	int setPBGetFriendValueReceive(PBGetFriendValueReceive& u_PBGetFriendValueReceive,const string& u_strToken,const PBFriendSend& u_PBFriendSend);
protected:
private:
};

extern cProcUserGetFriendValue g_cProcUserGetFriendValue;

#endif