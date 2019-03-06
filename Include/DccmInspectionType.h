#pragma once

// ���Ի�������	TEST_ENV_TYPE
#define TET_NULL					0x0000 // �޶���

// 0xnn01 Sensor��ʼ��
#define TET_INIT					0x0001
#define TET_INIT_AFTER_POWER_ON		0x0101 //����SENSOR �ϵ��Streamingǰ��ʼ��
#define TET_INIT_AFTER_STREAM		0x0201 //����SENSOR Streaming���ʼ��

// 0xnn02 Զ��
#define TET_INF						0x0002 //Զ����������ֻ��վλ���־�������
#define TET_INF_1M					0x0102
#define TET_INF_5M					0x0202
#define TET_INF_10M					0x0302

// 0xnn03 �о�
#define TET_MID						0x0003 //�о���������ֻ��վλ���־�������
#define TET_MID_15CM				0x0103
#define TET_MID_20CM				0x0203
#define TET_MID_30CM				0x0303
#define TET_MID_50CM				0x0403

// 0xnn04 ����
#define TET_MAC						0x0004 //������������ֻ��վλ���־�������
#define TET_MAC_7CM					0x0104
#define TET_MAC_8CM					0x0204
#define TET_MAC_10CM				0x0304

// 0xnn05 �׳�
#define TET_WHITE					0x0005 //�׳���������ֻ��վλ���־�������
#define TET_WHITE_1CM				0x0105
#define TET_WHITE_2CM				0x0205

// 0xnn06 �ڳ�
#define TET_BLACK					0x0006 //�ڳ���������ֻ��վλ���־�������


// 0xnn07 ���������Ŀ
#define TET_FOCUS					0x0007 //�ڳ���������ֻ��վλ���־�������


#define TET_ALL						8		// �ܹ�8������

