#pragma once
#include "DccmBase.h"

#define I2C_RETRY_NUM    4
#define I2C_RETRY_WAIT   1  // 1ms
#define UNEXCEPT_REG 0xFFFF
#define UNEXCEPT_VALUE 0x0000

enum ZC533_Type
{
	ZC533_READ		 = 0, //0x00
	ZC533_WRITE	 = 1, //0x01
	ZC533_PAGE_ERASE= 2, //0x10
	ZC533_ALL_ERASE = 3, //0x11
};

enum eProjectName	//项目名称 PN_
{
	PN_NULL = -1,
	PN_BYD_TKU8341M = 0,
	PN_BYD_THY8348M,
	PN_HLT_EYD2467M,
	PN_BYD_TYD8470M,
	PN_HLT_HLT6014,
	PN_BYD_FHB6551M,
	PN_BYD_PLQ8579M,
	PN_BYD_TRJ8495M,
};


class OtpCheckBase
{
public:
	OtpCheckBase(void);
	~OtpCheckBase(void);

	virtual BOOL CheckValidGroup();
	virtual BOOL Check_ModuleInfo();
	virtual BOOL Check_AWB();
	virtual BOOL Check_LSC();
	virtual BOOL Check_AF();	// AF + AF_Code
	virtual BOOL Check_PDAF();	// PDAF + PDAF_Coef_K
	virtual BOOL SaveOTPData();	
	////////////////////////////////EEPROM////////////////////////////////
	BOOL GT24C64_Read(USHORT address,unsigned char *Data,int size);
	BOOL GT24C64_Programming(USHORT address,unsigned char *Data,int size);

	BOOL ZC533_Read(USHORT address,unsigned char *Data,int size);//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop /* = FALSE */

	BOOL DW9763_Read(USHORT address,unsigned char *Data,int size);//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop /* = FALSE */

	virtual BOOL i2c_write(USHORT reg, USHORT val);
	virtual BOOL i2c_read(USHORT reg, USHORT *pval);
	virtual BOOL i2c_write_byte(USHORT reg, BYTE val);
	virtual BOOL i2c_read_byte(USHORT reg, BYTE *pval);
private:
	virtual BOOL ReadSensor_Byte(WORD reg, BYTE* val);	// 从Sensor指定寄存器读取一个BYTE
	virtual BOOL WriteSensor_Byte(WORD reg, BYTE val);	// 从Sensor写入寄存器写入一个BYTE
	virtual BOOL ReadSensor_Word(WORD reg, WORD* val);	// 从Sensor指定寄存器读取二个BYTE
	virtual BOOL WriteSensor_Word(WORD reg, WORD val);	// 从Sensor写入寄存器写入二个BYTE
	virtual BYTE ReadSensor_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BOOL bStop = FALSE);
	virtual BYTE WriteSensor_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize);

	virtual BOOL ReadEeprom_Byte(WORD reg, BYTE* val); // 从SENSOR指定寄存器读取一个BYTE
	virtual BOOL WriteEeprom_Byte(WORD reg, BYTE val); // 从SENSOR写入寄存器写入一个BYTE
	virtual BOOL ReadEeprom_Word(WORD reg, WORD* val); // 从SENSOR指定寄存器读取二个BYTE
	virtual BOOL WriteEeprom_Word(WORD reg, WORD val); // 从SENSOR写入寄存器写入二个BYTE
	virtual BYTE ReadEeprom_MultiBytes(USHORT reg, BYTE regSize, BYTE *pVal, USHORT valSize, BOOL bStop = FALSE);
	virtual BYTE WriteEeprom_MultiBytes(USHORT reg, BYTE regSize, BYTE *pVal, USHORT valSize);
	
	virtual BOOL ReadSensorPage_Byte(WORD reg, BYTE* val, BYTE page);	// 从Sensor指定寄存器读取一个BYTE
	virtual BOOL WriteSensorPage_Byte(WORD reg, BYTE val, BYTE page);	// 从Sensor写入寄存器写入一个BYTE
	virtual BOOL ReadSensorPage_Word(WORD reg, WORD* val, BYTE page);	// 从Sensor指定寄存器读取二个BYTE
	virtual BOOL WriteSensorPage_Word(WORD reg, WORD val, BYTE page);	// 从Sensor写入寄存器写入二个BYTE
	virtual BYTE ReadSensorPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop = FALSE);
	virtual BYTE WriteSensorPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page);
	
	virtual BOOL ReadEepromPage_Byte(WORD reg, BYTE* val, BYTE page);	// 从Eeprom指定寄存器读取一个BYTE
	virtual BOOL WriteEepromPage_Byte(WORD reg, BYTE val, BYTE page);	// 从Eeprom写入寄存器写入一个BYTE
	virtual BOOL ReadEepromPage_Word(WORD reg, WORD* val, BYTE page);	// 从Eeprom指定寄存器读取二个BYTE
	virtual BOOL WriteEepromPage_Word(WORD reg, WORD val, BYTE page);	// 从Eeprom写入寄存器写入二个BYTE
	virtual BYTE ReadEepromPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop = FALSE);
	virtual BYTE WriteEepromPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page);

	// Check Flag
	virtual	BOOL CheckFlag_ModuleInfo();	
	virtual	BOOL CheckFlag_AWB();			
	virtual	BOOL CheckFlag_LSC();			
	virtual	BOOL CheckFlag_AF();			
	virtual	BOOL CheckFlag_PDAF();			

	// Check Sum
	virtual	BOOL CheckSum_ModuleInfo();		
	virtual	BOOL CheckSum_AWB();			
	virtual	BOOL CheckSum_LSC();			
	virtual	BOOL CheckSum_AF();				
	virtual	BOOL CheckSum_PDAF();	
	virtual BYTE SumCalc(BYTE* pVal, int valSize);// Sum()%256

	// Check AF Code
	virtual	BOOL CheckCode_Stc();				
	virtual	BOOL CheckCode_Inf();
	virtual	BOOL CheckCode_Mac();

public:
	DccmControl_Interface* m_pInterface;

	typedef struct tagAddressFlag
	{// Flag的Address
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;
		WORD pdaf;
		WORD pdaf_coef_k;

	}AddressFlag;

	typedef struct tagAddressSum
	{// Sum的Address
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;
		WORD pdaf;
		WORD pdaf_coef_k;

	}AddressSum;

	typedef struct tagOtpSize
	{// 每个块占用的空间
		int moduleInfo;
		int awb;
		int lsc;
		int af;
		int pdaf;
		int pdaf_coef_k;

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
	WORD m_flagValid;
	WORD m_flagEmpty;
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
	BOOL m_bCheckStc;	// 起始Code
	BOOL m_bCheckInf;	// 远景Code
	BOOL m_bCheckMac;	// 近景Code

	WORD m_stcLower;	// 起始Code下边界
	WORD m_stcUpper;	// 起始Code上边界
	WORD m_infLower;	//
	WORD m_infUpper;	//
	WORD m_macUpper;	//
	WORD m_macLower;	//

};

