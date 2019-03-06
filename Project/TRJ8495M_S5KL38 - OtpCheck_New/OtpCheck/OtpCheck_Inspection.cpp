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
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
OtpCheck_Inspection::OtpCheck_Inspection(DccmControl_Interface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	SetConfigPath(pInterface->GetConfigPathName());	 // ����Config·��	
	SetName(lpszName);	                             // ���ò�����Ŀ������
	pDlg = new OptionDlg(NULL, this);                 // ���ɶԻ������
	LoadOption();                                    // ��ȡ����
	pDlg->Create(OptionDlg::IDD, NULL);               // �����Ի��򴰿�
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         // �õ��Ի�����

	//TODO: ���ò�����Ŀ������ 
	SetType(TYPE_IMAGE);

	//TODO: �������Ա�����ʼ��
	m_pOtpCheck = NULL;

	//.....
}

OtpCheck_Inspection::~OtpCheck_Inspection(void)
{

}

/******************************************************************
������:    Initialize
��������:  ���ڲ���ǰ��ʼ��һЩ������״̬������ռ��
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int OtpCheck_Inspection::Initialize()
{
	Inspection::Initialize();
	//TODO:�ڴ���ӳ�ʼ������

    
	return 0;
}


/******************************************************************
������:    Testing
��������:  ���ĳһ����Թ��ܴ�������
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int OtpCheck_Inspection::Testing()
{
   //TODO:�ڴ���Ӳ��������
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
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int OtpCheck_Inspection::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������

//	SAFE_DELETE(m_pOtpCheck);
	return 0;
}


/******************************************************************
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int OtpCheck_Inspection::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ���� 
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
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int OtpCheck_Inspection::SaveOption()
{
	if(!Inspection::SaveOption()) return -1;
	//TODO: �ڴ���Ӵ洢����	   


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
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR OtpCheck_Inspection::GetReportHeaders()
{
	static CString header;
	header = _T("");
	return header;
}


/******************************************************************
������:    GetReportLowerLimits
��������:  ������Ա�׼������ֵ
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR OtpCheck_Inspection::GetReportLowerLimits()
{
	static CString LowerLimit;
	LowerLimit = _T("");
	return LowerLimit;
}


/******************************************************************
������:    GetReportUpperLimits
��������:  ������Ա�׼������ֵ
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR OtpCheck_Inspection::GetReportUpperLimits()
{
	static CString UpperLimit;
	UpperLimit = _T("");
	return UpperLimit;
}


/******************************************************************
������:    GetReportContents
��������:  �����Ӧ������������Ĳ��Ե�����
����ֵ��   �ַ���ָ��
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
		m_pInterface->AddLog(L"δ֪��Ŀ����");
		m_pOtpCheck = new OtpCheckBase;
		break;
	}
	m_pOtpCheck->m_pInterface = m_pInterface;
	m_pInterface->AddLog(L"OtpCheck��Ŀ���ƣ�" + szName, COLOR_BLUE);
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