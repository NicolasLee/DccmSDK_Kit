#include "StdAfx.h"
#include "PDAF_QualL4_Inspection.h"
#include "ImageProc.h"



#ifndef Dccm_EXP
    #define Dccm_EXP	__declspec(dllexport)
#endif	/* Dccm_EXP */


extern "C" Dccm_EXP Inspection * CreateInspectionItem(DccmControl_Interface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new PD_CALI_Qual_2_Inspection(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
PD_CALI_Qual_2_Inspection::PD_CALI_Qual_2_Inspection(DccmControl_Interface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	SetConfigPath(pInterface->GetConfigPath());	 //设置CONFIG路径	
	SetName(lpszName);	                             //设置测试项目的名字
	pDlg = new OptionDlg(NULL,this);                 //生成对话框对象
	LoadOption();                                    //读取参数
	pDlg->Create(OptionDlg::IDD,NULL);               //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TET_MID);


	//TODO: 其他类成员构造初始化
	FrameCnt = 0;
	CaptureSize = 0;
	Lock_OneTime = FALSE;
	//创建保存路径
	CString Path = m_pInterface->GetExeDir();
	FileFolderPath += Path + _T("PDAF\\");
	CreateDirectory(FileFolderPath, NULL);
	FileFolderPath +=   _T("Qual\\");
	CreateDirectory(FileFolderPath, NULL);
	if (0 == pDlg->m_QUALPDAFitem)
	{
		FileFolderPath += _T("L4\\");
	}
	else if (1 == pDlg->m_QUALPDAFitem)
	{
		FileFolderPath += _T("L5\\");
	}
	CreateDirectory(FileFolderPath, NULL);	
	//.....
}

PD_CALI_Qual_2_Inspection::~PD_CALI_Qual_2_Inspection(void)
{

}

/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int PD_CALI_Qual_2_Inspection::Initialize()
{
	Inspection::Initialize();
	//TODO:在此添加初始化代码
	bInit = FALSE;
	int i = 0;
	if(!Lock_OneTime)
	{
		m_pInterface->RestartCamera(1);
		Sleep(m_SleepTime * 200);
 		m_pInterface->GetDriverIC()->WriteAF(0);
 		Sleep(m_SleepTime * 200);
	
		ZC533_LastPage = -1;
		// 得到抓图的数量
		CaptureSize = 10;
		CString str;
		str.Format(_T("Capture size = %d"), CaptureSize);
		m_pInterface->AddLog(str);	

		lensPos = new int[CaptureSize];

		if (!CheckOTPIsBurning())   return 0;

		// number of lens positions to be used for dcc calibration. do not change
		const uint16_t dcc_stack_sz = DCC_STACK_SZ;

		// read AF calibration
		// move lens from INF/PAN to MACRO in 1/7 equally spaced steps
		// this example assumes INF/PAN focus=240 and MACRO focus=800
		/*******************************************
		Second Part(1): Phase Difference Conversion Coefficient Calibration 
		********************************************/
		BYTE AFCheckCode[4] = {0};
		GT24C64_Read(0x000E, AFCheckCode, 4);
		InfinityCode = (AFCheckCode[0]<<8)|AFCheckCode[1];
		MacroCode = (AFCheckCode[2]<<8)|AFCheckCode[3];

// 		AFCheckCode[0] = 0;
// 		AFCheckCode[1] = 0xac;
// 		AFCheckCode[2] = 0x01;
// 		AFCheckCode[3] = 0x77;
// 		InfinityCode = (AFCheckCode[0] << 8) | AFCheckCode[1];
// 		MacroCode = (AFCheckCode[2] << 8) | AFCheckCode[3];

		if(InfinityCode>MacroCode)
		{
			m_pInterface->AddLog(_T("远景大于近景 !"),COLOR_RED);
			return 0;
		}
		if(InfinityCode<50 || InfinityCode>400)
		{
			m_pInterface->AddLog(_T("远景Code值异常 !"),COLOR_RED);
			return 0;
		}
		if(MacroCode>700 || MacroCode<100)
		{
			m_pInterface->AddLog(_T("近景Code值异常 !"),COLOR_RED);
			return 0;
		}

		info.Format(_T("InfCode = %d, MacCode = %d."),InfinityCode,MacroCode);
		m_pInterface->AddLog(info);
/*
#if 0//3P3
		BYTE GainMapData[1078] = {0};
		ZC533_Read(0x781, GainMapData, 1078);

		BYTE sumcalc = SumCalc(GainMapData,1078,255,1);
		BYTE readsum = 0;
		ZC533_Read(0xBB7, &readsum, 1);
		if (readsum != sumcalc)
		{
			m_pInterface->AddLog(_T("Read Gainmap data failed."),COLOR_RED);			
			return 0;
		}
		GainMapData2D.VersionNum = (GainMapData[0]<<8) | GainMapData[1];
		GainMapData2D.OffsetX   =(GainMapData[2]<<8) | GainMapData[3];
		GainMapData2D.OffsetY   =(GainMapData[4]<<8) | GainMapData[5];
		GainMapData2D.RatioX    =(GainMapData[6]<<8) | GainMapData[7];
		GainMapData2D.RatioY    =(GainMapData[8]<<8) | GainMapData[9];
		GainMapData2D.MapWidth  =(GainMapData[10]<<8) | GainMapData[11];
		GainMapData2D.MapHeight =(GainMapData[12]<<8) | GainMapData[13];
		
		for (i=0; i<GainMapData2D.MapWidth * GainMapData2D.MapHeight; i++)
		{
			GainMapData2D.Left_GainMap[i] = (GainMapData[14+i*2]<<8) | GainMapData[15+i*2]; 
			GainMapData2D.Right_GainMap[i] = (GainMapData[14+532+i*2]<<8) | GainMapData[15+532+i*2]; 
		}
#else //3l8
		/*BYTE GainMapData[890] = {0};
		DW9763_Read(0x470, GainMapData, 890);

		gain_map.cal_version = GainMapData[1]<<8 | GainMapData[0];
		gain_map.width = GainMapData[3]<<8 | GainMapData[2];
		gain_map.height = GainMapData[5]<<8 | GainMapData[4]; 
		for (i= 0; i< gain_map.width * gain_map.height; i++)
		{
			gain_map.gm_l[i] = GainMapData[7+i*2]<<8|GainMapData[6+i*2];
			gain_map.gm_r[i] = GainMapData[7+gain_map.width*gain_map.height*2+i*2]<<8|GainMapData[6+gain_map.width*gain_map.height*2+i*2];
		}*//*
#endif
*/		//OV13855
		BYTE GainMapData[890] = {0};
		GT24C64_Read(0x712, GainMapData, 890);

		gain_map.cal_version = GainMapData[0]<<8 | GainMapData[1];
		gain_map.width = GainMapData[2]<<8 | GainMapData[3];
		gain_map.height = GainMapData[4]<<8 | GainMapData[5]; 
		for (i= 0; i< gain_map.width * gain_map.height; i++)
		{
			gain_map.gm_l[i] = GainMapData[6+i*2]<<8|GainMapData[7+i*2];
			gain_map.gm_r[i] = GainMapData[6+gain_map.width*gain_map.height*2+i*2]<<8|GainMapData[7+gain_map.width*gain_map.height*2+i*2];
		}

#if 0
		OTP_single_defect_2DGainmap(GainMapData2D,gmvs.GainDiff);
#else
	//	OTP_single_defect_1DGainmap(GainMapData1D,gmvs.GainDiff);
#endif
		float StepCode = (float)((MacroCode -	InfinityCode)/9.0);
		//////////////////////////////////////////////////////////////////////////
		lensPos[0]=(USHORT)(InfinityCode);
		lensPos[1]=(USHORT)((InfinityCode+StepCode+0.5));
		lensPos[2]=(USHORT)((InfinityCode+2*StepCode+0.5));
		lensPos[3]=(USHORT)(InfinityCode+3*StepCode+0.5);
		lensPos[4]=(USHORT)(InfinityCode+4*StepCode+0.5);
		lensPos[5]=(USHORT)(InfinityCode+5*StepCode+0.5);
		lensPos[6]=(USHORT)(InfinityCode+6*StepCode+0.5);
		lensPos[7]=(USHORT)(InfinityCode+7*StepCode+0.5);
		lensPos[8]=(USHORT)(InfinityCode+8*StepCode+0.5);
		lensPos[9]=(USHORT)(MacroCode);
		Imagefilename[0] = _T("01");
		Imagefilename[1] = _T("02");
		Imagefilename[2] = _T("03");
		Imagefilename[3] = _T("04");
		Imagefilename[4] = _T("05");
		Imagefilename[5] = _T("06");
		Imagefilename[6] = _T("07");
		Imagefilename[7] = _T("08");
		Imagefilename[8] = _T("09");
		Imagefilename[9] = _T("10");

 		 // 驱动马达到远景
		 m_pInterface->GetDriverIC()->WriteAF(lensPos[0]);
		 Sleep(m_SleepTime * 200);

		 CaptureFrame = 0;
		Lock_OneTime = TRUE;
	}

	if (pDlg->m_ExposureEn)
	{	
		if (m_pInterface->Exposure(pDlg->m_Exp_Target,  pDlg->m_Exp_Tolerance, EXP_G, 3, 50,50,25) != 0)//中心1/2
			return -1;
	}
    
	bInit = TRUE;

	return 0;
}

BOOL PD_CALI_Qual_2_Inspection::CheckOTPIsBurning()
{
	BYTE flag;
	GT24C64_Read(0x0000,&flag,1);

// 	if (flag == 0xFF)
// 	{
// 		m_pInterface->AddLog(_T("OTP内容为空，请先烧录画质"),COLOR_RED);
// 		return FALSE;
// 	}
/*	else */if (flag == 0x01)
	{
		m_pInterface->AddLog(_T("OTP 已烧录过，重新烧录..."),COLOR_RED);
		//return FALSE;
	}
	return TRUE;
}

BYTE PD_CALI_Qual_2_Inspection::SumCalc(BYTE* pVal, int valSize, int mod,int add)
{
	ASSERT(pVal != NULL);

	ULONGLONG  sum = 0;

	for (int i = 0; i<valSize; i++)
	{
		sum += pVal[i];
	}

	return (BYTE)(sum % mod+add);
}
/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int PD_CALI_Qual_2_Inspection::Testing()
{
   //TODO:在此添加测试项代码
	if (!bInit)
	{
		SetResult(RESULT_INSPECTION_NG);
		return RET_END;
	}

	int nRet = PD_CALI_Proc2();
	if(nRet == 0) 
	{
		SetResult(RESULT_INSPECTION_OK);
		m_pInterface->AddLog(_T("Qual PD CALI STEP2 PASS"), COLOR_GREEN, 330);
	}
	else if (nRet > 0)	// fail
	{
		SetResult(RESULT_INSPECTION_NG);
		m_pInterface->AddLog(_T("Qual PD CALI STEP2 FAIL"), COLOR_RED, 330);
	}
	else
	{
		return RET_CONTIUNE;
	}
	return RET_END;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int PD_CALI_Qual_2_Inspection::Finalize()
{
	//TODO:在此添加回收结束代码
	Lock_OneTime = FALSE;

// 	for (int i=0; i<CaptureSize; i++)
// 	{
// 		SAFE_DELETE_ARRAY(RawArray[i]);
// 	}
// 	SAFE_DELETE_ARRAY(RawArray);
	SAFE_DELETE_ARRAY(lensPos);

	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int PD_CALI_Qual_2_Inspection::LoadOption()
{
	//TODO: 在此添加读取代码 
	pDlg->m_ExposureEn      = ReadConfigInt(_T("ExposureEn"), 1);
	pDlg->m_Exp_Target      = ReadConfigInt(_T("Exp_Target"), 200);
	pDlg->m_Exp_Tolerance   = ReadConfigInt(_T("Exp_Tolerance"),4);
	pDlg->m_bSaveRaw		= ReadConfigInt(_T("EnSaveRaw"),	1);
	pDlg->m_berase = ReadConfigInt(_T("EnErase"),0);
	m_CodeSPC		= ReadConfigInt(_T("CodeDiff"),	1);

	pDlg->m_QVLEn      = ReadConfigInt(_T("QvlEn"), 1);
	pDlg->m_negvalue   = ReadConfigInt(_T("NegValueEn"), 0);
	pDlg->m_QUALPDAFitem = ReadConfigInt(_T("QUALPDAFVersion"), 0);

	pDlg->m_InfH    = ReadConfigInt(_T("InfH"), 1023);
	pDlg->m_InfL    = ReadConfigInt(_T("InfL"), 0);
	pDlg->m_MacH    = ReadConfigInt(_T("MacH"), 1023);
	pDlg->m_MacL    = ReadConfigInt(_T("MacL"), 0);
	pDlg->m_MidH    = ReadConfigInt(_T("MidH"), 1023);
	pDlg->m_MidL    = ReadConfigInt(_T("MidL"), 0);
	pDlg->m_AFDif    = ReadConfigInt(_T("AFDif"), 300);

	pDlg->m_KH    = ReadConfigInt(_T("KH"), 100000);
	pDlg->m_KL    = ReadConfigInt(_T("KL"), -100000);

	pDlg->m_szDEV_ADDR	= ReadConfigString(_T("m_sDev_Addr"),_T("A0"));
	pDlg->m_szReg_ADDR	= ReadConfigString(_T("m_sReg_Addr"),_T("98B"));
	m_PageSize			= ReadConfigInt(_T("m_PageSize"),		806);
	m_SleepTime			= ReadConfigInt(_T("m_SleepTime"),		5);
	m_I2cMode			= ReadConfigInt(_T("m_I2CMode"),		3);
	pDlg->m_szInf_Lsb_ADDR = ReadConfigString(_T("m_szInf_Lsb_ADDR"),_T("3d"));
	pDlg->m_szInf_Msb_ADDR = ReadConfigString(_T("m_szInf_Msb_ADDR"),_T("3e"));
	pDlg->m_szMac_Lsb_ADDR = ReadConfigString(_T("m_szMac_Lsb_ADDR"),_T("3f"));
	pDlg->m_szMac_Msb_ADDR = ReadConfigString(_T("m_szMac_Msb_ADDR"),_T("40"));
	m_Ini_Name = ReadConfigString(_T("INI_NAME"), _T("setting_sample(3L8_4208_3120)_DLL_2.0.1_20151223.ini"));

	MinPixelAvg = ReadConfigInt(_T("MinPixelAvg"),64);
	MaxPixelAvg = ReadConfigInt(_T("MaxPixelAvg"),1023);
	MinPDAvg = ReadConfigInt(_T("MinPDAvg"),64);
	MaxPDAvg = ReadConfigInt(_T("MaxPDAvg"),1023);
	MinGain = ReadConfigInt(_T("MinGain"),64);
	MaxGain = ReadConfigInt(_T("MaxGain"),1024);
	MaxPixelThresh = ReadConfigInt(_T("MaxPixelThresh"),256);
	GainDiff = ReadConfigInt(_T("GainDiff"),256);

	swscanf_s(pDlg->m_szDEV_ADDR.GetBuffer(),_T("%X"),&m_Dev_Addr);
	swscanf_s(pDlg->m_szReg_ADDR.GetBuffer(),_T("%X"),&m_Reg_Addr);
	swscanf_s(pDlg->m_szInf_Lsb_ADDR.GetBuffer(),_T("%X"),&inf_lsb_reg);
	swscanf_s(pDlg->m_szInf_Msb_ADDR.GetBuffer(),_T("%X"),&inf_msb_reg);
	swscanf_s(pDlg->m_szMac_Lsb_ADDR.GetBuffer(),_T("%X"),&mac_lsb_reg);
	swscanf_s(pDlg->m_szMac_Msb_ADDR.GetBuffer(),_T("%X"),&mac_msb_reg);

	//........................ 
	return Inspection::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int PD_CALI_Qual_2_Inspection::SaveOption()
{
	if(!Inspection::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	WriteConfigInt(_T("ExposureEn"),	pDlg->m_ExposureEn);
	WriteConfigInt(_T("Exp_Target"),	pDlg->m_Exp_Target);
	WriteConfigInt(_T("Exp_Tolerance"), pDlg->m_Exp_Tolerance);
	WriteConfigInt(_T("EnSaveRaw"),		pDlg->m_bSaveRaw);
	WriteConfigInt(_T("EnErase"),		pDlg->m_berase);
	WriteConfigInt(_T("CodeDiff"), m_CodeSPC);

	WriteConfigInt(_T("QvlEn      "),	pDlg->m_QVLEn);
	WriteConfigInt(_T("NegValueEn      "),	pDlg->m_negvalue);
	WriteConfigInt(_T("QUALPDAFVersion      "), pDlg->m_QUALPDAFitem);

	WriteConfigInt(_T("InfH      "),pDlg->m_InfH);
	WriteConfigInt(_T("InfL      "),pDlg->m_InfL);
	WriteConfigInt(_T("MacH      "),pDlg->m_MacH);
	WriteConfigInt(_T("MacL      "),pDlg->m_MacL);
	WriteConfigInt(_T("MidH      "),pDlg->m_MidH);
	WriteConfigInt(_T("MidL      "),pDlg->m_MidL);
	WriteConfigInt(_T("AFDif      "),pDlg->m_AFDif);

	WriteConfigInt(_T("KH      "),pDlg->m_KH);
	WriteConfigInt(_T("KL      "),pDlg->m_KL);

	WriteConfigString(_T("m_sDev_Addr"),pDlg->m_szDEV_ADDR);
	WriteConfigString(_T("m_sReg_Addr"),pDlg->m_szReg_ADDR);
	WriteConfigInt(_T("m_PageSize"), m_PageSize);
	WriteConfigInt(_T("m_SleepTime"),m_SleepTime);
	WriteConfigInt(_T("m_I2CMode"),m_I2cMode);
	WriteConfigString(_T("m_szInf_Lsb_ADDR"),pDlg->m_szInf_Lsb_ADDR);
	WriteConfigString(_T("m_szInf_Msb_ADDR"),pDlg->m_szInf_Msb_ADDR);
	WriteConfigString(_T("m_szMac_Lsb_ADDR"),pDlg->m_szMac_Lsb_ADDR);
	WriteConfigString(_T("m_szMac_Msb_ADDR"),pDlg->m_szMac_Msb_ADDR);
	WriteConfigString(_T("INI_NAME"), m_Ini_Name);

	WriteConfigInt(_T("MinPixelAvg      "),MinPixelAvg);
	WriteConfigInt(_T("MaxPixelAvg      "),MaxPixelAvg);
	WriteConfigInt(_T("MinPDAvg      "),MinPDAvg);
	WriteConfigInt(_T("MaxPDAvg      "),MaxPDAvg);
	WriteConfigInt(_T("MinGain      "),MinGain);
	WriteConfigInt(_T("MaxGain      "),MaxGain);
	WriteConfigInt(_T("MaxPixelThresh      "),MaxPixelThresh);
	WriteConfigInt(_T("GainDiff      "),GainDiff);

	swscanf_s(pDlg->m_szDEV_ADDR.GetBuffer(),_T("%X"),&m_Dev_Addr);
	swscanf_s(pDlg->m_szReg_ADDR.GetBuffer(),_T("%X"),&m_Reg_Addr);
	swscanf_s(pDlg->m_szInf_Lsb_ADDR.GetBuffer(),_T("%X"),&inf_lsb_reg);
	swscanf_s(pDlg->m_szInf_Msb_ADDR.GetBuffer(),_T("%X"),&inf_msb_reg);
	swscanf_s(pDlg->m_szMac_Lsb_ADDR.GetBuffer(),_T("%X"),&mac_lsb_reg);
	swscanf_s(pDlg->m_szMac_Msb_ADDR.GetBuffer(),_T("%X"),&mac_msb_reg);

	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR PD_CALI_Qual_2_Inspection::GetReportHeaders()
{
	static CString header;
	header = _T("");
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR PD_CALI_Qual_2_Inspection::GetReportLowerLimits()
{
	static CString LowerLimit;
	LowerLimit = _T("");
	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR PD_CALI_Qual_2_Inspection::GetReportUpperLimits()
{
	static CString UpperLimit;
	UpperLimit = _T("");
	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR PD_CALI_Qual_2_Inspection::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}



int PD_CALI_Qual_2_Inspection::PD_CALI_Proc2(void)
{
//cnw
// 	BYTE DCCData1[103] = { 0 };
// 	for (int i = 0; i < 102; i++)
// 	{
// 		DCCData1[i] = 0x55;
// 	}
// 	DCCData1[102] = SumCalc(DCCData1, 102, 255, 0);
// 
// 	BYTE Flag1[1] = { 0x01 };
// 	GT24C64_Programming(0xA8D, Flag1, 1);//写入Flag
// 	if (!GT24C64_Programming(0xA8E, DCCData1, 103))//写入Proc2数值
// 	{
// 		info.Format(_T("PDAF读回的数据与写入的数据不一致!"), COLOR_RED);
// 		m_pInterface->AddLog(info);
// 		return 1;
// 	}
// 	else
// 	{
// 		m_pInterface->AddLog(_T("PDAF Step2 烧录成功!"), COLOR_BLUE);
// 	}
// 
// 	return 0;

	if(FrameCnt <= 3)
	{
		FrameCnt++;
		return -2;
	}
	else
		FrameCnt = 0;

	// 获取RAW BUF
	USHORT* raw_buf = m_pInterface->GetImage().Raw_Buf;
	int width = m_pInterface->GetImage().width;
	int height = m_pInterface->GetImage().height;


	int i = 0;
	int j = 0;

	if (CaptureFrame<CaptureSize)
	{
		int i = CaptureFrame;

		//保存Raw图
		CString Path;
		CString str;
		if (!pDlg->m_bSaveRaw)
		{
			Path.Format(_T("%d.raw"), i);
			Path = FileFolderPath + Path;
		}
		else
		{
			str.Format(_T("%d\\"), m_pInterface->GetModuleNum());
			str = FileFolderPath + str;
			CreateDirectory(str,NULL);
			Path.Format(_T("%d.raw"), i);
			Path = str + Path;
		}

        char CurImgFilename[MAX_PATH];
		strcpy_s(CurImgFilename,CT2A(Path.GetBuffer(Path.GetLength())));
#ifndef TestVersion
		if (PathFileExists(Path))
		{
			if (!DeleteFile(Path.GetBuffer(Path.GetLength())))
			{
				m_pInterface->AddLog(_T("Raw图删除失败!"),COLOR_RED);
				return 1;
			}
		}
		SaveRawImage(raw_buf,width,height,Path);
		Sleep(10);
#else
		m_pInterface->AddLog(_T("This is Test Version!"),COLOR_RED);
#endif

		if ((i+1)<CaptureSize)
		{	
			m_pInterface->GetDriverIC()->WriteAF(lensPos[i+1]);
			Sleep(200);
		}

		CaptureFrame++;
		return -1;//continue
	}
	void *p;
	char path_eeprom_bin_file[MAX_PATH];
	char path_eeprom_txt_file[MAX_PATH];
	char path_cal_diagnostic_file[MAX_PATH];
	char path_cal_diagnostic_text_file[MAX_PATH];
	char path_dcc_stack[DCC_STACK_SZ][MAX_PATH];

	dll_config_t    dll_cfg;
	sensor_config_t sensor_cfg;

	dccmap_t        dcc_map;
	pdaf_cal_t      pdaf_cal;

	int16_t  stack_id;
	uint16_t *img_l;
	uint16_t raw_width, raw_height;

	int32_t  rc;
	// number of lens positions to be used for dcc calibration. do not change
   const uint16_t dcc_stack_sz = DCC_STACK_SZ;  
   float tolerance_table[48] = DCC_VAL_TOL_SPARSE;

	CString DCCMap;
	DCCMap = FileFolderPath + _T("eepromdccmap.txt");
	strcpy_s(path_eeprom_txt_file,CT2A(DCCMap.GetBuffer(DCCMap.GetLength())));//File to store PDAF Pattern

	CString DCCmap_Debug;
	DCCmap_Debug = FileFolderPath + _T("eeprom_dccmap.bin");
	strcpy_s(path_eeprom_bin_file,CT2A(DCCmap_Debug.GetBuffer(DCCmap_Debug.GetLength())));//File to store PDAF Pattern

	CString DCCcal_diagnostic;
	DCCcal_diagnostic = FileFolderPath + _T("diagnostic.cal");
	strcpy_s(path_cal_diagnostic_file,CT2A(DCCcal_diagnostic.GetBuffer(DCCcal_diagnostic.GetLength())));//File to store PDAF Pattern

	DCCcal_diagnostic = FileFolderPath + _T("diagnostic.txt");
	strcpy_s(path_cal_diagnostic_text_file,CT2A(DCCcal_diagnostic.GetBuffer(DCCcal_diagnostic.GetLength())));//File to store PDAF Pattern

	//////////////////////////////////////////////////////////////////////////

	get_dll_cfg(&dll_cfg);
	dll_cfg.chart_type = LINECHART;
	memcpy(&dll_cfg.dcc_tolerance_table, tolerance_table, sizeof(tolerance_table));
	// allocate scratch buffer for calibration dll
	for (i=0;i<5;i++)
	{
		p = PDAF_Cal_create( &dll_cfg ); 
		if( p != NULL ) 
		{
			i=0;
			break;
		}
		else 
			PDAF_Cal_destroy( p );
	}
	if (i!=0)
	{
		m_pInterface->AddLog(_T("dll memory allocation error!"),COLOR_RED);
		return FALSE;
	}

	//print string
	char version_info[4096];
	PDAF_Cal_get_lib_version(version_info);
	info.Format(_T("%s"),CA2T(version_info));
	m_pInterface->AddLog(info);

	char feature_info[4096];
	PDAF_Cal_get_lib_features(feature_info);
	info.Format(_T("%s"),CA2T(feature_info));
	m_pInterface->AddLog(info);

	//3L8
	sensor_cfg.image_width      = 4208;
	sensor_cfg.image_height     = 3120;
	sensor_cfg.bit_depth        = 10;
	sensor_cfg.black_lvl        = 64;
	sensor_cfg.cfa              = GRBG;
	sensor_cfg.pd_block_width=64;
	sensor_cfg.pd_block_height=64;
	sensor_cfg.pd_block_start_x=24;
	sensor_cfg.pd_block_start_y=24;
	sensor_cfg.pd_block_n_pd_pairs=16;
	uint16_t pd_block_l_x[] =
	{ 4, 4, 8, 8,20,20,24,24,36,36,40,40,52,52,56,56};

	uint16_t pd_block_l_y[] = 
	{11,59,23,47,15,55,27,43,27,43,15,55,23,47,11,59};	

	uint16_t pd_block_r_x[] =
	{ 4, 4, 8, 8,20,20,24,24,36,36,40,40,52,52,56,56};

	uint16_t pd_block_r_y[] =
	{ 7,63,27,43,11,59,31,39,31,39,11,59,27,43, 7,63};

	memcpy(sensor_cfg.pd_block_l_x, pd_block_l_x, 16*sizeof(uint16_t));
	memcpy(sensor_cfg.pd_block_l_y, pd_block_l_y, 16*sizeof(uint16_t)); 
	memcpy(sensor_cfg.pd_block_r_x, pd_block_r_x, 16*sizeof(uint16_t)); 
	memcpy(sensor_cfg.pd_block_r_y, pd_block_r_y, 16*sizeof(uint16_t)); 

	////
// 	sensor_cfg.image_width      = 4208;
// 	sensor_cfg.image_height     = 3120;
// 	sensor_cfg.bit_depth        = 10;
// 	sensor_cfg.black_lvl        = 64;
// 	sensor_cfg.cfa				=RGGB;
// 	sensor_cfg.pd_block_width	=32;
// 	sensor_cfg.pd_block_height	=32;
// 	sensor_cfg.pd_block_start_x	=24;
// 	sensor_cfg.pd_block_start_y	=24;
// 	sensor_cfg.pd_block_n_pd_pairs=4;
// 
// 	uint16_t pd_block_r_x[] = {2,9,18,25};
// 	uint16_t pd_block_r_y[] = {5,24,5,24};
// 	uint16_t pd_block_l_x[] = {1,10,17,26};
// 	uint16_t pd_block_l_y[] = {8,21,8,21};
// 
// 	memcpy(sensor_cfg.pd_block_l_x, pd_block_l_x, sensor_cfg.pd_block_n_pd_pairs*sizeof(uint16_t));
// 	memcpy(sensor_cfg.pd_block_l_y, pd_block_l_y, sensor_cfg.pd_block_n_pd_pairs*sizeof(uint16_t)); 
// 	memcpy(sensor_cfg.pd_block_r_x, pd_block_r_x, sensor_cfg.pd_block_n_pd_pairs*sizeof(uint16_t)); 
// 	memcpy(sensor_cfg.pd_block_r_y, pd_block_r_y, sensor_cfg.pd_block_n_pd_pairs*sizeof(uint16_t)); 
	////

	raw_width = width;
	raw_height = height;
	img_l = NULL;
	img_l = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));
	if (img_l == NULL ) 
	{
		return -1;
	}

	/////////////////////////////////////////////////////////////////////////
	// PER MODULE CALIBRATION
	CString rawfilename;
	// initialize return code
	rc = 0;
	gain_map.cal_version=4;
	for( stack_id=0; stack_id < dcc_stack_sz; stack_id++ ) 
	{
		if (!pDlg->m_bSaveRaw)
		{
			info.Format(_T("%d.raw"), stack_id);
			rawfilename = FileFolderPath + info;
			strcpy_s(path_dcc_stack[stack_id],CT2A(rawfilename.GetBuffer(rawfilename.GetLength())));//File to store PDAF Pattern
		}
		else
		{
			info.Format(_T("%d\\%d.raw"),m_pInterface->GetModuleNum(),stack_id);
			rawfilename = FileFolderPath + info;
			strcpy_s(path_dcc_stack[stack_id],CT2A(rawfilename.GetBuffer(rawfilename.GetLength())));//File to store PDAF Pattern
		}


		if (read_raw_file(path_dcc_stack[stack_id], 
			img_l, raw_width, raw_height) ) 
		{
			free(img_l); 
			PDAF_Cal_destroy( p );
			m_pInterface->AddLog(_T("read dcc calibration test image失败!"),COLOR_RED);
			return 1;
		}

		// move lens to next position here!
		// to save time move lens and read next raw 
		// while running PDAF_CAL_dcc_stack
		// add dcc calibration image to stack
		rc |= PDAF_Cal_add_raw(p, stack_id, lensPos[stack_id],
			img_l, &sensor_cfg, &gain_map);

		if (rc != 0) 
		{
			free(img_l); 
			PDAF_Cal_destroy( p );
			print_rc(rc);
			return 1;
		}
	}


	// proceed to dcc calibration if stacking returned no error
	if( rc == 0 ) 
	{
		rc |= PDAF_Cal_get_dccmap(p, &dcc_map, path_cal_diagnostic_file);
	}

	if (rc != 0) 
	{
		free(img_l); 
		PDAF_Cal_destroy( p );
		print_rc(rc);
		return 1;
	}
	// get OTP/EEPROM block for write
	PDAF_Cal_get_calibration_block( &gain_map, &dcc_map, &pdaf_cal);

	// program pdaf_cal into EEPROM

	// generate .bin f
	write_cal_buf_bin_file( path_eeprom_bin_file, (char *) &pdaf_cal, sizeof(pdaf_cal));

	// save calibration buffer as text file for viewing
	write_cal_buf_txt_file(path_eeprom_txt_file, &pdaf_cal);

	cal_dump_to_txt(path_cal_diagnostic_file, path_cal_diagnostic_text_file);
	// do not write NVM if return code indicates error


	/////////////////////////////////////////////////////////////////////////
	// DE-INIT
	free(img_l); 

	PDAF_Cal_destroy( p );

	//////////////////////////////Burning//////////////////////////////////////
	BYTE DCCData[103] = {0};
	DCCData[0] = dcc_map.q_format>>8;
	DCCData[1] = dcc_map.q_format&0xff;
	DCCData[2] = dcc_map.width>>8;
	DCCData[3] = dcc_map.width&0xff;
	DCCData[4] = dcc_map.height>>8;
	DCCData[5] = dcc_map.height&0xff;
	for(i=0; i<48; i++)
	{
		DCCData[6+i*2] = dcc_map.dm[i]>>8;
		DCCData[7+i*2] = dcc_map.dm[i]&0xff;
		info.Format(_T("DCCData[%d]=%d"),6+i*2,dcc_map.dm[i]>>8,COLOR_RED);
		m_pInterface->AddLog(info);
		info.Format(_T("DCCData[%d]=%d"),7+i*2,dcc_map.dm[i]&0xff,COLOR_RED);
		m_pInterface->AddLog(info);
	}
	//checksum otp
// 	BYTE GainMapandDCCData[994] = {0};
// 	FP5518_Read(0x712, GainMapandDCCData, 993);
// 	for(int i=0;i<102;i++)
// 	{
// 		GainMapandDCCData[891+i]=DCCData[i];
// 	}
// 	GainMapandDCCData[993] = SumCalc(GainMapandDCCData+1, 992, 255, 0);
//	DCCData[102] = GainMapandDCCData[992];


#if 0
		DCCData[102]=pdaf_cal.checksum;
#else
	DCCData[102]=SumCalc(DCCData, 102, 255, 0);
#endif

	//将之前的烧录数据读出全部一次重写
// 	BYTE AFData[8]={0};
// 	BYTE GainMapData[891]={0};
// 	ZC533_Read(0x0016, AFData, 8);
// 	ZC533_Read(0x0708, GainMapData, 891);
// 	ZC533_All_Erase();
// 	Sleep(20);
// 	if(!ZC533_Programming(0x0016,AFData,8))
// 	{
// 		info.Format(_T("AF读回的数据重写错误!"),COLOR_RED);
// 		m_pInterface->AddLog(info);
// 		return 1;
// 	}
	//读出AF数据
// 	BYTE AFData[8]={0};
// 	if(!FP5518_Read(0x0016, AFData, 8))
// 	{
// 		info.Format(_T("AFData读取失败!"),COLOR_RED);
// 		m_pInterface->AddLog(info);
// 	}
// 	Sleep(100);
// 	if(!FP5518_All_Erase())
// 	{
// 		info.Format(_T("擦除数据失败!"),COLOR_RED);
// 		m_pInterface->AddLog(info);
// 	}
// 	if(!FP5518_Programming(0x0016,AFData,8))
// 	{
// 		info.Format(_T("AF读回数据写入失败！"),COLOR_RED);
// 		m_pInterface->AddLog(info);
// 	}

// 	if(!FP5518_Programming(0x0708,GainMapandDCCData,994))
// 	{
// 		info.Format(_T("PDAFData读回的数据重写错误!"),COLOR_RED);
// 		m_pInterface->AddLog(info);
// 		return 1;
// 	}
// 	else
// 	{
// 		m_pInterface->AddLog(_T("PDAF Step2 烧录成功!"),COLOR_BLUE);
// 	}

// 	if(!ZC533_Programming(0x0A83,DCCData,103))
// 	{
// 		info.Format(_T("GainMapData读回的数据重写错误!"),COLOR_RED);
// 		m_pInterface->AddLog(info);
// 		return 1;
// 	}
// 	m_pInterface->AddLog(_T("PDAF Step2 烧录成功!"),COLOR_BLUE);

	BYTE Flag[1] = { 0x01 };
	GT24C64_Programming(0xA8D, Flag, 1);//写入Flag
	if(!GT24C64_Programming(0xA8E,DCCData,103))//写入Proc2数值
	{
		info.Format(_T("PDAF读回的数据与写入的数据不一致!"),COLOR_RED);
		m_pInterface->AddLog(info);
		return 1;
	}
	else
	{
		m_pInterface->AddLog(_T("PDAF Step2 烧录成功!"),COLOR_BLUE);
	}
//	SaveDataIntoFile(dccmap.dm,-1,"DCC_data",48);//waiting

// 	if(!ZC533_Programming(0xAE9,&GainMapandDCCData[992],1))//写入Proc2数值
// 	{
// 		info.Format(_T("PDAF Checksum读回的数据与写入的数据不一致!"),COLOR_RED);
// 		m_pInterface->AddLog(info);
// 		return 1;
// 	}
// 	else
// 	{
// 		m_pInterface->AddLog(_T("PDAF Step2 Checksum 烧录成功!"),COLOR_BLUE);
// 	}

	return 0;
}

void PD_CALI_Qual_2_Inspection::print_rc(unsigned int rc)
{
	// error codes from gain map calibration

	// 0x0001: Memory allocation error
	// 0x0002: Invalid sensor configuration or sensor gain
	// 0x0004: Invalid pd pixel pattern within block
	// 0x0008: Gain Map: pixel values too high for calibration
	// 0x0010: Gain Map: pixel values too low for calibration
	// 0x0020: Gain Map: gain map shows maximum value that is too high
	// 0x0040: DCC: phase disparity values out of bound. invalid input image 
	// 0x0080: DCC: phase disparity values out of order. LEFT and RIGHT pixels may be swapped 
	// 0x0100: DCC: insufficient number of lens sweep prior to dcc calculation
	// 0x0200: DCC: insufficient match between zero phase disparity focus peak

	if (rc & 0x0001)
		m_pInterface->AddLog(_T("\nError code 0x0001: Memory allocation error."),COLOR_RED);
	if (rc & 0x0002)
		m_pInterface->AddLog(_T("\nError code 0x0002: Invalid sensor configuration or sensor gain."),COLOR_RED);
	if (rc & 0x0004)
		m_pInterface->AddLog(_T("\nError code 0x0004: Invalid pd pixel pattern within block."),COLOR_RED);
	if (rc & 0x0008)
		m_pInterface->AddLog(_T("\nError code 0x0008: Gain Map: pd pixel values too high for calibration."),COLOR_RED);
	if (rc & 0x0010)
		m_pInterface->AddLog(_T("\nError code 0x0010: Gain Map: pd pixel values too low for calibration."),COLOR_RED);
	if (rc & 0x0020)
		m_pInterface->AddLog(_T("\nError code 0x0020: Gain Map: gain map shows maximum value that is too high."),COLOR_RED);
	if (rc & 0x0040)
		m_pInterface->AddLog(_T("\nError code 0x0040: DCC: phase disparity values out of bound. invalid input image."),COLOR_RED);
	if (rc & 0x0080)
		m_pInterface->AddLog(_T("\nError code 0x0080: DCC: phase disparity values out of order. LEFT and RIGHT pixels may be swapped."),COLOR_RED);
	if (rc & 0x0100)
		m_pInterface->AddLog(_T("\nError code 0x0100: DCC: insufficient number of lens sweep prior to dcc calculation."),COLOR_RED);
	if (rc & 0x0200)
		m_pInterface->AddLog(_T("\nError code 0x0200: DCC: insufficient match between zero phase disparity focus peak."),COLOR_RED);
	if (rc & 0x0400)
		m_pInterface->AddLog(_T("\nError code 0x0400: DCC: insufficient range of pd values from MACRO to INF."),COLOR_RED);
	if (rc & 0x1000)
		m_pInterface->AddLog(_T("\nError code 0x1000: DCC: Please check line chart condition."),COLOR_RED);	    
	if (rc & 0x2000)
		m_pInterface->AddLog(_T("\nError code 0x2000: DCC: object distance is too close to stack fit boundary."),COLOR_RED);
}

int PD_CALI_Qual_2_Inspection::Display_GmCheckResult(int GainMapFlag)
{
	if (GainMapFlag & 0x0200)
	{
		m_pInterface->AddLog(_T("0x0200: GainMap Calibration, Memory error. "),COLOR_RED);
		return -1;
	}
	if (GainMapFlag & 0x0100)
	{
		m_pInterface->AddLog(_T("0x0100: GainMap Calibration, Input Parameter Error. "),COLOR_RED);
		return -1;
	}
	if (GainMapFlag & 0x0080)
	{
		m_pInterface->AddLog(_T("0x0080: Error, GainMap Calibration, PD signal too weak. "),COLOR_RED);
		return -1;
	}
	if (GainMapFlag & 0x0040)
	{
		m_pInterface->AddLog(_T("0x0040: Error, GainMap Calibration, PD signal too strong. "),COLOR_RED);
		return -1;
	}
	if (GainMapFlag & 0x0020)
	{
		m_pInterface->AddLog(_T("0x0020: Error, GainMap Calibration, Normal Pixel too weak. "),COLOR_RED);
		return -1;
	}
	if (GainMapFlag & 0x0010)
	{
		m_pInterface->AddLog(_T("0x0010: Error, GainMap Calibration, Normal Pixel too strong. "),COLOR_RED);
		return -1;
	}
	if (GainMapFlag & 0x0008)
	{
		m_pInterface->AddLog(_T("0x0008: Error, GainMap Calibration, max of Normal Pixel too weak. "),COLOR_RED);
		return -1;
	}
	if (GainMapFlag & 0x0004)
	{
		m_pInterface->AddLog(_T("0x0004: Error, GainMap Calibration, pattern file error. "),COLOR_RED);
		return -1;
	}

	if (GainMapFlag & 0x0002)
	{
		m_pInterface->AddLog(_T("0x0002: Warning, GainMap Calibration, GM value too small. "),COLOR_RED);
		return 0;
	}
	if (GainMapFlag & 0x0001)
	{
		m_pInterface->AddLog(_T("0x0001: Warning, GainMap Calibration, GM value too big. "),COLOR_RED);
		return 0;
	}

	m_pInterface->AddLog(_T("The Gainmap is Normal. "),COLOR_BLUE);
	return 0;
}

int PD_CALI_Qual_2_Inspection::GetAFcode(void)
{
	m_pInterface ->AddLog(_T("Get AF Data Start"),COLOR_BLACK);
	CString info;

	vector<double>* vpStcAF = m_pInterface->GetStorage(_T("AutoFocusStart"));
	vector<double>* vpMacAF = m_pInterface->GetStorage(_T("AutoFocusNear"));
	vector<double>* vpInfAF = m_pInterface->GetStorage(_T("AutoFocusFar"));
	vector<double>* vpMidAF = m_pInterface->GetStorage(_T("AutoFocusMiddle"));

	if (vpInfAF == NULL || vpInfAF->size() == 0)
	{
		Inf_code = 0;
		info = _T("远景Code值为空");
		m_pInterface->AddLog(info, COLOR_RED);
     	return -1; //cnw
	}
	else
		Inf_code = (USHORT)vpInfAF->at(0);

	if(Inf_code > pDlg->m_InfH || Inf_code <  pDlg->m_InfL)
	{
		info.Format(_T("远景Code(%d)超标,请重测远景"), Inf_code);
		m_pInterface->AddLog(info, COLOR_RED);
		return -1;
	}

	if (vpMacAF == NULL || vpMacAF->size() == 0)
	{
		Mac_code = 0;
		info = _T("近景Code值为空");
		m_pInterface->AddLog(info, COLOR_RED);
    	return -1; //cnw
	}
	else
		Mac_code = (USHORT)vpMacAF->at(0);

	if (Mac_code > pDlg->m_MacH || Mac_code <  pDlg->m_MacL)
	{
		info.Format(_T("近景Code(%d)超标,请重测近景"), Mac_code);
		m_pInterface->AddLog(info, COLOR_RED);
		return -1;
	}

	if (vpMidAF == NULL || vpMidAF->size() == 0)
	{
		Mid_code = 0;
		info = _T("中景Code值为空");
		m_pInterface->AddLog(info, COLOR_RED);
    	return -1; //cnw
	}
	else
		Mid_code = (USHORT)vpMidAF->at(0);

	if (Mid_code > pDlg->m_MidH || Mid_code <  pDlg->m_MidL)
	{
		info.Format(_T("中景Code(%d)超标,请重测中景"), Mid_code);
		m_pInterface->AddLog(info, COLOR_RED);
		return -1;
	}

	if (Inf_code >= Mac_code-pDlg->m_AFDif || Inf_code>Mac_code)
	{
		info.Format(_T("远景Code(%d)近景Code(%d)太接近，请重新对焦"), Inf_code, Mac_code);
		m_pInterface->AddLog(info, COLOR_RED);
    	return -1; //cnw
	}

	info.Format(_T("远景Code: %d\n中景Code: %d\n近景Code: %d"), Inf_code, Mid_code,Mac_code);
	m_pInterface->AddLog(info, COLOR_BLUE);

	/////////////////////////temp///////////////////////////////////////////
// 	Inf_code = 180;//cnw
// 	Mid_code = 235;//cnw
// 	Mac_code = 600;//cnw
	//////////////////////////////////////////////////////////////////////////

	m_pInterface ->AddLog(_T("Get AF Data End"));
	return 0;	
}

void PD_CALI_Qual_2_Inspection::SaveRawImage(USHORT* raw_buf, int width, int height, CString& path)
{
//	if (!pDlg->m_bSaveRaw) return ;

	FILE* file;
	if (_wfopen_s(&file, path.GetBuffer() , _T("wb+")) == 0)
	{
		int cnt = fwrite( (void*)raw_buf, 2, width*height, file);	// 返回实际写入的数据项个数count
		fclose(file);

		CString str;
		if (cnt == width*height)
		{
			str = path + _T(" saved");
			m_pInterface->AddLog(str);
		}
		else
		{
			str = path + _T(" Written Error");
			m_pInterface->AddLog(str, COLOR_RED);
		}
	}
	else
	{
		CString str = path + _T(" Written Error");
		m_pInterface->AddLog(str, COLOR_RED);
	}
}

int PD_CALI_Qual_2_Inspection::EEPROM_S5K3M2(UCHAR* data, int size)
{
	// 存储到EEPROM
	BOOL bOK = TRUE;
	CString info;
	if (m_PageSize == 0 || m_Dev_Addr == 0 ) 
	{
		m_pInterface->AddLog(_T("参数设置错误"), COLOR_RED);
		return -1;
	}

	int remain = m_Reg_Addr % m_PageSize;
	int i = 0;
	for (i=0;i<32-remain;i++)
	{
		m_pInterface->I2C_Write(BYTE(m_Dev_Addr),USHORT(m_Reg_Addr+i),data[i],m_I2cMode);
		Sleep(2);
	}
	int quotient = (size-(32-remain))/m_PageSize;	// 商
	int remainder = (size-(32-remain))%m_PageSize;// 余数
	int j=i;
	int reg = m_Reg_Addr+i;
	int dev = m_Dev_Addr;

	while(quotient)
	{
		m_pInterface->I2C_MultiBytesWrite(BYTE(dev), USHORT(reg), 2, &data[j], m_PageSize);
		Sleep(10*m_SleepTime);
		quotient -- ;
		j += m_PageSize;
		reg += m_PageSize;
	}


	if (remainder != 0)
	{
		m_pInterface->I2C_MultiBytesWrite(BYTE(dev), USHORT(reg), 2, &data[j], remainder);
		Sleep(10*m_SleepTime);
	}

	// 读出数据验证写入正确性

	UCHAR* Rdata = new UCHAR[size];
	USHORT tmp = 0;
	for (i=0;i<32-remain;i++)
	{
		m_pInterface->I2C_Read(BYTE(m_Dev_Addr), USHORT(m_Reg_Addr + i), &tmp, m_I2cMode);
		Rdata[i] = (UCHAR)tmp;
		Sleep(2);
	}
	quotient =(size-(32-remain))/m_PageSize;	// 商
	remainder = (size-(32-remain))%m_PageSize;// 余数

	j=i;
	reg = m_Reg_Addr+i;
	dev = m_Dev_Addr;
	while(quotient)
	{
		BOOL bRet = m_pInterface->I2C_MultiBytesRead(BYTE(dev), USHORT(reg), 2, &Rdata[j], m_PageSize, TRUE);
		Sleep(10*m_SleepTime);
		quotient -- ;
		j += m_PageSize;
		reg += m_PageSize;
	}

	if (remainder != 0)
	{
		m_pInterface->I2C_MultiBytesRead(BYTE(dev), USHORT(reg), 2, &Rdata[j], remainder, TRUE);
		Sleep(10*m_SleepTime);
	}

	for (int i=0; i<size; i++)
	{
		if (data[i] != Rdata[i])
		{
			bOK = FALSE;
			info.Format(_T("%d: 0x%02X != 0x%02X"),i,data[i],Rdata[i]);
			m_pInterface->AddLog(info, COLOR_RED);
		}
	}

	SAFE_DELETE_ARRAY(Rdata);
	if (bOK)
	{
		m_pInterface->AddLog(_T("Eeprom Saving Successful"), COLOR_BLUE);
		return 0;
	}
	else
	{
		m_pInterface->AddLog(_T("Eeprom Saving Failed"), COLOR_RED);
		return -2;
	}

}

int PD_CALI_Qual_2_Inspection::EEPROM_S5K3L8(UCHAR* data, int size)
{
	m_pInterface->AddLog(_T("OTP Proc2 Data Burning Start!"));
	int result = 0;
	int i = 0;
	int proc2sum = 0;
	for (i=0; i<size; i++)
	{
		proc2sum += data[i];
	}
	proc2sum = proc2sum % 255 + 1;
	BYTE* ps = new BYTE[1];
	ps[0] = proc2sum;

	if (!GT24C64_Programming(m_Reg_Addr,data,size))
	{
		m_pInterface->AddLog(_T("PDAF Proc2 Data Burning Fail！"), COLOR_RED);
		result |= -1;
	}
	if (!GT24C64_Programming(0x0DBE,ps,1))
	{
		m_pInterface->AddLog(_T("PDAF Proc2 Checksum Burning Fail！"), COLOR_RED);
		result |=  -1;
	}

	BYTE* Read_Data = new BYTE[size];
	if (!GT24C64_Read(m_Reg_Addr,Read_Data,size))
	{
		m_pInterface->AddLog(_T("PDAF Proc2 Data Read Fail！"), COLOR_RED);
		result |=  -1;
	}
	else
	{
		if (!CheckEqual(data,Read_Data,size))
		{
			m_pInterface->AddLog(_T("PDAF Proc2 Data WriteRead not equal！"), COLOR_RED);
			result |= -1;
		}
	}

	BYTE* Read_Sum = new BYTE[1];
	if (!GT24C64_Read(0x0DBE,Read_Sum,1))
	{
		m_pInterface->AddLog(_T("PDAF Proc2 Sum Read Fail！"), COLOR_RED);
		result |= -1;
	}
	else
	{
		if (!CheckEqual(ps,Read_Sum,1))
		{
			m_pInterface->AddLog(_T("PDAF Proc2 Sum WriteRead not equal！"), COLOR_RED);
			result |= -1;
		}
	}
	result |= 0;
	
	m_pInterface->AddLog(_T("OTP Proc2 Data Burning Success!"));
	if (result == 0)
	{
		SAFE_DELETE_ARRAY(ps);
		SAFE_DELETE_ARRAY(Read_Data);
		SAFE_DELETE_ARRAY(Read_Sum);
		return 0;
	}
	else
	{
		SAFE_DELETE_ARRAY(ps);
		SAFE_DELETE_ARRAY(Read_Data);
		SAFE_DELETE_ARRAY(Read_Sum);
		return -1;
	}
}

void PD_CALI_Qual_2_Inspection::Raw10toRaw8(USHORT *Src, BYTE *Dest, long number)//Raw10
{
	long DesCount = 0;

	for (long i = 0; i < number; i++ )//i = i + 5
	{
		Dest[i] = ((Src[i]>>2) & 0xff);
	}
}

CString PD_CALI_Qual_2_Inspection::GetModulePath()
{
	int  i    = 0;
	int  len  = 0;
	bool flag = false;
	CString myStr;
	TCHAR ModulePath[MAX_PATH] = {0};
	char path[512] = {0};
	GetModuleFileNameW(NULL, ModulePath, MAX_PATH);//return  real  lenghth
	PathRemoveFileSpec(ModulePath);
	myStr.Format(ModulePath);
	return myStr;
}

void PD_CALI_Qual_2_Inspection::SaveCurrentRaw10(CString filename, USHORT *buff, int width, int height)
{
	FILE *file;

	if (_wfopen_s(&file, filename.GetBuffer() , _T("wb+")) == 0)
	{
		int cnt = fwrite( (void*)buff, 2, width*height, file);	// 返回实际写入的数据项个数count
		fclose(file);
		CString str;
		if (cnt == width*height)
		{
			str = filename + _T(" saved");
			m_pInterface->AddLog(str);
		}
		else
		{
			str = filename + _T(" Written Error");
			m_pInterface->AddLog(str, COLOR_RED);
		}
	}
	else 
	{
		CString str = filename + _T(" Written Error");
		m_pInterface->AddLog(str, COLOR_RED);
	}
	return;
}

void PD_CALI_Qual_2_Inspection::SaveCurrentRaw8(CString filename, BYTE *buff, int width, int height)
{
	FILE *file;

	if (_wfopen_s(&file, filename.GetBuffer() , _T("wb+")) == 0)
	{
		int cnt = fwrite( (void*)buff, 1, width*height, file);	// 返回实际写入的数据项个数count
		fclose(file);
		CString str;
		if (cnt == width*height)
		{
			str = filename + _T(" saved");
			m_pInterface->AddLog(str);
		}
		else
		{
			str = filename + _T(" Written Error");
			m_pInterface->AddLog(str, COLOR_RED);
		}
	}
	else 
	{
		CString str = filename + _T(" Written Error");
		m_pInterface->AddLog(str, COLOR_RED);
	}

	return;
}

BOOL PD_CALI_Qual_2_Inspection::CheckEqual(BYTE *data1, BYTE *data2, int num)
{
	CString info;
	if (data1 == NULL || data2 == NULL)
	{
		return FALSE;
	}

	if (num <= 0)
	{
		return FALSE;
	}	

	for (int i=0; i<num; i++)
	{
		if (data1[i] != data2[i])
		{
			info.Format(_T("Data %d Check Error! 0x%x != 0x%x"),i,data1[i],data2[i]);
			m_pInterface->AddLog(info, COLOR_RED);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL PD_CALI_Qual_2_Inspection::GT24C64_Read(USHORT address,unsigned char *Data,int size)
{
	UCHAR slaveid = 0xA0;//cnw
	int i=0;
	int group=0;
	int num=0;
	//	byte *CheckData=new byte[size];
	int Count= address%32;
	Count=32-Count;
	if(size-Count>0)
	{
		group = (size-Count)/32;
		num=(size-Count)%32;
	}
	else
	{
		group=0;
		num=size;
		Count=0;
	}
	if(Count>0)
		m_pInterface->I2C_MultiBytesRead(slaveid, address, 2, Data, Count);
	Sleep(6);
	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesRead(slaveid, address + (Count + i * 32), 2, Data + (Count + i * 32), 32);
		Sleep(2);
	}
	m_pInterface->I2C_MultiBytesRead(slaveid, address + (Count + i * 32), 2, Data + (Count + i * 32), num);
	return TRUE;

}

BOOL PD_CALI_Qual_2_Inspection::GT24C64_Programming(USHORT address,unsigned char *Data,int size)
{
	UCHAR salveid = 0xA0;//cnw
	int i=0;
	int group=0;
	int num=0;
	BYTE *CheckData=new BYTE[size];
	Sleep(2);
	int Count= address%32;
	Count=32-Count;
	if(size-Count>0)
	{
		group = (size-Count)/32;
		num=(size-Count)%32;
	}
	else
	{
		group=0;
		num=size;
		Count=0;
	}
	m_pInterface->I2C_MultiBytesWrite(salveid, address, 2, Data, Count);
	Sleep(2);

	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesWrite(salveid, address + (Count + i * 32), 2, Data + (Count + i * 32), 32);
		Sleep(2);
	}
	m_pInterface->I2C_MultiBytesWrite(salveid, address + (Count + i * 32), 2, Data + (Count + i * 32), num);
	Sleep(2);
	if(Count>0)
		m_pInterface->I2C_MultiBytesRead(salveid, address, 2, CheckData, Count);

	Sleep(2);
	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesRead(salveid, address + (Count + i * 32), 2, CheckData + (Count + i * 32), 32);
		Sleep(2);
	}
	m_pInterface->I2C_MultiBytesRead(salveid, address + (Count + i * 32), 2, CheckData + (Count + i * 32), num);
	for (i=0;i<size;i++)
	{
		if (CheckData[i]!=Data[i])
		{
			SAFE_DELETE_ARRAY(CheckData);
			return FALSE;
		}
	}
	SAFE_DELETE_ARRAY(CheckData);
	return TRUE;
}


BOOL PD_CALI_Qual_2_Inspection::ZC533_Page_Erase(BYTE page)
{
	//m_pInterface->AddDebugLog(L"TKU8341M WriteEepromPage_Byte");
	ZC533_Erase_lock = ZC533_LastPage - page;
	ZC533_LastPage = page;
	BOOL bRet = TRUE;
	if (ZC533_Erase_lock != 0)//不允许连续两次erase 同一page  ZC533_Erase_lock != 0
	{
		bRet &= m_pInterface->I2C_Write(0xB0, ((ZC533_PAGE_ERASE<<14)|(page<<6)), 0, I2CMODE_16_8);
		Sleep(6*m_SleepTime);
	}

	return bRet;
}

BOOL PD_CALI_Qual_2_Inspection::FM24C64_Read(USHORT address,unsigned char *Data,int size)
{
	int i=0;
	int group=0;
	int num=0;

	int Count= address%32;
	Count=32-Count;
	if(size-Count>0)
	{
		group = (size-Count)/32;
		num=(size-Count)%32;
	}
	else
	{
		group=0;
		num=size;
		Count=0;
	}
	if(Count>0)
		m_pInterface->I2C_MultiBytesRead(0xA0,address,2,Data,Count);
	Sleep(30);
	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,Data+(Count+i*32),32);
		Sleep(10);
	}
	m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,Data+(Count+i*32),num);
	return TRUE;

}

BOOL PD_CALI_Qual_2_Inspection::FM24C64_Programming(USHORT address,unsigned char *Data,int size)
{
	int i=0;
	int group=0;
	int num=0;
	BYTE* CheckData=new BYTE[size]();
	BOOL bRet = TRUE;

	do 
	{	
		Sleep(10);
		int Count= address%32;
		Count=32-Count;
		if(size-Count>0)
		{
			group = (size-Count)/32;
			num=(size-Count)%32;
		}
		else
		{
			group=0;
			num=size;
			Count=0;
		}
		m_pInterface->I2C_MultiBytesWrite(0xA0,address,2,Data,Count);
		Sleep(10);

		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesWrite(0xA0,address+(Count+i*32),2,Data+(Count+i*32),32);
			Sleep(10);
		}
		m_pInterface->I2C_MultiBytesWrite(0xA0,address+(Count+i*32),2,Data+(Count+i*32),num);
		Sleep(10);
		if(Count>0)
			m_pInterface->I2C_MultiBytesRead(0xA0,address,2,CheckData,Count);

		Sleep(10);
		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,CheckData+(Count+i*32),32);
			Sleep(10);
		}
		m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,CheckData+(Count+i*32),num);
		for (i=0;i<size;i++)
		{
			if (CheckData[i]!=Data[i])
			{
				bRet = FALSE;
				break;
			}
		}
	} while (0);

	SAFE_DELETE_ARRAY(CheckData);
	return bRet;
}

BOOL PD_CALI_Qual_2_Inspection::ZC533_All_Erase()
{
	BOOL bRet = TRUE;
	bRet &= m_pInterface->I2C_Write(0xB0, (ZC533_ALL_ERASE<<14), 0, I2CMODE_16_8);
	Sleep(6*m_SleepTime);
	return bRet;
}

BOOL PD_CALI_Qual_2_Inspection::ZC533_Read(USHORT address,unsigned char *Data,int size)
{
	int pagesize = 64;
	int i=0;
	int j=0;
	int group = 0;
	int num = 0;
	int retgroup = 0;
	int retnum = 0;
	int retsize = 0;
	int Totalretsize = 0;
	int page = address/pagesize;
	int reg =address%pagesize;

	int Count=pagesize-page;
	int ret = size-Count;
	if(ret>0)
	{
		group = (size-Count)/pagesize;
		num=(size-Count)%pagesize;
	}
	else
	{
		group=0;
		num=size;
		Count=0;
	}
	retsize = pagesize-reg;
	Totalretsize = size- retsize;
	retgroup = Totalretsize/pagesize;
	retnum = Totalretsize%pagesize;

	if (ret>0)
	{
		if(Count>0)
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|(page<<6)|(reg&0x3f)),2,Data,retsize);
		Sleep(10);

		for (i=0;i<retgroup;i++)
		{
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),pagesize);//|(reg&0x3f)   +(Count+i*pagesize)
			Sleep(10);
		}
		m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),retnum);//|(reg&0x3f)   +(Count+i*pagesize)
	}
	else
	{
		m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page)<<6)|(reg&0x3f))+(Count+i*32),2,Data+(Count+i*32),num);
		Sleep(10);
	}

	return TRUE;
}

BOOL PD_CALI_Qual_2_Inspection::ZC533_Programming(USHORT address,unsigned char *Data,int size)//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page
{
	int pagesize = 64;
	int i=0;
	int j=0;
	int group = 0;
	int num = 0;
	int retgroup = 0;
	int retnum = 0;
	int retsize = 0;
	int Totalretsize = 0;
	BYTE* CheckData=new BYTE[size]();
	int page = address/pagesize;
	int reg =address%pagesize;

	int Count=pagesize-page;
	int ret = size-Count;
	if(ret>0)
	{
		group = (size-Count)/pagesize;
		num=(size-Count)%pagesize;
	}
	else
	{
		group=0;
		num=size;
		Count=0;
	}
	retsize = pagesize-reg;
	Totalretsize = size- retsize;
	retgroup = Totalretsize/pagesize;
	retnum = Totalretsize%pagesize;

	BOOL bRet = TRUE;
	do 
	{
		if (ret>0)
		{
			if (!ZC533_Page_Erase(page))	
			{
				bRet = FALSE;
				break;
			}
			m_pInterface->I2C_MultiBytesWrite(0xB0,((ZC533_WRITE<<14)|(page<<6)|(reg&0x3f)),2,Data,retsize);
			Sleep(10);

			for (i=0;i<retgroup;i++)
			{
				if (!ZC533_Page_Erase(page+i+1))	
				{
					bRet = FALSE;
					break;
				}
				// 			for (j=0; j<pagesize; j++)//如果64个连续写不进，可以考虑改成32个连续写
				// 			{
				m_pInterface->I2C_MultiBytesWrite(0xB0,((ZC533_WRITE<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),pagesize);//|(j&0x3f)
				Sleep(10);
				//				}
			}
			if (!ZC533_Page_Erase(page+i+1))	
			{
				bRet = FALSE;
				break;
			}
			m_pInterface->I2C_MultiBytesWrite(0xB0,((ZC533_WRITE<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),retnum);//|(reg&0x3f))+(Count+i*pagesize)
			Sleep(10);
		}
		else
		{
			if (!ZC533_Page_Erase(page))	
			{
				bRet = FALSE;
				break;
			}
			m_pInterface->I2C_MultiBytesWrite(0xB0,((ZC533_WRITE<<14)|(page<<6)|(reg&0x3f))+(Count+i*pagesize),2,Data+(Count+i*pagesize),num);
			Sleep(10);
		}
		////////////////////////////////Read//////////////////////////////////////////
		if (ret>0)
		{
			if(Count>0)
				m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|(page<<6)|(reg&0x3f)),2,CheckData,retsize);
			Sleep(5);

			for (i=0;i<retgroup;i++)
			{
				m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,CheckData+(retsize+i*pagesize),pagesize);//|(reg&0x3f)   +(Count+i*pagesize)
				Sleep(5);
			}
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,CheckData+(retsize+i*pagesize),retnum);//|(reg&0x3f)   +(Count+i*pagesize)
			Sleep(5);
			for (i=0;i<size;i++)
			{
				if (CheckData[i]!=Data[i])
				{
					info.Format(_T("读写对比失败! 第%d数据Write:%x, Read:%x"),i,Data[i],CheckData[i]);
					m_pInterface->AddLog(info,COLOR_RED);
					bRet = FALSE;
					break;
				}
			}
		}
		else
		{
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page)<<6)|(reg&0x3f))+(Count+i*32),2,CheckData+(Count+i*32),num);
			Sleep(5);
			for (i=0;i<size;i++)
			{
				if (CheckData[i]!=Data[i])
				{
					info.Format(_T("读写对比失败! 第%d数据Write:%x, Read:%x"),i,Data[i],CheckData[i]);
					m_pInterface->AddLog(info,COLOR_RED);
					bRet = FALSE;
					break;
				}
			}
		}
	} while (0);

	SAFE_DELETE_ARRAY(CheckData);
	return bRet;
}

BOOL PD_CALI_Qual_2_Inspection::DW9763_Page_Erase(BYTE page)
{
	BOOL bRet = TRUE;

	bRet &= m_pInterface->I2C_Write(0xB0, ((1<<7)|(page<<2)|(0x02)), 0, I2CMODE_8_8);
	Sleep(4*m_SleepTime);

	return bRet;
}

BOOL PD_CALI_Qual_2_Inspection::DW9763_All_Erase()
{
	BOOL bRet = TRUE;
	bRet &= m_pInterface->I2C_Write(0xB0, 0x81, 0xee, I2CMODE_8_8);
	Sleep(15*m_SleepTime);

	m_pInterface->AddLog(_T("DW9763 All Data Erased!"),COLOR_LIGHTGOLDENRODYELLOW);
	return bRet;
}

BOOL PD_CALI_Qual_2_Inspection::DW9763_Read(USHORT address,unsigned char *Data,int size)
{
	int i=0;
	int group=0;
	int num=0;
	//	byte *CheckData=new byte[size];
	int Count= address%32;
	Count=32-Count;
	if(size-Count>0)
	{
		group = (size-Count)/32;
		num=(size-Count)%32;
	}
	else
	{
		group=0;
		num=size;
		Count=0;
	}
	if(Count>0)
		m_pInterface->I2C_MultiBytesRead(0xB0,address,2,Data,Count);
	Sleep(30);
	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,Data+(Count+i*32),32);
		Sleep(10);
	}
	m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,Data+(Count+i*32),num);
	return TRUE;
}

BOOL PD_CALI_Qual_2_Inspection::DW9763_Programming(USHORT address,unsigned char *Data,int size)//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page
{
	int i=0;
	int group=0;
	int num=0;
	BYTE* CheckData=new BYTE[size]();
	BOOL bRet = TRUE;

	do 
	{	
		Sleep(10);
		int Count= address%32;
		Count=32-Count;
		if(size-Count>0)
		{
			group = (size-Count)/32;
			num=(size-Count)%32;
		}
		else
		{
			group=0;
			num=size;
			Count=0;
		}
		m_pInterface->I2C_MultiBytesWrite(0xB0,address,2,Data,Count);
		Sleep(10);

		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesWrite(0xB0,address+(Count+i*32),2,Data+(Count+i*32),32);
			Sleep(10);
		}
		m_pInterface->I2C_MultiBytesWrite(0xB0,address+(Count+i*32),2,Data+(Count+i*32),num);
		Sleep(10);
		if(Count>0)
			m_pInterface->I2C_MultiBytesRead(0xB0,address,2,CheckData,Count);

		Sleep(10);
		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,CheckData+(Count+i*32),32);
			Sleep(10);
		}
		m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,CheckData+(Count+i*32),num);
		for (i=0;i<size;i++)
		{
			if (CheckData[i]!=Data[i])
			{
				bRet = FALSE;
				break;
			}
		}
	} while (0);

	SAFE_DELETE_ARRAY(CheckData);
	return bRet;
}

BOOL PD_CALI_Qual_2_Inspection::DW9767_All_Erase()
{
	BOOL bRet = TRUE;

	bRet &= m_pInterface->I2C_Write(0x18, 0x02, 0x10, I2CMODE_8_8);
	bRet &= m_pInterface->I2C_Write(0xB0, 0x81, 0xee, I2CMODE_8_8);
	Sleep(15);
	bRet &= m_pInterface->I2C_Write(0x18, 0x02, 0x00, I2CMODE_8_8);

	m_pInterface->AddLog(_T("DW9767 All Data Erased!"),COLOR_LIGHTGOLDENRODYELLOW);
	return bRet;
}

BOOL PD_CALI_Qual_2_Inspection::DW9767_Page_Erase(BYTE page)
{
	BOOL bRet = TRUE;
	bRet &= m_pInterface->I2C_Write(0x18, 0x02, 0x10, I2CMODE_8_8);
	bRet &= m_pInterface->I2C_Write(0xB0, ((1<<7)|(page<<2)|(0x02)), 0, I2CMODE_8_8);
	Sleep(4);
	bRet &= m_pInterface->I2C_Write(0x18, 0x02, 0x00, I2CMODE_8_8);

	return bRet;
}

BOOL PD_CALI_Qual_2_Inspection::DW9767_Read(USHORT address,unsigned char *Data,int size)
{
	int i=0;
	int group=0;
	int num=0;
	//	byte *CheckData=new byte[size];
	int Count= address%32;
	Count=32-Count;
	if(size-Count>0)
	{
		group = (size-Count)/32;
		num=(size-Count)%32;
	}
	else
	{
		group=0;
		num=size;
		Count=0;
	}
	if(Count>0)
		m_pInterface->I2C_MultiBytesRead(0xB0,address,2,Data,Count);
	Sleep(30);
	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,Data+(Count+i*32),32);
		Sleep(10);
	}
	m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,Data+(Count+i*32),num);
	return TRUE;
}

BOOL PD_CALI_Qual_2_Inspection::DW9767_Programming(USHORT address,unsigned char *Data,int size)//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page
{
	m_pInterface->I2C_Write(0x18, 0x02, 0x10, I2CMODE_8_8);
	int i=0;
	int group=0;
	int num=0;
	BYTE* CheckData=new BYTE[size]();
	BOOL bRet = TRUE;

	do 
	{	
		Sleep(10);
		int Count= address%32;
		Count=32-Count;
		if(size-Count>0)
		{
			group = (size-Count)/32;
			num=(size-Count)%32;
		}
		else
		{
			group=0;
			num=size;
			Count=0;
		}
		m_pInterface->I2C_MultiBytesWrite(0xB0,address,2,Data,Count);
		Sleep(10);

		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesWrite(0xB0,address+(Count+i*32),2,Data+(Count+i*32),32);
			Sleep(10);
		}
		m_pInterface->I2C_MultiBytesWrite(0xB0,address+(Count+i*32),2,Data+(Count+i*32),num);
		Sleep(10);
		if(Count>0)
			m_pInterface->I2C_MultiBytesRead(0xB0,address,2,CheckData,Count);

		Sleep(10);
		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,CheckData+(Count+i*32),32);
			Sleep(10);
		}
		m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,CheckData+(Count+i*32),num);
		m_pInterface->I2C_Write(0x18, 0x02, 0x10, I2CMODE_8_8);
		for (i=0;i<size;i++)
		{
			if (CheckData[i]!=Data[i])
			{
				info.Format(_T("DW9761/63/67读写对比失败，序号:%d, 读取值:%d, 写入值:%d"), i, CheckData[i], Data[i]);
				m_pInterface->AddLog(info);
				bRet = FALSE;
				break;
			}
		}
	} while (0);

	SAFE_DELETE_ARRAY(CheckData);
	return bRet;
}

BOOL PD_CALI_Qual_2_Inspection::FP5518_All_Erase()
{
	m_pInterface ->AddLog(_T("开始擦除..."),COLOR_BLACK);
	m_pInterface->I2C_Write(0xB0, 0x81, 0xee, I2CMODE_8_8);
	Sleep(100);
	m_pInterface ->AddLog(_T("EEprom擦除完成"),COLOR_BLACK);
	return TRUE;
}

BOOL PD_CALI_Qual_2_Inspection::FP5518_Read(USHORT address, BYTE *value, int length)
{
	int res;
	if ( length > 32 )
	{
		int group = length/32;
		int num = length%32;
		int i = 0;
		for (i = 0; i < group; i++)
		{
			res = m_pInterface->I2C_MultiBytesRead(0xB0, address+i*32, 2, value+i*32,32);
			Sleep(1);

			if (res != DT_ERROR_OK)
			{
				m_pInterface ->AddLog(_T("读取失败!"),COLOR_RED);
				return false;
			}
		}
		if(num > 0)
		{
			res = m_pInterface->I2C_MultiBytesRead(0xB0, address+i*32, 2, value+i*32,num);
			Sleep(1);
		}
		if (res != DT_ERROR_OK)
		{
			m_pInterface ->AddLog(_T("读取失败!"),COLOR_RED);
			return false;
		}
	}
	else
	{
		res = m_pInterface->I2C_MultiBytesRead(0xB0,address,2,value,length);
		Sleep(1);

		if (res != DT_ERROR_OK)
		{
			m_pInterface ->AddLog(_T("读取失败!"),COLOR_RED);
			return false;
		}
	}

	return true;
}

BOOL PD_CALI_Qual_2_Inspection::FP5518_Programming(USHORT address, BYTE *value, int length)
{
	int res;
	BOOL WriteFlag=TRUE;
	if ( length > 32 )
	{
		int group = length/32;
		int num = length%32;
		int i = 0;
		for (i = 0; i < group; i++)
		{
			res = m_pInterface->I2C_MultiBytesWrite(0xB0,address+i*32,2,value+i*32,32);
			Sleep(1);

			if (res != DT_ERROR_OK)
			{
				m_pInterface ->AddLog(_T("写入失败!"),COLOR_RED);
				WriteFlag=FALSE;
			}
		}
		if(num >0)
		{	
			res = m_pInterface->I2C_MultiBytesWrite(0xB0,address+i*32,2,value+i*32,num);
			Sleep(1);
		}
		if (res != DT_ERROR_OK)
		{
			m_pInterface ->AddLog(_T("写入失败!"),COLOR_RED);
			WriteFlag=FALSE;
		}
	}
	else
	{
		res = m_pInterface->I2C_MultiBytesWrite(0xB0,address,2,value,length);
		Sleep(1);

		if (res != DT_ERROR_OK)
		{
			m_pInterface ->AddLog(_T("写入失败!"),COLOR_RED);
			WriteFlag=FALSE;
		}
	}	
	return WriteFlag;
}