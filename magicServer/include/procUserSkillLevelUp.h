#ifndef PROCUSERSKILLEVEL_H
#define PROCUSERSKILLEVEL_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserSkillLevelUp
{
public:
	cProcUserSkillLevelUp();
	~cProcUserSkillLevelUp();
	bool getCardSkillLevelExp(const unsigned int& u_itemId,unsigned int& u_skillLevel,\
		const unsigned int& u_userId,unsigned int& u_skillExp);

	bool updateCardSkillLevelExp(const unsigned int& u_itemId,unsigned int& u_skillLevel,\
		const unsigned int& u_userId,unsigned int& u_skillExp);

	//技能是否能升级
	bool isSkillCanLevelUp(unsigned int& u_skillLevel,const unsigned int& u_skillExp);
	int setPBSkillLevelUpReceive(PBSkillLevelUpReceive& u_PBSkillLevelUpReceive,const string& u_strToken,PBSkillLevelUpSend& u_PBSkillLevelUpSend);


protected:
private:
};

extern cProcUserSkillLevelUp g_cProcUserSkillLevelUp;
#endif