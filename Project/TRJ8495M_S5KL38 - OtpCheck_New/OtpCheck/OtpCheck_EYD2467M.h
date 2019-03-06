#pragma once
#define IIC_SENSOR_EYD2467M 1
#include "OtpCheckBase.h"

class OtpCheck_EYD2467M : public OtpCheckBase
{
public:
	OtpCheck_EYD2467M(void);
	~OtpCheck_EYD2467M(void);

	virtual BOOL Check_ModuleInfo();
	virtual BOOL Check_AWB();
	virtual BOOL Check_LSC();
	virtual BOOL Check_AF();

#ifdef IIC_SENSOR_EYD2467M
	virtual BOOL ReadSensor(USHORT reg, USHORT* val);	
	virtual BOOL WriteSensor(USHORT reg, USHORT val);	
	virtual BYTE ReadSensor_MultiBytes(WORD reg, USHORT regSize, USHORT *pVal, WORD valSize, BOOL bStop = FALSE);
	virtual BYTE WriteSensor_MultiBytes(WORD reg, USHORT regSize, USHORT *pVal, WORD valSize);
#else
	virtual BOOL ReadEeprom_Byte(WORD reg, BYTE* val); //从SENSOR指定寄存器读取一个BYTE
	virtual BOOL WriteEeprom_Byte(WORD reg, BYTE val); //从SENSOR写入寄存器写入一个BYTE
/*	virtual BOOL ReadEeprom_Word(WORD reg, WORD* val); //从SENSOR指定寄存器读取二个BYTE
	virtual BOOL WriteEeprom_Word(WORD reg, WORD val); //从SENSOR写入寄存器写入二个BYTE
*/	virtual BYTE ReadEeprom_MultiBytes(USHORT reg, BYTE regSize, BYTE *pVal, USHORT valSize, BOOL bStop = FALSE);
	virtual BYTE WriteEeprom_MultiBytes(USHORT reg, BYTE regSize, BYTE *pVal, USHORT valSize);
#endif

	virtual BOOL OTPInit();
	virtual BOOL OTPRelease();
	virtual BOOL ReadOTPData(USHORT startAddr, USHORT EndAddr, USHORT *Data);
	virtual BOOL WriteOTPData(USHORT startAddr, USHORT EndAddr, USHORT *Data);

private:
	virtual	INT CheckFlag_ModuleInfo();	// 0x01: Group1 0x13: Group2 0x37:Group3 其他：数据为空或无效
	virtual	INT CheckFlag_AWB();			// 0x01: Group1 0x13: Group2 0x37:Group3 其他：数据为空或无效
	virtual	INT CheckFlag_LSC();			// 0x01: Group1 0x13: Group2 0x37:Group3 其他：数据为空或无效
	virtual	INT CheckFlag_AF();			// 0x01: Group1 0x13: Group2 0x37:Group3 其他：数据为空或无效

	virtual	BOOL CheckSum_Module(USHORT group);		// Sum (from 0x0001 to 0x000A)%256
	virtual	BOOL CheckSum_AWB(USHORT group);			// Sum (from 0x0011 to 0x001C)%256
	virtual	BOOL CheckSum_LSC(USHORT group);			// valSize of LSC is 1768 Bytes 17 x 13 x 4 (channel) x 2 (bytes)= 1768 Sum(0x0021 ~ 0x0708)%25
	virtual	BOOL CheckSum_AF(USHORT group);				// Sum(0x0741 ~ 0x0744)%256
	virtual	BOOL CheckAbnormal(USHORT *pVal, int valSize);           // 检测数据是否连续为0或ff
//	virtual BOOL SaveLSCData(USHORT *pVal,int size);
	virtual BOOL checkSum_Module_Base(USHORT group);
	virtual BOOL checkSum_LSC_Base(USHORT group);
	virtual BOOL checkSum_AWB_Base(USHORT group);

	virtual INT SumCalc(USHORT* pVal, int valSize,int mod,int add);

	virtual	BOOL CheckCode_Stc();				
	virtual	BOOL CheckCode_Inf();
	virtual	BOOL CheckCode_Mac();

public:
//	DccmControl_Interface* m_pInterface;

	typedef struct tagAddressFlag
	{// Flag的Address
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;

	}AddressFlag;

	typedef struct tagAddressSum
	{// Sum的Address
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;

	}AddressSum;

	typedef struct tagAddressBaseSum
	{// Sum的Address
		WORD moduleInfo;
		WORD awb;
		WORD lsc;

	}AddressBaseSum;

	typedef struct tagBaseredu
	{//BaseSum的redu
		WORD moduleInfo;
		WORD awb;
		WORD lsc;

	}Baseredu;

	typedef struct tagredu
	{//BaseSum的redu
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;

	}redu;

	typedef struct tagSumSize
	{// Sum占几个字节
		BYTE moduleInfo;
		BYTE awb;
		BYTE lsc;
		BYTE af;
	}SumSize;

	typedef struct tagOtpSize
	{// 每个块占用的空间
		int moduleInfo;
		int awb;
		int lsc;
		int af;

	}OtpSize;

	typedef struct tagOtpPage
	{// 有Page类型的空间所在Page
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;
		WORD pdaf;
		WORD pdaf_coef_k;

	}OtpPage;

	typedef struct tagAddressCode
	{// AF Code的Address
		WORD stcL;
		WORD stcH;
		WORD infL;
		WORD infH;
		WORD macL;
		WORD macH;

	}AddressCode;

	AddressFlag m_addrFlag;
	AddressSum	m_addrSum;
	AddressBaseSum m_addrBaseSum;
	redu m_redu;
	Baseredu m_Baseredu;
	SumSize    m_Sumsize;
	OtpSize		m_otpSize;
	OtpPage		m_otpPage;
	AddressCode	m_addrCode;

	// slave addr
	WORD	m_slaveSensor;
	WORD	m_slaveEeprom;
	BYTE	m_modeSensor;//iic mode
	BYTE	m_modeEeprom;//iic mode
	BYTE	m_regSize;	// used for multiple read

	// valid flag val
	WORD m_flagEmpty;
	WORD m_flagValid;	
	WORD m_flagGroup1;
	WORD m_flagGroup2;
	WORD m_flagGroup3;

	CString m_szInfo;

	BOOL m_bCheckFlag_ModuleInfo;
	BOOL m_bCheckFlag_AWB;
	BOOL m_bCheckFlag_LSC;
	BOOL m_bCheckFlag_AF;
	BOOL m_bCheckFlag_PDAF;

	BOOL m_bCheckSum_ModuleInfo;
	BOOL m_bCheckSum_AWB;
	BOOL m_bCheckSum_LSC;
	BOOL m_bCheckSum_AF;
	BOOL m_bCheckSum_PDAF;

	// AF Code
// 	BOOL m_bCheckStc;	// 起始Code
// 	BOOL m_bCheckInf;	// 远景Code
// 	BOOL m_bCheckMac;	// 近景Code
// 
// 	WORD m_stcLower;	// 起始Code下边界
// 	WORD m_stcUpper;	// 起始Code上边界
// 	WORD m_infLower;	//
// 	WORD m_infUpper;	//
// 	WORD m_macUpper;	//
// 	WORD m_macLower;	//

//	Hi843 m_hi843;
};

