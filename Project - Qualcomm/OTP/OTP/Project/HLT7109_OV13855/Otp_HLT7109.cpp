#include "stdafx.h"
#include "Otp_HLT7109.h"
#include "..\..\OTP_SensorSetting.h"

COtpHLT7109::COtpHLT7109(void)
{
	ZeroMemory(OTP_Data_Write.AFInfo,sizeof(OTP_Data_Write.AFInfo));
	ZeroMemory(OTP_Data_Write.BaseInfo,sizeof(OTP_Data_Write.BaseInfo));
	ZeroMemory(HLT7109_LSCWriteData,sizeof(HLT7109_LSCWriteData));
	ZeroMemory(OTP_Data_Write.AWBInfo,sizeof(OTP_Data_Write.AWBInfo));
	ZeroMemory(OTP_Data_Write.PDAF1,sizeof(OTP_Data_Write.PDAF1));

	m_flagEmpty = FLAG_EMPTY;
	m_flagValid = FLAG_VALID;

	m_addrFlag.moduleInfo	= REG_BASIC_FLAG;
	m_addrFlag.awb				= REG_WB_FLAG;
	m_addrFlag.lsc					= REG_LSC_FLAG;
	m_addrFlag.af					= REG_AF_FLAG;
	m_addrFlag.pdaf			    = REG_PDAF_FLAG;
	m_addrFlag.pdaf2				= REG_PDAF_PROC2_FIRSTADDR-1; // Proc2 First Data的上一个地址

	m_addrSum.moduleInfo	= REG_BASIC_CHECKSUM;
	m_addrSum.awb				= REG_WB_CHECKSUM;
	m_addrSum.lsc					= REG_LSC_CHECKSUM;
	m_addrSum.af					= REG_AF_CHECKSUM;
	m_addrSum.pdaf				= REG_PDAF_PROC1_SUMADDR;

// 	m_otpSize.moduleInfo	= m_addrSum.moduleInfo - m_addrFlag.moduleInfo - 1;
// 	m_otpSize.awb			= m_addrSum.awb - m_addrFlag.awb - 1;
// 	m_otpSize.lsc			= m_addrSum.lsc - m_addrFlag.lsc - 1;
// 	m_otpSize.af			= m_addrSum.af	- m_addrFlag.af - 1;
// 
// 	m_addrCode.stcL			= 0;
// 	m_addrCode.stcH			= 0;
// 	m_addrCode.infL			= 0x0801;
// 	m_addrCode.infH			= 0x0802;
// 	m_addrCode.macL			= 0x0803;
// 	m_addrCode.macH			= 0x0804;

//	SetOtpName(_T("TYD8470M"));

	PreSubclassOtp();
}


COtpHLT7109::~COtpHLT7109(void)
{

}

void COtpHLT7109::PreSubclassOtp()
{
	tagOtpCtrl* control = GetControlParams();
	control->sensorob = SENSOROB;
	control->quallsccompress = QUALLSCCOMPRESS;

	OtpIDStruct *ID = GetIDGroup();	
	ID->mtkver1 = BASIC_MTKVER1;
	ID->mtkver2 = BASIC_MTKVER2;
	ID->mtkver3 = BASIC_MTKVER3;
	ID->mtkver4 = BASIC_MTKVER4;
	ID->mid = BASIC_MID;
	ID->apid = BASIC_APID;
	ID->lens = BASIC_LENSID;
	ID->vcm = BASIC_VCMID;
	ID->driverIC = BASIC_DRIVERICID;
	ID->eeprom = BASIC_EEPROMID;
	ID->colortemp = BASIC_COLORTEMP;
	ID->biten = BASIC_BITEN;
	ID->blackmask = BASIC_BLACKMASK;

	tagAwbOtpCtrl *wbCtrl = GetWBControlParams();
	wbCtrl->enLencBasedWB = TRUE;
	wbCtrl->enSaveRawLencBMP = FALSE;
	wbCtrl->enCheckWB = TRUE;
	wbCtrl->percentDifRG = 5.0;
	wbCtrl->percentDifBG = 5.0;
	wbCtrl->percentDifRG_Pre = 15.0;
	wbCtrl->percentDifBG_Pre = 15.0;
	wbCtrl->enUpdateWB = TRUE;
	wbCtrl->enSaveWBFile = FALSE;
	wbCtrl->wbFileName = _T("WB.csv");
	wbCtrl->roiCalcMode = ROI_CALC_10;
	wbCtrl->factorAmplify = AWB_FACTOR;//1024;

	WBGainRegStruct* wbReg = GetWBGainRegister();
	wbReg->addrGr = GAIN_ADDR_GR;
	wbReg->addrR = GAIN_ADDR_R;
	wbReg->addrB = GAIN_ADDR_B;
	wbReg->addrGb = GAIN_ADDR_GB;
	wbReg->dataDefaultGain = GAIN_DEFAULT_VAL;	
// 	wbReg->addrGrHigh = HLT7109_ADDR_GAIN_GR_H;
// 	wbReg->addrGrLow  = HLT7109_ADDR_GAIN_GR_L;
// 	wbReg->addrGbHigh = HLT7109_ADDR_GAIN_GB_H;
// 	wbReg->addrGbLow  = HLT7109_ADDR_GAIN_GB_L;
// 	wbReg->addrRHigh  = HLT7109_ADDR_GAIN_R_H;
// 	wbReg->addrRLow   = HLT7109_ADDR_GAIN_R_L;
// 	wbReg->addrBHigh  = HLT7109_ADDR_GAIN_B_H;
// 	wbReg->addrBLow   = HLT7109_ADDR_GAIN_B_L;


	tagAwbRatio *wbGolden = GetWBGoldenValue();		
	wbGolden->rg  = m_goldenrg;
	wbGolden->bg  = m_goldenbg;

	LencOtpControlStruct *lencCtrl = GetLencControlParams();
// 	lencCtrl->inputTarget = HLT7109_VAL_LSC_TARGET;
// 	lencCtrl->inputLow = HLT7109_VAL_LSC_LOW;
// 	lencCtrl->inputHigh = HLT7109_VAL_LSC_HIGH;
	lencCtrl->enDynamicInput = FALSE;
	lencCtrl->enCheckLenc = FALSE;//TRUE;//
	lencCtrl->enShadingTestAfterOtp = FALSE;	
	lencCtrl->shadingTestType = SHADING_TYPE;//SHADING_TYPE_QUALCOMM;//
	lencCtrl->enUpdateLenc = FALSE;	
	lencCtrl->enSaveLencFile = FALSE;//TRUE;//
	lencCtrl->lencFileName = _T("Lenc.csv");
	lencCtrl->enSaveRawLencFile = FALSE;
	lencCtrl->lencRawFileName = _T("LencRaw.csv");

//	LencRegStruct *lencReg = GetLencRegister();
// 	lencReg->addrCtrl = HLT7109_ADDR_LSC_CTRL;
// 	lencReg->lencEnable = HLT7109_ADDR_LSC_EN;
// 	lencReg->lencDisable = HLT7109_ADDR_LSC_DIS;
//	lencReg->addrDataStart = REG_INVALID;
//	lencReg->dataSize = HLT7109_OTP_LSC_SIZE;

	ExpControlStruct *expCtrl = GetExpControlParams();
	expCtrl->type = EXP_LINEAR;
	expCtrl->roiCalcMode = ROI_CALC_10;
	expCtrl->filterChannel = EXP_CHANNEL_G;
// 	expCtrl->target = HLT7109_VAL_EXP_TARGET;
// 	expCtrl->lowLimit = HLT7109_VAL_EXP_LOW;
// 	expCtrl->highLimit = HLT7109_VAL_EXP_HIGH;
	expCtrl->effectFrame = 3;
	expCtrl->stableFrame = 3;

	ExpRegisterStruct *expReg = GetExpRegister();
	expReg->mask = 0x00FFFF;
	expReg->addrHigh = REG_INVALID;
// 	expReg->addrMid = HLT7109_ADDR_EXP_H;
// 	expReg->addrLow = HLT7109_ADDR_EXP_L;

	ExpRangeStruct *expRange = GetExpRangeParams(); 
	expRange->def = 0x00f6;
	expRange->max = 0xFF00;
	expRange->min = 0;

	MTKPDAFStruct*   mtkpdaf = GetMTKPDAFParams();
	mtkpdaf->Pdaf_FlagAddr = REG_PDAF_FLAG;
	mtkpdaf->Proc1_FirstAddr =  REG_PDAF_PROC1_FIRSTADDR;
	mtkpdaf->Proc1Size= SIZE_PDAF_PROC1;
	mtkpdaf->Proc2_FirstAddr =  REG_PDAF_PROC2_FIRSTADDR;
	mtkpdaf->Proc2Size = SIZE_PDAF_PROC2;
	mtkpdaf->TotalSize = SIZE_PDAF_PROC1 + SIZE_PDAF_PROC2;
	mtkpdaf->Proc1_SumAddr =  REG_PDAF_PROC1_SUMADDR;
	mtkpdaf->Proc2_SumAddr =  REG_PDAF_PROC2_SUMADDR;
	mtkpdaf->Pdaf_SumAddr =  REG_PDAF_SUMADDR;

	EepromStruct eeprom;
	eeprom.deviceID = 0xC0;
	eeprom.addrByteSize = 2;
	eeprom.pageSize = 64;
	eeprom.msWriteTime = 10;

	SetEepromParams(&eeprom);
}

void COtpHLT7109::SetOtpSensorParams()
{
	OtpSensorStruct* params = GetOtpSensorParams();

	params->name = _T("HLT7109_MIPI");
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
	params->otpInit.format = IMAGE_FORMAT_OTP_RAW10;
	params->otpInit.sequence = IMAGE_SEQUENCE_GBRG;
	params->otpInit.mhzMCLK = 24;
//	params->otpInit.params = HLT7109_OTP_Settings;
//	params->otpInit.paramSize = sizeofHLT7109_OTP_Settings;

	params->verifyInit.width  = 4208;
	params->verifyInit.height = 3120;
	params->verifyInit.format = IMAGE_FORMAT_OTP_RAW10;
	params->verifyInit.sequence = IMAGE_SEQUENCE_GBRG;
	params->verifyInit.mhzMCLK = 24;
// 	params->verifyInit.params = HLT7109_OTP_Settings;
// 	params->verifyInit.paramSize = sizeofHLT7109_OTP_Settings;	
}


BOOL COtpHLT7109::EraseAllData()
{
	GT24C64_All_Erase(SIZE_TOTAL_GROUP);
	m_pInterface->AddLog(_T("OTP所有数据已全部擦除!"));
	return TRUE;
}

void COtpHLT7109::SetWBDefaultGain(void)
{	
	return OV13855_SetDefaultAwb();
}

BOOL COtpHLT7109::StartProgram()
{
	return TRUE;
}

BOOL COtpHLT7109::WriteOTP()
{
	HLT7109_GetBasicData();
	HLT7109_GetAFData();
	CreateBurningData();
	if (!m_dummyburn)
	{
		if (!HLT7109_OtpWrite())//
		{return FALSE;}
	}
	else
	{
		SaveOTPData();
		m_pInterface->AddLog(_T("This is Dummy Burn!"),COLOR_RED);
	}
	return TRUE;
}

void COtpHLT7109::SaveOTPData()
{
	m_pInterface->AddLog(_T("Read OTP Start!"));
	///////////////////////////////Read OTP////////////////////////////////////
	int valSize = 3519;
	BYTE* pVal = new BYTE[valSize]();

	int i = 0;

	for (i=0; i<SIZE_BASIC_GROUP+1; i++)
	{
		pVal[i] = Data_Write.data_Basic[i];
	}
	for (i=0; i<SIZE_WB_GROUP+1; i++)
	{
		pVal[i+64] = Data_Write.data_WB[i];
	}
	for (i=0; i<SIZE_LSC_GROUP+1; i++)
	{
		pVal[i+128] = Data_Write.data_Lenc[i];
	}
	for (i=0; i<SIZE_AF_GROUP+1; i++)
	{
		pVal[i+2048] = Data_Write.data_AF[i];
	}
	for (i=0; i<SIZE_PDAF_GROUP+1; i++)
	{
		pVal[i+2112] = Data_Write.data_PDAF[i];
	}

	//////////////////////////////////////////////////////////////////////////
	CString DirPath = m_pInterface->GetExeDir();
	CString Path ;
	Path = DirPath + _T("Report\\");
	CreateDirectory(Path,NULL);
	info.Format(_T("OTP Dummy Data_%d.txt"),m_pInterface->GetModuleNum());
	Path += info;
	FILE* file = NULL;

	CStringA temp, num;
	num.Format("Apply WB Gain:\nR:%d\nGr:%d\nGb:%d\nB:%d\nRG:%d\nBG:%d\nGG:%d\n", Applyawbdata[0],Applyawbdata[1],Applyawbdata[2],Applyawbdata[3],Applyawbdata[4],Applyawbdata[5],Applyawbdata[6]);
	temp += num;

	for(i=0; i<valSize; i++)
	{
		num.Format("%04d\t0x%04x,0x%02x\n", i,0x0000+i,(unsigned char)pVal[i]);
		temp += num;
	}

	SAFE_DELETE_ARRAY(pVal);
	char* p = temp.GetBuffer();
	if (_wfopen_s(&file,Path.GetBuffer(),_T("wt+"))==0)
	{
		fwrite(p,1,strlen(p),file);
		fclose(file);
		Path = Path+_T(" saved");
		m_pInterface->AddLog(Path,COLOR_BLUE);
	}

	m_pInterface->AddLog(_T("Read OTP End!"));
	return;
}

BOOL COtpHLT7109::WriteOTP(vector<double>& vData)
{
	HLT7109_GetBasicData();
	HLT7109_GetAFData();
	CreateBurningData();
	if (!m_dummyburn)
	{
		if (!HLT7109_OtpWrite())//
		{return FALSE;}
	}
	else
	{
//		Programming_3L8LSC();
		SaveOTPData(vData);
		m_pInterface->AddLog(_T("This is Dummy Burn!"),COLOR_RED);
	}
	return TRUE;
}

void COtpHLT7109::SaveOTPData(vector<double>& vData)
{
	m_pInterface->AddLog(_T("Read OTP Start!"));
	///////////////////////////////Read OTP////////////////////////////////////
	int valSize = SIZE_TOTAL_GROUP;
	BYTE* pVal = new BYTE[valSize];

	memset(pVal, 0x00, valSize*sizeof(BYTE));

	int i = 0;

	for (i=0; i<SIZE_BASIC_GROUP+1; i++)
	{
		pVal[i] = Data_Write.data_Basic[i];
	}
	for (i=0; i<SIZE_WB_GROUP+1; i++)
	{
		pVal[i+64] = Data_Write.data_WB[i];
	}
	for (i=0; i<SIZE_LSC_GROUP+1; i++)
	{
		pVal[i+128] = Data_Write.data_Lenc[i];
	}
	for (i=0; i<SIZE_AF_GROUP+1; i++)
	{
		pVal[i+2048] = Data_Write.data_AF[i];
	}
	for (i=0; i<SIZE_PDAF_GROUP+1; i++)
	{
		pVal[i+2112] = Data_Write.data_PDAF[i];
	}

	//////////////////////////////////////////////////////////////////////////
	CString DirPath = m_pInterface->GetExeDir();
	CString Path ;
	Path.Format(_T("OTP Dummy Data_%d.txt"),m_pInterface->GetModuleNum());
	Path = DirPath+Path;
	FILE* file = NULL;

	CStringA temp, num;
	num.Format("Apply WB Gain:\nR:%d\nGr:%d\nGb:%d\nB:%d\nRG:%d\nBG:%d\nGG:%d\n", Applyawbdata[0],Applyawbdata[1],Applyawbdata[2],Applyawbdata[3],Applyawbdata[4],Applyawbdata[5],Applyawbdata[6]);
	temp += num;

	num.Format("Shading Data After SRAM:\nB1:%.1f\nB2:%.1f\nB3:%.1f\nB4:%.1f\n",vData.at(0), vData.at(1), vData.at(2), vData.at(3));
	temp += num;

	for(i=0; i<valSize; i++)
	{
		num.Format("%04d\t0x%04x,0x%02x\n", i,0x0000+i,(unsigned char)pVal[i]);
		temp += num;
	}

	SAFE_DELETE_ARRAY(pVal);
	char* p = temp.GetBuffer();
	if (_wfopen_s(&file,Path.GetBuffer(),_T("wt+"))==0)
	{
		fwrite(p,1,strlen(p),file);
		fclose(file);
		Path = Path+_T(" saved");
		m_pInterface->AddLog(Path,COLOR_BLUE);
	}

	m_pInterface->AddLog(_T("Read OTP End!"));
	return;
}

void COtpHLT7109::CreateBurningData(void)
{
	int i = 0;
	//////////////////////////////////Basic//////////////////////////////////////
	Data_Write.data_Basic[0] = FLAG_VALID;
	for (i = 0; i < SIZE_BASIC_GROUP-1; i++)
	{
		Data_Write.data_Basic[i+1] = OTP_Data_Write.BaseInfo[i];
	}
	//////////////////////////////////AWB//////////////////////////////////////
	Data_Write.data_WB[0] = FLAG_VALID;
	for (i = 0; i < SIZE_WB_GROUP-1; i++)
	{
		Data_Write.data_WB[i+1] = OTP_Data_Write.AWBInfo[i];
	}
	//////////////////////////////////LSC//////////////////////////////////////
	Data_Write.data_Lenc[0] = FLAG_VALID;
	if (m_LSCItem != 2)
	{
		if (m_LSCItem == 0)
		{
			for (i = 0; i < SIZE_MTK_LSC; i++)
			{
				Data_Write.data_Lenc[i+1] = HLT7109_LSCWriteData[i] = m_mtklscdata[i];
			}
			for (i=0; i<2; i++)
			{
				Data_Write.lsc_rsvd[i] = 0;
			}
		}
		else	if (m_LSCItem == 1)
		{
			for (i = 0; i < SIZE_LSC_GROUP-1; i++)
			{
				Data_Write.data_Lenc[i+1] = HLT7109_LSCWriteData[i] = m_quallscdata[i];
			}
		}


	}
	///////////////////////////////////AF/////////////////////////////////////
	Data_Write.data_AF[0] = FLAG_VALID;
	for (i = 0; i < SIZE_AF_GROUP-1; i++)
	{
		Data_Write.data_AF[i+1] = OTP_Data_Write.AFInfo[i];
	}
	//////////////////////////////////PDAF////////////////////////////////////
	if (m_EnPDAF)
	{
		Data_Write.data_PDAF[0] = FLAG_VALID;
		for (i = 0; i < SIZE_PDAF_PROC1; i++)
		{
			Data_Write.pdaf_proc1[i] = OTP_Data_Write.PDAF1[i] = m_proc1data[i];
		}
		for (i=0; i<2; i++)
		{
			Data_Write.pdaf_rsv1[i] = 0;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	HLT7109_CalcAllCheckSum();
	return;
}

BOOL COtpHLT7109::CheckOTP()
{
	if (!HLT7109_OTPVerify())		return FALSE;

	return TRUE;
}

BOOL COtpHLT7109::VerifyLSC()
{
	if (m_LSCItem == 2)
	{
		if (!HLT7109_VerifyLSC())		return FALSE; //for sensor LSC
	}
	return TRUE;
}

BOOL COtpHLT7109::VerifyPreWB()
{
	if (m_EnAWB)
	{
		if (!GetOTPRGBG())		return FALSE;
		OV13855_ApplyAWB(m_wbCurrent,m_goldenrg,m_goldenbg);
	}
	return TRUE;
}

BOOL COtpHLT7109::VerifyAWB()
{
	if (m_EnAWB)
	{
		if (!HLT7109_VerifyAWB())		return FALSE;
	}
	return TRUE;
}

// BOOL COtpHLT7109::EndProgram()
// {
// 	return TRUE;
// }


BOOL COtpHLT7109::GetAWB()
{
	if (m_EnAWB)
	{
		if (!HLT7109_GetAWBData(OTP_Data_Write))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL COtpHLT7109::HLT7109_GetAWBData(HLT7109OTPDataSpec &awbdata)
{
	m_pInterface ->AddLog(_T("Get AWB Data Start"));

	if (!HLT7109_GetCurrentAWB(&m_wbCurrent))		return FALSE;

	awbdata.AWBInfo[0] = (m_wbCurrent.r>>8)&0x0f;
	awbdata.AWBInfo[1] = m_wbCurrent.r&0xff;
	awbdata.AWBInfo[2] = (m_wbCurrent.gr>>8)&0x0f;
	awbdata.AWBInfo[3] = m_wbCurrent.gr&0xff;
	awbdata.AWBInfo[4] = (m_wbCurrent.gb>>8)&0x0f;
	awbdata.AWBInfo[5] = m_wbCurrent.gb&0xff;
	awbdata.AWBInfo[6] = (m_wbCurrent.b>>8)&0x0f;
	awbdata.AWBInfo[7] = m_wbCurrent.b&0xff;

	awbdata.AWBInfo[8] = (m_goldenr>>8)&0x0f;
	awbdata.AWBInfo[9] = m_goldenr&0xff;
	awbdata.AWBInfo[10] = (m_goldengr>>8)&0x0f;
	awbdata.AWBInfo[11] = m_goldengr&0xff;
	awbdata.AWBInfo[12] = (m_goldengb>>8)&0x0f;
	awbdata.AWBInfo[13] = 	m_goldengb&0xff;
	awbdata.AWBInfo[14] = (m_goldenb>>8)&0x0f;
	awbdata.AWBInfo[15] = 	m_goldenb&0xff;

	awbdata.AWBInfo[16] =0;
	awbdata.AWBInfo[17] = 0;


	m_pInterface ->AddLog(_T("Get AWB Data End"));
	OV13855_ApplyAWB(m_wbCurrent, m_goldenrg, m_goldenbg);
	return TRUE;
}

BOOL COtpHLT7109::CheckOTPAWB()
{
	if (m_EnAWB)
	{
		m_pInterface ->AddLog(_T("Check Apply AWB Data Start"));
		if (!GetAWBInfo(&m_wbCurrent))		return FALSE;

		if (!HLT7109_CheckAWB(m_wbCurrent,1))				return FALSE;

		//Apply After
		m_pInterface->AddLog(_T("After Apply WB:"));
		info.Format(_T("RG_fin = %02d"),m_wbCurrent.rg);
		m_pInterface->AddLog(info);
		info.Format(_T("BG_fin = %02d"),m_wbCurrent.bg);
		m_pInterface->AddLog(info);
		info.Format(_T("GG_fin = %02d"),m_wbCurrent.gb_gr);
		m_pInterface->AddLog(info);
		info.Format(_T("R = %02d"),m_wbCurrent.r);
		m_pInterface->AddLog(info);
		info.Format(_T("B = %02d"),m_wbCurrent.b);
		m_pInterface->AddLog(info);
		info.Format(_T("Gr = %02d"),m_wbCurrent.gr);
		m_pInterface->AddLog(info);
		info.Format(_T("Gb = %02d"),m_wbCurrent.gb);
		m_pInterface->AddLog(info);
		m_pInterface ->AddLog(_T("Check Apply AWB Data Pass!"),COLOR_BLUE);
	}

	return TRUE;
}

BOOL COtpHLT7109::HLT7109_GetCurrentAWB(tagAwbRatio* wb)
{
	if (!InitAWB())		return FALSE;

	if (!GetAWBInfo(&m_wbCurrent))		return FALSE;

	if (!HLT7109_CheckAWB(m_wbCurrent,0))				return FALSE;

	//Apply Before
	m_pInterface->AddLog(_T("Before Apply WB:"));
#ifndef  Raw8Data
	info.Format(_T("OB(10bit) = %02d"),SENSOROB);
#else
	info.Format(_T("OB(8bit) = %02d"),SENSOROB);
#endif
	m_pInterface->AddLog(info);
	info.Format(_T("RG_fin = %02d"),m_wbCurrent.rg);
	m_pInterface->AddLog(info);
	info.Format(_T("BG_fin = %02d"),m_wbCurrent.bg);
	m_pInterface->AddLog(info);
	info.Format(_T("GG_fin = %02d"),m_wbCurrent.gb_gr);
	m_pInterface->AddLog(info);
	info.Format(_T("R = %02d"),m_wbCurrent.r);
	m_pInterface->AddLog(info);
	info.Format(_T("B = %02d"),m_wbCurrent.b);
	m_pInterface->AddLog(info);
	info.Format(_T("G = %02d"),m_wbCurrent.g);
	m_pInterface->AddLog(info);

	Applyawbdata[0] = m_wbCurrent.r;//for data save
	Applyawbdata[1] = m_wbCurrent.gr;
	Applyawbdata[2] = m_wbCurrent.gb;
	Applyawbdata[3] = m_wbCurrent.b;
	Applyawbdata[4] = m_wbCurrent.rg;
	Applyawbdata[5] = m_wbCurrent.bg;
	Applyawbdata[6] = m_wbCurrent.gb_gr;

	return TRUE;
}

BOOL COtpHLT7109::GetAWBInfo(tagAwbRatio* wb)
{
	// getwb
	ImageInfo imgInfo = m_pInterface->GetImage();

	int width = imgInfo.width;
	int height= imgInfo.height;

	wb->r = ImageProc::GetAveR_ROI_Raw(imgInfo.Raw_Buf, imgInfo.width, imgInfo.height, m_wbCtrl.roi, imgInfo.RawFormat);
	wb->g = ImageProc::GetAveG_ROI_Raw(imgInfo.Raw_Buf, imgInfo.width, imgInfo.height, m_wbCtrl.roi, imgInfo.RawFormat);
	wb->b = ImageProc::GetAveB_ROI_Raw(imgInfo.Raw_Buf, imgInfo.width, imgInfo.height, m_wbCtrl.roi, imgInfo.RawFormat);
	wb->gb = ImageProc::GetAveGb_ROI_Raw(imgInfo.Raw_Buf, imgInfo.width, imgInfo.height, m_wbCtrl.roi, imgInfo.RawFormat);
	wb->gr = ImageProc::GetAveGr_ROI_Raw(imgInfo.Raw_Buf, imgInfo.width, imgInfo.height, m_wbCtrl.roi, imgInfo.RawFormat);

	//check...
	wb->rg		= (UINT)(1.0 * (wb->r-SENSOROB) / (wb->g-SENSOROB)*AWB_FACTOR + 0.5); 
	wb->bg		= (UINT)(1.0 * (wb->b-SENSOROB) / (wb->g-SENSOROB)*AWB_FACTOR + 0.5);
	wb->gb_gr	= (UINT)(1.0 * (wb->gb-SENSOROB)/ (wb->gr-SENSOROB)*AWB_FACTOR+ 0.5);

	//raw8
// 	wb->r = (UINT)(wb->r/4 + 0.5);
// 	wb->g = (UINT)(wb->g/4 + 0.5);
// 	wb->b = (UINT)(wb->b/4 + 0.5);
// 	wb->gb = (UINT)(wb->gb/4 + 0.5);
// 	wb->gr = (UINT)(wb->gr/4 + 0.5);

	m_szStr.Format(_T("R/G: %d B/G: %d"), wb->rg, wb->bg);
	m_pInterface->AddLog(m_szStr, COLOR_BLUE);
	return TRUE;
}

BOOL COtpHLT7109::HLT7109_CheckAWB(tagAwbRatio wb, int type)
{
	BOOL bRet = TRUE;
	float rg_diff = 0; float bg_diff = 0;
	m_pInterface ->AddLog(_T("Check AWB Data Start!"));

	switch(type)// 0:apply前, 1:apply 后, 2:OTP内awb
	{
	case 0:
		rg_diff = (float)(abs((int)(wb.rg-m_goldenrg))*1.0/m_goldenrg)*100;
		bg_diff =(float)(abs((int)(wb.bg-m_goldenbg))*1.0/m_goldenbg)*100;
		//加入GG_diff?

		if (wb.rg<m_rgainl 
		 ||wb.bg<m_bgainl
	     ||wb.rg>m_rgainh
	     ||wb.bg>m_bgainh)
		{
			m_pInterface ->AddLog(_T("Apply前wb gain值超出范围"),COLOR_RED);
			return FALSE;
		}

		if (rg_diff>m_orgdif || bg_diff > m_orgdif)
		{
			m_pInterface ->AddLog(_T("Apply前wb gain diff超出范围"),COLOR_RED);
		    return FALSE;
		}
		else
		{
			info.Format(_T("Apply前rgdiff: %.2f,bgdiff:%.2f在允许范围%.2f内"),rg_diff,bg_diff,m_orgdif);
			m_pInterface->AddLog(info);
		}
		break;

	case 1:
		rg_diff = (float)(abs((int)(wb.rg-m_goldenrg))*1.0/m_goldenrg)*100;
		bg_diff = (float)(abs((int)(wb.bg-m_goldenbg))*1.0/m_goldenbg)*100;

		if (rg_diff>m_rgaindif || bg_diff > m_bgaindif)
		{
			m_pInterface ->AddLog(_T("Apply后wb gain diff超出范围"),COLOR_RED);
			return FALSE;
		}
		else
		{
			info.Format(_T("Apply后rgdiff: %.2f,bgdiff:%.2f在允许范围%.2f，%.2f内"),rg_diff,bg_diff,m_rgaindif,m_bgaindif);
			m_pInterface->AddLog(info);
		}
		break;

	case 2:
		rg_diff = (float)(abs((int)(wb.rg-m_goldenrg))*1.0/m_goldenrg)*100;
		bg_diff = (float)(abs((int)(wb.bg-m_goldenbg))*1.0/m_goldenbg)*100;

		if (wb.rg<m_rgainl 
			||wb.bg<m_bgainl
			||wb.rg>m_rgainh
			||wb.bg>m_bgainh)
		{
			m_pInterface ->AddLog(_T("OTP内 gain值超出范围"),COLOR_RED);
			return FALSE;
		}

		if (rg_diff>m_rgaindif || bg_diff > m_bgaindif)
		{
			m_pInterface ->AddLog(_T("OTP内 gain diff超出范围"),COLOR_RED);
			return FALSE;
		}
		else
		{
			info.Format(_T("ApplyOTP内gain后rgdiff: %.2f,bgdiff:%.2f在允许范围%.2f，%.2f内"),rg_diff,bg_diff,m_rgaindif,m_bgaindif);
			m_pInterface->AddLog(info);
		}
		break;

	default:
		return FALSE;
		break;
	}
	m_pInterface ->AddLog(_T("Check AWB Data Pass!"),COLOR_BLUE);
	return TRUE;
}

void COtpHLT7109::HLT7109_GetBasicData(void)
{
//	m_pInterface ->AddLog(_T("Get Basic Data Start"));
	CTime time = CTime::GetCurrentTime();

	OTP_Data_Write.BaseInfo[0] = BASIC_MID;
	OTP_Data_Write.BaseInfo[1] = BASIC_APID;
	OTP_Data_Write.BaseInfo[2] = BASIC_LENSID;
	OTP_Data_Write.BaseInfo[3] = BASIC_VCMID;
	OTP_Data_Write.BaseInfo[4] = BASIC_DRIVERICID;
	OTP_Data_Write.BaseInfo[5] = BASIC_IRID;
	OTP_Data_Write.BaseInfo[6] = BASIC_BLACKMASK;
	OTP_Data_Write.BaseInfo[7] = 0;
	OTP_Data_Write.BaseInfo[8] = time.GetYear() - 2000;
	OTP_Data_Write.BaseInfo[9] = time.GetMonth();
	OTP_Data_Write.BaseInfo[10] = time.GetDay();
	OTP_Data_Write.BaseInfo[11] = 0;
	OTP_Data_Write.BaseInfo[12] = 0;
	OTP_Data_Write.BaseInfo[13] = 0;
	OTP_Data_Write.BaseInfo[14] = 0;
	OTP_Data_Write.BaseInfo[15] = 0;
	OTP_Data_Write.BaseInfo[16] = 0;
	OTP_Data_Write.BaseInfo[17] = 0;

// 	for (int i=0; i<6; i++)
// 	{
// 		OTP_Data_Write.BaseInfo[14+i] = 0;
// 	}
// 	OTP_Data_Write.BaseInfo[7] = BASIC_IRID;
// 	OTP_Data_Write.BaseInfo[9] = BASIC_AFFLAG;
// 	OTP_Data_Write.BaseInfo[10] = BASIC_LIGHTFLAG;
// 	OTP_Data_Write.BaseInfo[11] = BASIC_LSCVER;
// 	OTP_Data_Write.BaseInfo[12] = BASIC_CALIVER1;
// 	OTP_Data_Write.BaseInfo[13] = BASIC_CALIVER2;
// 	OTP_Data_Write.BaseInfo[14] = BASIC_CALIVER3;
// 	OTP_Data_Write.BaseInfo[15] = BASIC_CALIVER4;
// 	OTP_Data_Write.BaseInfo[16] = BASIC_SERIALNO1;
// 	OTP_Data_Write.BaseInfo[17] = BASIC_SERIALNO2;
// 	OTP_Data_Write.BaseInfo[18] = BASIC_SERIALNO3;
// 	OTP_Data_Write.BaseInfo[19] = BASIC_PDVER;

	info.Format(_T("MID = 0x%02x"),BASIC_MID);
	m_pInterface->AddLog(info);
	info.Format(_T("LensID = 0x%02x"),BASIC_LENSID);
	m_pInterface->AddLog(info);
	info.Format(_T("VCMID = 0x%02x"),BASIC_VCMID);
	m_pInterface->AddLog(info);
	info.Format(_T("DriverICID = 0x%02x"),BASIC_DRIVERICID);
	m_pInterface->AddLog(info);
// 	info.Format(_T("IRID =0x%02x"),BASIC_IRID);
// 	m_pInterface->AddLog(info);

	m_pInterface ->AddLog(_T("Get Basic Data"));
	return;
}

void COtpHLT7109::HLT7109_GetAFData(void)
{
//	m_pInterface ->AddLog(_T("Get AF Data Start"));

	GT24C64_Read(m_addrFlag.af+1, OTP_Data_Write.AFInfo,12);

	OTP_Data_Write.AFInfo[2] = (m_macCode>>8)&0x0f;//10cm
	OTP_Data_Write.AFInfo[3] = m_macCode&0xff;
	OTP_Data_Write.AFInfo[6] = (m_infCode>>8)&0x0f;//10m
	OTP_Data_Write.AFInfo[7] = m_infCode&0xff;
	OTP_Data_Write.AFInfo[8] = 0;
	OTP_Data_Write.AFInfo[9] = 0;

	m_pInterface ->AddLog(_T("Get AF Data"));
	return;
}
void COtpHLT7109::HLT7109_CalcAllCheckSum(void)
{
	int i = 0;
	int sum = 0;

	//Basic
	if (m_EnBasic)
	{
		sum = SumCalc(OTP_Data_Write.BaseInfo,SIZE_BASIC_GROUP-2,255,1);
		Data_Write.data_Basic[SIZE_BASIC_GROUP-1] = (sum>>8)&0xff;
		Data_Write.data_Basic[SIZE_BASIC_GROUP] = sum&0xff;
	}

	//AWB
	if (m_EnAWB)
	{
		// 	for (i=0; i<SIZE_BASIC_GROUP-1; i++)
		// 	{
		// 		sum += OTP_Data_Write.BaseInfo[i];
		// 	}
		// 	for (i=0; i<SIZE_WB_GROUP;i++)
		// 	{
		// 		sum += OTP_Data_Write.AWBInfo[i];
		// 	}
		// 	sum = sum%255 + 1;
		// 	Data_Write.data_Basic[1] = sum;
		sum = SumCalc(OTP_Data_Write.AWBInfo,SIZE_WB_GROUP-2,255,1);
		Data_Write.data_WB[SIZE_WB_GROUP-1] = (sum>>8)&0xff;
		Data_Write.data_WB[SIZE_WB_GROUP] = sum&0xff;
	}

	// LSC
	if (m_EnLSC)
	{
		if (m_LSCItem != 2)
		{
			sum = SumCalc(HLT7109_LSCWriteData, SIZE_LSC_GROUP-4, 255,1);
			Data_Write.data_Lenc[SIZE_LSC_GROUP-1] = (sum>>8)&0xff;
			Data_Write.data_Lenc[SIZE_LSC_GROUP] = sum&0xff;
		}
	}

	//AF	
	if (m_EnAF)
	{
		sum = SumCalc(OTP_Data_Write.AFInfo,SIZE_AF_GROUP-2,255,1);
		Data_Write.data_AF[SIZE_AF_GROUP-1] = (sum>>8)&0xff;
		Data_Write.data_AF[SIZE_AF_GROUP] = sum&0xff;
	}

	if (m_EnPDAF)
	{
		//PDAF1
		sum = SumCalc(OTP_Data_Write.PDAF1,SIZE_PDAF_PROC1,255,1);
		proc1sum = sum;
		Data_Write.pdaf_proc1checksum[0] = (proc1sum>>8) & 0xff;
		Data_Write.pdaf_proc1checksum[1] = proc1sum & 0xff;
	}

	return;
}


BOOL COtpHLT7109::HLT7109_OtpWrite(void)
{
	m_pInterface->AddLog(_T("OTP Burning Start!"));

	if (!ProgrammingBasic())  return FALSE;

	if (m_EnAWB)
	{
		if (!ProgrammingAWB())  return FALSE;
	}

	if (m_EnLSC)
	{
		if (!ProgrammingLSC())  return FALSE;
	}

	if (m_EnAF)
	{
		if (!ProgrammingAF())  return FALSE;
	}

	if (m_EnPDAF)
	{
		BOOL bRet = ProgrammingPDAF();

		if (bRet)
		{
			m_pInterface->AddLog(_T("OTP Burning Success!"),COLOR_BLUE);
		}
		return bRet;
	}

	m_pInterface->AddLog(_T("OTP Burning Success!"),COLOR_BLUE);
	return TRUE;
}

BOOL COtpHLT7109::ProgrammingBasic()
{
	if (m_LSCItem == 0)
	{
		if (GT24C64_Programming(m_addrFlag.moduleInfo,Data_Write.data_Basic,sizeof(Data_Write.data_Basic)) != TRUE)
		{
			m_pInterface->AddLog(_T("Basic Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
	}
	else if (m_LSCItem == 2)
	{
		if (m_sensoritem == 0)
		{
			if (m_LscGroup>m_grouplimit)
			{
				m_pInterface->AddLog(_T("OTP只允许烧录第一组!"),COLOR_RED);		
				return FALSE;
			}
			Programming_3L8Basic();
		}

	}

	msleep(10);

	return TRUE;
}

void COtpHLT7109::Programming_3L8Basic()
{
	int i = 0;
	BYTE WriteData[64] = {0};
	if (m_LscGroup == 1)
	{
		for (i=0; i<SIZE_BASIC_GROUP+1; i++)
		{
			WriteData[i] = Data_Write.data_Basic[i];
		}
		S5K3L8_WriteOTPPage(1,WriteData);
	}
	else if (m_LscGroup == 2)
	{
		for (i=0; i<SIZE_BASIC_GROUP+1; i++)
		{
			WriteData[i+OTPGAP] = Data_Write.data_Basic[i];
		}
		S5K3L8_WriteOTPPage(1,WriteData);
	}
	return;
}

BOOL COtpHLT7109::ProgrammingAWB()
{
	if (m_LSCItem != 2)
	{
		if (GT24C64_Programming(m_addrFlag.awb,Data_Write.data_WB,sizeof(Data_Write.data_WB)) != TRUE)
		{
			m_pInterface->AddLog(_T("AWB Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
		msleep(10);
	}
	else
	{
		if (m_sensoritem == 0)
		{
			Programming_3L8AWB();
		}
	}

	return TRUE;
}

void COtpHLT7109::Programming_3L8AWB()
{
	int i = 0;
	BYTE WriteData[64] = {0};
	if (m_LscGroup == 1)
	{
		for (i=0; i<SIZE_WB_GROUP; i++)
		{
			WriteData[i+9] = Data_Write.data_WB[i];
		}
		S5K3L8_WriteOTPPage(1,WriteData);
	}
	else if (m_LscGroup == 2)
	{
		for (i=0; i<SIZE_BASIC_GROUP+1; i++)
		{
			WriteData[i+9+OTPGAP] = Data_Write.data_WB[i];
		}
		S5K3L8_WriteOTPPage(1,WriteData);
	}
	return;
}

BOOL COtpHLT7109::ProgrammingLSC()
{
	if (m_LSCItem != 2)
	{
		if (GT24C64_Programming(m_addrFlag.lsc,Data_Write.data_Lenc,sizeof(Data_Write.data_Lenc)) != TRUE)
		{
			m_pInterface->AddLog(_T("LSC Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
		msleep(10);
	}
	else
	{
		if (m_sensoritem == 0)
		{
			Programming_3L8LSC();
		}
	}

	return TRUE;
}

BOOL COtpHLT7109::Programming_3L8LSC()
{
	BYTE _LSCData[417] = {0};
	if (m_LscGroup == 1)//OTP开头已有检测过
	{
		S5K3L8_ReadOTPPage(26, _LSCData);
	}
	if (m_LscGroup == 2)
	{
		S5K3L8_ReadOTPPage(19, _LSCData);
	}
	if ( (_LSCData[8]|_LSCData[7]|_LSCData[6]|_LSCData[5]|_LSCData[4]|_LSCData[3]|_LSCData[2]|_LSCData[1]|_LSCData[0]) != 0)
	{
		m_pInterface->AddLog(_T("OTP内已有LSC数据！"), COLOR_RED);
		return FALSE;
	}

	CString tempStr;
	CString strWorkPath  = GetModulePath()+"LSC\\Sensor\\3L8";
	tempStr.Format(_T("\\3L8LSCOTP.sset"));
	tempStr = strWorkPath + tempStr;
	
	CStdioFile file;
	if ( !file.Open( (tempStr), CFile::modeRead | CFile::typeText ) )
	{
		m_pInterface->AddLog(_T("打开文件失败！"), COLOR_RED);
		return FALSE;
	}

	int j = 0;

	CString strLine;

	int	 intVal;
	int  intLSCAddr;
	int  intLSCValue; 

	int nCnt = 0;
	int i=0;
	int k=0;
	while ( file.ReadString(strLine) )
	{ 
		strLine.TrimLeft();
		strLine.TrimRight();//去除左右空格
		if ( strLine.Find( _T("s"), 0 ) == 0 )
		{
			int nLength =strLine.GetLength();
			if(nLength==7)//setting
			{
				intVal = strtoul( CT2A(strLine.Mid(1,6)), NULL, 16 );
				intLSCAddr = intVal>>8;//16进制
				intLSCValue = intVal%256;
				if (!m_dummyburn)
				{
					i2c_write_byte( intLSCAddr, intLSCValue );
				}
			}
			else//写入数据
			{
				int nAddrStart = strtoul( CT2A(strLine.Mid(1,4)), NULL, 16);
				int nLength2 = ( strLine.GetLength() - 5 )/2;
				BYTE* ParaList = (BYTE*) new BYTE[nLength2*sizeof(BYTE)];

				memset( ParaList, 0, nLength2); 

				for(i = 0; i < nLength2; i++ )
				{
					ParaList[i] = (BYTE)strtoul( CT2A(strLine.Mid( 5+i*2, 2 )), NULL, 16);
					if (!m_dummyburn)
					{
						i2c_write_byte(nAddrStart+i,ParaList[i]);
					}
					HLT7109_LSCWriteData[k] = ParaList[i];
					k++;
				}
			}
		}

	}
	file.Close();

	//calc lsc checksum
	HLT7109_LSCchecksum=0;
	if(m_LscGroup==1)
	{
		for (i=0;i<417;i++)
		{
			HLT7109_LSCchecksum+=HLT7109_LSCWriteData[i+31];
		}
		HLT7109_LSCchecksum=HLT7109_LSCchecksum%255+1;
	}
	if (m_LscGroup==2)
	{
		for (i=0;i<417;i++)
		{
			HLT7109_LSCchecksum+=HLT7109_LSCWriteData[i+62];
		}
		HLT7109_LSCchecksum=HLT7109_LSCchecksum%255+1;
	}

	HLT7109_WriteLSCFlagSum();

	return TRUE;
}

void COtpHLT7109::HLT7109_WriteLSCFlagSum()
{
	Data_Write.data_Lenc[0] = FLAG_VALID;
	Data_Write.data_Lenc[1] = HLT7109_LSCchecksum;

	BYTE WriteData[64] = {0};
	int i = 0;
	if (m_LscGroup == 1)
	{
		for (i=0; i<2; i++)
		{
			WriteData[i+22] = Data_Write.data_Lenc[i];
		}
		if (!m_dummyburn)
		{
			S5K3L8_WriteOTPPage(1,WriteData);
		}
	}
	if (m_LscGroup==2)
	{
		for (i=0; i<2; i++)
		{
			WriteData[i+22+OTPGAP] = Data_Write.data_Lenc[i];
		}
		if (!m_dummyburn)
		{
			S5K3L8_WriteOTPPage(1,WriteData);
		}
	}
	return;
}

BOOL COtpHLT7109::ProgrammingAF()
{
	if (m_LSCItem != 2)
	{
		if (GT24C64_Programming(m_addrFlag.af,Data_Write.data_AF,sizeof(Data_Write.data_AF)) != TRUE)
		{
			m_pInterface->AddLog(_T("AF Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
		msleep(10);
	}
	else
	{
		if (m_sensoritem == 0)
		{
			Programming_3L8AF();
		}
	}

	return TRUE;
}

void COtpHLT7109::Programming_3L8AF()
{
	int i = 0;
	BYTE WriteData[64] = {0};
	if (m_LscGroup == 1)
	{
		for (i=0; i<SIZE_AF_GROUP+1; i++)
		{
			WriteData[i+16] = Data_Write.data_AF[i];
		}
		S5K3L8_WriteOTPPage(1,WriteData);
	}
	else if (m_LscGroup == 2)
	{
		for (i=0; i<SIZE_AF_GROUP+1; i++)
		{
			WriteData[i+16+OTPGAP] = Data_Write.data_AF[i];
		}
		S5K3L8_WriteOTPPage(1,WriteData);
	}
	return;
}

BOOL COtpHLT7109::ProgrammingPDAF()
{
	BOOL bRet = TRUE;
	//MTK
	if (m_EnPDAF)				
	{				

		BYTE* flag = new BYTE[1];			
		flag[0] = (BYTE)m_flagValid;			
		BYTE* sum1 = new BYTE[1];			
		sum1[0] = (BYTE)proc1sum;		
		do 			
		{			
			if (GT24C64_Programming(m_addrFlag.pdaf, flag, 1 ) != TRUE)		
			{		
				m_pInterface->AddLog(_T("PDAF Flag Write Fail!"),COLOR_RED);	
				bRet &= FALSE;	
				break;	
			}		
			else		
			{		
				if (GT24C64_Programming(m_addrFlag.pdaf+1, OTP_Data_Write.PDAF1, sizeof(OTP_Data_Write.PDAF1)) != TRUE)	
				{	
					m_pInterface->AddLog(_T("PDAF Proc1 Data Write Fail!"),COLOR_RED);
					bRet &= FALSE;
					break;
				}	

				if (GT24C64_Programming(m_addrFlag.pdaf+497, Data_Write.pdaf_rsv1, sizeof(Data_Write.pdaf_rsv1)) != TRUE)	
				{	
					m_pInterface->AddLog(_T("PDAF Proc1 Data Write Fail!"),COLOR_RED);
					bRet &= FALSE;
					break;
				}	

				if (GT24C64_Programming(m_addrSum.pdaf, Data_Write.pdaf_proc1checksum, 2) != TRUE)	
				{	
					m_pInterface->AddLog(_T("PDAF Proc1 CheckSum Write Fail!"),COLOR_RED);
					bRet &= FALSE;
					break;
				}	
			}		
		} while (0);			

		SAFE_DELETE_ARRAY(flag);			
		SAFE_DELETE_ARRAY(sum1);			
	}				

	//Qual
	return bRet;
}

BOOL COtpHLT7109::HLT7109_OTPVerify(void)
{
	if (!m_dummyburn)//!m_dummyburn
	{
		if (!HLT7109_CheckOTPReadWrite())		return FALSE;
	}
	else 
	{
		if (m_LSCItem == 2)
		{
			if (m_sensoritem == 0)
			{
				Sensor3L8_StreamOn();
			}
		}
		m_pInterface->AddLog(_T("This is Dummy Burn!"),COLOR_RED);
	}
   return TRUE;
}

BOOL COtpHLT7109::HLT7109_CheckOTPReadWrite(void)
{
	m_pInterface->AddLog(_T("Check OTP Data Start!"));

	if (m_LSCItem == 0)
	{	
			if (GT24C64_Read(m_addrFlag.moduleInfo, Data_Read.data_Basic,sizeof(Data_Read.data_Basic)) != TRUE)
		{
			m_pInterface->AddLog(_T("Read Basic Data Fail!"),COLOR_RED);		
			return FALSE;
		}
		else
		{
			if (!CheckEqual(Data_Write.data_Basic,Data_Read.data_Basic,sizeof(Data_Write.data_Basic)))
			{
				m_pInterface->AddLog(_T("Check Basic Data Fail!"),COLOR_RED);
				return FALSE;
			}
		}

		if (m_EnAWB)
		{	
			if (GT24C64_Read(m_addrFlag.awb, Data_Read.data_WB,sizeof(Data_Read.data_WB)) != TRUE)
			{
				m_pInterface->AddLog(_T("Read AWB Data Fail!"),COLOR_RED);		
				return FALSE;
			}
			else
			{
				if (!CheckEqual(Data_Write.data_WB,Data_Read.data_WB,sizeof(Data_Write.data_WB)))
				{
					m_pInterface->AddLog(_T("Check AWB Data Fail!"),COLOR_RED);
					return FALSE;
				}
			}
		}

		if (m_EnLSC)
		{	
			if (GT24C64_Read(m_addrFlag.lsc, Data_Read.data_Lenc,sizeof(Data_Read.data_Lenc)) != TRUE)
			{
				m_pInterface->AddLog(_T("Read LSC Data Fail!"),COLOR_RED);		
				return FALSE;
			}
			else
			{
				if (!CheckEqual(Data_Write.data_Lenc,Data_Read.data_Lenc,sizeof(Data_Write.data_Lenc)))
				{
					m_pInterface->AddLog(_T("Check LSC Data Fail!"),COLOR_RED);
					return FALSE;
				}
			}
		}

		if (m_EnAF)
		{
			if (GT24C64_Read(m_addrFlag.af, Data_Read.data_AF,sizeof(Data_Read.data_AF)) != TRUE)
			{
				m_pInterface->AddLog(_T("Read AF Data Fail!"),COLOR_RED);		
				return FALSE;
			}
			else
			{
				if (!CheckEqual(Data_Write.data_AF,Data_Read.data_AF,sizeof(Data_Write.data_AF)))
				{
					m_pInterface->AddLog(_T("Check AF Data Fail!"),COLOR_RED);
					return FALSE;
				}
			}
		}

		BOOL result = TRUE;
		if (m_EnPDAF)
		{
			BYTE *read_flag = new BYTE[1];
			BYTE *flag_check = new BYTE[1];
			flag_check[0] = (BYTE)m_flagValid;
			BYTE *Read_Sum = new BYTE[1];
			GT24C64_Read(m_addrSum.pdaf, Read_Sum, 1);
			BYTE *Write_Sum = new BYTE[1];
			Write_Sum[0] = (BYTE)proc1sum;

			/////////////////////////////////PDAF//////////////////////////////////////
			do 
			{
				if (GT24C64_Read(m_addrFlag.pdaf, read_flag, 1 ) != TRUE)
				{
					m_pInterface->AddLog(_T("Read PDAF Flag Fail!"),COLOR_RED);		
					result&= FALSE;
					break;
				}
				else
				{
					if (!CheckEqual(read_flag, flag_check,1)) 
					{
						m_pInterface->AddLog(_T("Check PDAF Flag Data Fail!"),COLOR_RED);
						result&= FALSE;
						break;
					}

					if (GT24C64_Read(m_addrFlag.pdaf+1, Data_Read.pdaf_proc1, sizeof (Data_Read.pdaf_proc1)) != TRUE)
					{
						m_pInterface->AddLog(_T("Read PDAF Data Fail!"),COLOR_RED);
						result&= FALSE;
						break;
					}

					if (GT24C64_Read(m_addrFlag.pdaf+497, Data_Read.pdaf_rsv1, sizeof (Data_Read.pdaf_rsv1)) != TRUE)
					{
						m_pInterface->AddLog(_T("Read PDAF rsv Data Fail!"),COLOR_RED);
						result&= FALSE;
						break;
					}

					if (GT24C64_Read(m_addrSum.pdaf, Data_Read.pdaf_proc1checksum, sizeof (Data_Read.pdaf_proc1checksum)) != TRUE)
					{
						m_pInterface->AddLog(_T("Read PDAF Checksum Data Fail!"),COLOR_RED);
						result&= FALSE;
						break;
					}

					if (!CheckEqual(Data_Write.pdaf_proc1, Data_Read.pdaf_proc1,sizeof(Data_Read.pdaf_proc1)))
					{
						m_pInterface->AddLog(_T("Check PDAF Data Fail!"),COLOR_RED);
						result&= FALSE;
						break;
					}

					if (!CheckEqual(Data_Write.pdaf_rsv1, Data_Read.pdaf_rsv1,sizeof(Data_Read.pdaf_rsv1)))
					{
						m_pInterface->AddLog(_T("Check rsv PDAF Data Fail!"),COLOR_RED);
						result&= FALSE;
						break;
					}

					if (!CheckEqual(Data_Write.pdaf_proc1checksum, Data_Read.pdaf_proc1checksum,sizeof(Data_Read.pdaf_proc1checksum)))
					{
						m_pInterface->AddLog(_T("Check PDAF checksum Data Fail!"),COLOR_RED);
						result&= FALSE;
						break;
					}
				}
				////////////////////////////////////Sum//////////////////////////////////
// 				if (!CheckEqual(Read_Sum, Write_Sum,1)) 
// 				{
// 					m_pInterface->AddLog(_T("Check PDAF Flag Data Fail!"),COLOR_RED);
// 					result&= FALSE;
// 					break;
// 				}		

			} while (0);

			SAFE_DELETE_ARRAY(read_flag);
			SAFE_DELETE_ARRAY(flag_check);
			SAFE_DELETE_ARRAY(Read_Sum);
			SAFE_DELETE_ARRAY(Write_Sum);
		}

		if (result)
		{
			m_pInterface->AddLog(_T("Check OTP Data Success!"),COLOR_BLUE);	
		}
		else
		{
			return FALSE;
		}
	}

	if (m_LSCItem == 2)
	{
		if (m_sensoritem == 0)
		{
			Sensor3L8_StreamOn();//strem off能否读OTP数据?
			if (!Sensor3L8_CheckReadWrite()) return FALSE; 
			if (!Sensor3L8_CheckLSCReadWrite()) return FALSE;
			m_pInterface->AddLog(_T("Check OTP Data Success!"),COLOR_BLUE);	
		}
	}

	return TRUE;
}

BOOL COtpHLT7109::Sensor3L8_CheckReadWrite()
{
	BYTE ReadData[64] = {0};
	S5K3L8_ReadOTPPage(1,ReadData);

	int i = 0;
	if (m_LscGroup == 1)
	{
		for (i=0;i<SIZE_BASIC_GROUP+1; i++)
		{
			if (ReadData[i] != Data_Write.data_Basic[i])
			{
				m_pInterface->AddLog(_T("Basic Info读写对比失败！"),COLOR_RED);
				return FALSE;
			}
		}
		for (i=0; i<SIZE_WB_GROUP; i++)
		{
			if (ReadData[i+9] != Data_Write.data_WB[i])
			{
				m_pInterface->AddLog(_T("AWB Info读写对比失败！"),COLOR_RED);
				return FALSE;
			}
		}
		for (i=0; i<SIZE_AF_GROUP+1; i++)
		{
			if (ReadData[i+16] != Data_Write.data_AF[i])
			{
				m_pInterface->AddLog(_T("AF Info读写对比失败！"),COLOR_RED);
				return FALSE;
			}
		}
		for (i=0; i<2; i++)
		{
			if (ReadData[i+22] != Data_Write.data_Lenc[i])
			{
				m_pInterface->AddLog(_T("LSC Info读写对比失败！"),COLOR_RED);
				return FALSE;
			}
		}
	}
	if (m_LscGroup == 2)
	{
		for (i=0;i<SIZE_BASIC_GROUP+1; i++)
		{
			if (ReadData[i+OTPGAP] != Data_Write.data_Basic[i])
			{
				m_pInterface->AddLog(_T("Basic Info读写对比失败！"),COLOR_RED);
				return FALSE;
			}
		}
		for (i=0; i<SIZE_WB_GROUP; i++)
		{
			if (ReadData[i+9+OTPGAP] != Data_Write.data_WB[i])
			{
				m_pInterface->AddLog(_T("AWB Info读写对比失败！"),COLOR_RED);
				return FALSE;
			}
		}
		for (i=0; i<SIZE_AF_GROUP+1; i++)
		{
			if (ReadData[i+16+OTPGAP] != Data_Write.data_AF[i])
			{
				m_pInterface->AddLog(_T("AF Info读写对比失败！"),COLOR_RED);
				return FALSE;
			}
		}
		for (i=0; i<2; i++)
		{
			if (ReadData[i+22+OTPGAP] != Data_Write.data_Lenc[i])
			{
				m_pInterface->AddLog(_T("LSC Info读写对比失败！"),COLOR_RED);
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL COtpHLT7109::Sensor3L8_CheckLSCReadWrite()
{
	int i = 0;
	if (m_LscGroup == 1)
	{
		S5K3L8_ReadOTPPage(0x19,HLT7109_LSCReadData);
		msleep(100);
		S5K3L8_ReadOTPPage(0x1A,HLT7109_LSCReadData+64);
		msleep(100);
		S5K3L8_ReadOTPPage(0x1B,HLT7109_LSCReadData+128);
		msleep(100);
		S5K3L8_ReadOTPPage(0x1C,HLT7109_LSCReadData+192);
		msleep(100);
		S5K3L8_ReadOTPPage(0x1D,HLT7109_LSCReadData+256);
		msleep(100);
		S5K3L8_ReadOTPPage(0x1E,HLT7109_LSCReadData+320);
		msleep(100);
		S5K3L8_ReadOTPPage(0x1F,HLT7109_LSCReadData+384);
		msleep(100);	
		for (i=31;i<(417+31);i++)
		{	
			if (HLT7109_LSCReadData[i] != HLT7109_LSCWriteData[i])
			{
				m_pInterface->AddLog(_T("LSC Info读写对比失败！"));
				return 0;
			}
		}
	}

	if (m_LscGroup == 2)
	{
		S5K3L8_ReadOTPPage(0x12,HLT7109_LSCReadData);
		msleep(100);
		S5K3L8_ReadOTPPage(0x13,HLT7109_LSCReadData+64);
		msleep(100);
		S5K3L8_ReadOTPPage(0x14,HLT7109_LSCReadData+128);
		msleep(100);
		S5K3L8_ReadOTPPage(0x15,HLT7109_LSCReadData+192);
		msleep(100);
		S5K3L8_ReadOTPPage(0x16,HLT7109_LSCReadData+256);
		msleep(100);
		S5K3L8_ReadOTPPage(0x17,HLT7109_LSCReadData+320);
		msleep(100);
		S5K3L8_ReadOTPPage(0x18,HLT7109_LSCReadData+384);
		msleep(100);
		S5K3L8_ReadOTPPage(0x19,HLT7109_LSCReadData+448);
		msleep(100);
		for (i=62;i<(417+62);i++)
		{	
			if (HLT7109_LSCReadData[i] != HLT7109_LSCWriteData[i])
			{
				m_pInterface->AddLog(_T("LSC Info读写对比失败！"));
				return 0;
			}
		}
	}
	return TRUE;
}

BOOL COtpHLT7109::GetOTPRGBG()
{
	UINT r = Data_Write.data_WB[1]<<8 | Data_Write.data_WB[2];
	UINT gr = Data_Write.data_WB[3]<<8 | Data_Write.data_WB[4];
	UINT gb = Data_Write.data_WB[5]<<8 | Data_Write.data_WB[6];
	UINT b = Data_Write.data_WB[7]<<8 | Data_Write.data_WB[8];

	UINT g = (gr + gb)/2;
	
	m_wbCurrent.rg = (UINT)(1.0 * (r-SENSOROB) / (g-SENSOROB)*AWB_FACTOR + 0.5); 
	m_wbCurrent.bg = (UINT)(1.0 * (b-SENSOROB) / (g-SENSOROB)*AWB_FACTOR + 0.5); 
	return TRUE;
}

BOOL COtpHLT7109::HLT7109_VerifyLSC()
{
	m_pInterface->AddLog(_T("Verify LSC:"));
	if (m_sensoritem == 0)
	{
		m_pInterface->RestartCamera(1);
		SaveExp = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllExp"), 1);
		m_pInterface->GetImageSensor()->WriteExp(SaveExp);
		SaveGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"RecordExp",_T("ExpGain"), 1);
		m_pInterface->GetImageSensor()->WriteAnalogGain(SaveGain);

		i2c_write(0x6028, 0x2000);
		i2c_write(0x602a, 0x0cf4);
		i2c_write_byte(0x6f12,0x01);
		msleep(100);

		i2c_write(0x6028, 0x4000);
		i2c_write(0x602a, 0x0b00);
		i2c_write_byte(0x6f12, 0x01);
		msleep(100);

		i2c_write(0x602A, 0x0100);
		i2c_write_byte(0x6F12, 0x01);
		msleep(100);
		i2c_write(0x602a, 0x305A);
		i2c_write_byte(0x6f12, 0x08);
		msleep(100);	
		m_pInterface->AddLog(_T("AutoLoading LSC OK!"),COLOR_BLUE);
	}

	return TRUE;
}

BOOL COtpHLT7109::HLT7109_VerifyAWB()
{
	if (!GetAWBInfo(&m_wbCurrent))		return FALSE;
	if (!HLT7109_CheckAWB(m_wbCurrent,2))		return FALSE;
	return TRUE;
}