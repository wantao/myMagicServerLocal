#ifndef PROCUSERACTIVEBATTLE_H
#define PROCUSERACTIVEBATTLE_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserActiveBattle
{
public:
	cProcUserActiveBattle();
	~cProcUserActiveBattle();
	int getUserActiveBattle(PBBattle& u_PBBattle,const string& u_strToken);
protected:
private:
};

extern cProcUserActiveBattle g_cProcUserActiveBattle;

#endif