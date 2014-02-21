#ifndef STRUCTDEF_H
#define STRUCTDEF_H
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <map>
using namespace std;

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

typedef map<int,int> mapRand;

typedef map<string,string> resourceDataMap;

#endif