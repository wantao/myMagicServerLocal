#ifndef CUSERGETNOTICE_H
#define CUSERGETNOTICE_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cUserGetNotice
{
public:
	cUserGetNotice();
	~cUserGetNotice();
	int setPBFiveSixStarCard(PBGameNotice& u_PBGameNotice,const int& u_noticeTimePoint);
	int setPBEvolutionSuccess(PBGameNotice& u_PBGameNotice,const int& u_noticeTimePoint);
	int setPBPassLevelPlayerInfo(PBGameNotice& u_PBGameNotice,const int& u_noticeTimePoint);
	int setPBCapacity(PBGameNotice& u_PBGameNotice,const int& u_noticeTimePoint);
	int setGameActiveInfo(PBGameNotice& u_PBGameNotice);
	int procSetPBGameNotice(PBGameNotice& u_PBGameNotice,const int& u_noticeTimePoint);
	int setPBGameNotice(PBGameNotice& u_PBGameNotice,const string& u_strToken);

protected:
private:
};

extern cUserGetNotice g_cUserGetNotice;

#endif