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
	//ɾ�����Ǽ�����֪ͨ
	void deleteHighStarCardGotNotice(const int& u_noticeTimePoint);
	//ɾ�������ɹ�֪ͨ
	void deleteEvolutionSuccessNotice(const int& u_noticeTimePoint);
	//ɾ��ս����ͻ��֪ͨ
	void deleteFightCapacityBreakThroughNotice(const int& u_noticeTimePoint);

	//��ȡͨ���ô�ص��������
	int getMaxLevelRankByLevelCode(const string& u_levelCode);
	//���ݴ�����ֺ�ͨ���ô�ص����������ɾ��ͨ��֪ͨ
	void deletePassBigLevelNotice(const string& u_levelCode,const int& u_maxRank);
	//ɾ��ͨ��֪ͨ��¼��Ϣ
	void deletePassBigLevelNotice(/*const int& u_noticeTimePoint*/);
protected:
private:
};

extern cDeleteTmpData g_cDeleteTmpData;

#endif