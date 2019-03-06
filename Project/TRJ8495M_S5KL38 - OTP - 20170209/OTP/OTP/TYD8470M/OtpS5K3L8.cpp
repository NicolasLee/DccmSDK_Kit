#include "stdafx.h"
#include "OtpS5K3L8.h"
#include "..//OTP_SensorSetting.h"

COtpS5K3L8::COtpS5K3L8(void)
{
	ZeroMemory(OTP_Data_Write.AFInfo,sizeof(OTP_Data_Write.AFInfo));
	ZeroMemory(OTP_Data_Write.BaseInfo,sizeof(OTP_Data_Write.BaseInfo));
	ZeroMemory(S5K3L8_LSCWriteData,sizeof(S5K3L8_LSCWriteData));
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

	SetOtpName(_T("TYD8470M"));

	PreSubclassOtp();
}


COtpS5K3L8::~COtpS5K3L8(void)
{

}

void COtpS5K3L8::PreSubclassOtp()
{
	tagOtpCtrl* control = GetControlParams();
// 	control->manualReInit = FALSE;
// 	control->enProgram = TRUE;
// 	control->enForceCalibrate = FALSE;
// 	control->enSaveRawBMP = FALSE;
// 	control->enSaveCorrectBMP = FALSE;
// 	control->enShowVideoAfterOtp = FALSE;
// 	control->enLenc = FALSE;
// 	control->enWB = TRUE;
// 	control->enCheckAF = TRUE;
// 	control->enerase = FALSE;//应拉接口到桌面上
// 	control->enMTKLSC = TRUE;//应拉接口到桌面上
	control->sensorob = SENSOROB;
	control->quallsccompress = QUALLSCCOMPRESS;

	OtpIDStruct *ID = GetIDGroup();	
	ID->house = 0x42;
	ID->aftype = 0;
	ID->sensor = 0;
	ID->lens = 0x41;
	ID->vcm = 0x40;
	ID->driverIC = 0x11;
	ID->FNO = 0;

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
	wbCtrl->factorAmplify = 1024;//1024;

	WBGainRegStruct* wbReg = GetWBGainRegister();
	wbReg->addrGr = GAIN_ADDR_GR;
	wbReg->addrR = GAIN_ADDR_R;
	wbReg->addrB = GAIN_ADDR_B;
	wbReg->addrGb = GAIN_ADDR_GB;
	wbReg->dataDefaultGain = GAIN_DEFAULT_VAL;	
// 	wbReg->addrGrHigh = S5K3L8_ADDR_GAIN_GR_H;
// 	wbReg->addrGrLow  = S5K3L8_ADDR_GAIN_GR_L;
// 	wbReg->addrGbHigh = S5K3L8_ADDR_GAIN_GB_H;
// 	wbReg->addrGbLow  = S5K3L8_ADDR_GAIN_GB_L;
// 	wbReg->addrRHigh  = S5K3L8_ADDR_GAIN_R_H;
// 	wbReg->addrRLow   = S5K3L8_ADDR_GAIN_R_L;
// 	wbReg->addrBHigh  = S5K3L8_ADDR_GAIN_B_H;
// 	wbReg->addrBLow   = S5K3L8_ADDR_GAIN_B_L;


	tagAwbRatio *wbGolden = GetWBGoldenValue();		
	wbGolden->rg  = m_goldenrg;
	wbGolden->bg  = m_goldenbg;

	LencOtpControlStruct *lencCtrl = GetLencControlParams();
// 	lencCtrl->inputTarget = S5K3L8_VAL_LSC_TARGET;
// 	lencCtrl->inputLow = S5K3L8_VAL_LSC_LOW;
// 	lencCtrl->inputHigh = S5K3L8_VAL_LSC_HIGH;
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
// 	lencReg->addrCtrl = S5K3L8_ADDR_LSC_CTRL;
// 	lencReg->lencEnable = S5K3L8_ADDR_LSC_EN;
// 	lencReg->lencDisable = S5K3L8_ADDR_LSC_DIS;
//	lencReg->addrDataStart = REG_INVALID;
//	lencReg->dataSize = S5K3L8_OTP_LSC_SIZE;

	ExpControlStruct *expCtrl = GetExpControlParams();
	expCtrl->type = EXP_LINEAR;
	expCtrl->roiCalcMode = ROI_CALC_10;
	expCtrl->filterChannel = EXP_CHANNEL_G;
// 	expCtrl->target = S5K3L8_VAL_EXP_TARGET;
// 	expCtrl->lowLimit = S5K3L8_VAL_EXP_LOW;
// 	expCtrl->highLimit = S5K3L8_VAL_EXP_HIGH;
	expCtrl->effectFrame = 3;
	expCtrl->stableFrame = 3;

	ExpRegisterStruct *expReg = GetExpRegister();
	expReg->mask = 0x00FFFF;
	expReg->addrHigh = REG_INVALID;
// 	expReg->addrMid = S5K3L8_ADDR_EXP_H;
// 	expReg->addrLow = S5K3L8_ADDR_EXP_L;

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

void COtpS5K3L8::SetOtpSensorParams()
{
	OtpSensorStruct* params = GetOtpSensorParams();

	params->name = _T("S5K3L8_MIPI");
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
//	params->otpInit.params = S5K3L8_OTP_Settings;
//	params->otpInit.paramSize = sizeofS5K3L8_OTP_Settings;

	params->verifyInit.width  = 4208;
	params->verifyInit.height = 3120;
	params->verifyInit.format = IMAGE_FORMAT_RAW10;
	params->verifyInit.sequence = IMAGE_SEQUENCE_GBRG;
	params->verifyInit.mhzMCLK = 24;
// 	params->verifyInit.params = S5K3L8_OTP_Settings;
// 	params->verifyInit.paramSize = sizeofS5K3L8_OTP_Settings;	
}


BOOL COtpS5K3L8::EraseAllData()
{
// 	int i = 0;
// 	BYTE* eraseData = new BYTE[32]();
// 	memset(eraseData, 0xff, 32*sizeof(BYTE));//注意初始EEPROM是00还是ff
// 
// 	for (i=0; i<110; i++)
// 	{
// 		if (!GT24C64_Programming(0x0000+i*32,eraseData,32))//erase all
// 		{
// 			m_pInterface->AddLog(_T("OTP数据擦除失败!"),COLOR_RED);
// 			SAFE_DELETE_ARRAY(eraseData);
// 			return 0;
// 		}
// 	}
// 
// 	SAFE_DELETE_ARRAY(eraseData);
	DW9763_All_Erase();
	m_pInterface->AddLog(_T("OTP所有数据已全部擦除!"));
	return TRUE;
}

void COtpS5K3L8::SetWBDefaultGain(void)
{
	USHORT gainDefault = GAIN_DEFAULT_VAL;
	S5K3L8_SetWBGain(gainDefault, gainDefault, gainDefault, gainDefault);
	return;
}

void COtpS5K3L8::S5K3L8_SetWBGain(USHORT gainGr, USHORT gainR, USHORT gainB, USHORT gainGb)
{
	i2c_write( 0x6028, 0x4000);
	i2c_write( 0x602a, 0x0100);
	i2c_write_byte( 0x6f12, 0x00);// Stream off
	Sleep(100);

	i2c_write(GAIN_ADDR_GR,gainGr);
	i2c_write(GAIN_ADDR_R,gainR);
	i2c_write(GAIN_ADDR_B,gainB);
	i2c_write(GAIN_ADDR_GB,gainGb);

	i2c_write( 0x6028, 0x4000);
	i2c_write( 0x602a, 0x0100);
	i2c_write_byte( 0x6f12, 0x01 );// Stream on

	return;
}

BOOL COtpS5K3L8::StartProgram()
{
	return TRUE;
}

BOOL COtpS5K3L8::WriteOTP()
{
	S5K3L8_GetBasicData();
	S5K3L8_GetAFData();
	CreateBurningData();
	if (!m_dummyburn)
	{
		if (!S5K3L8_OtpWrite())//
		{return FALSE;}
	}
	else
	{
//		Programming_3L8LSC();
		SaveOTPData();
		m_pInterface->AddLog(_T("This is Dummy Burn!"),COLOR_RED);
	}
	return TRUE;
}

void COtpS5K3L8::SaveOTPData()
{
	m_pInterface->AddLog(_T("Read OTP Start!"));
	///////////////////////////////Read OTP////////////////////////////////////
// 	int valSize = 481;
// 	BYTE* pVal = new BYTE[valSize]();
// 	int i = 0;
// 
// 	if (m_LscGroup == 1)
// 	{
// 		for (i=0; i<SIZE_BASIC_GROUP+1; i++)
// 		{
// 			pVal[i] = Data_Write.data_Basic[i];
// 		}
// 		for (i=0; i<SIZE_WB_GROUP; i++)
// 		{
// 			pVal[i+9] = Data_Write.data_WB[i];
// 		}
// 		for (i=0; i<SIZE_AF_GROUP+1; i++)
// 		{
// 			pVal[i+16] = Data_Write.data_AF[i];
// 		}
// 		for (i=0; i<2; i++)
// 		{
// 			pVal[i+22] = Data_Write.data_Lenc[i];
// 		}
// 
// 		for (i=0; i<417; i++)
// 		{
// 			pVal[i+64] = S5K3L8_LSCWriteData[i+31];
// 		}
// 
// 	}
// 	else if (m_LscGroup == 2)
// 	{
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
// 
// 		for (i=0; i<417; i++)
// 		{
// 			pVal[i+64] = S5K3L8_LSCWriteData[i+62];
// 		}       
// 	}
	int valSize = 2112;
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

// 	num.Format("Page: 0x%x\n", 1);
// 	temp += num;
	for(i=0; i<valSize; i++)
	{
		num.Format("%04d\t0x%04x,0x%02x\n", i,0x0000+i,(unsigned char)pVal[i]);
		temp += num;
	}
// 	num.Format("LSCData:\n");
// 	temp += num;
// 	for(i=64; i<417+64; i++)
// 	{
// 		num.Format("%04d\t0x%02x\n", i,(unsigned char)pVal[i]);
// 		temp += num;
// 	}

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

BOOL COtpS5K3L8::WriteOTP(vector<double>& vData)
{
	S5K3L8_GetBasicData();
	S5K3L8_GetAFData();
	CreateBurningData();
	if (!m_dummyburn)
	{
		if (!S5K3L8_OtpWrite())//
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

void COtpS5K3L8::SaveOTPData(vector<double>& vData)
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
		for (i=0; i<2; i++)
		{
			pVal[i+22] = Data_Write.data_Lenc[i];
		}

		for (i=0; i<417; i++)
		{
			pVal[i+64] = S5K3L8_LSCWriteData[i+31];
		}

	}
	else if (m_LscGroup == 2)
	{
		for (i=0; i<SIZE_BASIC_GROUP+1; i++)
		{
			pVal[i+OTPGAP] = Data_Write.data_Basic[i];
		}
		for (i=0; i<SIZE_WB_GROUP; i++)
		{
			pVal[i+9+OTPGAP] = Data_Write.data_WB[i];
		}
		for (i=0; i<SIZE_AF_GROUP+1; i++)
		{
			pVal[i+16+OTPGAP] = Data_Write.data_AF[i];
		}
		for (i=0; i<2; i++)
		{
			pVal[i+22+OTPGAP] = Data_Write.data_Lenc[i];
		}

		for (i=0; i<417; i++)
		{
			pVal[i+64] = S5K3L8_LSCWriteData[i+62];
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

	num.Format("Shading Data After SRAM:\nB1:%.1f\nB2:%.1f\nB3:%.1f\nB4:%.1f\n",vData.at(0), vData.at(1), vData.at(2), vData.at(3));
	temp += num;

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

void COtpS5K3L8::CreateBurningData(void)
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
		for (i = 0; i < SIZE_LSC_GROUP-1; i++)
		{
			Data_Write.data_Lenc[i+1] = S5K3L8_LSCWriteData[i] = m_mtklscdata[i];
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
	}
	//////////////////////////////////////////////////////////////////////////
	S5K3L8_CalcAllCheckSum();
	return;
}

BOOL COtpS5K3L8::CheckOTP()
{
	if (!S5K3L8_OTPVerify())		return FALSE;

	return TRUE;
}

BOOL COtpS5K3L8::VerifyLSC()
{
	if (m_LSCItem == 2)
	{
		if (!S5K3L8_VerifyLSC())		return FALSE; //for sensor LSC
	}
	return TRUE;
}

BOOL COtpS5K3L8::VerifyPreWB()
{
	if (m_EnAWB)
	{
		if (!GetOTPRGBG())		return FALSE;
		S5K3L8_ApplyAWB(m_wbCurrent,m_goldenrg,m_goldenbg);
	}
	return TRUE;
}

BOOL COtpS5K3L8::VerifyAWB()
{
	if (m_EnAWB)
	{
		if (!S5K3L8_VerifyAWB())		return FALSE;
	}
	return TRUE;
}

// BOOL COtpS5K3L8::EndProgram()
// {
// 	return TRUE;
// }


BOOL COtpS5K3L8::GetAWB()
{
	if (m_EnAWB)
	{
		if (!S5K3L8_GetAWBData(OTP_Data_Write))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL COtpS5K3L8::S5K3L8_GetAWBData(S5K3L8OTPDataSpec &awbdata)
{
	m_pInterface ->AddLog(_T("Get AWB Data Start"));

	if (!S5K3L8_GetCurrentAWB(&m_wbCurrent))		return FALSE;

	awbdata.AWBInfo[0] = m_wbCurrent.rg&0xff;
	awbdata.AWBInfo[1] = (m_wbCurrent.rg>>8)&0x0f;
	awbdata.AWBInfo[2] = m_wbCurrent.bg&0xff;
	awbdata.AWBInfo[3] = (m_wbCurrent.bg>>8)&0x0f;
	awbdata.AWBInfo[4] = m_wbCurrent.gb_gr&0xff;
	awbdata.AWBInfo[5] = (m_wbCurrent.gb_gr>>8)&0x0f;

	awbdata.AWBInfo[6] = m_goldenrg&0xff;
	awbdata.AWBInfo[7] = (m_goldenrg>>8)&0x0f;
	awbdata.AWBInfo[8] = m_goldenbg&0xff;
	awbdata.AWBInfo[9] = (m_goldenbg>>8)&0x0f;
	awbdata.AWBInfo[10] = m_goldengg&0xff;
	awbdata.AWBInfo[11] = 	(m_goldengg>>8)&0x0f;

	awbdata.AWBInfo[12] = m_wbCurrent.r;
	awbdata.AWBInfo[13] = m_wbCurrent.gr;
	awbdata.AWBInfo[14] = m_wbCurrent.gb;
	awbdata.AWBInfo[15] = m_wbCurrent.b;

	awbdata.AWBInfo[16] = m_goldenr;
	awbdata.AWBInfo[17] = m_goldengr;
	awbdata.AWBInfo[18] = m_goldengb;
	awbdata.AWBInfo[19] = m_goldenb;

	m_pInterface ->AddLog(_T("Get AWB Data End"));
	S5K3L8_ApplyAWB(m_wbCurrent, m_goldenrg, m_goldenbg);
	return TRUE;
}

BOOL COtpS5K3L8::CheckOTPAWB()
{
	if (m_EnAWB)
	{
		m_pInterface ->AddLog(_T("Check Apply AWB Data Start"));
		if (!GetAWBInfo(&m_wbCurrent))		return FALSE;

		if (!S5K3L8_CheckAWB(m_wbCurrent,1))				return FALSE;

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

BOOL COtpS5K3L8::S5K3L8_GetCurrentAWB(tagAwbRatio* wb)
{
	if (!InitAWB())		return FALSE;

	if (!GetAWBInfo(&m_wbCurrent))		return FALSE;

	if (!S5K3L8_CheckAWB(m_wbCurrent,0))				return FALSE;

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

	Applyawbdata[0] = m_wbCurrent.r;
	Applyawbdata[1] = m_wbCurrent.gr;
	Applyawbdata[2] = m_wbCurrent.gb;
	Applyawbdata[3] = m_wbCurrent.b;
	Applyawbdata[4] = m_wbCurrent.rg;
	Applyawbdata[5] = m_wbCurrent.bg;
	Applyawbdata[6] = m_wbCurrent.gb_gr;

	return TRUE;
}

BOOL COtpS5K3L8::GetAWBInfo(tagAwbRatio* wb)
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

	//raw8
	wb->r = (UINT)(wb->r/4 + 0.5);
	wb->g = (UINT)(wb->g/4 + 0.5);
	wb->b = (UINT)(wb->b/4 + 0.5);
	wb->gb = (UINT)(wb->gb/4 + 0.5);
	wb->gr = (UINT)(wb->gr/4 + 0.5);

	//check...
	wb->rg		= (UINT)(1.0 * (wb->r-SENSOROB) / (wb->g-SENSOROB)*AWB_FACTOR + 0.5); 
	wb->bg		= (UINT)(1.0 * (wb->b-SENSOROB) / (wb->g-SENSOROB)*AWB_FACTOR + 0.5);
	wb->gb_gr	= (UINT)(1.0 * (wb->gb-SENSOROB)/ (wb->gr-SENSOROB)*AWB_FACTOR+ 0.5);

	m_szStr.Format(_T("R/G: %d B/G: %d"), wb->rg, wb->bg);
	m_pInterface->AddLog(m_szStr, COLOR_BLUE);
	return TRUE;
}

BOOL COtpS5K3L8::S5K3L8_CheckAWB(tagAwbRatio wb, int type)
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
		break;

	case 1:
		rg_diff = (float)(abs((int)(wb.rg-m_goldenrg))*1.0/m_goldenrg)*100;
		bg_diff = (float)(abs((int)(wb.bg-m_goldenbg))*1.0/m_goldenbg)*100;

		if (rg_diff>m_rgaindif || bg_diff > m_bgaindif)
		{
			m_pInterface ->AddLog(_T("Apply后wb gain diff超出范围"),COLOR_RED);
			return FALSE;
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
		break;

	default:
		return FALSE;
		break;
	}
	m_pInterface ->AddLog(_T("Check AWB Data Pass!"),COLOR_BLUE);
	return TRUE;
}

VOID COtpS5K3L8::S5K3L8_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG)
{
	USHORT r_ratio, b_ratio;

	r_ratio = 512 * (m_goldenrg) /(wb.rg);
	b_ratio = 512 * (m_goldenbg) /(wb.bg);

	if( !r_ratio || !b_ratio)
	{
		m_pInterface ->AddLog(_T("请先Get AWB!"),COLOR_RED);
		return;
	}
	USHORT R_GAIN;
	USHORT B_GAIN;
	USHORT Gr_GAIN;
	USHORT Gb_GAIN;
	USHORT G_GAIN;


	if(r_ratio >= 512 )
	{
		if(b_ratio>=512) 
		{
			R_GAIN = (USHORT)(GAIN_DEFAULT_VAL * r_ratio / 512);
			G_GAIN = GAIN_DEFAULT_VAL;	
			B_GAIN = (USHORT)(GAIN_DEFAULT_VAL * b_ratio / 512);
		}
		else
		{
			R_GAIN =  (USHORT)(GAIN_DEFAULT_VAL * r_ratio / b_ratio);
			G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio);
			B_GAIN = GAIN_DEFAULT_VAL;	
		}
	}
	else 			
	{
		if(b_ratio >= 512)
		{
			R_GAIN = GAIN_DEFAULT_VAL;	
			G_GAIN =(USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio);
			B_GAIN =(USHORT)(GAIN_DEFAULT_VAL *  b_ratio / r_ratio);

		} 
		else 
		{
			Gr_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio );
			Gb_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio );

			if(Gr_GAIN >= Gb_GAIN)
			{
				R_GAIN = GAIN_DEFAULT_VAL;
				G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio );
				B_GAIN = (USHORT)(GAIN_DEFAULT_VAL * b_ratio / r_ratio);
			} 
			else
			{
				R_GAIN =  (USHORT)(GAIN_DEFAULT_VAL * r_ratio / b_ratio );
				G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio );
				B_GAIN = GAIN_DEFAULT_VAL;
			}
		}	
	}
	i2c_write( 0x6028, 0x4000);
	i2c_write( 0x602a, 0x0100);
	i2c_write_byte( 0x6f12, 0x00);// Stream off
	Sleep(100);

	i2c_write(0x6028, 0x4000);	
	i2c_write(0x602a, 0x3058);
	i2c_write_byte(0x6f12, 0x01);

	i2c_write(0x602a, 0x0210);
	i2c_write(0x6f12, R_GAIN);

	i2c_write(0x602a, 0x0212);
	i2c_write(0x6f12, B_GAIN);

	i2c_write(0x602a, 0x020e);
	i2c_write(0x6f12, G_GAIN);

	i2c_write(0x602a, 0x0214);
	i2c_write(0x6f12, G_GAIN);

	i2c_write( 0x6028, 0x4000);
	i2c_write( 0x602a, 0x0100);
	i2c_write_byte( 0x6f12, 0x01 );// Stream on

	return;
}

void COtpS5K3L8::S5K3L8_GetBasicData(void)
{
//	m_pInterface ->AddLog(_T("Get Basic Data Start"));
	CTime time = CTime::GetCurrentTime();

	OTP_Data_Write.BaseInfo[0] = 0xFF;
	OTP_Data_Write.BaseInfo[1] = 0x00;
	OTP_Data_Write.BaseInfo[2] = 0x0B;
	OTP_Data_Write.BaseInfo[3] = 0x01;
	OTP_Data_Write.BaseInfo[4] = time.GetYear() - 2000;
	OTP_Data_Write.BaseInfo[5] = time.GetMonth();
	OTP_Data_Write.BaseInfo[6] = time.GetDay();
	OTP_Data_Write.BaseInfo[7] = BASIC_MID;
	OTP_Data_Write.BaseInfo[8] = BASIC_LENSID;
	OTP_Data_Write.BaseInfo[9] = BASIC_VCMID;
	OTP_Data_Write.BaseInfo[10] = BASIC_DRIVERICID;
	OTP_Data_Write.BaseInfo[11] = BASIC_EEPROMID;
	OTP_Data_Write.BaseInfo[12] = BASIC_COLORTEMP;
	OTP_Data_Write.BaseInfo[13] = BASIC_BITEN;

	for (int i=0; i<6; i++)
	{
		OTP_Data_Write.BaseInfo[14+i] = 0;
	}
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

void COtpS5K3L8::S5K3L8_GetAFData(void)
{
//	m_pInterface ->AddLog(_T("Get AF Data Start"));

	OTP_Data_Write.AFInfo[0] = m_infCode&0xff;
	OTP_Data_Write.AFInfo[1] = (m_infCode>>8)&0x0f;
	OTP_Data_Write.AFInfo[2] = m_macCode&0xff;
	OTP_Data_Write.AFInfo[3] = (m_macCode>>8)&0x0f;

	m_pInterface ->AddLog(_T("Get AF Data"));
	return;
}
void COtpS5K3L8::S5K3L8_CalcAllCheckSum(void)
{
	int i = 0;
	int sum = 0;

	// LSC
	if (m_LSCItem != 2)
	{
		sum = SumCalc(S5K3L8_LSCWriteData, SIZE_LSC_GROUP-1, 255,1);
		Data_Write.data_Lenc[SIZE_LSC_GROUP] = sum;
	}

	//AWB
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
	sum = SumCalc(OTP_Data_Write.AWBInfo,SIZE_WB_GROUP-1,255,1);
	Data_Write.data_WB[SIZE_WB_GROUP] = sum;

	//Basic
	sum = SumCalc(OTP_Data_Write.BaseInfo,SIZE_BASIC_GROUP-1,255,1);
	Data_Write.data_Basic[SIZE_BASIC_GROUP] = sum;

	//AF	
	sum = SumCalc(OTP_Data_Write.AFInfo,SIZE_AF_GROUP-1,255,1);
	Data_Write.data_AF[SIZE_AF_GROUP] = sum;

	if (m_EnPDAF)
	{
		//PDAF1
		sum = SumCalc(OTP_Data_Write.PDAF1,SIZE_PDAF_PROC1,255,1);
		proc1sum = sum;
	}

	return;
}


BOOL COtpS5K3L8::S5K3L8_OtpWrite(void)
{
	////////////////////////////////////Check Sensor OTP Empty/////////////////////////////////
	if (m_LSCItem == 2)
	{
		if (m_sensoritem == 0)
		{ 
			BYTE tmpdata[64] = {0};
			if (m_LscGroup != 1)
			{
				S5K3L8_ReadOTPPage(26,tmpdata);
				if ( (tmpdata[0] == 0) && (tmpdata[1] == 0)&& (tmpdata[2] == 0) && (tmpdata[3] == 0) && (tmpdata[4] == 0))
				{
					m_pInterface->AddLog(_T("第一组LSC数据为空，不可烧录第二组!"),COLOR_RED);		
					return FALSE;
				}
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
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

BOOL COtpS5K3L8::ProgrammingBasic()
{
	if (m_LSCItem == 0)
	{
		if (DW9763_Programming(m_addrFlag.moduleInfo,Data_Write.data_Basic,sizeof(Data_Write.data_Basic)) != TRUE)
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

	Sleep(10);

	return TRUE;
}

void COtpS5K3L8::Programming_3L8Basic()
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

BOOL COtpS5K3L8::ProgrammingAWB()
{
	if (m_LSCItem != 2)
	{
		if (DW9763_Programming(m_addrFlag.awb,Data_Write.data_WB,sizeof(Data_Write.data_WB)) != TRUE)
		{
			m_pInterface->AddLog(_T("AWB Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
		Sleep(10);
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

void COtpS5K3L8::Programming_3L8AWB()
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

BOOL COtpS5K3L8::ProgrammingLSC()
{
	if (m_LSCItem != 2)
	{
		if (DW9763_Programming(m_addrFlag.lsc,Data_Write.data_Lenc,sizeof(Data_Write.data_Lenc)) != TRUE)
		{
			m_pInterface->AddLog(_T("LSC Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
		Sleep(10);
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

BOOL COtpS5K3L8::Programming_3L8LSC()
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
	CString strWorkPath  = GetModulePath()+"\\3L8";
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
					S5K3L8_LSCWriteData[k] = ParaList[i];
					k++;
				}
			}
		}

	}
	file.Close();

	//calc lsc checksum
	S5K3L8_LSCchecksum=0;
	if(m_LscGroup==1)
	{
		for (i=0;i<417;i++)
		{
			S5K3L8_LSCchecksum+=S5K3L8_LSCWriteData[i+31];
		}
		S5K3L8_LSCchecksum=S5K3L8_LSCchecksum%255+1;
	}
	if (m_LscGroup==2)
	{
		for (i=0;i<417;i++)
		{
			S5K3L8_LSCchecksum+=S5K3L8_LSCWriteData[i+62];
		}
		S5K3L8_LSCchecksum=S5K3L8_LSCchecksum%255+1;
	}

	S5K3L8_WriteLSCFlagSum();

	return TRUE;
}

void COtpS5K3L8::S5K3L8_WriteLSCFlagSum()
{
	Data_Write.data_Lenc[0] = FLAG_VALID;
	Data_Write.data_Lenc[1] = S5K3L8_LSCchecksum;

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

BOOL COtpS5K3L8::ProgrammingAF()
{
	if (m_LSCItem != 2)
	{
		if (DW9763_Programming(m_addrFlag.af,Data_Write.data_AF,sizeof(Data_Write.data_AF)) != TRUE)
		{
			m_pInterface->AddLog(_T("AF Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
		Sleep(10);
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

void COtpS5K3L8::Programming_3L8AF()
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

BOOL COtpS5K3L8::ProgrammingPDAF()
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
	}				

	//Qual
	return bRet;
}

BOOL COtpS5K3L8::S5K3L8_OTPVerify(void)
{
	if (!m_dummyburn)//!m_dummyburn
	{
		if (!S5K3L8_CheckOTPReadWrite())		return FALSE;
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

BOOL COtpS5K3L8::S5K3L8_CheckOTPReadWrite(void)
{
	m_pInterface->AddLog(_T("Check OTP Data Start!"));

	if (m_LSCItem == 0)
	{	
			if (DW9763_Read(m_addrFlag.moduleInfo, Data_Read.data_Basic,sizeof(Data_Read.data_Basic)) != TRUE)
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

		if (DW9763_Read(m_addrFlag.awb, Data_Read.data_WB,sizeof(Data_Read.data_WB)) != TRUE)
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

		if (DW9763_Read(m_addrFlag.lsc, Data_Read.data_Lenc,sizeof(Data_Read.data_Lenc)) != TRUE)
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

		if (DW9763_Read(m_addrFlag.af, Data_Read.data_AF,sizeof(Data_Read.data_AF)) != TRUE)
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

		BYTE *read_flag = new BYTE[1];
		BYTE *flag_check = new BYTE[1];
		flag_check[0] = (BYTE)m_flagValid;
		BYTE *Read_Sum = new BYTE[1];
		DW9763_Read(m_addrSum.pdaf, Read_Sum, 1);
		BYTE *Write_Sum = new BYTE[1];
		Write_Sum[0] = (BYTE)proc1sum;
		BOOL result = TRUE;
		/////////////////////////////////PDAF//////////////////////////////////////
		do 
		{
			if (DW9763_Read(m_addrFlag.pdaf, read_flag, 1 ) != TRUE)
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

				if (DW9763_Read(m_addrFlag.pdaf+1, OTP_Data_Read.PDAF1, sizeof (OTP_Data_Read.PDAF1)) != TRUE)
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
		if (result)
		{
			m_pInterface->AddLog(_T("Check OTP Data Success!"),COLOR_BLUE);	
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

BOOL COtpS5K3L8::Sensor3L8_CheckReadWrite()
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

BOOL COtpS5K3L8::Sensor3L8_CheckLSCReadWrite()
{
	int i = 0;
	if (m_LscGroup == 1)
	{
		S5K3L8_ReadOTPPage(0x19,S5K3L8_LSCReadData);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1A,S5K3L8_LSCReadData+64);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1B,S5K3L8_LSCReadData+128);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1C,S5K3L8_LSCReadData+192);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1D,S5K3L8_LSCReadData+256);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1E,S5K3L8_LSCReadData+320);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1F,S5K3L8_LSCReadData+384);
		Sleep(100);	
		for (i=31;i<(417+31);i++)
		{	
			if (S5K3L8_LSCReadData[i] != S5K3L8_LSCWriteData[i])
			{
				m_pInterface->AddLog(_T("LSC Info读写对比失败！"));
				return 0;
			}
		}
	}

	if (m_LscGroup == 2)
	{
		S5K3L8_ReadOTPPage(0x12,S5K3L8_LSCReadData);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x13,S5K3L8_LSCReadData+64);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x14,S5K3L8_LSCReadData+128);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x15,S5K3L8_LSCReadData+192);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x16,S5K3L8_LSCReadData+256);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x17,S5K3L8_LSCReadData+320);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x18,S5K3L8_LSCReadData+384);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x19,S5K3L8_LSCReadData+448);
		Sleep(100);
		for (i=62;i<(417+62);i++)
		{	
			if (S5K3L8_LSCReadData[i] != S5K3L8_LSCWriteData[i])
			{
				m_pInterface->AddLog(_T("LSC Info读写对比失败！"));
				return 0;
			}
		}
	}
	return TRUE;
}

BOOL COtpS5K3L8::GetOTPRGBG()
{
	m_wbCurrent.rg = Data_Write.data_WB[2]<<8|Data_Write.data_WB[1];
	m_wbCurrent.bg = Data_Write.data_WB[4]<<8|Data_Write.data_WB[3];
	return TRUE;
}

BOOL COtpS5K3L8::S5K3L8_VerifyLSC()
{
	m_pInterface->AddLog(_T("Verify LSC:"));
	if (m_sensoritem == 0)
	{
		m_pInterface->RestartCamera(1);
		SaveExp = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllExp"), 1);
		m_pInterface->GetImageSensor()->WriteExp(SaveExp);
		SaveGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllGain"), 1);
		m_pInterface->GetImageSensor()->WriteAnalogGain(SaveGain);

		i2c_write(0x6028, 0x2000);
		i2c_write(0x602a, 0x0cf4);
		i2c_write_byte(0x6f12,0x01);
		Sleep(100);

		i2c_write(0x6028, 0x4000);
		i2c_write(0x602a, 0x0b00);
		i2c_write_byte(0x6f12, 0x01);
		Sleep(100);

		i2c_write(0x602A, 0x0100);
		i2c_write_byte(0x6F12, 0x01);
		Sleep(100);
		i2c_write(0x602a, 0x305A);
		i2c_write_byte(0x6f12, 0x08);
		Sleep(100);	
		m_pInterface->AddLog(_T("AutoLoading LSC OK!"),COLOR_BLUE);
	}

	return TRUE;
}

BOOL COtpS5K3L8::S5K3L8_VerifyAWB()
{
	if (!GetAWBInfo(&m_wbCurrent))		return FALSE;
	if (!S5K3L8_CheckAWB(m_wbCurrent,2))		return FALSE;
	return TRUE;
}