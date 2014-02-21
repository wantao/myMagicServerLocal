##include <unistd.h>
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
#include "../include/errorCode.h"
#include <errno.h>


#define BP_T 5.0

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

cardInfoAuthMap g_cardInfoAuthMap;

char buf[] = {
	'Q', 'A', 'Z', 'E', 'D', 'C', 'W', 'S', 'X', 'R', 'F', 'V', 'T',  
	'G', 'B', 'Y', 'H', 'N', 'U', 'J', 'M', 'I', 'K', 'O', 'L', 'P',  
	'q', 'a', 'z', 'w', 's', 'x', 'e', 'd', 'c', 'r', 'f', 'v', 't',  
	'g', 'b', 'y', 'h', 'n', 'u', 'j', 'm', 'i', 'k', 'o', 'l', 'p',  
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '@', '#', '$'
};


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
	randData = int(random(u_min,u_max+1));
#ifdef DEBUG_MOD
	if (randData >= u_max)
	{
		g_cMyGlog.debugLog("randData:%d,u_max:%d\n",randData,u_max);
	}
#endif
	if (randData > u_max)
	{
		randData = u_max;
	}

	///////test for 测试数据 begin
	//randData = u_max;
	//g_cMyGlog.debugLog("finalData:%d\n",randData);
	////////test for 测试数据 end
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
	else if (u_value == TWENTY_ONE_STAR)
	{
		u_finalGrowValue = getGrowValue(TWENTY_ONE_STAR_RAND_MIN,TWENTY_ONE_STAR_RAND_MAX);
	}
	else if (u_value == TWENTY_TWO_STAR)
	{
		u_finalGrowValue = getGrowValue(TWENTY_TWO_STAR_RAND_MIN,TWENTY_TWO_STAR_RAND_MAX);
	}
	else if (u_value == TWENTY_THREE_STAR)
	{
		u_finalGrowValue = getGrowValue(TWENTY_THREE_STAR_RAND_MIN,TWENTY_THREE_STAR_RAND_MAX);
	}
	else if (u_value == TWENTY_FOUR_STAR)
	{
		u_finalGrowValue = getGrowValue(TWENTY_FOUR_STAR_RAND_MIN,TWENTY_FOUR_STAR_RAND_MAX);
	}
	else if (u_value == TWENTY_FIVE_STAR)
	{
		u_finalGrowValue = getGrowValue(TWENTY_FIVE_STAR_RAND_MIN,TWENTY_FIVE_STAR_RAND_MAX);
	}
	else if (u_value == TWENTY_SIX_STAR)
	{
		u_finalGrowValue = getGrowValue(TWENTY_SIX_STAR_RAND_MIN,TWENTY_SIX_STAR_RAND_MAX);
	}
	else if (u_value == TWENTY_SEVEN_STAR)
	{
		u_finalGrowValue = getGrowValue(TWENTY_SEVEN_STAR_RAND_MIN,TWENTY_SEVEN_STAR_RAND_MAX);
	}
	else if (u_value == TWENTY_EIGHT_STAR)
	{
		u_finalGrowValue = getGrowValue(TWENTY_EIGHT_STAR_RAND_MIN,TWENTY_EIGHT_STAR_RAND_MAX);
	}
	else if (u_value == TWENTY_NINE_STAR)
	{
		u_finalGrowValue = getGrowValue(TWENTY_NINE_STAR_RAND_MIN,TWENTY_NINE_STAR_RAND_MAX);
	}
	else if (u_value == THIRTY_STAR)
	{
		u_finalGrowValue = getGrowValue(THIRTY_STAR_RAND_MIN,THIRTY_STAR_RAND_MAX);
	}
	else if (u_value == THIRTY_ONE_STAR)
	{
		u_finalGrowValue = getGrowValue(THIRTY_ONE_STAR_RAND_MIN,THIRTY_ONE_STAR_RAND_MAX);
	}
	else if (u_value == THIRTY_TWO_STAR)
	{
		u_finalGrowValue = getGrowValue(THIRTY_TWO_STAR_RAND_MIN,THIRTY_TWO_STAR_RAND_MAX);
	}
	else if (u_value == THIRTY_THREE_STAR)
	{
		u_finalGrowValue = getGrowValue(THIRTY_THREE_STAR_RAND_MIN,THIRTY_THREE_STAR_RAND_MAX);
	}
	else if (u_value == THIRTY_FOUR_STAR)
	{
		u_finalGrowValue = getGrowValue(THIRTY_FOUR_STAR_RAND_MIN,THIRTY_FOUR_STAR_RAND_MAX);
	}
	else if (u_value == THIRTY_FIVE_STAR)
	{
		u_finalGrowValue = getGrowValue(THIRTY_FIVE_STAR_RAND_MIN,THIRTY_FIVE_STAR_RAND_MAX);
	}
	else if (u_value == THIRTY_SIX_STAR)
	{
		u_finalGrowValue = getGrowValue(THIRTY_SIX_STAR_RAND_MIN,THIRTY_SIX_STAR_RAND_MAX);
	}
	else if (u_value == THIRTY_SEVEN_STAR)
	{
		u_finalGrowValue = getGrowValue(THIRTY_SEVEN_STAR_RAND_MIN,THIRTY_SEVEN_STAR_RAND_MAX);
	}
	else if (u_value == THIRTY_EIGHT_STAR)
	{
		u_finalGrowValue = getGrowValue(THIRTY_EIGHT_STAR_RAND_MIN,THIRTY_EIGHT_STAR_RAND_MAX);
	}
	else if (u_value == THIRTY_NINE_STAR)
	{
		u_finalGrowValue = getGrowValue(THIRTY_NINE_STAR_RAND_MIN,THIRTY_NINE_STAR_RAND_MAX);
	}
	else if (u_value == FOURTY_STAR)
	{
		u_finalGrowValue = getGrowValue(FOURTY_STAR_RAND_MIN,FOURTY_STAR_RAND_MAX);
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

//根据卡牌星级获取卡牌领导力
int getCardLeaderValueByStarLevel(const int& u_starLevel)
{
	int u_retLeaderValue = LEADER_VALUE_OF_ONE_STAR;
	if(u_starLevel == 1)
	{
		u_retLeaderValue = LEADER_VALUE_OF_ONE_STAR;
	}
	else if (u_starLevel == 2)
	{
		u_retLeaderValue = LEADER_VALUE_OF_TWO_STAR;
	}
	else if (u_starLevel == 3)
	{
		u_retLeaderValue = LEADER_VALUE_OF_THREE_STAR;
	}
	else if (u_starLevel == 4)
	{
		u_retLeaderValue = LEADER_VALUE_OF_FOUR_STAR;
	}
	else if (u_starLevel == 5)
	{
		u_retLeaderValue = LEADER_VALUE_OF_FIVE_STAR;
	}
	else if (u_starLevel == 6)
	{
		u_retLeaderValue = LEADER_VALUE_OF_SIX_STAR;
	}
	return u_retLeaderValue;
}

//成长档转化为BP
static float growNumToBP[] = {
	0.00f, 0.04f, 0.08f, 0.12f, 0.16f, 0.205f, 0.25f, 0.29f, 0.33f, 0.37f, 0.415f, 0.46f, 0.50f, 0.54f, 0.58f, 0.625f, 0.67f, 0.71f, 0.75f, 0.79f, 0.835f, 0.88f, 0.92f, 0.96f, 1.00f, 1.045f, 1.09f, 1.13f, 1.17f, 1.21f, 1.255f, 1.30f, 1.34f, 1.38f, 1.42f, 1.465f, 1.51f, 1.55f, 1.59f, 1.63f, 1.675f, 1.72f, 1.76f, 1.80f, 1.84f, 1.885f, 1.93f, 1.97f, 2.01f, 2.05f, 2.095f
};

//获取BP
float getCardBPByGrowAndRank(int _growNum, int rank)
{
	int growNum = _growNum;
	if(growNum > 100)
	{
		growNum = 100;
	}
	if(growNum < 0)
	{
		growNum = 0;
	}
	if(growNum > 50)
	{
		return (0.2f*growNum + 2 + growNumToBP[50]*(rank - 1) + growNumToBP[growNum - 50]*(rank - 1));
	}
	else
	{
		return (0.2f*growNum + 2 + growNumToBP[growNum]*(rank - 1));
	}
}

//通过BP成长和卡片等级得到卡片属性
CardAttribute getCardAttributeByCardBaseStatusGrowAndRank(CardBaseStatusGrow _baseStatusGrow, int rank)
{
	CardBaseStatus cbs;
	cbs.VTL = getCardBPByGrowAndRank(_baseStatusGrow.VTLGrow, rank);
	cbs.STR = getCardBPByGrowAndRank(_baseStatusGrow.STRGrow, rank);
	cbs.TGH = getCardBPByGrowAndRank(_baseStatusGrow.TGHGrow, rank);
	cbs.QUI = getCardBPByGrowAndRank(_baseStatusGrow.QUIGrow, rank);
	cbs.MGC = getCardBPByGrowAndRank(_baseStatusGrow.MGCGrow, rank);

	float t1,t2,t3,t4,t5,t6,t7;
	t1 = t2 = t3 = t4 = t5 = BP_T;
	t6 = t7 = 1.0f;
	CardAttribute attribute;
	attribute.LP =  20  + cbs.VTL*8*t1    + cbs.STR*2*t1    + cbs.TGH*3*t1    + cbs.QUI*3*t1    + cbs.MGC*1*t1;
	attribute.FP =  20  + cbs.VTL*1*t2    + cbs.STR*2*t2    + cbs.TGH*2*t2    + cbs.QUI*2*t2    + cbs.MGC*10*t2;
	attribute.ATK = 20  + cbs.VTL*0.1f*t3 + cbs.STR*2*t3    + cbs.TGH*0.2f*t3 + cbs.QUI*0.2f*t3 + cbs.MGC*0.1f*t3;
	attribute.DEF = 20  + cbs.VTL*0.1f*t4 + cbs.STR*0.2f*t4 + cbs.TGH*2.0f*t4 + cbs.QUI*0.2f*t4 + cbs.MGC*0.1f*t4;
	attribute.AGL = 20  + cbs.VTL*0.1f*t5 + cbs.STR*0.2f*t5 + cbs.TGH*0.2f*t5 + cbs.QUI*2*t5    + cbs.MGC*0.1f*t5;
	attribute.MND = 100 - cbs.VTL*0.3f*t6 - cbs.STR*0.1f*t6 + cbs.TGH*0.2f*t6 - cbs.QUI*0.1f*t6 + cbs.MGC*0.8f*t6;
	attribute.VCY = 100 + cbs.VTL*0.8f*t7 - cbs.STR*0.1f*t7 - cbs.TGH*0.1f*t7 + cbs.QUI*0.2f*t7 - cbs.MGC*0.3f*t7;
	return attribute;
}

//根据卡牌属性获取卡牌战斗力
int getFightCapacityByCardAttribute(const CardAttribute& u_CardAttribute)
{
	int u_fightCapacity = 0;
	u_fightCapacity = u_CardAttribute.ATK + u_CardAttribute.LP%5 + u_CardAttribute.DEF*1.2 \
		+ u_CardAttribute.ATK*1.5;
	if (u_CardAttribute.VCY <= 100)
	{
		u_fightCapacity += 10;
	}
	else
	{
		u_fightCapacity += 10;
		u_fightCapacity += (u_CardAttribute.VCY/100)*10;
	}
	if (u_CardAttribute.MND <= 100)
	{
		u_fightCapacity += 10;
	}
	else
	{
		u_fightCapacity += 10;
		u_fightCapacity += (u_CardAttribute.MND/100)*10;
	}
	return u_fightCapacity;
}

//根据技能编号和技能等级获取卡牌战斗力
int getFightCapacityBySkillCodeAndSkillLevel(const string& u_skillCode,const int& u_skillLevel)
{
	int u_fightCapacity = 0;
	char buf[128] = {0};
	sprintf(buf,"select * from skill_fight_capacity where skillCode='%s';",u_skillCode.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		if (atoi(u_skillCode.c_str()) != 0)
		{
			g_cMyGlog.debugLog("getFightCapacityBySkillCodeAndSkillLevel error,strSql:%s\n",strSql.c_str());
		}
		q.finalize();
		return u_fightCapacity;
	}
	u_fightCapacity = q.getIntFileds("baseFightCapacity") + u_skillLevel*(q.getIntFileds("addFightCapacityPerLevel"));
	q.finalize();
	return u_fightCapacity;
}

void getSkillCodeByCardId(const string& u_strCardId,string& u_passiveSkillCode,string& u_activeSkillCode)
{
	char buf[256] = {0};
	sprintf(buf,"select PassiveSkill,ActiveSkill from cardinfoinput where Id='%s';",u_strCardId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.debugLog("getSkillCodeByCardId error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return;
	}
	u_passiveSkillCode = q.getStringFileds("PassiveSkill");
	u_activeSkillCode = q.getStringFileds("ActiveSkill");
	q.finalize();
	return;
}

int getUserFightCapacity(const int& u_userId,int& u_userCapitorFightCapacity)
{
	int u_userFightCapacity = 0;
	char buf[256] = {0};
	sprintf(buf,"select * from m_usercardinfo%d where userID = %d and battlePosition != -1;",\
		u_userId%USER_CARD_TABLE_COUNT,u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserFightCapacity error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return u_userFightCapacity;
	}
	while (!q.eof())
	{
		CardBaseStatusGrow u_CardBaseStatusGrow;
		u_CardBaseStatusGrow.MGCGrow = q.getIntFileds("MGCGrow");
		u_CardBaseStatusGrow.QUIGrow = q.getIntFileds("QUIGrow");
		u_CardBaseStatusGrow.STRGrow = q.getIntFileds("STRGrow");
		u_CardBaseStatusGrow.TGHGrow = q.getIntFileds("TGHGrow");
		u_CardBaseStatusGrow.VTLGrow = q.getIntFileds("VTLGrow");
		CardAttribute u_CardAttribute = getCardAttributeByCardBaseStatusGrowAndRank(u_CardBaseStatusGrow,q.getIntFileds("cardLevel"));
		int u_attributeFightCapacity = getFightCapacityByCardAttribute(u_CardAttribute);
		string u_passSkillCode = "",u_actvieSkillCode = "";
		getSkillCodeByCardId(q.getStringFileds("cardId"),u_passSkillCode,u_actvieSkillCode);
		int u_passSkillFightCapacity = 0;
		int u_activeSkillFightCapacity = 0;
		if (u_actvieSkillCode.size() > 0)
		{
			u_activeSkillFightCapacity = getFightCapacityBySkillCodeAndSkillLevel(u_actvieSkillCode,q.getIntFileds("cardLevel"));
		}
		if (u_passSkillCode.size() > 0)
		{
			u_passSkillFightCapacity = getFightCapacityBySkillCodeAndSkillLevel(u_passSkillCode,q.getIntFileds("cardLevel"));
		}
		u_userFightCapacity += u_attributeFightCapacity + u_activeSkillFightCapacity + u_passSkillFightCapacity;
		if (q.getIntFileds("battlePosition") == 0)
		{
			u_userCapitorFightCapacity = u_attributeFightCapacity + u_activeSkillFightCapacity + u_passSkillFightCapacity;
		}
		q.nextRow();
	}
	q.finalize();
	return u_userFightCapacity;
}

void insertUserFightCapacity(const unsigned int& u_userId,const int& u_userFigthCapacity,\
							 const int& u_userCapitorCardCapacity)
{
	int u_class = getClassByFightCapacity(u_userCapitorCardCapacity);
	char buf[512] = {0};
	sprintf(buf,"insert into m_user_fight_capacity(userId,fightCapacity,capitorFightCapacity,class) \
				values(%d,%d,%d,%d);",u_userId,u_userFigthCapacity,u_userCapitorCardCapacity,u_class);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("insertUserFightCapacity error,strSql:%s\n",strSql.c_str());
		return;
	}
}

void updateUserFightCapacity(const unsigned int& u_userId,const int& u_userFigthCapacity,\
							 const int& u_userCapitorCardCapacity)
{
	int u_class = getClassByFightCapacity(u_userCapitorCardCapacity);
	char buf[512] = {0};
	sprintf(buf,"update m_user_fight_capacity set fightCapacity = %d,capitorFightCapacity = %d,class = %d \
				where userId = %d;",u_userFigthCapacity,u_userCapitorCardCapacity,u_class,u_userId);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUserFightCapacity error,strSql:%s\n",strSql.c_str());
		return;
	}
}

void getUserFightCapacity(const unsigned int& u_userId,int& u_userFigthCapacity,\
						 int& u_userCapitorCardCapacity)
{
	char buf[256] = {0};
	sprintf(buf,"select * from m_user_fight_capacity where userId = %d;",u_userId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getUserFightCapacity error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return;
	}
	u_userFigthCapacity = q.getIntFileds("fightCapacity");
	u_userCapitorCardCapacity = q.getIntFileds("capitorFightCapacity");
	q.finalize();
	return;
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
	g_cMyGlog.errorlogRecord("%s:   %s\n", eth, strIpAddress.c_str());

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
	g_cMyGlog.errorlogRecord("u_masterDbServerIp:%s\n",g_strDbHost.c_str());
	if (g_strDbHost.size() <= 0)
	{
		return -1;
	}

	string u_slaveDbServer = u_strSzbuf.substr(u_spacePosition[i-1]+1,u_strSzbuf.size()-u_spacePosition[i-1]-1);
	string u_slaveDb = "slaveDB:";
	g_strDbReadHost = u_slaveDbServer.substr(u_slaveDb.size(),u_slaveDbServer.size()-u_slaveDb.size());
	g_cMyGlog.errorlogRecord("u_slaveDbServerIp:%s\n",g_strDbReadHost.c_str());
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


int getRandData(int u_randMix,int u_randMax)
{
	double random(double,double);
	int randData = 1;
	struct timeval tv;
	gettimeofday(&tv,NULL);

	srand(tv.tv_usec+tv.tv_sec);
	randData = int(random(u_randMix,u_randMax));
	return randData;
}

//获取通知起止时刻点
int getNoticeTimePoint()
{
	int n_retNoticeTimePoint = 0;
	char buf[128] = {0};
	sprintf(buf,"select * from notice_time_point;");
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		g_cMyGlog.errorlogRecord("getNoticeTimePoint error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return n_retNoticeTimePoint;
	}
	n_retNoticeTimePoint = q.getIntFileds("noticeTimePoint");
	q.finalize();
	return n_retNoticeTimePoint;
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

//插入高星级卡牌通知
void insertHighStarCardNotice(const unsigned int& u_userId,const string& u_cardId,const int& u_cardStar)
{
	string n_UserName = getUserNameBasedUserId(u_userId);
	if (n_UserName.size() > 0)
	{
		int n_timeStamp = GetTimeStamp();
		char buf[512] = {0};
		sprintf(buf,"insert into high_star_card_got_notice(userName,cardId,cardStar,createTime) \
					values('%s','%s',%d,%d);",n_UserName.c_str(),u_cardId.c_str(),u_cardStar,n_timeStamp);
		string strSql(buf);
		if (g_cMagicServerDb.execSQL(strSql) != 0)
		{
			g_cMyGlog.errorlogRecord("insertHighStarCardNotice error,strSql:%s\n",strSql.c_str());
		}
	}
	return;
}

int getFightCapacityNoticeCount(const unsigned int& u_userId,const int& n_fightCapacityType)
{
	char buf[128] = {0};
	sprintf(buf,"select noticeCount from m_fight_capacity_notice_record where userId=%d and fightCapacityType=%d;",\
		u_userId,n_fightCapacityType);
	string strSql(buf);
	int n_retNoticeCount = 0;
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
#ifdef DEBUG_MOD
		g_cMyGlog.debugLog("getFightCapacityNoticeCount error,strSql:%s\n",strSql.c_str());
#endif
	}
	else
	{
		n_retNoticeCount = q.getIntFileds("noticeCount");
	}
	q.finalize();
	return n_retNoticeCount;
}

void insertFightCapacityNoticeCount(const unsigned int& u_userId,const int& u_fightCapacityType,const int& u_noticeCount)
{
	char buf[256] = {0};
	sprintf(buf,"insert into m_fight_capacity_notice_record(userId,fightCapacityType,noticeCount) \
				values(%d,%d,%d);",u_userId,u_fightCapacityType,u_noticeCount);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("insertFightCapacityNoticeCount error,strSql:%s\n",strSql.c_str());
	}
	return;
}

void insertFightCapacityBreakThroughNotice(const unsigned int& u_userId,const int& u_userFightCapacity)
{
	char buf[256] = {0};
	string n_userName = getUserNameBasedUserId(u_userId);
	int n_timeStamp = GetTimeStamp();
	sprintf(buf,"insert into fight_capacity_breakthrough_notice(userName,fightCapacity,createTime) \
				values('%s',%d,%d);",n_userName.c_str(),u_userFightCapacity,n_timeStamp);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("insertFightCapacityBreakThroughNotice error,strSql:%s\n",strSql.c_str());
	}
	return;
}

void procFightCapacityBreakThrough(const unsigned int& u_userId,const int& u_userFightCapacity)
{
	int n_fightCapacityType = 0;
	if (u_userFightCapacity > 100000)
	{
		if (u_userFightCapacity >= 100000 && u_userFightCapacity < 200000)
		{
			n_fightCapacityType = 1;	
		}
		else if (u_userFightCapacity >= 200000 && u_userFightCapacity < 300000)
		{
			n_fightCapacityType = 2;
		}
		else
		{
			n_fightCapacityType = 3;
		}
		int n_noticeCount = getFightCapacityNoticeCount(u_userId,n_fightCapacityType);
		if (n_noticeCount == 0)
		{
			n_noticeCount += 1;
			insertFightCapacityNoticeCount(u_userId,n_fightCapacityType,n_noticeCount);
			insertFightCapacityBreakThroughNotice(u_userId,u_userFightCapacity);
		}
	}
}

void procEvolutionSuccessNotice(const unsigned int& u_userId,const string& u_strCardId,const string& u_EvolutionId)
{
	char buf[256] = {0};
	string n_userName = getUserNameBasedUserId(u_userId);
	int n_timeStamp = GetTimeStamp();
	sprintf(buf,"insert into evolution_success_notice(userName,preCardId,cardId,createTime) \
				values('%s','%s','%s',%d);",n_userName.c_str(),u_strCardId.c_str(),u_EvolutionId.c_str(),n_timeStamp);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("procEvolutionSuccessNotice error,strSql:%s\n",strSql.c_str());
	}
	return;
}


//根据u_battleId，判断该关是否为大关的最后一小关，并带出大关编号和大关名字
bool isBigChapterLastLevel(const string& u_battleId,string& u_bigChapterCode,string& u_bigChapterName)
{
	char buf[128] = {0};
	sprintf(buf,"select * from big_chapter_table where levelLast='%s';",u_battleId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		//g_cMyGlog.errorlogRecord("isBigChapterLastLevel error,strSql:%s\n",strSql.c_str());
		q.finalize();
		return false;
	}
	else
	{
		u_bigChapterCode = q.getStringFileds("levelCode");
		u_bigChapterName = q.getStringFileds("levelName");
	}
	q.finalize();
	return true;
}

//获取通关该大关的名次
int getRankByBigChapterCode(const string& u_bigChapterCode)
{
	char buf[128] = {0};
	sprintf(buf,"select max(levelRank) from pass_level_notice where levelCode='%s';",u_bigChapterCode.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	int n_retRank = 0;
	if (q.eof())
	{
		q.finalize();
		return n_retRank;
	}
	n_retRank = q.getIntFileds("max(levelRank)");
	q.finalize();
	return n_retRank;
}

//获取通过大关卡的公告次数
int getPassBigLevelNoticeCount(const string& u_bigLevelCode,const unsigned int& u_userId)
{
	char buf[256] = {0};
	sprintf(buf,"select noticeCount from m_pass_level_notice_record where userId=%d and levelCode='%s';",\
		u_userId,u_bigLevelCode.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	int n_retNoticeCount = 0;
	if (q.eof())
	{
		q.finalize();
		return n_retNoticeCount;
	}
	n_retNoticeCount = q.getIntFileds("noticeCount");
	q.finalize();
	return n_retNoticeCount;
	
}

//插入通过大关卡通知次数
void insertPassBigLevelNoticedCount(const unsigned int& u_userId,const string& u_bigLevelCode,const int& u_count)
{
	char buf[256] = {0};
	sprintf(buf,"insert into m_pass_level_notice_record(userId,levelCode,noticeCount) values(%d,'%s',%d);",\
		u_userId,u_bigLevelCode.c_str(),u_count);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("insertPassBigLevelNoticedCount error,strSql:%s\n",strSql.c_str());
	}
	return;
}
void procInsertPassLevelNotice(const string& u_battleId,const unsigned int& u_userId)
{
	string n_bigChapterCode = "";
	string n_bigChapterName = "";
	if(isBigChapterLastLevel(u_battleId,n_bigChapterCode,n_bigChapterName))
	{
		int n_passBigLevelNoticeCount = getPassBigLevelNoticeCount(n_bigChapterCode,u_userId);
		if (n_passBigLevelNoticeCount == 0)
		{
			int n_rank = getRankByBigChapterCode(n_bigChapterCode);
			n_rank += 1;
			//只存储每大关的前10名
			if (n_rank > 10)
			{
				return;
			}
			string n_userName = getUserNameBasedUserId(u_userId);
			int n_timeStamp = GetTimeStamp();
			char buf[512] = {0};
			sprintf(buf,"insert into pass_level_notice(userName,levelRank,levelCode,\
						levelName,createTime) values('%s',%d,'%s','%s',%d);",n_userName.c_str(),\
						n_rank,n_bigChapterCode.c_str(),n_bigChapterName.c_str(),n_timeStamp);
			string strSql(buf);
			if (g_cMagicServerDb.execSQL(strSql) != 0)
			{
				g_cMyGlog.errorlogRecord("procInsertPassLevelNotice insert error,strSql:%s\n",strSql.c_str());
				return;
			}
			n_passBigLevelNoticeCount += 1;
			insertPassBigLevelNoticedCount(u_userId,n_bigChapterCode,n_passBigLevelNoticeCount);
		}
	}
	return;
}

///用户碎片信息增删查///begin
int getUserFragmentCount(const unsigned int& u_userId,const int& u_type,const string& u_strId)
{
	int u_retFragmentCount = 0;
	char buf[256] = {0};
	sprintf(buf,"select count from m_userequipinfo%d where userId=%d and equipTypeId = %d and equipMentId='%s'",\
		u_userId%USER_EQUIP_TABLE_COUNT,u_userId,u_type,u_strId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (q.eof())
	{
		u_retFragmentCount = 0;
		q.finalize();
		return u_retFragmentCount;
	}
	u_retFragmentCount = q.getIntFileds("count");
	q.finalize();
	return u_retFragmentCount;
}

void insertUserFragment(const unsigned int& u_userId,const int& u_type,\
					   const string& u_strId,const int& u_count)
{
	char buf[512] = {0};
	sprintf(buf,"insert into m_userEquipInfo%d(userId,equipTypeId,equipMentId,count) \
				values(%d,%d,'%s',%d);",u_userId%USER_EQUIP_TABLE_COUNT,u_userId,u_type,\
				u_strId.c_str(),u_count);
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("insertUserFragment error,strSql:%s\n",strSql.c_str());
	}
	return;
}

void updateUserFragment(const unsigned int& u_userId,const int& u_type,\
					   const string& u_strId,const int& u_count)
{
	char buf[512] = {0};
	sprintf(buf,"update m_userEquipInfo%d set count = %d where userId=%d and \
				equipTypeId = %d and equipMentId='%s';",\
				u_userId%USER_EQUIP_TABLE_COUNT,u_count,u_userId,u_type,\
				u_strId.c_str());
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUserFragment error,strSql:%s\n",strSql.c_str());
	}
	return;
} 

void addUserFragmentInfo(const unsigned int& u_userId,const int& u_type,const string& u_strId,const int& u_addCount)
{
	int u_userNowCount = getUserFragmentCount(u_userId,u_type,u_strId);
	int u_userAddAfterCount = u_userNowCount + u_addCount;
	
	if (u_userNowCount == 0)
	{
		insertUserFragment(u_userId,u_type,u_strId,u_userAddAfterCount);
	}
	else
	{
		updateUserFragment(u_userId,u_type,u_strId,u_userAddAfterCount);
	}

	return;
	
}

void cleanUserFragmentInfo(const unsigned int& u_userId,const int& u_type,const string& u_strId)
{
	char buf[128] = {0};
	sprintf(buf,"delete from m_userequipinfo%d  where userId = %d and equipMentId = '%s';", \
		u_userId%USER_EQUIP_TABLE_COUNT,u_userId,u_strId.c_str());
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("cleanUserFragmentInfo failure,strSql:%s\n",strSql.c_str());
	}
	return;
}

void deleteUserFragmentInfo(const unsigned int& u_userId,const int& u_type,const string& u_strId,const int& u_deleteCount)
{
	int u_userNowCount = getUserFragmentCount(u_userId,u_type,u_strId);
	int u_userDeleteAfterCount = u_userNowCount - u_deleteCount;
	if (u_userDeleteAfterCount <= 0)
	{
		cleanUserFragmentInfo(u_userId,u_type,u_strId);
	}
	else
	{
		updateUserFragment(u_userId,u_type,u_strId,u_userDeleteAfterCount);
	}
	return;
}
///用户碎片信息增删查///end


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

//获取卡牌等级
int getCardLevelById(const unsigned int& u_userId,const int& u_cardItemId)
{
	char buf[128] = {0};
	sprintf(buf,"select cardLevel from m_usercardinfo%d where userID = %d and itemId = %d;",\
		u_userId%USER_CARD_TABLE_COUNT,u_userId,u_cardItemId);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	int u_retCardLevel = 0;
	if (!q.eof())
	{
		u_retCardLevel = q.getIntFileds("cardLevel");
	}
	else
	{
		g_cMyGlog.errorlogRecord("getCardLevelById error,strSql:%s\n",strSql.c_str());
	}
	q.finalize();
	return u_retCardLevel;
}

int getCardMaxLevelByStrCardId(const string& u_strCardId)
{
	char buf[128] = {0};
	sprintf(buf,"select maxCardLevel from cardinfo where cardId = '%s';",u_strCardId.c_str());
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDb.master_slave_exQuery(strSql);
	int u_retMaxLevel = 0;
	if(!q.eof())
	{
		u_retMaxLevel = q.getIntFileds("maxCardLevel");
	}
	else
	{
		g_cMyGlog.errorlogRecord("getCardMaxLevelByStrCardId error,strSql:%s\n",strSql.c_str());
	}
	q.finalize();
	return u_retMaxLevel;
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

//获取充值用户最大扫荡和购买体力药水次数
void getUserMaxWipeOutAndBuyEnergyCount(const int& u_rechargedDiamond,int& u_maxWipeOutCount,int& u_maxBuyEnergyCount)
{
	char buf[256] = {0};
	sprintf(buf,"select wipeOutCount,buyEnergyCount from m_user_vip_privilege where needRechargedDiamond <= %d order by vipLevel desc limit 1;",\
		u_rechargedDiamond);
	string strSql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	if (!q.eof())
	{
		u_maxWipeOutCount = q.getIntFileds("wipeOutCount");
		u_maxBuyEnergyCount = q.getIntFileds("buyEnergyCount");
	}
	else
	{
		g_cMyGlog.errorlogRecord("getUserMaxWipeOutAndBuyEnergyCount error\n");
		g_cMyGlog.errorlogRecord("strSql:%s\n",strSql.c_str());
	}
	q.finalize();
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

//更新用户已购买体力药水次数
void updateUserBuyEnergyCount(const unsigned int& u_userId,const int& u_buyEnergyCount)
{
	char buf[256] = {0};
	sprintf(buf,"select * from m_user_wipeout_enery_count%d where userId=%d;",u_userId%20,u_userId);
	string strExQuerySql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strExQuerySql);
	memset(buf,0,sizeof(buf));
	if (q.eof())
	{
		sprintf(buf,"insert into m_user_wipeout_enery_count%d(userId,wipeOutCount,buyEnergyCount) \
					values(%d,%d,%d);",u_userId%20,u_userId,0,u_buyEnergyCount);
	}
	else
	{
		sprintf(buf,"update m_user_wipeout_enery_count%d set buyEnergyCount=%d  where userId=%d;",\
			u_userId%20,u_buyEnergyCount,u_userId);
	}
	q.finalize();
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUserBuyEnergyCount error,strSql:%s\n",strSql.c_str());
	}
}

//更新用户已扫荡次数
void updateUserWipeOutCount(const unsigned int& u_userId,const int& u_wipeOutCount)
{
	char buf[256] = {0};
	sprintf(buf,"select * from m_user_wipeout_enery_count%d where userId=%d;",u_userId%20,u_userId);
	string strExQuerySql(buf);
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strExQuerySql);
	memset(buf,0,sizeof(buf));
	if (q.eof())
	{
		sprintf(buf,"insert into m_user_wipeout_enery_count%d(userId,wipeOutCount,buyEnergyCount) \
					values(%d,%d,%d);",u_userId%20,u_userId,u_wipeOutCount,0);
	}
	else
	{
		sprintf(buf,"update m_user_wipeout_enery_count%d set wipeOutCount=%d  where userId=%d;",\
			u_userId%20,u_wipeOutCount,u_userId);
	}
	q.finalize();
	string strSql(buf);
	if (g_cMagicServerDb.execSQL(strSql) != 0)
	{
		g_cMyGlog.errorlogRecord("updateUserWipeOutCount error,strSql:%s\n",strSql.c_str());
	}
}

//判断用户是否已达当日最大体力购买次数
int isUserToMaxBuyEnergyCount(const unsigned int& u_userId)
{
	int u_wipeOutCount = 0,u_buyEnergyCount = 0,u_reChargedDiamond = 0,u_maxWipeOutCount = 0,u_maxBuyEnergyCount = 0;
	getUserWipeOutAndBuyEnergyCount(u_userId,u_wipeOutCount,u_buyEnergyCount);
	u_reChargedDiamond = getUserRechargedDiamond(u_userId);
	getUserMaxWipeOutAndBuyEnergyCount(u_reChargedDiamond,u_maxWipeOutCount,u_maxBuyEnergyCount);
	int u_retResult = 0;
	if (u_buyEnergyCount >= u_maxBuyEnergyCount)
	{
		u_retResult = K_TO_MAX_BUY_ENERGY_TIME;
	}
	else
	{
		u_buyEnergyCount += 1;
		updateUserBuyEnergyCount(u_userId,u_buyEnergyCount);
	}
	return u_retResult;
}


//判断用户是否已达当日最大最大扫荡次数
int isUserToMaxWipeOutCount(const unsigned int& u_userId)
{
	int u_wipeOutCount = 0,u_buyEnergyCount = 0,u_reChargedDiamond = 0,u_maxWipeOutCount = 0,u_maxBuyEnergyCount = 0;
	getUserWipeOutAndBuyEnergyCount(u_userId,u_wipeOutCount,u_buyEnergyCount);
	u_reChargedDiamond = getUserRechargedDiamond(u_userId);
	getUserMaxWipeOutAndBuyEnergyCount(u_reChargedDiamond,u_maxWipeOutCount,u_maxBuyEnergyCount);
	int u_retResult = 0;
	if (u_wipeOutCount >= u_maxWipeOutCount)
	{
		u_retResult = K_TO_MAX_WIP_OUT_COUNT;
	}
	else
	{
		u_wipeOutCount += 1;
		updateUserWipeOutCount(u_userId,u_wipeOutCount);
	}
	return u_retResult;
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