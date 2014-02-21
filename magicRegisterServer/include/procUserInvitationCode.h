#ifndef PROCUSERINVITAIONCODE_H
#define PROCUSERINVITAIONCODE_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserInvitationCode
{
public:
	cProcUserInvitationCode();
	~cProcUserInvitationCode();
	int getInvitationCodeUsedCount(const string& u_invitationCode,int& u_usedCount);
	void updateInvitationCodeCount(const string& u_invitationCode,const int& u_usedCount);
	void deleteInvitationCode(const string& u_invitationCode);
	int setPBInvitationCodeReceive(PBInvitationCodeReceive& u_PBInvitationCodeReceive,const PBInvitationCodeSend& u_PBInvitationCodeSend);
protected:
private:
};

extern cProcUserInvitationCode g_cProcUserInvitationCode;

#endif