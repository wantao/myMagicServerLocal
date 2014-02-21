#ifndef PROCMAINSERVERLOGIN_H
#define PROCMAINSERVERLOGIN_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcMainServerLogin
{
public:
	cProcMainServerLogin();
	~cProcMainServerLogin();
	bool isUserAccountRight(const PBMainServerLoginSend& u_PBMainServerLoginSend);
	bool isUserPasswordRight(const PBMainServerLoginSend& u_PBMainServerLoginSend);
	int setPBMainServerLoginReceive(PBMainServerLoginReceive& u_PBMainServerLoginReceive,\
		const PBMainServerLoginSend& u_PBMainServerLoginSend);
protected:
private:
};

extern cProcMainServerLogin g_cProcMainServerLogin;
#endif