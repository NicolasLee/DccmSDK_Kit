#include "stdafx.h"
#include "Otp_TYD8583M.h"
#include "..\..\OTP_SensorSetting.h"

COtpTYD8583M::COtpTYD8583M(void)
{
	ZeroMemory(OTP_Data_Write.AFInfo,sizeof(OTP_Data_Write.AFInfo));
	ZeroMemory(OTP_Data_Write.BaseInfo,sizeof(OTP_Data_Write.BaseInfo));
	ZeroMemory(TYD8583M_LSCWriteData,sizeof(TYD8583M_LSCWriteData));
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


COtpTYD8583M::~COtpTYD8583M(void)
{

}

void COtpTYD8583M::PreSubclassOtp()
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
	ID->lens = BASIC_LENSID;
	ID->vcm = BASIC_VCMID;
	ID->driverIC = BASIC_DRIVERICID;
	ID->eeprom = BASIC_EEPROMID;
	ID->colortemp = BASIC_COLORTEMP;
	ID->biten = BASIC_BITEN;
	ID->sensor = BASIC_SENSOR;

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
	wbCtrl->roiCalcMode = ROI_CALC_5;
	wbCtrl->factorAmplify = AWB_FACTOR;//1024;

	WBGainRegStruct* wbReg = GetWBGainRegister();
// 	wbReg->addrGr = GAIN_ADDR_GR;
// 	wbReg->addrR = GAIN_ADDR_R;
// 	wbReg->addrB = GAIN_ADDR_B;
// 	wbReg->addrGb = GAIN_ADDR_GB;
	wbReg->dataDefaultGain = GAIN_DEFAULT_VAL;	
	wbReg->addrGrHigh = ADDR_GAIN_GR_H;
	wbReg->addrGrLow  = ADDR_GAIN_GR_L;
	wbReg->addrGbHigh = ADDR_GAIN_GB_H;
	wbReg->addrGbLow  = ADDR_GAIN_GB_L;
	wbReg->addrRHigh  = ADDR_GAIN_R_H;
	wbReg->addrRLow   = ADDR_GAIN_R_L;
	wbReg->addrBHigh  = ADDR_GAIN_B_H;
	wbReg->addrBLow   = ADDR_GAIN_B_L;

	tagAwbRatio *wbGolden = GetWBGoldenValue();		
	wbGolden->rg  = m_goldenrg;
	wbGolden->bg  = m_goldenbg;

	LencOtpControlStruct *lencCtrl = GetLencControlParams();
// 	lencCtrl->inputTarget = TYD8583M_VAL_LSC_TARGET;
// 	lencCtrl->inputLow = TYD8583M_VAL_LSC_LOW;
// 	lencCtrl->inputHigh = TYD8583M_VAL_LSC_HIGH;
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
// 	lencReg->addrCtrl = TYD8583M_ADDR_LSC_CTRL;
// 	lencReg->lencEnable = TYD8583M_ADDR_LSC_EN;
// 	lencReg->lencDisable = TYD8583M_ADDR_LSC_DIS;
//	lencReg->addrDataStart = REG_INVALID;
//	lencReg->dataSize = TYD8583M_OTP_LSC_SIZE;

	ExpControlStruct *expCtrl = GetExpControlParams();
	expCtrl->type = EXP_LINEAR;
	expCtrl->roiCalcMode = ROI_CALC_5;
	expCtrl->filterChannel = EXP_CHANNEL_G;
// 	expCtrl->target = TYD8583M_VAL_EXP_TARGET;
// 	expCtrl->lowLimit = TYD8583M_VAL_EXP_LOW;
// 	expCtrl->highLimit = TYD8583M_VAL_EXP_HIGH;
	expCtrl->effectFrame = 3;
	expCtrl->stableFrame = 3;

	ExpRegisterStruct *expReg = GetExpRegister();
	expReg->mask = 0x00FFFF;
	expReg->addrHigh = REG_INVALID;
// 	expReg->addrMid = TYD8583M_ADDR_EXP_H;
// 	expReg->addrLow = TYD8583M_ADDR_EXP_L;

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

void COtpTYD8583M::SetOtpSensorParams()
{
	OtpSensorStruct* params = GetOtpSensorParams();

	params->name = _T("TYD8583M_MIPI");
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
//	params->otpInit.params = TYD8583M_OTP_Settings;
//	params->otpInit.paramSize = sizeofTYD8583M_OTP_Settings;

	params->verifyInit.width  = 4208;
	params->verifyInit.height = 3120;
	params->verifyInit.format = IMAGE_FORMAT_OTP_RAW10;
	params->verifyInit.sequence = IMAGE_SEQUENCE_GBRG;
	params->verifyInit.mhzMCLK = 24;
// 	params->verifyInit.params = TYD8583M_OTP_Settings;
// 	params->verifyInit.paramSize = sizeofTYD8583M_OTP_Settings;	
}


BOOL COtpTYD8583M::EraseAllData()
{
	DW9763_All_Erase();
	m_pInterface->AddLog(_T("OTP所有数据已全部擦除!"));
	return TRUE;
}

void COtpTYD8583M::SetWBDefaultGain(void)
{	
	return OV13855_SetDefaultAwb();
}

BOOL COtpTYD8583M::StartProgram()
{
	return TRUE;
}

BOOL COtpTYD8583M::WriteOTP()
{
	TYD8583M_GetBasicData();
	TYD8583M_GetAFData();
	CreateBurningData();
	if (!m_dummyburn)
	{
		if (!TYD8583M_OtpWrite())//
		{return FALSE;}
	}
	else
	{
		SaveOTPData();
		m_pInterface->AddLog(_T("This is Dummy Burn!"),COLOR_RED);
	}
	return TRUE;
}

void COtpTYD8583M::SaveOTPData()
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

BOOL COtpTYD8583M::WriteOTP(vector<double>& vData)
{
	TYD8583M_GetBasicData();
	TYD8583M_GetAFData();
	CreateBurningData();
	if (!m_dummyburn)
	{
		if (!TYD8583M_OtpWrite() && m_sensorotpisempty == FALSE)//
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

void COtpTYD8583M::SaveOTPData(vector<double>& vData)
{
	m_pInterface->AddLog(_T("Read OTP Start!"));
	///////////////////////////////Read OTP////////////////////////////////////
	int valSize = SIZE_TOTAL_GROUP;
	BYTE* pVal = new BYTE[valSize];

	memset(pVal, 0x00, valSize*sizeof(BYTE));

	int i = 0;

	pVal[0] = Data_Write.data_Basic[0];
	for (i=0; i<SIZE_BASIC_GROUP; i++)
	{
		pVal[i+1+(m_LscGroup-1)*OTPGAP_BASIC] = Data_Write.data_Basic[i+1];
	}
	pVal[52] = Data_Write.data_Lenc[0];
	for (i=0; i<SIZE_LSC_GROUP; i++)
	{
		pVal[i+1+(m_LscGroup-1)*OTPGAP_LSC+52] = Data_Write.data_Lenc[i+1];
	}
	pVal[2654] = Data_Write.data_WB[0];
	for (i=0; i<SIZE_WB_GROUP; i++)
	{
		pVal[i+1+(m_LscGroup-1)*OTPGAP_AWB+2654] = Data_Write.data_WB[i+1];
	}
	pVal[2815] = Data_Write.data_AF[0];
	for (i=0; i<SIZE_AF_GROUP; i++)
	{
		pVal[i+1+(m_LscGroup-1)*OTPGAP_AF+2815] = Data_Write.data_AF[i+1];
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
		num.Format("%04d\t0x%04x,0x%02x\n", i,0x0501+i,(unsigned char)pVal[i]);
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

void COtpTYD8583M::CreateBurningData(void)
{
	int i = 0;
	BYTE ValidFlag = 0;
	if (m_LscGroup == 1)
	{
		ValidFlag = FLAG_GROUP1;
	}
	else if (m_LscGroup == 2)
	{
		ValidFlag = FLAG_GROUP2;
	}
	else if (m_LscGroup == 3)
	{
		ValidFlag = FLAG_GROUP3;
	}
	//////////////////////////////////Basic//////////////////////////////////////
	Data_Write.data_Basic[0] = ValidFlag;
	for (i = 0; i < SIZE_BASIC_GROUP-1; i++)
	{
		Data_Write.data_Basic[i+1] = OTP_Data_Write.BaseInfo[i];
	}
	//////////////////////////////////AWB//////////////////////////////////////
	Data_Write.data_WB[0] = ValidFlag;
	for (i = 0; i < SIZE_WB_GROUP-1; i++)
	{
		Data_Write.data_WB[i+1] = OTP_Data_Write.AWBInfo[i];
	}
	//////////////////////////////////LSC//////////////////////////////////////
	Data_Write.data_Lenc[0] = ValidFlag;
	if (m_LSCItem != 2 || m_sensoritem == 3)
	{
		if (m_LSCItem == 0)
		{
			for (i = 0; i < SIZE_LSC_GROUP-1; i++)
			{
				Data_Write.data_Lenc[i+1] = TYD8583M_LSCWriteData[i] = m_mtklscdata[i];
			}
		}
		else	if (m_LSCItem == 1)
		{
			for (i = 0; i < SIZE_LSC_GROUP-1; i++)
			{
				Data_Write.data_Lenc[i+1] = TYD8583M_LSCWriteData[i] = m_quallscdata[i];
			}
		}
		else if (m_sensoritem == 3)
		{
			for (i=0; i<SIZE_LSC_GROUP-1; i++)
			{
				Data_Write.data_Lenc[i+1] = TYD8583M_LSCWriteData[i] = Hi1332_LSCData[i];
			}

		}
	}
	///////////////////////////////////AF/////////////////////////////////////
	Data_Write.data_AF[0] = ValidFlag;
	for (i = 0; i < SIZE_AF_GROUP-2; i++)
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
	}
	//////////////////////////////////////////////////////////////////////////
	TYD8583M_CalcAllCheckSum();
	return;
}

BOOL COtpTYD8583M::CheckOTP()
{
	if (!TYD8583M_OTPVerify())		return FALSE;

	return TRUE;
}

BOOL COtpTYD8583M::VerifyLSC()
{
	if (m_LSCItem == 2)
	{
		if (!TYD8583M_VerifyLSC())		return FALSE; //for sensor LSC
	}
	return TRUE;
}

BOOL COtpTYD8583M::VerifyPreWB()
{
	if (m_EnAWB)
	{
		if (!GetOTPRGBG())		return FALSE;
		HI1332_ApplyAWB(m_wbCurrent,m_goldenrg,m_goldenbg);
	}
	return TRUE;
}

BOOL COtpTYD8583M::VerifyAWB()
{
	if (m_EnAWB)
	{
		if (!TYD8583M_VerifyAWB())		return FALSE;
		if (m_SaveAWB)
		{
			SaveAwbData(&m_wbCurrent,2);
		}
	}
	return TRUE;
}

// BOOL COtpTYD8583M::EndProgram()
// {
// 	return TRUE;
// }


BOOL COtpTYD8583M::GetAWB()
{
	if (m_EnAWB)
	{
		if (!TYD8583M_GetAWBData(OTP_Data_Write))
		{
			return FALSE;
		}
		if (m_SaveAWB)
		{
			SaveAwbData(&m_wbCurrent, 0);
		}
	}
	return TRUE;
}

BOOL COtpTYD8583M::TYD8583M_GetAWBData(TYD8583MOTPDataSpec &awbdata)
{
	m_pInterface ->AddLog(_T("Get AWB Data Start"));

	if (!TYD8583M_GetCurrentAWB(&m_wbCurrent))		return FALSE;

	awbdata.AWBInfo[0] = (m_wbCurrent.rg>>8)&0x0f;
	awbdata.AWBInfo[1] = m_wbCurrent.rg&0xff;
	awbdata.AWBInfo[2] = (m_wbCurrent.bg>>8)&0x0f;
	awbdata.AWBInfo[3] = m_wbCurrent.bg&0xff;
	awbdata.AWBInfo[4] = (m_wbCurrent.gb_gr>>8)&0x0f;
	awbdata.AWBInfo[5] = m_wbCurrent.gb_gr&0xff;

	awbdata.AWBInfo[6] = (m_goldenrg>>8)&0x0f;
	awbdata.AWBInfo[7] = m_goldenrg&0xff;
	awbdata.AWBInfo[8] = (m_goldenbg>>8)&0x0f; 
	awbdata.AWBInfo[9] = m_goldenbg&0xff;
	awbdata.AWBInfo[10] = (m_goldengg>>8)&0x0f;
	awbdata.AWBInfo[11] = 	m_goldengg&0xff;

	awbdata.AWBInfo[12] = (m_wbCurrent.r>>8)&0x0f;
	awbdata.AWBInfo[13] = 	m_wbCurrent.r&0xff;
	awbdata.AWBInfo[14] = (m_wbCurrent.b>>8)&0x0f;
	awbdata.AWBInfo[15] = 	m_wbCurrent.b&0xff;
	awbdata.AWBInfo[16] = (m_wbCurrent.gr>>8)&0x0f;
	awbdata.AWBInfo[17] = 	m_wbCurrent.gr&0xff;
	awbdata.AWBInfo[18] = (m_wbCurrent.gb>>8)&0x0f;
	awbdata.AWBInfo[19] = 	m_wbCurrent.gb&0xff;

	awbdata.AWBInfo[20] = (m_goldenr>>8)&0x0f;
	awbdata.AWBInfo[21] = m_goldenr&0xff;
	awbdata.AWBInfo[22] = (m_goldenb>>8)&0x0f;
	awbdata.AWBInfo[23] = 	m_goldenb&0xff;
	awbdata.AWBInfo[24] = (m_goldengr>>8)&0x0f;
	awbdata.AWBInfo[25] = 	m_goldengr&0xff;
	awbdata.AWBInfo[26] = (m_goldengb>>8)&0x0f;
	awbdata.AWBInfo[27] = m_goldengb&0xff;

	for (int i = 0; i < RESERVERD_AWB; i++)
	{
		awbdata.AWBInfo[28+i] = RESERVERD_DATA;
	}

	m_pInterface ->AddLog(_T("Get AWB Data End"));
	HI1332_ApplyAWB(m_wbCurrent, m_goldenrg, m_goldenbg);
	return TRUE;
}

BOOL COtpTYD8583M::CheckOTPAWB()
{
	if (m_EnAWB)
	{
		m_pInterface ->AddLog(_T("Check Apply AWB Data Start"));
		if (!GetAWBInfo(&m_wbCurrent))		return FALSE;

		if (!TYD8583M_CheckAWB(m_wbCurrent,1))				return FALSE;
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

		if (m_SaveAWB)
		{
			SaveAwbData(&m_wbCurrent, 1);
		}
		m_pInterface ->AddLog(_T("Check Apply AWB Data Pass!"),COLOR_BLUE);
	}

	return TRUE;
}

BOOL COtpTYD8583M::TYD8583M_GetCurrentAWB(tagAwbRatio* wb)
{
	if (!InitAWB())		return FALSE;

	if (!GetAWBInfo(&m_wbCurrent))		return FALSE;

	if (!TYD8583M_CheckAWB(m_wbCurrent,0))				return FALSE;

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

BOOL COtpTYD8583M::GetAWBInfo(tagAwbRatio* wb)
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
	wb->rg		= (UINT)(1.0 * (wb->r-SENSOROB) / (wb->gr-SENSOROB)*AWB_FACTOR + 0.5); 
	wb->bg		= (UINT)(1.0 * (wb->b-SENSOROB) / (wb->gb-SENSOROB)*AWB_FACTOR + 0.5);
	wb->gb_gr	= (UINT)(1.0 * (wb->gb-SENSOROB)/ (wb->gr-SENSOROB)*AWB_FACTOR+ 0.5);

	//raw8
	wb->r = (UINT)(wb->r/4 + 0.5);
	wb->g = (UINT)(wb->g/4 + 0.5);
	wb->b = (UINT)(wb->b/4 + 0.5);
	wb->gb = (UINT)(wb->gb/4 + 0.5);
	wb->gr = (UINT)(wb->gr/4 + 0.5);

	m_szStr.Format(_T("R/G: %d B/G: %d"), wb->rg, wb->bg);
	m_pInterface->AddLog(m_szStr, COLOR_BLUE);
	return TRUE;
}

BOOL COtpTYD8583M::TYD8583M_CheckAWB(tagAwbRatio wb, int type)
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

void COtpTYD8583M::TYD8583M_GetBasicData(void)
{
//	m_pInterface ->AddLog(_T("Get Basic Data Start"));
	CTime time = CTime::GetCurrentTime();

	OTP_Data_Write.BaseInfo[0] = BASIC_MID;
	OTP_Data_Write.BaseInfo[1] = BASIC_LENSID;
	OTP_Data_Write.BaseInfo[2] = time.GetYear() - 2000;
	OTP_Data_Write.BaseInfo[3] = time.GetMonth();
	OTP_Data_Write.BaseInfo[4] = time.GetDay();
	OTP_Data_Write.BaseInfo[5] = BASIC_SENSOR;

	for (int i=0; i<RESERVERD_BASIC; i++)
	{
		OTP_Data_Write.BaseInfo[6+i] = RESERVERD_DATA;
	}

	info.Format(_T("MID = 0x%02x"),BASIC_MID);
	m_pInterface->AddLog(info);
	info.Format(_T("LensID = 0x%02x"),BASIC_LENSID);
	m_pInterface->AddLog(info);
// 	info.Format(_T("VCMID = 0x%02x"),BASIC_VCMID);
// 	m_pInterface->AddLog(info);
// 	info.Format(_T("DriverICID = 0x%02x"),BASIC_DRIVERICID);
// 	m_pInterface->AddLog(info);
// 	info.Format(_T("IRID =0x%02x"),BASIC_IRID);
// 	m_pInterface->AddLog(info);

	m_pInterface ->AddLog(_T("Get Basic Data"));
	return;
}

void COtpTYD8583M::TYD8583M_GetAFData(void)
{
//	m_pInterface ->AddLog(_T("Get AF Data Start"));

	OTP_Data_Write.AFInfo[0] = AF_LENSPIN;
	OTP_Data_Write.AFInfo[1] = AF_DIRECTION;
	OTP_Data_Write.AFInfo[2] = (m_infCode>>8)&0x0f;
	OTP_Data_Write.AFInfo[3] = m_infCode&0xff;
	OTP_Data_Write.AFInfo[4] = (m_macCode>>8)&0x0f;
	OTP_Data_Write.AFInfo[5] = m_macCode&0xff;

	m_pInterface ->AddLog(_T("Get AF Data"));
	return;
}
void COtpTYD8583M::TYD8583M_CalcAllCheckSum(void)
{
	int i = 0;
	int sum = 0;

	//Basic
	if (m_EnBasic)
	{
		sum = SumCalc(OTP_Data_Write.BaseInfo,SIZE_BASIC_GROUP-1,255,1);
		Data_Write.data_Basic[SIZE_BASIC_GROUP] = sum;
	}

	//AWB
	if (m_EnAWB)
	{
		sum = SumCalc(OTP_Data_Write.AWBInfo,SIZE_WB_GROUP-1,255,1);
		Data_Write.data_WB[SIZE_WB_GROUP] = sum;
	}

	// LSC
	if (m_EnLSC)
	{
		if (m_LSCItem != 2 || m_sensoritem == 3)
		{
			sum = SumCalc(TYD8583M_LSCWriteData, SIZE_LSC_GROUP-1, 255,1);
			Data_Write.data_Lenc[SIZE_LSC_GROUP] = sum;
		}
	}

	//AF	
	if (m_EnAF)
	{
		sum = SumCalc(OTP_Data_Write.AFInfo,SIZE_AF_GROUP-2,255,1);
		Data_Write.data_AF[SIZE_AF_GROUP-1] = sum>>8;
		Data_Write.data_AF[SIZE_AF_GROUP] = sum&0xff;
	}

	if (m_EnPDAF)
	{
		//PDAF1
		sum = SumCalc(OTP_Data_Write.PDAF1,SIZE_PDAF_PROC1,255,1);
		proc1sum = sum;
		Data_Write.pdaf_proc1checksum = proc1sum;
	}

	return;
}

BOOL COtpTYD8583M::CheckSensorOTPEmpty()
{
	HI1332InitOTP();
	if (m_EnBasic)
	{
		BYTE ReadData[SIZE_BASIC_GROUP] = {0};
		HI1332ContinuousRead(ReadData, m_addrFlag.moduleInfo+1+(m_LscGroup-1)*OTPGAP_BASIC, m_addrSum.moduleInfo+(m_LscGroup-1)*OTPGAP_BASIC);

		if (ReadData[0] != 0 || ReadData[1] != 0 || ReadData[2] != 0 ||ReadData[3] != 0 || ReadData[4] != 0)
		{
			m_pInterface->AddLog(_T("当前组内OTP数据不为空! 开始检查OTP内数据"),COLOR_RED);
			m_sensorotpisempty = TRUE;
			return FALSE;
		}
	}

	if (m_EnLSC)
	{
		BYTE ReadData[SIZE_LSC_GROUP] = {0};
		HI1332ContinuousRead(ReadData, m_addrFlag.lsc+1+(m_LscGroup-1)*OTPGAP_LSC, m_addrSum.lsc+(m_LscGroup-1)*OTPGAP_LSC);

		if (ReadData[0] != 0 || ReadData[1] != 0 || ReadData[2] != 0 ||ReadData[3] != 0 || ReadData[4] != 0)
		{
			m_pInterface->AddLog(_T("当前组内OTP数据不为空! 开始检查OTP内数据"),COLOR_RED);
			m_sensorotpisempty = TRUE;
			return FALSE;
		}
	}

	if (m_EnAWB)
	{
		BYTE ReadData[SIZE_WB_GROUP] = {0};
		HI1332ContinuousRead(ReadData, m_addrFlag.awb+1+(m_LscGroup-1)*OTPGAP_AWB, m_addrSum.awb+(m_LscGroup-1)*OTPGAP_AWB);

		if (ReadData[0] != 0 || ReadData[1] != 0 || ReadData[2] != 0 ||ReadData[3] != 0 || ReadData[4] != 0)
		{
			m_pInterface->AddLog(_T("当前组内OTP数据不为空! 开始检查OTP内数据"),COLOR_RED);
			m_sensorotpisempty = TRUE;
			return FALSE;
		}
	}

	if (m_EnAF)
	{
		BYTE ReadData[SIZE_AF_GROUP] = {0};
		HI1332ContinuousRead(ReadData, m_addrFlag.af+1+(m_LscGroup-1)*OTPGAP_AF, m_addrSum.af+1+(m_LscGroup-1)*OTPGAP_AF);

		if (ReadData[0] != 0 || ReadData[1] != 0 || ReadData[2] != 0 ||ReadData[3] != 0 || ReadData[4] != 0)
		{
			m_pInterface->AddLog(_T("当前组内OTP数据不为空! 开始检查OTP内数据"),COLOR_RED);
			m_sensorotpisempty = TRUE;
			return FALSE;
		}
	}
	HI1332ReleaseOTP();
 	return TRUE;
}

BOOL COtpTYD8583M::TYD8583M_OtpWrite(void)
{

	if (m_LSCItem == 2)
	{
		if (m_sensoritem == 3)
		{
			if (!CheckSensorOTPEmpty())		return FALSE;
		}
	}

	m_pInterface->AddLog(_T("OTP Burning Start!"));

	if (m_EnBasic)
	{
		if (!ProgrammingBasic())  return FALSE;
	}

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

BOOL COtpTYD8583M::ProgrammingBasic()
{
	if (m_LSCItem == 0)
	{
		if (FM24C64_Programming(m_addrFlag.moduleInfo,Data_Write.data_Basic,sizeof(Data_Write.data_Basic)) != TRUE)
		{
			m_pInterface->AddLog(_T("Basic Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
	}
	else if (m_LSCItem == 2)
	{
		if (m_sensoritem == 3)
		{
			if (m_LscGroup>m_grouplimit)
			{
				info.Format(_T("OTP只允许烧录前%d组！"),m_grouplimit);
				m_pInterface->AddLog(info,COLOR_RED);		
				return FALSE;
			}
			HI1332InitOTP();
			Programming_HI1332Basic();
		}
	}

	msleep(10);

	return TRUE;
}

void COtpTYD8583M::Programming_HI1332Basic()
{
	HI1332SingleWrite(&Data_Write.data_Basic[0],m_addrFlag.moduleInfo, m_addrFlag.moduleInfo);
	HI1332ContinuousWrite(Data_Write.data_Basic+1, m_addrFlag.moduleInfo+1+(m_LscGroup-1)*OTPGAP_BASIC,m_addrSum.moduleInfo+(m_LscGroup-1)*OTPGAP_BASIC);
	return;
}

BOOL COtpTYD8583M::ProgrammingAWB()
{
	if (m_LSCItem != 2)
	{
		if (FM24C64_Programming(m_addrFlag.awb,Data_Write.data_WB,sizeof(Data_Write.data_WB)) != TRUE)
		{
			m_pInterface->AddLog(_T("AWB Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
		msleep(10);
	}
	else
	{
		if (m_sensoritem == 3)
		{
			Programming_HI1332AWB();
		}
	}

	return TRUE;
}

void COtpTYD8583M::Programming_HI1332AWB()
{
	HI1332SingleWrite(&Data_Write.data_WB[0],m_addrFlag.awb, m_addrFlag.awb);
	HI1332ContinuousWrite(Data_Write.data_WB+1, m_addrFlag.awb+1+(m_LscGroup-1)*OTPGAP_AWB,m_addrSum.awb+(m_LscGroup-1)*OTPGAP_AWB);
	return;
}

BOOL COtpTYD8583M::ProgrammingLSC()
{
	if (m_LSCItem != 2)
	{
		if (FM24C64_Programming(m_addrFlag.lsc,Data_Write.data_Lenc,sizeof(Data_Write.data_Lenc)) != TRUE)
		{
			m_pInterface->AddLog(_T("LSC Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
		msleep(10);
	}
	else
	{
		if (m_sensoritem == 3)
		{
			Programming_HI1332LSC();
		}
	}

	return TRUE;
}

BOOL COtpTYD8583M::Programming_HI1332LSC()
{
	HI1332SingleWrite(&Data_Write.data_Lenc[0],m_addrFlag.lsc, m_addrFlag.lsc);
	HI1332LargeWrite(Data_Write.data_Lenc+1, m_addrFlag.lsc+1+(m_LscGroup-1)*OTPGAP_LSC,m_addrSum.lsc+(m_LscGroup-1)*OTPGAP_LSC);
	return TRUE;
}

BOOL COtpTYD8583M::ProgrammingAF()
{
	if (m_LSCItem != 2)
	{
		if (FM24C64_Programming(m_addrFlag.af,Data_Write.data_AF,sizeof(Data_Write.data_AF)) != TRUE)
		{
			m_pInterface->AddLog(_T("AF Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
		msleep(10);
	}
	else
	{
		if (m_sensoritem == 3)
		{
			Programming_HI1332AF();
		}
	}

	return TRUE;
}

void COtpTYD8583M::Programming_HI1332AF()
{
	HI1332SingleWrite(&Data_Write.data_AF[0],m_addrFlag.af, m_addrFlag.af);
	HI1332ContinuousWrite(Data_Write.data_AF+1, m_addrFlag.af+1+(m_LscGroup-1)*OTPGAP_AF,m_addrSum.af+1+(m_LscGroup-1)*OTPGAP_AF);
	HI1332ReleaseOTP();
	return ;
}

BOOL COtpTYD8583M::ProgrammingPDAF()
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
			if (FM24C64_Programming(m_addrFlag.pdaf, flag, 1 ) != TRUE)		
			{		
				m_pInterface->AddLog(_T("PDAF Flag Write Fail!"),COLOR_RED);	
				bRet &= FALSE;	
				break;	
			}		
			else		
			{		
				if (FM24C64_Programming(m_addrFlag.pdaf+1, OTP_Data_Write.PDAF1, sizeof(OTP_Data_Write.PDAF1)) != TRUE)	
				{	
					m_pInterface->AddLog(_T("PDAF Proc1 Data Write Fail!"),COLOR_RED);
					bRet &= FALSE;
					break;
				}	

				if (FM24C64_Programming(m_addrSum.pdaf, sum1, 1) != TRUE)	
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

BOOL COtpTYD8583M::TYD8583M_OTPVerify(void)
{
	if (!m_dummyburn)//!m_dummyburn
	{
		if (!m_sensorotpisempty)
		{
			if (!TYD8583M_CheckOTPReadWrite())		return FALSE;
		}
		else
		{
			if (!TYD8583M_CheckOTP()) return FALSE;
		}
	}
	else 
	{
		if (m_LSCItem == 2)
		{
			if (m_sensoritem == 3)
			{
			}
		}
		m_pInterface->AddLog(_T("This is Dummy Burn!"),COLOR_RED);
	}
   return TRUE;
}

BOOL  COtpTYD8583M::TYD8583M_CheckOTP()
{
	m_pInterface->AddLog(_T("Check OTP Data Start!"));
	BYTE ReadData = 0;
	HI1332InitOTP();
	if (m_EnBasic)
	{
		HI1332SingleRead(&ReadData, m_addrFlag.moduleInfo, m_addrFlag.moduleInfo);
		int BasicReadFlag = HI1332_CheckOTPFlag(ReadData);
		if (BasicReadFlag == -1) return FALSE;
		BYTE BasicReadData[SIZE_BASIC_GROUP] = {0};
		HI1332ContinuousRead(BasicReadData, m_addrFlag.moduleInfo+1+(BasicReadFlag-1)*OTPGAP_BASIC,m_addrSum.moduleInfo+(BasicReadFlag-1)*OTPGAP_BASIC);
		int sum = SumCalc(BasicReadData,SIZE_BASIC_GROUP-1,255,1);
		if (sum != BasicReadData[SIZE_BASIC_GROUP-1])
		{
			m_pInterface->AddLog(_T("Basic CheckSum异常"),COLOR_RED);
	HI1332ReleaseOTP();
			return FALSE;
		}
		m_pInterface->AddLog(_T("Check Basic Data Pass!"));
	}

	if (m_EnLSC)
	{
		HI1332SingleRead(&ReadData, m_addrFlag.lsc, m_addrFlag.lsc);
		int LSCReadFlag = HI1332_CheckOTPFlag(ReadData);
		if (LSCReadFlag == -1) return FALSE;
		BYTE LSCReadData[SIZE_LSC_GROUP] = {0};
		HI1332ContinuousRead(LSCReadData, m_addrFlag.lsc+1+(LSCReadFlag-1)*OTPGAP_LSC,m_addrSum.lsc+(LSCReadFlag-1)*OTPGAP_LSC);
		int sum = SumCalc(LSCReadData,SIZE_LSC_GROUP-1,255,1);
		if (sum != LSCReadData[SIZE_LSC_GROUP-1])
		{
			m_pInterface->AddLog(_T("LSC CheckSum异常"),COLOR_RED);
	HI1332ReleaseOTP();
			return FALSE;
		}
		m_pInterface->AddLog(_T("Check LSC Data Pass!"));
	}

	if (m_EnAWB)
	{
		HI1332SingleRead(&ReadData, m_addrFlag.awb, m_addrFlag.awb);
		int AWBReadFlag = HI1332_CheckOTPFlag(ReadData);
		if (AWBReadFlag == -1) return FALSE;
		BYTE AWBReadData[SIZE_WB_GROUP] = {0};
		HI1332ContinuousRead(AWBReadData, m_addrFlag.awb+1+(AWBReadFlag-1)*OTPGAP_AWB,m_addrSum.awb+(AWBReadFlag-1)*OTPGAP_AWB);
		int sum = SumCalc(AWBReadData,SIZE_WB_GROUP-1,255,1);
		if (sum != AWBReadData[SIZE_WB_GROUP-1])
		{
			m_pInterface->AddLog(_T("AWB CheckSum异常"),COLOR_RED);
	HI1332ReleaseOTP();
			return FALSE;
		}
		m_pInterface->AddLog(_T("Check AWB Data Pass!"));
	}

	if (m_EnAF)
	{
		HI1332SingleRead(&ReadData, m_addrFlag.af, m_addrFlag.af);
		int AFReadFlag = HI1332_CheckOTPFlag(ReadData);
		if (AFReadFlag == -1) return FALSE;
		BYTE AFReadData[SIZE_AF_GROUP] = {0};
		HI1332ContinuousRead(AFReadData, m_addrFlag.af+1+(AFReadFlag-1)*OTPGAP_AF,m_addrSum.af+1+(AFReadFlag-1)*OTPGAP_AF);
		int sum = SumCalc(AFReadData,SIZE_AF_GROUP-1,255,1);
		if (sum != AFReadData[SIZE_AF_GROUP-1])
		{
			m_pInterface->AddLog(_T("AF CheckSum异常"),COLOR_RED);
			HI1332ReleaseOTP();
			return FALSE;
		}
		m_pInterface->AddLog(_T("Check AF Data Pass!"));
	}
	HI1332ReleaseOTP();
	return TRUE;
}

int COtpTYD8583M::HI1332_CheckOTPFlag(BYTE ReadFlag)
{
	if (ReadFlag == 0)
	{
		m_pInterface->AddLog(_T("OTP内Flag为空"),COLOR_RED);
		return -1;
	}
	else if (ReadFlag == 1)
	{
		m_pInterface->AddLog(_T("Flag为第一组有效"));
		return 1;
	}
	else if (ReadFlag == 0x13)
	{
		m_pInterface->AddLog(_T("Flag为第二组有效"));
		return 2;
	}
	else if (ReadFlag == 0x37)
	{
		m_pInterface->AddLog(_T("Flag为第三组有效"));
		return 3;
	}
	else
	{
		m_pInterface->AddLog(_T("Flag异常"),COLOR_RED);
		return -1;
	}
}

BOOL COtpTYD8583M::TYD8583M_CheckOTPReadWrite(void)
{
	m_pInterface->AddLog(_T("Compare Read/Write Data Start!"));

	if (m_LSCItem == 0)
	{	
		if (m_EnBasic)
		{
			if (FM24C64_Read(m_addrFlag.moduleInfo, Data_Read.data_Basic,sizeof(Data_Read.data_Basic)) != TRUE)
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
		}


		if (m_EnAWB)
		{	
			if (FM24C64_Read(m_addrFlag.awb, Data_Read.data_WB,sizeof(Data_Read.data_WB)) != TRUE)
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
			if (FM24C64_Read(m_addrFlag.lsc, Data_Read.data_Lenc,sizeof(Data_Read.data_Lenc)) != TRUE)
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
			if (FM24C64_Read(m_addrFlag.af, Data_Read.data_AF,sizeof(Data_Read.data_AF)) != TRUE)
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
			FM24C64_Read(m_addrSum.pdaf, Read_Sum, 1);
			BYTE *Write_Sum = new BYTE[1];
			Write_Sum[0] = (BYTE)proc1sum;

			/////////////////////////////////PDAF//////////////////////////////////////
			do 
			{
				if (FM24C64_Read(m_addrFlag.pdaf, read_flag, 1 ) != TRUE)
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

					if (FM24C64_Read(m_addrFlag.pdaf+1, OTP_Data_Read.PDAF1, sizeof (OTP_Data_Read.PDAF1)) != TRUE)
					{
						m_pInterface->AddLog(_T("Read PDAF Data Fail!"),COLOR_RED);
						result&= FALSE;
						break;
					}

					if (!CheckEqual(OTP_Data_Write.PDAF1, OTP_Data_Read.PDAF1,sizeof(OTP_Data_Write.PDAF1)))
					{
						m_pInterface->AddLog(_T("Check PDAF Data Fail!"),COLOR_RED);
						result&= FALSE;
						break;
					}
				}
				////////////////////////////////////Sum//////////////////////////////////
				if (!CheckEqual(Read_Sum, Write_Sum,1)) 
				{
					m_pInterface->AddLog(_T("Check PDAF Flag Data Fail!"),COLOR_RED);
					result&= FALSE;
					break;
				}		

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
		if (m_sensoritem == 3)
		{
			HI1332InitOTP();
			if (m_EnBasic)
			{
				HI1332SingleRead(Data_Read.data_Basic, m_addrFlag.moduleInfo,m_addrFlag.moduleInfo);
				HI1332ContinuousRead(Data_Read.data_Basic+1, m_addrFlag.moduleInfo+1+(m_LscGroup-1)*OTPGAP_BASIC,  m_addrSum.moduleInfo+(m_LscGroup-1)*OTPGAP_BASIC);
				if (!CheckEqual(Data_Write.data_Basic,Data_Read.data_Basic,sizeof(Data_Write.data_Basic)))
				{
					m_pInterface->AddLog(_T("Check Basic Data Fail!"),COLOR_RED);
					HI1332ReleaseOTP();
					return FALSE;
				}
			}

			if (m_EnAWB)
			{	
				HI1332SingleRead(Data_Read.data_WB, m_addrFlag.awb,m_addrFlag.awb);
				HI1332ContinuousRead(Data_Read.data_WB+1, m_addrFlag.awb+1+(m_LscGroup-1)*OTPGAP_AWB,  m_addrSum.awb+(m_LscGroup-1)*OTPGAP_AWB);
				if (!CheckEqual(Data_Write.data_WB,Data_Read.data_WB,sizeof(Data_Write.data_WB)))
				{
					m_pInterface->AddLog(_T("Check AWB Data Fail!"),COLOR_RED);
					HI1332ReleaseOTP();
					return FALSE;
				}
			}

			if (m_EnLSC)
			{	
				HI1332SingleRead(Data_Read.data_Lenc, m_addrFlag.lsc,m_addrFlag.lsc);
				HI1332ContinuousRead(Data_Read.data_Lenc+1, m_addrFlag.lsc+1+(m_LscGroup-1)*OTPGAP_LSC,  m_addrSum.lsc+(m_LscGroup-1)*OTPGAP_LSC);
				if (!CheckEqual(Data_Write.data_Lenc,Data_Read.data_Lenc,sizeof(Data_Write.data_Lenc)))
				{
					m_pInterface->AddLog(_T("Check LSC Data Fail!"),COLOR_RED);
					HI1332ReleaseOTP();
					return FALSE;
				}
			}

			if (m_EnAF)
			{
				HI1332SingleRead(Data_Read.data_AF, m_addrFlag.af,m_addrFlag.af);
				HI1332ContinuousRead(Data_Read.data_AF+1, m_addrFlag.af+1+(m_LscGroup-1)*OTPGAP_AF,  m_addrSum.af+1+(m_LscGroup-1)*OTPGAP_AF);
				if (!CheckEqual(Data_Write.data_AF,Data_Read.data_AF,sizeof(Data_Write.data_AF)))
				{
					m_pInterface->AddLog(_T("Check AF Data Fail!"),COLOR_RED);
					HI1332ReleaseOTP();
					return FALSE;
				}
			}
			HI1332ReleaseOTP();
			m_pInterface->AddLog(_T("Check OTP Data Success!"),COLOR_BLUE);	
		}
	}

	return TRUE;
}

BOOL COtpTYD8583M::GetOTPRGBG()
{
	if(!m_sensorotpisempty)
	{
		m_wbCurrent.rg = Data_Write.data_WB[1]<<8|Data_Write.data_WB[2];
		m_wbCurrent.bg = Data_Write.data_WB[3]<<8|Data_Write.data_WB[4];
	}
	else
	{
		if(m_sensoritem == 3)
		{
			BYTE ReadData = 0;
			HI1332InitOTP();
			HI1332SingleRead(&ReadData, m_addrFlag.awb, m_addrFlag.awb);

			int ReadAWBFlag = HI1332_CheckOTPFlag(ReadData);
			BYTE ReadAWBData[SIZE_WB_GROUP] = {0};
			HI1332ContinuousRead(ReadAWBData, m_addrFlag.awb+1+(ReadAWBFlag-1)*OTPGAP_AWB, m_addrSum.awb+(ReadAWBFlag-1)*OTPGAP_AWB);
			m_wbCurrent.rg = ReadAWBData[0]<<8|ReadAWBData[1];
			m_wbCurrent.bg = ReadAWBData[2]<<8|ReadAWBData[3];
			HI1332ReleaseOTP();
			return TRUE;
		}
		else
		{
			m_pInterface->AddLog(_T("未知异常!"),COLOR_RED);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL COtpTYD8583M::TYD8583M_VerifyLSC()
{
	m_pInterface->AddLog(_T("Verify LSC:"));
	if (m_sensoritem == 3)
	{
		BYTE i=0,j=0,k=0;
		i2c_read_byte(0x021c,&j);

		i = j&0x20;
		k = j&0x10;
		if((i != 0)||( k!=0))
		{
			m_pInterface->AddLog(_T("MCU checkSum or Flag NG"),COLOR_RED);
			return FALSE;
		}

		i2c_write(0x0A04, 0x015e);
		msleep(200);
		m_pInterface->AddLog(_T("AutoLoading LSC OK!"),COLOR_BLUE);
	}

	return TRUE;
}

BOOL COtpTYD8583M::TYD8583M_VerifyAWB()
{
	if (!GetAWBInfo(&m_wbCurrent))		return FALSE;
	if (!TYD8583M_CheckAWB(m_wbCurrent,2))		return FALSE;
	return TRUE;
}