#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include "../include/common.h"
#include "../include/define.h"
#include "../include/magicServerDb.h"
#include "../include/myGlog.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>



//数据库主机
//string g_strDbHost = "rdsaivbifaivbif.mysql.rds.aliyuncs.com";
string g_strDbHost = "";//"121.199.57.88";
//从数据库主机
string g_strDbReadHost = "";//"121.199.23.192";
//数据库端口
int g_nDbPort = 3306;
//数据库名称
string g_strDatabase = "magic_card_server1_db";
//数据库用户名
string g_strUser = "root";
//string g_strUser = "wantao";
//数据库连接密码
string g_strPw = "wt315771557";

char buf[] = {
	'Q', 'A', 'Z', 'E', 'D', 'C', 'W', 'S', 'X', 'R', 'F', 'V', 'T',  
	'G', 'B', 'Y', 'H', 'N', 'U', 'J', 'M', 'I', 'K', 'O', 'L', 'P',  
	'q', 'a', 'z', 'w', 's', 'x', 'e', 'd', 'c', 'r', 'f', 'v', 't',  
	'g', 'b', 'y', 'h', 'n', 'u', 'j', 'm', 'i', 'k', 'o', 'l', 'p',  
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '@', '#', '$'
};

//资源文件大小
resourceDataMap g_resourceDataMap;

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

//产生一个1-5的随机整数
mapRand generateIntFiled()
{
	double random(double,double);
	mapRand mapRandObject;
	int randData = 0;
	while (mapRandObject.size() < INIT_CARD_COUNT)
	{
		struct timeval tv;
		gettimeofday(&tv,NULL);

		srand(tv.tv_usec+tv.tv_sec);
		randData = int(random(1,10));
		if (randData > 0)
		{
			mapRandObject.insert(make_pair(randData,INIT_CARD_COUNT));
		}
		
		
	}
	return mapRandObject;
}

//抽奖随即卡牌
int getOneRand()
{
	double random(double,double);
	int randData = 0;
	struct timeval tv;
	while (1)
	{
		gettimeofday(&tv,NULL);

		srand(tv.tv_usec+tv.tv_sec);
		randData = int(random(1,159));
		if (randData != 85 && randData != 86 \
			&& randData != 87 && randData != 88)
		{
			break;
		}
	}
	
	return randData;
}

//产生一个1000-10000的随机数
int monsterGetOneRand()
{
	double random(double,double);
	int randData = 0;
	struct timeval tv;
	gettimeofday(&tv,NULL);

	srand(tv.tv_usec+tv.tv_sec);
	randData = int(random(LOSE_MIN,LOSE_MAX));
	
	return randData;
}

//获取随即成长值
int getGrowValue(int u_min,int u_max)
{
	double random(double,double);
	int randData = 0;
	struct timeval tv;
	gettimeofday(&tv,NULL);

	srand(tv.tv_usec+tv.tv_sec);
	randData = int(random(u_min,u_max));
	return randData;
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


//获得成长档
int getGrow(const int& u_value)
{
	int u_finalGrowValue = 0;
	if (u_value == ONE_STAR)
	{
		u_finalGrowValue = getGrowValue(ONE_STAR_RAND_MIN,ONE_STAR_RAND_MAX);
	}
	else if (u_value == TWO_STAR)
	{
		u_finalGrowValue = getGrowValue(TWO_STAR_RAND_MIN,TWO_STAR_RAND_MAX);
	}
	else if (u_value == THREE_STAR)
	{
		u_finalGrowValue = getGrowValue(THREE_STAR_RAND_MIN,THREE_STAR_RAND_MAX);
	}
	else if (u_value == FOUR_STAR)
	{
		u_finalGrowValue = getGrowValue(FOUR_STAR_RAND_MIN,FOUR_STAR_RAND_MAX);
	}
	else if (u_value == FIVE_STAR)
	{
		u_finalGrowValue = getGrowValue(FIVE_STAR_RAND_MIN,FIVE_STAR_RAND_MAX);
	}
	else if (u_value == SIX_STAR)
	{
		u_finalGrowValue = getGrowValue(SIX_STAR_RAND_MIN,SIX_STAR_RAND_MAX);
	}
	else if (u_value == SEVEN_STAR)
	{
		u_finalGrowValue = getGrowValue(SEVEN_STAR_RAND_MIN,SEVEN_STAR_RAND_MAX);
	}
	else if (u_value == EIGHT_STAR)
	{
		u_finalGrowValue = getGrowValue(EIGHT_STAR_RAND_MIN,EIGHT_STAR_RAND_MAX);
	}
	else if (u_value == NINE_STAR)
	{
		u_finalGrowValue = getGrowValue(NINE_STAR_RAND_MIN,NINE_STAR_RAND_MAX);
	}
	else if (u_value == TEN_STAR)
	{
		u_finalGrowValue = getGrowValue(TEN_STAR_RAND_MIN,TEN_STAR_RAND_MAX);
	}
	else if (u_value == ELEVEN_STAR)
	{
		u_finalGrowValue = getGrowValue(ELEVEN_STAR_RAND_MIN,ELEVEN_STAR_RAND_MAX);
	}
	else if (u_value == TWELVE_STAR)
	{
		u_finalGrowValue = getGrowValue(TWELVE_STAR_RAND_MIN,TWELVE_STAR_RAND_MAX);
	}
	else if (u_value == THIRTEEN_STAR)
	{
		u_finalGrowValue = getGrowValue(THIRTEEN_STAR_RAND_MIN,THIRTEEN_STAR_RAND_MAX);
	}
	else if (u_value == FOURTEEN_STAR)
	{
		u_finalGrowValue = getGrowValue(FOURTEEN_STAR_RAND_MIN,FOURTEEN_STAR_RAND_MAX);
	}
	else if (u_value == FIFTEEN_STAR)
	{
		u_finalGrowValue = getGrowValue(FIFTEEN_STAR_RAND_MIN,FIFTEEN_STAR_RAND_MAX);
	}
	else if (u_value == SIXTEEN_STAR)
	{
		u_finalGrowValue = getGrowValue(SIXTEEN_STAR_RAND_MIN,SIXTEEN_STAR_RAND_MAX);
	}
	else if (u_value == SEVENTEEN_STAR)
	{
		u_finalGrowValue = getGrowValue(SEVENTEEN_STAR_RAND_MIN,SEVENTEEN_STAR_RAND_MAX);
	}
	else if (u_value == EIGHTEEN_STAR)
	{
		u_finalGrowValue = getGrowValue(EIGHTEEN_STAR_RAND_MIN,EIGHTEEN_STAR_RAND_MAX);
	}
	else if (u_value == NINETEEN_STAR)
	{
		u_finalGrowValue = getGrowValue(NINETEEN_STAR_RAND_MIN,NINETEEN_STAR_RAND_MAX);
	}
	else if (u_value == TWENTY_STAR)
	{
		u_finalGrowValue = getGrowValue(TWENTY_STAR_RAND_MIN,TWENTY_STAR_RAND_MAX);
	}
	return u_finalGrowValue;
}

//获得最大成长档 注册时为队长卡牌指定最大成长档
int getMaxGrow(const int& u_value)
{
	int u_finalGrowValue = 0;
	if (u_value == ONE_STAR)
	{
		u_finalGrowValue = ONE_STAR_RAND_MAX;
	}
	else if (u_value == TWO_STAR)
	{
		u_finalGrowValue = TWO_STAR_RAND_MAX;
	}
	else if (u_value == THREE_STAR)
	{
		u_finalGrowValue = THREE_STAR_RAND_MAX;
	}
	else if (u_value == FOUR_STAR)
	{
		u_finalGrowValue = FOUR_STAR_RAND_MAX;
	}
	else if (u_value == FIVE_STAR)
	{
		u_finalGrowValue = FIVE_STAR_RAND_MAX;
	}
	else if (u_value == SIX_STAR)
	{
		u_finalGrowValue = SIX_STAR_RAND_MAX;
	}
	else if (u_value == SEVEN_STAR)
	{
		u_finalGrowValue = SEVEN_STAR_RAND_MAX;
	}
	else if (u_value == EIGHT_STAR)
	{
		u_finalGrowValue = EIGHT_STAR_RAND_MAX;
	}
	else if (u_value == NINE_STAR)
	{
		u_finalGrowValue = NINE_STAR_RAND_MAX;
	}
	else if (u_value == TEN_STAR)
	{
		u_finalGrowValue = TEN_STAR_RAND_MAX;
	}
	else if (u_value == ELEVEN_STAR)
	{
		u_finalGrowValue = ELEVEN_STAR_RAND_MAX;
	}
	else if (u_value == TWELVE_STAR)
	{
		u_finalGrowValue = TWELVE_STAR_RAND_MAX;
	}
	else if (u_value == THIRTEEN_STAR)
	{
		u_finalGrowValue = THIRTEEN_STAR_RAND_MAX;
	}
	else if (u_value == FOURTEEN_STAR)
	{
		u_finalGrowValue = FOURTEEN_STAR_RAND_MAX;
	}
	else if (u_value == FIFTEEN_STAR)
	{
		u_finalGrowValue = FIFTEEN_STAR_RAND_MAX;
	}
	else if (u_value == SIXTEEN_STAR)
	{
		u_finalGrowValue = SIXTEEN_STAR_RAND_MAX;
	}
	else if (u_value == SEVENTEEN_STAR)
	{
		u_finalGrowValue = SEVENTEEN_STAR_RAND_MAX;
	}
	else if (u_value == EIGHTEEN_STAR)
	{
		u_finalGrowValue = EIGHTEEN_STAR_RAND_MAX;
	}
	else if (u_value == NINETEEN_STAR)
	{
		u_finalGrowValue = NINETEEN_STAR_RAND_MAX;
	}
	else if (u_value == TWENTY_STAR)
	{
		u_finalGrowValue = TWENTY_STAR_RAND_MAX;
	}
	return u_finalGrowValue;
}

//根据时间确定今天是星期几，时间格式2013 08 19
int getWeekDayByTime(const string& u_strTime)
{
	int u_finalWeedDay = 0;
	//int uWeekDay[] = {L_SUNDAY,L_MONDAY,L_TUESDAY,L_WEDNESDAY,L_THURSDAY,L_FRIDAY,L_SATURDAY};
	int u_year = atoi((u_strTime.substr(0,4)).c_str());
	int u_month = atoi((u_strTime.substr(5,2)).c_str());
	int u_day = atoi((u_strTime.substr(8,2)).c_str());
	if(u_month == 1|| u_month == 2)
	{
		u_month += 12;
		u_year = u_year - 1;
	}
	//基姆拉尔森计算公式计算今天是星期几（0-6代表星期一到星期日）
	u_finalWeedDay = (u_day+2*u_month+3*(u_month+1)/5+u_year+u_year/4-u_year/100+u_year/400)%7;
	//做一次转换（0-6代表星期日到星期一）
	u_finalWeedDay += 1;
	if(u_finalWeedDay == 7)
	{
		u_finalWeedDay = 0;
	}
	return u_finalWeedDay;
}

//获取文件内容
std::string GetFileDataByPath(std::string strResourcePath)
{
	std::string result;

	FILE* fp =NULL;
	fp = fopen(strResourcePath.c_str(),"rb");
	if(fp!=NULL)
	{
		struct stat s;
		stat(strResourcePath.c_str(), &s);
		int nBytesTotal = s.st_size;

		if(nBytesTotal > 0)
		{
			unsigned char pBuf[4096] = {0};
		
			int nBytesRead = 0;
			int nBytesLeft = nBytesTotal;
			while((nBytesRead = fread(pBuf,sizeof(unsigned char),4096,fp)) > 0 )
			{
				result.append((char*)pBuf,nBytesRead);

				nBytesLeft -= nBytesRead;

				if(nBytesLeft <= 0)
				{
					break;
				}
			}
		}
		fclose(fp);
	}
	return result;
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