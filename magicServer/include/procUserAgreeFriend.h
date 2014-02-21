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
		//判断用户是否存在
		bool isUserExist(const unsigned int& u_userId);
		//记录是否已经存在
		bool hasHistoryRecord(const unsigned int& u_userId,const unsigned int& u_friendId);
		//插入记录
		bool insertRecord(const unsigned int& u_userId,const unsigned int& u_friendId);
		//删除记录
		bool deleteRecord(const unsigned int& u_userId,const unsigned int& u_friendId);
		//填充PBFriendAgreeReceive
		int setPBFriendAgreeReceive(PBFriendAgreeReceive& u_PBFriendAgreeReceive,const string& u_strToken,\
			const unsigned int& u_friendId);
protected:
private:
};
extern cProcUserAgreeFriend g_cProcUserAgreeFriend;
#endif