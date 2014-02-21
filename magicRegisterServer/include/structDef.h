#ifndef STRUCTDEF_H
#define STRUCTDEF_H
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <map>
using namespace std;

//客户端发送过来的数据包头格式（包=dataPacket+protobuf）
typedef struct _dataPacket
{
	int nType;//请求类型
	int protobufLength;
	char tokenValue[33];//32位token值
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