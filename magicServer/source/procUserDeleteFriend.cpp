#include "../include/procUserDeleteFriend.h"
#include "../include/procUserGetAllFriend.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/myGlog.h"

cProcUserDeleteFriend g_cProcUserDeleteFriend;


cProcUserDeleteFriend::cProcUserDeleteFriend()
{

}

cProcUserDeleteFriend::~cProcUserDeleteFriend()
{

}

//É¾³ýºÃÓÑ
bool cProcUserDeleteFriend::deleteUserFriend(const unsigned int& u_userId,const unsigned int& u_friendId,const string& u_strTable)
{
	char buf[128] = {0};
	sprintf(buf,"delete from %s where \
				userId = %d and friendId = %d;",u_strTable.c_str(),u_userId,u_friendId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("AgreeFriend deleteRecord faliure,strSql:%s\n",strSql.c_str());
		return false;
	}
	return true;
}

//Ìî³äPBAllFriend
int cProcUserDeleteFriend::setPBAllFriend(PBAllFriend& u_PBAllFriend,const string& u_strToken,\
					const unsigned int& u_friendId)
{
	unsigned int u_userId = 0;
	int u_result = 0;
	if (!getUserIdbyToken(u_strToken,u_userId))
	{
		g_cMyGlog.errorlogRecord("procUserAddFriend getUserIdbyToken error,Token:%s\n",u_strToken.c_str());
		return K_ACCOUNT_UNNORMAL;
	}
	char buf[32] = {0};
	sprintf(buf,"m_userFriendInfo%d",u_userId%USER_FRIEND_TABLE_COUNT);
	string u_strTable(buf);
	deleteUserFriend(u_userId,u_friendId,u_strTable);

	memset(buf,0,sizeof(buf));
	sprintf(buf,"m_userFriendInfo%d",u_friendId%USER_FRIEND_TABLE_COUNT);
	string u_strTable1(buf);
	deleteUserFriend(u_friendId,u_userId,u_strTable1);
	g_cProcUserGetAllFriend.getAllFriendByUserId(u_PBAllFriend,u_userId);
	return u_result;

}