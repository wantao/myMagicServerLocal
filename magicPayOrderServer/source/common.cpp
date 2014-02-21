#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include  <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <sys/sysinfo.h>
#include <arpa/inet.h>
#include "../include/common.h"
#include "../include/define.h"
#include "../include/magicServerDb.h"
#include "../include/myGlog.h"
#include <errno.h>

//本机IP
string g_strLocalHost = "";

//数据库主机
//string g_strDbHost = "rdsaivbifaivbif.mysql.rds.aliyuncs.com";
string g_strDbHost = "";//"121.199.57.88";
//string g_strDbHost = "183.136.239.68";//"121.199.57.88";
//从数据库主机
//string g_strDbReadHost = "183.136.239.68";//"121.199.57.88";//"121.199.23.192";
string g_strDbReadHost = "";//"121.199.57.88";
//数据库端口
int g_nDbPort = 3306;
//数据库名称
string g_strDatabase = "magic_card_server1_db";
//数据库用户名
string g_strUser = "root";
//string g_strUser = "wantao";
//数据库连接密码
string g_strPw = "wt315771557";


//根据时间字符串还原成秒数
unsigned long MakeTime(const string& strTime)
{
	unsigned long uTime = 0;
	if(strTime.size()>0)
	{
		struct tm newTime;

		std::string subString = strTime.substr(0,4);
		int year = atoi(subString.c_str());
		subString = strTime.substr(5,2);
		int month = atoi(subString.c_str());
		subString = strTime.substr(8,2);
		int day = atoi(subString.c_str());

		subString = strTime.substr(11,2);
		int hour = atoi(subString.c_str());
		subString = strTime.substr(14,2);
		int minute = atoi(subString.c_str());
		subString = strTime.substr(17,2);
		int second = atoi(subString.c_str());

		newTime.tm_year = year - 1900;
		newTime.tm_mon = month -1;
		newTime.tm_mday = day;
		newTime.tm_hour = hour;
		newTime.tm_min = minute;
		newTime.tm_sec = second;

		uTime = mktime(&newTime);
	}
	return uTime;
}

//取时间戳
unsigned int GetTimeStamp()
{
	return time(NULL);
}

//根据时间戳还原时间
string GetTime(long lTime)
{
	char buf[128];
	memset(buf, 0, sizeof(buf));
	struct tm *newTime,timenow;
	newTime = &timenow;
	localtime_r( &lTime,newTime );

	sprintf(buf, "%4d-%02d-%02d %02d:%02d:%02d", newTime->tm_year+1900, newTime->tm_mon+1, newTime->tm_mday, newTime->tm_hour, newTime->tm_min, newTime->tm_sec);
	std::string strTime(buf);
	return strTime;
}

//获取时间格式：2013 08 19
string getStringTime(long lTime,int nType)
{
	char buf[128];
	memset(buf, 0, sizeof(buf));
	struct tm *newTime,timenow;
	newTime = &timenow;
	localtime_r( &lTime,newTime );
	if (nType == 0)
	{
		sprintf(buf, "%4d %02d %02d", newTime->tm_year+1900, newTime->tm_mon+1, newTime->tm_mday);
	}
	std::string strTime(buf);
	return strTime;
}

//根据token获取UserId
bool getUserIdbyToken(const string& u_strToken,unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select userId from m_userTokenInfo where tokenValue='%s';",u_strToken.c_str());
	string strSql(buf);

	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		q.finalize();
		return false;
	}
	else
	{
		u_userId = q.getIntFileds("userId");
	}
	q.finalize();
	return true;
}

//获取网卡ip地址
string GetIpAddress(char* eth,int nIndex)
{
	int   sock;
	struct   sockaddr_in   sin;
	struct   ifreq   ifr;

	std::string strIpAddress="";

	sock   =   socket(AF_INET,   SOCK_DGRAM,   0);
	if   (sock   ==   -1)
	{
		perror("socket");
	}

	strncpy(ifr.ifr_name,   eth,   IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ   -   1]   =   0;

	if   (ioctl(sock,   SIOCGIFADDR,   &ifr)   <   0)
	{
		perror("ioctl");
	}
	memcpy(&sin,   &ifr.ifr_addr,   sizeof(sin));
	strIpAddress = inet_ntoa(sin.sin_addr);
	g_cMyGlog.normalLog("%s:   %s\n", eth, strIpAddress.c_str());

	if(nIndex == 1) //获取百兆网卡MTU数值
	{
		cerr<<"eth:"<<eth<<" strIpAddress:"<<strIpAddress<<endl;
	}

	if(sock >= 0)
		close(sock);
	return strIpAddress;
}

int getDbServerIp()
{
	FILE* fp = fopen("/home/wt/DbFile.txt","r");
	if(fp == NULL)
	{
		cerr<<"open DbFile error"<<endl;
		g_cMyGlog.errorlogRecord("open DbFile failure,errno:%d,error:%s\n",errno,strerror(errno));
		return -1;
	}
	char szbuf[1024] = {0};
	int n_readByte = fread(szbuf,1,1024,fp);
	if (n_readByte <= 0)
	{
		g_cMyGlog.errorlogRecord("getDbServerIp getnothing from DbFile\n");
		if (fp != NULL)
		{
			fclose(fp);
			fp = NULL;
		}
		return -1;
	}
	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}
	string u_strSzbuf(szbuf);
	
	string flag="+";
	int position=0;
	int i=0;
	int u_spacePosition[3] = {0};
	while((position=u_strSzbuf.find_first_of(flag,position))!=string::npos)
	{
		//position=s.find_first_of(flag,position);  
		u_spacePosition[i] = position;
		position++;
		i++;
	}
	string u_masterDbServer = u_strSzbuf.substr(0,u_spacePosition[i-1]);
	string u_masterDb = "masterDb:";
	g_strDbHost = u_masterDbServer.substr(u_masterDb.size(),u_masterDbServer.size()-u_masterDb.size());
	g_cMyGlog.normalLog("u_masterDbServerIp:%s\n",g_strDbHost.c_str());
	if (g_strDbHost.size() <= 0)
	{
		return -1;
	}

	string u_slaveDbServer = u_strSzbuf.substr(u_spacePosition[i-1]+1,u_strSzbuf.size()-u_spacePosition[i-1]-1);
	string u_slaveDb = "slaveDB:";
	g_strDbReadHost = u_slaveDbServer.substr(u_slaveDb.size(),u_slaveDbServer.size()-u_slaveDb.size());
	g_cMyGlog.normalLog("u_slaveDbServerIp:%s\n",g_strDbReadHost.c_str());
	if (g_strDbReadHost.size() <= 0)
	{
		return -1;
	}
	return  0;
}
//获取玩家名字
string getUserNameBasedUserId(const unsigned int& u_userId)
{
	string n_retUserName = "";
	char buf[128] = {0};
	sprintf(buf,"select userName from m_gameuserinfo where userID=%d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserNameById error11,strSql:%s\n",strSql.c_str());
	}
	n_retUserName = q.getStringFileds("userName");
	q.finalize();
	return n_retUserName;
}

unsigned int getUserIdByUserMacAddress(const string& u_userMacAddress)
{
	unsigned int u_userId = 0;
	char buf[128] = {0};
	sprintf(buf,"select userID from m_gameuserinfo where macAddress='%s';",u_userMacAddress.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserIdByUserMacAddress not find macAddress,strSql:%s\n",strSql.c_str());
	}
	u_userId = q.getIntFileds("userID");
	q.finalize();
	return u_userId;
}

/************************************************************************/
/* get payServerPort                                                                     */
/************************************************************************/
int getPayServerPortByPayServerUrl(const string& u_payServerUrl)
{
	char buf[128] = {0};
	sprintf(buf,"select payServerPort from allserverinfo where payServerUrl = '%s';",u_payServerUrl.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	int u_retPort = 0;
	if (!q.eof())
	{
		u_retPort = q.getIntFileds("payServerPort");
	}
	else
	{
		g_cMyGlog.errorlogRecord("getPayServerPortByPayServerUrl error,strSql:%s\n",strSql.c_str());
	}
	q.finalize();
	return u_retPort;
}


//获取用户已充值的魔石数量
int getUserRechargedDiamond(const unsigned int& u_userId)
{
	char buf[128] = {0};
	sprintf(buf,"select rechargedDiamond from m_gameuserinfo where userID=%d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	int u_retReChargedDiamond = 0;
	if (!q.eof())
	{
		u_retReChargedDiamond = q.getIntFileds("rechargedDiamond");
	}
	q.finalize();
	return u_retReChargedDiamond;
}

//更新用户已充值的魔石数量
void updateUserReChargedDiamondNum(const unsigned int& u_userId,const int& u_addDiamonds)
{
	int u_rechargedDiamond = 0;
	u_rechargedDiamond = getUserRechargedDiamond(u_userId);
	u_rechargedDiamond += u_addDiamonds;

	char buf[128] = {0};
	sprintf(buf,"update m_gameuserinfo set rechargedDiamond=%d where userID=%d;",u_rechargedDiamond,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUserReChargedDiamondNum error,strSql:%s\n",strSql.c_str());
	}
}

//获取用户已扫荡和已购买体力药水次数
void getUserWipeOutAndBuyEnergyCount(const unsigned int& u_userId,int& u_wipeOutCount,int& u_buyEnergyCount)
{
	char buf[128] = {0};
	sprintf(buf,"select wipeOutCount,buyEnergyCount from m_user_wipeout_enery_count%d where userId=%d;",\
		u_userId%20,u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		u_wipeOutCount = q.getIntFileds("wipeOutCount");
		u_buyEnergyCount = q.getIntFileds("buyEnergyCount");
	}
	else
	{
		u_wipeOutCount = 0;
		u_buyEnergyCount = 0;
	}
	q.finalize();
}

//为用户信息新增用户已充值的魔石数量，当日可扫荡次数，当日已购买体力药水次数
void newAddSetSomeUserInfo(PBUser* &u_PBUser,const unsigned int& u_userId)
{
	int u_wipeOutCount = 0,u_buyEnergyCount = 0,u_reChargedDiamond = 0;
	getUserWipeOutAndBuyEnergyCount(u_userId,u_wipeOutCount,u_buyEnergyCount);
	u_reChargedDiamond = getUserRechargedDiamond(u_userId);
	u_PBUser->set_buyenergytime(u_buyEnergyCount);
	u_PBUser->set_rechargeddiamond(u_reChargedDiamond);
	u_PBUser->set_wipeoutcount(u_wipeOutCount);
}