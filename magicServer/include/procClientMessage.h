#ifndef PROCCLIENTMESSAGE_H
#define PROCCLIENTMESSAGE_H

#include "../include/global.h"
#include "../include/structDef.h"
#include "../include/MagicCard.pb.h"

class cProClientMessage
{
public:
	cProClientMessage();
	~cProClientMessage();
	//注册请处理
	string procRegister(const string& u_strClientData);
	//登录请求处理
	string procLogin(const string& u_strClientData);
	//获取系统信息请求处理
	//string procSystemInfo(const string& u_strClientData);
	//获取任务信息请求处理
	string procAllmission(const string& u_strClientData);
	//获取任务奖励请求处理
	string procMissionAward(const string& u_strClientData);
	//获取好友请求处理
	string procFriend(const string& u_strClientData);
	//获取等待确认的好友请求处理
	string procWaitingFrind(const string& u_strClientData);
	//主动添加好友请求处理
	string procAddFriend(const string& u_strClientData);
	//同意添加好友请求处理
	string procFriendAgree(const string& u_strClientData);
	//删除 好友请求处理
	string procFriendDelete(const string& u_strClientData);
	//购买体力请求处理
	string procBuyEnergy(const string& u_strClientData);
	//主动添加好友请求处理
	string procAwardFriend(const string& u_strClientData);
	//获取固定公告栏信息请求处理
	string procGetFixedAffiche(const string& u_strClientData);
	//获取流动公告栏信息请求处理
	string procRunningAffiche(const string& u_strClientData);
	//钻石抽奖请求处理
	string procAwarddiamond(const string& u_strClientData);
	//获取所有助战基友请求处理
	string procHelpFriend(const string& u_strClientData);
	//卡牌升级 
	string procCardLevelUp(const string& u_strClientData);
	//卡牌合成
	string procCardEvolution(const string& u_strClientData);
	//技能升级
	string procSkillLevelUp(const string& u_strClientData);
	//卡牌编队
	string procCardForm(const string& u_strClientData);
	//副本掉落 
	string procBattleLose(const string& u_strClientData);
	//副本结果
	string procBattleResult(const string& u_strClientData);
	//贩卖卡牌
	string procSoldCardResult(const string& u_strClientData);
	//获取友情值
	string procGetFriendValue(const string& u_strClientData);
	//获取友情值
	string procUserFriendReject(const string& u_strClientData);
	//商店购买请求
	string procUserShopBuy(const string& u_strClientData);
	//搜索玩家请求
	string procUserSearchPlayer(const string& u_strClientData);
	//玩家注册检查请求
	string procUserRegistCheck(const string& u_strClientData);
	//金币抽奖请求
	string procUserGoldAward(const string& u_strClientData);
	//获取活动副本
	string procUserActiveBattleReq(const string& u_strClientData);
	//获取用户体力
	string proUserGetEnergyReq(const string& u_strClientData);
	//商店购买请求
	string procUser91ShopBuy(const string& u_strClientData);
	//排行榜请求
	string procUserRankingReq(const string& u_strClientData);
	//获取通知请求
	string procUserGetNoticeReq(const string& u_strClientData);
	//bool Init();
	void unInit();
	//添加客户端套接字
	//void addClient(unsigned int& clientFd);
	//接收客户端消息处理并回应对应消息
	string procClientReqData(const string& u_strClientData);
	string createData(dataPacket& u_dataPacket,const string& u_strBody);
	//接收ProtobufData
	string recvProtobufData(unsigned int& clientFd,dataPacket& nDataPacket);

	//组装及发送数据
	bool createAndSendData(dataPacket& u_dataPacket,unsigned int& u_clientFd,string& u_strBody);

protected:
private:
	
};

extern cProClientMessage g_cProClientMessage;

#endif