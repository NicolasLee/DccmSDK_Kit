#include "StdAfx.h"
#include "PD_CALI_Qual_3_Inspection.h"
#include "ImageProc.h"



#ifndef Dccm_EXP
    #define Dccm_EXP	__declspec(dllexport)
#endif	/* Dccm_EXP */


extern "C" Dccm_EXP Inspection * CreateInspectionItem(DccmControl_Interface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new PD_CALI_Qual_3_Inspection(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
PD_CALI_Qual_3_Inspection::PD_CALI_Qual_3_Inspection(DccmControl_Interface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	SetConfigPath(pInterface->GetConfigPath());	 //设置CONFIG路径	
	SetName(lpszName);	                             //设置测试项目的名字
	pDlg = new OptionDlg(NULL,this);                 //生成对话框对象
	LoadOption();                                    //读取参数
	pDlg->Create(OptionDlg::IDD,NULL);               //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TET_INF);

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
	//.....
}

PD_CALI_Qual_3_Inspection::~PD_CALI_Qual_3_Inspection(void)
{

}

/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int PD_CALI_Qual_3_Inspection::Initialize()
{
	Inspection::Initialize();
	//TODO:在此添加初始化代码
	bInit = FALSE;
	verifylock = FALSE;
	int i = 0;
		if(!Lock_OneTime)
	{
		ZC533_LastPage = -1;
		// 得到抓图的数量
		CaptureSize = 4;
		CString str;
		str.Format(_T("Capture size = %d"), CaptureSize);
		m_pInterface->AddLog(str);	

		RawArray = new short *[CaptureSize];
		memset(RawArray, 0, CaptureSize* sizeof(USHORT*));
		lensPos = new int[CaptureSize];

		if (!CheckOTPIsBurning())   return 0;
// 		if (pDlg->m_berase)
// 		{
// 			if (!ZC533_All_Erase())  return 0;
// 			m_pInterface->AddLog(_T("OTP所有数据已全部擦除!"));
// 		}

		//gmvs.MinPixelAvg  :for 10-bit data, Pixel average value shouldn't smaller than this
		//gmvs.MaxPixelAvg  : ... shouldn't bigger than this
		//gmvs.MinPDAvg     : PD Signal average shouldn't smaller than this
		//gmvs.MaxPDAvg     : PD Signal average shouldn't bigger than this
		//gmvs.MinGain      : PD Gainmap shouldn't smaller than this
		//gmvs.MaxGain      : PD Gainmap value shouldn't bigger than this
		//gmvs.MaxPixelThresh   : max signal of normal pixel should be higher than this
		/* indicator of bit-width error: 
			if max < 256, then could be too dark for a flat image of 10-bit.
		*/
		gmvs.MinPixelAvg = MinPixelAvg;
		gmvs.MaxPixelAvg = MaxPixelAvg;//disabled
		gmvs.MinPDAvg = MinPDAvg;
		gmvs.MaxPDAvg = MaxPDAvg;//disabled
		gmvs.MinGain  = MinGain;//0.25x gain
		gmvs.MaxGain = MaxGain;// 4x gain
		gmvs.MaxPixelThresh = MaxPixelThresh;// 1/4  of max range[1023]
		gmvs.GainDiff = GainDiff;

		PDAFPatternValue.GlobalShift_x = 12; // PDAF Start position x
		PDAFPatternValue.GlobalShift_y = 12; // PDAF Start position y
		PDAFPatternValue.x_step = 64; //PDAF Block Width  
		PDAFPatternValue.y_step = 64; //PDAF Block Height
		PDAFPatternValue.ImageBits = 10; // Image Bit Width
		PDAFPatternValue.BlackLevel = 64; // Image Black Level
		PDAFPatternValue.SensorGain = 1;
		PDAFPatternValue.DSRatio = 2;
		PDAFPatternValue.PDAFonG = FALSE;

		CString PatternFilename;
		CString path = GetModulePath();
		PatternFilename = FileFolderPath + m_Ini_Name;//_T("\\SS_3P3_PDAFPatttern.txt");
		strcpy_s(PDAFPatternValue.PDAFPatternFilename,CT2A(PatternFilename.GetBuffer(PatternFilename.GetLength())));//File to store PDAF Pattern
		/*******************************************
		Second Part(1): Phase Difference Conversion Coefficient Calibration 
		********************************************/
		BYTE AFCheckCode[4] = {0};
		ZC533_Read(0x0741, AFCheckCode, 4);
		InfinityCode = (AFCheckCode[0]<<8)|AFCheckCode[1];
		MacroCode = (AFCheckCode[2]<<8)|AFCheckCode[3];

		info.Format(_T("InfCode = %d, MacCode = %d."),InfinityCode,MacroCode);
		m_pInterface->AddLog(info);

		BYTE GainMapData[1078] = {0};
		ZC533_Read(0x781, GainMapData, 1078);
		GainMapData2D.VersionNum = (GainMapData[0]<<8) | GainMapData[1];
		GainMapData2D.OffsetX   =(GainMapData[2]<<8) | GainMapData[3];
		GainMapData2D.OffsetY   =(GainMapData[4]<<8) | GainMapData[5];
		GainMapData2D.RatioX    =(GainMapData[6]<<8) | GainMapData[7];
		GainMapData2D.RatioY    =(GainMapData[8]<<8) | GainMapData[9];
		GainMapData2D.MapWidth  =(GainMapData[10]<<8) | GainMapData[11];
		GainMapData2D.MapHeight =(GainMapData[12]<<8) | GainMapData[13];

		for (i=0; i<GainMapData2D.MapWidth * GainMapData2D.MapHeight; i++)
		{
			GainMapData2D.Left_GainMap[i] = (GainMapData[13+i*2]<<8) | GainMapData[14+i*2]; 
			GainMapData2D.Right_GainMap[i] = (GainMapData[13+532+i*2]<<8) | GainMapData[14+532+i*2]; 
		}

		uint16_t GainMapFlag = -1;
		GainMapFlag = GM_Constraint(gmvs, &GainMapData2D, NULL);
		if (Display_GmCheckResult(GainMapFlag) == -1) return 1;

		OTP_single_defect_2DGainmap(GainMapData2D,gmvs.GainDiff);

		//////////////////////////////////////////////////////////////////////////
		vericode = (MacroCode+InfinityCode)/2;

		lensPos[0]=(USHORT)(InfinityCode+0.1*(MacroCode-InfinityCode));
		lensPos[1]=(USHORT)(MacroCode-0.1*(MacroCode-InfinityCode));

		lensPos[2]=(USHORT)(InfinityCode+0.2*(MacroCode-InfinityCode));
		lensPos[3]=(USHORT)(MacroCode-0.2*(MacroCode-InfinityCode));

		Imagefilename[0] = _T("50cm_80_Inf");
		Imagefilename[1] = _T("50cm_80_Mac");
		Imagefilename[2] = _T("50cm_60_Inf");
		Imagefilename[3] = _T("50cm_60_Mac");

		m_pInterface->GetDriverIC()->WriteAF(lensPos[0]);

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

BOOL PD_CALI_Qual_3_Inspection::CheckOTPIsBurning()
{
	BYTE flag;
	ZC533_Read(0x0000,&flag,1);

	if (flag == 0)
	{
		m_pInterface->AddLog(_T("OTP内容为空，请先烧录画质"),COLOR_RED);
		return FALSE;
	}
	else if (flag != 1)
	{
		m_pInterface->AddLog(_T("OTP Flag错误"),COLOR_RED);
		return FALSE;
	}

	return TRUE;
}
/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int PD_CALI_Qual_3_Inspection::Testing()
{
   //TODO:在此添加测试项代码
	if (!bInit)
	{
		SetResult(RESULT_INSPECTION_NG);
		return RET_END;
	}

	int nRet = PD_CALI_Proc3();
	if(nRet == 0) 
	{
		SetResult(RESULT_INSPECTION_OK);
		m_pInterface->AddLog(_T("Qual PD CALI STEP3 PASS"), COLOR_GREEN, 330);
	}
	else if (nRet > 0)	// fail
	{
		SetResult(RESULT_INSPECTION_NG);
		m_pInterface->AddLog(_T("Qual PD CALI STEP3 FAIL"), COLOR_RED, 330);
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
int PD_CALI_Qual_3_Inspection::Finalize()
{
	//TODO:在此添加回收结束代码
	Lock_OneTime = FALSE;

	for (int i=0; i<CaptureSize; i++)
	{
		SAFE_DELETE_ARRAY(RawArray[i]);
	}
	SAFE_DELETE_ARRAY(RawArray);
	SAFE_DELETE_ARRAY(lensPos);

	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int PD_CALI_Qual_3_Inspection::LoadOption()
{
	//TODO: 在此添加读取代码 
	pDlg->m_ExposureEn      = ReadConfigInt(_T("ExposureEn"), 1);
	pDlg->m_Exp_Target      = ReadConfigInt(_T("Exp_Target"), 200);
	pDlg->m_Exp_Tolerance   = ReadConfigInt(_T("Exp_Tolerance"),4);
	pDlg->m_bSaveRaw		= ReadConfigInt(_T("EnSaveRaw"),	1);
	pDlg->m_berase = ReadConfigInt(_T("EnErase"),0);
	m_CodeSPC		= ReadConfigInt(_T("CodeDiff"),	40);

	pDlg->m_QVLEn      = ReadConfigInt(_T("QvlEn"), 1);

	pDlg->m_InfH    = ReadConfigInt(_T("InfH"), 1023);
	pDlg->m_InfL    = ReadConfigInt(_T("InfL"), 0);
	pDlg->m_MacH    = ReadConfigInt(_T("MacH"), 1023);
	pDlg->m_MacL    = ReadConfigInt(_T("MacL"), 0);
	pDlg->m_MidH    = ReadConfigInt(_T("MidH"), 1023);
	pDlg->m_MidL    = ReadConfigInt(_T("MidL"), 0);
	pDlg->m_AFDif    = ReadConfigInt(_T("AFDif"), 300);

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
int PD_CALI_Qual_3_Inspection::SaveOption()
{
	if(!Inspection::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	WriteConfigInt(_T("ExposureEn"),	pDlg->m_ExposureEn);
	WriteConfigInt(_T("Exp_Target"),	pDlg->m_Exp_Target);
	WriteConfigInt(_T("Exp_Tolerance"), pDlg->m_Exp_Tolerance);
	WriteConfigInt(_T("EnSaveRaw"),		pDlg->m_bSaveRaw);
	WriteConfigInt(_T("EnErase"),		pDlg->m_berase);
	WriteConfigInt(_T("CodeDiff"), m_CodeSPC);

	WriteConfigInt(_T("QvlEn"),	pDlg->m_QVLEn);

	WriteConfigInt(_T("InfH      "),pDlg->m_InfH);
	WriteConfigInt(_T("InfL      "),pDlg->m_InfL);
	WriteConfigInt(_T("MacH      "),pDlg->m_MacH);
	WriteConfigInt(_T("MacL      "),pDlg->m_MacL);
	WriteConfigInt(_T("MidH      "),pDlg->m_MidH);
	WriteConfigInt(_T("MidL      "),pDlg->m_MidL);
	WriteConfigInt(_T("AFDif      "),pDlg->m_AFDif);


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
LPCTSTR PD_CALI_Qual_3_Inspection::GetReportHeaders()
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
LPCTSTR PD_CALI_Qual_3_Inspection::GetReportLowerLimits()
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
LPCTSTR PD_CALI_Qual_3_Inspection::GetReportUpperLimits()
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
LPCTSTR PD_CALI_Qual_3_Inspection::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}



int PD_CALI_Qual_3_Inspection::PD_CALI_Proc3(void)
{
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
//		short *GmCalibImage;

//	char GainMapFilename[MAX_PATH];
//	char GainMapBaseFilename[MAX_PATH];
//	int ImageExistFlag;

	if (CaptureFrame<CaptureSize)
	{
		int i = CaptureFrame;
		//复制BUFF
		RawArray[i] = new short [width * height];

		//保存Raw图
		CString Path;
		CString str;
		if (!pDlg->m_bSaveRaw)
		{
			Path.Format(_T("%s.raw"), Imagefilename[i]);
			Path = FileFolderPath + Path;
		}
		else
		{
			str.Format(_T("%d\\"), m_pInterface->GetModuleNum());
			str = FileFolderPath + str;
			CreateDirectory(str,NULL);
			Path.Format(_T("%s.raw"), Imagefilename[i]);
			Path = str + Path;
		}

        char CurImgFilename[MAX_PATH];
		strcpy_s(CurImgFilename,CT2A(Path.GetBuffer(Path.GetLength())));
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

		FILE* fp_r;
		if (_wfopen_s(&fp_r,  Path,_T("rb")))   //CT2A(sfilename)
		{
			info.Format(_T("Cannot Find Slope Calibration First Image DATA %s"),CA2T(CurImgFilename));
			m_pInterface->AddLog(info,COLOR_RED);
			return 1;
		}
		if (fp_r)
		{
			fread((void*)RawArray[i],2,width*height,fp_r);//读取Raw图
			fclose(fp_r);
		}

		if ((i+1)<CaptureSize)
		{	
			m_pInterface->GetDriverIC()->WriteAF(lensPos[i+1]);
			Sleep(m_SleepTime*100);
		}

		CaptureFrame++;
		return -1;//continue
	}

	if (!verifylock)
	{
		m_pInterface->GetDriverIC()->WriteAF(vericode);		
		Sleep(m_SleepTime*100);
		verifylock = TRUE;
		return -1;
	}
	/*******************************************
	Second Part(2): Phase Difference Conversion Coefficient Calibration 
	********************************************/
	GainMap.Flag_1D_2D = GainMapData2D.VersionNum;
	GainMap.GainMapData = &GainMapData2D;

	CalibrationKOutputParams CalibrationKOutputValues2;
	int PD_ConversionCoeff_Flag = PDAF_Cal_PDConversionCoef(RawArray[0],RawArray[1],width, height,(float)lensPos[0], (float)lensPos[1],GainMap, PDAFPatternValue,FALSE,CalibrationKOutputValues2);

	if (PD_ConversionCoeff_Flag == -1)
	{
		m_pInterface->AddLog(_T("Expected Output Width/Height in GainMap are Too Small."),COLOR_RED);
		return 1;
	}
	if (PD_ConversionCoeff_Flag == -2)
	{
		m_pInterface->AddLog(_T("Cannot find the Pattern Definition File."),COLOR_RED);
		return 1;
	}
	if (PD_ConversionCoeff_Flag == -3)
	{
		m_pInterface->AddLog(_T("Either Image is a Color Image."),COLOR_RED);
		return 1;
	}
	info.Format(_T("Phase Difference Conversion Coefficient k3= %d"),CalibrationKOutputValues2.PDConversionCoef[0]);
	m_pInterface->AddLog(info);
	info.Format(_T("FIRST IMAGE --> Phase Shift = %f Confidence Level = %d"),CalibrationKOutputValues2.FirstImage_PDShiftValue[0],CalibrationKOutputValues2.FirstImage_CLValue[0]);
	m_pInterface->AddLog(info);
	info.Format(_T("SECOND IMAGE --> Phase Shift = %f Confidence Level = %d"),CalibrationKOutputValues2.SecondImage_PDShiftValue[0],CalibrationKOutputValues2.SecondImage_CLValue[0]);
	m_pInterface->AddLog(info);
	//////////////////////////////////////////////////////////////////////////
	CalibrationKOutputParams CalibrationKOutputValues3;
	PD_ConversionCoeff_Flag = PDAF_Cal_PDConversionCoef(RawArray[2],RawArray[3],width, height,(float)lensPos[2], (float)lensPos[3],GainMap, PDAFPatternValue,FALSE,CalibrationKOutputValues3);

	if (PD_ConversionCoeff_Flag == -1)
	{
		m_pInterface->AddLog(_T("Expected Output Width/Height in GainMap are Too Small."),COLOR_RED);
		return 1;
	}
	if (PD_ConversionCoeff_Flag == -2)
	{
		m_pInterface->AddLog(_T("Cannot find the Pattern Definition File."),COLOR_RED);
		return 1;
	}
	if (PD_ConversionCoeff_Flag == -3)
	{
		m_pInterface->AddLog(_T("Either Image is a Color Image."),COLOR_RED);
		return 1;
	}

	info.Format(_T("Phase Difference Conversion Coefficient k4= %d"),CalibrationKOutputValues3.PDConversionCoef[0]);
	m_pInterface->AddLog(info);
	info.Format(_T("THIRD IMAGE --> Phase Shift = %f Confidence Level = %d"),CalibrationKOutputValues3.FirstImage_PDShiftValue[0],CalibrationKOutputValues3.FirstImage_CLValue[0]);
	m_pInterface->AddLog(info);
	info.Format(_T("FOURTH IMAGE --> Phase Shift = %f Confidence Level = %d"),CalibrationKOutputValues3.SecondImage_PDShiftValue[0],CalibrationKOutputValues3.SecondImage_CLValue[0]);
	m_pInterface->AddLog(info);
	/*******************************************
	Third Part(2): Verification Procedure 
	********************************************/
	m_pInterface->AddLog(_T("Start Verification..."));
	VerificationInputParams VerificationInputValue;
	VerificationOutputParams VerificationResult;

	BYTE PDConversionCoeff[2]={0};
	ZC533_Read(0x0BC1,PDConversionCoeff,2);
	int PD_ConversionCoeff_average=PDConversionCoeff[1]+(PDConversionCoeff[0]<<8);
	info.Format(_T("Difference Conversion Coefficient K12_aver= %d."),PD_ConversionCoeff_average);    
	m_pInterface->AddLog(info);
	PD_ConversionCoeff_average=(int)((PD_ConversionCoeff_average+(CalibrationKOutputValues2.PDConversionCoef[0]+CalibrationKOutputValues3.PDConversionCoef[0])/2)/2+0.5);
	info.Format(_T("Difference Conversion Coefficient K_aver= %d."),PD_ConversionCoeff_average);    
	m_pInterface->AddLog(info);

	PDConversionCoeff[0]=PD_ConversionCoeff_average>>8;
	PDConversionCoeff[1]=PD_ConversionCoeff_average&0xff;

	//保存Raw图
	CString Path;
	CString str;
	if (!pDlg->m_bSaveRaw)
	{
		Path.Format(_T("image_50cm_%d.raw"),vericode);
		Path = FileFolderPath + Path;
	}
	else
	{
		str.Format(_T("%d\\"), m_pInterface->GetModuleNum());
		str = FileFolderPath + str;
		CreateDirectory(str,NULL);
		Path.Format(_T("image_50cm_%d.raw"),vericode);
		Path = str + Path;
	}

	if (PathFileExists(Path))
	{
		if (!DeleteFile(Path.GetBuffer(Path.GetLength())))
		{
			m_pInterface->AddLog(_T("Verify Raw Delete Fail!"),COLOR_RED);
			return 1;
		}
	}
	SaveRawImage(raw_buf,width,height,Path);
	char VerificationFilename[MAX_PATH];
	strcpy_s(VerificationFilename,CT2A(Path.GetBuffer(Path.GetLength())));
	short *Distance50cmLensPosition9_Img = (short *)calloc(width*height,sizeof(short));
	FILE* fp_r;
	if (_wfopen_s(&fp_r,  Path,_T("rb")))   //CT2A(sfilename)
	{
		info.Format(_T("Cannot Find Verification Image DATA %s."),VerificationFilename);
		m_pInterface->AddLog(info,COLOR_RED);
		return 1;
	}
	if (fp_r)
	{
		fread((void*)Distance50cmLensPosition9_Img,2,width*height,fp_r);//读取Raw图
		fclose(fp_r);
	}
///////////////////////////////////////////////////////////////
#ifdef TestVersion
	Inf_code = 180;
#else
	if (GetAFcode())  return 1;//获取50cm 最清晰值。
#endif
//////////////////////////////////////////////////////////////
	VerificationInputValue.PD_ConversionCoeff = PD_ConversionCoeff_average;
	//Notes: these two parameters' values,'Distance50cmTrueFocusPosition' and 'ToleranceError', depend on module 
	//the correct true focus value need to be used.
	VerificationInputValue.Verification_Lens_Position = (float)vericode;
	VerificationInputValue.Distance50cmTrueFocusPosition = (float)Inf_code;
	VerificationInputValue.ToleranceError = (float)m_CodeSPC;

	bool VerificationFlag = PDAF_Cal_Verification(Distance50cmLensPosition9_Img,width,height, GainMap, PDAFPatternValue,&VerificationInputValue, VerificationResult);
	free(Distance50cmLensPosition9_Img);

	info.Format(_T("Original DAC = %f, \n Phase Shift = %f(Confidence Value=  %d), \n Coeff = %d \n Estimation DAC = %f"),
		VerificationInputValue.Verification_Lens_Position,VerificationResult.PDShiftValue,VerificationResult.ConfidenceValue,
		VerificationInputValue.PD_ConversionCoeff, VerificationResult.EstimationDAC);
	m_pInterface->AddLog(info);
	info.Format(_T("True DAC = %f and Error = %f"),VerificationInputValue.Distance50cmTrueFocusPosition,VerificationResult.Err);
	m_pInterface->AddLog(info);
	if (VerificationResult.Err > m_CodeSPC)
	{
		info.Format(_T("Code out of Range!\n Code Error %f > Limit %d"),VerificationResult.Err,m_CodeSPC);
		m_pInterface->AddLog(info,COLOR_RED);
		return 1;
	}
	/*******************************************
	End of Calibration:  Verification 
	********************************************/
	BYTE PDAF_OTPData[4] = {0};
	PDAF_OTPData[0] = 0x01;
	PDAF_OTPData[1] = PDConversionCoeff[0];
	PDAF_OTPData[2] = PDConversionCoeff[1];
	PDAF_OTPData[3] = (PDConversionCoeff[0]+PDConversionCoeff[1])%255 + 1;

	if (!ZC533_Programming(0x0BC0,PDAF_OTPData,4))  return 1;
	m_pInterface->AddLog(_T("Qual PDAF Burning success."),COLOR_BLUE);

	if (!CheckOTPIsBurning())   return 1;

	return 0;	// pass
}

int PD_CALI_Qual_3_Inspection::Display_GmCheckResult(int GainMapFlag)
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

BOOL PD_CALI_Qual_3_Inspection :: SaveDAC(int *lenspos)
{
	CString Path;
	CString str;
	CString FileFolderPath1;
	str.Format(_T("%d\\"),m_pInterface->GetModuleNum());
	Path.Format(_T("DAC.txt"));
	FileFolderPath1 = FileFolderPath + str;
	CreateDirectory(FileFolderPath1,NULL);
	Path = FileFolderPath1 + Path;

	if (PathFileExists(Path))
	{
		if(!DeleteFile(Path.GetBuffer(Path.GetLength())))
		{
			m_pInterface->AddLog(_T("DAC.txt删除失败!"),COLOR_RED);
			return FALSE;
		}
	}

	FILE* fp;
	errno_t err;
	err = freopen_s(&fp,CT2A(Path),("a"),stdout);
	if (err != 0)   //_wfreopen_s
	{
		m_pInterface->AddLog(_T("打开DAC.txt失败！"),COLOR_RED);
		return FALSE;
	}
	printf("%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d", lenspos[0], lenspos[1], lenspos[2], lenspos[3],lenspos[4],lenspos[5],lenspos[6],lenspos[7],lenspos[8],lenspos[9]);

	fclose(fp);	

	str = Path + _T(" saved");
	m_pInterface->AddLog(str);
	return TRUE;
}

int PD_CALI_Qual_3_Inspection::GetAFcode(void)
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
		Inf_code = vpInfAF->at(0);

	if(Inf_code > pDlg->m_InfH || Inf_code <  pDlg->m_InfL)
	{
		info.Format(_T("远景Code(%d)超标,请重测远景"), Inf_code);
		m_pInterface->AddLog(info, COLOR_RED);
		return -1;
	}

// 	if (vpMacAF == NULL || vpMacAF->size() == 0)
// 	{
// 		Mac_code = 0;
// 		info = _T("近景Code值为空");
// 		m_pInterface->AddLog(info, COLOR_RED);
//     	return -1; //cnw
// 	}
// 	else
// 		Mac_code = vpMacAF->at(0);
// 
// 	if (Mac_code > pDlg->m_MacH || Mac_code <  pDlg->m_MacL)
// 	{
// 		info.Format(_T("近景Code(%d)超标,请重测近景"), Mac_code);
// 		m_pInterface->AddLog(info, COLOR_RED);
// 		return -1;
// 	}
// 
// 	if (vpMidAF == NULL || vpMidAF->size() == 0)
// 	{
// 		Mid_code = 0;
// 		info = _T("中景Code值为空");
// 		m_pInterface->AddLog(info, COLOR_RED);
//     	return -1; //cnw
// 	}
// 	else
// 		Mid_code = vpMidAF->at(0);
// 
// 	if (Mid_code > pDlg->m_MidH || Mid_code <  pDlg->m_MidL)
// 	{
// 		info.Format(_T("中景Code(%d)超标,请重测中景"), Mid_code);
// 		m_pInterface->AddLog(info, COLOR_RED);
// 		return -1;
// 	}
// 
// 	if (Inf_code >= Mac_code-pDlg->m_AFDif || Inf_code>Mac_code)
// 	{
// 		info.Format(_T("远景Code(%d)近景Code(%d)太接近，请重新对焦"), Inf_code, Mac_code);
// 		m_pInterface->AddLog(info, COLOR_RED);
//     	return -1; //cnw
// 	}

	info.Format(_T("远景Code: %d"), Inf_code);//\n中景Code: %d\n近景Code: %d, Mid_code,Mac_code
	m_pInterface->AddLog(info, COLOR_BLUE);

	/////////////////////////temp///////////////////////////////////////////
// 	Inf_code = 180;//cnw
// 	Mid_code = 235;//cnw
// 	Mac_code = 600;//cnw
	//////////////////////////////////////////////////////////////////////////

	m_pInterface ->AddLog(_T("Get AF Data End"));
	return 0;	
}

// BOOL PD_CALI_Qual_3_Inspection::LoadMtkIni(void)
// {
// 	CString Path = m_pInterface->GetExeDir();
// 
// 	Path += m_Ini_Name;
// 
// 	char filePath[256];
// 	UnicodeToAnsi(Path.GetBuffer(), filePath, 256);
// 
// 	int nRet = pd_cali_init(filePath);
// 
// 	if (nRet != PD_ERR_OK)
// 	{
// 		switch(nRet)
// 		{
// 		case PD_ERR_FILE_NOT_FOUND:
// 			m_pInterface->AddLog(_T("Qual PD INI FILE_NOT_FOUND"), COLOR_RED);		
// 			break;
// 		case PD_ERR_INIT_FAIL:
// 			m_pInterface->AddLog(_T("Qual PD INI WRONG CONFIG"), COLOR_RED);		
// 			break;
// 		case PD_ERR_INVALID_PARAM:
// 			m_pInterface->AddLog(_T("Qual PD INI PARAMETER IS INVALID"), COLOR_RED);	
// 			break;
// 		}
// 		return FALSE;
// 	}
// 	else
// 	{
// 		return TRUE;
// 	}
// 
// }


void PD_CALI_Qual_3_Inspection::PDAFSaveProc3Bin(char* data, int size)
{
	CString str;
	str.Format(_T("output bytes size %d"), size);
	m_pInterface->AddLog(str);

	// 存储bin文件
	CString info;
	CString Path;
// 	if (!pDlg->m_QVLEn)
// 	{
		Path.Format(_T("PD_CALI_Qual_STEP_2_%d.bin"),m_pInterface->GetModuleNum());
		Path = FileFolderPath+Path;
// 	}
// 	else
// 	{
// 		CString FileFolderPath1;
// 		str.Format(_T("bin\\"));//
// 		FileFolderPath1 = FileFolderPath + str;
// 		CreateDirectory(FileFolderPath1,NULL);
// 		Path.Format(_T("%d.bin"),m_pInterface->GetModuleNum());
// 		Path = FileFolderPath1 + Path;// str + _T("\\")+
// 	}

	if (PathFileExists(Path))
	{
		if(!DeleteFile(Path.GetBuffer(Path.GetLength())))
		{
			m_pInterface->AddLog(_T("原bin档删除失败!"),COLOR_RED);
			return ;
		}
	}

	FILE* file1;
	if (_wfopen_s(&file1, Path.GetBuffer() , _T("wb+")) == 0)
	{
		fwrite(data, 1, size, file1);
		fclose(file1);
		info.Format(_T("%s saved"),Path);
		m_pInterface->AddLog(info);
	}


	// 保存txt文本
	Path.Format(_T("PD_CALI_Qual_STEP_2_%d.txt"),m_pInterface->GetModuleNum());
	Path = FileFolderPath+Path;
	if (PathFileExists(Path))
	{
		if(!DeleteFile(Path.GetBuffer(Path.GetLength())))
		{
			m_pInterface->AddLog(_T("原txt档删除失败!"),COLOR_RED);
			return ;
		}
	}

	CStringA temp, num;
	for (int i=0; i<size; i++)
	{
		num.Format("%02hhX\n", (unsigned char)data[i]);
		temp += num;
	}

	char* p = temp.GetBuffer();
	FILE* file2;
	if (_wfopen_s(&file2, Path.GetBuffer() , _T("wt+")) == 0)
	{
		fwrite(p, 1, strlen(p), file2);

		fclose(file2);
		info.Format(_T("%s saved"),Path);
		m_pInterface->AddLog(info);
	}

}


void PD_CALI_Qual_3_Inspection::SaveRawImage(USHORT* raw_buf, int width, int height, CString& path)
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

int PD_CALI_Qual_3_Inspection::EEPROM_S5K3M2(UCHAR* data, int size)
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
		m_pInterface->I2C_Write(USHORT(m_Dev_Addr),USHORT(m_Reg_Addr+i),data[i],m_I2cMode);
		Sleep(2);
	}
	int quotient = (size-(32-remain))/m_PageSize;	// 商
	int remainder = (size-(32-remain))%m_PageSize;// 余数
	int j=i;
	int reg = m_Reg_Addr+i;
	int dev = m_Dev_Addr;

	while(quotient)
	{
		m_pInterface->I2C_MultiBytesWrite(USHORT(dev), USHORT(reg), 2, &data[j], m_PageSize);
		Sleep(10*m_SleepTime);
		quotient -- ;
		j += m_PageSize;
		reg += m_PageSize;
	}


	if (remainder != 0)
	{
		m_pInterface->I2C_MultiBytesWrite(USHORT(dev), USHORT(reg), 2, &data[j], remainder);	
		Sleep(10*m_SleepTime);
	}

	// 读出数据验证写入正确性

	UCHAR* Rdata = new UCHAR[size];
	USHORT tmp = 0;
	for (i=0;i<32-remain;i++)
	{
		m_pInterface->I2C_Read(USHORT(m_Dev_Addr),USHORT(m_Reg_Addr+i),&tmp,m_I2cMode);
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
		BOOL bRet = m_pInterface->I2C_MultiBytesRead(USHORT(dev), USHORT(reg), 2, &Rdata[j], m_PageSize, TRUE);
		Sleep(10*m_SleepTime);
		quotient -- ;
		j += m_PageSize;
		reg += m_PageSize;
	}

	if (remainder != 0)
	{
		m_pInterface->I2C_MultiBytesRead(USHORT(dev), USHORT(reg), 2, &Rdata[j], remainder, TRUE);	
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

	delete [] Rdata;
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

int PD_CALI_Qual_3_Inspection::EEPROM_S5K3L8(UCHAR* data, int size)
{
	m_pInterface->AddLog(_T("OTP Proc3 Data Burning Start!"));
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
		m_pInterface->AddLog(_T("PDAF Proc3 Data Burning Fail！"), COLOR_RED);
		result |= -1;
	}
	if (!GT24C64_Programming(0x0DBE,ps,1))
	{
		m_pInterface->AddLog(_T("PDAF Proc3 Checksum Burning Fail！"), COLOR_RED);
		result |=  -1;
	}

	BYTE* Read_Data = new BYTE[size];
	if (!GT24C64_Read(m_Reg_Addr,Read_Data,size))
	{
		m_pInterface->AddLog(_T("PDAF Proc3 Data Read Fail！"), COLOR_RED);
		result |=  -1;
	}
	else
	{
		if (!CheckEqual(data,Read_Data,size))
		{
			m_pInterface->AddLog(_T("PDAF Proc3 Data WriteRead not equal！"), COLOR_RED);
			result |= -1;
		}
	}

	BYTE* Read_Sum = new BYTE[1];
	if (!GT24C64_Read(0x0DBE,Read_Sum,1))
	{
		m_pInterface->AddLog(_T("PDAF Proc3 Sum Read Fail！"), COLOR_RED);
		result |= -1;
	}
	else
	{
		if (!CheckEqual(ps,Read_Sum,1))
		{
			m_pInterface->AddLog(_T("PDAF Proc3 Sum WriteRead not equal！"), COLOR_RED);
			result |= -1;
		}
	}
	result |= 0;
	
	m_pInterface->AddLog(_T("OTP Proc3 Data Burning Success!"));
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

// void PD_CALI_Qual_3_Inspection::GetErrorCString(int code)
// {
// 	CString info;
// 	BOOL bRet = FALSE;
// 	switch(code)
// 	{
// 	case PD_ERR_OK:
// 		info = _T("PD_ERR_OK");
// 		bRet = TRUE;
// 		break;
// 	case PD_ERR_FILE_NOT_FOUND:
// 		info = _T("PD_ERR_FILE_NOT_FOUND");
// 		break;
// 	case PD_ERR_INIT_FAIL:
// 		info = _T("PD_ERR_INIT_FAIL");
// 		break;
// 	case PD_ERR_INVALID_PARAM:
// 		info = _T("PD_ERR_INVALID_PARAM");
// 		break;
// 	case PD_ERR_OUTPUT_MEM_NOT_SUFFICIENT:
// 		info = _T("PD_ERR_OUTPUT_MEM_NOT_SUFFICIENT");
// 		break;
// 	case PD_ERR_NOT_INITIALIZED:
// 		info = _T("PD_ERR_NOT_INITIALIZED");
// 		break;
// 	case PD_ERR_INPUT_DATA_VERIFIED_FAIL:
// 		info = _T("PD_ERR_INPUT_DATA_VERIFIED_FAIL");
// 		break;
// 	case PD_ERR_OUTPUT_DATA_VERIFIED_FAIL:
// 		info = _T("PD_ERR_OUTPUT_DATA_VERIFIED_FAIL");
// 		break;
// 	case PD_ERR_QUALITY_VERIFIED_FAIL:
// 		info = _T("PD_ERR_QUALITY_VERIFIED_FAIL");
// 		break;
// 	case PD_ERR_GENERAL_ERROR:
// 		info = _T("PD_ERR_GENERAL_ERROR");
// 		break;
// 	default:
// 		info = _T("Undefined!");
// 		break;
// 	}
// 
// 	if (bRet)
// 		m_pInterface->AddLog(info, COLOR_BLUE);
// 	else
// 		m_pInterface->AddLog(info, COLOR_RED);
// }
// 
// int PD_CALI_Qual_3_Inspection::PD_CALI_Verify_Proc3(int size)
// {
// 	CString info;
// 	BYTE* Read_Proc3 = new BYTE[size];
// 	if (!GT24C64_Read(m_Reg_Addr,Read_Proc3,size))
// 	{
// 		m_pInterface->AddLog(_T("Read PDAF Proc3 Data Fail!"),COLOR_RED);
// 		SAFE_DELETE_ARRAY(Read_Proc3);
// 		return 1;
// 	}
// 
// 	if (!LoadMtkIni())
// 	{
// 		return 1;
// 	}
// 	int ret = pd_cali_verify_proc2(Read_Proc3);
// 	if (ret == PD_ERR_OK)
// 	{
// 		m_pInterface->AddLog(_T("Eeprom verified PASS"), COLOR_BLUE);
// 	}
// 	else
// 	{
// 		SAFE_DELETE_ARRAY(Read_Proc3);
// 		GetErrorCString(ret);
// 		return 1;
// 	}
// 
// 	int step2code = 0;
// 	pd_cali_get_DAC_proc2(Read_Proc3,step2code);
// 	info.Format(_T("get proc2 dac = %d"),step2code);
// 	m_pInterface->AddLog(info);
// 
// 	int diff = abs(step2code - (int)Mid_code);
// 	if (diff>m_CodeSPC)
// 	{
// 		info.Format(_T("Actual Diff： %d 大于 Limit Diff： %d"),diff,m_CodeSPC);
// 		m_pInterface->AddLog(info,COLOR_RED);
// 		SAFE_DELETE_ARRAY(Read_Proc3);
// 		return 1;
// 	}
// 
// 	SAFE_DELETE_ARRAY(Read_Proc3);
// 	return 0;
// 
// }
void PD_CALI_Qual_3_Inspection::Raw10toRaw8(USHORT *Src, BYTE *Dest, long number)//Raw10
{
	long DesCount = 0;

	for (long i = 0; i < number; i++ )//i = i + 5
	{
		Dest[i] = ((Src[i]>>2) & 0xff);
	}
}

CString PD_CALI_Qual_3_Inspection::GetModulePath()
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

void PD_CALI_Qual_3_Inspection::SaveCurrentRaw10(CString filename, USHORT *buff, int width, int height)
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

void PD_CALI_Qual_3_Inspection::SaveCurrentRaw8(CString filename, BYTE *buff, int width, int height)
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

BOOL PD_CALI_Qual_3_Inspection::CheckEqual(BYTE *data1, BYTE *data2, int num)
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

BOOL PD_CALI_Qual_3_Inspection::GT24C64_Read(USHORT address,unsigned char *Data,int size)
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
		m_pInterface->I2C_MultiBytesRead(0xA0,address,2,Data,Count);
	Sleep(m_SleepTime*6);
	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,Data+(Count+i*32),32);
		Sleep(m_SleepTime*2);
	}
	m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,Data+(Count+i*32),num);
	return TRUE;

}

BOOL PD_CALI_Qual_3_Inspection::GT24C64_Programming(USHORT address,unsigned char *Data,int size)
{
	int i=0;
	int group=0;
	int num=0;
	BYTE *CheckData=new BYTE[size];
	Sleep(m_SleepTime*2);
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
	Sleep(m_SleepTime*2);

	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesWrite(0xA0,address+(Count+i*32),2,Data+(Count+i*32),32);
		Sleep(m_SleepTime*2);
	}
	m_pInterface->I2C_MultiBytesWrite(0xA0,address+(Count+i*32),2,Data+(Count+i*32),num);
	Sleep(m_SleepTime*2);
	if(Count>0)
		m_pInterface->I2C_MultiBytesRead(0xA0,address,2,CheckData,Count);

	Sleep(m_SleepTime*2);
	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,CheckData+(Count+i*32),32);
		Sleep(m_SleepTime*2);
	}
	m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,CheckData+(Count+i*32),num);
	for (i=0;i<size;i++)
	{
		if (CheckData[i]!=Data[i])
		{
			delete[] CheckData;
			return FALSE;
		}
	}
	delete[] CheckData;
	return TRUE;
}


BOOL PD_CALI_Qual_3_Inspection::ZC533_Page_Erase(BYTE page)
{
	//m_pInterface->AddDebugLog(L"TKU8341M WriteEepromPage_Byte");
	ZC533_Erase_lock = ZC533_LastPage - page;
	ZC533_LastPage = page;
	BOOL bRet = TRUE;
	if (1)//不允许连续两次erase 同一page  ZC533_Erase_lock != 0
	{
		bRet &= m_pInterface->I2C_Write(0xB0, ((ZC533_PAGE_ERASE<<14)|(page<<6)), 0, I2CMODE_16_8);
		Sleep(6*m_SleepTime);
	}

	return bRet;
}

BOOL PD_CALI_Qual_3_Inspection::ZC533_All_Erase()
{
	BOOL bRet = TRUE;
	bRet &= m_pInterface->I2C_Write(0xB0, (ZC533_ALL_ERASE<<14), 0, I2CMODE_16_8);
	Sleep(6*m_SleepTime);
	return bRet;
}

BOOL PD_CALI_Qual_3_Inspection::ZC533_Read(USHORT address,unsigned char *Data,int size)
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
		Sleep(m_SleepTime*10);

		for (i=0;i<retgroup;i++)
		{
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),pagesize);//|(reg&0x3f)   +(Count+i*pagesize)
			Sleep(m_SleepTime*10);
		}
		m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),retnum);//|(reg&0x3f)   +(Count+i*pagesize)
	}
	else
	{
		m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page)<<6)|(reg&0x3f))+(Count+i*32),2,Data+(Count+i*32),num);
		Sleep(m_SleepTime*10);
	}

	return TRUE;
}

BOOL PD_CALI_Qual_3_Inspection::ZC533_Programming(USHORT address,unsigned char *Data,int size)//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page
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
			Sleep(m_SleepTime*10);

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
				Sleep(m_SleepTime*10);
				//				}
			}
			if (!ZC533_Page_Erase(page+i+1))	
			{
				bRet = FALSE;
				break;
			}
			m_pInterface->I2C_MultiBytesWrite(0xB0,((ZC533_WRITE<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),retnum);//|(reg&0x3f))+(Count+i*pagesize)
			Sleep(m_SleepTime*10);
		}
		else
		{
			if (!ZC533_Page_Erase(page))	
			{
				bRet = FALSE;
				break;
			}
			m_pInterface->I2C_MultiBytesWrite(0xB0,((ZC533_WRITE<<14)|(page<<6)|(reg&0x3f))+(Count+i*pagesize),2,Data+(Count+i*pagesize),num);
			Sleep(m_SleepTime*10);
		}
		////////////////////////////////Read//////////////////////////////////////////
		if (ret>0)
		{
			if(Count>0)
				m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|(page<<6)|(reg&0x3f)),2,CheckData,retsize);
			Sleep(m_SleepTime*5);

			for (i=0;i<retgroup;i++)
			{
				m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,CheckData+(retsize+i*pagesize),pagesize);//|(reg&0x3f)   +(Count+i*pagesize)
				Sleep(m_SleepTime*5);
			}
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,CheckData+(retsize+i*pagesize),retnum);//|(reg&0x3f)   +(Count+i*pagesize)
			Sleep(m_SleepTime*5);
			for (i=0;i<size;i++)
			{
				if (CheckData[i]!=Data[i])
				{
					m_pInterface->AddLog(_T("读写对比失败!"),COLOR_RED);
					bRet = FALSE;
					break;
				}
			}
		}
		else
		{
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page)<<6)|(reg&0x3f))+(Count+i*32),2,CheckData+(Count+i*32),num);
			Sleep(m_SleepTime*5);
			for (i=0;i<size;i++)
			{
				if (CheckData[i]!=Data[i])
				{
					m_pInterface->AddLog(_T("读写对比失败!"),COLOR_RED);
					bRet = FALSE;
					break;
				}
			}
		}
	} while (0);

	SAFE_DELETE_ARRAY(CheckData);
	return bRet;
}
