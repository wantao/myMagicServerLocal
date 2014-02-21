#ifndef MAGICSERVERDB_H
#define MAGICSERVERDB_H
#include <iostream>
#include "common.h"
#include "MysqlQuery.h"
#include "MysqlDB.h"
#include "../include/MagicCard.pb.h"
using namespace std;

class cMagicServerDb:public MysqlDB
{
public:
	cMagicServerDb();
	~cMagicServerDb();
	bool Init();
	//从数据库实例化
	bool DbReadInit();
	void unInit();

	////////////得到PBSystemInfoReceive所需的东西/////begin
	//填充PBSystemInfoReceive
	void setPBSystemInfoReceive(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//获取公告信息
	void PBSystemInfoReceive_setAfficheInfo(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//获取服务器所有分区信息
	void PBSystemInfoReceive_setServerInfo(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//获取游戏版本信息
	void PBSystemInfoReceive_setGameVersion(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//获取资源版本信息
	void PBSystemInfoReceive_setResourceVersion(PBSystemInfoReceive& u_PBSystemInfoReceive);
	//获取副本版本信息
	void PBSystemInfoReceive_setBattleVersion(PBSystemInfoReceive& u_PBSystemInfoReceive);
	////////////得到PBSystemInfoReceive所需的东西/////end
	

	////////////////////////注册处理 得到PBLoginReceive所需的信息/////begin

	//判断是否有任务完成
	bool hasMissionFinished(const unsigned int& u_userId);
	//插入新用户
	int PBLoginReceive_gernerateNewUser(PBUser* u_PBUser,const PBRegisterSend& u_PBRegisterSend,unsigned int& newUserId,const int& u_regType);
	//判断注册用户帐号是否已经存在
	bool isRegisterUserAccountExist(const string& u_strUserAccount,const int& u_plateform);
	//判断mac地址地址注册的用户是否已经存在
	bool isRegisterUserMacExist(const string& u_strUserMac,const int& u_plateform);
	//判断用户昵称是否存在
	bool isRegisterUserNameExist(const string& u_strUserName);
	//根据用户MacAddress和UserName生成新的用户信息 
	//0:注册成功，1：此MacAddress已经注册，2：注册失败
	int PBLoginReceive_gernerateUserByMacAndUserName(PBUser* u_PBUser,const PBRegisterSend& u_PBRegisterSend,unsigned int& newUserId);
	//获取活动副本信息
	bool PBLoginReceive_getActiveBattleInfo(PBBattle* u_PBBattle,const unsigned int& u_userId);

	
	//插入初始卡牌信息
	bool insertInitCardInfo(PBCard* u_PBCard,const int& u_CardId,unsigned int& nNewUserId,MysqlQuery& q,int battlePosition);
	//获取初始卡牌信息
	bool getInitCardInfoByCardId(PBCard* u_PBCard,const int& u_CardId,unsigned int& nNewUserId,int battlePosition);
	//根据职业编号获取队长卡牌ID
	string getCapitorCardByProfession(const int& u_playerProfessionIndex);
	//填充PBAllCard
	int PBLoginReceive_setAndGetUserCardInfo(PBAllCard* u_PBAllCard,unsigned int& nNewUserId,const int& u_profession);

	//填充PBAllMission
	void PBLoginReceive_setMissionsInfo(PBAllMission* u_PBAllMission,unsigned int& nNewUserId);

	//填充战斗卡牌信息
	void PBLoginReceive_setBattleCardInfo(PBBattleCard* u_PBBattleCard,unsigned int& nNewUserId);

	bool isExistToken(string& strToken);

	bool isExistUserId(unsigned int& nNewUserId);

	bool insertUserIdToken(string& strToken,unsigned int& nNewUserId);

	bool updateUserToken(string& strToken,unsigned int& nNewUserId);

	void PBLoginReceive_setToken(string& strTokenRet,unsigned int& nNewUserId);

	////////////////////////注册处理 得到PBLoginReceive所需的信息/////end


	///////////////////登录处理 得到PBLoginReceive所需的信息/////begin
	////////////////////登录处理 得到PBLoginReceive所需的信息/////end

	//填充好友信息
	void PBLoginReceive_setFriendInfo(PBAllFriend& u_PBAllFriend,unsigned int& nNewUserId);


	//根据userToken查找对应的用户ID
	int getUserIdByUserToken(string& userToken);
	//token值和用id插入表中
	bool InsertUserTokenInfo(string& userToken,int& userId);

	//数据库连接保活
	void mysqlKeepAlive(int u_Type);
	//删除用户
	void deletUserByUserId(int u_userId);
	//判断是否需要更新数据
	bool isSeverExecuteDataUpdate(const string& u_serverIp);
	//防止主从同步延迟造成的从从库取不到数据的情况
	MysqlQuery master_slave_exQuery(const string& strSql);
	
	
protected:
private:
	//数据库连接开关
	bool m_bConnect;
};
extern cMagicServerDb g_cMagicServerDb;
extern cMagicServerDb g_cMagicServerDbRead;
#endif



