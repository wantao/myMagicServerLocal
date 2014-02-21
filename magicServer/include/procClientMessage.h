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
	//ע���봦��
	string procRegister(const string& u_strClientData);
	//��¼������
	string procLogin(const string& u_strClientData);
	//��ȡϵͳ��Ϣ������
	//string procSystemInfo(const string& u_strClientData);
	//��ȡ������Ϣ������
	string procAllmission(const string& u_strClientData);
	//��ȡ������������
	string procMissionAward(const string& u_strClientData);
	//��ȡ����������
	string procFriend(const string& u_strClientData);
	//��ȡ�ȴ�ȷ�ϵĺ���������
	string procWaitingFrind(const string& u_strClientData);
	//������Ӻ���������
	string procAddFriend(const string& u_strClientData);
	//ͬ����Ӻ���������
	string procFriendAgree(const string& u_strClientData);
	//ɾ�� ����������
	string procFriendDelete(const string& u_strClientData);
	//��������������
	string procBuyEnergy(const string& u_strClientData);
	//������Ӻ���������
	string procAwardFriend(const string& u_strClientData);
	//��ȡ�̶���������Ϣ������
	string procGetFixedAffiche(const string& u_strClientData);
	//��ȡ������������Ϣ������
	string procRunningAffiche(const string& u_strClientData);
	//��ʯ�齱������
	string procAwarddiamond(const string& u_strClientData);
	//��ȡ������ս����������
	string procHelpFriend(const string& u_strClientData);
	//�������� 
	string procCardLevelUp(const string& u_strClientData);
	//���ƺϳ�
	string procCardEvolution(const string& u_strClientData);
	//��������
	string procSkillLevelUp(const string& u_strClientData);
	//���Ʊ��
	string procCardForm(const string& u_strClientData);
	//�������� 
	string procBattleLose(const string& u_strClientData);
	//�������
	string procBattleResult(const string& u_strClientData);
	//��������
	string procSoldCardResult(const string& u_strClientData);
	//��ȡ����ֵ
	string procGetFriendValue(const string& u_strClientData);
	//��ȡ����ֵ
	string procUserFriendReject(const string& u_strClientData);
	//�̵깺������
	string procUserShopBuy(const string& u_strClientData);
	//�����������
	string procUserSearchPlayer(const string& u_strClientData);
	//���ע��������
	string procUserRegistCheck(const string& u_strClientData);
	//��ҳ齱����
	string procUserGoldAward(const string& u_strClientData);
	//��ȡ�����
	string procUserActiveBattleReq(const string& u_strClientData);
	//��ȡ�û�����
	string proUserGetEnergyReq(const string& u_strClientData);
	//�̵깺������
	string procUser91ShopBuy(const string& u_strClientData);
	//���а�����
	string procUserRankingReq(const string& u_strClientData);
	//��ȡ֪ͨ����
	string procUserGetNoticeReq(const string& u_strClientData);
	//bool Init();
	void unInit();
	//��ӿͻ����׽���
	//void addClient(unsigned int& clientFd);
	//���տͻ�����Ϣ������Ӧ��Ӧ��Ϣ
	string procClientReqData(const string& u_strClientData);
	string createData(dataPacket& u_dataPacket,const string& u_strBody);
	//����ProtobufData
	string recvProtobufData(unsigned int& clientFd,dataPacket& nDataPacket);

	//��װ����������
	bool createAndSendData(dataPacket& u_dataPacket,unsigned int& u_clientFd,string& u_strBody);

protected:
private:
	
};

extern cProClientMessage g_cProClientMessage;

#endif