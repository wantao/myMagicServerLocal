#include "../include/procClientMessage.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/CSocketBase.h"
#include "../include/magicServerDb.h"
#include "../include/procUserLogin.h"
#include "../include/procUserAllMissions.h"
#include "../include/procUserMissionAward.h"
#include "../include/procUserGetAllFriend.h"
#include "../include/procUserWaitingFriend.h"
#include "../include/procUserAddFriend.h"
#include "../include/procUserAgreeFriend.h"
#include "../include/procUserDeleteFriend.h"
#include "../include/procUserDiamondAward.h"
#include "../include/procUserUseEnergy.h"
#include "../include/procUserAwardFriend.h"
#include "../include/procUserHelpFriend.h"
#include "../include/procUserCardLevelUp.h"
#include "../include/procUserSkillLevelUp.h"
#include "../include/procUserCardForm.h"
#include "../include/procUserGetFixedAffiche.h"
#include "../include/procUserRunningAffiche.h"
#include "../include/procUserBattle.h"
#include "../include/procUserBattleResult.h"
#include "../include/procUserCardEvolution.h"
#include "../include/procUserSoldCard.h"
#include "../include/procUserGetFriendValue.h"
#include "../include/procUserRejectFriend.h"
#include "../include/procUserShopBuy.h"
#include "../include/procUserSearchFriend.h"
#include "../include/procUserRegistCheck.h"
#include "../include/procUserGoldAward.h"
#include "../include/procUserActiveBattle.h"
#include "../include/updateUserBaseInfo.h"
#include "../include/myGlog.h"
#include "../include/procUserRankReq.h"
#include "../include/cUserGetNotice.h"



cProClientMessage g_cProClientMessage;

cProClientMessage::cProClientMessage()
{
}

cProClientMessage::~cProClientMessage()
{
}


//接收ProtobufData
string cProClientMessage::recvProtobufData(unsigned int& clientFd,dataPacket& nDataPacket)
{
	int protobufLen = 0;
	int nRecvDataLen = 0;
	string strProtobufData = "";
	protobufLen = nDataPacket.protobufLength;

	char *protoBuf = NULL;
	protoBuf = (char*)malloc((protobufLen+1)*sizeof(char));
	if (protoBuf == NULL)
	{
		return strProtobufData;
	}
	nRecvDataLen = recv(clientFd,protoBuf,protobufLen,MSG_WAITALL);
	if (protobufLen != nRecvDataLen)
	{
		g_cMyGlog.errorlogRecord("error:nRecvDataLen:%d,protobufLen:%d\n",nRecvDataLen,protobufLen);
	}
	else
	{
		strProtobufData.append(protoBuf,protobufLen);
	}
	if (protoBuf)
	{
		free(protoBuf);
		protoBuf = NULL;
	}
	return strProtobufData;
}

string cProClientMessage::createData(dataPacket& u_dataPacket,const string& u_strBody)
{
	string u_retData = "";
	u_dataPacket.protobufLength = u_strBody.size();
	u_retData.append((char*)&u_dataPacket,sizeof(dataPacket));
	u_retData.append(u_strBody.c_str(),u_strBody.size());

	return u_retData;
}

//注册请处理
string cProClientMessage::procRegister(const string& u_strClientData)
{	
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	int u_result = 0;
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);
	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procRegister error");
		return u_retData;
	}
	PBRegisterSend u_PBRegisterSend;

	if (!u_PBRegisterSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procRegister ParseFromString error\n");
		return u_retData;
	}
	PBLoginReceive u_PBLoginReceive;
	PBUser *u_PBUser;
	//玩家信息
	u_PBUser = u_PBLoginReceive.mutable_user();
	int nPBUser_Result;
	unsigned int nNewUserId = 0;
	nPBUser_Result = g_cMagicServerDb.PBLoginReceive_gernerateUserByMacAndUserName(u_PBUser,u_PBRegisterSend,nNewUserId);
	if (nPBUser_Result == -1)
	{
		g_cMyGlog.errorlogRecord("procRegister cut down connect!");
		return u_retData;
	}
	string strBody = "";
	if (nPBUser_Result != PROC_SUCCESS)
	{
		g_cMyGlog.errorlogRecord("procRegister error: %d\n",nPBUser_Result);
		u_PBLoginReceive.set_result(nPBUser_Result);

		nDataPacket.nType = nPBUser_Result;
	}
	else
	{
		u_PBLoginReceive.set_result(PROC_SUCCESS);

		//所有卡牌信息
		PBAllCard* u_PBAllCard;
		u_PBAllCard = u_PBLoginReceive.mutable_cards();
		u_result = g_cMagicServerDb.PBLoginReceive_setAndGetUserCardInfo(u_PBAllCard,nNewUserId,u_PBRegisterSend.captainindex());
		if (u_result == -1)
		{
			g_cMyGlog.errorlogRecord("professionId error:%d\n",u_PBRegisterSend.captainindex());
			g_cMagicServerDb.deletUserByUserId(nNewUserId);
			return u_retData;
		}

		//副本信息
		PBBattle* u_PBBattle;
		u_PBBattle = u_PBLoginReceive.mutable_battle();
		u_PBBattle->set_pveprogress(INIT_BATTLE_PROGRESS);

		int u_userId = nNewUserId;
		g_cUpdateUserBaseInfo.updateUserActiveBattleInfoByUserID(u_userId);
		g_cMagicServerDb.PBLoginReceive_getActiveBattleInfo(u_PBBattle,nNewUserId);
		

		//所有任务信息
		PBAllMission* u_PBAllMission;
		u_PBAllMission = u_PBLoginReceive.mutable_missions();
		g_cMagicServerDb.PBLoginReceive_setMissionsInfo(u_PBAllMission,nNewUserId);


		//战斗卡牌
		PBBattleCard* u_PBBattleCard;
		u_PBBattleCard = u_PBLoginReceive.mutable_battlecards();
		g_cMagicServerDb.PBLoginReceive_setBattleCardInfo(u_PBBattleCard,nNewUserId);

		//填充token值
		string strToken("");
		g_cMagicServerDb.PBLoginReceive_setToken(strToken,nNewUserId);

		u_PBLoginReceive.set_token(strToken);

		int u_userFightCapacity = 0;
		int u_userCapitorCardFightCapacity = 0;
		u_userFightCapacity = getUserFightCapacity(nNewUserId,u_userCapitorCardFightCapacity);
		insertUserFightCapacity(nNewUserId,u_userFightCapacity,u_userCapitorCardFightCapacity);

		u_PBLoginReceive.set_fightcapacity(u_userFightCapacity);

		//memcpy(strHead.tokenValue,strToken.c_str(),strToken.size());
		u_PBLoginReceive.SerializeToString(&strBody);

		
	}
	u_retData = createData(nDataPacket,strBody);
	return u_retData;
}

//组装及发送数据
bool cProClientMessage::createAndSendData(dataPacket& u_dataPacket,unsigned int& u_clientFd,string& u_strBody)
{
	string strSendData("");
	u_dataPacket.protobufLength = u_strBody.size();
	strSendData.append((char*)&u_dataPacket,sizeof(dataPacket));
	strSendData.append(u_strBody.c_str(),u_strBody.size());


	if (!g_cSockBase.mvSendMsgToSocket(u_clientFd,strSendData))
	{
		g_cMyGlog.errorlogRecord("createAndSendData sent to %d failure,nType:%d\n",u_clientFd,u_dataPacket.nType);
		return false;
	}

	return true;
}
//登录请求处理
string cProClientMessage::procLogin(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);
	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procLogin error\n");
		return u_retData;
	}
	PBLoginReceive u_PBLoginReceive;
	int u_procResult =g_cProcUserLogin.procUserLogin(u_PBLoginReceive,strProtobufData);
	if (u_procResult == -1)
	{
		g_cMyGlog.errorlogRecord("procLogin cut down connect!");
		return u_retData;
	}
	string u_strBody("");
	if (u_procResult == PROC_SUCCESS)
	{
		u_PBLoginReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_procResult;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}
////获取系统信息请求处理
//bool cProClientMessage::procSystemInfo(unsigned int& clientFd,dataPacket& nDataPacket)
//{
//	//包格式 dataPacket +　protobufData;
//	PBSystemInfoReceive u_PBSystemInfoReceive;
//	string strReturnData = "";
//	string strProtobufData = "";
//
//	g_cMagicServerDb.setPBSystemInfoReceive(u_PBSystemInfoReceive);
//	u_PBSystemInfoReceive.SerializeToString(&strProtobufData);
//	
//	nDataPacket.protobufLength = strProtobufData.size();
//	
//	strReturnData.append((char*)&nDataPacket,sizeof(nDataPacket));
//	strReturnData.append(strProtobufData.c_str(),strProtobufData.size());
//
//	if (!g_cSockBase.mvSendMsgToSocket(clientFd,strReturnData))
//	{
//		cerr<<"sent to "<<clientFd<<"failure"<<endl;
//		return false;
//	}
//	return true;
//}
//获取任务信息请求处理
string cProClientMessage::procAllmission(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());

	PBAllMission u_PBAllMission;
	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);
	//cerr<<"in procAllmission"<<endl;
	int u_result = -1;

	u_result = g_cProcUserAllMissions.procUserAllMission(u_PBAllMission,u_strToken);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procAllmission cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBAllMission.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}

	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}
//获取任务奖励请求处理
string cProClientMessage::procMissionAward(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);
	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procMissionAward error\n");
		return u_retData;
	}

	PBMissionAwardSend u_PBMissionAwardSend;
	if (!u_PBMissionAwardSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procMissionAward ParseFromString\n");
		return u_retData;
	}

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);
	//cerr<<"procMissionAward missionId:"<<u_PBMissionAwardSend.missionid()<<endl;
	
	PBMissionAwardReceive u_PBMissionAwardReceive;
	int u_result = -1;
	u_result = g_cProUserMissionAward.procUserMissionAward(u_PBMissionAwardReceive,u_strToken,u_PBMissionAwardSend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procMissionAward cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBMissionAwardReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}
//获取好友请求处理
string cProClientMessage::procFriend(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);
	
	PBAllFriend u_PBAllFriend;
	int u_result = -1;
	u_result = g_cProcUserGetAllFriend.getUserAllFriend(u_PBAllFriend,u_strToken);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procFriend cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBAllFriend.SerializeToString(&u_strBody);
		//cerr<<"AAA size:"<<u_strBody.size()<<endl;
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}
//获取等待确认的好友请求处理
string cProClientMessage::procWaitingFrind(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBWaitingFriend u_PBWaitingFriend;
	int u_result = -1;
	u_result = g_cProcUserWaitingFriend.procUserWaitingFriend(u_PBWaitingFriend,u_strToken);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procWaitingFrind cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBWaitingFriend.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}
//主动添加好友请求处理
string cProClientMessage::procAddFriend(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);
	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procAddFriend error\n");
		return u_retData;
	}

	PBFriendSend u_PBFriendSend;
	if (!u_PBFriendSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procAddFriend ParseFromString\n");
		return u_retData;
	}
	

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	unsigned int u_friend = u_PBFriendSend.friendid();

	PBAddFriendReceive u_PBAddFriendReceive;
	int u_result = -1;
	u_result = g_cProcUserAddFriend.procUserAddFriend(u_PBAddFriendReceive,u_strToken,u_friend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procAddFriend cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBAddFriendReceive.SerializeToString(&u_strBody);	
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}
//同意添加好友请求处理
string cProClientMessage::procFriendAgree(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procAddFriend error\n");
		return u_retData;
	}

	PBFriendSend u_PBFriendSend;
	if (!u_PBFriendSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procFriendAgree ParseFromString error\n");
		return u_retData;
	}
	

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	unsigned int u_friend = u_PBFriendSend.friendid();

	PBFriendAgreeReceive u_PBFriendAgreeReceive;

	int u_result = -1;
	u_result = g_cProcUserAgreeFriend.setPBFriendAgreeReceive(u_PBFriendAgreeReceive,u_strToken,u_friend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procFriendAgree cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBFriendAgreeReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}
//删除 好友请求处理
string cProClientMessage::procFriendDelete(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procFriendDelete error\n");
		return u_retData;
	}

	PBFriendSend u_PBFriendSend;
	if (!u_PBFriendSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procFriendDelete ParseFromString\n");
		return u_retData;
	}
	

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	unsigned int u_friend = u_PBFriendSend.friendid();

	PBAllFriend u_PBAllFriend;
	int u_result = -1;
	u_result = g_cProcUserDeleteFriend.setPBAllFriend(u_PBAllFriend,u_strToken,u_friend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procFriendDelete cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBAllFriend.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}
//使用体力请求处理
string cProClientMessage::procBuyEnergy(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBUseEnergyReceive u_PBUseEnergyReceive;
	int u_result = -1;
	u_result = g_cProcUserUseEnergy.setPBUseEnergyReceive(u_PBUseEnergyReceive,u_strToken);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procBuyEnergy cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBUseEnergyReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	
	return u_retData;
}
//友情抽奖请求处理
string cProClientMessage::procAwardFriend(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procAwardFriend error\n");
		return u_retData;
	}

	PBAwardSend u_PBAwardSend;
	if (!u_PBAwardSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procAwardFriend ParseFromString error\n");
		return u_retData;
	}
	

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBAwardReceive u_PBAwardReceive;
	int u_result = -1;
	u_result = g_cProcUserAwardFriend.setPBAwardReceive(u_PBAwardReceive,u_strToken,u_PBAwardSend.awardtype());
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procAwardFriend cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBAwardReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}

	u_retData = createData(nDataPacket,u_strBody);

	return u_retData;
}
//获取固定公告栏信息请求处理
string cProClientMessage::procGetFixedAffiche(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBSystemInfoReceive u_PBSystemInfoReceive;
	int u_result = -1;
	u_result = g_cProcUserGetFixedAffiche.setPBSystemInfoReceive(u_PBSystemInfoReceive,u_strToken);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procGetFixedAffiche cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBSystemInfoReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}
//获取流动公告栏信息请求处理
string cProClientMessage::procRunningAffiche(const string& u_strClientData)
{

	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBRunningAfficheReceive u_PBRunningAfficheReceive;
	int u_result = -1;
	u_result = g_cProcUserRunningAffiche.setPBRunningAfficheReceive(u_PBRunningAfficheReceive,u_strToken);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procRunningAffiche cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBRunningAfficheReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;

}
//钻石抽奖请求处理
string cProClientMessage::procAwarddiamond(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procAwarddiamond error\n");
		return u_retData;
	}

	PBAwardSend u_PBAwardSend;
	if (!u_PBAwardSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procAwarddiamond ParseFromString\n");
		return u_retData;
	}

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBAwardReceive u_PBAwardReceive;
	int u_result = -1;
	u_result = g_cProcUserDiamondAward.setPBAwardReceive(u_PBAwardReceive,u_strToken,u_PBAwardSend.awardtype());
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procAwarddiamond cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBAwardReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData ;
}
//获取所有助战基友请求处理
string cProClientMessage::procHelpFriend(const string& u_strClientData)
{
	g_cMyGlog.debugLog("in procHelpFriend\n");
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBHelpFriend u_PBHelpFriend;
	int u_result = -1;
	u_result = g_cProcUserHelpFriend.setPBHelpFriend(u_PBHelpFriend,u_strToken);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procHelpFriend cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBHelpFriend.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//卡牌升级 
string cProClientMessage::procCardLevelUp(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procCardLevelUp error\n");
		return u_retData;
	}

	PBCardLevelUpSend u_PBCardLevelUpSend;
	if (!u_PBCardLevelUpSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procCardLevelUp ParseFromString error\n");
		return u_retData;
	}
	

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBCardLevelUpReceive u_PBCardLevelUpReceive;
	int u_result = -1;
	u_result = g_cProcUserCardLevel.setPBCardLevelUpReceive(u_PBCardLevelUpReceive,u_strToken,u_PBCardLevelUpSend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procCardLevelUp cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBCardLevelUpReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//卡牌合成
string cProClientMessage::procCardEvolution(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procCardEvolution error\n");
		return u_retData;
	}

	PBCardEvolutionSend u_PBCardEvolutionSend;
	if (!u_PBCardEvolutionSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procCardEvolution ParseFromString error\n");
		return u_retData;
	}

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBCardEvolutionRecieve u_PBCardEvolutionRecieve;
	int u_result = -1;
	u_result = g_cProcUserCardEvolution.setPBCardEvolutionRecieve(u_PBCardEvolutionRecieve,u_strToken,u_PBCardEvolutionSend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procCardEvolution cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBCardEvolutionRecieve.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//技能升级
string cProClientMessage::procSkillLevelUp(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procSkillLevelUp error\n");
		return u_retData;
	}

	PBSkillLevelUpSend u_PBSkillLevelUpSend;
	if (!u_PBSkillLevelUpSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procSkillLevelUp ParseFromString error\n");
		return u_retData;
	}

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBSkillLevelUpReceive u_PBSkillLevelUpReceive;
	int u_result = -1;
	u_result = g_cProcUserSkillLevelUp.setPBSkillLevelUpReceive(u_PBSkillLevelUpReceive,u_strToken,u_PBSkillLevelUpSend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procSkillLevelUp cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBSkillLevelUpReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	///////DE_BUG////test begin
//#ifdef DEBUG_MOD
//	g_cMyGlog.errorlogRecord("u_strBody.size:%d\n",u_strBody.size());
//	PBSkillLevelUpReceive u_PBSkillLevelUpReceiveTmp;
//	if (!u_PBSkillLevelUpReceiveTmp.ParseFromString(u_strBody))
//	{
//		g_cMyGlog.debugLog("u_PBSkillLevelUpReceiveTmp ParseFromString error\n");
//	}
//	g_cMyGlog.debugLog("skillUpResult:%d\n",u_PBSkillLevelUpReceiveTmp.skillupresult());
//	g_cMyGlog.debugLog("targetCardinfo:\n");
//	g_cProUserBattleResult.printCardInfo(u_PBSkillLevelUpReceiveTmp.targetcard());
//	g_cMyGlog.debugLog("AllCardinfo:\n");
//	g_cProUserBattleResult.printUserAllCardInfo(u_PBSkillLevelUpReceiveTmp.cards());
//	g_cMyGlog.debugLog("fightCapacity:%d\n",u_PBSkillLevelUpReceiveTmp.fightcapacity());
//#endif
	///////DE_BUG////test end
	u_retData = createData(nDataPacket,u_strBody);
//#ifdef DEBUG_MOD
//	g_cMyGlog.debugLog("headerLength:%d\n",sizeof(nDataPacket));
//	g_cMyGlog.debugLog("bodylength:%d\n",u_strBody.size());
//#endif
	return u_retData;
}

//卡牌编队
string cProClientMessage::procCardForm(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procCardForm error\n");
		return u_retData;
	}

	PBCardFormSend u_PBCardFormSend;
	if (!u_PBCardFormSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procCardForm ParseFromString error\n");
		return u_retData;
	}

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBCardFormReceive u_PBCardFormReceive;
	int u_result = -1;
	u_result = g_cProcUserCardForm.setPBCardFormReceive(u_PBCardFormReceive,u_strToken,u_PBCardFormSend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procCardForm cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBCardFormReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//副本掉落 
string cProClientMessage::procBattleLose(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procBattleLose111 error\n");
		return u_retData;
	}

	PBBattleSend u_PBBattleSend;
	if (!u_PBBattleSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procBattleLose ParseFromString error\n");
		return u_retData;
	}

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBBattleReceive u_PBBattleReceive;
	int u_result = -1;
	u_result = g_cProcUserBattle.setPBBattleReceive(u_PBBattleReceive,u_strToken,u_PBBattleSend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procBattleLose cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBBattleReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//副本结果
string cProClientMessage::procBattleResult(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procBattleResult error!\n");
		return u_retData;
	}

	PBBattleResultSend u_PBBattleResultSend;
	if (!u_PBBattleResultSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procBattleResult ParseFromString error\n");
		return u_retData;
	}
	

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBBattleResultReceive u_PBBattleResultReceive;
	int u_result = -1;
	u_result = g_cProUserBattleResult.setPBBattleResultReceive(u_PBBattleResultReceive,u_strToken,u_PBBattleResultSend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procBattleResult cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBBattleResultReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//贩卖卡牌
string cProClientMessage::procSoldCardResult(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("11procSoldCardResult error\n");
		return u_retData;
	}

	PBSoldCardSend u_PBSoldCardSend;
	if (!u_PBSoldCardSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("u_PBSoldCardSend ParseFromString error\n");
		return u_retData;
	}

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBSoldCardReceive u_PBSoldCardReceive;
	int u_result = -1;
	u_result = g_cProcUserSoldCard.setPBSoldCardReceive(u_PBSoldCardReceive,u_strToken,u_PBSoldCardSend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("u_PBSoldCardSend cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBSoldCardReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}


//获取友情值
string cProClientMessage::procGetFriendValue(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procGetFriendValue error\n");
		return u_retData;
	}

	PBFriendSend u_PBFriendSend;
	if (!u_PBFriendSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procGetFriendValue ParseFromString error\n");
		return u_retData;
	}

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBGetFriendValueReceive u_PBGetFriendValueReceive;
	int u_result = -1;
	u_result = g_cProcUserGetFriendValue.setPBGetFriendValueReceive(u_PBGetFriendValueReceive,u_strToken,u_PBFriendSend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procGetFriendValue cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBGetFriendValueReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//拒绝添加好友请求
string cProClientMessage::procUserFriendReject(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procUserFriendReject error\n");
		return u_retData;
	}

	PBFriendSend u_PBFriendSend;
	if (!u_PBFriendSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procUserFriendReject ParseFromString error\n");
		return u_retData;
	}

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBWaitingFriend u_PBWaitingFriend;
	int u_result = -1;
	u_result = g_cProcUserRejectFriend.setPBWaitingFriendReject(u_PBWaitingFriend,u_strToken,u_PBFriendSend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procUserFriendReject cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBWaitingFriend.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}


//商店购买请求
string cProClientMessage::procUserShopBuy(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procUserShopBuy error\n");
		return u_retData;
	}

	PBShopBuySend u_PBShopBuySend;
	if (!u_PBShopBuySend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procUserShopBuy ParseFromString error\n");
		return u_retData;
	}

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBShopBuySendReceive u_PBShopBuySendReceive;
	int u_result = -1;
	u_result = g_cProcUserShopBuy.setPBShopBuySendReceive(u_PBShopBuySendReceive,u_strToken,u_PBShopBuySend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procUserShopBuy cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBShopBuySendReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//搜索玩家请求
string cProClientMessage::procUserSearchPlayer(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procUserSearchPlayer error!\n");
		return u_retData;
	}

	PBSearchFriendSend u_PBSearchFriendSend;
	if (!u_PBSearchFriendSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procUserSearchPlayer ParseFromString error\n");
		return u_retData;
	}

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBSearchFriendReceive u_PBSearchFriendReceive;
	int u_result = -1;
	u_result = g_cProcUserSearchFriend.setPBSearchFriendReceive(u_PBSearchFriendReceive,u_strToken,u_PBSearchFriendSend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procUserSearchPlayer cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBSearchFriendReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//玩家注册检查请求
string cProClientMessage::procUserRegistCheck(const string& u_strClientData)
{
	//cerr<<"in procUserRegistCheck"<<endl;
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procUserRegistCheck error!\n");
		return u_retData;
	}

	PBRegisterCheckSend u_PBRegisterCheckSend;
	if (!u_PBRegisterCheckSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procUserRegistCheck ParseFromString error!\n");
		return u_retData;
	}

	/*string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);*/

	PBRegisterCheckReceive u_PBRegisterCheckReceive;
	int u_result = -1;
	u_result = g_cProcUserRegistCheck.setPBRegisterCheckReceive(u_PBRegisterCheckReceive,u_PBRegisterCheckSend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procUserRegistCheck cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBRegisterCheckReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//金币抽奖请求
string cProClientMessage::procUserGoldAward(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procUserGoldAward error!\n");
		return u_retData;
	}

	PBAwardSend u_PBAwardSend;
	if (!u_PBAwardSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procUserGoldAward ParseFromString error!\n");
		return u_retData;
	}


	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBAwardReceive u_PBAwardReceive;
	int u_result = -1;
	u_result = g_cProUserGoldAward.setPBAwardReceive(u_PBAwardReceive,u_strToken,u_PBAwardSend.awardtype());
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procUserGoldAward cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBAwardReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}

	u_retData = createData(nDataPacket,u_strBody);

	return u_retData;
}

//获取活动副本
string cProClientMessage::procUserActiveBattleReq(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBBattle u_PBBattle;
	int u_result = -1;
	u_result = g_cProcUserActiveBattle.getUserActiveBattle(u_PBBattle,u_strToken);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procUserActiveBattleReq cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBBattle.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//获取用户体力
string cProClientMessage::proUserGetEnergyReq(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBEnergyReceive u_PBEnergyReceive;
	int u_result = -1;
	u_result = g_cProcUserUseEnergy.getUserEnergy(u_PBEnergyReceive,u_strToken);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("g_cProcUserUseEnergy cut down connect!\n");
		return u_retData;
	}
	string u_strBody("");
	if (u_result == 0)
	{
		u_PBEnergyReceive.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//商店购买请求
string cProClientMessage::procUser91ShopBuy(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procUser91ShopBuy error\n");
		return u_retData;
	}

	PBIAPSend u_PBIAPSend;
	if (!u_PBIAPSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procUser91ShopBuy ParseFromString error\n");
		return u_retData;
	}

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBIAPRecv u_PBIAPRecv;
	int u_result = -1;
	u_result = g_cProcUserShopBuy.setPBIAPRecv(u_PBIAPRecv,u_strToken,u_PBIAPSend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procUser91ShopBuy cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBIAPRecv.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//排行榜请求
string cProClientMessage::procUserRankingReq(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());
	string strProtobufData = "";
	strProtobufData = u_strClientData.substr(sizeof(dataPacket),nDataPacket.protobufLength);

	if (strProtobufData.size() == 0)
	{
		g_cMyGlog.errorlogRecord("procUserRankingReq error\n");
		return u_retData;
	}

	PBRankSend u_PBRankSend;
	if (!u_PBRankSend.ParseFromString(strProtobufData))
	{
		g_cMyGlog.errorlogRecord("procUserRankingReq ParseFromString error\n");
		return u_retData;
	}

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBRankRecv u_PBRankRecv;
	int u_result = -1;
	u_result = g_cProcUserRankReq.setPBRankRecv(u_PBRankRecv,u_strToken,u_PBRankSend);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procUserRankingReq cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBRankRecv.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}

//获取通知请求
string cProClientMessage::procUserGetNoticeReq(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());

	string u_strToken("");
	u_strToken.append(nDataPacket.tokenValue,sizeof(nDataPacket.tokenValue) - 1);

	PBGameNotice u_PBGameNotice;
	int u_result = -1;
	u_result = g_cUserGetNotice.setPBGameNotice(u_PBGameNotice,u_strToken);
	if (u_result == -1)
	{
		g_cMyGlog.errorlogRecord("procUserGetNoticeReq cut down connect!\n");
		return u_retData;
	}

	string u_strBody("");
	if (u_result == 0)
	{
		u_PBGameNotice.SerializeToString(&u_strBody);
	}
	else
	{
		nDataPacket.nType = u_result;
	}
	u_retData = createData(nDataPacket,u_strBody);
	return u_retData;
}
//接收客户端消息处理并回应对应消息
string cProClientMessage::procClientReqData(const string& u_strClientData)
{
	string u_retData = "";
	dataPacket nDataPacket;
	string u_strDataPacket = u_strClientData.substr(0,sizeof(dataPacket));
	memcpy(&nDataPacket,u_strDataPacket.c_str(),u_strDataPacket.size());

#ifdef DEBUG_MOD
	g_cMyGlog.debugLog("procClientReqData:nType:%d\n",nDataPacket.nType);
#endif

	switch(nDataPacket.nType)
	{
		case REGISTER:
			u_retData = procRegister(u_strClientData);
			break;
		case LOGIN:
			u_retData = procLogin(u_strClientData);
			break;
		case ALLMISSION:
			u_retData = procAllmission(u_strClientData);
			break;
		case MISSIONAWARD:
			u_retData = procMissionAward(u_strClientData);
			break;
		case FRIEND:
			u_retData = procFriend(u_strClientData);
			break;
		case WATITINGFRIEND:
			u_retData = procWaitingFrind(u_strClientData);
			break;
		case ADDFRIEND:
			u_retData = procAddFriend(u_strClientData);
			break;
		case FRIENDAGREE:
			u_retData = procFriendAgree(u_strClientData);
			break;
		case FRIENDDELETE:
			u_retData = procFriendDelete(u_strClientData);
			break;
		/*case BUYENERGY:
			u_retData = procBuyEnergy(u_strClientData);
			break;*/
		case AWARDFRIEND:
			u_retData = procAwardFriend(u_strClientData);
			break;
		case GETFIEXDAFFICHE:
			u_retData = procGetFixedAffiche(u_strClientData);
			break;
		case RUNNINGAFFICHE:
			u_retData = procRunningAffiche(u_strClientData);
			break;
		case AWARDDIAMOND:
			u_retData = procAwarddiamond(u_strClientData);
			break;
		case HELPFRIEND:
			u_retData = procHelpFriend(u_strClientData);
			g_cMyGlog.debugLog("u_retData.size:%d\n",u_retData.size());
			break;
		case CARD_LEVLE_UP:
			u_retData = procCardLevelUp(u_strClientData);
			break;
		case CARD_EVOLUTION:
			u_retData = procCardEvolution(u_strClientData);
			break;
		case SKIIL_LEVEV_UP:
			u_retData = procSkillLevelUp(u_strClientData);
			break;
		case CARD_FORM:
			u_retData = procCardForm(u_strClientData);
			break;
		case BATTLE_LOSE:
			u_retData = procBattleLose(u_strClientData);
			break;
		case BATTLE_RESULT:
			u_retData = procBattleResult(u_strClientData);
			break;
		case SOLD_CARD:
			//g_cMyGlog.errorlogRecord("SOLD_CARD:%d\n",SOLD_CARD);
			u_retData = procSoldCardResult(u_strClientData);
			break;
		case GET_FRIEND_VALUE:
			u_retData = procGetFriendValue(u_strClientData);
			break;
		case REJECT_FRIEND_REQ:
			u_retData = procUserFriendReject(u_strClientData);
			break;
		case SHOP_BUY:
			//g_cMyGlog.errorlogRecord("proc SHOP_BUY:%d\n",SHOP_BUY);
			u_retData = procUserShopBuy(u_strClientData);
			break;
		case SEARCH_PLAYER:
			u_retData = procUserSearchPlayer(u_strClientData);
			break;
		case REGIST_CHECK:
			u_retData = procUserRegistCheck(u_strClientData);
			break;
		case GOLD_LOTTERY:
			u_retData = procUserGoldAward(u_strClientData);
			break;
		case ACTIVE_BATTLE_REQ:
			u_retData = procUserActiveBattleReq(u_strClientData);
			break;
		case GET_USER_ENERGY_REQ:
			u_retData = proUserGetEnergyReq(u_strClientData);
			break;
		case USER_BUY_RESULT_REQ:
			u_retData = procUser91ShopBuy(u_strClientData);
			break;
		case PLAYER_RANK_REQ:
			u_retData = procUserRankingReq(u_strClientData);
			break;
		case GET_NOTICE_REQ:
			u_retData = procUserGetNoticeReq(u_strClientData);
			break;
		default:
			g_cMyGlog.errorlogRecord("recvClientMessageAndSend unkown nType:%d\n",nDataPacket.nType);

	}
	
	return u_retData;
}

void cProClientMessage::unInit()
{

}


