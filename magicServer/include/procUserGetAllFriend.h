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
	//填充好友队长卡牌信息
	bool getCaptainCardInfo(PBCard* u_PBCard,const unsigned int& u_userID);
	//填充好友信息
	bool getFriendInfo(PBFriend* u_PBFriend,const unsigned& u_userId);
	//更新用户领取友情值状态
	void updateGetFriendValueFlag(const unsigned int& u_userId,const unsigned int& u_friendId,\
		bool u_getFlag);
	//获取领取友情值状态
	bool getFriendValueFlag(const unsigned int& u_userId,const unsigned int& u_friendId,\
		int& u_getStamp,int& u_getFriendValueFlag);
	//设置PBFriend中是否已领取友情值标志
	void setPBFriendFlag(PBFriend* &u_PBFriend,const unsigned int& u_userId,\
		const unsigned int& u_friendId);
	//获取特定用户ID的所有好友信息
	bool getAllFriendByUserId(PBAllFriend& u_PBAllFriend,const unsigned& u_userId);
	//获取用户所有好友信息
	int getUserAllFriend(PBAllFriend& u_PBAllFriend,const string& u_strToken);
protected:
private:
};

extern cProcUserGetAllFriend g_cProcUserGetAllFriend;
#endif