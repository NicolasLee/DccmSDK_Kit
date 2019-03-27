#include "StdAfx.h"
#include "OtpCheck_Inspection.h"
#include "ImageProc.h"



#ifndef DCCM_EXP
    #define DCCM_EXP	__declspec(dllexport)
#endif	/* DCCM_EXP */


extern "C" DCCM_EXP Inspection * CreateInspectionItem(DccmControl_Interface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new OtpCheck_Inspection(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
OtpCheck_Inspection::OtpCheck_Inspection(DccmControl_Interface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	SetConfigPath(pInterface->GetConfigPath());	 // 设置Config路径	
	SetName(lpszName);	                             // 设置测试项目的名字
	pDlg = new OptionDlg(NULL, this);                 // 生成对话框对象
	LoadOption();                                    // 读取参数
	pDlg->Create(OptionDlg::IDD, NULL);               // 创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         // 得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(pDlg->CurTestType);

	//TODO: 其他类成员构造初始化
	m_pOtpCheck = NULL;

	//.....
}

OtpCheck_Inspection::~OtpCheck_Inspection(void)
{

}

/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int OtpCheck_Inspection::Initialize()
{
	Inspection::Initialize();
	//TODO:在此添加初始化代码

	return RET_END;
}


/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int OtpCheck_Inspection::Testing()
{
   //TODO:在此添加测试项代码
	m_pInterface->PauseGrabFrame();
	m_pOtpCheck->InitProject();
	int iRet = OtpCheck_Test();
	m_pOtpCheck->EndProject();
	m_pInterface->ResumeGrabFrame();

 	switch(iRet) 
	{
	case FUNCTION_FAIL:
		m_pInterface->AddLog(L"FAIL", COLOR_RED);
		SetResult(RESULT_INSPECTION_NG);
		break;
	case FUNCTION_PASS:
		m_pInterface->AddLog(L"PASS", COLOR_GREEN);
		SetResult(RESULT_INSPECTION_OK);
		break;
	default://RESULT_INSPECTION_NULL
		SetResult(RESULT_INSPECTION_NULL);
		break;
	}
   return RET_END;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int OtpCheck_Inspection::Finalize()
{
	//TODO:在此添加回收结束代码
	return RET_END;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int OtpCheck_Inspection::LoadOption()
{
	//TODO: 在此添加读取代码 
	pDlg->m_projName = (eProjectName)ReadConfigInt(L"ProjectName", -1);
	pDlg->m_OtpType	= ReadConfigInt(L"m_OtpType", 0);
	pDlg->CurTestType		= ReadConfigInt(_T("CurTestType"), TET_WHITE);
	pDlg->m_bCheckFlag_ModuleInfo = ReadConfigInt(L"m_bCheckFlag_ModuleInfo", 0);
	pDlg->m_bCheckFlag_AWB		= ReadConfigInt(L"m_bCheckFlag_AWB", 0);
	pDlg->m_bCheckFlag_LSC		= ReadConfigInt(L"m_bCheckFlag_LSC", 0);
	pDlg->m_bCheckFlag_AF		= ReadConfigInt(L"m_bCheckFlag_AF", 0);
	pDlg->m_bCheckFlag_PDAF		= ReadConfigInt(L"m_bCheckFlag_PDAF", 0);
	pDlg->m_bCheckFlag_PDAF		= ReadConfigInt(L"m_bCheckFlag_PDAF", 0);

	pDlg->m_bCheckSum_ModuleInfo = ReadConfigInt(L"m_bCheckSum_ModuleInfo", 0);
	pDlg->m_bCheckSum_AWB		= ReadConfigInt(L"m_bCheckSum_AWB", 0);
	pDlg->m_bCheckSum_LSC		= ReadConfigInt(L"m_bCheckSum_LSC", 0);
	pDlg->m_bCheckSum_AF		= ReadConfigInt(L"m_bCheckSum_AF", 0);
	pDlg->m_bCheckSum_PDAF		= ReadConfigInt(L"m_bCheckSum_PDAF", 0);

	pDlg->m_bCompareAF   = ReadConfigInt(L"m_bCompareAF", 0);
	pDlg->m_bCheckStc		= ReadConfigInt(L"m_bCheckStc", 0);
	pDlg->m_bCheckInf			= ReadConfigInt(L"m_bCheckInf", 0);
	pDlg->m_bCheckMac		= ReadConfigInt(L"m_bCheckMac", 0);
	pDlg->m_bCheckMid      = ReadConfigInt(L"m_bCheckMid", 0);

	pDlg->m_bAFCompareLimit = ReadConfigInt(L"m_bAFCompareLimit", 0);
	pDlg->m_stcLower			= ReadConfigInt(L"m_stcLower", 0);
	pDlg->m_stcUpper			= ReadConfigInt(L"m_stcUpper", 300);
	pDlg->m_infLower			= ReadConfigInt(L"m_infLower", 200);
	pDlg->m_infUpper			= ReadConfigInt(L"m_infUpper", 600);
	pDlg->m_macLower		= ReadConfigInt(L"m_macLower", 400);
	pDlg->m_macUpper		= ReadConfigInt(L"m_macUpper", 800);
	pDlg->m_midLower		= ReadConfigInt(L"m_midLower", 400);
	pDlg->m_midUpper		= ReadConfigInt(L"m_midUpper", 800);

	pDlg->m_bCheckRG	=	ReadConfigInt(L"m_bCheckRG", 0);
	pDlg->m_bCheckBG	=	ReadConfigInt(L"m_bCheckBG", 0);
	pDlg->m_bCheckGG	=	ReadConfigInt(L"m_bCheckGG", 0);
	pDlg->m_bCheckR		=	ReadConfigInt(L"m_bCheckR", 0);
	pDlg->m_bCheckGr		=	ReadConfigInt(L"m_bCheckGr", 0);
	pDlg->m_bCheckGb	=	ReadConfigInt(L"m_bCheckGb", 0);
	pDlg->m_bCheckB		=	ReadConfigInt(L"m_bCheckB", 0);

	pDlg->m_RGLower	=	ReadConfigInt(L"m_RGLower", 0);
	pDlg->m_RGUpper	=	ReadConfigInt(L"m_RGUpper", 1023);
	pDlg->m_BGLower	=	ReadConfigInt(L"m_BGLower", 0);
	pDlg->m_BGUpper	=	ReadConfigInt(L"m_BGUpper", 1023);
	pDlg->m_GGLower	=	ReadConfigInt(L"m_GGLower", 0);
	pDlg->m_GGUpper	=	ReadConfigInt(L"m_GGUpper", 1023);
	pDlg->m_RLower		=	ReadConfigInt(L"m_RLower", 0);
	pDlg->m_RUpper		=	ReadConfigInt(L"m_RUpper", 1023);
	pDlg->m_GrLower	=	ReadConfigInt(L"m_GrLower", 0);
	pDlg->m_GrUpper	=	ReadConfigInt(L"m_GrUpper", 1023);
	pDlg->m_GbLower	=	ReadConfigInt(L"m_GbLower", 0);
	pDlg->m_GbUpper	=	ReadConfigInt(L"m_GbUpper", 1023);
	pDlg->m_BLower		=	ReadConfigInt(L"m_BLower", 0);
	pDlg->m_BUpper		=	ReadConfigInt(L"m_BUpper", 1023);

	pDlg->m_bCheckGoldRG = ReadConfigInt(L"m_bCheckGoldRG", 0);
	pDlg->m_bCheckGoldBG = ReadConfigInt(L"m_bCheckGoldBG", 0);
	pDlg->m_bCheckGoldGG = ReadConfigInt(L"m_bCheckGoldGG", 0);
	pDlg->m_bCheckGoldR = ReadConfigInt(L"m_bCheckGoldR", 0);
	pDlg->m_bCheckGoldGr = ReadConfigInt(L"m_bCheckGoldGr", 0);
	pDlg->m_bCheckGoldGb = ReadConfigInt(L"m_bCheckGoldGb", 0);
	pDlg->m_bCheckGoldB = ReadConfigInt(L"m_bCheckGoldB", 0);

	pDlg->m_GoldRG = ReadConfigInt(L"m_GoldRG", 0);
	pDlg->m_GoldBG = ReadConfigInt(L"m_GoldBG", 0);
	pDlg->m_GoldGG = ReadConfigInt(L"m_GoldGG", 0);
	pDlg->m_GoldR = ReadConfigInt(L"m_GoldR", 0);
	pDlg->m_GoldGr = ReadConfigInt(L"m_GoldGr", 0);
	pDlg->m_GoldGb = ReadConfigInt(L"m_GoldGb", 0);
	pDlg->m_GoldB = ReadConfigInt(L"m_GoldB", 0);

	//........................
	return Inspection::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int OtpCheck_Inspection::SaveOption()
{
	if(!Inspection::SaveOption()) return FUNCTION_FAIL;
	//TODO: 在此添加存储代码	   


	WriteConfigInt(L"ProjectName", pDlg->m_projName);
	WriteConfigInt(L"m_OtpType", pDlg->m_OtpType);
	WriteConfigInt(_T("CurTestType"), pDlg->CurTestType);
	WriteConfigInt(L"m_bCheckFlag_ModuleInfo", pDlg->m_bCheckFlag_ModuleInfo);
	WriteConfigInt(L"m_bCheckFlag_AWB",		pDlg->m_bCheckFlag_AWB);
	WriteConfigInt(L"m_bCheckFlag_LSC",		pDlg->m_bCheckFlag_LSC);
	WriteConfigInt(L"m_bCheckFlag_AF",		pDlg->m_bCheckFlag_AF);
	WriteConfigInt(L"m_bCheckFlag_PDAF",	pDlg->m_bCheckFlag_PDAF);
	WriteConfigInt(L"m_bCheckFlag_PDAF",	pDlg->m_bCheckFlag_PDAF);

	WriteConfigInt(L"m_bCheckSum_ModuleInfo", pDlg->m_bCheckSum_ModuleInfo);
	WriteConfigInt(L"m_bCheckSum_AWB",		pDlg->m_bCheckSum_AWB);
	WriteConfigInt(L"m_bCheckSum_LSC",		pDlg->m_bCheckSum_LSC);
	WriteConfigInt(L"m_bCheckSum_AF",		pDlg->m_bCheckSum_AF);
	WriteConfigInt(L"m_bCheckSum_PDAF",		pDlg->m_bCheckSum_PDAF);

	WriteConfigInt(L"m_bCompareAF",		pDlg->m_bCompareAF);
	WriteConfigInt(L"m_bCheckStc",		pDlg->m_bCheckStc);
	WriteConfigInt(L"m_bCheckInf",		pDlg->m_bCheckInf);
	WriteConfigInt(L"m_bCheckMac",		pDlg->m_bCheckMac);
	WriteConfigInt(L"m_bCheckMid",		pDlg->m_bCheckMid);

	WriteConfigInt(L"m_bAFCompareLimit",		pDlg->m_bAFCompareLimit);
	WriteConfigInt(L"m_stcLower",		pDlg->m_stcLower);
	WriteConfigInt(L"m_stcUpper",		pDlg->m_stcUpper);
	WriteConfigInt(L"m_infLower",		pDlg->m_infLower);
	WriteConfigInt(L"m_infUpper",		pDlg->m_infUpper);
	WriteConfigInt(L"m_macLower",		pDlg->m_macLower);
	WriteConfigInt(L"m_macUpper",		pDlg->m_macUpper);
	WriteConfigInt(L"m_midLower",		pDlg->m_midLower);
	WriteConfigInt(L"m_midUpper",		pDlg->m_midUpper);

	WriteConfigInt(L"m_bCheckRG",		pDlg->m_bCheckRG);
	WriteConfigInt(L"m_bCheckBG",		pDlg->m_bCheckBG);
	WriteConfigInt(L"m_bCheckGG",		pDlg->m_bCheckGG);
	WriteConfigInt(L"m_bCheckR",		pDlg->m_bCheckR);
	WriteConfigInt(L"m_bCheckGr",		pDlg->m_bCheckGr);
	WriteConfigInt(L"m_bCheckGb",		pDlg->m_bCheckGb);
	WriteConfigInt(L"m_bCheckB",		pDlg->m_bCheckB);

	WriteConfigInt(L"m_RGLower",		pDlg->m_RGLower);
	WriteConfigInt(L"m_RGUpper",		pDlg->m_RGUpper);
	WriteConfigInt(L"m_BGLower",		pDlg->m_BGLower);
	WriteConfigInt(L"m_BGUpper",		pDlg->m_BGUpper);
	WriteConfigInt(L"m_GGLower",		pDlg->m_GGLower);
	WriteConfigInt(L"m_GGUpper",		pDlg->m_GGUpper);
	WriteConfigInt(L"m_RLower",		pDlg->m_RLower);
	WriteConfigInt(L"m_RUpper",		pDlg->m_RUpper);
	WriteConfigInt(L"m_GrLower",		pDlg->m_GrLower);
	WriteConfigInt(L"m_GrUpper",		pDlg->m_GrUpper);
	WriteConfigInt(L"m_GbLower",		pDlg->m_GbLower);
	WriteConfigInt(L"m_GbUpper",		pDlg->m_GbUpper);
	WriteConfigInt(L"m_BLower",		pDlg->m_BLower);
	WriteConfigInt(L"m_BUpper",		pDlg->m_BUpper);

	WriteConfigInt(L"m_bCheckGoldRG",		pDlg->m_bCheckGoldRG);
	WriteConfigInt(L"m_bCheckGoldBG",		pDlg->m_bCheckGoldBG);
	WriteConfigInt(L"m_bCheckGoldGG",		pDlg->m_bCheckGoldGG);
	WriteConfigInt(L"m_bCheckGoldR",		pDlg->m_bCheckGoldR);
	WriteConfigInt(L"m_bCheckGoldGr",		pDlg->m_bCheckGoldGr);
	WriteConfigInt(L"m_bCheckGoldGb",		pDlg->m_bCheckGoldGb);
	WriteConfigInt(L"m_bCheckGoldB",		pDlg->m_bCheckGoldB);

	WriteConfigInt(L"m_GoldRG", pDlg->m_GoldRG);
	WriteConfigInt(L"m_GoldBG", pDlg->m_GoldBG);
	WriteConfigInt(L"m_GoldGG", pDlg->m_GoldGG);
	WriteConfigInt(L"m_GoldR", pDlg->m_GoldR);
	WriteConfigInt(L"m_GoldGr", pDlg->m_GoldGr);
	WriteConfigInt(L"m_GoldGb", pDlg->m_GoldGb);
	WriteConfigInt(L"m_GoldB", pDlg->m_GoldB);

	MatchProjName(pDlg->m_projName);
	pDlg->PostMessage(WM_COMMAND,   MAKEWPARAM(IDC_COMBO_PROJ_NAME, CBN_SELCHANGE), (LPARAM)pDlg->m_hWnd);

	m_pOtpCheck->m_OtpType = pDlg->m_OtpType;

	m_pOtpCheck->m_bCheckFlag_ModuleInfo = pDlg->m_bCheckFlag_ModuleInfo;
	m_pOtpCheck->m_bCheckFlag_AWB		= pDlg->m_bCheckFlag_AWB;
	m_pOtpCheck->m_bCheckFlag_LSC		= pDlg->m_bCheckFlag_LSC;
	m_pOtpCheck->m_bCheckFlag_AF		= pDlg->m_bCheckFlag_AF;
	m_pOtpCheck->m_bCheckFlag_PDAF		= pDlg->m_bCheckFlag_PDAF;
	m_pOtpCheck->m_bCheckFlag_PDAF = pDlg->m_bCheckFlag_PDAF;

	m_pOtpCheck->m_bCheckSum_ModuleInfo = pDlg->m_bCheckSum_ModuleInfo;
	m_pOtpCheck->m_bCheckSum_AWB		= pDlg->m_bCheckSum_AWB;
	m_pOtpCheck->m_bCheckSum_LSC		= pDlg->m_bCheckSum_LSC;
	m_pOtpCheck->m_bCheckSum_AF			= pDlg->m_bCheckSum_AF;
	m_pOtpCheck->m_bCheckSum_PDAF		= pDlg->m_bCheckSum_PDAF;

	m_pOtpCheck->m_bCompareAF = pDlg->m_bCompareAF;
	m_pOtpCheck->m_bCheckStc = pDlg->m_bCheckStc;
	m_pOtpCheck->m_bCheckInf = pDlg->m_bCheckInf;
	m_pOtpCheck->m_bCheckMac = pDlg->m_bCheckMac;
	m_pOtpCheck->m_bCheckMid = pDlg->m_bCheckMid;
	m_pOtpCheck->m_bAFCompareLimit = pDlg->m_bAFCompareLimit;
	m_pOtpCheck->m_stcLower = pDlg->m_stcLower;
	m_pOtpCheck->m_stcUpper = pDlg->m_stcUpper;
	m_pOtpCheck->m_infLower = pDlg->m_infLower;
	m_pOtpCheck->m_infUpper = pDlg->m_infUpper;
	m_pOtpCheck->m_macLower = pDlg->m_macLower;
	m_pOtpCheck->m_macUpper = pDlg->m_macUpper;
	m_pOtpCheck->m_midLower = pDlg->m_midLower;
	m_pOtpCheck->m_midUpper = pDlg->m_midUpper;

	m_pOtpCheck->m_bCheckRG = pDlg->m_bCheckRG;
	m_pOtpCheck->m_bCheckBG = pDlg->m_bCheckBG;
	m_pOtpCheck->m_bCheckGG = pDlg->m_bCheckGG;
	m_pOtpCheck->m_bCheckR = pDlg->m_bCheckR;
	m_pOtpCheck->m_bCheckGr = pDlg->m_bCheckGr;
	m_pOtpCheck->m_bCheckGb = pDlg->m_bCheckGb;
	m_pOtpCheck->m_bCheckB = pDlg->m_bCheckB;
	m_pOtpCheck->m_RGLower = pDlg->m_RGLower;
	m_pOtpCheck->m_RGUpper = pDlg->m_RGUpper;
	m_pOtpCheck->m_BGLower = pDlg->m_BGLower;
	m_pOtpCheck->m_BGUpper = pDlg->m_BGUpper;
	m_pOtpCheck->m_GGLower = pDlg->m_GGLower;
	m_pOtpCheck->m_GGUpper = pDlg->m_GGUpper;
	m_pOtpCheck->m_RLower = pDlg->m_RLower;
	m_pOtpCheck->m_RUpper = pDlg->m_RUpper;
	m_pOtpCheck->m_GrLower = pDlg->m_GrLower;
	m_pOtpCheck->m_GrUpper = pDlg->m_GrUpper;
	m_pOtpCheck->m_GbLower = pDlg->m_GbLower;
	m_pOtpCheck->m_GbUpper = pDlg->m_GbUpper;
	m_pOtpCheck->m_BLower = pDlg->m_BLower;
	m_pOtpCheck->m_BUpper = pDlg->m_BUpper;

	m_pOtpCheck->m_bCheckGoldRG = pDlg->m_bCheckGoldRG;
	m_pOtpCheck->m_bCheckGoldBG = pDlg->m_bCheckGoldBG;
	m_pOtpCheck->m_bCheckGoldGG = pDlg->m_bCheckGoldGG;
	m_pOtpCheck->m_bCheckGoldR = pDlg->m_bCheckGoldR;
	m_pOtpCheck->m_bCheckGoldGr = pDlg->m_bCheckGoldGr;
	m_pOtpCheck->m_bCheckGoldGb = pDlg->m_bCheckGoldGb;
	m_pOtpCheck->m_bCheckGoldB = pDlg->m_bCheckGoldB;
	
	m_pOtpCheck->m_GoldRG = pDlg->m_GoldRG;
	m_pOtpCheck->m_GoldBG = pDlg->m_GoldBG;
	m_pOtpCheck->m_GoldGG = pDlg->m_GoldGG;
	m_pOtpCheck->m_GoldR = pDlg->m_GoldR;
	m_pOtpCheck->m_GoldGr = pDlg->m_GoldGr;
	m_pOtpCheck->m_GoldGb = pDlg->m_GoldGb;
	m_pOtpCheck->m_GoldB = pDlg->m_GoldB;

	UpdateOTPCheckUI();
	//........................
	return RET_END;
}

void OtpCheck_Inspection::UpdateOTPCheckUI()
{
	pDlg->OnBnClickedCheckCompareaf();
	pDlg->OnBnClickedCheckStc();
	pDlg->OnBnClickedCheckInf();
	pDlg->OnBnClickedCheckMac();
	pDlg->OnBnClickedCheckMid();
	pDlg->OnBnClickedCheckGoldrg();
	pDlg->OnBnClickedCheckGoldbg();
	pDlg->OnBnClickedCheckGoldr();
	pDlg->OnBnClickedCheckGoldgr();
	pDlg->OnBnClickedCheckGoldgb();
	pDlg->OnBnClickedCheckGoldb();
	pDlg->OnBnClickedCheckRg();
	pDlg->OnBnClickedCheckBg();
	pDlg->OnBnClickedCheckR();
	pDlg->OnBnClickedCheckGr();
	pDlg->OnBnClickedCheckGb();
	pDlg->OnBnClickedCheckB();
	return;
}

/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR OtpCheck_Inspection::GetReportHeaders()
{
	static CString Header(_T("ModuleNum,Stc,Inf,Mac,Mid,R,Gr,Gb,B,RGain,BGain,GoldenR,GoldenGr,GoldenGb,GoldenB,GoldenRG,GoldenBG"));

	return Header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR OtpCheck_Inspection::GetReportLowerLimits()
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
LPCTSTR OtpCheck_Inspection::GetReportUpperLimits()
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
LPCTSTR OtpCheck_Inspection::GetReportContents()
{
	static CString Content;
	Content.Format(L"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
		m_pInterface->GetModuleNum(),
		m_pOtpCheck->m_afValue.m_bStcCode,
		m_pOtpCheck->m_afValue.m_bInfCode,
		m_pOtpCheck->m_afValue.m_bMacCode,
		m_pOtpCheck->m_afValue.m_bMidCode,
		m_pOtpCheck->m_valueAWB.R,
		m_pOtpCheck->m_valueAWB.Gr,
		m_pOtpCheck->m_valueAWB.Gb,
		m_pOtpCheck->m_valueAWB.B,
		m_pOtpCheck->m_valueAWB.RG,
		m_pOtpCheck->m_valueAWB.BG,
		m_pOtpCheck->m_valueAWB.GOLDR,
		m_pOtpCheck->m_valueAWB.GOLDGr,
		m_pOtpCheck->m_valueAWB.GOLDGb,
		m_pOtpCheck->m_valueAWB.GOLDB,
		m_pOtpCheck->m_valueAWB.GOLDRG,
		m_pOtpCheck->m_valueAWB.GOLDBG
		);

	return Content;
}

void OtpCheck_Inspection::MatchProjName(eProjectName projName)
{
	SAFE_DELETE(m_pOtpCheck);
	CString szName;
	switch(projName)
	{
	case PN_BYD_TKU8615M:
		m_pOtpCheck = new OtpCheck_TKU8615M;
		szName = L"TKU8615M";
		break;
	case PN_BYD_TRJ8564M:
		m_pOtpCheck = new OtpCheck_TRJ8564M;
		szName = L"TRJ8564M";
		break;
	case PN_HLT_HLT7038:
		m_pOtpCheck = new OtpCheck_HLT7038;
		szName = L"HLT7038";
		break;
	case PN_HLT_HLT7178:
		m_pOtpCheck = new OtpCheck_HLT7178;
		szName = L"HLT7178";
		break;
	case PN_HLT_HLT7215:
		m_pOtpCheck = new OtpCheck_HLT7215;
		szName = L"HLT7215";
		break;
	case PN_BYD_TYD8592M:
		m_pOtpCheck = new OtpCheck_TYD8592M;
		szName = L"TYD8592M";
		break;
	case PN_BYD_TYD8583M:
		m_pOtpCheck = new OtpCheck_TYD8583M;
		szName = L"TYD8583M";
		break;
	case PN_HLT_HLT7014:
		m_pOtpCheck = new OtpCheck_HLT7014;
		szName = L"HLT7014";
		break;
	case PN_BYD_FYD8853M:
		m_pOtpCheck = new OtpCheck_FYD8853M;
		szName = L"FYD8853M";
		break;
	case PN_BYD_CMK8440M:
		m_pOtpCheck = new OtpCheck_CMK8440;
		szName = L"CMK8440M";
		break;
	case PN_SAA30L2:
		m_pOtpCheck = new OtpCheck_SAA30L2K;
		szName = L"SAA30L2K";
		break;
	case PN_CB801C:
		m_pOtpCheck = new OtpCheck_CB801C;
		szName = L"CMK8440M";
		break;
	default:
		m_pInterface->AddLog(L"未知项目名称");
		m_pOtpCheck = new OtpCheckBase;
		break;
	}
	m_pOtpCheck->m_pInterface = m_pInterface;
	m_pInterface->AddLog(L"OtpCheck项目名称：" + szName, COLOR_BLUE);
}

int OtpCheck_Inspection::OtpCheck_Test()
{
	if (pDlg->m_bCheckFlag_ModuleInfo || pDlg->m_bCheckSum_ModuleInfo)
	{
		m_pInterface->AddLog(L"Check_ModuleInfo ...", COLOR_BLUE);
		if (11 == pDlg->m_projName)
		{
			if (m_pOtpCheck->Check_S5K4H7ModuleInfo())
			{
				m_pInterface->AddLog(L"Check_ModuleInfo NG", COLOR_RED);
				return FUNCTION_FAIL;
			}
			else
			{
				m_pInterface->AddLog(L"Check_ModuleInfo OK", COLOR_GREEN);
			}
		}
		else
		{
			if (m_pOtpCheck->Check_ModuleInfo())
			{
				m_pInterface->AddLog(L"Check_ModuleInfo NG", COLOR_RED);
				return FUNCTION_FAIL;
			}
			else
			{
				m_pInterface->AddLog(L"Check_ModuleInfo OK", COLOR_GREEN);
			}
		}
	}

	if (pDlg->m_bCheckFlag_AWB || pDlg->m_bCheckSum_AWB||pDlg->m_bCheckGoldR||pDlg->m_bCheckGoldGr||pDlg->m_bCheckGoldGb||pDlg->m_bCheckGoldB||pDlg->m_bCheckGoldRG||pDlg->m_bCheckGoldBG||pDlg->m_bCheckGoldGG||pDlg->m_bCheckRG||pDlg->m_bCheckBG||pDlg->m_bCheckGG||pDlg->m_bCheckR||pDlg->m_bCheckGr||pDlg->m_bCheckGb||pDlg->m_bCheckB)
	{
 		m_pInterface->AddLog(L"Check_AWB ...", COLOR_BLUE);
		if (11 == pDlg->m_projName)
		{
			if (m_pOtpCheck->Check_S5K4H7AWB())
			{
				m_pInterface->AddLog(L"Check_AWB NG", COLOR_RED);
				return FUNCTION_FAIL;
			}
			else
			{
				m_pInterface->AddLog(L"Check_AWB OK", COLOR_GREEN);
			}
		}
		else
		{
			if (m_pOtpCheck->Check_AWB())
			{
				m_pInterface->AddLog(L"Check_AWB NG", COLOR_RED);
				return FUNCTION_FAIL;
			}
			else
			{
				m_pInterface->AddLog(L"Check_AWB OK", COLOR_GREEN);
			}
		}
	}

	if (pDlg->m_bCheckFlag_LSC || pDlg->m_bCheckSum_LSC)
	{
		m_pInterface->AddLog(L"Check_LSC ...", COLOR_BLUE);
		if (11 == pDlg->m_projName)
		{
			if (m_pOtpCheck->Check_S5K4H7LSC())
			{
				m_pInterface->AddLog(L"Check_LSC NG", COLOR_RED);
				return FUNCTION_FAIL;
			}
			else
			{
				m_pInterface->AddLog(L"Check_LSC OK", COLOR_GREEN);
			}
		}
		else
		{
			if (m_pOtpCheck->Check_LSC())
			{
				m_pInterface->AddLog(L"Check_LSC NG", COLOR_RED);
				return FUNCTION_FAIL;
			}
			else
			{
				m_pInterface->AddLog(L"Check_LSC OK", COLOR_GREEN);
			}
		}
	}

	if (pDlg->m_bCheckFlag_AF || pDlg->m_bCheckSum_AF||pDlg->m_bCheckInf||pDlg->m_bCheckMac||pDlg->m_bCheckStc||pDlg->m_bCheckMid||pDlg->m_bCompareAF)
	{	
		m_pInterface->AddLog(L"Check_AF ...", COLOR_BLUE);
		if (m_pOtpCheck->Check_AF())
		{
			m_pInterface->AddLog(L"Check_AF NG", COLOR_RED);
			return FUNCTION_FAIL;
		}
		else
		{
			m_pInterface->AddLog(L"Check_AF OK", COLOR_GREEN);
		}
	}

	if (pDlg->m_bCheckFlag_PDAF || pDlg->m_bCheckSum_PDAF)
	{
		m_pInterface->AddLog(L"Check_PDAF ...", COLOR_BLUE);
		if (m_pOtpCheck->Check_PDAF())
		{
			m_pInterface->AddLog(L"Check_PDAF NG", COLOR_RED);
			return FUNCTION_FAIL;
		}
		else
		{
			m_pInterface->AddLog(L"Check_PDAF OK", COLOR_GREEN);
		}
	}

	return FUNCTION_PASS;
}