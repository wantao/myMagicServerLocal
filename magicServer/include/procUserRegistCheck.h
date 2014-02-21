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
	//��ȡע������
	int getRegistType(const PBRegisterCheckSend& u_PBRegisterCheckSend);
	//���mac��ַ�Ƿ����
	int isMacAddressExist(const string& u_macAddress,const int& u_plateform);
	//����˺��Ƿ����
	int isUserAccountExist(const string& u_userAccount,const int& u_plateform);
	//����ǳ��Ƿ����
	int isUserNameExist(const string& u_userName);
	int setPBRegisterCheckReceive(PBRegisterCheckReceive& u_PBRegisterCheckReceive,const PBRegisterCheckSend& u_PBRegisterCheckSend);
protected:
private:
};

extern cProcUserRegistCheck g_cProcUserRegistCheck;
#endif