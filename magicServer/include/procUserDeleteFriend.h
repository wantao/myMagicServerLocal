#ifndef PROCUSERDELETEFRIEND_H
#define PROCUSERDELETEFRIEND_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserDeleteFriend
{
public:
	cProcUserDeleteFriend();
	~cProcUserDeleteFriend();
	//ɾ������
	bool deleteUserFriend(const unsigned int& u_userId,const unsigned int& u_friendId,const string& u_strTable);
	//���PBAllFriend
	int setPBAllFriend(PBAllFriend& u_PBAllFriend,const string& u_strToken,\
		const unsigned int& u_friendId);
protected:
private:
};

extern cProcUserDeleteFriend g_cProcUserDeleteFriend;

#endif