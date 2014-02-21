#ifndef MYJSONPARSER_H
#define MYJSONPARSER_H

#include <string>
#include "../jsoncpp/include/json/json.h"

using namespace std;

class cMyJsonParser
{
public:
	cMyJsonParser();
	~cMyJsonParser();
	string MyWriteJson(const string& u_data);
	int procJsonData(const unsigned int& u_userId,const string& u_maccAddress,const string& u_jsonData);
	string getUserNameByUserId(const unsigned int& u_userId);
	//for Ios IAP
	int getBuyTypeByProcductId(const string& u_procductId);
	void insertIOSOrder(const string& u_userAccount,const Json::Value& u_arrayObj/*,const string& u_serverUrl*/);
private:
protected:
};

//extern cMyJsonParser g_cMyJsonParser;
#endif