#include "stdafx.h"
#include "OtpS5K3L8_KB_PD.h"
#include "..//OTP_SensorSetting.h"


COtpS5K3L8_KB_PD::COtpS5K3L8_KB_PD(void)
{
	memset(&m_writeGroup, 0, sizeof(m_writeGroup));	
	memset(&m_readGroup, 0, sizeof(m_readGroup));	

	SetOtpName(_T("S5K3L8_PD"));

	PreSubclassOtp();
}


COtpS5K3L8_KB_PD::~COtpS5K3L8_KB_PD(void)
{

}

void COtpS5K3L8_KB_PD::PreSubclassOtp()
{
	tagOtpCtrl* control = GetControlParams();
	control->verifyReInit = FALSE;
	control->manualReInit = FALSE;
	control->enProgram = TRUE;
	control->enForceCalibrate = FALSE;
	control->enSaveRawBMP = FALSE;
	control->enSaveCorrectBMP = FALSE;
	control->enShowVideoAfterOtp = FALSE;
	control->enLenc = TRUE;
	control->enWB = TRUE;
	control->enCheckAF = TRUE;

	OtpIDStruct *ID = GetIDGroup();	
	ID->vendor = 0x45;
	ID->lens = 0x00;
	ID->vcm = 0x00;
	ID->driverIC = 0x00;

	tagAwbOtpCtrl *wbCtrl = GetWBControlParams();
	wbCtrl->enLencBasedWB = FALSE;
	wbCtrl->enSaveRawLencBMP = FALSE;
	wbCtrl->enCheckWB = TRUE;
	wbCtrl->percentDifRG = 5.0;
	wbCtrl->percentDifBG = 5.0;
	wbCtrl->enUpdateWB = TRUE;//TRUE;//
	wbCtrl->enSaveWBFile = FALSE;//TRUE;//
	wbCtrl->wbFileName = _T("WB.csv");
	wbCtrl->roiCalcMode = ROI_CALC_10;
	wbCtrl->factorAmplify = 1024;//1024;

	WBGainRegStruct* wbReg = GetWBGainRegister();
	wbReg->addrGrHigh = S5K3L8_KB_PD_ADDR_GAIN_GR_H;
	wbReg->addrGrLow  = S5K3L8_KB_PD_ADDR_GAIN_GR_L;
	wbReg->addrGbHigh = S5K3L8_KB_PD_ADDR_GAIN_GB_H;
	wbReg->addrGbLow  = S5K3L8_KB_PD_ADDR_GAIN_GB_L;
	wbReg->addrRHigh  = S5K3L8_KB_PD_ADDR_GAIN_R_H;
	wbReg->addrRLow   = S5K3L8_KB_PD_ADDR_GAIN_R_L;
	wbReg->addrBHigh  = S5K3L8_KB_PD_ADDR_GAIN_B_H;
	wbReg->addrBLow   = S5K3L8_KB_PD_ADDR_GAIN_B_L;
	wbReg->dataDefaultGain = S5K3L8_KB_PD_DATA_DEFAULT_GAIN;


	tagAwbRatio *wbGolden = GetWBGoldenValue();		
	wbGolden->rg  = 525;
	wbGolden->bg  = 565;


	LencOtpControlStruct *lencCtrl = GetLencControlParams();
	lencCtrl->enDynamicInput = FALSE;
	lencCtrl->inputHigh = 67;
	lencCtrl->inputTarget = 65;
	lencCtrl->inputLow = 63;
	lencCtrl->enCheckLenc = FALSE;//TRUE;//
	lencCtrl->enShadingTestAfterOtp = FALSE;	
	lencCtrl->shadingTestType = SHADING_TYPE_MTK;//SHADING_TYPE_QUALCOMM;//
	lencCtrl->enUpdateLenc = FALSE;	
	lencCtrl->enSaveLencFile = FALSE;//TRUE;//
	lencCtrl->lencFileName = _T("Lenc.csv");
	lencCtrl->enSaveRawLencFile = FALSE;
	lencCtrl->lencRawFileName = _T("LencRaw.csv");

	LencRegStruct *lencReg = GetLencRegister();
	lencReg->addrCtrl = REG_INVALID;
	lencReg->lencEnable = REG_INVALID;
	lencReg->lencDisable = REG_INVALID;
	lencReg->addrDataStart = REG_INVALID;
	lencReg->dataSize = S5K3L8_KB_PD_MTK_OTP_LSC_SIZE;

	ExpControlStruct *expCtrl = GetExpControlParams();
	expCtrl->type = EXP_LINEAR;
	expCtrl->roiCalcMode = ROI_CALC_10;
	expCtrl->filterChannel = EXP_CHANNEL_G;
	expCtrl->target = 160*4+64;
	expCtrl->lowLimit = 150*4+64;
	expCtrl->highLimit = 170*4+64;
	expCtrl->effectFrame = 3;
	expCtrl->stableFrame = 3;

	ExpRegisterStruct *expReg = GetExpRegister();
	expReg->mask = 0x00FFFF;
	expReg->addrHigh = REG_INVALID;
	expReg->addrMid = S5K3L8_KB_PD_ADDR_EXP_H;
	expReg->addrLow = S5K3L8_KB_PD_ADDR_EXP_L;

	ExpRangeStruct *expRange = GetExpRangeParams(); 
	expRange->def = 0x00f6;
	expRange->max = 0xFF00;
	expRange->min = 0;


	EepromStruct eeprom;
	eeprom.deviceID = 0xB0;
	eeprom.addrByteSize = 2;
	eeprom.pageSize = 64;
	eeprom.msWriteTime = 10;

	SetEepromParams(&eeprom);
}

void COtpS5K3L8_KB_PD::SetOtpSensorParams()
{
	OtpSensorStruct* params = GetOtpSensorParams();

	params->name = _T("S5K3L8_KB_PD_MIPI");
	params->RESET = PIN_HIGH;
	params->PWDN = PIN_HIGH;
	params->mvAVDD = 2800;
	params->mvDOVDD = 1800;
	params->mvCORE  = 1200;
	params->mvAFVCC = 2800;
	params->mvVPP   = 0;

	params->i2c.addr  = 0x5a;//0x20 CMA191 CMA332 CM8447 CM8442; 0x5A CMA286
	params->i2c.mode  = I2C_MODE_A16D8;
	params->i2c.speed = I2C_SPEED_400K;
	params->flagReg1  = 0x0000;
	params->flagData1 = 0x30;
	params->flagMask1 = 0xff;
	params->flagReg2  = 0x0001;
	params->flagData2 = 0xc8;
	params->flagMask2 = 0xff;

	params->port = SENSOR_PORT_MIPI;

	params->polarPCLK = FALSE;
	params->polarHSYNC = FALSE;

	params->regEffectFrame = 3;

	params->OB = 64;
	params->OBLine = 0;

	params->otpInit.width  = 4208;
	params->otpInit.height = 3120;
	params->otpInit.format = IMAGE_FORMAT_RAW10;
	params->otpInit.sequence = IMAGE_SEQUENCE_GBRG;
	params->otpInit.mhzMCLK = 24;
	params->otpInit.params = S5K3L8_OTP_Settings;
	params->otpInit.paramSize = sizeofS5K3L8_OTP_Settings;

	params->verifyInit.width  = 4208;
	params->verifyInit.height = 3120;
	params->verifyInit.format = IMAGE_FORMAT_RAW10;
	params->verifyInit.sequence = IMAGE_SEQUENCE_GBRG;
	params->verifyInit.mhzMCLK = 24;
	params->verifyInit.params = S5K3L8_OTP_Settings;
	params->verifyInit.paramSize = sizeofS5K3L8_OTP_Settings;	
}

BOOL COtpS5K3L8_KB_PD::CreateBasicGroup(const OtpIDStruct* ID)
{
	CTime time = CTime::GetCurrentTime();
	m_writeGroup.flag = S5K3L8_KB_PD_FLAG_VALID; 
	m_writeGroup.year = time.GetYear() - 2000;
	m_writeGroup.month = time.GetMonth();
	m_writeGroup.date  = time.GetDay();
	m_writeGroup.vendor = ID->vendor;
	m_writeGroup.lens  = ID->lens;
	m_writeGroup.vcm   = ID->vcm;
	m_writeGroup.driverIC  = ID->driverIC;
	m_writeGroup.IR = ID->IR;
	m_writeGroup.temperature = ID->temperature;
	m_writeGroup.afType = 0x01;
	m_writeGroup.lightSource = 0x02;
	m_writeGroup.lencDll = 0x30;
	m_writeGroup.calibrationVersion[0] = 0x01;
	m_writeGroup.calibrationVersion[1] = 0x00;
	m_writeGroup.calibrationVersion[2] = 0x00;
	m_writeGroup.calibrationVersion[3] = 0x00;
	m_writeGroup.serialNum[0] = 0x00;
	m_writeGroup.serialNum[1] = 0x00;
	m_writeGroup.serialNum[2] = 0x00;
	m_writeGroup.pdDll = 0x25;
	int sum = 0;
	for (int i=1;i<21;i++)
	{
		sum+= m_writeGroup.dataBasic[i];
	}

	m_writeGroup.checkSumBasic = sum %255 +1;

	return TRUE;
}

BOOL COtpS5K3L8_KB_PD::LoadBasicSettings(OtpIDStruct *IDCurrent)
{
	IDCurrent->vendor   = m_readGroup.vendor;
	IDCurrent->lens     = m_readGroup.lens;
	IDCurrent->vcm      = m_readGroup.vcm;
	IDCurrent->driverIC = m_readGroup.driverIC;
	IDCurrent->IR       = m_readGroup.IR;
	IDCurrent->temperature = m_readGroup.temperature;


	return TRUE;
}

void COtpS5K3L8_KB_PD::ShowUpdateInfoBasic()
{
	CString info;
	info.Format(_T("Flag: 0x%02x"),m_readGroup.flag);
	m_pInterface->AddLog(info);

	info.Format(_T("Year: %d"),m_readGroup.year);
	m_pInterface->AddLog(info);

	info.Format(_T("Month: %d"),m_readGroup.month);
	m_pInterface->AddLog(info);

	info.Format(_T("Day: %d"),m_readGroup.date);
	m_pInterface->AddLog(info);

	info.Format(_T("Vendor:0x%02x"),m_readGroup.vendor);
	m_pInterface->AddLog(info);

	info.Format(_T("Lens: 0x%02x"),m_readGroup.lens);
	m_pInterface->AddLog(info);
}

BOOL COtpS5K3L8_KB_PD::CreateWBGroup(const tagAwbRatio* wbCurrent,const tagAwbRatio* wbGolden)
{
	m_writeGroup.flagWB = S5K3L8_KB_PD_FLAG_VALID;
	m_writeGroup.currentRG_L = wbCurrent->rg&0xff;
	m_writeGroup.currentRG_H = (wbCurrent->rg>>8)&0xff;
	m_writeGroup.currentBG_L = wbCurrent->bg&0xff;
	m_writeGroup.currentBG_H = (wbCurrent->bg>>8)&0xff;
	m_writeGroup.currentGbGr_L = wbCurrent->gb_gr&0xff;
	m_writeGroup.currentGbGr_H = (wbCurrent->gb_gr>>8)&0xff;

	int wbSum = 0;
	for (int i=1;i<7;i++)
	{
		wbSum+= m_writeGroup.dataWB[i];
	}
	m_writeGroup.checkSumWB = wbSum%255 +1;

	return TRUE;
}

BOOL COtpS5K3L8_KB_PD::LoadWBSettings(tagAwbRatio* wbCurrent,tagAwbRatio* wbGolden)
{
	
	wbCurrent->rg = m_readGroup.currentRG_H* 256 + m_readGroup.currentRG_L;
	wbCurrent->bg = m_readGroup.currentBG_H* 256 + m_readGroup.currentBG_L;
	wbCurrent->gb_gr = m_readGroup.currentGbGr_H* 256 + m_readGroup.currentGbGr_L;

	return TRUE;
}

void COtpS5K3L8_KB_PD::ShowUpdateInfoAWB()
{
	tagAwbRatio *wbCurrent = GetWBCurrentValue();	

	CString info;
	info.Format(_T("Current R/G: %d"), wbCurrent->rg);
	m_pInterface->AddLog(info);

	info.Format(_T("Current B/G: %d"), wbCurrent->bg);
	m_pInterface->AddLog(info);

	info.Format(_T("Current GbGr: %d"), wbCurrent->gb_gr);
	m_pInterface->AddLog(info);

}

BOOL COtpS5K3L8_KB_PD::CreateLencGroup(const BYTE *lencData)
{
	m_writeGroup.flagLenc = S5K3L8_KB_PD_FLAG_VALID;
	memcpy(m_writeGroup.lenc,lencData,S5K3L8_KB_PD_MTK_OTP_LSC_SIZE);
	int sum =0;

	for (int i=0;i<S5K3L8_KB_PD_MTK_OTP_LSC_SIZE;i++)
	{
		sum+= m_writeGroup.lenc[i];
	}

	m_writeGroup.checkSumLenc = sum %255 +1;

	return TRUE;
}

BOOL COtpS5K3L8_KB_PD::OtpWrite()
{
	m_pInterface->AddLog(_T("Start OTP Program..."));
	int ret = 0;
	for (int i=0; i<32; i++)
	{
		ret = eeprom_multi_write(0x4000+i*0x40,m_writeGroup.data+64*i,64);
		Sleep(10);
		if (!ret)
		{
			m_pInterface->AddLog(_T("Write Eeprom Error"),RGB(255,0,0));
			return 0;
		}
	}
	if (OtpVerify())
	{
		return TRUE;
	}
	else
	{
		eeprom_write(0x8000,0xff);
		eeprom_write(0x4000,S5K3L8_KB_PD_FLAG_INVALID);
		eeprom_write(0x8040,0xff);
		eeprom_write(0x4040,S5K3L8_KB_PD_FLAG_INVALID);
		eeprom_write(0x8080,0xff);
		eeprom_write(0x4080,S5K3L8_KB_PD_FLAG_INVALID);
		Sleep(10);
		return FALSE;
	}
}

BOOL COtpS5K3L8_KB_PD::OtpVerify()
{
	OtpRead();

	BYTE *write = m_writeGroup.data;
	BYTE *read  = m_readGroup.data;
	BOOL bRet = TRUE;

	for (int i=0; i<S5K3L8_KB_PD_MTK_OTP_GROUP_SIZE; i++)
	{
		if (*read != *write)
		{
			CString info;
			info.Format(_T("addr=%d, %02X!=%02X"), i, *read, *write);
			m_pInterface->AddLog(info, COLOR_RED);
			bRet = FALSE;
		}
		write++;
		read++;
	}
	if (bRet)
	{
		m_pInterface->AddLog(_T("OTP Verify Finished"));
		return TRUE;
	}
	m_pInterface->AddLog(_T("OTP Verify Error"),COLOR_RED);
	return FALSE;
	
}

BOOL COtpS5K3L8_KB_PD::OtpRead()
{
	ResetReadGroup();
	BOOL bRet = FALSE;
	for (int i=0; i<32; i++)
	{
		bRet = eeprom_multi_read(0x40*i, m_readGroup.data+64*i, 64);
		Sleep(10);
		if(!bRet)
		{
			m_pInterface->AddLog(_T("Read EEPROM Err!"), COLOR_RED);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL COtpS5K3L8_KB_PD::CheckOtpEmpty()
{
	BOOL bRet = FALSE;
	BYTE eraseData = 0xff;
	for (int i=0; i<32; i++)
	{
 		bRet = eeprom_write(0x8000+i*0x40, eraseData);
		Sleep(10);
		if (!bRet)
		{
			m_pInterface->AddLog(_T("Erase Eeprom Error"), COLOR_RED);
			return FALSE;
		}
	}

	OtpRead();
	tagOtpCtrl *control = GetControlParams();	
	if (control->enCheckAF)
	{
		BYTE afFlag = 0;
		eeprom_read(0x800, &afFlag);
		Sleep(10);
		if (afFlag != S5K3L8_KB_PD_FLAG_VALID)
		{
			m_szStr.Format(_T("AF Flag Invalid: 0x%02x"), afFlag);
			m_pInterface->AddLog(m_szStr, COLOR_RED);
			return FALSE;
		}
		if (!isCheckAFSumRight())
		{
			m_pInterface->AddLog(_T("AF CheckSum Invalid"), COLOR_RED);
			return FALSE;
		}
		m_pInterface->AddLog(_T("AF Check Valid"), COLOR_GREEN);

		BYTE pdFlag = 0;
		eeprom_read(0x840, &pdFlag);
		Sleep(10);
		if (pdFlag != S5K3L8_KB_PD_FLAG_VALID)
		{
			m_szStr.Format(_T("PD Flag Invalid: 0x%02x"), pdFlag);
			m_pInterface->AddLog(m_szStr, COLOR_RED);
			return FALSE;
		}
		m_pInterface->AddLog(_T("PDAF Flag Check Valid"), COLOR_GREEN);
	}

	if(m_readGroup.flag == S5K3L8_KB_PD_FLAG_EMPTY)
	{
		m_pInterface->AddLog(_T("OTP MTK is Empty"), COLOR_BLUE);
		return TRUE;
	}
	else if((m_readGroup.flag	 ==	S5K3L8_KB_PD_FLAG_VALID)&&
			(m_readGroup.flagWB	 ==	S5K3L8_KB_PD_FLAG_VALID)&&
			(m_readGroup.flagLenc==	S5K3L8_KB_PD_FLAG_VALID))
	{
		m_pInterface->AddLog(_T("OTP MTK is Valid"), COLOR_GREEN);
		return FALSE;
	}
	else
		m_pInterface->AddLog(_T("OTP MTK is Invalid"), COLOR_RED);

	return FALSE;
}

BOOL COtpS5K3L8_KB_PD::CheckOtpValid()
{
	OtpRead();
	tagOtpCtrl* control = GetControlParams();
	CString info;
	if (control->enCheckAF)
	{
		BYTE afFlag = 0;
		eeprom_read(0x800,&afFlag);
		Sleep(10);
		if (afFlag!=S5K3L8_KB_PD_FLAG_VALID)
		{
			info.Format(_T("AF Flag Invalid:0x%02x"),afFlag);
			m_pInterface->AddLog(info,COLOR_RED);
			return FALSE;
		}
		if (isCheckAFSumRight())
		{
			m_pInterface->AddLog(_T("AF CheckSum Valid"));
		}
		else
		{
			m_pInterface->AddLog(_T("AF CheckSum Invalid"),COLOR_RED);
			return FALSE;
		}

		BYTE pdFlag = 0;
		eeprom_read(0x840,&pdFlag);
		Sleep(10);
		if (pdFlag!=S5K3L8_KB_PD_FLAG_VALID)
		{
			CString info;
			info.Format(_T("PD Flag Invalid:0x%02x"),pdFlag);
			m_pInterface->AddLog(info,COLOR_RED);
			return FALSE;
		}
// 		if (!isCheckPDAFSumRight())
// 		{
// 			m_pInterface->AddLog(_T("PD CheckSum Invalid"),COLOR_RED);
// 			return FALSE;
// 		}
	}
	if ((m_readGroup.flag==S5K3L8_KB_PD_FLAG_VALID)&&
		(m_readGroup.flagWB==S5K3L8_KB_PD_FLAG_VALID)&&
		(m_readGroup.flagLenc==S5K3L8_KB_PD_FLAG_VALID))
	{
		if (isCheckSumRight())
		{
			m_pInterface->AddLog(_T("OTP MTK CheckSum Valid"));
			return TRUE;
		}
		else
		{
			m_pInterface->AddLog(_T("OTP MTK CheckSum Invalid"),COLOR_RED);
			return FALSE;
		}
	}
	m_pInterface->AddLog(_T("OTP MTK FLAG Invalid"),COLOR_RED);
	return FALSE;
}

BOOL COtpS5K3L8_KB_PD::isCheckAFSumRight()
{
	BYTE afData[6] = {0};
	for (int i=0;i<6;i++)
	{
		eeprom_multi_read(0x800,afData,6);
		Sleep(10);
	}
	BYTE afCheckSum = CalcCheckSum(afData+1,4,255) +1;
	if (afCheckSum==afData[5])
	{
		return TRUE;
	}
	return FALSE;
}

BOOL COtpS5K3L8_KB_PD::isCheckPDAFSumRight()
{
	BYTE pdData[1408] = {0};
	for ( int i=33;i<55;i++)
	{
		eeprom_multi_read(0x40*i,pdData+64*(i-33),64);
	}

	int pdCheckSum1 = CalcCheckSum(&pdData[1],496,255) +1;
	int pdCheckSum2 = CalcCheckSum(&pdData[497],908,255) +1;

	if ((pdData[1405]!=pdCheckSum1)||
		(pdData[1406]!=pdCheckSum2))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL COtpS5K3L8_KB_PD::isCheckSumRight()
{
	BYTE basicCheckSum = CalcCheckSum(&m_readGroup.dataBasic[1],20,255) +1;
	BYTE wbCheckSum = CalcCheckSum(&m_readGroup.dataWB[1],6,255)+1;
	BYTE lencCheckSum = CalcCheckSum(m_readGroup.lenc,S5K3L8_KB_PD_MTK_OTP_LSC_SIZE,255) +1;
	if ((basicCheckSum!=m_readGroup.checkSumBasic)||
		(wbCheckSum!=m_readGroup.checkSumWB)||
		(lencCheckSum!=m_readGroup.checkSumLenc))
	{
		return FALSE;
	}

	return TRUE;
}

void COtpS5K3L8_KB_PD::ResetOtpGroup()
{
	ResetWriteGroup();
	ResetReadGroup();
}

void COtpS5K3L8_KB_PD::ResetWriteGroup()
{
	memset(&m_writeGroup, 0, sizeof(m_writeGroup));	
}

void COtpS5K3L8_KB_PD::ResetReadGroup()
{
	memset(&m_readGroup, 0, sizeof(m_readGroup));	
	
}

BOOL COtpS5K3L8_KB_PD::GetLSC(BYTE *lencData, int inputTarget)
{
	ASSERT(lencData!=NULL);
	if (GetLencRegister()->dataSize!=S5K3L8_KB_PD_MTK_OTP_LSC_SIZE)
	{
		return FALSE;
	}
	m_pInterface->AddLog(_T("Waiting For Lsc Calc..."));

	ImageInfo imgInfo = m_pInterface->GetImage();
	BYTE* pDtRaw10 = new BYTE[imgInfo.width*imgInfo.height * 2 + 1024];
	ImageProc::RawToDtRaw10(imgInfo.Raw_Buf, pDtRaw10, imgInfo.width, imgInfo.height);
	BOOL ret = MtkEeprom->create_lsc_group_Mtk(pDtRaw10, lencData, imgInfo.width, imgInfo.height, S5K3L8_KB_PD_MTK_OTP_LSC_SIZE);
	if (!ret)
	{
		m_pInterface->AddLog(_T("Create MTK LSC Err!"), COLOR_RED);
		SAFE_DELETE_ARRAY(pDtRaw10);
		return FALSE;
	} 
	m_pInterface->AddLog(_T("MTK LSC Calibration Success"));
	SAFE_DELETE_ARRAY(pDtRaw10);
	return TRUE;
}

BOOL COtpS5K3L8_KB_PD::OTPReadAll()
{
	CString otpDataName = _T("S5K3L8_Eeprom.txt");
	otpDataName = m_pInterface->GetExeDir() + otpDataName;
	CStdioFile MyFile(otpDataName, CFile::modeCreate|CFile::modeWrite|CFile::typeText); 
	MyFile.SeekToEnd();
	CString info;
	BYTE readData[10240] = {0};
	MyFile.WriteString(_T("OTP Data Start...\n"));
	int ret = 0;
	for (int i=0;i<160;i++)
	{
		ret = 0;
		ret = eeprom_multi_read(0x0000+0x40*i,readData+64*i,64);
		Sleep(10);
		if(!ret)
		{
			m_pInterface->AddLog(_T("Read EEPROM Err!"), RGB(255,0,0));
			return 0;
		}
	}
	for (int i=0;i<160;i++)
	{
		info.Format(_T("Page:%d,\n"),i);
		MyFile.WriteString(info);
		for (int j=0;j<64;j++)
		{
			info.Format(_T("0x%02x,0x%02x\n"),j,readData[i*64+j]);
			MyFile.WriteString(info);
		}
	}
	MyFile.WriteString(_T("OTP Data END...\n"));
	return TRUE;
}

BOOL COtpS5K3L8_KB_PD::LoadAFSettings()
{
// 	BYTE dataAF[6] = {0};
// 	for (int i=0;i<6;i++)
// 	{
// 		eeprom_multi_read(0x800,dataAF,6);
// 		Sleep(10);
// 	}
// 	int infCode = dataAF[2]* 256 + dataAF[1];
// 	int macCode = dataAF[4]* 256 + dataAF[3];
// 	SetAFInfinityCode(infCode);
// 	SetAFMacroCode(macCode);

	return TRUE;
}

void COtpS5K3L8_KB_PD::ShowAFUpdateInfo()
{
// 	CString info;
// 	info.Format(_T("InfCode:%d"),GetAFInfinityCode());
// 	m_pInterface->AddLog(info);
// 
// 	info.Format(_T("MacCode:%d"),GetAFMacroCode());
// 	m_pInterface->AddLog(info);
}

BOOL COtpS5K3L8_KB_PD::IsContinueOTP()
{
	return TRUE;
}
