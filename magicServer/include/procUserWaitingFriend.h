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
	//填充等待确认好友的队长卡牌信息
	bool getCaptainCardInfo(PBCard* u_PBCard,const unsigned int& u_userID);
	//填充等待确认好友的信息
	bool getFriendInfo(PBFriend* u_PBFriend,const unsigned& u_userId);
	//根据用户ID得到等待确认好友ID，并填充等待确认好友信息
	bool getUserWaitingFriend(PBWaitingFriend& u_PBWaitingFriend,const unsigned int& u_userId);
	//处理等待确认好友请求
	int procUserWaitingFriend(PBWaitingFriend& u_PBWaitingFriend,const string& u_strToken);
protected:
private:
};
extern cProcUserWaitingFriend g_cProcUserWaitingFriend;
#endif