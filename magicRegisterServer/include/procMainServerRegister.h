#ifndef PROCMAINSERVERREGISTER_H
#define PROCMAINSERVERREGISTER_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcMainServerRegister
{
public:
	cProcMainServerRegister();
	~cProcMainServerRegister();
	bool isUserAccountExist(const PBMainServerRegisterSend& u_PBMainServerRegisterSend);
	bool insertUserAccount(const PBMainServerRegisterSend& u_PBMainServerRegisterSend);
	int setPBMainServerRegisterReceive(PBMainServerRegisterReceive& u_PBMainServerRegisterReceive,\
		const PBMainServerRegisterSend& u_PBMainServerRegisterSend);
protected:
private:
};

extern cProcMainServerRegister g_cProcMainServerRegister;
#endif