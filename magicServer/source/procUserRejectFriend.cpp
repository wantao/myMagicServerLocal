#include "../include/procUserRejectFriend.h"
#include "../include/procUserAgreeFriend.h"
#include "../include/procUserWaitingFriend.h"
#include "../include/errorCode.h"
#include "../include/common.h"
#include "../include/myGlog.h"

cProcUserRejectFriend g_cProcUserRejectFriend;

cProcUserRejectFriend::cProcUserRejectFriend()
{

}

cProcUserRejectFriend::~cProcUserRejectFriend()
{

}



int cProcUserRejectFriend::setPBWaitingFriendReject(PBWaitingFriend& u_PBWaitingFriend,const string& u_strToken,const PBFriendSend& u_PBFriendSend)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("setPBWaitingFriendReject getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	g_cProcUserAgreeFriend.deleteRecord(u_userId,u_PBFriendSend.friendid());
	g_cProcUserWaitingFriend.getUserWaitingFriend(u_PBWaitingFriend,u_userId);
	return u_result;
}
