#include "stdafx.h"
#include "Otp_HLT7012.h"
#include "..\..\OTP_SensorSetting.h"

COtpHLT7012::COtpHLT7012(void)
{
	ZeroMemory(OTP_Data_Write.AFInfo,sizeof(OTP_Data_Write.AFInfo));
	ZeroMemory(OTP_Data_Write.BaseInfo,sizeof(OTP_Data_Write.BaseInfo));
	ZeroMemory(HLT7012_LSCWriteData,sizeof(HLT7012_LSCWriteData));
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
	m_addrFlag.l3pdaf			    = REG_L3PDAF_FLAG;
	m_addrFlag.l3pdaf1          = REG_L3PDAF_PROC1_FLAG;
	m_addrFlag.l3pdaf2				= REG_L3PDAF_PROC2_FLAG;  

	m_addrSum.moduleInfo	= REG_BASIC_CHECKSUM;
	m_addrSum.awb				= REG_WB_CHECKSUM;
	m_addrSum.lsc					= REG_LSC_CHECKSUM;
	m_addrSum.af					= REG_AF_CHECKSUM;
	m_addrSum.pdaf				= REG_PDAF_SUMADDR;
	m_addrSum.pdaf1				= REG_PDAF_PROC1_SUMADDR;
	m_addrSum.pdaf2			= REG_PDAF_PROC2_SUMADDR;
	m_addrSum.l3pdaf			    = REG_L3PDAF_FLAG;
	m_addrSum.l3pdaf1          = REG_L3PDAF_PROC1_SUMADDR;
	m_addrSum.l3pdaf2				= REG_L3PDAF_PROC2_SUMADDR; 
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


COtpHLT7012::~COtpHLT7012(void)
{

}

void COtpHLT7012::PreSubclassOtp()
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
// 	control->enerase = FALSE;//Ӧ���ӿڵ������� 
// 	control->enMTKLSC = TRUE;//Ӧ���ӿڵ������� 
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
	wbCtrl->roiCalcMode = ROI_CALC_10;//���������ӿ�
	wbCtrl->factorAmplify = 1024;//1024;

	WBGainRegStruct* wbReg = GetWBGainRegister();
	wbReg->addrGr = GAIN_ADDR_GR;
	wbReg->addrR = GAIN_ADDR_R;
	wbReg->addrB = GAIN_ADDR_B;
	wbReg->addrGb = GAIN_ADDR_GB;
	wbReg->dataDefaultGain = GAIN_DEFAULT_VAL;	
// 	wbReg->addrGrHigh = HLT7012_ADDR_GAIN_GR_H;
// 	wbReg->addrGrLow  = HLT7012_ADDR_GAIN_GR_L;
// 	wbReg->addrGbHigh = HLT7012_ADDR_GAIN_GB_H;
// 	wbReg->addrGbLow  = HLT7012_ADDR_GAIN_GB_L;
// 	wbReg->addrRHigh  = HLT7012_ADDR_GAIN_R_H;
// 	wbReg->addrRLow   = HLT7012_ADDR_GAIN_R_L;
// 	wbReg->addrBHigh  = HLT7012_ADDR_GAIN_B_H;
// 	wbReg->addrBLow   = HLT7012_ADDR_GAIN_B_L;


	tagAwbRatio *wbGolden = GetWBGoldenValue();		
	wbGolden->rg  = m_goldenrg;
	wbGolden->bg  = m_goldenbg;

	LencOtpControlStruct *lencCtrl = GetLencControlParams();
// 	lencCtrl->inputTarget = HLT7012_VAL_LSC_TARGET;
// 	lencCtrl->inputLow = HLT7012_VAL_LSC_LOW;
// 	lencCtrl->inputHigh = HLT7012_VAL_LSC_HIGH;
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
// 	lencReg->addrCtrl = HLT7012_ADDR_LSC_CTRL;
// 	lencReg->lencEnable = HLT7012_ADDR_LSC_EN;
// 	lencReg->lencDisable = HLT7012_ADDR_LSC_DIS;
//	lencReg->addrDataStart = REG_INVALID;
//	lencReg->dataSize = HLT7012_OTP_LSC_SIZE;

	ExpControlStruct *expCtrl = GetExpControlParams();
	expCtrl->type = EXP_LINEAR;
	expCtrl->roiCalcMode = ROI_CALC_10;
	expCtrl->filterChannel = EXP_CHANNEL_G;
// 	expCtrl->target = HLT7012_VAL_EXP_TARGET;
// 	expCtrl->lowLimit = HLT7012_VAL_EXP_LOW;
// 	expCtrl->highLimit = HLT7012_VAL_EXP_HIGH;
	expCtrl->effectFrame = 3;
	expCtrl->stableFrame = 3;

	ExpRegisterStruct *expReg = GetExpRegister();
	expReg->mask = 0x00FFFF;
	expReg->addrHigh = REG_INVALID;
// 	expReg->addrMid = HLT7012_ADDR_EXP_H;
// 	expReg->addrLow = HLT7012_ADDR_EXP_L;

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

void COtpHLT7012::SetOtpSensorParams()
{
	OtpSensorStruct* params = GetOtpSensorParams();

	params->name = _T("HLT7012_MIPI");
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
//	params->otpInit.params = HLT7012_OTP_Settings;
//	params->otpInit.paramSize = sizeofHLT7012_OTP_Settings;

	params->verifyInit.width  = 4208;
	params->verifyInit.height = 3120;
	params->verifyInit.format = IMAGE_FORMAT_OTP_RAW10;
	params->verifyInit.sequence = IMAGE_SEQUENCE_GBRG;
	params->verifyInit.mhzMCLK = 24;
// 	params->verifyInit.params = HLT7012_OTP_Settings;
// 	params->verifyInit.paramSize = sizeofHLT7012_OTP_Settings;	
}


BOOL COtpHLT7012::EraseAllData()
{
	if(m_LSCItem == 0)
	{
		int i = 0;
		BYTE* eraseData = new BYTE[32]();
		memset(eraseData, 0xff, 32*sizeof(BYTE));//ע���ʼEEPROM��00����ff

		for (i=0; i<110; i++)
		{
			if (!GT24C64_Programming(0x0000+i*32,eraseData,32))//erase all
			{
				m_pInterface->AddLog(_T("OTP���ݲ���ʧ��!"),COLOR_RED);
				SAFE_DELETE_ARRAY(eraseData);
				return 0;
			}
		}

		SAFE_DELETE_ARRAY(eraseData);
	}
	else if (m_LSCItem == 1)
	{
		if (!DW9763_All_Erase())	return FALSE;
	}

	m_pInterface->AddLog(_T("OTP����������ȫ������!"));
	return TRUE;
}

void COtpHLT7012::SetWBDefaultGain(void)
{
	USHORT gainDefault = GAIN_DEFAULT_VAL;
	HLT7012_SetWBGain(gainDefault, gainDefault, gainDefault, gainDefault);
	return;
}

void COtpHLT7012::HLT7012_SetWBGain(USHORT gainGr, USHORT gainR, USHORT gainB, USHORT gainGb)
{
	i2c_write( 0x6028, 0x4000);
	i2c_write( 0x602a, 0x0100);
	i2c_write_byte( 0x6f12, 0x00);// Stream off
	Sleep(100);

	i2c_write(0x6028, 0x4000);	
	i2c_write(0x602a, 0x305c);
	i2c_write_byte(0x6f12, 0x01);

	i2c_write(GAIN_ADDR_GR,gainGr);
	i2c_write(GAIN_ADDR_R,gainR);
	i2c_write(GAIN_ADDR_B,gainB);
	i2c_write(GAIN_ADDR_GB,gainGb);

	i2c_write( 0x6028, 0x4000);
	i2c_write( 0x602a, 0x0100);
	i2c_write_byte( 0x6f12, 0x01 );// Stream on

	return;
}

BOOL COtpHLT7012::StartProgram()
{
	return TRUE;
}

BOOL COtpHLT7012::WriteOTP()
{
	HLT7012_GetBasicData();
	HLT7012_GetAFData();
	CreateBurningData();
	if (!m_dummyburn)
	{
		if (!HLT7012_OtpWrite())//
		{return FALSE;}
	}
	else
	{
		SaveOTPData();
		m_pInterface->AddLog(_T("This is Dummy Burn!"),COLOR_RED);
	}
	return TRUE;
}

void COtpHLT7012::SaveOTPData()
{
	m_pInterface->AddLog(_T("Read OTP Start!"));
	///////////////////////////////Read OTP////////////////////////////////////
	int valSize = SIZE_TOTAL_GROUP;
	int i = 0;
	ULONG sum = 0;
	ULONG Cnt = 0;
	BYTE* pVal = new BYTE[valSize];
	memset(pVal, 0xff, valSize*sizeof(BYTE));

	for (i=0; i<SIZE_BASIC_GROUP+1; i++)
	{
		pVal[i] = Data_Write.data_Basic[i];
		sum++;
	}
	sum+=4;
	Cnt = sum+1;
	for (i=0; i<SIZE_WB_GROUP+1; i++)
	{
		pVal[Cnt+i] = Data_Write.data_WB[i];
		sum++;
	}
	Cnt = sum+1;
	for (i=0;i<SIZE_AF_GROUP+1; i++)
	{
		pVal[Cnt+i] = Data_Write.data_AF[i];
		sum++;
	}
	Cnt = sum+1;
	for (i=0;i<SIZE_LSC_GROUP+1; i++)
	{
		pVal[Cnt+i] = Data_Write.data_Lenc[i];
		sum++;
	}
	Cnt = sum+1;
	for (i=0; i<SIZE_PDAF_GROUP1+1; i++)
	{
		pVal[Cnt+i] = Data_Write.data_PDAF1[i];
		sum++;		
	}
	sum+=SIZE_PDAF_GROUP2+1;
	sum+=6;
	Cnt = sum+1;
	for (i=0; i<SIZE_L3PDAF_GROUP1+1; i++)
	{
		pVal[Cnt+i] = Data_Write.data_L3PDAF1[i];
		sum++;
	}
	sum+=SIZE_L3PDAF_GROUP2+1;
	Cnt = sum+1;
	for (i = 0; i<3; i++)
	{
		pVal[Cnt+i] = Data_Write.data_date[i];
		sum++;
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

// 	if (m_LSCItem == 2)//ֻ��sensor lsc�Ų�shading
// 	{
// 		num.Format("Shading Data After SRAM:\nB1:%.1f\nB2:%.1f\nB3:%.1f\nB4:%.1f\n",vData.at(0), vData.at(1), vData.at(2), vData.at(3));
// 		temp += num;
// 	}

	// 	int group = valSize/64;
	// 	int ret = valSize%64;
	// 
	// 	for (j=0; j<group; j++)
	// 	{
	// 		num.Format("Page: 0x%x\n", j);
	// 		temp += num;
	// 		for(i=0; i<64; i++)
	// 		{
	// 			num.Format("%04d\t0x%02x,0x%02x\n", i+j*64,0x00+i,(unsigned char)pVal[i+j*64]);
	// 			temp += num;
	// 		}
	// 	}
	// 	num.Format("Page: 0x%x\n", j);
	// 	temp += num;
	// 	for (i=0; i<ret; i++)
	// 	{
	// 		num.Format("%04d\t0x%04x,0x%02x\n", i+j*64,0x00+i,(unsigned char)pVal[i+j*64]);
	// 		temp += num;
	// 	}

	for (i=0; i<valSize; i++)
	{
		num.Format("%04d\t0x%04x,0x%02x\n", i,0x00+i,(unsigned char)pVal[i]);
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

BOOL COtpHLT7012::WriteOTP(vector<double>& vData)
{
	HLT7012_GetBasicData();
	HLT7012_GetAFData();
	CreateBurningData();
	if (!m_dummyburn)
	{
		if (!HLT7012_OtpWrite())//
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

void COtpHLT7012::SaveOTPData(vector<double>& vData)
{
	m_pInterface->AddLog(_T("Read OTP Start!"));
	///////////////////////////////Read OTP////////////////////////////////////
	int valSize = 2456;
	int i = 0;
	int j = 0;
	BYTE* pVal = new BYTE[valSize]();

	for (i=0; i<SIZE_BASIC_GROUP+1; i++)
	{
		pVal[i] = Data_Write.data_Basic[i];
	}
	for (i=0; i<SIZE_WB_GROUP+1; i++)
	{
		pVal[0x10+i] = Data_Write.data_WB[i];
	}
	for (i=0;i<SIZE_LSC_GROUP+1; i++)
	{
		pVal[0x20+i] = Data_Write.data_Lenc[i];
	}
	for (i=0;i<SIZE_AF_GROUP+1; i++)
	{
		pVal[0x740+i] = Data_Write.data_AF[i];
	}
	for (i=0; i<SIZE_PDAF_GROUP1+1; i++)
	{
		pVal[0x780+i] = Data_Write.data_PDAF1[i];
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

	if (m_LSCItem == 2)//ֻ��sensor lsc�Ų�shading
	{
		num.Format("Shading Data After SRAM:\nB1:%.1f\nB2:%.1f\nB3:%.1f\nB4:%.1f\n",vData.at(0), vData.at(1), vData.at(2), vData.at(3));
		temp += num;
	}

// 	int group = valSize/64;
// 	int ret = valSize%64;
// 
// 	for (j=0; j<group; j++)
// 	{
// 		num.Format("Page: 0x%x\n", j);
// 		temp += num;
// 		for(i=0; i<64; i++)
// 		{
// 			num.Format("%04d\t0x%02x,0x%02x\n", i+j*64,0x00+i,(unsigned char)pVal[i+j*64]);
// 			temp += num;
// 		}
// 	}
// 	num.Format("Page: 0x%x\n", j);
// 	temp += num;
// 	for (i=0; i<ret; i++)
// 	{
// 		num.Format("%04d\t0x%04x,0x%02x\n", i+j*64,0x00+i,(unsigned char)pVal[i+j*64]);
// 		temp += num;
// 	}

	for (i=0; i<valSize; i++)
	{
		num.Format("%04d\t0x%04x,0x%02x\n", i,0x00+i,(unsigned char)pVal[i]);
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

void COtpHLT7012::CreateBurningData(void)
{
	int i = 0;
	//////////////////////////////////Basic//////////////////////////////////////
	Data_Write.data_Basic[0] = FLAG_VALID;
	for (i = 0; i < SIZE_BASIC_GROUP-1; i++)
	{
		Data_Write.data_Basic[i+1] = OTP_Data_Write.BaseInfo[i];
	}
	//////////////////////////////////AWB//////////////////////////////////////
	if (m_EnAWB)
	{
		Data_Write.data_WB[0] = FLAG_VALID;
		for (i = 0; i < SIZE_WB_GROUP-1; i++)
		{
			Data_Write.data_WB[i+1] = OTP_Data_Write.AWBInfo[i];
		}
	}
	//////////////////////////////////LSC//////////////////////////////////////
	if (m_EnLSC)
	{
		Data_Write.data_Lenc[0] = FLAG_VALID;
		if (m_LSCItem == 0)
		{
			for (i = 0; i < SIZE_LSC_GROUP-1; i++)
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
	}
	///////////////////////////////////AF/////////////////////////////////////
	if (m_EnAF)
	{
		Data_Write.data_AF[0] = FLAG_VALID;
		for (i = 0; i < SIZE_AF_GROUP-1; i++)
		{
			Data_Write.data_AF[i+1] = OTP_Data_Write.AFInfo[i];
		}
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
// 			for (i=0;i<Reserve_PDAF1; i++)
// 			{
// 				Data_Write.reservedpdaf1[i] = Reserverd_Data;
// 			}
			Data_Write.data_L3PDAF1[0] = FLAG_VALID;
			for (i=0; i<SIZE_L_PDAF_PROC1;i++)
			{
				Data_Write.data_L3PDAF1[i+1] = OTP_Data_Write.L3PDAF1[i] = Qual_L3_GainMapData[i];
			}
		}

	}
	/////////////////////////////////////other/////////////////////////////////////
	for (i=0; i<3; i++)
	{
		Data_Write.data_date[i] = OTP_Data_Write.DATE[i];
	}
	//////////////////////////////////////////////////////////////////////////
	HLT7012_CalcAllCheckSum();
	return;
}

BOOL COtpHLT7012::CheckOTP()
{
	if (!HLT7012_OTPVerify())		return FALSE;

	return TRUE;
}

BOOL COtpHLT7012::VerifyLSC()
{
	if (m_LSCItem == 2)
	{
		if (!HLT7012_VerifyLSC())		return FALSE; //for sensor LSC
	}
	return TRUE;
}

BOOL COtpHLT7012::VerifyPreWB()
{
	if (m_EnAWB)
	{
		if (!GetOTPRGBG())		return FALSE;
		HLT7012_ApplyAWB(m_wbCurrent,m_goldenrg,m_goldenbg);
	}
	return TRUE;
}

BOOL COtpHLT7012::VerifyAWB()
{
	if (m_EnAWB)
	{
		if (!HLT7012_VerifyAWB())		return FALSE;
	}
	return TRUE;
}

// BOOL COtpHLT7012::EndProgram()
// {
// 	return TRUE;
// }


BOOL COtpHLT7012::GetAWB()
{
	if (m_EnAWB)
	{
		if (!InitAWB())		return FALSE;
		if (!HLT7012_GetAWBData(OTP_Data_Write))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL COtpHLT7012::HLT7012_GetAWBData(HLT7012OTPDataSpec &awbdata)
{
	m_pInterface ->AddLog(_T("Get AWB Data Start"));

	if (m_qualawb)
	{
		if (!Qual_GetAWBData())
		{
			return FALSE;
		}
		Qual_GetAWBInfo(&m_wbCurrent);
	}
	else
	{
		if (!HLT7012_GetCurrentAWB(&m_wbCurrent))		return FALSE;
	}

// 	awbdata.AWBInfo[0] = (m_wbCurrent.rg>>8)&0x0f;
// 	awbdata.AWBInfo[1] = m_wbCurrent.rg&0xff;
// 	awbdata.AWBInfo[2] = (m_wbCurrent.bg>>8)&0x0f;
// 	awbdata.AWBInfo[3] = m_wbCurrent.bg&0xff;
// 	awbdata.AWBInfo[4] = (m_wbCurrent.gb_gr>>8)&0x0f;
// 	awbdata.AWBInfo[5] = m_wbCurrent.gb_gr&0xff;

	awbdata.AWBInfo[0] = m_wbCurrent.rg&0xff;
	awbdata.AWBInfo[1] = (m_wbCurrent.rg>>8)&0x0f;
	awbdata.AWBInfo[2] = m_wbCurrent.bg&0xff;
	awbdata.AWBInfo[3] = (m_wbCurrent.bg>>8)&0x0f;
	awbdata.AWBInfo[4] = m_wbCurrent.gb_gr&0xff;
	awbdata.AWBInfo[5] = (m_wbCurrent.gb_gr>>8)&0x0f;

	//Reserverd Data ��������д
	for (int i=0;i<Reserve_AWB;i++)
	{
		awbdata.AWBInfo[6+i] = Reserverd_Data;
	}

	Applyawbdata[0] = Qual_wb_r;
	Applyawbdata[1] = Qual_wb_gr;
	Applyawbdata[2] = Qual_wb_gb;
	Applyawbdata[3] = Qual_wb_r;
	Applyawbdata[4] = m_wbCurrent.rg;
	Applyawbdata[5] = m_wbCurrent.bg;
	Applyawbdata[6] = m_wbCurrent.gb_gr;
	m_pInterface ->AddLog(_T("Get AWB Data End"));
	HLT7012_ApplyAWB(m_wbCurrent, m_goldenrg, m_goldenbg);
	return TRUE;
}

BOOL COtpHLT7012::CheckOTPAWB()
{
	if (m_EnAWB)
	{
		m_pInterface ->AddLog(_T("Check Apply AWB Data Start"));
		if (!GetAWBInfo(&m_wbCurrent))		return FALSE;

		if (!HLT7012_CheckAWB(m_wbCurrent,1))				return FALSE;

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

BOOL COtpHLT7012::HLT7012_GetCurrentAWB(tagAwbRatio* wb)
{
	if (!GetAWBInfo(&m_wbCurrent))		return FALSE;

	if (!HLT7012_CheckAWB(m_wbCurrent,0))				return FALSE;

	//Apply Before
	m_pInterface->AddLog(_T("Before Apply WB:"));
	info.Format(_T("OB(10bit) = %02d"),SENSOROB);
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

	return TRUE;
}

BOOL COtpHLT7012::GetAWBInfo(tagAwbRatio* wb)
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
	wb->bg		= (UINT)(1.0 * (wb->b-SENSOROB) / (wb->gr-SENSOROB)*AWB_FACTOR + 0.5);
	wb->gb_gr	= (UINT)(1.0 * (wb->gb-SENSOROB)/ (wb->gr-SENSOROB)*AWB_FACTOR+ 0.5);

	m_szStr.Format(_T("R/G: %d B/G: %d"), wb->rg, wb->bg);
	m_pInterface->AddLog(m_szStr, COLOR_BLUE);
	return TRUE;
}

BOOL COtpHLT7012::HLT7012_CheckAWB(tagAwbRatio wb, int type)
{
	BOOL bRet = TRUE;
	float rg_diff = 0; float bg_diff = 0;
	m_pInterface ->AddLog(_T("Check AWB Data Start!"));

	switch(type)// 0:applyǰ, 1:apply ��, 2:OTP��awb
	{
	case 0:
		rg_diff = (float)(abs((int)(wb.rg-m_goldenrg))*1.0/m_goldenrg)*100;
		bg_diff =(float)(abs((int)(wb.bg-m_goldenbg))*1.0/m_goldenbg)*100;
		//����GG_diff?

		if (wb.rg<m_rgainl 
		 ||wb.bg<m_bgainl
	     ||wb.rg>m_rgainh
	     ||wb.bg>m_bgainh)
		{
			m_pInterface ->AddLog(_T("Applyǰwb gainֵ������Χ"),COLOR_RED);
			return FALSE;
		}

		if (rg_diff>m_orgdif || bg_diff > m_orgdif)
		{
			m_pInterface ->AddLog(_T("Applyǰwb gain diff������Χ"),COLOR_RED);
		    return FALSE;
		}
		break;

	case 1:
		rg_diff = (float)(abs((int)(wb.rg-m_goldenrg))*1.0/m_goldenrg)*100;
		bg_diff = (float)(abs((int)(wb.bg-m_goldenbg))*1.0/m_goldenbg)*100;

		if (rg_diff>m_rgaindif || bg_diff > m_bgaindif)
		{
			m_pInterface ->AddLog(_T("Apply��wb gain diff������Χ"),COLOR_RED);
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
			m_pInterface ->AddLog(_T("OTP�� gainֵ������Χ"),COLOR_RED);
			return FALSE;
		}

		if (rg_diff>m_rgaindif || bg_diff > m_bgaindif)
		{
			m_pInterface ->AddLog(_T("OTP�� gain diff������Χ"),COLOR_RED);
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

VOID COtpHLT7012::HLT7012_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG)
{
	USHORT r_ratio, b_ratio;

	r_ratio = 512 * (m_goldenrg) /(wb.rg);
	b_ratio = 512 * (m_goldenbg) /(wb.bg);

	if( !r_ratio || !b_ratio)
	{
		m_pInterface ->AddLog(_T("����Get AWB!"),COLOR_RED);
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

void COtpHLT7012::HLT7012_GetBasicData(void)
{
//	m_pInterface ->AddLog(_T("Get Basic Data Start"));
	CTime time = CTime::GetCurrentTime();

	OTP_Data_Write.BaseInfo[0] = BASIC_MID;
	OTP_Data_Write.BaseInfo[1] = BASIC_LENSID;
	OTP_Data_Write.BaseInfo[2] = BASIC_VCMID;
	OTP_Data_Write.BaseInfo[3] = BASIC_OISID;
	OTP_Data_Write.BaseInfo[4] = time.GetYear() - 2000;
	OTP_Data_Write.BaseInfo[5] = time.GetMonth();
	OTP_Data_Write.BaseInfo[6] = time.GetDay();
	OTP_Data_Write.BaseInfo[7] = BASIC_CUSTOM;

	for (int i=0;i<Reserve_Basic;i++)
	{
		OTP_Data_Write.BaseInfo[8+i] = Reserverd_Data;
	}

	OTP_Data_Write.DATE[0] = time.GetYear() - 2000;
	OTP_Data_Write.DATE[1] = time.GetMonth();
	OTP_Data_Write.DATE[2] = time.GetDay();

	info.Format(_T("MID = 0x%02x"),BASIC_MID);
	m_pInterface->AddLog(info);
	info.Format(_T("LensID = 0x%02x"),BASIC_LENSID);
	m_pInterface->AddLog(info);
	info.Format(_T("VCMID = 0x%02x"),BASIC_VCMID);
	m_pInterface->AddLog(info);
// 	info.Format(_T("DriverICID = 0x%02x"),BASIC_DRIVERICID);
// 	m_pInterface->AddLog(info);
// 	info.Format(_T("IRID =0x%02x"),BASIC_IRID);
// 	m_pInterface->AddLog(info);

	m_pInterface ->AddLog(_T("Get Basic Data."));
	return;
}

void COtpHLT7012::HLT7012_GetAFData(void)
{
//	m_pInterface ->AddLog(_T("Get AF Data Start"));
	OTP_Data_Write.AFInfo[0] = 0x02;//Hor Direction
	OTP_Data_Write.AFInfo[1] = m_infCode&0xff;
	OTP_Data_Write.AFInfo[2] = (m_infCode>>8)&0x0f;
	OTP_Data_Write.AFInfo[3] = m_macCode&0xff;
	OTP_Data_Write.AFInfo[4] = (m_macCode>>8)&0x0f;

	m_pInterface ->AddLog(_T("Get AF Data"));
	return;
}
void COtpHLT7012::HLT7012_CalcAllCheckSum(void)
{
	int i = 0;
	int sum = 0;

	// LSC
	if (m_LSCItem != 2)
	{
		sum = SumCalc(OTP_Data_Write.LSCInfo, SIZE_LSC_GROUP-1, 256,0);
		Data_Write.data_Lenc[SIZE_LSC_GROUP] = sum;
	}

	//AWB
	sum = SumCalc(OTP_Data_Write.AWBInfo,SIZE_WB_GROUP-1-Reserve_AWB,256,0);
	Data_Write.data_WB[SIZE_WB_GROUP] = sum;

	//Basic
	sum = SumCalc(OTP_Data_Write.BaseInfo,SIZE_BASIC_GROUP-1-Reserve_Basic-1,256,0);
	Data_Write.data_Basic[SIZE_BASIC_GROUP] = sum;

	//AF	
	sum = SumCalc(OTP_Data_Write.AFInfo,SIZE_AF_GROUP-1,256,0);
	Data_Write.data_AF[SIZE_AF_GROUP] = sum;

	if (m_EnPDAF)
	{
// 		//PDAF1
// 		sum = SumCalc(OTP_Data_Write.PDAF1,SIZE_PDAF_PROC1,256,0);
// 		Data_Write.data_PDAF1[SIZE_PDAF_PROC1+1] = sum;//checksum��reserveǰ��
// 
// 		//L3PDAF1
// 		sum = SumCalc(OTP_Data_Write.L3PDAF1,SIZE_L3PDAF_GROUP1,256,0);
// 		Data_Write.data_L3PDAF1[SIZE_L3PDAF_GROUP1+1] = sum;//checksum��reserveǰ��

		//PDAF1
		sum = 0;
		BYTE PDAF1sumdata[SIZE_PDAF_PROC1-13] = {0};
		for (i = 0; i <SIZE_PDAF_PROC1-13; i++)
		{
			sum+= OTP_Data_Write.PDAF1[i+13];
		}
		sum = sum % 256;
 		Data_Write.data_PDAF1[SIZE_PDAF_PROC1+1] = sum;

		//L3PDAF1
		sum = 0;
		BYTE L3PDAF1sumdata[SIZE_L_PDAF_PROC1-6] = {0};
		for (i = 0; i <SIZE_L_PDAF_PROC1-6; i++)
		{
			sum+= OTP_Data_Write.L3PDAF1[i+6];
		}
		sum = sum % 256;
 		Data_Write.data_L3PDAF1[SIZE_L_PDAF_PROC1+1] = sum;
	}

	return;
}

BOOL COtpHLT7012::CheckSensorOTPEmpty()
{
	if (m_sensoritem == 0)
	{
		BYTE tmpdata[64] = {0};
		if (m_LscGroup != 1)
		{
			S5K3L8_ReadOTPPage(26,tmpdata);

			if ( (tmpdata[0] == 0) && (tmpdata[1] == 0)&& (tmpdata[2] == 0) && (tmpdata[3] == 0) && (tmpdata[4] == 0))
			{
				m_pInterface->AddLog(_T("��һ��LSC����Ϊ�գ�������¼�ڶ���!"),COLOR_RED);		
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL COtpHLT7012::HLT7012_OtpWrite(void)
{
	////////////////////////////////////Check Sensor OTP Empty/////////////////////////////////
	if (m_LSCItem == 2)
	{
		if (!CheckSensorOTPEmpty())		return FALSE;
	}
	/////////////////////////////////Burning/////////////////////////////////////////
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
		if (!ProgrammingPDAF())  return FALSE;
	}

	m_pInterface->AddLog(_T("OTP Burning Success!"),COLOR_BLUE);
	return TRUE;
}

BOOL COtpHLT7012::ProgrammingBasic()
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
				m_pInterface->AddLog(_T("OTPֻ������¼��һ��!"),COLOR_RED);		
				return FALSE;
			}
			Programming_3L8Basic();
		}

	}
	return TRUE;
}

void COtpHLT7012::Programming_3L8Basic()
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

BOOL COtpHLT7012::ProgrammingAWB()
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
		if (DW9763_Programming(m_addrFlag.awb,Data_Write.data_WB,sizeof(Data_Write.data_WB)) != TRUE)
		{
			m_pInterface->AddLog(_T("AWB Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
	}
	else if(m_LSCItem == 2)
	{
		if (m_sensoritem == 0)
		{
			Programming_3L8AWB();
		}
	}

	return TRUE;
}

void COtpHLT7012::Programming_3L8AWB()
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

BOOL COtpHLT7012::ProgrammingLSC()
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
		if (DW9763_Programming(m_addrFlag.lsc,Data_Write.data_Lenc,sizeof(Data_Write.data_Lenc)) != TRUE)
		{
			m_pInterface->AddLog(_T("LSC Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
	}
	else if(m_LSCItem == 2)
	{
		if (m_sensoritem == 0)
		{
			Programming_3L8LSC();
		}
	}

	return TRUE;
}

BOOL COtpHLT7012::Programming_3L8LSC()
{
	BYTE _LSCData[417] = {0};
	if (m_LscGroup == 1)//OTP��ͷ���м���
	{
		S5K3L8_ReadOTPPage(26, _LSCData);
	}
	if (m_LscGroup == 2)
	{
		S5K3L8_ReadOTPPage(19, _LSCData);
	}
	if ( (_LSCData[8]|_LSCData[7]|_LSCData[6]|_LSCData[5]|_LSCData[4]|_LSCData[3]|_LSCData[2]|_LSCData[1]|_LSCData[0]) != 0)
	{
		m_pInterface->AddLog(_T("OTP������LSC���ݣ�"), COLOR_RED);
		return FALSE;
	}

	CString tempStr;
	CString strWorkPath  = GetModulePath()+"\\3L8";
	tempStr.Format(_T("\\3L8LSCOTP.sset"));
	tempStr = strWorkPath + tempStr;
	
	CStdioFile file;
	if ( !file.Open( (tempStr), CFile::modeRead | CFile::typeText ) )
	{
		m_pInterface->AddLog(_T("���ļ�ʧ�ܣ�"), COLOR_RED);
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
		strLine.TrimRight();//ȥ�����ҿո�
		if ( strLine.Find( _T("s"), 0 ) == 0 )
		{
			int nLength =strLine.GetLength();
			if(nLength==7)//setting
			{
				intVal = strtoul( CT2A(strLine.Mid(1,6)), NULL, 16 );
				intLSCAddr = intVal>>8;//16����
				intLSCValue = intVal%256;
				if (!m_dummyburn)
				{
					i2c_write_byte( intLSCAddr, intLSCValue );
				}
			}
			else//д������
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
					HLT7012_LSCWriteData[k] = ParaList[i];
					k++;
				}
			}
		}

	}
	file.Close();

	//calc lsc checksum
	HLT7012_LSCchecksum=0;
	if(m_LscGroup==1)
	{
		for (i=0;i<417;i++)
		{
			HLT7012_LSCchecksum+=HLT7012_LSCWriteData[i+31];
		}
		HLT7012_LSCchecksum=HLT7012_LSCchecksum%255+1;
	}
	if (m_LscGroup==2)
	{
		for (i=0;i<417;i++)
		{
			HLT7012_LSCchecksum+=HLT7012_LSCWriteData[i+62];
		}
		HLT7012_LSCchecksum=HLT7012_LSCchecksum%255+1;
	}

	HLT7012_WriteLSCFlagSum();

	return TRUE;
}

void COtpHLT7012::HLT7012_WriteLSCFlagSum()
{
	Data_Write.data_Lenc[0] = FLAG_VALID;
	Data_Write.data_Lenc[1] = HLT7012_LSCchecksum;

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

BOOL COtpHLT7012::ProgrammingAF()
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
		if (DW9763_Programming(m_addrFlag.af,Data_Write.data_AF,sizeof(Data_Write.data_AF)) != TRUE)
		{
			m_pInterface->AddLog(_T("AF Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
	}
	else if (m_LSCItem == 2)
	{
		if (m_sensoritem == 0)
		{
			Programming_3L8AF();
		}
	}

	return TRUE;
}

void COtpHLT7012::Programming_3L8AF()
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

BOOL COtpHLT7012::ProgrammingPDAF()
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
		if (DW9763_Programming(m_addrFlag.pdaf1,Data_Write.data_PDAF1,sizeof(Data_Write.data_PDAF1)) != TRUE)
		{
			m_pInterface->AddLog(_T("Qual PDAF Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}

		if (DW9763_Programming(m_addrFlag.l3pdaf1,Data_Write.data_L3PDAF1,sizeof(Data_Write.data_L3PDAF1)) != TRUE)
		{
			m_pInterface->AddLog(_T("Qual L3PDAF Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}

		if (DW9763_Programming(0x0E84,Data_Write.data_date,sizeof(Data_Write.data_date)) != TRUE)
		{
			m_pInterface->AddLog(_T("DATE Info Write Fail!"),COLOR_RED);		
			return FALSE;
		}
	}

	return TRUE;
}

BOOL COtpHLT7012::HLT7012_OTPVerify(void)
{
	if (!m_dummyburn)//!m_dummyburn 
	{
		if (!HLT7012_CheckOTPReadWrite())		return FALSE;
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

BOOL COtpHLT7012::HLT7012_CheckOTPReadWrite(void)
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

		BYTE *read_flag = new BYTE[1];
		BYTE *flag_check = new BYTE[1];
		flag_check[0] = (BYTE)m_flagValid;
		BYTE *Read_Sum = new BYTE[1];
		GT24C64_Read(m_addrSum.pdaf, Read_Sum, 1);
		BYTE *Write_Sum = new BYTE[1];
		Write_Sum[0] = (BYTE)proc1sum;
		BOOL result = TRUE;
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

				if (GT24C64_Read(m_addrFlag.pdaf+1, OTP_Data_Read.PDAF1, sizeof (OTP_Data_Read.PDAF1)) != TRUE)
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

	if (m_LSCItem == 1)
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

		if (DW9763_Read(m_addrFlag.pdaf1, Data_Read.data_PDAF1,sizeof(Data_Read.data_PDAF1)) != TRUE)
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

		if (DW9763_Read(m_addrFlag.l3pdaf1, Data_Read.data_L3PDAF1,sizeof(Data_Read.data_L3PDAF1)) != TRUE)
		{
			m_pInterface->AddLog(_T("Read L3PDAF Proc1 Data Fail!"),COLOR_RED);		
			return FALSE;
		}
		else
		{
			if (!CheckEqual(Data_Write.data_L3PDAF1,Data_Read.data_L3PDAF1,sizeof(Data_Write.data_L3PDAF1)))
			{
				m_pInterface->AddLog(_T("Check L3PDAF Proc1 Data Fail!"),COLOR_RED);
				return FALSE;
			}
		}

		if (DW9763_Read(0x0E84, Data_Read.data_date,sizeof(Data_Read.data_date)) != TRUE)
		{
			m_pInterface->AddLog(_T("Read Date Data Fail!"),COLOR_RED);		
			return FALSE;
		}
		else
		{
			if (!CheckEqual(Data_Write.data_date,Data_Read.data_date,sizeof(Data_Write.data_date)))
			{
				m_pInterface->AddLog(_T("Check Date Data Fail!"),COLOR_RED);
				return FALSE;
			}
		}
	}

	if (m_LSCItem == 2)
	{
		if (m_sensoritem == 0)
		{
			Sensor3L8_StreamOn();
			if (!Sensor3L8_CheckReadWrite()) return FALSE; 
			if (!Sensor3L8_CheckLSCReadWrite()) return FALSE;
			m_pInterface->AddLog(_T("Check OTP Data Success!"),COLOR_BLUE);	
		}
	}


	m_pInterface->AddLog(_T("Check OTP Data OK!"),COLOR_BLUE);
	return TRUE;
}

BOOL COtpHLT7012::Sensor3L8_CheckReadWrite()
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
				m_pInterface->AddLog(_T("Basic Info��д�Ա�ʧ�ܣ�"),COLOR_RED);
				return FALSE;
			}
		}
		for (i=0; i<SIZE_WB_GROUP; i++)
		{
			if (ReadData[i+9] != Data_Write.data_WB[i])
			{
				m_pInterface->AddLog(_T("AWB Info��д�Ա�ʧ�ܣ�"),COLOR_RED);
				return FALSE;
			}
		}
		for (i=0; i<SIZE_AF_GROUP+1; i++)
		{
			if (ReadData[i+16] != Data_Write.data_AF[i])
			{
				m_pInterface->AddLog(_T("AF Info��д�Ա�ʧ�ܣ�"),COLOR_RED);
				return FALSE;
			}
		}
		for (i=0; i<2; i++)
		{
			if (ReadData[i+22] != Data_Write.data_Lenc[i])
			{
				m_pInterface->AddLog(_T("LSC Info��д�Ա�ʧ�ܣ�"),COLOR_RED);
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
				m_pInterface->AddLog(_T("Basic Info��д�Ա�ʧ�ܣ�"),COLOR_RED);
				return FALSE;
			}
		}
		for (i=0; i<SIZE_WB_GROUP; i++)
		{
			if (ReadData[i+9+OTPGAP] != Data_Write.data_WB[i])
			{
				m_pInterface->AddLog(_T("AWB Info��д�Ա�ʧ�ܣ�"),COLOR_RED);
				return FALSE;
			}
		}
		for (i=0; i<SIZE_AF_GROUP+1; i++)
		{
			if (ReadData[i+16+OTPGAP] != Data_Write.data_AF[i])
			{
				m_pInterface->AddLog(_T("AF Info��д�Ա�ʧ�ܣ�"),COLOR_RED);
				return FALSE;
			}
		}
		for (i=0; i<2; i++)
		{
			if (ReadData[i+22+OTPGAP] != Data_Write.data_Lenc[i])
			{
				m_pInterface->AddLog(_T("LSC Info��д�Ա�ʧ�ܣ�"),COLOR_RED);
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL COtpHLT7012::Sensor3L8_CheckLSCReadWrite()
{
	int i = 0;
	if (m_LscGroup == 1)
	{
		S5K3L8_ReadOTPPage(0x19,HLT7012_LSCReadData);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1A,HLT7012_LSCReadData+64);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1B,HLT7012_LSCReadData+128);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1C,HLT7012_LSCReadData+192);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1D,HLT7012_LSCReadData+256);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1E,HLT7012_LSCReadData+320);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1F,HLT7012_LSCReadData+384);
		Sleep(100);	
		for (i=31;i<(417+31);i++)
		{	
			if (HLT7012_LSCReadData[i] != HLT7012_LSCWriteData[i])
			{
				m_pInterface->AddLog(_T("LSC Info��д�Ա�ʧ�ܣ�"));
				return 0;
			}
		}
	}

	if (m_LscGroup == 2)
	{
		S5K3L8_ReadOTPPage(0x12,HLT7012_LSCReadData);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x13,HLT7012_LSCReadData+64);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x14,HLT7012_LSCReadData+128);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x15,HLT7012_LSCReadData+192);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x16,HLT7012_LSCReadData+256);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x17,HLT7012_LSCReadData+320);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x18,HLT7012_LSCReadData+384);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x19,HLT7012_LSCReadData+448);
		Sleep(100);
		for (i=62;i<(417+62);i++)
		{	
			if (HLT7012_LSCReadData[i] != HLT7012_LSCWriteData[i])
			{
				m_pInterface->AddLog(_T("LSC Info��д�Ա�ʧ�ܣ�"));
				return 0;
			}
		}
	}
	return TRUE;
}

BOOL COtpHLT7012::GetOTPRGBG()
{
	m_wbCurrent.rg = Data_Read.data_WB[2]<<8|Data_Read.data_WB[1];
	m_wbCurrent.bg = Data_Read.data_WB[4]<<8|Data_Read.data_WB[3];
	return TRUE;
}

BOOL COtpHLT7012::HLT7012_VerifyLSC()
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

BOOL COtpHLT7012::HLT7012_VerifyAWB()
{
	if (!GetAWBInfo(&m_wbCurrent))		return FALSE;
	if (!HLT7012_CheckAWB(m_wbCurrent,2))		return FALSE;
	return TRUE;
}