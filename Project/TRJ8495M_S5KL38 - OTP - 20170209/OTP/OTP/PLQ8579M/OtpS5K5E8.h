#pragma once

#include <vector>
#include <Windows.h>
#include "..//OtpBase.h"
#include "OtpDefineS5K5E8.h"

class COtpS5K5E8 : public COtpBase
{
public:
	COtpS5K5E8(void);
	~COtpS5K5E8(void);

	////////////////////////////////Init Otp//////////////////////////////////////
	BOOL EraseAllData(void);
	void   SetWBDefaultGain(void);
	void   S5K5E8_SetWBGain(USHORT gainGr, USHORT gainR, USHORT gainB, USHORT gainGb);

	/////////////////////////////////LSC///////////////////////////////////////

	////////////////////////////////AWB////////////////////////////////////
	BOOL GetAWB(void);
	BOOL CheckOTPAWB(void);
    BOOL GetAWBInfo(tagAwbRatio* wb);
	BOOL S5K5E8_GetAWBData(S5K5E8OTPDataSpec &awbdata);
	BOOL S5K5E8_GetCurrentAWB(tagAwbRatio* wb);
	void S5K5E8_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG);
	BOOL S5K5E8_CheckAWB(tagAwbRatio wb,int type);

	/////////////////////////////Burning/////////////////////////////////////////
	void   CreateBurningData(void);
	void   S5K5E8_GetBasicData(void);
	void   S5K5E8_GetAFData(void);
	void   S5K5E8_CalcAllCheckSum(void);
	BOOL S5K5E8_OtpWrite(void);
	BOOL CheckSensorOTPEmpty(void);
	BOOL WriteOTP(void);
	BOOL WriteOTP(vector<double>& vData);
	BOOL ProgrammingBasic(void);
	BOOL Programming_5E8Basic(void);
	BOOL ProgrammingAWB(void);
	BOOL Programming_5E8AWB(void);
	BOOL ProgrammingLSC(void);
	BOOL Programming_5E8LSC(void);
	BOOL ProgrammingAF(void);
	BOOL Programming_5E8AF(void);
	BOOL ProgrammingPDAF(void);

	void SaveOTPData();	
	void SaveOTPData(vector<double>& vData);
	/////////////////////////////////Verify/////////////////////////////////////
	BOOL CheckOTP(void);
	BOOL S5K5E8_OTPVerify(void);
	BOOL S5K5E8_CheckOTPReadWrite(void);
	BOOL Sensor5E8_CheckReadWrite(void);
	BOOL Sensor5E8_CheckLSCReadWrite(void);
	BOOL S5K5E8_VerifyLSC();
	BOOL S5K5E8_VerifyAWB();
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
	S5K5E8OTPDataSpec  OTP_Data_Write,OTP_Data_Read;

public:
	CString info;

	typedef struct tagAddressFlag
	{// Flag��Address
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;
		WORD pdaf1;
		WORD pdaf2;
		WORD pdaf;
	}AddressFlag;

	typedef struct tagAddressSum
	{// Sum��Address
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;
		WORD pdaf1;
		WORD pdaf2;
		WORD pdaf;

	}AddressSum;

	typedef struct tagOtpSize
	{// ÿ����ռ�õĿռ�
		int moduleInfo;
		int awb;
		int lsc;
		int af;
		int pdaf;
	}OtpSize;

	typedef struct tagOtpPage
	{// ��Page���͵Ŀռ�����Page
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;
		WORD pdaf;
		WORD pdaf_coef_k;

	}OtpPage;

	typedef struct tagAddressCode
	{// AF Code��Address
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

	BYTE S5K5E8_LSCWriteData[360];
	BYTE S5K5E8_LSCReadData[360];
	int S5K5E8_LSCchecksum;
	int Applyawbdata[7];
	BYTE S5K5E8_SensorLSCSum;
};