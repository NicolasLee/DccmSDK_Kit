#include "stdafx.h"
#include "OtpS5K3L8_KB.h"
#include "..//OTP_SensorSetting.h"


COtpS5K3L8_KB::COtpS5K3L8_KB(void)
{
	memset(&m_writeGroup, 0, sizeof(m_writeGroup));	
	memset(&m_readGroup, 0, sizeof(m_readGroup));	

	SetOtpName(_T("S5K3L8_PD"));

	PreSubclassOtp();
}


COtpS5K3L8_KB::~COtpS5K3L8_KB(void)
{

}

void COtpS5K3L8_KB::PreSubclassOtp()
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
	control->enCheckAF = FALSE;

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
	wbReg->addrGrHigh = S5K3L8_KB_ADDR_GAIN_GR_H;
	wbReg->addrGrLow  = S5K3L8_KB_ADDR_GAIN_GR_L;
	wbReg->addrGbHigh = S5K3L8_KB_ADDR_GAIN_GB_H;
	wbReg->addrGbLow  = S5K3L8_KB_ADDR_GAIN_GB_L;
	wbReg->addrRHigh  = S5K3L8_KB_ADDR_GAIN_R_H;
	wbReg->addrRLow   = S5K3L8_KB_ADDR_GAIN_R_L;
	wbReg->addrBHigh  = S5K3L8_KB_ADDR_GAIN_B_H;
	wbReg->addrBLow   = S5K3L8_KB_ADDR_GAIN_B_L;
	wbReg->dataDefaultGain = S5K3L8_KB_DATA_DEFAULT_GAIN;


	tagAwbRatio *wbGolden = GetWBGoldenValue();		
	wbGolden->r   = 80;
	wbGolden->gr  = 159;
	wbGolden->gb  = 160;
	wbGolden->b   = 86;
	wbGolden->rg  = 512;
	wbGolden->bg  = 550;


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
	lencReg->dataSize = S5K3L8_KB_MTK_OTP_LSC_SIZE;

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
	expReg->addrMid = S5K3L8_KB_ADDR_EXP_H;
	expReg->addrLow = S5K3L8_KB_ADDR_EXP_L;

	ExpRangeStruct *expRange = GetExpRangeParams(); 
	expRange->def = 0x00f6;
	expRange->max = 0xFF00;
	expRange->min = 0;


	EepromStruct eeprom;
	eeprom.deviceID = 0x28;
	eeprom.addrByteSize = 2;
	eeprom.pageSize = 16;
	eeprom.msWriteTime = 10;

	SetEepromParams(&eeprom);
}

void COtpS5K3L8_KB::SetOtpSensorParams()
{
	OtpSensorStruct* params = GetOtpSensorParams();

	params->name = _T("S5K3L8_KB_MIPI");
	params->RESET = PIN_HIGH;
	params->PWDN = PIN_HIGH;
	params->mvAVDD = 2800;
	params->mvDOVDD = 1800;
	params->mvCORE  = 1200;
	params->mvAFVCC = 2800;
	params->mvVPP   = 0;

	params->i2c.addr  = 0x20;//0x20 CMA191 CMA332 CM8447 CM8442; 0x5A CMA286
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

BOOL COtpS5K3L8_KB::CreateBasicGroup(const OtpIDStruct* ID)
{
	CTime time = CTime::GetCurrentTime();
	m_writeGroup.flag = S5K3L8_KB_FLAG_VALID; 
	m_writeGroup.vendor = ID->vendor;
	m_writeGroup.year = time.GetYear() - 2000;
	m_writeGroup.month = time.GetMonth();
	m_writeGroup.date  = time.GetDay();
	m_writeGroup.lens  = ID->lens;
	m_writeGroup.vcm   = ID->vcm;
	m_writeGroup.driverIC  = ID->driverIC;

	int sum = 0;
	for (int i=1;i<8;i++)
	{
		sum+= m_writeGroup.dataBasic[i];
	}
	
	m_writeGroup.checkSumBasic = sum %256;

	return TRUE;

}

BOOL COtpS5K3L8_KB::LoadBasicSettings(OtpIDStruct *IDCurrent)
{
	IDCurrent->vendor   = m_readGroup.vendor;
	IDCurrent->lens     = m_readGroup.lens;
	IDCurrent->vcm      = m_readGroup.vcm;
	IDCurrent->driverIC = m_readGroup.driverIC;

	return TRUE;
}

void COtpS5K3L8_KB::ShowUpdateInfoBasic()
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

BOOL COtpS5K3L8_KB::CreateWBGroup(const tagAwbRatio* wbCurrent,const tagAwbRatio* wbGolden)
{
	m_writeGroup.flagWB = S5K3L8_KB_FLAG_VALID;
	m_writeGroup.currentR = int(1.0 * wbCurrent->r / 4 + 0.5);
	m_writeGroup.currentGr= int(1.0 * wbCurrent->gr/ 4 + 0.5);
	m_writeGroup.currentGb= int(1.0 * wbCurrent->gb/ 4 + 0.5);
	m_writeGroup.currentB = int(1.0 * wbCurrent->b / 4 + 0.5);

	m_writeGroup.goldenR  = wbGolden->r;
	m_writeGroup.goldenGr = wbGolden->gr;
	m_writeGroup.goldenGb = wbGolden->gb;
	m_writeGroup.goldenB  = wbGolden->b;

	int wbSum = 0;
	for (int i=1;i<9;i++)
	{
		wbSum+= m_writeGroup.dataWB[i];
	}
	m_writeGroup.checkSumWB = wbSum%256;

	return TRUE;
}

BOOL COtpS5K3L8_KB::LoadWBSettings(tagAwbRatio* wbCurrent,tagAwbRatio* wbGolden)
{
	wbCurrent->r = m_readGroup.currentR;
	wbCurrent->gr= m_readGroup.currentGr;
	wbCurrent->gb= m_readGroup.currentGb;
	wbCurrent->b = m_readGroup.currentB;
	int currentG = int(1.0*(wbCurrent->gr + wbCurrent->gb) / 2 +0.5);
	tagAwbOtpCtrl *wbCtrl = GetWBControlParams();
	wbCurrent->rg = int(1.0* wbCtrl->factorAmplify * wbCurrent->r / currentG + 0.5);
	wbCurrent->bg = int(1.0* wbCtrl->factorAmplify * wbCurrent->b / currentG + 0.5);

	wbGolden->r  = m_readGroup.goldenR;
	wbGolden->gr = m_readGroup.goldenGr;
	wbGolden->gb = m_readGroup.goldenGb;
	wbGolden->b  = m_readGroup.goldenB;


	return TRUE;
}

void COtpS5K3L8_KB::ShowUpdateInfoAWB()
{
	tagAwbRatio *wbCurrent = GetWBCurrentValue();	

	CString info;
	info.Format(_T("CurrentR: %d"),wbCurrent->r);
	m_pInterface->AddLog(info);
	info.Format(_T("CurrentGr: %d"),wbCurrent->gr);
	m_pInterface->AddLog(info);
	info.Format(_T("CurrentGb: %d"),wbCurrent->gb);
	m_pInterface->AddLog(info);
	info.Format(_T("CurrentB: %d"),wbCurrent->b);
	m_pInterface->AddLog(info);

	info.Format(_T("GoldenR: %d"),m_readGroup.goldenR);
	m_pInterface->AddLog(info);
	info.Format(_T("GoldenGr: %d"),m_readGroup.goldenGr);
	m_pInterface->AddLog(info);
	info.Format(_T("GoldenGb: %d"),m_readGroup.goldenGb);
	m_pInterface->AddLog(info);
	info.Format(_T("GoldenB: %d"),m_readGroup.goldenB);
	m_pInterface->AddLog(info);

	info.Format(_T("Current R/G: %d"), wbCurrent->rg);
	m_pInterface->AddLog(info);

	info.Format(_T("Current B/G: %d"), wbCurrent->bg);
	m_pInterface->AddLog(info);
}

BOOL COtpS5K3L8_KB::CreateLencGroup(const BYTE *lencData)
{
	m_writeGroup.flagLenc = S5K3L8_KB_FLAG_VALID;
	memcpy(m_writeGroup.lenc,lencData,S5K3L8_KB_MTK_OTP_LSC_SIZE);
	int sum =0;

	for (int i=0;i<S5K3L8_KB_MTK_OTP_LSC_SIZE;i++)
	{
		sum+= m_writeGroup.lenc[i];
	}

	m_writeGroup.checkSumLenc = sum %256;

	return TRUE;
}

BOOL COtpS5K3L8_KB::OtpWrite()
{
	m_pInterface->AddLog(_T("Start OTP Program..."));
	int ret = 0;
	for (int i=0;i<S5K3L8_KB_MTK_OTP_GROUP_SIZE/16;i++)
	{
		ret = eeprom_multi_write(0x0000+16*i,m_writeGroup.data+16*i,16);
		Sleep(10);
		if (!ret)
		{
			m_pInterface->AddLog(_T("Write Eeprom Error"),RGB(255,0,0));
			return 0;
		}
	}

	ret = eeprom_multi_write(0x760, m_writeGroup.data+0x760, 4);
	Sleep(10);
	if (!ret)
	{
		m_pInterface->AddLog(_T("Write Eeprom Error"),RGB(255,0,0));
		return 0;
	}
	if (OtpVerify())
	{
		return TRUE;
	}
	else
	{
		eeprom_write(0x0000,0x00);
		Sleep(10);
		return FALSE;
	}
}

BOOL COtpS5K3L8_KB::OtpVerify()
{
	OtpRead();

	BYTE *write = m_writeGroup.data;
	BYTE *read  = m_readGroup.data;
	BOOL bRet = TRUE;

	for (int i=0; i<S5K3L8_KB_MTK_OTP_GROUP_SIZE; i++)
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

BOOL COtpS5K3L8_KB::OtpRead()
{
	ResetReadGroup();
	int ret=0;
	for (int i=0;i<S5K3L8_KB_MTK_OTP_GROUP_SIZE/16;i++)
	{
		ret = eeprom_multi_read(0x28,0x0000+16*i,m_readGroup.data+16*i,16);
		Sleep(10);
		if(!ret)
		{
			m_pInterface->AddLog(_T("Read EEPROM Err!"), RGB(255,0,0));
			return 0;
		}
	}
	ret = eeprom_multi_read(0x28,0x0760,m_readGroup.data+0x0760,4);
	Sleep(10);
	if(!ret)
	{
		m_pInterface->AddLog(_T("Read EEPROM Err!"), RGB(255,0,0));
		return 0;
	}

	return TRUE;
}

BOOL COtpS5K3L8_KB::CheckOtpEmpty()
{
	OtpRead();
	tagOtpCtrl *control = GetControlParams();	
	if (!control->enWB || !control->enLenc)	
	{
		return FALSE;
	}

	if (control->enCheckAF)
	{
		BYTE afFlag= 0;
		eeprom_read(0x0764,&afFlag);
		Sleep(10);
		if (afFlag!= S5K3L8_KB_FLAG_VALID)
		{
			CString info;
			info.Format(_T("AF Flag Invalid:0x%02x"),afFlag);
			m_pInterface->AddLog(info,COLOR_RED);
			return FALSE;
		}
		if (!isCheckAFSumRight())
		{
			m_pInterface->AddLog(_T("AF CheckSum Invalid"),COLOR_RED);
			return FALSE;
		}
	}

	if(m_readGroup.flag==0xff)
	{
		m_pInterface->AddLog(_T("OTP MTK is Empty"));
		return TRUE;
	}
	if ((m_readGroup.flag==S5K3L8_KB_FLAG_VALID)&&
		(m_readGroup.flagWB==S5K3L8_KB_FLAG_VALID)&&
		(m_readGroup.flagLenc==S5K3L8_KB_FLAG_VALID))
	{
		m_pInterface->AddLog(_T("OTP MTK is Valid"));
		return FALSE;
	}
	m_pInterface->AddLog(_T("OTP MTK is Invalid"),COLOR_RED);
	return FALSE;
}

BOOL COtpS5K3L8_KB::CheckOtpValid()
{
	OtpRead();
	tagOtpCtrl* control = GetControlParams();
	CString info;
	if (control->enCheckAF)
	{
		BYTE afFlag = 0;
		eeprom_read(0x0764,&afFlag);
		Sleep(10);
		if (afFlag!=S5K3L8_KB_FLAG_VALID)
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
	}
	if ((m_readGroup.flag==S5K3L8_KB_FLAG_VALID)&&
		(m_readGroup.flagWB==S5K3L8_KB_FLAG_VALID)&&
		(m_readGroup.flagLenc==S5K3L8_KB_FLAG_VALID))
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

BOOL COtpS5K3L8_KB::isCheckAFSumRight()
{
	BYTE afData[6] = {0};
	for (int i=0;i<6;i++)
	{
		eeprom_read(S5K3L8_KB_EEPROM_ADDR_AF_FLAG+i,&afData[i]);
		Sleep(10);
	}
	BYTE afCheckSum = CalcCheckSum(afData+1,4,256);
	if (afCheckSum==afData[5])
	{
		return TRUE;
	}
	return FALSE;
}

BOOL COtpS5K3L8_KB::isCheckSumRight()
{
	BYTE basicCheckSum = CalcCheckSum(&m_readGroup.dataBasic[1],7,256);
	BYTE wbCheckSum = CalcCheckSum(&m_readGroup.dataWB[1],8,256);
	BYTE lencCheckSum = CalcCheckSum(m_readGroup.lenc,S5K3L8_KB_MTK_OTP_LSC_SIZE,256);
	if ((basicCheckSum!=m_readGroup.checkSumBasic)||
		(wbCheckSum!=m_readGroup.checkSumWB)||
		(lencCheckSum!=m_readGroup.checkSumLenc))
	{
		return FALSE;
	}

	return TRUE;
}

void COtpS5K3L8_KB::ResetOtpGroup()
{
	ResetWriteGroup();
	ResetReadGroup();
}

void COtpS5K3L8_KB::ResetWriteGroup()
{
	memset(&m_writeGroup, 0, sizeof(m_writeGroup));	
}

void COtpS5K3L8_KB::ResetReadGroup()
{
	memset(&m_readGroup, 0, sizeof(m_readGroup));	
	
}

BOOL COtpS5K3L8_KB::GetLSC(BYTE *lencData, int inputTarget)
{
	ASSERT(lencData!=NULL);
	if (GetLencRegister()->dataSize!=S5K3L8_KB_MTK_OTP_LSC_SIZE)
	{
		return FALSE;
	}
	m_pInterface->AddLog(_T("Waiting For Lsc Calc..."));
// 	BYTE *imgData =GetFrame(2);
// 	if (imgData == NULL)
// 	{
// 
// 		return FALSE;
// 	}
// 
// 	BOOL ret;
// 	ret=MtkEeprom->create_lsc_group_Mtk(imgData,lencData,GetVideoWidth(),GetVideoHeight(),S5K3L8_KB_MTK_OTP_LSC_SIZE);
// 	if (!ret)
// 	{
// 		m_pInterface->AddLog(_T("Create MTK LSC Err!", COLOR_RED);
// 		return FALSE;
// 	} 
// 	m_pInterface->AddLog(_T("MTK LSC Calibration Success"));
	return TRUE;
}

BOOL COtpS5K3L8_KB::OTPReadAll()
{
	CString otpDataName = _T("S5K3L8_Eeprom.txt");
	otpDataName = m_pInterface->GetExeDir() + otpDataName;
	CStdioFile MyFile(otpDataName, CFile::modeCreate|CFile::modeWrite|CFile::typeText); 
	MyFile.SeekToEnd();
	CString info;
	BYTE readData[2048] = {0};
	MyFile.WriteString(_T("OTP Data Start...\n"));
	int ret = 0;
	for (int i=0;i<2048/16;i++)
	{
		ret = 0;
		ret = eeprom_multi_read(0xa0,0x0000+16*i,readData+16*i,16);
		Sleep(10);
		if(!ret)
		{
			m_pInterface->AddLog(_T("Write EEPROM Err!"), RGB(255,0,0));
			return 0;
		}
	}
	for (int i=0;i<2048;i++)
	{
		info.Format(_T("0x%04x,0x%02x,\n"),0x0000+i,readData[i]);
		MyFile.WriteString(info);
	}
	MyFile.WriteString(_T("OTP Data END...\n"));
	return TRUE;
}

BOOL COtpS5K3L8_KB::LoadAFSettings()
{
// 	BYTE dataAF[6] = {0};
// 	for (int i=0;i<6;i++)
// 	{
// 		eeprom_read(S5K3L8_KB_EEPROM_ADDR_AF_FLAG+i,dataAF+i);
// 		Sleep(10);
// 	}
// 	int macCode = dataAF[1]* 256 + dataAF[2];
// 	int infCode = dataAF[3]* 256 + dataAF[4];
// 	SetAFInfinityCode(infCode);
// 	SetAFMacroCode(macCode);

	return TRUE;
}

void COtpS5K3L8_KB::ShowAFUpdateInfo()
{
// 	CString info;
// 	info.Format(_T("InfCode:%d"),GetAFInfinityCode());
// 	m_pInterface->AddLog(info);
// 
// 	info.Format(_T("MacCode:%d"),GetAFMacroCode());
// 	m_pInterface->AddLog(info);
}

BOOL COtpS5K3L8_KB::IsContinueOTP()
{
	return TRUE;
}
