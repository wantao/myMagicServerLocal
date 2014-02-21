#ifndef PROCUSERGETFIXEDAFFICHE_H
#define PROCUSERGETFIXEDAFFICHE_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserGetFixedAffiche
{
public:
	cProcUserGetFixedAffiche();
	~cProcUserGetFixedAffiche();
	int setPBSystemInfoReceive(PBSystemInfoReceive& u_PBSystemInfoReceive,const string& u_strToken);
protected:
private:
};

extern cProcUserGetFixedAffiche g_cProcUserGetFixedAffiche;
#endif