#ifndef _PROCUSERRUNNINGAFFICHE_H
#define _PROCUSERRUNNINGAFFICHE_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserRunningAffiche
{
public:
	cProcUserRunningAffiche();
	~cProcUserRunningAffiche();
	bool proxySetRunningAffiche(PBRunningAfficheReceive& u_PBRunningAfficheReceive);
	int setPBRunningAfficheReceive(PBRunningAfficheReceive& u_PBRunningAfficheReceive,const string& u_strToken);
protected:
private:
};

extern cProcUserRunningAffiche g_cProcUserRunningAffiche;

#endif

