#pragma once

#include "OtpCheckBase.h"

class OtpCheck_FHB6551M :public OtpCheckBase
{
public:
	OtpCheck_FHB6551M(void);
	~OtpCheck_FHB6551M(void);

	virtual BOOL Check_ModuleInfo();
	virtual BOOL Check_AWB();
	virtual BOOL Check_LSC();
	virtual BOOL Check_AF();
	virtual BOOL Check_PDAF();

	virtual BYTE SumCalc(BYTE* pVal, int valSize);// Sum()%255+1

#ifdef IIC_SENSOR_FHB6551M
	virtual BOOL ReadSensorPage_Byte(WORD reg, BYTE* val, BYTE page);	// 从Sensor指定寄存器读取一个BYTE
	virtual BOOL WriteSensorPage_Byte(WORD reg, BYTE val, BYTE page);	// 从Sensor写入寄存器写入一个BYTE
	virtual BOOL ReadSensorPage_Word(WORD reg, WORD* val, BYTE page);	// 从Sensor指定寄存器读取二个BYTE
	virtual BOOL WriteSensorPage_Word(WORD reg, WORD val, BYTE page);		// 从Sensor写入寄存器写入二个BYTE
	virtual BYTE ReadSensorPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop = FALSE);
	virtual BYTE WriteSensorPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page);
#else
// 	virtual BOOL ReadEepromPage_Byte(WORD reg, BYTE* val, BYTE page);	// 从Eeprom指定寄存器读取一个BYTE
// 	virtual BOOL WriteEepromPage_Byte(WORD reg, BYTE val, BYTE page);	// 从Eeprom写入寄存器写入一个BYTE
	virtual BOOL ReadEeprom_Byte(WORD reg, BYTE* val); // 从Eeprom指定寄存器读取一个BYTE
	virtual BOOL WriteEeprom_Byte(WORD reg, BYTE val); // 从Eeprom指定寄存器读取一个BYTE
/*	virtual BOOL ReadEepromPage_Word(WORD reg, WORD* val, BYTE page);	// 从Eeprom指定寄存器读取二个BYTE
	virtual BOOL WriteEepromPage_Word(WORD reg, WORD val, BYTE page);		// 从Eeprom写入寄存器写入二个BYTE*/
// 	virtual BYTE ReadEepromPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop = FALSE);
// 	virtual BYTE WriteEepromPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page);
#endif

private:
	virtual	BOOL CheckFlag_ModuleInfo();	
	virtual	BOOL CheckFlag_AWB();			
	virtual	BOOL CheckFlag_LSC();			
	virtual	BOOL CheckFlag_AF();			
 	virtual	BOOL CheckFlag_PDAF();	// Step1、Step2	

	virtual BOOL CheckFlag(BOOL flag,int addr, CString str);
	virtual	BOOL CheckSum_ModuleInfo();		
	virtual	BOOL CheckSum_AWB();			
	virtual	BOOL CheckSum_LSC();	

	virtual	BOOL CheckAbnormal(BYTE *pVal,int size);
	virtual	BOOL CheckSum_AF();				
 	BOOL CheckSum_PDAF_Step1();		// Step1	
	BOOL CheckSum_PDAF_Step2();		// Step2

	BOOL SaveLSCData(BYTE *pVal,int size);
	BOOL SaveOTPData();	

	virtual	BOOL CheckCode_Stc();				
	virtual	BOOL CheckCode_Inf();
	virtual	BOOL CheckCode_Mac();

private:
	typedef struct tagAddressFlag
	{// Flag的Address
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;
		WORD pdaf;
		WORD pdafStep2Start;

	}AddressFlag;

	typedef struct tagAddressSum
	{// Sum的Address
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;
		WORD pdafStep1;
		WORD pdafStep2;

	}AddressSum;

	typedef struct tagOtpSize
	{// 每个块占用的空间
		int moduleInfo;
		int awb;
		int lsc;
		int af;
		int pdafStep1;
		int pdafStep2;

	}OtpSize;

	typedef struct tagOtpPage
	{// 有Page类型的空间所在Page
		BYTE moduleInfo;
		BYTE awb;
		BYTE lsc;
		BYTE af;
		BYTE pdafStep1;
		BYTE pdafStep1_End;
		BYTE pdafStep2;
		BYTE pdafStep2_End;

	}OtpPage;

	AddressFlag m_addrFlag;
	AddressSum	m_addrSum;
	OtpSize		m_otpSize;
	OtpPage		m_otpPage;

	BYTE		m_pageSize;
	enum eEepromCommandType
	{
		EEPROM_Read		 = 0, //0x00
		EEPROM_Write	 = 1, //0x01
		EEPROM_Page_Erase= 2, //0x10
		EEPROM_All_Erase = 3, //0x11
	};
};

