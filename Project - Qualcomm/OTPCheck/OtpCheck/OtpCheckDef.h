#pragma once
#define I2C_RETRY_NUM    4
#define I2C_RETRY_WAIT   1  // 1ms
#define UNEXCEPT_REG 0xFFFF
#define UNEXCEPT_VALUE 0x0000

typedef enum FUNCTION_RESULT
{		
	FUNCTION_PASS  = 0,     // OK
	FUNCTION_FAIL  = -1,     // NG
}function_result;

typedef struct tagValidFlag
{//ÿ��ģ�����Ч�� ����OTP   -1Ϊ��Ч 0Ϊ�� 
	USHORT empty;
	USHORT valid;
	USHORT invalid;
	USHORT validGroup1;
	USHORT invalidGroup1;
	USHORT validGroup2;
	USHORT invalidGroup2;
	USHORT validGroup3;
	USHORT invalidGroup3;
}Flag;

typedef struct tagValidGroup
{//ÿ��ģ�����Ч�� ����OTP   -1Ϊ��Ч 0Ϊ�� 
	int empty;
	int invalid;
	int group1;
	int group2;
	int group3;
	int basic;
	int lsc;
	int awb;
	int af;
	int pdaf;
}Group;

typedef struct tagAddressFlag
{// Flag��Address
	USHORT basic;
	USHORT lsc;
	USHORT awb;
	USHORT af;
	USHORT pdaf1;
	USHORT pdaf2;
}AddressFlag;

typedef struct tagAddressSum
{// CheckSum��Address
	USHORT basic;
	USHORT awb;
	USHORT lsc;
	USHORT af;
	USHORT pdaf1;
	USHORT pdaf2;
}AddressSum;

typedef struct tagCalcCheckSum
{
	USHORT divisor;
	USHORT addend;
}CalcCheckSum;

typedef struct tagGapSize
{// CheckSum��Address
	USHORT basic;
	USHORT awb;
	USHORT lsc;
	USHORT af;
	USHORT pdaf1;
	USHORT pdaf2;
}GapSize;

typedef struct tagOtpSize
{// ÿ��ģ��ռ�õĿռ�
	UINT basic;
	UINT awb;
	UINT lsc;
	UINT af;
	UINT pdaf1;
	UINT pdaf2;

}OtpSize;

typedef struct tagOtpPage
{// ��Page���͵�Sensor����EEPROM����Page
	USHORT basic;
	USHORT awb;
	USHORT lsc;
	USHORT af;
	USHORT pdaf1;
	USHORT pdaf1_End;
	USHORT pdaf2;
	USHORT pdaf2_End;

}OtpPage;

typedef struct tagAFAddress
{// AF Code��Address
	USHORT stcL;
	USHORT stcH;
	USHORT infL;
	USHORT infH;
	USHORT macL;
	USHORT macH;
	USHORT midL;
	USHORT midH;

}AFAddress;

typedef struct tagAFValue
{//AF Value
	USHORT m_bStcCode;
	USHORT m_bInfCode;
	USHORT m_bMacCode;
	USHORT m_bMidCode;

}AFValue;

typedef struct tagAddressAWB
{// AWB ��Address
	USHORT RGL;
	USHORT RGH;
	USHORT BGL;
	USHORT BGH;
	USHORT GGL;
	USHORT GGH;
	USHORT RL;
	USHORT RH;
	USHORT GrL;
	USHORT GrH;
	USHORT GbL;
	USHORT GbH;
	USHORT BL;
	USHORT BH;

	USHORT GOLDRGL;
	USHORT GOLDRGH;
	USHORT GOLDBGL;
	USHORT GOLDBGH;
	USHORT GOLDGGL;
	USHORT GOLDGGH;
	USHORT GOLDRL;
	USHORT GOLDRH;
	USHORT GOLDGrL;
	USHORT GOLDGrH;
	USHORT GOLDGbL;
	USHORT GOLDGbH;
	USHORT GOLDBL;
	USHORT GOLDBH;
}AddressAWB;

typedef struct tagValueAWB
{// AWB ��Value
	USHORT RG;
	USHORT BG;
	USHORT GG;
	USHORT R;
	USHORT Gr;
	USHORT Gb;
	USHORT B;
	USHORT GOLDRG;
	USHORT GOLDBG;
	USHORT GOLDGG;
	USHORT GOLDR;
	USHORT GOLDGr;
	USHORT GOLDGb;
	USHORT GOLDB;

	BYTE RGL;
	BYTE RGH;
	BYTE BGL;
	BYTE BGH;
	BYTE GGL;
	BYTE GGH;
	BYTE RL;
	BYTE RH;
	BYTE GrL;
	BYTE GrH;
	BYTE GbL;
	BYTE GbH;
	BYTE BL;
	BYTE BH;
	BYTE GOLDRGL;
	BYTE GOLDRGH;
	BYTE GOLDBGL;
	BYTE GOLDBGH;
	BYTE GOLDGGL;
	BYTE GOLDGGH;
	BYTE GOLDRL;
	BYTE GOLDRH;
	BYTE GOLDGrL;
	BYTE GOLDGrH;
	BYTE GOLDGbL;
	BYTE GOLDGbH;
	BYTE GOLDBL;
	BYTE GOLDBH;
}ValueAWB;
