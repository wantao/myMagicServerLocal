#ifndef DELETETMPDATA_H
#define DELETETMPDATA_H

#include <iostream>
using namespace std;

class cDeleteTmpData
{
public:
	cDeleteTmpData();
	~cDeleteTmpData();
	void deleteTmpEquipInfo(const unsigned int& u_tableId);
	void deleteTmpCardInfo(const unsigned int& u_tableId);
	void deleteTmpData();
	void deleteUserActiveBattleInfoById(const int& u_tableId);
	void deleteUserActiveBattleInfo();
	bool InitDeleteTmpData();
	//删除高星级卡牌通知
	void deleteHighStarCardGotNotice(const int& u_noticeTimePoint);
	//删除进化成功通知
	void deleteEvolutionSuccessNotice(const int& u_noticeTimePoint);
	//删除战斗力突破通知
	void deleteFightCapacityBreakThroughNotice(const int& u_noticeTimePoint);

	//获取通过该大关的最大排名
	int getMaxLevelRankByLevelCode(const string& u_levelCode);
	//根据大关名字和通过该大关的最大排名，删除通关通知
	void deletePassBigLevelNotice(const string& u_levelCode,const int& u_maxRank);
	//删除通关通知记录信息
	void deletePassBigLevelNotice(/*const int& u_noticeTimePoint*/);
protected:
private:
};

extern cDeleteTmpData g_cDeleteTmpData;

#endif