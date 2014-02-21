#include "../include/common.h"
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/magicServerDb.h"


//数据库主机
string g_strDbHost = "";//"121.199.57.88";//"rdsaivbifaivbif.mysql.rds.aliyuncs.com";
string g_strDbReadHost = "";
//string g_strDbHost = "183.136.239.68";
//数据库端口
int g_nDbPort = 3306;
//数据库名称
string g_strDatabase = "magic_card_server1_db";
//数据库用户名
string g_strUser = "root";
//数据库连接密码
string g_strPw = "wt315771557";

char buf[] = {
	'Q', 'A', 'Z', 'E', 'D', 'C', 'W', 'S', 'X', 'R', 'F', 'V', 'T',  
	'G', 'B', 'Y', 'H', 'N', 'U', 'J', 'M', 'I', 'K', 'O', 'L', 'P',  
	'q', 'a', 'z', 'w', 's', 'x', 'e', 'd', 'c', 'r', 'f', 'v', 't',  
	'g', 'b', 'y', 'h', 'n', 'u', 'j', 'm', 'i', 'k', 'o', 'l', 'p',  
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '@', '#', '$'
};

char randBuf[] = {
	'0','1','2','3','4','5','6','7','8','9',  
	'q','a','z','w','s','x','e','d','c','r',
	'f','v','t','g','b','y','h','n','u','j', 
	'm','i','k','o','l','p'			
};


//产生指定长度的邀请码
string generateInvitationCode(int u_length)
{
	string strToken;
	double random(double,double);
	struct timeval tv;

	gettimeofday(&tv,NULL);
	srand(tv.tv_usec+tv.tv_sec);
	for(int icnt = 0; icnt < u_length; ++icnt)
	{
		int i = int(random(0,36));
		if (i >= 36)
		{
			i = 35;
		}
		strToken += randBuf[i];
	}

	return strToken;
}
double random(double start, double end)
{
	return start+(end-start)*rand()/(RAND_MAX + 1.0);
}

//产生随机token
string generateToken()
{
	string strToken;
	double random(double,double);
	struct timeval tv;
	
	gettimeofday(&tv,NULL);
	srand(tv.tv_usec+tv.tv_sec);
	for(int icnt = 0; icnt < 32; ++icnt)
	{
		strToken += buf[int(random(0,64))];
	}

	return strToken;
}

int getPreMaxMissionId()
{
	FILE* fp = fopen("./preMaxMissionId.txt","r");
	if(fp == NULL)
	{
		cerr<<"open DbFile error"<<endl;
		return -1;
	}
	char szbuf[1024] = {0};
	fread(szbuf,1,1024,fp);
	int u_maxPreMissionId = 0;
	u_maxPreMissionId = atoi(szbuf);
	return u_maxPreMissionId;
}


int getDbServerIp()
{
	FILE* fp = fopen("./DbFile.txt","r");
	if(fp == NULL)
	{
		cerr<<"open DbFile error"<<endl;
		return -1;
	}
	char szbuf[1024] = {0};
	fread(szbuf,1,1024,fp);

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
	//g_cMyGlog.normalLog("u_masterDbServerIp:%s\n",g_strDbHost.c_str());
	if (g_strDbHost.size() <= 0)
	{
		return -1;
	}
	cerr<<"g_strDbHost:"<<g_strDbHost<<endl;
	string u_slaveDbServer = u_strSzbuf.substr(u_spacePosition[i-1]+1,u_strSzbuf.size()-u_spacePosition[i-1]-1);
	string u_slaveDb = "slaveDB:";
	g_strDbReadHost = u_slaveDbServer.substr(u_slaveDb.size(),u_slaveDbServer.size()-u_slaveDb.size());
	//g_cMyGlog.normalLog("u_slaveDbServerIp:%s\n",g_strDbReadHost.c_str());
	if (g_strDbReadHost.size() <= 0)
	{
		return -1;
	}
	return  0;
}


int getClassByFightCapacity(const int& u_fightCapacity)
{
	int u_retClass = 1;
	if (u_fightCapacity > 0 && u_fightCapacity <= 5000)
	{
		u_retClass = 1;
	}
	else if (u_fightCapacity > 5001 && u_fightCapacity <= 10000)
	{
		u_retClass = 2;
	}
	else if (u_fightCapacity > 10001 && u_fightCapacity <= 15000)
	{
		u_retClass = 3;
	}
	else if (u_fightCapacity > 15001 && u_fightCapacity <= 20000)
	{
		u_retClass = 4;
	}
	else if (u_fightCapacity > 20001 && u_fightCapacity <= 30000)
	{
		u_retClass = 5;
	}
	else if (u_fightCapacity > 30001 && u_fightCapacity <= 40000)
	{
		u_retClass = 6;
	}
	else if (u_fightCapacity > 40001 && u_fightCapacity <= 60000)
	{
		u_retClass = 7;
	}
	else if (u_fightCapacity > 60001 && u_fightCapacity <= 80000)
	{
		u_retClass = 8;
	}
	else if (u_fightCapacity > 80001 && u_fightCapacity <= 110000)
	{
		u_retClass = 9;
	}
	else if (u_fightCapacity > 110001)
	{
		u_retClass = 10;
	}
	return u_retClass;
}


bool updateUserFightCapacity(const unsigned int& u_userId,const int& u_userFigthCapacity,\
							 const int& u_userCapitorCardCapacity)
{
	int u_class = getClassByFightCapacity(u_userCapitorCardCapacity);
	char buf[512] = {0};
	sprintf(buf,"update m_user_fight_capacity set fightCapacity = %d,capitorFightCapacity = %d,class = %d \
				where userId = %d;",u_userFigthCapacity,u_userCapitorCardCapacity,u_class,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		printf("updateUserFightCapacity error,strSql:%s\n",strSql.c_str());
		return false;
	}
	return true;
}

void getUserFightCapacity(const unsigned int& u_userId,int& u_userFigthCapacity,\
						  int& u_userCapitorCardCapacity)
{
	char buf[256] = {0};
	sprintf(buf,"select * from m_user_fight_capacity where userId = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.execQuery(strSql);
	if (q.eof())
	{
		printf("getUserFightCapacity error,strSql:%s\n",strSql.c_str());
		return;
	}
	u_userFigthCapacity = q.getIntFileds("fightCapacity");
	u_userCapitorCardCapacity = q.getIntFileds("capitorFightCapacity");
	q.finalize();
	return;
}
