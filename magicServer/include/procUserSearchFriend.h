#ifndef PROCUSERSEARCHFRIEND_H
#define PROCUSERSEARCHFRIEND_H

#include <iostream>
#include "../include/MagicCard.pb.h"
#include "../include/magicServerDb.h"
using namespace std;

class cProcUserSearchFriend
{
public:
	cProcUserSearchFriend();
	~cProcUserSearchFriend();
	//获取好友ID列表
	string getUserFriendIdList(const unsigned int& u_userId);
	//代理填充SetPBSearchFriendReceive
	int proxySetPBSearchFriendReceive(PBSearchFriendReceive& u_PBSearchFriendReceive,const unsigned int& u_userId,\
		PBSearchFriendSend& u_PBSearchFriendSend);
	int setPBSearchFriendReceive(PBSearchFriendReceive& u_PBSearchFriendReceive,const string& u_strToken,\
		PBSearchFriendSend& u_PBSearchFriendSend);
protected:
private:
};

extern cProcUserSearchFriend g_cProcUserSearchFriend;
#endif