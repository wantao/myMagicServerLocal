#ifndef PROCUSERRESOURCEDOWNLOAD_H
#define PROCUSERRESOURCEDOWNLOAD_H

#include <iostream>
#include "../include/MagicCard.pb.h"
using namespace std;

class cProcUserResourceDownLoad
{
public:
	cProcUserResourceDownLoad();
	~cProcUserResourceDownLoad();
	//���س�ʼ��Դ����
	void InitLoadResourceData();
	//int convertGbk2Utf(string& str);
protected:
private:
};

extern cProcUserResourceDownLoad g_cProcUserResourceDownLoad;

#endif