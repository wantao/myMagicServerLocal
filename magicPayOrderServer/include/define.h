#ifndef DEFINE_H
#define DEFINE_H

#include <iostream>
#include <map>

using namespace std;
//成功处理
#define PROC_SUCCESS	0	

//请求处理失败
#define PROC_FAILURE	-1

//注册请求
#define REGISTER 1
//登陆请求
#define LOGIN	 2
//获取系统信息请求
#define	SYSTEMINFO	 3
//获取任务信息请求
#define	ALLMISSION	 4
//任务奖励请求
#define	MISSIONAWARD 5
//获取好友信息请求
#define	FRIEND	 6
//获取等待确认好友请求
#define	WATITINGFRIEND	 7
//主动添加好友请求
#define	ADDFRIEND	 8
//同意添加好友请求
#define	FRIENDAGREE	 9
//好友删除请求
#define	FRIENDDELETE	 10
//购买体力请求
#define	BUYENERGY	 11
//友情抽奖请求
#define	AWARDFRIEND	 12
//获取固定公告栏信息
#define	GETFIEXDAFFICHE	 13
//获取流动公告栏信息
#define	RUNNINGAFFICHE	 14
//钻石抽奖请求
#define	AWARDDIAMOND	 15
//获取所有助战基友信息
#define	HELPFRIEND	 16
//卡牌升级
#define CARD_LEVLE_UP 17
//卡牌合成
#define CARD_EVOLUTION	18
//卡牌技能升级
#define SKIIL_LEVEV_UP	19
//卡牌编队请求
#define CARD_FORM	20
//副本掉落请求
#define BATTLE_LOSE	21
//副本结果请求
#define BATTLE_RESULT	22
//贩卖卡牌请求
#define SOLD_CARD	23
//领取友情值
#define GET_FRIEND_VALUE	24
//拒绝好友请求
#define REJECT_FRIEND_REQ	25
//商店购买请求
#define SHOP_BUY	26
//查找人物
#define SEARCH_PLAYER	27
//注册检查
#define REGIST_CHECK	28
//金币抽奖
#define GOLD_LOTTERY	29
//活动副本请求
#define ACTIVE_BATTLE_REQ	30
//资源下载请求
#define RESOURCE_DOWNLOAD_REQ	31
//邀请码验证
#define INVITATION_CODE_AUTHENTICATION	32
//获取体力请求
#define	GET_USER_ENERGY_REQ	33
//用户查询购买结果请求
#define USER_BUY_RESULT_REQ	34
//主服务器注册请求
#define MAIN_SERVER_REGIST_REQ	35
//主服务器登录请求
#define MAIN_SERVER_LOGIN_REQ	36
//玩家排行榜请求
#define PLAYER_RANK_REQ	37
//玩家获取公告请求
#define GET_NOTICE_REQ	38


//初始化玩家信息
//初始等级
#define USERLEVEL	1

//初始经验
#define USEREXP	0

//初始体力
#define ENERGY	22

//初始购买体力次数
#define BUYENEYGYTIME	0

//购买体力最大次数
#define MAX_BUYENEYGYTIME	2

//初始钻石数
#define	DIAMONDNUMBER	0

//初始友情值
#define FRIENDVALUE	200

//初始金币数
#define GOLDNUMBER	500

//初始领导力
#define	LEADERVALUE	18

//初始副本进度
#define INIT_BATTLE_PROGRESS "1-0"

//初始卡牌张数
#define INIT_CARD_COUNT	4

//初始任务个数
#define INIT_MISSION_COUNT 5

//登录任务初始ID
#define	LOGIN_MISSION 1

//非战斗卡牌
#define NOT_BATTLE_POSITION -1

//队长卡牌位置
#define CAPITOR_POSITION 0

//恢复一点体力所需的时间（单位s）
#define ONE_ENERGY_NEED_TIME	300

//用户卡牌分表个数
#define USER_CARD_TABLE_COUNT	100

//用户任务表分表个数
#define USER_MISSION_TABLE_COUNT	100

//用户好友表分表个数
#define USER_FRIEND_TABLE_COUNT 20

//用户好友请求分表个数
#define USER_FRIEND_REQ_TABLE_COUNT	20

//用户装备分表个数
#define USER_EQUIP_TABLE_COUNT	20

//用户初始最大好友个数
#define USER_MAX_FRIEND_COUNT 20

//用户最终最大好友个数
#define USER_FINAL_MAX_FRIEND_COUNT 40

//添加好友请求成功
#define ADD_FRIEND_SUCCESS 0

//用户不存在
#define USER_NOT_EXIST 1

//对方好友已满
#define OTHER_SIDE_FRIENN_MAX 2

//用户好友已满
#define USER_FRIEND_MAX 3

//对方已是自己好友
#define IS_USER_FRIEND 4

//购买一次体力恢复的体力值
#define REGAIN_ENERGY 10

//购买一次体力所需钻石数
#define ENERGY_DIAMOND_NEED	20


///////////购买体力结果////////////
//购买成功
#define BUY_ENERGY_SUCCESS 0

//已达到当日最大购买次数
#define BUY_ENERGY_OVER_MAX	1

//钻石不足
#define LESS_DIAMOND	2

//体力已满
#define ENERGY_ENOUGH	3

///////////购买体力结果////////////


//最大友情值
#define MAX_FRIEND_VALUE	5000

////友情抽奖/////
////抽奖一次所需花费的友情值
#define AWARD_FRIEND_VALUE 200

//友情抽奖成功
#define AWARD_FRIEND_SUCCESS 0

//友情值不足
#define AWARD_FRIEND_VALUE_LESS	1

////友情抽奖/////

///一次推送流动公告栏条数
#define RUNNING_AFFICHE_COUNTS 4

////钻石抽奖/////
////抽奖一次所需花费的钻石值
#define AWARD_DIAMOND_NUMBER 100

//钻石抽奖成功
#define AWARD_DIAMOND_SUCCESS 0

//钻石值不足
#define AWARD_DIAMOND_VALUE_LESS	2
////钻石抽奖/////

//卡牌达到上限
#define USER_CARD_TO_MAX	3

//抽奖金币不足
#define AWARD_GOLD_LESS	4

//金币抽奖一次所需金币
#define ONE_GOLD_AWARD_NEED_GOLD	1000


///////卡牌升级/////////
//吃掉一张卡牌所需金币数
#define EAT_ONE_CARD_GOLD	1000
//升一级所需经验数
#define	UP_ONE_LEVEL	1000
//升级成功
#define CARD_LEVEL_UP_SUCCESS	0
//金币不足
#define CARD_LEVEL_UP_LESS_GOLD	1
//卡牌不存在
#define CARD_LEVEL_UP_NOT_EXIST	2
///////卡牌升级/////////


//////////技能升级////////
//技能升级成功
#define SKILL_UP_SUCCESS	0
//技能熟练度不足
#define SKILL_NOT_ENOUGH	1

//技能等级已满
#define SKILL_LEVEL_MAX	2

//技能升级一级所需的技能熟练度proficiency
#define SKILL_UP_PROFICIENCY  50

//////////技能升级////////

/////////////////卡牌合成
//合成成功
#define EVOLUTION_SUCCESS	0

//卡牌不存在
#define EVOLUTION_CARD_NOT_EXIST	1

//金币不足
#define EVOLUTION_LESS_GOLD	2

//合成失败
#define	EVOLUTION_FAILURE	3
///////////////////卡牌合成


//////////卡牌编队//////////
#define CARD_FORM_SUCCESS	0
#define CARD_FORM_FAILURE	1
//////////卡牌编队//////////


///////掉落率随即范围///////
#define LOSE_MIN	1
#define LOSE_MAX	10000
#define MONSTER_EQUIP_TYPE_COUNT	1
///////掉落率随即范围///////

#define EQUIP_TMP_TABLE_COUNT	100
#define CARD_TMP_TABLE_COUNT	100


/////////副本战斗结果////////
#define BATTLE_WIN	0
#define BATTLE_FINAL_LOSE	1
/////////副本战斗结果////////


/////成长档随机范围宏定义//////begin
///一星成长档
#define ONE_STAR	1
#define ONE_STAR_RAND_MIN	4
#define ONE_STAR_RAND_MAX	7
///一星成长档

///二星成长档
#define TWO_STAR	2
#define TWO_STAR_RAND_MIN	8
#define TWO_STAR_RAND_MAX	12
///二星成长档

///三星成长档
#define THREE_STAR	3
#define THREE_STAR_RAND_MIN	13
#define THREE_STAR_RAND_MAX	17
///三星成长档

///四星成长档
#define FOUR_STAR	4
#define FOUR_STAR_RAND_MIN	18
#define FOUR_STAR_RAND_MAX	22
///四星成长档

///五星成长档
#define FIVE_STAR	5
#define FIVE_STAR_RAND_MIN	23
#define FIVE_STAR_RAND_MAX	27
///五星成长档

///六星成长档
#define SIX_STAR	6
#define SIX_STAR_RAND_MIN	28
#define SIX_STAR_RAND_MAX	32
///六星成长档

///七星成长档
#define SEVEN_STAR	7
#define SEVEN_STAR_RAND_MIN	33
#define SEVEN_STAR_RAND_MAX	37
///七星成长档

///八星成长档
#define EIGHT_STAR	8
#define EIGHT_STAR_RAND_MIN	38
#define EIGHT_STAR_RAND_MAX	42
///八星成长档

///九星成长档
#define NINE_STAR	9
#define NINE_STAR_RAND_MIN	43
#define NINE_STAR_RAND_MAX	47
///九星成长档

///十星成长档
#define TEN_STAR	10
#define TEN_STAR_RAND_MIN	48
#define TEN_STAR_RAND_MAX	50
///十星成长档

///十一星成长档
#define ELEVEN_STAR	11
#define ELEVEN_STAR_RAND_MIN	51
#define ELEVEN_STAR_RAND_MAX	54
///十一星成长档

///十二星成长档
#define TWELVE_STAR	12
#define TWELVE_STAR_RAND_MIN	55
#define TWELVE_STAR_RAND_MAX	59
///十二星成长档

///十三星成长档
#define THIRTEEN_STAR	13
#define THIRTEEN_STAR_RAND_MIN	60
#define THIRTEEN_STAR_RAND_MAX	64
///十三星成长档

///十四星成长档
#define FOURTEEN_STAR	14
#define FOURTEEN_STAR_RAND_MIN	65
#define FOURTEEN_STAR_RAND_MAX	69
///十四星成长档

///十五星成长档
#define FIFTEEN_STAR	15
#define FIFTEEN_STAR_RAND_MIN	70
#define FIFTEEN_STAR_RAND_MAX	74
///十五星成长档

///十六星成长档
#define SIXTEEN_STAR	16
#define SIXTEEN_STAR_RAND_MIN	75
#define SIXTEEN_STAR_RAND_MAX	79
///十六星成长档


///十七星成长档
#define SEVENTEEN_STAR	17
#define SEVENTEEN_STAR_RAND_MIN	80
#define SEVENTEEN_STAR_RAND_MAX	84
///十七星成长档

///十八星成长档
#define EIGHTEEN_STAR	18
#define EIGHTEEN_STAR_RAND_MIN	85
#define EIGHTEEN_STAR_RAND_MAX	89
///十八星成长档

///十九星成长档
#define NINETEEN_STAR	19
#define NINETEEN_STAR_RAND_MIN	90
#define NINETEEN_STAR_RAND_MAX	94
///十九星成长档

///二十星成长档
#define TWENTY_STAR	20
#define TWENTY_STAR_RAND_MIN	95
#define TWENTY_STAR_RAND_MAX	100
///二十星成长档


/////成长档随机范围宏定义//////end


//从好友表中取出的助战基友个数
#define FRIEND_HELP_FRIEND_COUNT	3

//从用户表中取出的助战基友个数
#define NOT_FRIEND_HELP_FRIEND_COUNT	5

//好友友情值增加
#define FRIEND_ADD_VALUE	10
//非好友友情值增加
#define NOT_FRIEND_ADD_VALUE	5

//玩家升级所需的经验
#define USER_LEVEL_UP_NEED_EXP(u_level)	(u_level+1)*(u_level+1)*(u_level+1)*(u_level+1)

//卡牌升级所需经验
#define CARD_LEVEL_UP_NEED_EXP(u_cardLevel)	u_cardLevel*160 //(u_cardLevel + 1)*(u_cardLevel + 1)*(u_cardLevel + 1)*6

//玩家等级上限
#define USER_MAX_LEVEL	100

//卡牌等级上限
#define CARD_LEVEL_MAX	70
//玩家拥有卡牌上限
#define USER_INIT_MAX_OWN_CARDS	40

//贩卖卡牌成功
#define SOLD_CARD_SUCCESS	0
//贩卖卡牌不存在
#define SOLD_CARD_NOT_EXISTS	1

//0，注册成功，1，注册Id已经存在，2，注册失败，3，登录成功，4，用户名或密码不正确
///注册或登录结果

//注册成功
#define REGIST_SUCCESS	0

//注册Id已经存在
#define REGIST_ID_EXISTS	1

//注册失败
#define REGIST_FAILURE	2

//登录成功
#define LOGIN_SUCCESS	3

//用户名或密码不正确
#define LOGIN_USERORPASSWD_NOT_EXIST	4

///注册或登录结果



//////////////每一级最大技能熟练度/////////////////

//一级最大技能熟练度
#define ONE_LEVEL_MAX_SKILLEXP	9
//二级最大技能熟练度
#define TWO_LEVEL_MAX_SKILLEXP	27
//三级最大技能熟练度
#define THREE_LEVEL_MAX_SKILLEXP	63
//四级最大技能熟练度
#define FOURE_LEVEL_MAX_SKILLEXP	124
//五级最大技能熟练度
#define FIVE_LEVEL_MAX_SKILLEXP	189
//六级最大技能熟练度
#define SIX_LEVEL_MAX_SKILLEXP	252
//七级最大技能熟练度
#define SEVEN_LEVEL_MAX_SKILLEXP	272
//八级最大技能熟练度
#define EIGHT_LEVEL_MAX_SKILLEXP	315
//九级最大技能熟练度
#define NINE_LEVEL_MAX_SKILLEXP	378

//////////////每一级最大技能熟练度/////////////////

//友情值最大领取次数
#define GET_FRIEND_VALUE_MAX_COUNT	30

//进入活动副本初始次数
#define ENTER_ACTIVE_BATTLE_INIT_COUNT	1

//小型黄金史莱姆价钱
#define MONEY_OF_2001	3000
//中型黄金史莱姆价钱
#define MONEY_OF_2002	6000
//大型黄金史莱姆价钱
#define MONEY_OF_2003	10000

//小号水蓝鼠经验值
#define EXP_OF_2004		5000
//中号水蓝鼠经验值
#define EXP_OF_2005		10000
//大号水蓝鼠经验值
#define EXP_OF_2006		20000

//一星卡牌所需领导力
#define LEADER_VALUE_OF_ONE_STAR	3
//二星卡牌所需领导力
#define LEADER_VALUE_OF_TWO_STAR	4
//三星卡牌所需领导力
#define LEADER_VALUE_OF_THREE_STAR	5
//四星卡牌所需领导力
#define LEADER_VALUE_OF_FOUR_STAR	6
//五星卡牌所需领导力
#define LEADER_VALUE_OF_FIVE_STAR	7
//六星卡牌所需领导力
#define LEADER_VALUE_OF_SIX_STAR	8

//任务类型
typedef enum MissionTypeEnum
{
	LOGIN_MISSION_TYPE = 1,//登陆任务
	BATTLE_MISSION,//剧情任务
	LEVEL_UP_MISSION,//升级任务
	PAY_MISSION,	//充值任务
	ACTIVE_BATTLE_MISSION	//活动副本任务
}MissionTypeEnum;

//任务奖励类型
typedef enum MissionAwardTypeEnum
{
	AWARD_MONEY = 1,//奖励金币
	FRIEND_VALUE_AWARD,//奖励友情值
	AWARD_MAGIC_STON,//奖励魔石
	AWARD_ENERGY = 5, //奖励满疲劳药水
	AWARD_CARD,	//奖励卡牌
	AWARD_USER_EXP	//奖励经验
}MissionAwardTypeEnum;

//充值或奖励的条件
typedef enum payMissionAwardEnum
{
	M_1_YUAN_CONDITION = 1,//累计充值1元
	M_50_YUAN_CONDITION = 50,//累计充值50元
	M_100_YUAN_CONDITION = 100,//累计充值100元
	M_500_YUAN_CONDITION = 500,//累计充值500元
	M_1500_YUAN_CONDITION = 1500,//累计充值1500元
	M_2000_YUAN_CONDITION = 2000,//累计充值2000元
	M_3000_YUAN_CONDITION = 3000,//累计充值3000元
	M_5000_YUAN_CONDITION = 5000,//累计充值5000元
}payMissionAwardEnum;

typedef enum weekDayEnum
{
	L_SUNDAY = 0,//星期日
	L_MONDAY = 1,//星期一
	L_TUESDAY = 2,//星期二
	L_WEDNESDAY = 3,//星期三
	L_THURSDAY = 4,//星期四
	L_FRIDAY = 5,//星期五
	L_SATURDAY = 6//星期六

}weekDayEnum;

typedef enum userPlateFormEnum
{
	P_91 = 0,//91平台
	P_IOS = 1//ios平台
}userPlateFormEnum;

//玩家活动副本表个数
#define ACTIVE_BATTLE_COUNT 20

typedef map<string,int> activeBattleMap;

#define USE_FRIEND_MAX_COUNT_ONE_DAY 1

//一星卡牌能升到的最大等级
#define ONE_STAR_MAX_CARD_LEVEL	12
//二星卡牌能升到的最大等级
#define TWO_STAR_MAX_CARD_LEVEL	20
//三星卡牌能升到的最大等级
#define THREE_STAR_MAX_CARD_LEVEL 30
//四星卡牌能升到的最大等级
#define FOUR_STAR_MAX_CARD_LEVEL 40
//五星卡牌能升到的最大等级
#define FIVE_STAR_MAX_CARD_LEVEL 55
//六星卡牌能升到的最大等级
#define SIX_STAR_MAX_CARD_LEVEL	70


#endif