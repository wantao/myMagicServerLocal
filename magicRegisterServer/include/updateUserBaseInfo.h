#ifndef UPDATEUSERBASEINFO_H
#define UPDATEUSERBASEINFO_H

#include "../include/define.h"
class cUpdateUserBaseInfo
{
public:
	cUpdateUserBaseInfo();
	~cUpdateUserBaseInfo();
	bool getActiveBattleMapByWeekDay(const int& u_weekDay,activeBattleMap& u_activeBattleMap);
	string getActiveBattleInsertList(const int& u_userId,activeBattleMap u_activeBattleMap);
	void insertUserActiveBattleInfo(const int& u_userId,const activeBattleMap& u_activeBattleMap);
	void updateUserActiveBattleInfoByUserID(const int& u_userId);
	void updateUserActiveBattleInfo();
	//初始化领取友情值次数
	void InitUserGetFriendValueCount();
	//初始化领取友情值的状态
	void InitUserGetFriendValueFlag();

protected:
private:
};

extern cUpdateUserBaseInfo g_cUpdateUserBaseInfo;

#endif