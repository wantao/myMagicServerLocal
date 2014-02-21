#ifndef PROCUSERREGISTCHECK_H
#define PROCUSERREGISTCHECK_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserRegistCheck
{
public:
	cProcUserRegistCheck();
	~cProcUserRegistCheck();
	//获取注册类型
	int getRegistType(const PBRegisterCheckSend& u_PBRegisterCheckSend);
	//检查mac地址是否存在
	int isMacAddressExist(const string& u_macAddress,const int& u_plateform);
	//检查账号是否存在
	int isUserAccountExist(const string& u_userAccount,const int& u_plateform);
	//检查昵称是否存在
	int isUserNameExist(const string& u_userName);
	int setPBRegisterCheckReceive(PBRegisterCheckReceive& u_PBRegisterCheckReceive,const PBRegisterCheckSend& u_PBRegisterCheckSend);
protected:
private:
};

extern cProcUserRegistCheck g_cProcUserRegistCheck;
#endif