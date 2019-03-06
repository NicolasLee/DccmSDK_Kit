#pragma once

#include <vector>
#include <Windows.h>
#include "OtpDefine_HLT7012.h"

class COtpHLT7012 : public COtpdefineHLT7012
{
public:
	COtpHLT7012(void);
	~COtpHLT7012(void);

	////////////////////////////////Init Otp//////////////////////////////////////
	BOOL EraseAllData(void);
	void   SetWBDefaultGain(void);
	void   HLT7012_SetWBGain(USHORT gainGr, USHORT gainR, USHORT gainB, USHORT gainGb);

	/////////////////////////////////LSC///////////////////////////////////////

	////////////////////////////////AWB////////////////////////////////////
	BOOL GetAWB(void);
	BOOL CheckOTPAWB(void);
    BOOL GetAWBInfo(tagAwbRatio* wb);
	BOOL HLT7012_GetAWBData(HLT7012OTPDataSpec &awbdata);
	BOOL HLT7012_GetCurrentAWB(tagAwbRatio* wb);
	void HLT7012_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG);
	BOOL HLT7012_CheckAWB(tagAwbRatio wb,int type);

	/////////////////////////////Burning/////////////////////////////////////////
	void   CreateBurningData(void);
	void   HLT7012_GetBasicData(void);
	void   HLT7012_GetAFData(void);
	void   HLT7012_CalcAllCheckSum(void);
	BOOL HLT7012_OtpWrite(void);
	BOOL CheckSensorOTPEmpty(void);
	BOOL WriteOTP(void);
	BOOL WriteOTP(vector<double>& vData);
	BOOL ProgrammingBasic(void);
	void Programming_3L8Basic(void);
	BOOL ProgrammingAWB(void);
	void Programming_3L8AWB(void);
	BOOL ProgrammingLSC(void);
	BOOL Programming_3L8LSC(void);
	void HLT7012_WriteLSCFlagSum(void);
	BOOL ProgrammingAF(void);
	void Programming_3L8AF(void);
	BOOL ProgrammingPDAF(void);

	void SaveOTPData();	
	void SaveOTPData(vector<double>& vData);
	/////////////////////////////////Verify/////////////////////////////////////
	BOOL CheckOTP(void);
	BOOL HLT7012_OTPVerify(void);
	BOOL HLT7012_CheckOTPReadWrite(void);
	BOOL Sensor3L8_CheckReadWrite(void);
	BOOL Sensor3L8_CheckLSCReadWrite(void);
	BOOL HLT7012_VerifyLSC();
	BOOL HLT7012_VerifyAWB();
	BOOL VerifyLSC(void);
	BOOL VerifyPreWB(void);
	BOOL VerifyAWB(void);
	BOOL GetOTPRGBG(void);

	//////////////////////////////////////////////////////////////////////////
//	BOOL InitProgram(void);
	BOOL StartProgram(void);
//	BOOL EndProgram(void);



protected:
	void PreSubclassOtp(void);
	void SetOtpSensorParams(void);
private:
    OtpGroup Data_Write,Data_Read;
	tagAwbRatio m_wbCurrent;
	HLT7012OTPDataSpec  OTP_Data_Write,OTP_Data_Read;

public:
	CString info;

	typedef struct tagAddressFlag
	{// Flag的Address
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;
		WORD pdaf1;
		WORD pdaf2;
		WORD pdaf;

		WORD l3pdaf1;
		WORD l3pdaf2;
		WORD l3pdaf;
	}AddressFlag;

	typedef struct tagAddressSum
	{// Sum的Address
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;
		WORD pdaf1;
		WORD pdaf2;
		WORD pdaf;

		WORD l3pdaf1;
		WORD l3pdaf2;
		WORD l3pdaf;
	}AddressSum;

	typedef struct tagOtpSize
	{// 每个块占用的空间
		int moduleInfo;
		int awb;
		int lsc;
		int af;
		int pdaf;
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
	WORD m_flagEmpty;
	WORD m_flagValid;
// 	WORD m_flagGroup1;
// 	WORD m_flagGroup2;
// 	WORD m_flagGroup3;
	
	int proc1sum;

	BYTE HLT7012_LSCWriteData[512];
	BYTE HLT7012_LSCReadData[512];
	int HLT7012_LSCchecksum;
};