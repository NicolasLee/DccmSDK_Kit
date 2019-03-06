#pragma once

#include "OtpDef.h"
#include "DccmBase.h"
#include "ImageProc.h"

// base otp class of PlatfomrTool and inner Sensor

#define I2C_RETRY_NUM    4
#define I2C_RETRY_WAIT   1  // 1ms
class COtpBase
{
public:
	DccmControl_Interface* m_pInterface;
	CString m_szStr;

	void SetRoi(CRect &roi, UINT width, UINT height, BYTE roiCalcMode, UINT x=0, UINT y=0, UINT roiWidth=0, UINT roiHeight=0);
	void FormatRoiRect(CRect &rt, UINT width, UINT height);
	BOOL CheckEqual(BYTE *data1, BYTE *data2, int num);
public:	// ÉÕÂ¼Á÷³Ì
	BOOL InitProgram(void);
	virtual BOOL StartProgram(void);
	virtual BOOL PreOtp(void);
	virtual BOOL OtpWrite(void) = 0;
	virtual BOOL OtpVerify(void) = 0;
	virtual BOOL OtpRead(void) = 0;
	virtual BOOL CheckOtpEmpty(void) = 0;
	virtual BOOL CheckOtpValid(void) = 0;
	virtual void ResetOtpGroup(void) = 0;

	// awb
	virtual BOOL ProcAWB(void);
	virtual BOOL InitAWB(void);
	virtual BOOL UpdateAWB();
	virtual BOOL ApplyAWB(const tagAwbRatio *wbCurrent, const tagAwbRatio *wbGolden);
	virtual BOOL GetAWB(tagAwbRatio* wb);
	virtual BOOL CheckAWB(void);
	virtual void ShowUpdateInfoAWB(void);

	virtual BOOL CheckWBGoldenValue(void);
	BOOL CheckWBDifference(void);
	BOOL CheckWBCurrentValue(void);
	BOOL GetWBGainRatio(UINT goldenRG, UINT goldenBG, UINT currentRG, UINT currentBG, double &ratioR, double &ratioG, double &ratioB);
	void SetWBDefaultGain(void);
	void WriteWBReg(USHORT addrHigh, USHORT addrLow, USHORT gain);
	void SetWBGain(USHORT gainGr, USHORT gainR, USHORT gainB, USHORT gainGb);
	USHORT WBRatioToRegGain(double ratio);
	virtual BOOL CreateWBGroup(const tagAwbRatio *wbCurrent, const tagAwbRatio *wbGolden);
	virtual BOOL LoadWBSettings(tagAwbRatio *wbCurrent, tagAwbRatio *wbCurrentGolden);


	// lsc
	virtual BOOL InitLSC(void);
	virtual BOOL ProcLSC(void);
	virtual BOOL GetLSC(BYTE *lencData, int inputTarget);
	virtual BOOL ApplyLSC(BYTE *data, BYTE *lencData);
	virtual BOOL UpdateLSC(void);
	virtual BOOL ApplyLSC(const BYTE *lencData);
	virtual BOOL CheckLSC(void);
	virtual void ShowUpdateInfoLSC(void);

	virtual BOOL GetDynamicLenc(void);
	virtual BOOL CreateLencGroup(const BYTE *lencData);
	virtual BOOL LoadLencSettings(BYTE *lencData);

	BYTE* GetLencData(void) const;
	virtual void EnableLenc(void);
	virtual void DisableLenc(void);

	// basic
	virtual BOOL InitBasic(void);
	virtual BOOL UpdateBasic(void);
	virtual BOOL CheckBasic(void);
	virtual void ShowUpdateInfoBasic(void);

	virtual BOOL CreateBasicGroup(const OtpIDStruct *ID);
	virtual BOOL LoadBasicSettings(OtpIDStruct *IDCurrent);

	BOOL UpdateAF();
	BOOL UpdateOtp(void);

public:

	virtual void PreSubclassOtp(void) = 0;
	virtual void SetOtpSensorParams(void) = 0;





public:	// 
	CString m_otpName;
	CString SetOtpName(CString otpName);
	CString GetOtpName(void);

	int CalcCheckSum(BYTE *data, int num, int mod);
	void SetSensorOB(BYTE OB);	
	BYTE GetSensorOB(void) const;

	static OtpSensorStruct m_otpSensor;
	static OtpPowerStruct m_otpPower;
	static tagOtpCtrl m_otpCtrl;
	static OtpSensorStruct* GetOtpSensorParams(void);
	static OtpPowerStruct* GetOtpPowerParams(void);
	static tagOtpCtrl* GetControlParams(void);

	static OtpIDStruct m_IDGroup;
	OtpIDStruct m_IDCurrent;
	static OtpIDStruct* GetIDGroup(void);	
	OtpIDStruct* GetIDCurrent(void);		

	BYTE m_sensorOB;
	static tagAwbOtpCtrl m_wbCtrl;
	static WBGainRegStruct m_wbGainReg;
	static tagAwbRatio m_wbGolden;
	tagAwbRatio m_wbCurrentGolden;
	tagAwbRatio m_wbCurrent;
	tagAwbRatio m_wbCorrect;
	static tagAwbOtpCtrl* GetWBControlParams(void);
	static WBGainRegStruct* GetWBGainRegister(void);					 
	static tagAwbRatio* GetWBGoldenValue(void);	
	tagAwbRatio* GetWBCurrentGoldenValue(void);					 
	tagAwbRatio* GetWBCurrentValue(void);		

	static LencOtpControlStruct m_lencCtrl;
	static LencRegStruct m_lencReg;
	BYTE *m_lencData;
	static LencOtpControlStruct* GetLencControlParams(void);
	static LencRegStruct* GetLencRegister(void);	

	static ExpControlStruct m_expCtrl;
	static ExpRegisterStruct m_expRegister;
	static ExpRangeStruct m_expRange;
	ExpStatusStruct m_expStatus;
	static ExpControlStruct* GetExpControlParams(void); 
	static ExpRegisterStruct* GetExpRegister(void);					 
	static ExpRangeStruct* GetExpRangeParams(void); 




	EepromStruct m_Eeprom;
	void SetEepromParams(EepromStruct *eeprom);

	BOOL eeprom_write(USHORT addr, USHORT reg, BYTE val);
	BOOL eeprom_read(USHORT addr, USHORT reg, BYTE *pval);	
	BOOL eeprom_multi_write(USHORT addr, USHORT reg, BYTE *pval, USHORT length);
	BOOL eeprom_multi_read(USHORT addr, USHORT reg, BYTE *pval, USHORT length);

	BOOL eeprom_write(USHORT reg, BYTE val);
	BOOL eeprom_read(USHORT reg, BYTE *pval);	
	BOOL eeprom_multi_write(USHORT reg, BYTE *pval, USHORT length);
	BOOL eeprom_multi_read(USHORT reg, BYTE *pval, USHORT length);

	BOOL i2c_write(USHORT reg, USHORT val);
	BOOL i2c_read(USHORT reg, USHORT *pval);	
	BOOL i2c_multi_write(USHORT reg, BYTE *pval, USHORT length);
	BOOL i2c_multi_read(USHORT reg, BYTE *pval, USHORT length, BOOL noStop=FALSE);
	BOOL i2c_write(USHORT addr, USHORT reg, USHORT val, BYTE mode);
	BOOL i2c_read(USHORT addr, USHORT reg, USHORT *pval, BYTE mode);
	BOOL i2c_multi_write(USHORT addr, USHORT reg, BYTE regSize, BYTE* pval, USHORT length);
	BOOL i2c_multi_read(USHORT addr, USHORT reg, BYTE regSize, BYTE* pval, USHORT length, BOOL noStop=FALSE);

};