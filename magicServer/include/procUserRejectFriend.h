#ifndef PROCUSERREJECTFRIEND_H
#define PROCUSERREJECTFRIEND_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;


class cProcUserRejectFriend
{
public:
	cProcUserRejectFriend();
	~cProcUserRejectFriend();
	int setPBWaitingFriendReject(PBWaitingFriend& u_PBWaitingFriend,const string& u_strToken,const PBFriendSend& u_PBFriendSend);
protected:
private:
};

extern cProcUserRejectFriend g_cProcUserRejectFriend;

#endif