#ifndef STRUCTDEF_H
#define STRUCTDEF_H
#include <iostream>
#include <memory.h>
#include <stdlib.h>
using namespace std;
typedef struct _serverInfo
{
	int id;
	char serverName[64];
	char serverUrl[128];
	_serverInfo()
	{
		id = 0;
		memset(serverName,0,sizeof(serverName));
		memset(serverUrl,0,sizeof(serverUrl));
	}
}serverInfo;

//�ͻ��˷��͹��������ݰ�ͷ��ʽ����=dataPacket+protobuf��
typedef struct _dataPacket
{
	int nType;//��������
	int protobufLength;
	char tokenValue[33];//32λtokenֵ
	_dataPacket()
	{
		nType = -1;
		protobufLength = 0;
		memset(tokenValue,0,sizeof(tokenValue));
	}
}dataPacket;

#endif