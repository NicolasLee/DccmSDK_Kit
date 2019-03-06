#include "stdafx.h"
#include "Otp_HLTE1M.h"
#include "..\..\OTP_SensorSetting.h"

COtpHLTE1M::COtpHLTE1M(void)
{
	ZeroMemory(OTP_Data_Write.AFInfo,sizeof(OTP_Data_Write.AFInfo));
	ZeroMemory(OTP_Data_Write.BaseInfo,sizeof(OTP_Data_Write.BaseInfo));
	ZeroMemory(S5K4H8_LSCWriteData,sizeof(S5K4H8_LSCWriteData));
	ZeroMemory(OTP_Data_Write.AWBInfo,sizeof(OTP_Data_Write.AWBInfo));
	ZeroMemory(OTP_Data_Write.PDAF1,sizeof(OTP_Data_Write.PDAF1));

	m_flagEmpty = FLAG_EMPTY;
	m_flagValid = FLAG_VALID;

	m_addrFlag.moduleInfo	= REG_BASIC_FLAG;
	m_addrFlag.awb				= REG_WB_FLAG;
	m_addrFlag.lsc					= REG_LSC_FLAG;
	m_addrFlag.af					= REG_AF_FLAG;
	m_addrFlag.pdaf			    = REG_PDAF_FLAG;
	m_addrFlag.pdaf1          = REG_PDAF_PROC1_FLAG;
	m_addrFlag.pdaf2				= REG_PDAF_PROC2_FLAG; 

	m_addrSum.moduleInfo	= REG_BASIC_CHECKSUM;
	m_addrSum.awb				= REG_WB_CHECKSUM;
	m_addrSum.lsc					= REG_LSC_CHECKSUM;
	m_addrSum.af					= REG_AF_CHECKSUM;
	m_addrSum.pdaf				= REG_PDAF_SUMADDR;
	m_addrSum.pdaf1				= REG_PDAF_PROC1_SUMADDR;
	m_addrSum.pdaf2			= REG_PDAF_PROC2_SUMADDR;

	SetOtpName(_T("HLTE1M"));

//	PreSubclassOtp();
}


COtpHLTE1M::~COtpHLTE1M(void)
{

}

void COtpHLTE1M::PreSubclassOtp()
{
	tagOtpCtrl* control = GetControlParams();
// 	control->manualReInit = FALSE;
// 	control->enProgram = TRUE;
// 	control->enForceCalibrate = FALSE;
// 	control->enSaveRawBMP = FALSE;
// 	control->enSaveCorrectBMP = FALSE;
// 	control->enShowVideoAfterOtp = FALSE;
	control->sensorob = SENSOROB;
	control->quallsccompress = QUALLSCCOMPRESS;

	OtpIDStruct *ID = GetIDGroup();	
	ID->house = BASIC_MID;
	ID->aftype = BASIC_AFFLAG;
	ID->sensor = BASIC_SENSOR;
	ID->lens = BASIC_LENSID;
	ID->vcm = BASIC_VCMID;
	ID->driverIC = BASIC_DRIVERICID;
		ID->FNO = BASIC_FNO;

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
	wbCtrl->roiCalcMode = m_awb_size;
	wbCtrl->factorAmplify = AWB_FACTOR;

	WBGainRegStruct* wbReg = GetWBGainRegister();
	wbReg->addrGr = GAIN_ADDR_GR;
	wbReg->addrR = GAIN_ADDR_R;
	wbReg->addrB = GAIN_ADDR_B;
	wbReg->addrGb = GAIN_ADDR_GB;
	wbReg->dataDefaultGain = GAIN_DEFAULT_VAL;	
// 	wbReg->addrGrHigh = S5K4H8_ADDR_GAIN_GR_H;
// 	wbReg->addrGrLow  = S5K4H8_ADDR_GAIN_GR_L;
// 	wbReg->addrGbHigh = S5K4H8_ADDR_GAIN_GB_H;
// 	wbReg->addrGbLow  = S5K4H8_ADDR_GAIN_GB_L;
// 	wbReg->addrRHigh  = S5K4H8_ADDR_GAIN_R_H;
// 	wbReg->addrRLow   = S5K4H8_ADDR_GAIN_R_L;
// 	wbReg->addrBHigh  = S5K4H8_ADDR_GAIN_B_H;
// 	wbReg->addrBLow   = S5K4H8_ADDR_GAIN_B_L;


	tagAwbRatio *wbGolden = GetWBGoldenValue();		
	wbGolden->rg  = m_goldenrg;
	wbGolden->bg  = m_goldenbg;

	LencOtpControlStruct *lencCtrl = GetLencControlParams();
// 	lencCtrl->inputTarget = S5K4H8_VAL_LSC_TARGET;
// 	lencCtrl->inputLow = S5K4H8_VAL_LSC_LOW;
// 	lencCtrl->inputHigh = S5K4H8_VAL_LSC_HIGH;
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
// 	lencReg->addrCtrl = S5K4H8_ADDR_LSC_CTRL;
// 	lencReg->lencEnable = S5K4H8_ADDR_LSC_EN;
// 	lencReg->lencDisable = S5K4H8_ADDR_LSC_DIS;
//	lencReg->addrDataStart = REG_INVALID;
//	lencReg->dataSize = S5K4H8_OTP_LSC_SIZE;

	ExpControlStruct *expCtrl = GetExpControlParams();
	expCtrl->type = EXP_LINEAR;
	expCtrl->roiCalcMode = ROI_CALC_5;
	expCtrl->filterChannel = EXP_CHANNEL_G;
// 	expCtrl->target = S5K4H8_VAL_EXP_TARGET;
// 	expCtrl->lowLimit = S5K4H8_VAL_EXP_LOW;
// 	expCtrl->highLimit = S5K4H8_VAL_EXP_HIGH;
	expCtrl->effectFrame = 3;
	expCtrl->stableFrame = 3;

	ExpRegisterStruct *expReg = GetExpRegister();
	expReg->mask = 0x00FFFF;
	expReg->addrHigh = REG_INVALID;
// 	expReg->addrMid = S5K4H8_ADDR_EXP_H;
// 	expReg->addrLow = S5K4H8_ADDR_EXP_L;

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

void COtpHLTE1M::SetOtpSensorParams()
{
	OtpSensorStruct* params = GetOtpSensorParams();

	params->name = _T("HLTE1M_MIPI");
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
//	params->otpInit.params = S5K4H8_OTP_Settings;
//	params->otpInit.paramSize = sizeofS5K4H8_OTP_Settings;

	params->verifyInit.width  = 4208;
	params->verifyInit.height = 3120;
	params->verifyInit.format = IMAGE_FORMAT_OTP_RAW10;
	params->verifyInit.sequence = IMAGE_SEQUENCE_GBRG;
	params->verifyInit.mhzMCLK = 24;
// 	params->verifyInit.params = S5K4H8_OTP_Settings;
// 	params->verifyInit.paramSize = sizeofS5K4H8_OTP_Settings;	
}


BOOL COtpHLTE1M::EraseAllData()
{
	if(m_LSCItem == 0)
	{
		int i = 0;
		BYTE* eraseData = new BYTE[32]();
		memset(eraseData, 0xff, 32*sizeof(BYTE));//注意初始EEPROM是00还是ff

		for (i=0; i<110; i++)
		{
			if (!GT24C64_Programming(0x0000+i*32,eraseData,32))//erase all
			{
				m_pInterface->AddLog(_T("OTP数据擦除失败!"),COLOR_RED);
				SAFE_DELETE_ARRAY(eraseData);
				return 0;
			}
		}

		SAFE_DELETE_ARRAY(eraseData);
	}
	else if (m_LSCItem == 1)
	{
		if (!ZC533_All_Erase())	return FALSE;
	}

	m_pInterface->AddLog(_T("OTP所有数据已全部擦除!"));
	return TRUE;
}

void COtpHLTE1M::SetWBDefaultGain(void)
{
	return S5K4H8_SetDefaultAwb();
}

BOOL COtpHLTE1M::WriteOTP()
{
	S5K4H8_GetBasicData();
	S5K4H8_GetAFData();
	CreateBurningData();
	if (!m_dummyburn)
	{
		if (!S5K4H8_OtpWrite())//
		{return FALSE;}
	}
	else
	{
		m_pInterface->AddLog(_T("This is Dummy Burn!"),COLOR_RED);
	}
	return TRUE;
}

void COtpHLTE1M::SaveOTPData()
{
	m_pInterface->AddLog(_T("Read OTP Start!"));
	///////////////////////////////Read OTP////////////////////////////////////
	int valSize = 481;
	BYTE* pVal = new BYTE[valSize]();
	int i = 0;

	if (m_LscGroup == 1)
	{
		for (i=0; i<SIZE_BASIC_GROUP+1; i++)
		{
			pVal[i] = Data_Write.data_Basic[i];
		}
		for (i=0; i<SIZE_WB_GROUP; i++)
		{
			pVal[i+9] = Data_Write.data_WB[i];
		}
		for (i=0; i<SIZE_AF_GROUP+1; i++)
		{
			pVal[i+16] = Data_Write.data_AF[i];
		}
// 		for (i=0; i<2; i++)
// 		{
// 			pVal[i+22] = Data_Write.data_Lenc[i];
// 		}

		for (i=0; i<417; i++)
		{
			pVal[i+64] = S5K4H8_LSCWriteData[i+31];
		}

	}
	else if (m_LscGroup == 2)
	{
// 		for (i=0; i<SIZE_BASIC_GROUP+1; i++)
// 		{
// 			pVal[i+OTPGAP] = Data_Write.data_Basic[i];
// 		}
// 		for (i=0; i<SIZE_WB_GROUP; i++)
// 		{
// 			pVal[i+9+OTPGAP] = Data_Write.data_WB[i];
// 		}
// 		for (i=0; i<SIZE_AF_GROUP+1; i++)
// 		{
// 			pVal[i+16+OTPGAP] = Data_Write.data_AF[i];
// 		}
// 		for (i=0; i<2; i++)
// 		{
// 			pVal[i+22+OTPGAP] = Data_Write.data_Lenc[i];
// 		}

		for (i=0; i<417; i++)
		{
			pVal[i+64] = S5K4H8_LSCWriteData[i+62];
		}       
	}


	//////////////////////////////////////////////////////////////////////////
	CString DirPath = m_pInterface->GetExeDir();
	CString Path ;
	Path.Format(_T("OTP Data_%d.txt"),m_pInterface->GetModuleNum());
	Path = DirPath+Path;
	FILE* file = NULL;

	CStringA temp, num;
	num.Format("Apply WB Gain:\nR:%d\nGr:%d\nGb:%d\nB:%d\nRG:%d\nBG:%d\nGG:%d\n", Applyawbdata[0],Applyawbdata[1],Applyawbdata[2],Applyawbdata[3],Applyawbdata[4],Applyawbdata[5],Applyawbdata[6]);
	temp += num;

// 	vector<int> vshadingdata;
// 
// 	vshadingdata.push_back(ShadingCalc.m_roiCnt.);
// 	vshadingdata.push_back(2);
// 	vshadingdata.push_back(3);
// 	vshadingdata.push_back(4);
// 	vshadingdata.push_back(5);
// 	num.Format("Shading Data After SRAM:\nB1:%d\nB2:%d\nB3:%d\nB4:%d\n",vshadingdata.push_back(1),);
// 	temp += num;

	num.Format("Page: 0x%x\n", 1);
	temp += num;
	for(i=0; i<64; i++)
	{
		num.Format("%04d\t0x%04x,0x%02x\n", i,0x0A04+i,(unsigned char)pVal[i]);
		temp += num;
	}
	num.Format("LSCData:\n");
	temp += num;
	for(i=64; i<417+64; i++)
	{
		num.Format("%04d\t0x%02x\n", i,(unsigned char)pVal[i]);
		temp += num;
	}

// 	int j=0;
// 	int k=0;
// 	for (i = 0; i<valSize; i+=64)
// 	{
// 		if (i == 0)
// 		{
// 			num.Format("Page: 0x%x\n", 1);
// 			temp += num;
// 		}
// 		else if (i%64 == 0)
// 		{
// 			num.Format("Page: 0x%x\n", 0x12+k);
// 			temp += num;
// 			k++;
// 		}
// 
// 		for(j=0; j<64; j++)
// 		{
// 			num.Format("%04d\t0x%04x,0x%02x\n", i+j,0x0A04+j,(unsigned char)pVal[i+j]);
// 			temp += num;
// 		}
// 	}

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

BOOL COtpHLTE1M::WriteOTP(vector<double>& vData)
{
	S5K4H8_GetBasicData();
	S5K4H8_GetAFData();
	CreateBurningData();
	if (!m_dummyburn)
	{
		if (!S5K4H8_OtpWrite() && m_sensorotpisempty ==FALSE)//
		{return FALSE;}
	}
	else
	{
		Programming_4H8LSC();
		SaveOTPData(vData);
		m_pInterface->AddLog(_T("This is Dummy Burn!"),COLOR_RED);
	}
	return TRUE;
}

void COtpHLTE1M::SaveOTPData(vector<double>& vData)
{
	m_pInterface->AddLog(_T("Read OTP Start!"));
	///////////////////////////////Read OTP////////////////////////////////////
	int valSize = 64;
	int i = 0;
	int j = 0;
	BYTE* pVal = new BYTE[valSize]();

	pVal[0] = Data_Write.data_Basic[0];
	pVal[52] = Data_Write.data_AF[0];
	if (m_LscGroup == 1)
	{
		for (i=0; i<SIZE_BASIC_GROUP+1; i++)
		{
			pVal[i+1] = Data_Write.data_Basic[i+1];
		}
		for (i=0; i<SIZE_WB_GROUP; i++)
		{
			pVal[i+SIZE_BASIC_GROUP+1] = Data_Write.data_WB[i];
		}
		for (i=0; i<SIZE_AF_GROUP; i++)
		{
			pVal[53+i] = Data_Write.data_AF[i+1];
		}
	}
	else if (m_LscGroup == 2)
	{
		for (i=0; i<SIZE_BASIC_GROUP+1; i++)
		{
			pVal[i+1+OTPGAP_BAISC] = Data_Write.data_Basic[i+1];
		}
		for (i=0; i<SIZE_WB_GROUP; i++)
		{
			pVal[i+SIZE_BASIC_GROUP+1+OTPGAP_BAISC] = Data_Write.data_WB[i];
		}
		for (i=0; i<SIZE_AF_GROUP; i++)
		{
			pVal[53+i+OTPGAP_AF] = Data_Write.data_AF[i+1];
		}
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

	if (m_LSCItem == 2)//只有sensor lsc才测shading
	{
		num.Format("Shading Data After SRAM:\nB1:%.1f\nB2:%.1f\nB3:%.1f\nB4:%.1f\n",vData.at(0), vData.at(1), vData.at(2), vData.at(3));
		temp += num;
	}

	num.Format("Page: 0x%02x\n", 15);
	temp += num;
	for(i=0; i<64; i++)
	{
		num.Format("%04d\t0x%02x,0x%02x\n", i,0x0A04+i,(unsigned char)pVal[i]);
		temp += num;
	}
	num.Format("LSC Data:\n");
	temp += num;

	for (i=0; i<448; i++)
	{
		num.Format("%04d\t0x%02x\n", i, (unsigned char)S5K4H8_LSCWriteData[i]);
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

void COtpHLTE1M::CreateBurningData(void)
{
	int i = 0;
	//////////////////////////////////Basic//////////////////////////////////////
	if (m_LscGroup == 1)
	{
		Data_Write.data_Basic[0] = 0x44;
	}
	else if (m_LscGroup == 2)
	{
		Data_Write.data_Basic[0] = 0xDD;
	}

	for (i = 0; i < SIZE_BASIC_GROUP; i++)
	{
		Data_Write.data_Basic[i+1] = OTP_Data_Write.BaseInfo[i];
	}

	//////////////////////////////////AWB//////////////////////////////////////
//	Data_Write.data_WB[0] = FLAG_VALID;
	for (i = 0; i < SIZE_WB_GROUP; i++)
	{
		Data_Write.data_WB[i] = OTP_Data_Write.AWBInfo[i];
	}

	//////////////////////////////////LSC//////////////////////////////////////
//	Data_Write.data_Lenc[0] = FLAG_VALID;
	if (m_LSCItem == 0)
	{
		for (i = 0; i < SIZE_LSC_GROUP; i++)
		{
			Data_Write.data_Lenc[i+1] =  OTP_Data_Write.LSCInfo[i] = m_mtklscdata[i];
		}
	}
	if (m_LSCItem==1)
	{
		for (i=0; i<SIZE_LSC_GROUP-1;i++)
		{
			Data_Write.data_Lenc[i+1] =  OTP_Data_Write.LSCInfo[i] = m_quallscdata[i];
		}
	}
	///////////////////////////////////AF/////////////////////////////////////
	if (m_LscGroup == 1)
	{
		Data_Write.data_AF[0] = 0x44;
	}
	else if (m_LscGroup == 2)
	{
		Data_Write.data_AF[0] = 0xDD;
	}
	for (i = 0; i < SIZE_AF_GROUP; i++)
	{
		Data_Write.data_AF[i+1] = OTP_Data_Write.AFInfo[i];
	}

	//////////////////////////////////PDAF////////////////////////////////////
	if (m_EnPDAF)
	{
		Data_Write.data_PDAF1[0] = FLAG_VALID;
		if (m_PDAFItem == 0)
		{
			for (i = 0; i < SIZE_PDAF_PROC1; i++)
			{
				Data_Write.pdaf_proc1[i] = OTP_Data_Write.PDAF1[i] = m_proc1data[i];
			}
		}
		if (m_PDAFItem == 1)
		{
			for (i=0; i<SIZE_PDAF_PROC1;i++)
			{
				Data_Write.data_PDAF1[i+1] = OTP_Data_Write.PDAF1[i] = Qual_J_GainMapData[i];
			}
			for (i=0;i<Reserve_PDAF1; i++)
			{
				Data_Write.reservedpdaf1[i] = Reserverd_Data;
			}
		}

	}
	//////////////////////////////////////////////////////////////////////////
	S5K4H8_CalcAllCheckSum();

	return;
}

BOOL COtpHLTE1M::CheckOTP()
{
	if (!S5K4H8_OTPVerify())		return FALSE;
	return TRUE;
}

BOOL COtpHLTE1M::VerifyLSC()
{
	if (m_LSCItem == 2)
	{
		if (!S5K4H8_VerifyLSC())		return FALSE; //for sensor LSC
	}
	return TRUE;
}

BOOL COtpHLTE1M::VerifyPreWB()
{
	if (m_EnAWB)
	{
		if (!GetOTPRGBG())		return FALSE;
		S5K4H8_ApplyAWB(m_wbCurrent,m_goldenrg,m_goldenbg);
	}
	return TRUE;
}

BOOL COtpHLTE1M::VerifyAWB()
{
	if (m_EnAWB)
	{
		if (!S5K4H8_VerifyAWB())		return FALSE;
		if (m_SaveAWB)
		{
			SaveAwbData(&m_wbCurrent, 2);
		}
	}
	return TRUE;
}

BOOL COtpHLTE1M::GetAWB()
{
	if (m_EnAWB)
	{
		if (!InitAWB())		return FALSE;
		if (!S5K4H8_GetAWBData(OTP_Data_Write))
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

BOOL COtpHLTE1M::S5K4H8_GetAWBData(S5K4H8OTPDataSpec &awbdata)
{
	m_pInterface ->AddLog(_T("Get AWB Data Start"));

	if (!S5K4H8_GetCurrentAWB(&m_wbCurrent))		return FALSE;


	awbdata.AWBInfo[0] = (m_wbCurrent.rg>>8)&0x0f;
	awbdata.AWBInfo[1] = m_wbCurrent.rg&0xff;
	awbdata.AWBInfo[2] = (m_wbCurrent.bg>>8)&0x0f;
	awbdata.AWBInfo[3] = m_wbCurrent.bg&0xff;
	awbdata.AWBInfo[4] = (m_wbCurrent.gb_gr>>8)&0x0f;
	awbdata.AWBInfo[5] = m_wbCurrent.gb_gr&0xff;

	//Reserverd Data
	for (int i=0;i<Reserve_AWB;i++)
	{
		awbdata.AWBInfo[6+i] = Reserverd_Data;
	}

	Applyawbdata[0] = m_wbCurrent.r;//for dummy save data
	Applyawbdata[1] = m_wbCurrent.gr;
	Applyawbdata[2] = m_wbCurrent.gb;
	Applyawbdata[3] = m_wbCurrent.b;
	Applyawbdata[4] = m_wbCurrent.rg;
	Applyawbdata[5] = m_wbCurrent.bg;
	Applyawbdata[6] = m_wbCurrent.gb_gr;
	m_pInterface ->AddLog(_T("Get AWB Data End"));
	S5K4H8_ApplyAWB(m_wbCurrent, m_goldenrg, m_goldenbg);
	return TRUE;
}

BOOL COtpHLTE1M::CheckOTPAWB()
{
	if (m_EnAWB)
	{
		m_pInterface ->AddLog(_T("Check Apply AWB Data Start"));
		if (!GetAWBInfo(&m_wbCurrent))		return FALSE;

		if (!S5K4H8_CheckAWB(m_wbCurrent,1))				return FALSE;

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

BOOL COtpHLTE1M::S5K4H8_GetCurrentAWB(tagAwbRatio* wb)
{
	if (!GetAWBInfo(&m_wbCurrent))		return FALSE;

	if (!S5K4H8_CheckAWB(m_wbCurrent,0))				return FALSE;

	//Apply Before
	m_pInterface->AddLog(_T("Before Apply WB:"));
#ifndef Raw8Data
	info.Format(_T("OB(10bit) = %02d"),SENSOROB);
#else
	info.Format(_T("OB(8bit) = %02d"),SENSOROB/4);
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
	info.Format(_T("Gr = %02d"),m_wbCurrent.gr);
	m_pInterface->AddLog(info);
	info.Format(_T("Gb = %02d"),m_wbCurrent.gb);
	m_pInterface->AddLog(info);

	return TRUE;
}

BOOL COtpHLTE1M::GetAWBInfo(tagAwbRatio* wb)
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

	//check
	wb->rg		= (UINT)(1.0 * (wb->r-SENSOROB) / (wb->g-SENSOROB)*AWB_FACTOR + 0.5); 
	wb->bg		= (UINT)(1.0 * (wb->b-SENSOROB) / (wb->g-SENSOROB)*AWB_FACTOR + 0.5);
	wb->gb_gr	= (UINT)(1.0 * (wb->gb-SENSOROB)/ (wb->gr-SENSOROB)*AWB_FACTOR+ 0.5);

	//raw8
#ifdef Raw8Data
	wb->r = (UINT)(wb->r/4 + 0.5);
	wb->g = (UINT)(wb->g/4 + 0.5);
	wb->b = (UINT)(wb->b/4 + 0.5);
	wb->gb = (UINT)(wb->gb/4 + 0.5);
	wb->gr = (UINT)(wb->gr/4 + 0.5);

	wb->rg = (UINT)(wb->rg/2 + 0.5);
	wb->bg = (UINT)(wb->bg/2 + 0.5);
	wb->gb_gr = (UINT)(wb->gb_gr/2 + 0.5);
#endif

	m_szStr.Format(_T("R/G: %d B/G: %d"), wb->rg, wb->bg);
	m_pInterface->AddLog(m_szStr, COLOR_BLUE);
	return TRUE;
}

BOOL COtpHLTE1M::S5K4H8_CheckAWB(tagAwbRatio wb, int type)
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

void COtpHLTE1M::S5K4H8_GetBasicData(void)
{
//	m_pInterface ->AddLog(_T("Get Basic Data Start"));
	CTime time = CTime::GetCurrentTime();

	OTP_Data_Write.BaseInfo[0] = BASIC_MID;
	OTP_Data_Write.BaseInfo[1] = time.GetYear() - 2000;
	OTP_Data_Write.BaseInfo[2] = time.GetMonth();
	OTP_Data_Write.BaseInfo[3] = time.GetDay();
	OTP_Data_Write.BaseInfo[4] = BASIC_LENSID;
// 	OTP_Data_Write.BaseInfo[5] = BASIC_VCMID;
// 	OTP_Data_Write.BaseInfo[6] = BASIC_DRIVERICID;
// 	OTP_Data_Write.BaseInfo[7] = BASIC_SENSOR,
// 	OTP_Data_Write.BaseInfo[7] = BASIC_LSCVER;
// 	OTP_Data_Write.BaseInfo[8] = BASIC_PDAFVER;
// 	OTP_Data_Write.BaseInfo[9] = BASIC_COLORTEMP;
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

	for (int i = 0; i<Reserve_Basic; i++)
	{
		OTP_Data_Write.BaseInfo[5+i] = Reserverd_Data;
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

	m_pInterface ->AddLog(_T("Get Basic Data."));
	return;
}

void COtpHLTE1M::S5K4H8_GetAFData(void)
{
//	m_pInterface ->AddLog(_T("Get AF Data Start"));
	OTP_Data_Write.AFInfo[0] = (m_macCode>>8)&0x0f;
	OTP_Data_Write.AFInfo[1] = m_macCode&0xff;
	OTP_Data_Write.AFInfo[2] = (m_infCode>>8)&0x0f;
	OTP_Data_Write.AFInfo[3] = m_infCode&0xff;

	m_pInterface ->AddLog(_T("Get AF Data."));
	return;
}
void COtpHLTE1M::S5K4H8_CalcAllCheckSum(void)
{
	int i = 0;
	int sum = 0;

	// LSC
	if (m_LSCItem != 2)
	{
		sum = SumCalc(OTP_Data_Write.LSCInfo, SIZE_LSC_GROUP-1, 255,1);
		Data_Write.data_Lenc[SIZE_LSC_GROUP] = sum;
	}

// 	//AWB
// 	sum = SumCalc(OTP_Data_Write.AWBInfo,SIZE_WB_GROUP,255,1);
// 	Data_Write.data_WB[SIZE_WB_GROUP] = sum;
// 
// 	//Basic
// 	sum = SumCalc(OTP_Data_Write.BaseInfo,SIZE_BASIC_GROUP-1,255,1);
// 	Data_Write.data_Basic[SIZE_BASIC_GROUP] = sum;

	//AF	
// 	sum = SumCalc(OTP_Data_Write.AFInfo,SIZE_AF_GROUP,255,1);
// 	Data_Write.data_AF[SIZE_AF_GROUP] = sum;

	if (m_EnPDAF)
	{
		//PDAF1
		sum = SumCalc(OTP_Data_Write.PDAF1,SIZE_PDAF_PROC1,255,1);
		Data_Write.data_PDAF1[SIZE_PDAF_PROC1+1] = sum;//checksum在reserve前面
	}

	return;
}

BOOL COtpHLTE1M::CheckSensorOTPEmpty()
{
	BYTE tmpdata[64] = {0};
	if (m_sensoritem == 2)
	{		
		S5K4H8ReadOTPpage(4,0x0a04,tmpdata,64);

// 		S5K4H8ReadOTPpage(15,0x0a04,tmpdata,64);
// 		Sensor4H8_CheckLSCReadWrite();
		if (m_LscGroup != 1)
		{
			if ( (tmpdata[0] == 0) && (tmpdata[1] == 0)&& (tmpdata[2] == 0) && (tmpdata[3] == 0) && (tmpdata[4] == 0))
			{
				m_pInterface->AddLog(_T("第一组LSC数据为空，不可烧录第二组!"),COLOR_RED);		
				return FALSE;
			}
			S5K4H8ReadOTPpage(10,0x0a04,tmpdata,64);
			if ( (tmpdata[0] != 0) || (tmpdata[1] != 0)|| (tmpdata[2] != 0) || (tmpdata[3] != 0) || (tmpdata[4] != 0))
			{
				m_pInterface->AddLog(_T("第二组LSC数据不为空，开始检测OTP内已有数据!"),COLOR_RED);		
				m_sensorotpisempty = TRUE;
				return FALSE;
			}
		}
		else if (m_LscGroup == 1)
		{
			if ( (tmpdata[0] != 0) || (tmpdata[1] != 0)|| (tmpdata[2] != 0) || (tmpdata[3] != 0) || (tmpdata[4] != 0))
			{
				m_pInterface->AddLog(_T("第一组LSC数据不为空，开始检测OTP内已有数据!"),COLOR_RED);		
				m_sensorotpisempty = TRUE;
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL COtpHLTE1M::S5K4H8_OtpWrite(void)
{
	////////////////////////////////////Check Sensor OTP Empty/////////////////////////////////
	if (m_LSCItem == 2)
	{
		if (!CheckSensorOTPEmpty())		return FALSE;
	}
	/////////////////////////////////Burning/////////////////////////////////////////
	m_pInterface->AddLog(_T("OTP Burning Start!"));

	if (!ProgrammingBasic())  return FALSE; 
	m_pInterface->AddLog(_T("Basic Burning End!"));

	if (m_EnAWB)
	{
		if (!ProgrammingAWB())  return FALSE;
		m_pInterface->AddLog(_T("AWB Burning End!"));
	}

	if (m_EnLSC)
	{
		if (!ProgrammingLSC())  return FALSE;
		m_pInterface->AddLog(_T("LSC Burning End!"));
	}

	if (m_EnAF)
	{
		if (!ProgrammingAF())  return FALSE;
		m_pInterface->AddLog(_T("AF Burning End!"));
	}

	if (m_EnPDAF)
	{
		if (!ProgrammingPDAF())  return FALSE;
		m_pInterface->AddLog(_T("PDAF Burning End!"));
	}

	m_pInterface->AddLog(_T("OTP Burning Success!"),COLOR_BLUE);
	return TRUE;
}

BOOL COtpHLTE1M::ProgrammingBasic()
{
	if (m_LSCItem == 0)
	{
		if (GT24C64_Programming(m_addrFlag.moduleInfo,Data_Write.data_Basic,sizeof(Data_Write.data_Basic)) != TRUE)
		{
			m_pInterface->AddLog(_T("Basic Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
	}
	else if (m_LSCItem == 1)
	{
		if (ZC533_Programming(m_addrFlag.moduleInfo,Data_Write.data_Basic,sizeof(Data_Write.data_Basic)) != TRUE)
		{
			m_pInterface->AddLog(_T("Basic Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
	}
	else if (m_LSCItem == 2)
	{
		if (m_sensoritem == 2)
		{
			if (m_LscGroup>m_grouplimit)
			{
				m_pInterface->AddLog(_T("OTP只允许烧录第一组!"),COLOR_RED);		
				return FALSE;
			}
			Programming_4H8Basic();
		}

	}
	return TRUE;
}

BOOL COtpHLTE1M::Programming_4H8Basic()
{
	int i = 0;
	
	memset(S5K4H8_WriteData,0,64*sizeof(BYTE));
	S5K4H8_WriteData[0] = Data_Write.data_Basic[0];
	for (i=0; i<SIZE_BASIC_GROUP; i++)
	{
		S5K4H8_WriteData[i+1+(m_LscGroup-1)*OTPGAP_BAISC] = Data_Write.data_Basic[i+1];
	}

//	if (!S5K4H8WriteOTPpage(15,m_addrFlag.moduleInfo,WriteData,64)) return FALSE;

	return TRUE;
}

BOOL COtpHLTE1M::ProgrammingAWB()
{
	if(m_LSCItem == 0)
	{
		if (GT24C64_Programming(m_addrFlag.awb,Data_Write.data_WB,sizeof(Data_Write.data_WB)) != TRUE)
		{
			m_pInterface->AddLog(_T("AWB Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
	}
	else if(m_LSCItem == 1)
	{
		if (ZC533_Programming(m_addrFlag.awb,Data_Write.data_WB,sizeof(Data_Write.data_WB)) != TRUE)
		{
			m_pInterface->AddLog(_T("AWB Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
	}
	else if(m_LSCItem == 2)
	{
		if (m_sensoritem == 2)
		{
			Programming_4H8AWB();
		}
	}

	return TRUE;
}

BOOL COtpHLTE1M::Programming_4H8AWB()
{
	int i = 0;

	for (i=0; i<SIZE_WB_GROUP; i++)
	{
		S5K4H8_WriteData[i+8+(m_LscGroup-1)*OTPGAP_BAISC] = Data_Write.data_WB[i];
	}

//	if (!S5K4H8WriteOTPpage(15,m_addrFlag.moduleInfo,WriteData,64)) return FALSE;

	return TRUE;
}

BOOL COtpHLTE1M::ProgrammingLSC()
{
	if (m_LSCItem == 0)
	{
		if (GT24C64_Programming(m_addrFlag.lsc,Data_Write.data_Lenc,sizeof(Data_Write.data_Lenc)) != TRUE)
		{
			m_pInterface->AddLog(_T("LSC Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
	}
	else if (m_LSCItem == 1)
	{
		if (ZC533_Programming(m_addrFlag.lsc,Data_Write.data_Lenc,sizeof(Data_Write.data_Lenc)) != TRUE)
		{
			m_pInterface->AddLog(_T("LSC Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
	}
	else if(m_LSCItem == 2)
	{
		if (m_sensoritem == 2)
		{
			Programming_4H8LSC();
		}
	}

	return TRUE;
}

BOOL COtpHLTE1M::Programming_4H8LSC()
{
	BYTE _LSCData[10] = {0};

	if (m_LscGroup == 1)
	{
		if (!S5K4H8ReadOTPpage(4, 0x0a04,  _LSCData, 10)) return FALSE;		
	}
	else if (m_LscGroup == 2)
	{
		if (!S5K4H8ReadOTPpage(10, 0x0a04,  _LSCData, 10)) return FALSE;		
	}


	if (!m_dummyburn)
	{
		if ( (_LSCData[8]|_LSCData[7]|_LSCData[6]|_LSCData[5]|_LSCData[4]|_LSCData[3]|_LSCData[2]|_LSCData[1]|_LSCData[0]) != 0)
		{
			m_pInterface->AddLog(_T("OTP内已有LSC数据！无法重复烧录"), COLOR_RED);
			return FALSE;
		}
	}


	m_pInterface->bSensorEnable(3^3,1);
	msleep(10);
	m_pInterface->bSensorEnable(3,1);
	msleep(10);

	BYTE stram_flag = 0;
	do 
	{
		i2c_read_byte(0x0100,&stram_flag);
		if (stram_flag == 0)
		{
			i2c_write_byte(0x0100,0x01);
		}
		i2c_read_byte(0x0100,&stram_flag);
	} while (stram_flag == 0);

	CString tempStr;
	CString strWorkPath  = GetModulePath()+"\\LSC\\Sensor\\4H8";			
	tempStr.Format(_T("\\4H8LSCOTP.ini"));
	tempStr = strWorkPath + tempStr;
	
	CStdioFile file;
	if ( !file.Open( (tempStr), CFile::modeRead | CFile::typeText ) )
	{
		m_pInterface->AddLog(_T("打开文件失败！"), COLOR_RED);
		return FALSE;
	}

	CString strLine;
	int intVal;
	int  intLSCAddr;
	int  intLSCValue; 
	int page=0;
	while ( file.ReadString(strLine) )
	{ 
		strLine.TrimLeft();
		strLine.TrimRight();
		if ( strLine.Find( _T("s"), 0 ) != -1 )
		{
			int nLength = strLine.GetLength();
			if (nLength == 7)
			{
					intVal = strtoul( CT2A(strLine.Mid(1,6)), NULL, 16 );
					intLSCAddr = intVal>>8;
					intLSCValue = intVal%256;
					if (!m_dummyburn)
					{
						i2c_write_byte(intLSCAddr,intLSCValue);
//						msleep(10);
					}
			}
			else
			{
				if (m_LscGroup == 1 || m_LscGroup == 2)
				{
					nLength = nLength - 5;
					strLine = strLine.Right(nLength);
					for (int k = 0; k<64; k++ )
					{
						intLSCValue = strtoul( CT2A(strLine.Mid(k*2,2)), NULL, 16);
						S5K4H8_LSCWriteData[k+page*64] = intLSCValue;
						if (!m_dummyburn)
						{
							i2c_write_byte(0x0A04+k, intLSCValue);
//							msleep(2);
						}
					}
				}
				page ++;
			}
		}
		else if (strLine.Find( _T("p"), 0 ) != -1)
		{
			msleep(100);
		}
	}

	file.Close();
	SaveExp = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllExp"), 1);
	m_pInterface->GetImageSensor()->WriteExp(SaveExp);
	SaveGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"RecordExp",_T("ExpGain"), 1);
	m_pInterface->GetImageSensor()->WriteAnalogGain(SaveGain);
	return TRUE;
}

BOOL COtpHLTE1M::ProgrammingAF()
{
	if (m_LSCItem == 0)
	{
		if (GT24C64_Programming(m_addrFlag.af,Data_Write.data_AF,sizeof(Data_Write.data_AF)) != TRUE)
		{
			m_pInterface->AddLog(_T("AF Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
	}
	else if (m_LSCItem == 1)
	{
		if (ZC533_Programming(m_addrFlag.af,Data_Write.data_AF,sizeof(Data_Write.data_AF)) != TRUE)
		{
			m_pInterface->AddLog(_T("AF Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
	}
	else if (m_LSCItem == 2)
	{
		if (m_sensoritem == 2)
		{
			Programming_4H8AF();
		}
	}

	return TRUE;
}

BOOL COtpHLTE1M::Programming_4H8AF()
{
	int i = 0;

	S5K4H8_WriteData[52] = Data_Write.data_AF[0];
	for (i=0; i<SIZE_AF_GROUP; i++)
	{
		S5K4H8_WriteData[i+53+(m_LscGroup-1)*OTPGAP_AF] = Data_Write.data_AF[i+1];
	}

	if (!S5K4H8WriteOTPpage(15,m_addrFlag.moduleInfo,S5K4H8_WriteData,64)) return FALSE;

	return TRUE;
}

BOOL COtpHLTE1M::ProgrammingPDAF()
{
	BOOL bRet = TRUE;

	//MTK		
	if (m_PDAFItem == 0)
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

				if (GT24C64_Programming(m_addrSum.pdaf, sum1, 1) != TRUE)	
				{	
					m_pInterface->AddLog(_T("PDAF Proc1 CheckSum Write Fail!"),COLOR_RED);
					bRet &= FALSE;
					break;
				}	
			}		
		} while (0);			

		SAFE_DELETE_ARRAY(flag);			
		SAFE_DELETE_ARRAY(sum1);		
		return bRet;
	}
	//Qual
	if (m_PDAFItem == 1)
	{
		if (ZC533_Programming(m_addrFlag.pdaf1,Data_Write.data_PDAF1,sizeof(Data_Write.data_PDAF1)) != TRUE)
		{
			m_pInterface->AddLog(_T("AF Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
	}

	return TRUE;
}

BOOL COtpHLTE1M::S5K4H8_OTPVerify(void)
{
	if (!m_dummyburn)
	{
		if (!m_sensorotpisempty)
		{
			if (!S5K4H8_CheckOTPReadWrite())		return FALSE;
		}
		else
		{
			if (!S5K4H8_CheckOTP())		return FALSE;
		}
	}
	else 
	{
		m_pInterface->AddLog(_T("This is Dummy Burn!"),COLOR_RED);
	}
   return TRUE;
}

BOOL COtpHLTE1M::S5K4H8_CheckOTP()
{
	BYTE otpdata[64] = {0};
	BYTE readBasicflag = 0;
	BYTE readAFflag = 0;
	S5K4H8ReadOTPpage(15,0x0A04, otpdata,64);
	if (otpdata[0] == 0 || otpdata[52] == 0)
	{
		m_pInterface->AddLog(_T("OTP内Basic Flag或者AF Flag为空!"),COLOR_RED);
		return FALSE;
	}
	else if (otpdata[0] == 0x44)
	{
		readBasicflag = 1;
	}
	else if (otpdata[0] == 0xDD)
	{
		readBasicflag = 2;
	}
	else
	{
		m_pInterface->AddLog(_T("Basic Flag异常!"),COLOR_RED);
		return FALSE;
	}

	if (otpdata[52] == 0x44)
	{
		readAFflag = 1;
	}
	else if (otpdata[52] == 0xDD)
	{
		readAFflag = 2;
	}
	else
	{
		m_pInterface->AddLog(_T("Basic Flag异常!"),COLOR_RED);
		return FALSE;
	}

	if (otpdata[1+(readBasicflag-1)*OTPGAP_BAISC] != BASIC_MID)
	{
		m_pInterface->AddLog(_T("MID异常!"),COLOR_RED);
		return FALSE;
	}
	if (otpdata[5+(readBasicflag-1)*OTPGAP_BAISC] != BASIC_LENSID)
	{
		m_pInterface->AddLog(_T("LensID异常!"),COLOR_RED);
		return FALSE;
	}


	m_pInterface->AddLog(_T("OTP内已有数据检查Pass!"),COLOR_BLUE);
	return TRUE;
}

BOOL COtpHLTE1M::S5K4H8_CheckOTPReadWrite(void)
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

		if (GT24C64_Read(m_addrFlag.pdaf1, Data_Read.data_PDAF1,sizeof(Data_Read.data_PDAF1)) != TRUE)
		{
			m_pInterface->AddLog(_T("Read PDAF Proc1 Data Fail!"),COLOR_RED);		
			return FALSE;
		}
		else
		{
			if (!CheckEqual(Data_Write.data_PDAF1,Data_Read.data_PDAF1,sizeof(Data_Write.data_PDAF1)))
			{
				m_pInterface->AddLog(_T("Check PDAF Proc1 Data Fail!"),COLOR_RED);
				return FALSE;
			}
		}
	}

	if (m_LSCItem == 1)
	{
		if (ZC533_Read(m_addrFlag.moduleInfo, Data_Read.data_Basic,sizeof(Data_Read.data_Basic)) != TRUE)
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

		if (ZC533_Read(m_addrFlag.awb, Data_Read.data_WB,sizeof(Data_Read.data_WB)) != TRUE)
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

		if (ZC533_Read(m_addrFlag.lsc, Data_Read.data_Lenc,sizeof(Data_Read.data_Lenc)) != TRUE)
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

		if (ZC533_Read(m_addrFlag.af, Data_Read.data_AF,sizeof(Data_Read.data_AF)) != TRUE)
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

		if (ZC533_Read(m_addrFlag.pdaf1, Data_Read.data_PDAF1,sizeof(Data_Read.data_PDAF1)) != TRUE)
		{
			m_pInterface->AddLog(_T("Read PDAF Proc1 Data Fail!"),COLOR_RED);		
			return FALSE;
		}
		else
		{
			if (!CheckEqual(Data_Write.data_PDAF1,Data_Read.data_PDAF1,sizeof(Data_Write.data_PDAF1)))
			{
				m_pInterface->AddLog(_T("Check PDAF Proc1 Data Fail!"),COLOR_RED);
				return FALSE;
			}
		}
	}

	if (m_LSCItem == 2)
	{
		if (m_sensoritem == 2)
		{
			if (!Sensor4H8_CheckReadWrite()) return FALSE; 
			if (!Sensor4H8_CheckLSCReadWrite()) return FALSE;
			m_pInterface->AddLog(_T("Check OTP Data Success!"),COLOR_BLUE);	
		}
	}

	m_pInterface->AddLog(_T("Check OTP Data OK!"),COLOR_BLUE);
	return TRUE;
}

BOOL COtpHLTE1M::Sensor4H8_CheckReadWrite()
{
	BYTE ReadData[64] = {0};
	if (!S5K4H8ReadOTPpage(15,0x0A04,ReadData,64)) return FALSE;

	int i = 0;

	if (ReadData[0] != Data_Write.data_Basic[0])
	{
		m_pInterface->AddLog(_T("Basic Flag读写对比失败！"),COLOR_RED);
		return FALSE;
	}
	for (i=0;i<SIZE_BASIC_GROUP; i++)
	{
		if (ReadData[i+1+(m_LscGroup-1)*OTPGAP_BAISC] != Data_Write.data_Basic[i+1])
		{
			m_pInterface->AddLog(_T("Basic Info读写对比失败！"),COLOR_RED);
			return FALSE;
		}
	}
	if (m_EnAWB)
	{
		for (i=0; i<SIZE_WB_GROUP; i++)
		{
			if (ReadData[i+8+(m_LscGroup-1)*OTPGAP_BAISC] != Data_Write.data_WB[i])
			{
				m_pInterface->AddLog(_T("AWB Info读写对比失败！"),COLOR_RED);
				return FALSE;
			}
		}
	}

	if (m_EnAF)
	{
		if (ReadData[52] != Data_Write.data_AF[0])
		{
			m_pInterface->AddLog(_T("AF Flag读写对比失败！"),COLOR_RED);
			return FALSE;
		}
		for (i=0; i<SIZE_AF_GROUP; i++)
		{
			if (ReadData[i+53+(m_LscGroup-1)*OTPGAP_AF] != Data_Write.data_AF[i+1])
			{
				m_pInterface->AddLog(_T("AF Info读写对比失败！"),COLOR_RED);
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL COtpHLTE1M::Sensor4H8_CheckLSCReadWrite()
{
	int i = 0;
	if (m_LscGroup == 1)
	{
		if (!S5K4H8ReadOTPpage(3,0x0A04, S5K4H8_LSCReadData,64)) return FALSE;
		msleep(10);
		if (!S5K4H8ReadOTPpage(4,0x0A04, S5K4H8_LSCReadData+64,64)) return FALSE;
		msleep(10);
		if (!S5K4H8ReadOTPpage(5,0x0A04, S5K4H8_LSCReadData+64*2,64)) return FALSE;
		msleep(10);
		if (!S5K4H8ReadOTPpage(6,0x0A04, S5K4H8_LSCReadData+64*3,64)) return FALSE;
		msleep(10);
		if (!S5K4H8ReadOTPpage(7,0x0A04, S5K4H8_LSCReadData+64*4,64)) return FALSE;
		msleep(10);
		if (!S5K4H8ReadOTPpage(8,0x0A04, S5K4H8_LSCReadData+64*5,64)) return FALSE;
		msleep(10);
		if (!S5K4H8ReadOTPpage(9,0x0A04, S5K4H8_LSCReadData+64*6,64)) return FALSE;
		msleep(10);

		for (i= 47; i<407; i++)
		{
			if (S5K4H8_LSCReadData[i] != S5K4H8_LSCWriteData[i])
			{
				m_pInterface->AddLog(_T("LSC Info读写对比失败！"));
				return 0;
			}
		}
	}
	if (m_LscGroup == 2)
	{
		if (!S5K4H8ReadOTPpage(9,0x0A04, S5K4H8_LSCReadData,64)) return FALSE;
		msleep(10);
		if (!S5K4H8ReadOTPpage(10,0x0A04, S5K4H8_LSCReadData+64,64)) return FALSE;
		msleep(10);
		if (!S5K4H8ReadOTPpage(11,0x0A04, S5K4H8_LSCReadData+64*2,64)) return FALSE;
		msleep(10);
		if (!S5K4H8ReadOTPpage(12,0x0A04, S5K4H8_LSCReadData+64*3,64)) return FALSE;
		msleep(10);
		if (!S5K4H8ReadOTPpage(13,0x0A04, S5K4H8_LSCReadData+64*4,64)) return FALSE;
		msleep(10);
		if (!S5K4H8ReadOTPpage(14,0x0A04, S5K4H8_LSCReadData+64*5,64)) return FALSE;
		msleep(10);
		for (i= 24; i<384; i++)
		{
			if (S5K4H8_LSCReadData[i] != S5K4H8_LSCWriteData[i])
			{
				m_pInterface->AddLog(_T("LSC Info读写对比失败！"));
				return 0;
			}
		}
	}
	return TRUE;
}

BOOL COtpHLTE1M::GetOTPRGBG()
{
	if (!m_sensorotpisempty)
	{
		m_wbCurrent.rg = Data_Write.data_WB[0]<<8|Data_Write.data_WB[1];
		m_wbCurrent.bg = Data_Write.data_WB[2]<<8|Data_Write.data_WB[3];
	}
	else
	{
		if (m_sensoritem == 2)
		{
			BYTE readwbflag = 0;
			S5K4H8ReadOTPpage(15, m_addrFlag.moduleInfo, &readwbflag, 1);
			if (readwbflag == 0x44)
			{
				readwbflag = 1;
			}
			else if(readwbflag == 0xdd)
			{
				readwbflag = 2;
			}

			BYTE readwb[4] = {0};
			S5K4H8ReadOTPpage(15, m_addrFlag.awb+(readwbflag-1)*OTPGAP_BAISC, readwb, 4);
			m_wbCurrent.rg = readwb[0]<<8|readwb[1];
			m_wbCurrent.bg = readwb[2]<<8|readwb[3];
		}
	}
	return TRUE;
}

BOOL COtpHLTE1M::S5K4H8_VerifyLSC()
{
	m_pInterface->AddLog(_T("Verify LSC:"));
	if (m_sensoritem == 2)
	{
		m_pInterface->RestartCamera(1);
		SaveExp = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllExp"), 1);
		m_pInterface->GetImageSensor()->WriteExp(SaveExp);
		SaveGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"RecordExp",_T("ExpGain"), 1);
		m_pInterface->GetImageSensor()->WriteAnalogGain(SaveGain);

		i2c_write(0x6028, 0x4000);
		i2c_write(0x602a, 0x0B00);
		i2c_write_byte(0x6F12, 0x01);
		msleep(100);

		i2c_write(0x6028, 0x4000);
		i2c_write(0x602a, 0x0100);
		i2c_write_byte(0x6F12, 0x01);
		m_pInterface->AddLog(_T("AutoLoading LSC OK!"),COLOR_BLUE);
	}

	return TRUE;
}

BOOL COtpHLTE1M::S5K4H8_VerifyAWB()
{
	if (!GetAWBInfo(&m_wbCurrent))		return FALSE;
	if (!S5K4H8_CheckAWB(m_wbCurrent,2))		return FALSE;
	return TRUE;
}