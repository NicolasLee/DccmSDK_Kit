// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OtpCheck.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "OtpCheck_Inspection.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, OtpCheck_Inspection* pInspection)
	: CDialog(OptionDlg::IDD, pParent)
	, m_bCompareAF(FALSE)
	, m_bCheckStc(FALSE)
	, m_bCheckInf(FALSE)
	, m_bCheckMac(FALSE)
	, m_bCheckMid(FALSE)
	, m_stcLower(0)
	, m_stcUpper(300)
	, m_infLower(200)
	, m_infUpper(600)
	, m_macLower(400)
	, m_macUpper(800)
	, m_midLower(300)
	, m_midUpper(500)
	, m_projName(PN_NULL)
	, m_bCheckFlag_ModuleInfo(FALSE)
	, m_bCheckFlag_AWB(FALSE)
	, m_bCheckFlag_LSC(FALSE)
	, m_bCheckFlag_AF(FALSE)
	, m_bCheckFlag_PDAF(FALSE)
	, m_bCheckSum_ModuleInfo(FALSE)
	, m_bCheckSum_AWB(FALSE)
	, m_bCheckSum_LSC(FALSE)
	, m_bCheckSum_AF(FALSE)
	, m_bCheckSum_PDAF(FALSE)
	, m_OtpType(FALSE)
{
	m_pInspection = pInspection;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);               
	DDX_Check(pDX,IDC_CHECK_COMPAREAF, m_bCompareAF);
	DDX_Check(pDX, IDC_CHECK_STC, m_bCheckStc);
	DDX_Check(pDX, IDC_CHECK_INF, m_bCheckInf);
	DDX_Check(pDX, IDC_CHECK_MAC, m_bCheckMac);
	DDX_Check(pDX, IDC_CHECK_MID, m_bCheckMid);
	DDX_Check(pDX, IDC_CHECK_RG, m_bCheckRG);
	DDX_Check(pDX, IDC_CHECK_BG, m_bCheckBG);
	DDX_Check(pDX, IDC_CHECK_GG, m_bCheckGG);
	DDX_Check(pDX, IDC_CHECK_R, m_bCheckR);
	DDX_Check(pDX, IDC_CHECK_GR, m_bCheckGr);
	DDX_Check(pDX, IDC_CHECK_GB, m_bCheckGb);
	DDX_Check(pDX, IDC_CHECK_B, m_bCheckB);
	DDX_Check(pDX, IDC_CHECK_GOLDRG, m_bCheckGoldRG);
	DDX_Check(pDX, IDC_CHECK_GOLDBG, m_bCheckGoldBG);
	DDX_Check(pDX, IDC_CHECK_GOLDGG, m_bCheckGoldGG);
	DDX_Check(pDX, IDC_CHECK_GOLDR, m_bCheckGoldR);
	DDX_Check(pDX, IDC_CHECK_GOLDGR, m_bCheckGoldGr);
	DDX_Check(pDX, IDC_CHECK_GOLDGB, m_bCheckGoldGb);
	DDX_Check(pDX, IDC_CHECK_GOLDB, m_bCheckGoldB);
	DDX_Check(pDX, IDC_CHECK_FLAG_MODULE_INFO, m_bCheckFlag_ModuleInfo );
	DDX_Check(pDX, IDC_CHECK_FLAG_AWB, m_bCheckFlag_AWB        );
	DDX_Check(pDX, IDC_CHECK_FLAG_LSC, m_bCheckFlag_LSC        );
	DDX_Check(pDX, IDC_CHECK_FLAG_AF, m_bCheckFlag_AF         );
	DDX_Check(pDX, IDC_CHECK_FLAG_PDAF, m_bCheckFlag_PDAF       );
	DDX_Check(pDX, IDC_CHECK_SUM_MODULE_INFO, m_bCheckSum_ModuleInfo);
	DDX_Check(pDX, IDC_CHECK_SUM_AWB,	 m_bCheckSum_AWB        );
	DDX_Check(pDX, IDC_CHECK_SUM_LSC, m_bCheckSum_LSC        );
	DDX_Check(pDX, IDC_CHECK_SUM_AF, m_bCheckSum_AF         );
	DDX_Check(pDX, IDC_CHECK_SUM_PDAF, m_bCheckSum_PDAF );
	DDX_Text(pDX, IDC_EDIT_AFCOMPARELIMIT, m_bAFCompareLimit);
	DDX_Text(pDX, IDC_EDIT_STC_LOWER, m_stcLower);
	DDX_Text(pDX, IDC_EDIT_STC_UPPER, m_stcUpper);
	DDX_Text(pDX, IDC_EDIT_INF_LOWER, m_infLower);
	DDX_Text(pDX, IDC_EDIT_INF_UPPER, m_infUpper);
	DDX_Text(pDX, IDC_EDIT_MAC_LOWER, m_macLower);
	DDX_Text(pDX, IDC_EDIT_MAC_UPPER, m_macUpper);
	DDX_Text(pDX, IDC_EDIT_MID_LOWER, m_midLower);
	DDX_Text(pDX, IDC_EDIT_MID_UPPER, m_midUpper);
	DDX_Text(pDX, IDC_EDIT_RG_LOWER, m_RGLower);
	DDX_Text(pDX, IDC_EDIT_RG_UPPER, m_RGUpper);
	DDX_Text(pDX, IDC_EDIT_BG_LOWER, m_BGLower);
	DDX_Text(pDX, IDC_EDIT_BG_UPPER, m_BGUpper);
	DDX_Text(pDX, IDC_EDIT_GG_LOWER, m_GGLower);
	DDX_Text(pDX, IDC_EDIT_GG_UPPER, m_GGUpper);
	DDX_Text(pDX, IDC_EDIT_R_LOWER, m_RLower);
	DDX_Text(pDX, IDC_EDIT_R_UPPER, m_RUpper);
	DDX_Text(pDX, IDC_EDIT_GR_LOWER, m_GrLower);
	DDX_Text(pDX, IDC_EDIT_GR_UPPER, m_GrUpper);
	DDX_Text(pDX, IDC_EDIT_GB_LOWER, m_GbLower);
	DDX_Text(pDX, IDC_EDIT_GB_UPPER, m_GbUpper);
	DDX_Text(pDX, IDC_EDIT_B_LOWER, m_BLower);
	DDX_Text(pDX, IDC_EDIT_B_UPPER, m_BUpper);
	DDX_Text(pDX, IDC_EDIT_GOLDRG, m_GoldRG);
	DDX_Text(pDX, IDC_EDIT_GOLDBG, m_GoldBG);
	DDX_Text(pDX, IDC_EDIT_GOLDGG, m_GoldGG);
	DDX_Text(pDX, IDC_EDIT_GOLDR, m_GoldR);
	DDX_Text(pDX, IDC_EDIT_GOLDGR, m_GoldGr);
	DDX_Text(pDX, IDC_EDIT_GOLDGB, m_GoldGb);
	DDX_Text(pDX, IDC_EDIT_GOLDB, m_GoldB);
	DDX_Control(pDX, IDC_COMBO_PROJ_NAME, m_comboProjName);
	DDX_Radio(pDX, IDC_RADIO_OTPTYPE, m_OtpType);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
	ON_CBN_SELCHANGE(IDC_COMBO_PROJ_NAME, &OptionDlg::OnCbnSelchangeComboProjName)
	ON_BN_CLICKED(IDC_ReadOTP, &OptionDlg::OnBnClickedSaveOTPData)
	ON_BN_CLICKED(IDC_CHECK_COMPAREAF, &OptionDlg::OnBnClickedCheckCompareaf)
	ON_BN_CLICKED(IDC_CHECK_STC, &OptionDlg::OnBnClickedCheckStc)
	ON_BN_CLICKED(IDC_CHECK_INF, &OptionDlg::OnBnClickedCheckInf)
	ON_BN_CLICKED(IDC_CHECK_MAC, &OptionDlg::OnBnClickedCheckMac)
	ON_BN_CLICKED(IDC_CHECK_MID, &OptionDlg::OnBnClickedCheckMid)
	ON_BN_CLICKED(IDC_CHECK_GOLDRG, &OptionDlg::OnBnClickedCheckGoldrg)
	ON_BN_CLICKED(IDC_CHECK_GOLDBG, &OptionDlg::OnBnClickedCheckGoldbg)
	ON_BN_CLICKED(IDC_CHECK_GOLDR, &OptionDlg::OnBnClickedCheckGoldr)
	ON_BN_CLICKED(IDC_CHECK_GOLDGR, &OptionDlg::OnBnClickedCheckGoldgr)
	ON_BN_CLICKED(IDC_CHECK_GOLDGB, &OptionDlg::OnBnClickedCheckGoldgb)
	ON_BN_CLICKED(IDC_CHECK_GOLDB, &OptionDlg::OnBnClickedCheckGoldb)
	ON_BN_CLICKED(IDC_CHECK_RG, &OptionDlg::OnBnClickedCheckRg)
	ON_BN_CLICKED(IDC_CHECK_BG, &OptionDlg::OnBnClickedCheckBg)
	ON_BN_CLICKED(IDC_CHECK_R, &OptionDlg::OnBnClickedCheckR)
	ON_BN_CLICKED(IDC_CHECK_GR, &OptionDlg::OnBnClickedCheckGr)
	ON_BN_CLICKED(IDC_CHECK_GB, &OptionDlg::OnBnClickedCheckGb)
	ON_BN_CLICKED(IDC_CHECK_B, &OptionDlg::OnBnClickedCheckB)
	ON_CBN_SELCHANGE(IDC_COMBO_SET_TYPE, &OptionDlg::OnCbnSelchangeComboSetType)
END_MESSAGE_MAP()


// OptionDlg 消息处理程序
void OptionDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;

	CDialog::PostNcDestroy();
}

// 对话框窗口数据更新的响应函数
LRESULT OptionDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	//TURE 控件的值—>变量    
	//FALSE 变量—>控件的值
	BOOL	bSaveAndValidate = (BOOL) wParam; 

	int ret = UpdateData(bSaveAndValidate);

	// TODO: Add your specialized code here
	if(bSaveAndValidate)
	{
	}
	else
	{
	}

	return ret;
}

// 主程序按下ctrl+数字键的响应消息，通过此消息接受外界指令。
LRESULT OptionDlg::OnCtrlKeyNum(WPARAM wParam, LPARAM lParam)
{

	return 1;
}

// 响应模组关闭消息
LRESULT OptionDlg::OnCameraStop(WPARAM wParam, LPARAM lParam)
{

	return 1;
}

void OptionDlg::OnBnClickedSaveOTPData()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pInspection->m_pInterface->PauseGrabFrame();
	m_pInspection->m_pOtpCheck->SaveOTPData();
	m_pInspection->m_pInterface->ResumeGrabFrame();
	return;
};

BOOL OptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_comboProjName.InsertString(0,L"TKU8615M");
	m_comboProjName.InsertString(1,L"TRJ8564M");
	m_comboProjName.InsertString(2,L"HLT7038");
	m_comboProjName.InsertString(3,L"HLT7178");
	m_comboProjName.InsertString(4,L"HLT7215");
	m_comboProjName.InsertString(5,L"TYD8592M");
	m_comboProjName.InsertString(6,L"TYD8583M");
	m_comboProjName.InsertString(7,L"HLT7014");
	m_comboProjName.InsertString(8,L"FYD8853M");	
	m_comboProjName.InsertString(9,L"CMK8440M");
	m_comboProjName.InsertString(10, L"SAA30L2");
	m_comboProjName.InsertString(11, L"CB801C");
	m_comboProjName.SetCurSel(m_projName);

	CComboBox* pComboSetType = (CComboBox*)GetDlgItem(IDC_COMBO_SET_TYPE);
	for (int i = 0; i < TET_ALL; i++)
	{
		pComboSetType->AddString(m_pInspection->GetTypeName(i));
	}
	pComboSetType->SetCurSel(CurTestType);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void OptionDlg::OnCbnSelchangeComboProjName()
{
	// TODO: 在此添加控件通知处理程序代码
	m_projName = (eProjectName)m_comboProjName.GetCurSel();
	switch(m_projName)
	{
	case PN_BYD_TKU8615M:
		break;
	case PN_BYD_TRJ8564M:
		break;
	case PN_HLT_HLT7038:
		break;
	case PN_HLT_HLT7178:
		break;
	case PN_HLT_HLT7215:
		break;
	case PN_BYD_TYD8592M:
		break;
	case PN_BYD_TYD8583M:
		break;
	case PN_HLT_HLT7014:
		break;
	case PN_BYD_FYD8853M:
		break;
	case PN_BYD_CMK8440M:
		break;
	case PN_SAA30L2:
		break;
	case PN_CB801C:
		break;
	default:
		break;
	}
}

void OptionDlg::OnBnClickedCheckCompareaf()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_COMPAREAF))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_AFCOMPARELIMIT))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_AFCOMPARELIMIT))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckStc()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_STC))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_STC_LOWER))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_EDIT_STC_UPPER))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_STC_LOWER))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_EDIT_STC_UPPER))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckInf()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_INF))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_INF_LOWER))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_EDIT_INF_UPPER))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_INF_LOWER))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_EDIT_INF_UPPER))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckMac()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_MAC))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_MAC_LOWER))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_EDIT_MAC_UPPER))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_MAC_LOWER))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_EDIT_MAC_UPPER))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckMid()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_MID))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_MID_LOWER))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_EDIT_MID_UPPER))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_MID_LOWER))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_EDIT_MID_UPPER))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckGoldrg()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_GOLDRG))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_GOLDRG))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_GOLDRG))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckGoldbg()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_GOLDBG))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_GOLDBG))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_GOLDBG))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckGoldr()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_GOLDR))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_GOLDR))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_GOLDR))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckGoldgr()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_GOLDGR))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_GOLDGR))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_GOLDGR))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckGoldgb()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_GOLDGB))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_GOLDGB))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_GOLDGB))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckGoldb()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_GOLDB))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_GOLDB))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_GOLDB))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckRg()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_RG))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_RG_LOWER))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_EDIT_RG_UPPER))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_RG_LOWER))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_EDIT_RG_UPPER))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckBg()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_BG))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_BG_LOWER))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_EDIT_BG_UPPER))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_BG_LOWER))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_EDIT_BG_UPPER))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckR()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_R))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_R_LOWER))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_EDIT_R_UPPER))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_R_LOWER))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_EDIT_R_UPPER))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckGr()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_GR))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_GR_LOWER))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_EDIT_GR_UPPER))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_GR_LOWER))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_EDIT_GR_UPPER))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckGb()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_GB))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_GB_LOWER))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_EDIT_GB_UPPER))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_GB_LOWER))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_EDIT_GB_UPPER))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedCheckB()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_CHECK_B))->GetCheck();

	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_EDIT_B_LOWER))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_EDIT_B_UPPER))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_EDIT_B_LOWER))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_EDIT_B_UPPER))->EnableWindow(FALSE);
	}
}


void OptionDlg::OnCbnSelchangeComboSetType()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szTemp, szTypeName;
	CComboBox* pComboSetType = (CComboBox*)GetDlgItem(IDC_COMBO_SET_TYPE);
	CurTestType = pComboSetType->GetCurSel();
	m_pInspection->SetType(CurTestType);

	pComboSetType->GetLBText(m_pInspection->GetType(), szTypeName);
	szTemp.Format(L"%s 已切换为 %s", m_pInspection->GetName(), szTypeName);
	m_pInspection->m_pInterface->AddLog(szTemp, COLOR_BLUE);
}
