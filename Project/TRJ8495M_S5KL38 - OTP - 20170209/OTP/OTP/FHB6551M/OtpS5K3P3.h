#pragma once

#include <vector>
#include <Windows.h>
#include "..//OtpBase.h"
#include "OtpDefineS5K3P3.h"

class COtpS5K3P3 : public COtpBase
{
public:
	COtpS5K3P3(void);
	~COtpS5K3P3(void);

	////////////////////////////////Init Otp//////////////////////////////////////
	BOOL EraseAllData(void);
	void   SetWBDefaultGain(void);
	void   S5K3P3_SetWBGain(USHORT gainGr, USHORT gainR, USHORT gainB, USHORT gainGb);

	/////////////////////////////////LSC///////////////////////////////////////

	////////////////////////////////AWB////////////////////////////////////
	BOOL GetAWB(void);
	BOOL CheckOTPAWB(void);
    BOOL GetAWBInfo(tagAwbRatio* wb);
	BOOL S5K3P3_GetAWBData(S5K3P3OTPDataSpec &awbdata);
	BOOL S5K3P3_GetCurrentAWB(tagAwbRatio* wb);
	void S5K3P3_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG);
	BOOL S5K3P3_CheckAWB(tagAwbRatio wb,int type);

	/////////////////////////////Burning/////////////////////////////////////////
	void   CreateBurningData(void);
	void   S5K3P3_GetBasicData(void);
	void   S5K3P3_GetAFData(void);
	void   S5K3P3_CalcAllCheckSum(void);
	BOOL S5K3P3_OtpWrite(void);
	BOOL CheckSensorOTPEmpty(void);
	BOOL WriteOTP(void);
	BOOL WriteOTP(vector<double>& vData);
	BOOL ProgrammingBasic(void);
	void Programming_3P3Basic(void);
	BOOL ProgrammingAWB(void);
	void Programming_3P3AWB(void);
	BOOL ProgrammingLSC(void);
	BOOL Programming_3P3LSC(void);
	void S5K3P3_WriteLSCFlagSum(void);
	BOOL ProgrammingAF(void);
	void Programming_3P3AF(void);
	BOOL ProgrammingPDAF(void);

	void SaveOTPData();	
	void SaveOTPData(vector<double>& vData);
	/////////////////////////////////Verify/////////////////////////////////////
	BOOL CheckOTP(void);
	BOOL S5K3P3_OTPVerify(void);
	BOOL S5K3P3_CheckOTPReadWrite(void);
	BOOL Sensor3P3_CheckReadWrite(void);
	BOOL Sensor3P3_CheckLSCReadWrite(void);
	BOOL S5K3P3_VerifyLSC();
	BOOL S5K3P3_VerifyAWB();
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
	S5K3P3OTPDataSpec  OTP_Data_Write,OTP_Data_Read;

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

	BYTE S5K3P3_LSCWriteData[512];
	BYTE S5K3P3_LSCReadData[512];
	int S5K3P3_LSCchecksum;
	int Applyawbdata[7];
};