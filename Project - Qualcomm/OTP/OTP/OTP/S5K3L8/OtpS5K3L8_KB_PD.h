#pragma once
#include "..\\MTK_EEPROM\\OtpMtkEeprom_15X15.h"
#include "OtpDefineS5K3L8_KB_PD.h"
#include "..\\OtpBase.h"

#include <vector>
using namespace std;

class COtpS5K3L8_KB_PD : public COtpBase
{
public:
	COtpS5K3L8_KB_PD(void);
	~COtpS5K3L8_KB_PD(void);

	COtpMtkEeprom_15X15 *MtkEeprom;
protected:
	 void PreSubclassOtp(void);
	 void SetOtpSensorParams(void);


	 BOOL OtpWrite(void);
	 BOOL OtpVerify(void);
	 BOOL OtpRead(void);

	 BOOL CheckOtpEmpty();
	 BOOL CheckOtpValid();
	BOOL isCheckSumRight();
	BOOL isCheckAFSumRight();
	BOOL isCheckPDAFSumRight();
	 void ResetOtpGroup();

	 BOOL LoadBasicSettings(OtpIDStruct *IDCurrent);
	 BOOL CreateBasicGroup(const OtpIDStruct* ID);
	void ShowUpdateInfoBasic();

	 BOOL LoadWBSettings(tagAwbRatio* wbCurrent,tagAwbRatio* wbGolden);
	 BOOL CreateWBGroup(const tagAwbRatio* wbCurrent,const tagAwbRatio* wbGolden);
	 void ShowUpdateInfoAWB();

	 BOOL LoadAFSettings();
	void ShowAFUpdateInfo();

	 BOOL GetLSC(BYTE *lencData, int inputTarget);
	 BOOL CreateLencGroup(const BYTE *lencData);

	void ResetWriteGroup(void);
	void ResetReadGroup(void);
	BOOL OTPReadAll();
	BOOL IsContinueOTP();

private:
	S5K3L8_KB_PDEEPROMOtpGroup m_writeGroup;
	S5K3L8_KB_PDEEPROMOtpGroup m_readGroup;
	
};

