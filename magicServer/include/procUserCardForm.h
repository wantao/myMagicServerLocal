#ifndef PROCUSERCARDFORM_H
#define PROCUSERCARDFORM_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserCardForm
{
public:
	cProcUserCardForm();
	~cProcUserCardForm();
	bool updateCardPosition(const unsigned int& u_userId,const unsigned int& u_itemId,\
		int u_battlePosition,bool isBattleStatus);
	int getUserLeaderValue(const unsigned int& u_userId);
	bool getCardStarLevel(const unsigned int& u_userId,const int& u_cardItemId,int& u_cardLeaderValue);
	bool getFormCardLeaderValue(const unsigned int& u_userId,PBCardFormSend& u_PBCardFormSend,int& u_formCardLeaderValue);
	int isLeaderValueEnough(const unsigned int& u_userId,PBCardFormSend& u_PBCardFormSend);
	int setPBCardFormReceive(PBCardFormReceive& u_PBCardFormReceive,const string& u_strToken,PBCardFormSend& u_PBCardFormSend);
protected:
private:
};

extern cProcUserCardForm g_cProcUserCardForm;

#endif