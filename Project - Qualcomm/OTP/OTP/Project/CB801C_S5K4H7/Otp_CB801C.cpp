#include "stdafx.h"
#include "Otp_CB801C.h"
#include "..\..\OTP_SensorSetting.h"

COtpCB801C::COtpCB801C(void)
{
	ZeroMemory(OTP_Data_Write.AFInfo,sizeof(OTP_Data_Write.AFInfo));
	ZeroMemory(OTP_Data_Write.BaseInfo,sizeof(OTP_Data_Write.BaseInfo));
	ZeroMemory(S5K4H7_LSCWriteData,sizeof(S5K4H7_LSCWriteData));
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


COtpCB801C::~COtpCB801C(void)
{

}

void COtpCB801C::PreSubclassOtp()
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
// 	wbReg->addrGrHigh = S5K4H7_ADDR_GAIN_GR_H;
// 	wbReg->addrGrLow  = S5K4H7_ADDR_GAIN_GR_L;
// 	wbReg->addrGbHigh = S5K4H7_ADDR_GAIN_GB_H;
// 	wbReg->addrGbLow  = S5K4H7_ADDR_GAIN_GB_L;
// 	wbReg->addrRHigh  = S5K4H7_ADDR_GAIN_R_H;
// 	wbReg->addrRLow   = S5K4H7_ADDR_GAIN_R_L;
// 	wbReg->addrBHigh  = S5K4H7_ADDR_GAIN_B_H;
// 	wbReg->addrBLow   = S5K4H7_ADDR_GAIN_B_L;


	tagAwbRatio *wbGolden = GetWBGoldenValue();		
	wbGolden->rg  = m_goldenrg;
	wbGolden->bg  = m_goldenbg;

	LencOtpControlStruct *lencCtrl = GetLencControlParams();
// 	lencCtrl->inputTarget = S5K4H7_VAL_LSC_TARGET;
// 	lencCtrl->inputLow = S5K4H7_VAL_LSC_LOW;
// 	lencCtrl->inputHigh = S5K4H7_VAL_LSC_HIGH;
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
// 	lencReg->addrCtrl = S5K4H7_ADDR_LSC_CTRL;
// 	lencReg->lencEnable = S5K4H7_ADDR_LSC_EN;
// 	lencReg->lencDisable = S5K4H7_ADDR_LSC_DIS;
//	lencReg->addrDataStart = REG_INVALID;
//	lencReg->dataSize = S5K4H7_OTP_LSC_SIZE;

	ExpControlStruct *expCtrl = GetExpControlParams();
	expCtrl->type = EXP_LINEAR;
	expCtrl->roiCalcMode = ROI_CALC_5;
	expCtrl->filterChannel = EXP_CHANNEL_G;
// 	expCtrl->target = S5K4H7_VAL_EXP_TARGET;
// 	expCtrl->lowLimit = S5K4H7_VAL_EXP_LOW;
// 	expCtrl->highLimit = S5K4H7_VAL_EXP_HIGH;
	expCtrl->effectFrame = 3;
	expCtrl->stableFrame = 3;

	ExpRegisterStruct *expReg = GetExpRegister();
	expReg->mask = 0x00FFFF;
	expReg->addrHigh = REG_INVALID;
// 	expReg->addrMid = S5K4H7_ADDR_EXP_H;
// 	expReg->addrLow = S5K4H7_ADDR_EXP_L;

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

void COtpCB801C::SetOtpSensorParams()
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
//	params->otpInit.params = S5K4H7_OTP_Settings;
//	params->otpInit.paramSize = sizeofS5K4H7_OTP_Settings;

	params->verifyInit.width  = 4208;
	params->verifyInit.height = 3120;
	params->verifyInit.format = IMAGE_FORMAT_OTP_RAW10;
	params->verifyInit.sequence = IMAGE_SEQUENCE_GBRG;
	params->verifyInit.mhzMCLK = 24;
// 	params->verifyInit.params = S5K4H7_OTP_Settings;
// 	params->verifyInit.paramSize = sizeofS5K4H7_OTP_Settings;	
}


BOOL COtpCB801C::EraseAllData()
{
// 	if(m_LSCItem == 0)
// 	{
// 		int i = 0;
// 		BYTE* eraseData = new BYTE[32]();
// 		memset(eraseData, 0xff, 32*sizeof(BYTE));//注意初始EEPROM是00还是ff
// 
// 		for (i=0; i<110; i++)
// 		{
// 			if (!GT24C64_Programming(0x0000+i*32,eraseData,32))//erase all
// 			{
// 				m_pInterface->AddLog(_T("OTP数据擦除失败!"),COLOR_RED);
// 				SAFE_DELETE_ARRAY(eraseData);
// 				return 0;
// 			}
// 		}
// 
// 		SAFE_DELETE_ARRAY(eraseData);
// 	}
// 	else if (m_LSCItem == 1)
// 	{
// 		if (!ZC533_All_Erase())	return FALSE;
// 	}
// 
// 	m_pInterface->AddLog(_T("OTP所有数据已全部擦除!"));
	return TRUE;
}

void COtpCB801C::SetWBDefaultGain(void)
{
	return S5K4H7_SetDefaultAwb();
}

BOOL COtpCB801C::WriteOTP()
{
	S5K4H7_GetBasicData();
//	S5K4H7_GetAFData();
	CreateBurningData();
	if (!m_dummyburn)
	{
		if (!S5K4H7_OtpWrite())//
		{return FALSE;}
	}
	else
	{
		m_pInterface->AddLog(_T("This is Dummy Burn!"),COLOR_RED);
	}
	return TRUE;
}

void COtpCB801C::SaveOTPData()
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
			pVal[i+64] = S5K4H7_LSCWriteData[i+31];
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
			pVal[i+64] = S5K4H7_LSCWriteData[i+62];
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

BOOL COtpCB801C::WriteOTP(vector<double>& vData)
{
	S5K4H7_GetBasicData();
	S5K4H7_GetAFData();
	CreateBurningData();
	if (!m_dummyburn)
	{
		if (!S5K4H7_OtpWrite() && m_sensorotpisempty ==FALSE)//
		{return FALSE;}
	}
	else
	{
// 		Programming_4H7LSC();
// 		SaveOTPData(vData);
		m_pInterface->AddLog(_T("This is Dummy Burn!"),COLOR_RED);
	}
	return TRUE;
}

void COtpCB801C::SaveOTPData(vector<double>& vData)
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
		num.Format("%04d\t0x%02x\n", i, (unsigned char)S5K4H7_LSCWriteData[i]);
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

void COtpCB801C::CreateBurningData(void)
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
//	Data_Write.data_Lenc[0] = FLAG_VALID;
	if (m_LSCItem == 2)
	{
		for (i = 0; i < SIZE_LSC_GROUP; i++)
		{
			Data_Write.data_Lenc[i] = OTP_Data_Write.LSCInfo[i] = WriteLencData[i];
		}
	}
	if (m_LSCItem == 0)
	{
		for (i = 0; i < SIZE_LSC_GROUP; i++)
		{
			Data_Write.data_Lenc[i] = OTP_Data_Write.LSCInfo[i] = WriteLencData[i];
		}
	}
	if (m_LSCItem==1)
	{
		for (i = 0; i < SIZE_LSC_GROUP; i++)
		{
			Data_Write.data_Lenc[i] = OTP_Data_Write.LSCInfo[i] = WriteLencData[i];
		}
	}
	///////////////////////////////////AF/////////////////////////////////////


	//////////////////////////////////PDAF////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	S5K4H7_CalcAllCheckSum();

	return;
}

BOOL COtpCB801C::CheckOTP()
{
	if (!S5K4H7_OTPVerify())		return FALSE;
	return TRUE;
}

BOOL COtpCB801C::VerifyLSC()
{
	if (m_LSCItem == 2)
	{
		if (!S5K4H7_VerifyLSC())		return FALSE; //for sensor LSC
	}
	return TRUE;
}

BOOL COtpCB801C::VerifyPreWB()
{
	if (m_EnAWB)
	{
		if (!GetOTPRGBG())		return FALSE;
		S5K4H7_ApplyAWB(m_wbCurrent,m_goldenrg,m_goldenbg);
	}
	return TRUE;
}

BOOL COtpCB801C::VerifyAWB()
{
	if (m_EnAWB)
	{
		if (!S5K4H7_VerifyAWB())		return FALSE;
		if (m_SaveAWB)
		{
			SaveAwbData(&m_wbCurrent, 2);
		}
	}
	return TRUE;
}

BOOL COtpCB801C::GetAWB()
{
	if (m_EnAWB)
	{
		if (!InitAWB())		return FALSE;
		if (!S5K4H7_GetAWBData(OTP_Data_Write))
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

BOOL COtpCB801C::S5K4H7_GetAWBData(S5K4H7OTPDataSpec &awbdata)
{
	m_pInterface ->AddLog(_T("Get AWB Data Start"));

	if (!S5K4H7_GetCurrentAWB(&m_wbCurrent))		return FALSE;


	awbdata.AWBInfo[0] = (m_wbCurrent.rg>>8)&0x0f;
	awbdata.AWBInfo[1] = m_wbCurrent.rg&0xff;
	awbdata.AWBInfo[2] = (m_wbCurrent.bg>>8)&0x0f;
	awbdata.AWBInfo[3] = m_wbCurrent.bg&0xff;
	awbdata.AWBInfo[4] = (m_wbCurrent.gb_gr>>8)&0x0f;
	awbdata.AWBInfo[5] = m_wbCurrent.gb_gr&0xff;

	awbdata.AWBInfo[6] = (m_goldenrg >> 8) & 0x0f;
	awbdata.AWBInfo[7] = m_goldenrg & 0xff;
	awbdata.AWBInfo[8] = (m_goldenbg >> 8) & 0x0f;
	awbdata.AWBInfo[9] = m_goldenbg & 0xff;
	awbdata.AWBInfo[10] = (m_goldengg >> 8) & 0x0f;
	awbdata.AWBInfo[11] = m_goldengg & 0xff;

	Applyawbdata[0] = m_wbCurrent.r;//for dummy save data
	Applyawbdata[1] = m_wbCurrent.gr;
	Applyawbdata[2] = m_wbCurrent.gb;
	Applyawbdata[3] = m_wbCurrent.b;
	Applyawbdata[4] = m_wbCurrent.rg;
	Applyawbdata[5] = m_wbCurrent.bg;
	Applyawbdata[6] = m_wbCurrent.gb_gr;
	m_pInterface ->AddLog(_T("Get AWB Data End"));
	S5K4H7_ApplyAWB(m_wbCurrent, m_goldenrg, m_goldenbg);
	return TRUE;
}

BOOL COtpCB801C::CheckOTPAWB()
{
	if (m_EnAWB)
	{
		m_pInterface ->AddLog(_T("Check Apply AWB Data Start"));
		if (!GetAWBInfo(&m_wbCurrent))		return FALSE;

		if (!S5K4H7_CheckAWB(m_wbCurrent,1))				return FALSE;

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

BOOL COtpCB801C::S5K4H7_GetCurrentAWB(tagAwbRatio* wb)
{
	if (!GetAWBInfo(&m_wbCurrent))		return FALSE;

	if (!S5K4H7_CheckAWB(m_wbCurrent,0))				return FALSE;

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

BOOL COtpCB801C::GetAWBInfo(tagAwbRatio* wb)
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

BOOL COtpCB801C::S5K4H7_CheckAWB(tagAwbRatio wb, int type)
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

void COtpCB801C::S5K4H7_GetBasicData(void)
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

void COtpCB801C::S5K4H7_GetAFData(void)
{

	return;
}
void COtpCB801C::S5K4H7_CalcAllCheckSum(void)
{
	int i = 0;
	int sum = 0;

	// LSC
	//AWB
	sum = SumCalc(OTP_Data_Write.AWBInfo,SIZE_WB_GROUP-1,255,0);
	Data_Write.data_WB[SIZE_WB_GROUP] = sum;

	//Basic
	sum = SumCalc(OTP_Data_Write.BaseInfo,SIZE_BASIC_GROUP-1,255,0);
	Data_Write.data_Basic[SIZE_BASIC_GROUP] = sum;

	//AF	
// 	sum = SumCalc(OTP_Data_Write.AFInfo,SIZE_AF_GROUP,255,0);
// 	Data_Write.data_AF[SIZE_AF_GROUP] = sum;

	

	return;
}

BOOL COtpCB801C::CheckSensorOTPEmpty()
{
	BYTE tmpdata = 0;
	if (m_sensoritem == 2)
	{		
		S5K4H7ReadOTPByte(0, 0x0A3D, &tmpdata, 1);

// 		S5K4H7ReadOTPpage(15,0x0a04,tmpdata,64);
// 		Sensor4H8_CheckLSCReadWrite();
		if (m_LscGroup != 1)
		{
			if (tmpdata == 0x00)
			{
				m_pInterface->AddLog(_T("第一组LSC数据为空，不可烧录第二组!"), COLOR_RED);
				return FALSE;
			}
			else if (tmpdata == 0x11)
			{
				m_pInterface->AddLog(_T("第二组LSC数据不为空，开始检测OTP内已有数据!"), COLOR_RED);
				m_sensorotpisempty = TRUE;
				return FALSE;
			}

		}
		else if (m_LscGroup == 1)
		{
			if (tmpdata == 0x01)
			{
				m_pInterface->AddLog(_T("第一组LSC数据不为空，开始检测OTP内已有数据!"), COLOR_RED);
				m_sensorotpisempty = TRUE;
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL COtpCB801C::S5K4H7_OtpWrite(void)
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

BOOL COtpCB801C::ProgrammingBasic()
{
	if (m_LSCItem == 0)
	{
// 		if (GT24C64_Programming(m_addrFlag.moduleInfo,Data_Write.data_Basic,sizeof(Data_Write.data_Basic)) != TRUE)
// 		{
// 			m_pInterface->AddLog(_T("Basic Info Write Fail!"),COLOR_RED);		
// 			return FALSE;
// 		}
	}
	else if (m_LSCItem == 1)
	{
// 		if (ZC533_Programming(m_addrFlag.moduleInfo,Data_Write.data_Basic,sizeof(Data_Write.data_Basic)) != TRUE)
// 		{
// 			m_pInterface->AddLog(_T("Basic Info Write Fail!"),COLOR_RED);		
// 			return FALSE;
// 		}
	}
	else if (m_LSCItem == 2)
	{
		if (m_sensoritem == 4)
		{
			if (m_LscGroup>m_grouplimit)
			{
				m_pInterface->AddLog(_T("OTP只允许烧录第一组!"),COLOR_RED);		
				return FALSE;
			}
			Programming_4H7Basic();
		}

	}
	return TRUE;
}

BOOL COtpCB801C::Programming_4H7Basic()
{
	
	BYTE Read1, Read2,readflag;
	BYTE ReadData = 0 ;
	S5K4H7ReadOTPByte(21, 0x0A06, &ReadData, 1);
	Read1 = (ReadData >> 0) & 0x01;
	Read2 = (ReadData >> 1) & 0x01;
	if (m_LscGroup == 1)
	{
		if (Read1 != 0)
		{
			m_pInterface->AddLog(_T("OTP第一组内已有数据!"), COLOR_RED);
			return FALSE;
		}
		readflag = 0x01;
	}
	else if (m_LscGroup == 2)
	{
		if (Read2 != 0)
		{
			m_pInterface->AddLog(_T("OTP第二组内已有数据!"), COLOR_RED);
			return FALSE;
		}
		readflag = 0x13;
	}

	S5K4H7WriteSingleByte(21, 0x0A06, readflag);//flag
	S5K4H7WriteMultiByte(21, 0x0A07, Data_Write.data_Basic, 1,9);
	

//	if (!S5K4H7WriteOTPpage(15,m_addrFlag.moduleInfo,WriteData,64)) return FALSE;

	return TRUE;
}

BOOL COtpCB801C::ProgrammingAWB()
{
	if (m_LSCItem == 0)
	{
		// 		if (GT24C64_Programming(m_addrFlag.moduleInfo,Data_Write.data_Basic,sizeof(Data_Write.data_Basic)) != TRUE)
		// 		{
		// 			m_pInterface->AddLog(_T("Basic Info Write Fail!"),COLOR_RED);		
		// 			return FALSE;
		// 		}
	}
	else if (m_LSCItem == 1)
	{
		// 		if (ZC533_Programming(m_addrFlag.moduleInfo,Data_Write.data_Basic,sizeof(Data_Write.data_Basic)) != TRUE)
		// 		{
		// 			m_pInterface->AddLog(_T("Basic Info Write Fail!"),COLOR_RED);		
		// 			return FALSE;
		// 		}
	}
	else if (m_LSCItem == 2)
	{
		if (m_sensoritem == 4)
		{
			if (m_LscGroup > m_grouplimit)
			{
				m_pInterface->AddLog(_T("OTP只允许烧录第一组!"), COLOR_RED);
				return FALSE;
			}
			Programming_4H7AWB();
		}

	}

	return TRUE;
}

BOOL COtpCB801C::Programming_4H7AWB()
{
	BYTE Read1, Read2, readflag;
	BYTE ReadData = 0;
	S5K4H7ReadOTPByte(21, 0x0A19, &ReadData, 1);
	Read1 = (ReadData >> 0) & 0x01;
	Read2 = (ReadData >> 1) & 0x01;
	if (m_LscGroup == 1)
	{
		if (Read1 != 0)
		{
			m_pInterface->AddLog(_T("OTP第一组内已有数据!"), COLOR_RED);
			return FALSE;
		}
		readflag = 0x01;
	}
	else if (m_LscGroup == 2)
	{
		if (Read2 != 0)
		{
			m_pInterface->AddLog(_T("OTP第二组内已有数据!"), COLOR_RED);
			return FALSE;
		}
		readflag = 0x13;
	}
	S5K4H7WriteSingleByte(21, 0x0A19, readflag);//flag
	S5K4H7WriteMultiByte(21, 0x0A1A, Data_Write.data_WB, 1, 13);

	return TRUE;
}

BOOL COtpCB801C::ProgrammingLSC()
{
	if (m_LSCItem == 0)
	{
		// 		if (GT24C64_Programming(m_addrFlag.moduleInfo,Data_Write.data_Basic,sizeof(Data_Write.data_Basic)) != TRUE)
		// 		{
		// 			m_pInterface->AddLog(_T("Basic Info Write Fail!"),COLOR_RED);		
		// 			return FALSE;
		// 		}
	}
	else if (m_LSCItem == 1)
	{
		// 		if (ZC533_Programming(m_addrFlag.moduleInfo,Data_Write.data_Basic,sizeof(Data_Write.data_Basic)) != TRUE)
		// 		{
		// 			m_pInterface->AddLog(_T("Basic Info Write Fail!"),COLOR_RED);		
		// 			return FALSE;
		// 		}
	}
	else if (m_LSCItem == 2)
	{
		if (m_sensoritem == 4)
		{
			if (m_LscGroup > m_grouplimit)
			{
				m_pInterface->AddLog(_T("OTP只允许烧录第一组!"), COLOR_RED);
				return FALSE;
			}
			Programming_4H7LSC();
		}

	}

	return TRUE;
}

BOOL COtpCB801C::Programming_4H7LSC()
{
	USHORT checksum;
	BYTE Read1, Read2, readflag;
	BYTE ReadData = 0;
	S5K4H7ReadOTPByte(0, 0x0A3D, &ReadData, 1);//LSC flag自动烧录
	Read1 = (ReadData >> 0) & 0x01;
	Read2 = (ReadData >> 1) & 0x01;
	if (m_LscGroup == 1)
	{
		if (Read1 != 0)
		{
			m_pInterface->AddLog(_T("OTP第一组内已有数据!"), COLOR_RED);
			return FALSE;
		}
		checksum = 0x0A04;
		for (int i = 0; i < 5; i++)
		{
			S5K4H7WriteMultiByte(1 + i, 0x0A04, Data_Write.data_Lenc, 64 * i, 64 * i + 64);
		}
		S5K4H7WriteMultiByte(6, 0x0A04, Data_Write.data_Lenc, 320, 360);
	}
	else if (m_LscGroup == 2)
	{
		if (Read2 != 0)
		{
			m_pInterface->AddLog(_T("OTP第二组内已有数据!"), COLOR_RED);
			return FALSE;
		}
		checksum = 0x0A05;

		S5K4H7WriteMultiByte(6, 0x0A2C, Data_Write.data_Lenc, 0, 24);
		for (int i = 0; i < 5; i++)
		{
			S5K4H7WriteMultiByte(7 + i, 0x0A04, Data_Write.data_Lenc, 64 * i + 24, 64 * i + 88);
		}
		S5K4H7WriteMultiByte(12, 0x0A04, Data_Write.data_Lenc, 344, 360);
	}
	S5K4H7WriteSingleByte(21, checksum, LscWriteChecksum);
	return TRUE;
}

BOOL COtpCB801C::ProgrammingAF()
{
// 	if (m_LSCItem == 0)
// 	{
// 		if (GT24C64_Programming(m_addrFlag.af,Data_Write.data_AF,sizeof(Data_Write.data_AF)) != TRUE)
// 		{
// 			m_pInterface->AddLog(_T("AF Info Write Fail!"),COLOR_RED);		
// 			return FALSE;
// 		}
// 	}
// 	else if (m_LSCItem == 1)
// 	{
// 		if (ZC533_Programming(m_addrFlag.af,Data_Write.data_AF,sizeof(Data_Write.data_AF)) != TRUE)
// 		{
// 			m_pInterface->AddLog(_T("AF Info Write Fail!"),COLOR_RED);		
// 			return FALSE;
// 		}
// 	}
// 	else if (m_LSCItem == 2)
// 	{
// 		if (m_sensoritem == 2)
// 		{
// 			Programming_4H7AF();
// 		}
// 	}

	return TRUE;
}

BOOL COtpCB801C::Programming_4H7AF()
{
	int i = 0;

// 	S5K4H7_WriteData[52] = Data_Write.data_AF[0];
// 	for (i=0; i<SIZE_AF_GROUP; i++)
// 	{
// 		S5K4H7_WriteData[i+53+(m_LscGroup-1)*OTPGAP_AF] = Data_Write.data_AF[i+1];
// 	}
// 
// 	if (!S5K4H7WriteOTPpage(15,m_addrFlag.moduleInfo,S5K4H7_WriteData,64)) return FALSE;

	return TRUE;
}

BOOL COtpCB801C::ProgrammingPDAF()
{
	BOOL bRet = TRUE;

	

	return TRUE;
}

BOOL COtpCB801C::S5K4H7_OTPVerify(void)
{
	if (!m_dummyburn)
	{
		if (!m_sensorotpisempty)
		{
			if (!S5K4H7_CheckOTPReadWrite())		return FALSE;
		}
		else
		{
			if (!S5K4H7_CheckOTP())		return FALSE;
		}
	}
	else 
	{
		m_pInterface->AddLog(_T("This is Dummy Burn!"),COLOR_RED);
	}
   return TRUE;
}

BOOL COtpCB801C::S5K4H7_CheckOTP()
{
	BYTE otpdata[64] = {0};
	BYTE readBasicflag = 0;
	BYTE readAFflag = 0;
	S5K4H7ReadOTPByte(21,0x0A04, otpdata,64);
	if (otpdata[2] == 0 || otpdata[21] == 0)
	{
		m_pInterface->AddLog(_T("OTP内Basic Flag或者AWB Flag为空!"),COLOR_RED);
		return FALSE;
	}
	else if (otpdata[2] == 0x01)
	{
		readBasicflag = 1;
	}
	else if (otpdata[2] == 0x13)
	{
		readBasicflag = 2;
	}
	else
	{
		m_pInterface->AddLog(_T("Basic Flag异常!"),COLOR_RED);
		return FALSE;
	}

	if (otpdata[21] == 0x01)
	{
		readAFflag = 1;
	}
	else if (otpdata[21] == 0x13)
	{
		readAFflag = 2;
	}
	else
	{
		m_pInterface->AddLog(_T("AWB Flag异常!"),COLOR_RED);
		return FALSE;
	}

	if (otpdata[3 + (readBasicflag - 1)*SIZE_BASIC_GROUP] != BASIC_MID)
	{
		m_pInterface->AddLog(_T("MID异常!"),COLOR_RED);
		return FALSE;
	}
	if (otpdata[7 + (readBasicflag - 1)*SIZE_BASIC_GROUP] != BASIC_LENSID)
	{
		m_pInterface->AddLog(_T("LensID异常!"),COLOR_RED);
		return FALSE;
	}


	m_pInterface->AddLog(_T("OTP内已有数据检查Pass!"),COLOR_BLUE);
	return TRUE;
}

BOOL COtpCB801C::S5K4H7_CheckOTPReadWrite(void)
{
	m_pInterface->AddLog(_T("Check OTP Data Start!"));

	if (m_LSCItem == 0)
	{	
		
	}

	if (m_LSCItem == 1)
	{
		
	}

	if (m_LSCItem == 2)
	{
		if (m_sensoritem == 4)
		{
			if (!Sensor4H7_CheckReadWrite()) return FALSE; 
			if (!Sensor4H7_CheckLSCReadWrite()) return FALSE;
			m_pInterface->AddLog(_T("Check OTP Data Success!"),COLOR_BLUE);	
		}
	}

	m_pInterface->AddLog(_T("Check OTP Data OK!"),COLOR_BLUE);
	return TRUE;
}

BOOL COtpCB801C::Sensor4H7_CheckReadWrite()
{
	BYTE ReadData[64] = {0};
	if (!S5K4H7ReadOTPpage(21,ReadData)) return FALSE;

	int i = 0;

	if (ReadData[2] != Data_Write.data_Basic[0])
	{
		m_pInterface->AddLog(_T("Basic Flag读写对比失败！"),COLOR_RED);
		return FALSE;
	}
	for (i=0;i<SIZE_BASIC_GROUP; i++)
	{
		if (ReadData[i + 3 + (m_LscGroup - 1)*SIZE_BASIC_GROUP] != Data_Write.data_Basic[i + 1])
		{
			m_pInterface->AddLog(_T("Basic Info读写对比失败！"),COLOR_RED);
			return FALSE;
		}
	}
	if (m_EnAWB)
	{
		if (ReadData[21] != Data_Write.data_WB[0])
		{
			m_pInterface->AddLog(_T("AWB Flag读写对比失败！"), COLOR_RED);
			return FALSE;
		}
		for (i=0; i<SIZE_WB_GROUP; i++)
		{
			if (ReadData[i + 22 + (m_LscGroup - 1)*SIZE_WB_GROUP] != Data_Write.data_WB[i+1])
			{
				m_pInterface->AddLog(_T("AWB Info读写对比失败！"),COLOR_RED);
				return FALSE;
			}
		}
	}

	if (m_EnAF)
	{
// 		if (ReadData[52] != Data_Write.data_AF[0])
// 		{
// 			m_pInterface->AddLog(_T("AF Flag读写对比失败！"),COLOR_RED);
// 			return FALSE;
// 		}
// 		for (i=0; i<SIZE_AF_GROUP; i++)
// 		{
// 			if (ReadData[i+53+(m_LscGroup-1)*OTPGAP_AF] != Data_Write.data_AF[i+1])
// 			{
// 				m_pInterface->AddLog(_T("AF Info读写对比失败！"),COLOR_RED);
// 				return FALSE;
// 			}
// 		}
	}
	return TRUE;
}

BOOL COtpCB801C::Sensor4H7_CheckLSCReadWrite()
{
	int i = 0;
	BYTE checksum;
	if (m_LscGroup == 1)
	{
		
		if (!S5K4H7ReadOTPByte(1, 0x0A04, S5K4H7_LSCReadData, 64)) return FALSE;
		msleep(10);
		if (!S5K4H7ReadOTPByte(2, 0x0A04, S5K4H7_LSCReadData + 64, 64)) return FALSE;
		msleep(10);
		if (!S5K4H7ReadOTPByte(3, 0x0A04, S5K4H7_LSCReadData + 64 * 2, 64)) return FALSE;
		msleep(10);
		if (!S5K4H7ReadOTPByte(4, 0x0A04, S5K4H7_LSCReadData + 64 * 3, 64)) return FALSE;
		msleep(10);
		if (!S5K4H7ReadOTPByte(5, 0x0A04, S5K4H7_LSCReadData + 64 * 4, 64)) return FALSE;
		msleep(10);
		if (!S5K4H7ReadOTPByte(6, 0x0A04, S5K4H7_LSCReadData + 64 * 5, 40)) return FALSE;
		msleep(10);

		for (i= 0; i<360; i++)
		{
			if (S5K4H7_LSCReadData[i] != WriteLencData[i])
			{
				m_pInterface->AddLog(_T("LSC Info读写对比失败！"));
				return 0;
			}
		}
		if (!S5K4H7ReadOTPByte(1, 0x0A04, &checksum, 1))return FALSE;
		if (checksum != LscWriteChecksum)
		{
			m_pInterface->AddLog(_T("LSC Checksum读写对比失败！"));
			return 0;
		}
	}
	if (m_LscGroup == 2)
	{
		if (!S5K4H7ReadOTPByte(6, 0x0A2C, S5K4H7_LSCReadData, 24)) return FALSE;
		msleep(10);
		if (!S5K4H7ReadOTPByte(7, 0x0A04, S5K4H7_LSCReadData + 24, 64)) return FALSE;
		msleep(10);
		if (!S5K4H7ReadOTPByte(8, 0x0A04, S5K4H7_LSCReadData + 24 + 64, 64)) return FALSE;
		msleep(10);
		if (!S5K4H7ReadOTPByte(9, 0x0A04, S5K4H7_LSCReadData + 24 + 64 * 2, 64)) return FALSE;
		msleep(10);
		if (!S5K4H7ReadOTPByte(10, 0x0A04, S5K4H7_LSCReadData + 24 + 64 * 3, 64)) return FALSE;
		msleep(10);
		if (!S5K4H7ReadOTPByte(11, 0x0A04, S5K4H7_LSCReadData + 24 + 64 * 4, 64)) return FALSE;
		msleep(10);
		if (!S5K4H7ReadOTPByte(12, 0x0A04, S5K4H7_LSCReadData + 24 + 64 * 5, 16)) return FALSE;
		msleep(10);
		for (i = 0; i<360; i++)
		{
			if (S5K4H7_LSCReadData[i] != S5K4H7_LSCWriteData[i])
			{
				m_pInterface->AddLog(_T("LSC Info读写对比失败！"));
				return 0;
			}
		}
		if (!S5K4H7ReadOTPByte(1, 0x0A05, &checksum, 1))return FALSE;
		if (checksum != LscWriteChecksum)
		{
			m_pInterface->AddLog(_T("LSC Checksum读写对比失败！"));
			return 0;
		}
	}
	return TRUE;
}

BOOL COtpCB801C::GetOTPRGBG()
{
	if (!m_sensorotpisempty)
	{
		m_wbCurrent.rg = Data_Write.data_WB[1]<<8|Data_Write.data_WB[2];
		m_wbCurrent.bg = Data_Write.data_WB[3]<<8|Data_Write.data_WB[4];
	}
	else
	{
		if (m_sensoritem == 2)
		{
			BYTE readwbflag = 0;
			S5K4H7ReadOTPByte(21, 0x0A19, &readwbflag, 1);
			if (readwbflag == 0x01)
			{
				readwbflag = 1;
			}
			else if(readwbflag == 0x13)
			{
				readwbflag = 2;
			}

			BYTE readwb[4] = {0};
			S5K4H7ReadOTPByte(21, 0x0A1A + (readwbflag - 1)*SIZE_WB_GROUP, readwb, 4);
			m_wbCurrent.rg = readwb[0]<<8|readwb[1];
			m_wbCurrent.bg = readwb[2]<<8|readwb[3];
		}
	}
	return TRUE;
}

BOOL COtpCB801C::S5K4H7_VerifyLSC()
{
	m_pInterface->AddLog(_T("Verify LSC:"));
	if (m_sensoritem == 2)
	{
		m_pInterface->RestartCamera(1);
		SaveExp = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllExp"), 1);
		m_pInterface->GetImageSensor()->WriteExp(SaveExp);
		SaveGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"RecordExp",_T("ExpGain"), 1);
		m_pInterface->GetImageSensor()->WriteAnalogGain(SaveGain);
		
		i2c_write_byte(0x3400, 0x00);
		i2c_write_byte(0x0B00, 0x01);
		i2c_write_byte(0x0100, 0x01);
		msleep(100);

		m_pInterface->AddLog(_T("AutoLoading LSC OK!"),COLOR_BLUE);
	}

	return TRUE;
}

BOOL COtpCB801C::S5K4H7_VerifyAWB()
{
	if (!GetAWBInfo(&m_wbCurrent))		return FALSE;
	if (!S5K4H7_CheckAWB(m_wbCurrent,2))		return FALSE;
	return TRUE;
}