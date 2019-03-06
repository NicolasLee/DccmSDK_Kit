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
	SetConfigPath(pInterface->GetConfigPathName());	 // 设置Config路径	
	SetName(lpszName);	                             // 设置测试项目的名字
	pDlg = new OptionDlg(NULL, this);                 // 生成对话框对象
	LoadOption();                                    // 读取参数
	pDlg->Create(OptionDlg::IDD, NULL);               // 创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         // 得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TYPE_IMAGE);

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

    
	return 0;
}


/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int OtpCheck_Inspection::Testing()
{
   //TODO:在此添加测试项代码
	int iRet = OtpCheck_Test();
	switch(iRet) 
	{
	case RESULT_FAIL:
		m_pInterface->AddLog(L"FAIL", COLOR_RED);
		break;
	case RESULT_PASS:
		m_pInterface->AddLog(L"PASS", COLOR_GREEN);
		break;
	default://RESULT_NULL
		iRet = RET_END;
		break;
	}
	SetResult(iRet);
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

//	SAFE_DELETE(m_pOtpCheck);
	return 0;
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
	pDlg->m_bCheckSum_PDAF		= ReadConfigInt(L"m_bCheckSum_PDAF", 0);

	pDlg->m_bCheckStc			= ReadConfigInt(L"m_bCheckStc", 0);
	pDlg->m_bCheckInf			= ReadConfigInt(L"m_bCheckInf", 0);
	pDlg->m_bCheckMac			= ReadConfigInt(L"m_bCheckMac", 0);

	pDlg->m_stcLower			= ReadConfigInt(L"m_stcLower", 0);
	pDlg->m_stcUpper			= ReadConfigInt(L"m_stcUpper", 300);
	pDlg->m_infLower			= ReadConfigInt(L"m_infLower", 200);
	pDlg->m_infUpper			= ReadConfigInt(L"m_infUpper", 600);
	pDlg->m_macLower			= ReadConfigInt(L"m_macLower", 400);
	pDlg->m_macUpper			= ReadConfigInt(L"m_macUpper", 800);

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
	if(!Inspection::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   


	WriteConfigInt(L"ProjectName", pDlg->m_projName);

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
	WriteConfigInt(L"m_bCheckSum_PDAF",		pDlg->m_bCheckSum_PDAF);

	WriteConfigInt(L"m_bCheckStc",		pDlg->m_bCheckStc);
	WriteConfigInt(L"m_bCheckInf",		pDlg->m_bCheckInf);
	WriteConfigInt(L"m_bCheckMac",		pDlg->m_bCheckMac);

	WriteConfigInt(L"m_stcLower",		pDlg->m_stcLower);
	WriteConfigInt(L"m_stcUpper",		pDlg->m_stcUpper);
	WriteConfigInt(L"m_infLower",		pDlg->m_infLower);
	WriteConfigInt(L"m_infUpper",		pDlg->m_infUpper);
	WriteConfigInt(L"m_macLower",		pDlg->m_macLower);
	WriteConfigInt(L"m_macUpper",		pDlg->m_macUpper);

	MatchProjName(pDlg->m_projName);
	pDlg->PostMessage(WM_COMMAND,   MAKEWPARAM(IDC_COMBO_PROJ_NAME, CBN_SELCHANGE), (LPARAM)pDlg->m_hWnd);
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
	m_pOtpCheck->m_bCheckSum_PDAF = pDlg->m_bCheckSum_PDAF;

	m_pOtpCheck->m_bCheckStc = pDlg->m_bCheckStc;
	m_pOtpCheck->m_bCheckInf = pDlg->m_bCheckInf;
	m_pOtpCheck->m_bCheckMac = pDlg->m_bCheckMac;
	m_pOtpCheck->m_stcLower = pDlg->m_stcLower;
	m_pOtpCheck->m_stcUpper = pDlg->m_stcUpper;
	m_pOtpCheck->m_infLower = pDlg->m_infLower;
	m_pOtpCheck->m_infUpper = pDlg->m_infUpper;
	m_pOtpCheck->m_macLower = pDlg->m_macLower;
	m_pOtpCheck->m_macUpper = pDlg->m_macUpper;
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR OtpCheck_Inspection::GetReportHeaders()
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
	Content = _T("");
	return Content;
}

void OtpCheck_Inspection::MatchProjName(eProjectName projName)
{
	SAFE_DELETE(m_pOtpCheck);
	CString szName;
	switch(projName)
	{
	case PN_BYD_THY8348M:
		m_pOtpCheck = new OtpCheck_THY8348M;
		szName = L"THY8348M";
		break;
	case PN_BYD_TKU8341M:
		m_pOtpCheck = new OtpCheck_TKU8341M;
		szName = L"TKU8341M";
		break;
	case PN_HLT_EYD2467M:
		m_pOtpCheck = new OtpCheck_EYD2467M;
		szName = L"EYD2467M";
		break;
	case PN_BYD_TYD8470M:
		m_pOtpCheck = new OtpCheck_TYD8470M;
		szName = L"TYD8470M";
		break;
	case PN_HLT_HLT6014:
		m_pOtpCheck = new OtpCheck_HLT6014;
		szName = L"HLT6014";
		break;
	case PN_BYD_FHB6551M:
		m_pOtpCheck = new OtpCheck_FHB6551M;
		szName = L"FHB6551M";
	case PN_BYD_PLQ8579M:
		m_pOtpCheck = new OtpCheck_PLQ8579M;
		szName = L"PLQ8579M";
		break;
	case PN_BYD_TRJ8495M:
		m_pOtpCheck = new OtpCheck_TRJ8495M;
		szName = L"TRJ8495M";
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
	BOOL bRet = TRUE;

#ifdef  sensorOTP
	if (!m_pOtpCheck->CheckValidGroup())
	{
		m_pInterface->AddLog(L"Check OTP Valid Group Fail", COLOR_RED);
		return RESULT_FAIL;
		bRet &= FALSE;
	}
#endif
	if (pDlg->m_bCheckFlag_ModuleInfo || pDlg->m_bCheckSum_ModuleInfo)
	{
		m_pInterface->AddLog(L"Check_ModuleInfo ...", COLOR_BLUE);
		if (!m_pOtpCheck->Check_ModuleInfo())
		{
			m_pInterface->AddLog(L"Check_ModuleInfo NG", COLOR_RED);
			return RESULT_FAIL;
			bRet &= FALSE;
		}
		else
		{
			if (pDlg->m_bCheckFlag_ModuleInfo)
			{
				m_pInterface->AddLog(L"Check_ModuleInfo OK", COLOR_GREEN);
			}
		}
	}

	if (pDlg->m_bCheckFlag_AWB || pDlg->m_bCheckSum_AWB)
	{
		m_pInterface->AddLog(L"Check_AWB ...", COLOR_BLUE);
		if (!m_pOtpCheck->Check_AWB())
		{
			m_pInterface->AddLog(L"Check_AWB NG", COLOR_RED);
			return RESULT_FAIL;
			bRet &= FALSE;
		}
		else
		{
			m_pInterface->AddLog(L"Check_AWB OK", COLOR_GREEN);
		}
	}

	if (pDlg->m_bCheckFlag_LSC || pDlg->m_bCheckSum_LSC)
	{
		m_pInterface->AddLog(L"Check_LSC ...", COLOR_BLUE);
		if (!m_pOtpCheck->Check_LSC())
		{
			m_pInterface->AddLog(L"Check_LSC NG", COLOR_RED);
			return RESULT_FAIL;
			bRet &= FALSE;
		}
		else
		{
			m_pInterface->AddLog(L"Check_LSC OK", COLOR_GREEN);
		}
	}

	if (pDlg->m_bCheckFlag_AF || pDlg->m_bCheckSum_AF)
	{	
		m_pInterface->AddLog(L"Check_AF ...", COLOR_BLUE);
		if (!m_pOtpCheck->Check_AF())
		{
			m_pInterface->AddLog(L"Check_AF NG", COLOR_RED);
			return RESULT_FAIL;
			bRet &= FALSE;
		}
		else
		{
			m_pInterface->AddLog(L"Check_AF OK", COLOR_GREEN);
		}
	}

	if (pDlg->m_bCheckFlag_PDAF || pDlg->m_bCheckSum_PDAF)
	{
		m_pInterface->AddLog(L"Check_PDAF ...", COLOR_BLUE);
		if (!m_pOtpCheck->Check_PDAF())
		{
			m_pInterface->AddLog(L"Check_PDAF NG", COLOR_RED);
			return RESULT_FAIL;
			bRet &= FALSE;
		}
		else
		{
			m_pInterface->AddLog(L"Check_PDAF OK", COLOR_GREEN);
		}
	}

	return bRet ?  RESULT_PASS :  RESULT_FAIL;
}