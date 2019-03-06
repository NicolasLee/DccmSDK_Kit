#pragma once

#include <vector>
#include <Windows.h>

#include "OtpDefine_HLTE1M.h"

class COtpHLTE1M : public COtpdefineHLTE1M
{
public:
	COtpHLTE1M(void);
	~COtpHLTE1M(void);

	////////////////////////////////Init Otp//////////////////////////////////////
	BOOL EraseAllData(void);
	void   SetWBDefaultGain(void);

	/////////////////////////////////LSC///////////////////////////////////////

	////////////////////////////////AWB////////////////////////////////////
	BOOL GetAWB(void);
	BOOL CheckOTPAWB(void);
    BOOL GetAWBInfo(tagAwbRatio* wb);
	BOOL S5K4H8_GetAWBData(S5K4H8OTPDataSpec &awbdata);
	BOOL S5K4H8_GetCurrentAWB(tagAwbRatio* wb);
	BOOL S5K4H8_CheckAWB(tagAwbRatio wb,int type);

	/////////////////////////////Burning/////////////////////////////////////////
	void   CreateBurningData(void);
	void   S5K4H8_GetBasicData(void);
	void   S5K4H8_GetAFData(void);
	void   S5K4H8_CalcAllCheckSum(void);
	BOOL S5K4H8_OtpWrite(void);
	BOOL CheckSensorOTPEmpty(void);
	BOOL WriteOTP(void);
	BOOL WriteOTP(vector<double>& vData);
	BOOL ProgrammingBasic(void);
	BOOL Programming_4H8Basic(void);
	BOOL ProgrammingAWB(void);
	BOOL Programming_4H8AWB(void);
	BOOL ProgrammingLSC(void);
	BOOL Programming_4H8LSC(void);
	BOOL ProgrammingAF(void);
	BOOL Programming_4H8AF(void);
	BOOL ProgrammingPDAF(void);

	void SaveOTPData();	
	void SaveOTPData(vector<double>& vData);
	/////////////////////////////////Verify/////////////////////////////////////
	BOOL CheckOTP(void);
	BOOL S5K4H8_OTPVerify(void);
	BOOL S5K4H8_CheckOTP(void);
	BOOL S5K4H8_CheckOTPReadWrite(void);
	BOOL Sensor4H8_CheckReadWrite(void);
	BOOL Sensor4H8_CheckLSCReadWrite(void);
	BOOL S5K4H8_VerifyLSC();
	BOOL S5K4H8_VerifyAWB();
	BOOL VerifyLSC(void);
	BOOL VerifyPreWB(void);
	BOOL VerifyAWB(void);
	BOOL GetOTPRGBG(void);

	//////////////////////////////////////////////////////////////////////////

protected:
	void PreSubclassOtp(void);
	void SetOtpSensorParams(void);
private:
    OtpGroup Data_Write,Data_Read;
	tagAwbRatio m_wbCurrent;
	S5K4H8OTPDataSpec  OTP_Data_Write,OTP_Data_Read;

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

	BYTE S5K4H8_LSCWriteData[448];
	BYTE S5K4H8_LSCReadData[448];
	int S5K4H8_LSCchecksum;
	BYTE S5K4H8_SensorLSCSum;
};