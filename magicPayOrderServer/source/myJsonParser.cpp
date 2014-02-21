#include <stdio.h>
#include "../include/myJsonParser.h"
#include "../include/myGlog.h"
#include "../include/procUserShopBuy.h"
#include "../include/magicServerDb.h"





//cMyJsonParser g_cMyJsonParser;

cMyJsonParser::cMyJsonParser()
{

}

cMyJsonParser::~cMyJsonParser()
{

}

string cMyJsonParser::MyWriteJson(const string& u_data)
{
	string retStrJson = "";
	Json::Value root;
	root["receipt-data"] = u_data.c_str();
	//root.toStyledString();
	retStrJson = root.toStyledString();
#ifdef DEBUG_MOD
	cerr<<"u_data:"<<u_data<<endl;
	//g_cMyGlog.debugLog("u_data:%s\n",u_data.c_str());
	//g_cMyGlog.debugLog("retStrJson:%s\n",retStrJson.c_str());
#endif
	
	//cout << retStrJson << endl;
	return retStrJson;
}

string cMyJsonParser::getUserNameByUserId(const unsigned int& u_userId)
{
	string n_retUserName = "";
	char buf[128] = {0};
	sprintf(buf,"select userName from m_gameuserinfo where userID=%d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("getUserNameByUserId error11,strSql:%s\n",strSql.c_str());
	}
	n_retUserName = q.getStringFileds("userName");
	q.finalize();
	return n_retUserName;
}


//for Ios IAP
int cMyJsonParser::getBuyTypeByProcductId(const string& u_procductId)
{
	int u_retBuyType = 0;
	char buf[128] = {0};
	sprintf(buf,"select * from m_product_price where productId='%s';",u_procductId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("updateUserInfoByProcductId error,strSql:%s\n",strSql.c_str());
	}
	else
	{
		u_retBuyType = q.getIntFileds("buyType");
	}
	q.finalize();
	return u_retBuyType;
}

void cMyJsonParser::insertIOSOrder(const string& u_userAccount,const Json::Value& u_arrayObj/*,const string& u_serverUrl*/)
{
	char buf[1024] = {0};
	sprintf(buf,"insert into m_ios_order_table(Id,userAccount,transaction_id,product_id,\
				quantity,purchase_date,app_item_id,bid,bvrs,isProcced) \
				values(NULL,'%s','%s','%s','%s','%s','%s','%s','%s',%d);",u_userAccount.c_str(),\
				u_arrayObj["transaction_id"].asString().c_str(),u_arrayObj["product_id"].asString().c_str(),\
				u_arrayObj["quantity"].asString().c_str(),u_arrayObj["purchase_date"].asString().c_str(),\
				u_arrayObj["item_id"].asString().c_str(),u_arrayObj["bid"].asString().c_str(),\
				u_arrayObj["bvrs"].asString().c_str(),1);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("insertIOSOrder error,strSql:%s\n",strSql.c_str());
	}
}

int cMyJsonParser::procJsonData(const unsigned int& u_userId,const string& u_maccAddress,const string& u_jsonData/*,const string& u_serverUrl*/)
{
	using namespace std;

	Json::Reader reader;
	Json::Value value;
	int u_result = -1;
	//g_cMyGlog.debugLog("in procJsonData\n");
	if (reader.parse(u_jsonData, value))
	{
		int n_status = -1;
		n_status = value["status"].asInt();
		if (n_status != 0)
		{
			g_cMyGlog.errorlogRecord("n_status:%d\n",n_status);
			return u_result;
		}
		const Json::Value arrayObj = value["receipt"];
		int n_buyType = getBuyTypeByProcductId(arrayObj["product_id"].asString());
		if (n_buyType <= 0)
		{
			return u_result;
		}
		u_result = g_cProcUserShopBuy.proc91ShopBuy(u_userId,n_buyType);
		if (u_result == -1)
		{
			return u_result;
		}
		
		//string u_userName = getUserNameByUserId(u_userId);
		insertIOSOrder(u_maccAddress,arrayObj/*,u_serverUrl*/);
		
	/*	g_cMyGlog.debugLog("receipt data:\n");
		g_cMyGlog.debugLog("quantity:%s",arrayObj["quantity"].asString().c_str());
		g_cMyGlog.debugLog("product_id:%s",arrayObj["product_id"].asString().c_str());
		g_cMyGlog.debugLog("transaction_id:%s",arrayObj["transaction_id"].asString().c_str());
		g_cMyGlog.debugLog("purchase_date:%s",arrayObj["purchase_date"].asString().c_str());
		g_cMyGlog.debugLog("app_item_id:%s",arrayObj["item_id"].asString().c_str());
		g_cMyGlog.debugLog("bid:%s",arrayObj["bid"].asString().c_str());
		g_cMyGlog.debugLog("bvrs:%s",arrayObj["bvrs"].asString().c_str());*/
		
		
	}
	return u_result;
}