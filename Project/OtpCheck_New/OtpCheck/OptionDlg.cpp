// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OtpCheck.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "OtpCheck_Inspection.h"

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, OtpCheck_Inspection* pInspection)
	: CDialog(OptionDlg::IDD, pParent)
	, m_bCheckStc(FALSE)
	, m_bCheckInf(FALSE)
	, m_bCheckMac(FALSE)
	, m_stcLower(0)
	, m_stcUpper(300)
	, m_infLower(200)
	, m_infUpper(600)
	, m_macLower(400)
	, m_macUpper(800)
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
{
	m_pInspection = pInspection;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);               

	DDX_Check(pDX, IDC_CHECK_STC, m_bCheckStc);
	DDX_Check(pDX, IDC_CHECK_INF, m_bCheckInf);
	DDX_Check(pDX, IDC_CHECK_MAC, m_bCheckMac);
	DDX_Check(pDX, IDC_CHECK_FLAG_MODULE_INFO,	m_bCheckFlag_ModuleInfo );
	DDX_Check(pDX, IDC_CHECK_FLAG_AWB,			m_bCheckFlag_AWB        );
	DDX_Check(pDX, IDC_CHECK_FLAG_LSC,			m_bCheckFlag_LSC        );
	DDX_Check(pDX, IDC_CHECK_FLAG_AF,			m_bCheckFlag_AF         );
	DDX_Check(pDX, IDC_CHECK_FLAG_PDAF,			m_bCheckFlag_PDAF       );
	DDX_Check(pDX, IDC_CHECK_SUM_MODULE_INFO,	m_bCheckSum_ModuleInfo);
	DDX_Check(pDX, IDC_CHECK_SUM_AWB,			m_bCheckSum_AWB        );
	DDX_Check(pDX, IDC_CHECK_SUM_LSC,			m_bCheckSum_LSC        );
	DDX_Check(pDX, IDC_CHECK_SUM_AF,			m_bCheckSum_AF         );
	DDX_Check(pDX, IDC_CHECK_SUM_PDAF,			m_bCheckSum_PDAF       );
	DDX_Text(pDX, IDC_EDIT_STC_LOWER, m_stcLower);
	DDX_Text(pDX, IDC_EDIT_STC_UPPER, m_stcUpper);
	DDX_Text(pDX, IDC_EDIT_INF_LOWER, m_infLower);
	DDX_Text(pDX, IDC_EDIT_INF_UPPER, m_infUpper);
	DDX_Text(pDX, IDC_EDIT_MAC_LOWER, m_macLower);
	DDX_Text(pDX, IDC_EDIT_MAC_UPPER, m_macUpper);
	DDX_Control(pDX, IDC_COMBO_PROJ_NAME, m_comboProjName);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
	ON_CBN_SELCHANGE(IDC_COMBO_PROJ_NAME, &OptionDlg::OnCbnSelchangeComboProjName)
	ON_BN_CLICKED(IDC_ReadOTP, &OptionDlg::OnBnClickedButton)
END_MESSAGE_MAP()


// OptionDlg ��Ϣ�������
void OptionDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	delete this;

	CDialog::PostNcDestroy();
}

// �Ի��򴰿����ݸ��µ���Ӧ����
LRESULT OptionDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	//TURE �ؼ���ֵ��>����    
	//FALSE ������>�ؼ���ֵ
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

// ��������ctrl+���ּ�����Ӧ��Ϣ��ͨ������Ϣ�������ָ�
LRESULT OptionDlg::OnCtrlKeyNum(WPARAM wParam, LPARAM lParam)
{

	return 1;
}

// ��Ӧģ��ر���Ϣ
LRESULT OptionDlg::OnCameraStop(WPARAM wParam, LPARAM lParam)
{

	return 1;
}

BOOL OptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_comboProjName.InsertString(0, L"TKU8341M");
	m_comboProjName.InsertString(1, L"THY8348M");
	m_comboProjName.InsertString(2, L"EYD2467M");
	m_comboProjName.InsertString(3, L"TYD8470M");
	m_comboProjName.InsertString(4,L"HLT6014");
	m_comboProjName.InsertString(5,L"FHB6551M");
	m_comboProjName.SetCurSel(m_projName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void OptionDlg::OnCbnSelchangeComboProjName()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_projName = (eProjectName)m_comboProjName.GetCurSel();
	switch(m_projName)
	{
	case PN_BYD_THY8348M:
		break;
	case PN_BYD_TKU8341M:
		break;
	case PN_HLT_EYD2467M:
		break;
	case PN_BYD_TYD8470M:
		break;		
	case PN_HLT_HLT6014:
		break;
	case PN_BYD_FHB6551M:
		break;

	default:
		break;
	}
}


void OptionDlg::OnBnClickedButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pInspection->m_pOtpCheck->SaveOTPData();
	return;
};
