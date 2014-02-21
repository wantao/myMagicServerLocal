#ifndef DEFINE_H
#define DEFINE_H

#include <iostream>
#include <map>

using namespace std;
//�ɹ�����
#define PROC_SUCCESS	0	

//������ʧ��
#define PROC_FAILURE	-1

//ע������
#define REGISTER 1
//��½����
#define LOGIN	 2
//��ȡϵͳ��Ϣ����
#define	SYSTEMINFO	 3
//��ȡ������Ϣ����
#define	ALLMISSION	 4
//����������
#define	MISSIONAWARD 5
//��ȡ������Ϣ����
#define	FRIEND	 6
//��ȡ�ȴ�ȷ�Ϻ�������
#define	WATITINGFRIEND	 7
//������Ӻ�������
#define	ADDFRIEND	 8
//ͬ����Ӻ�������
#define	FRIENDAGREE	 9
//����ɾ������
#define	FRIENDDELETE	 10
//������������
#define	BUYENERGY	 11
//����齱����
#define	AWARDFRIEND	 12
//��ȡ�̶���������Ϣ
#define	GETFIEXDAFFICHE	 13
//��ȡ������������Ϣ
#define	RUNNINGAFFICHE	 14
//��ʯ�齱����
#define	AWARDDIAMOND	 15
//��ȡ������ս������Ϣ
#define	HELPFRIEND	 16
//��������
#define CARD_LEVLE_UP 17
//���ƺϳ�
#define CARD_EVOLUTION	18
//���Ƽ�������
#define SKIIL_LEVEV_UP	19
//���Ʊ������
#define CARD_FORM	20
//������������
#define BATTLE_LOSE	21
//�����������
#define BATTLE_RESULT	22
//������������
#define SOLD_CARD	23
//��ȡ����ֵ
#define GET_FRIEND_VALUE	24
//�ܾ���������
#define REJECT_FRIEND_REQ	25
//�̵깺������
#define SHOP_BUY	26
//��������
#define SEARCH_PLAYER	27
//ע����
#define REGIST_CHECK	28
//��ҳ齱
#define GOLD_LOTTERY	29
//���������
#define ACTIVE_BATTLE_REQ	30
//��Դ��������
#define RESOURCE_DOWNLOAD_REQ	31
//��������֤
#define INVITATION_CODE_AUTHENTICATION	32
//��ȡ��������
#define	GET_USER_ENERGY_REQ	33
//�û���ѯ����������
#define USER_BUY_RESULT_REQ	34
//��������ע������
#define MAIN_SERVER_REGIST_REQ	35
//����������¼����
#define MAIN_SERVER_LOGIN_REQ	36
//������а�����
#define PLAYER_RANK_REQ	37
//��һ�ȡ��������
#define GET_NOTICE_REQ	38


//��ʼ�������Ϣ
//��ʼ�ȼ�
#define USERLEVEL	1

//��ʼ����
#define USEREXP	0

//��ʼ����
#define ENERGY	22

//��ʼ������������
#define BUYENEYGYTIME	0

//��������������
#define MAX_BUYENEYGYTIME	2

//��ʼ��ʯ��
#define	DIAMONDNUMBER	0

//��ʼ����ֵ
#define FRIENDVALUE	200

//��ʼ�����
#define GOLDNUMBER	500

//��ʼ�쵼��
#define	LEADERVALUE	18

//��ʼ��������
#define INIT_BATTLE_PROGRESS "1-0"

//��ʼ��������
#define INIT_CARD_COUNT	4

//��ʼ�������
#define INIT_MISSION_COUNT 5

//��¼�����ʼID
#define	LOGIN_MISSION 1

//��ս������
#define NOT_BATTLE_POSITION -1

//�ӳ�����λ��
#define CAPITOR_POSITION 0

//�ָ�һ�����������ʱ�䣨��λs��
#define ONE_ENERGY_NEED_TIME	300

//�û����Ʒֱ����
#define USER_CARD_TABLE_COUNT	100

//�û������ֱ����
#define USER_MISSION_TABLE_COUNT	100

//�û����ѱ�ֱ����
#define USER_FRIEND_TABLE_COUNT 20

//�û���������ֱ����
#define USER_FRIEND_REQ_TABLE_COUNT	20

//�û�װ���ֱ����
#define USER_EQUIP_TABLE_COUNT	20

//�û���ʼ�����Ѹ���
#define USER_MAX_FRIEND_COUNT 20

//�û����������Ѹ���
#define USER_FINAL_MAX_FRIEND_COUNT 40

//��Ӻ�������ɹ�
#define ADD_FRIEND_SUCCESS 0

//�û�������
#define USER_NOT_EXIST 1

//�Է���������
#define OTHER_SIDE_FRIENN_MAX 2

//�û���������
#define USER_FRIEND_MAX 3

//�Է������Լ�����
#define IS_USER_FRIEND 4

//����һ�������ָ�������ֵ
#define REGAIN_ENERGY 10

//����һ������������ʯ��
#define ENERGY_DIAMOND_NEED	20


///////////�����������////////////
//����ɹ�
#define BUY_ENERGY_SUCCESS 0

//�Ѵﵽ������������
#define BUY_ENERGY_OVER_MAX	1

//��ʯ����
#define LESS_DIAMOND	2

//��������
#define ENERGY_ENOUGH	3

///////////�����������////////////


//�������ֵ
#define MAX_FRIEND_VALUE	5000

////����齱/////
////�齱һ�����軨�ѵ�����ֵ
#define AWARD_FRIEND_VALUE 200

//����齱�ɹ�
#define AWARD_FRIEND_SUCCESS 0

//����ֵ����
#define AWARD_FRIEND_VALUE_LESS	1

////����齱/////

///һ��������������������
#define RUNNING_AFFICHE_COUNTS 4

////��ʯ�齱/////
////�齱һ�����軨�ѵ���ʯֵ
#define AWARD_DIAMOND_NUMBER 100

//��ʯ�齱�ɹ�
#define AWARD_DIAMOND_SUCCESS 0

//��ʯֵ����
#define AWARD_DIAMOND_VALUE_LESS	2
////��ʯ�齱/////

//���ƴﵽ����
#define USER_CARD_TO_MAX	3

//�齱��Ҳ���
#define AWARD_GOLD_LESS	4

//��ҳ齱һ��������
#define ONE_GOLD_AWARD_NEED_GOLD	1000


///////��������/////////
//�Ե�һ�ſ�����������
#define EAT_ONE_CARD_GOLD	1000
//��һ�����辭����
#define	UP_ONE_LEVEL	1000
//�����ɹ�
#define CARD_LEVEL_UP_SUCCESS	0
//��Ҳ���
#define CARD_LEVEL_UP_LESS_GOLD	1
//���Ʋ�����
#define CARD_LEVEL_UP_NOT_EXIST	2
///////��������/////////


//////////��������////////
//���������ɹ�
#define SKILL_UP_SUCCESS	0
//���������Ȳ���
#define SKILL_NOT_ENOUGH	1

//���ܵȼ�����
#define SKILL_LEVEL_MAX	2

//��������һ������ļ���������proficiency
#define SKILL_UP_PROFICIENCY  50

//////////��������////////

/////////////////���ƺϳ�
//�ϳɳɹ�
#define EVOLUTION_SUCCESS	0

//���Ʋ�����
#define EVOLUTION_CARD_NOT_EXIST	1

//��Ҳ���
#define EVOLUTION_LESS_GOLD	2

//�ϳ�ʧ��
#define	EVOLUTION_FAILURE	3
///////////////////���ƺϳ�


//////////���Ʊ��//////////
#define CARD_FORM_SUCCESS	0
#define CARD_FORM_FAILURE	1
//////////���Ʊ��//////////


///////�������漴��Χ///////
#define LOSE_MIN	1
#define LOSE_MAX	10000
#define MONSTER_EQUIP_TYPE_COUNT	1
///////�������漴��Χ///////

#define EQUIP_TMP_TABLE_COUNT	100
#define CARD_TMP_TABLE_COUNT	100


/////////����ս�����////////
#define BATTLE_WIN	0
#define BATTLE_FINAL_LOSE	1
/////////����ս�����////////


/////�ɳ��������Χ�궨��//////begin
///һ�ǳɳ���
#define ONE_STAR	1
#define ONE_STAR_RAND_MIN	4
#define ONE_STAR_RAND_MAX	7
///һ�ǳɳ���

///���ǳɳ���
#define TWO_STAR	2
#define TWO_STAR_RAND_MIN	8
#define TWO_STAR_RAND_MAX	12
///���ǳɳ���

///���ǳɳ���
#define THREE_STAR	3
#define THREE_STAR_RAND_MIN	13
#define THREE_STAR_RAND_MAX	17
///���ǳɳ���

///���ǳɳ���
#define FOUR_STAR	4
#define FOUR_STAR_RAND_MIN	18
#define FOUR_STAR_RAND_MAX	22
///���ǳɳ���

///���ǳɳ���
#define FIVE_STAR	5
#define FIVE_STAR_RAND_MIN	23
#define FIVE_STAR_RAND_MAX	27
///���ǳɳ���

///���ǳɳ���
#define SIX_STAR	6
#define SIX_STAR_RAND_MIN	28
#define SIX_STAR_RAND_MAX	32
///���ǳɳ���

///���ǳɳ���
#define SEVEN_STAR	7
#define SEVEN_STAR_RAND_MIN	33
#define SEVEN_STAR_RAND_MAX	37
///���ǳɳ���

///���ǳɳ���
#define EIGHT_STAR	8
#define EIGHT_STAR_RAND_MIN	38
#define EIGHT_STAR_RAND_MAX	42
///���ǳɳ���

///���ǳɳ���
#define NINE_STAR	9
#define NINE_STAR_RAND_MIN	43
#define NINE_STAR_RAND_MAX	47
///���ǳɳ���

///ʮ�ǳɳ���
#define TEN_STAR	10
#define TEN_STAR_RAND_MIN	48
#define TEN_STAR_RAND_MAX	50
///ʮ�ǳɳ���

///ʮһ�ǳɳ���
#define ELEVEN_STAR	11
#define ELEVEN_STAR_RAND_MIN	51
#define ELEVEN_STAR_RAND_MAX	54
///ʮһ�ǳɳ���

///ʮ���ǳɳ���
#define TWELVE_STAR	12
#define TWELVE_STAR_RAND_MIN	55
#define TWELVE_STAR_RAND_MAX	59
///ʮ���ǳɳ���

///ʮ���ǳɳ���
#define THIRTEEN_STAR	13
#define THIRTEEN_STAR_RAND_MIN	60
#define THIRTEEN_STAR_RAND_MAX	64
///ʮ���ǳɳ���

///ʮ���ǳɳ���
#define FOURTEEN_STAR	14
#define FOURTEEN_STAR_RAND_MIN	65
#define FOURTEEN_STAR_RAND_MAX	69
///ʮ���ǳɳ���

///ʮ���ǳɳ���
#define FIFTEEN_STAR	15
#define FIFTEEN_STAR_RAND_MIN	70
#define FIFTEEN_STAR_RAND_MAX	74
///ʮ���ǳɳ���

///ʮ���ǳɳ���
#define SIXTEEN_STAR	16
#define SIXTEEN_STAR_RAND_MIN	75
#define SIXTEEN_STAR_RAND_MAX	79
///ʮ���ǳɳ���


///ʮ���ǳɳ���
#define SEVENTEEN_STAR	17
#define SEVENTEEN_STAR_RAND_MIN	80
#define SEVENTEEN_STAR_RAND_MAX	84
///ʮ���ǳɳ���

///ʮ���ǳɳ���
#define EIGHTEEN_STAR	18
#define EIGHTEEN_STAR_RAND_MIN	85
#define EIGHTEEN_STAR_RAND_MAX	89
///ʮ���ǳɳ���

///ʮ���ǳɳ���
#define NINETEEN_STAR	19
#define NINETEEN_STAR_RAND_MIN	90
#define NINETEEN_STAR_RAND_MAX	94
///ʮ���ǳɳ���

///��ʮ�ǳɳ���
#define TWENTY_STAR	20
#define TWENTY_STAR_RAND_MIN	95
#define TWENTY_STAR_RAND_MAX	100
///��ʮ�ǳɳ���


/////�ɳ��������Χ�궨��//////end


//�Ӻ��ѱ���ȡ������ս���Ѹ���
#define FRIEND_HELP_FRIEND_COUNT	3

//���û�����ȡ������ս���Ѹ���
#define NOT_FRIEND_HELP_FRIEND_COUNT	5

//��������ֵ����
#define FRIEND_ADD_VALUE	10
//�Ǻ�������ֵ����
#define NOT_FRIEND_ADD_VALUE	5

//�����������ľ���
#define USER_LEVEL_UP_NEED_EXP(u_level)	(u_level+1)*(u_level+1)*(u_level+1)*(u_level+1)

//�����������辭��
#define CARD_LEVEL_UP_NEED_EXP(u_cardLevel)	u_cardLevel*160 //(u_cardLevel + 1)*(u_cardLevel + 1)*(u_cardLevel + 1)*6

//��ҵȼ�����
#define USER_MAX_LEVEL	100

//���Ƶȼ�����
#define CARD_LEVEL_MAX	70
//���ӵ�п�������
#define USER_INIT_MAX_OWN_CARDS	40

//�������Ƴɹ�
#define SOLD_CARD_SUCCESS	0
//�������Ʋ�����
#define SOLD_CARD_NOT_EXISTS	1

//0��ע��ɹ���1��ע��Id�Ѿ����ڣ�2��ע��ʧ�ܣ�3����¼�ɹ���4���û��������벻��ȷ
///ע����¼���

//ע��ɹ�
#define REGIST_SUCCESS	0

//ע��Id�Ѿ�����
#define REGIST_ID_EXISTS	1

//ע��ʧ��
#define REGIST_FAILURE	2

//��¼�ɹ�
#define LOGIN_SUCCESS	3

//�û��������벻��ȷ
#define LOGIN_USERORPASSWD_NOT_EXIST	4

///ע����¼���



//////////////ÿһ�������������/////////////////

//һ�������������
#define ONE_LEVEL_MAX_SKILLEXP	9
//���������������
#define TWO_LEVEL_MAX_SKILLEXP	27
//���������������
#define THREE_LEVEL_MAX_SKILLEXP	63
//�ļ������������
#define FOURE_LEVEL_MAX_SKILLEXP	124
//�弶�����������
#define FIVE_LEVEL_MAX_SKILLEXP	189
//���������������
#define SIX_LEVEL_MAX_SKILLEXP	252
//�߼������������
#define SEVEN_LEVEL_MAX_SKILLEXP	272
//�˼������������
#define EIGHT_LEVEL_MAX_SKILLEXP	315
//�ż������������
#define NINE_LEVEL_MAX_SKILLEXP	378

//////////////ÿһ�������������/////////////////

//����ֵ�����ȡ����
#define GET_FRIEND_VALUE_MAX_COUNT	30

//����������ʼ����
#define ENTER_ACTIVE_BATTLE_INIT_COUNT	1

//С�ͻƽ�ʷ��ķ��Ǯ
#define MONEY_OF_2001	3000
//���ͻƽ�ʷ��ķ��Ǯ
#define MONEY_OF_2002	6000
//���ͻƽ�ʷ��ķ��Ǯ
#define MONEY_OF_2003	10000

//С��ˮ������ֵ
#define EXP_OF_2004		5000
//�к�ˮ������ֵ
#define EXP_OF_2005		10000
//���ˮ������ֵ
#define EXP_OF_2006		20000

//һ�ǿ��������쵼��
#define LEADER_VALUE_OF_ONE_STAR	3
//���ǿ��������쵼��
#define LEADER_VALUE_OF_TWO_STAR	4
//���ǿ��������쵼��
#define LEADER_VALUE_OF_THREE_STAR	5
//���ǿ��������쵼��
#define LEADER_VALUE_OF_FOUR_STAR	6
//���ǿ��������쵼��
#define LEADER_VALUE_OF_FIVE_STAR	7
//���ǿ��������쵼��
#define LEADER_VALUE_OF_SIX_STAR	8

//��������
typedef enum MissionTypeEnum
{
	LOGIN_MISSION_TYPE = 1,//��½����
	BATTLE_MISSION,//��������
	LEVEL_UP_MISSION,//��������
	PAY_MISSION,	//��ֵ����
	ACTIVE_BATTLE_MISSION	//���������
}MissionTypeEnum;

//����������
typedef enum MissionAwardTypeEnum
{
	AWARD_MONEY = 1,//�������
	FRIEND_VALUE_AWARD,//��������ֵ
	AWARD_MAGIC_STON,//����ħʯ
	AWARD_ENERGY = 5, //������ƣ��ҩˮ
	AWARD_CARD,	//��������
	AWARD_USER_EXP	//��������
}MissionAwardTypeEnum;

//��ֵ����������
typedef enum payMissionAwardEnum
{
	M_1_YUAN_CONDITION = 1,//�ۼƳ�ֵ1Ԫ
	M_50_YUAN_CONDITION = 50,//�ۼƳ�ֵ50Ԫ
	M_100_YUAN_CONDITION = 100,//�ۼƳ�ֵ100Ԫ
	M_500_YUAN_CONDITION = 500,//�ۼƳ�ֵ500Ԫ
	M_1500_YUAN_CONDITION = 1500,//�ۼƳ�ֵ1500Ԫ
	M_2000_YUAN_CONDITION = 2000,//�ۼƳ�ֵ2000Ԫ
	M_3000_YUAN_CONDITION = 3000,//�ۼƳ�ֵ3000Ԫ
	M_5000_YUAN_CONDITION = 5000,//�ۼƳ�ֵ5000Ԫ
}payMissionAwardEnum;

typedef enum weekDayEnum
{
	L_SUNDAY = 0,//������
	L_MONDAY = 1,//����һ
	L_TUESDAY = 2,//���ڶ�
	L_WEDNESDAY = 3,//������
	L_THURSDAY = 4,//������
	L_FRIDAY = 5,//������
	L_SATURDAY = 6//������

}weekDayEnum;

typedef enum userPlateFormEnum
{
	P_91 = 0,//91ƽ̨
	P_IOS = 1//iosƽ̨
}userPlateFormEnum;

//��һ���������
#define ACTIVE_BATTLE_COUNT 20

typedef map<string,int> activeBattleMap;

#define USE_FRIEND_MAX_COUNT_ONE_DAY 1

//һ�ǿ��������������ȼ�
#define ONE_STAR_MAX_CARD_LEVEL	12
//���ǿ��������������ȼ�
#define TWO_STAR_MAX_CARD_LEVEL	20
//���ǿ��������������ȼ�
#define THREE_STAR_MAX_CARD_LEVEL 30
//���ǿ��������������ȼ�
#define FOUR_STAR_MAX_CARD_LEVEL 40
//���ǿ��������������ȼ�
#define FIVE_STAR_MAX_CARD_LEVEL 55
//���ǿ��������������ȼ�
#define SIX_STAR_MAX_CARD_LEVEL	70


#endif