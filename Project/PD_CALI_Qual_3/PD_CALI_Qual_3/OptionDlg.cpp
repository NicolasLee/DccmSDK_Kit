// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PD_CALI_Qual_3.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "PD_CALI_Qual_3_Inspection.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, PD_CALI_Qual_3_Inspection* pInspection)
	: CDialog(OptionDlg::IDD, pParent)
	, m_bSaveRaw(false)
{
	m_pInspection = pInspection;

	m_szDEV_ADDR = _T(""); 
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EXP_TOLERANCE, m_Exp_Tolerance);
	DDV_MinMaxInt(pDX, m_Exp_Tolerance, 0, 256);
	DDX_Text(pDX, IDC_EXP_TARGET, m_Exp_Target);
	DDV_MinMaxInt(pDX, m_Exp_Target, 0, 256);
	DDX_Check(pDX, IDC_CHECK_EXPOSURE, m_ExposureEn);
	DDX_Check(pDX, IDC_QVL, m_QVLEn);

	DDX_Text(pDX, IDC_DEV_ADDR, m_szDEV_ADDR);
	DDV_MaxChars(pDX, m_szDEV_ADDR, 2);
	DDX_Text(pDX, IDC_REG_ADDR, m_szReg_ADDR);
	DDV_MaxChars(pDX,m_szReg_ADDR,4);
	DDX_Text(pDX, IDC_PAGE_SIZE, m_pInspection->m_PageSize);
	DDV_MinMaxInt(pDX, m_pInspection->m_PageSize, 0, 1024);
	DDX_Text(pDX,IDC_SLEEP_TIME,m_pInspection->m_SleepTime);
	DDX_Text(pDX,IDC_I2C_MODE,m_pInspection->m_I2cMode);

	DDX_Text(pDX, IDC_INF_LSB_REG,m_szInf_Lsb_ADDR);
	DDV_MaxChars(pDX,m_szInf_Lsb_ADDR,4);
	DDX_Text(pDX, IDC_INF_MSB_REG, m_szInf_Msb_ADDR);
	DDV_MaxChars(pDX,m_szInf_Msb_ADDR,4);
	DDX_Text(pDX, IDC_MAC_LSB_REG, m_szMac_Lsb_ADDR);
	DDV_MaxChars(pDX,m_szMac_Lsb_ADDR,4);
	DDX_Text(pDX, IDC_MAC_MSB_REG, m_szMac_Msb_ADDR);
	DDV_MaxChars(pDX,m_szMac_Msb_ADDR,4);
	DDX_Text(pDX, IDC_INI_NAME, m_pInspection->m_Ini_Name);
	DDX_Check(pDX, IDC_CHECK_SAVE_RAW, m_bSaveRaw);
	DDX_Check(pDX, IDC_CHECK_ERASE, m_berase);

	DDX_Text(pDX, IDC_InfH, m_InfH);
	DDV_MinMaxUInt(pDX, m_InfH, 0, 1023);
	DDX_Text(pDX, IDC_InfL, m_InfL);
	DDV_MinMaxUInt(pDX, m_InfL, 0, 1023);
	DDX_Text(pDX, IDC_MacH, m_MacH);
	DDV_MinMaxUInt(pDX, m_MacH, 0, 1023);
	DDX_Text(pDX, IDC_MacL, m_MacL);
	DDV_MinMaxUInt(pDX, m_MacL, 0, 1023);
	DDX_Text(pDX, IDC_MidH, m_MidH);
	DDV_MinMaxUInt(pDX, m_MacH, 0, 1023);
	DDX_Text(pDX, IDC_MidL, m_MidL);
	DDV_MinMaxUInt(pDX, m_MacL, 0, 1023);
	DDX_Text(pDX, IDC_AFDif, m_AFDif);
	DDV_MinMaxUInt(pDX, m_AFDif, 0, 1023);

	DDX_Text(pDX, IDC_CodeDiff, m_pInspection->m_CodeSPC);
	DDV_MinMaxInt(pDX, m_pInspection->m_CodeSPC, 0, 1023);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
END_MESSAGE_MAP()


// OptionDlg 消息处理程序
void OptionDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;

	CDialog::PostNcDestroy();
}

//对话框窗口数据更新的响应函数
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

//主程序按下ctrl+数字键的响应消息，通过此消息接受外界指令。
LRESULT OptionDlg::OnCtrlKeyNum(WPARAM wParam, LPARAM lParam)
{

	return 1;
}

//响应模组关闭消息
LRESULT OptionDlg::OnCameraStop(WPARAM wParam, LPARAM lParam)
{

	return 1;
}

BOOL OptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
