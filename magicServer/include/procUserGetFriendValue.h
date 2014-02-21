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
	//获取用户已领取友情值次数
	bool getFriendValueCount(const unsigned int& u_userId,int& u_getFriendValueCount);
	//更新已领取友情值次数
	bool updateGetFriendValueCount(const unsigned int& u_userId,const int& u_getFriendValueCount);
	int setPBGetFriendValueReceive(PBGetFriendValueReceive& u_PBGetFriendValueReceive,const string& u_strToken,const PBFriendSend& u_PBFriendSend);
protected:
private:
};

extern cProcUserGetFriendValue g_cProcUserGetFriendValue;

#endif