#ifndef DELETETMPDATA_H
#define DELETETMPDATA_H

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
protected:
private:
};

extern cDeleteTmpData g_cDeleteTmpData;

#endif