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
	//判断用户是否存在
	bool isUserExist(const unsigned int& u_userId);
	//判断用户好友是否达到上限
	bool isUserFriendToMax(const unsigned int& u_userId);
	//该插入记录是否已经存在
	bool hasHistoryRecord(const unsigned int& u_userId,const unsigned int& u_friendId);
	//插入记录
	bool insertRecord(const unsigned int& u_userId,const unsigned int& u_friendId);
	//插入添加好友请求
	bool insertAddFriendReq(const unsigned int& u_userId,const unsigned int& u_friendId);
	//对方是否已经是好友
	bool isFriend(const unsigned int& u_userId,const unsigned int& u_friendId);
	//处理用户添加好友请求
	int procUserAddFriend(PBAddFriendReceive& u_PBAddFriendReceive,const string& u_strToken,\
		const unsigned int& u_friendId);
	int getUserMaxFriendCount(const unsigned int& u_userId);
protected:
private:
};

extern cProcUserAddFriend g_cProcUserAddFriend;

#endif