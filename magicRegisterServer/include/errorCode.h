#ifndef ERRORCODE_H
#define ERRORCODE_H

typedef enum errorCodeEnum
{
	K_REGIST_ACCOUNT_REPEAT	= 10001,//ע���ʺ��ظ�
	K_REGIST_USERNAME_REPEAT,//ע���ǳ��ظ�
	K_ACCOUNT_OR_PASSWD_ERROR,	//��¼�ʺŻ��������
	K_ACCOUNT_UNNORMAL,//�ʺ��쳣��tokenֵ����
	K_AWARD_HAS_GOT,//��������ȡ
	K_MISSION_NOT_FINISHED,//����δ���
	K_BACKPACK_HAS_FULL,//��������
	K_OPPOSIT_FRIEND_FULL,//�Է���������
	K_OWN_FRIEND_FULL,//�Լ���������
	K_OPPOSIT_IS_FRIEND,//�Է��Ѿ������ĺ���
	K_FRIEND_IS_NOT_EXIST,//���Ѳ�����
	K_FRIEND_VALUE_HAS_GOT,//����ֵ����ȡ
	K_MAGIC_STONE_NOT_ENOUGH,//ħʯ����
	K_TO_MAX_BUY_ENERGY_TIME,//�ﵽ��������
	K_FRIEND_VALUE_NOT_ENOUGH,//����ֵ����
	K_CARD_NOT_EXIST,//���Ʋ�����
	K_GOLD_NOT_ENOUGH,//��Ҳ���
	K_CARD_LEVEL_HAS_TO_MAX,//�����Ѵﵽ���ȼ�
	K_CARD_CAN_NOT_EVOLUTION,//���Ʋ��ɺϳ�
	K_EVOLUTION_MATERIAL_NOT_ENOUGH,//�ϳɲ��ϲ���
	K_SKILL_EXP_NOT_ENOUGH,//���������Ȳ���
	K_SKILL_LEVEL_TO_MAX,//���ܴﵽ���ȼ�
	K_CAPITOR_NOT_EXIST,//�ӳ����Ʋ�����
	K_BATTLE_ID_NOT_EXIST,//����������
	K_ENERGY_NOT_ENOUGH,//��������
	K_NOT_ENTER_BATTLE,//δ���븱������ֹ�ظ����͸����������ƭȡ����������
	K_LEVEL_OF_ENTER_BATTLE_NOT_ENOUGH, //�û��ȼ��������븱��
	K_TO_MAX_GET_FRIEND_VALUE_COUNT,	//�Դﵽ�����ȡ����ֵ����
	K_TO_MAX_FRIEND_VALUE,	//����ֵ����
	K_TO_MAX_ENTER_ACTIVE_COUNT	//�ﵽ���ս�������������
	//K_TO_MAX_ENERGY	//����ֵ����
}errorCodeEnum;

#endif