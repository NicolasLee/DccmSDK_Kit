// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OTP.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "OTP_Inspection.h"

// OptionDlg 对话框
#define  MaxGroup 2
IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, OTP_Inspection* pInspection)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_goldengain = 0;
	m_LSCItem = 0;
	m_PDAFItem = 0;
	m_pInspection = pInspection;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SleepTime, m_sleeptime);
	DDV_MinMaxUInt(pDX, m_sleeptime, 0, 10);
	DDX_Text(pDX, IDC_SD_EXPTOLERANCE, m_exp_tolerance);
	DDV_MinMaxUInt(pDX, m_exp_tolerance, 0, 100);
	DDX_Text(pDX, IDC_SD_EXPTARGET, m_exp_target);
	DDV_MinMaxUInt(pDX, m_exp_target, 1, 255);
	DDX_Check(pDX, IDC_CHECK_EXPOUSE, m_ExposureEn);
	DDX_Text(pDX, IDC_SD_PDAFEXPTARGET, m_exp_pdaftarget);
	DDV_MinMaxUInt(pDX, m_exp_pdaftarget, 1, 255);
	DDX_Check(pDX, IDC_CHECK_PDAFEXPOUSE, m_PDAFExposureEn);
	DDX_Check(pDX, IDC_ENLSC, m_EnLSC);
	DDX_Check(pDX, IDC_ENAWB, m_EnAWB);
	DDX_Check(pDX, IDC_ENAF, m_EnAF);
	DDX_Check(pDX, IDC_ENPDAF, m_EnPDAF);
	DDX_Check(pDX, IDC_ENQVL, m_EnQVL);
	DDX_Check(pDX, IDC_CHECK_ERASE, m_berase);      
	DDX_Check(pDX, IDC_CHECKINF, m_bcheckinf);
	DDX_Check(pDX, IDC_CHECKMAC, m_bcheckmac);
	DDX_Check(pDX, IDC_CHECKSTA, m_bchecksta);
	DDX_Check(pDX, IDC_DummyBurn, m_dummyburn); 
	DDX_Check(pDX, IDC_TwoSettings,m_twosetting);

	DDX_Text(pDX, IDC_GoldenRG, m_goldenrg);
	DDX_Text(pDX, IDC_GoldenBG, m_goldenbg);
	DDX_Text(pDX, IDC_GoldenGG, m_goldengg);

	DDX_Text(pDX, IDC_RGainH, m_rgainh);
	DDV_MinMaxUInt(pDX, m_rgainh, 0, 1023);
	DDX_Text(pDX, IDC_RGainL, m_rgainl);
	DDV_MinMaxUInt(pDX, m_rgainl, 0, 1023);
	DDX_Text(pDX, IDC_BGainH, m_bgainh);
	DDV_MinMaxUInt(pDX, m_bgainh, 0, 1023);
	DDX_Text(pDX, IDC_BGainL, m_bgainl);
	DDV_MinMaxUInt(pDX, m_bgainl, 0, 1023);
	DDX_Text(pDX, IDC_RGainDif, m_rgaindif);
	DDV_MinMaxDouble(pDX, m_rgaindif, 0.0, 100.0);
	DDX_Text(pDX, IDC_BGainDif, m_bgaindif);
	DDV_MinMaxDouble(pDX, m_bgaindif, 0.0, 100.0);
	DDX_Text(pDX, IDC_OrgDif, m_orgdif);
	DDV_MinMaxDouble(pDX, m_orgdif, 0.0, 100.0);
	DDX_Text(pDX, IDC_GoldenR, m_goldenr);
	DDV_MinMaxUInt(pDX, m_goldenr, 0, 1023);
	DDX_Text(pDX, IDC_GoldenB, m_goldenb);
	DDV_MinMaxUInt(pDX, m_goldenb, 0, 1023);
	DDX_Text(pDX, IDC_GoldenGr, m_goldengr);
	DDV_MinMaxUInt(pDX, m_goldengr, 0, 1023);
	DDX_Text(pDX, IDC_GoldenGb, m_goldengb);
	DDV_MinMaxUInt(pDX, m_goldengb, 0, 1023);

	DDX_Text(pDX, IDC_InfH, m_InfH);
	DDV_MinMaxUInt(pDX, m_InfH, 0, 1023);
	DDX_Text(pDX, IDC_InfL, m_InfL);
	DDV_MinMaxUInt(pDX, m_InfL, 0, 1023);
	DDX_Text(pDX, IDC_MacH, m_MacH);
	DDV_MinMaxUInt(pDX, m_MacH, 0, 1023);
	DDX_Text(pDX, IDC_MacL, m_MacL);
	DDV_MinMaxUInt(pDX, m_MacL, 0, 1023);
	DDX_Text(pDX, IDC_StcH, m_StcH);
	DDV_MinMaxUInt(pDX, m_StcH, 0, 1023);
	DDX_Text(pDX, IDC_StcL, m_StcL);
	DDV_MinMaxUInt(pDX, m_StcL, 0, 1023);
	DDX_Text(pDX, IDC_AFDif, m_AFDif);
	DDV_MinMaxUInt(pDX, m_AFDif, 0, 1023);
	
	DDX_Text(pDX, IDC_GroupLimit, m_grouplimit);
	DDV_MinMaxUInt(pDX, m_grouplimit, 1, 3);	

	DDX_Text(pDX, IDC_MTK_INI_NAME, m_MTK_INI_Name);
	DDX_Text(pDX, IDC_Qual_INI_NAME, m_Qual_INI_Name);

	DDX_Control(pDX, IDC_ShadingItem, m_ShadingList);
	DDX_Text(pDX, IDC_YMAX, m_ymax);
	DDV_MinMaxDouble(pDX, m_ymax, 1, 100);	
	DDX_Text(pDX, IDC_YMIN, m_ymin);
	DDV_MinMaxDouble(pDX, m_ymin, 1, 100);	
	DDX_Text(pDX, IDC_YDIF, m_ydiff);
	DDV_MinMaxDouble(pDX, m_ydiff, 1, 100);	
	DDX_Text(pDX, IDC_RGDIFF, m_rgdiff);
	DDV_MinMaxDouble(pDX, m_rgdiff, 1, 100);	
	DDX_Text(pDX, IDC_BGDIFF, m_bgdiff);
	DDV_MinMaxDouble(pDX, m_bgdiff, 1, 100);

	DDX_Radio(pDX, IDC_GOLDENGAIN, m_goldengain); //0 gain 1 channel
	DDX_Radio(pDX, IDC_ENMTKLSC, m_LSCItem); // 0 MTK 1 Qual 2 Sensor
	DDX_Control(pDX,IDC_CFAItem,m_cfaList);
	DDX_Control(pDX,IDC_QUALLSCMODE,m_quallsclist);
	DDX_Check(pDX,IDC_QUALAWB,m_qualawb);
	DDX_Control(pDX,IDC_SensorLSCItem,m_SensorList);
	DDX_Text(pDX, IDC_LSCTarget, m_lscTarget);
	DDX_Text(pDX, IDC_SensorLSCGroup, m_lscGroup);
	DDV_MinMaxUInt(pDX, m_lscGroup, 1, MaxGroup);


	DDX_Radio(pDX, IDC_ENMTKPDAF, m_PDAFItem); // 0 MTK 1 Qual 2Sensor
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
	ON_EN_CHANGE(IDC_GoldenGr, &OptionDlg::OnEnChangeGoldengr)
	ON_EN_CHANGE(IDC_GoldenR, &OptionDlg::OnEnChangeGoldenr)
	ON_EN_CHANGE(IDC_GoldenB, &OptionDlg::OnEnChangeGoldenb)
	ON_EN_CHANGE(IDC_GoldenGb, &OptionDlg::OnEnChangeGoldengb)
	ON_CBN_SELCHANGE(IDC_ShadingItem, &OptionDlg::OnCbnSelchangeShadingItem)
	ON_BN_CLICKED(IDC_GOLDENCHANNEL, &OptionDlg::OnBnClickedGoldenchannel)
	ON_BN_CLICKED(IDC_GOLDENGAIN, &OptionDlg::OnBnClickedGoldengain)
	ON_BN_CLICKED(IDC_ENLSC, &OptionDlg::OnBnClickedEnlsc)
	ON_BN_CLICKED(IDC_ENAWB, &OptionDlg::OnBnClickedEnawb)
	ON_BN_CLICKED(IDC_ENAF, &OptionDlg::OnBnClickedEnaf)
	ON_BN_CLICKED(IDC_ENPDAF, &OptionDlg::OnBnClickedEnpdaf)
	ON_BN_CLICKED(IDC_ENSENSORLSC, &OptionDlg::OnBnClickedEnmtklsc)
	ON_BN_CLICKED(IDC_ENMTKLSC, &OptionDlg::OnBnClickedEnmtklsc)
	ON_CBN_SELCHANGE(IDC_SensorLSCItem, &OptionDlg::OnCbnSelchangeSensorlscitem)
	ON_CBN_SELCHANGE(IDC_CFAItem, &OptionDlg::OnCbnSelchangeCfaitem)
	ON_BN_CLICKED(IDC_ENQUALLSC, &OptionDlg::OnBnClickedEnmtklsc)
	ON_CBN_SELCHANGE(IDC_QUALLSCMODE, &OptionDlg::OnCbnSelchangeQuallscmode)
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

BOOL OptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	AddTestList();

//	((CButton *)GetDlgItem(IDC_GOLDENGAIN))->SetCheck(TRUE); //??
//	m_wbgroup.SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void OptionDlg::OnEnChangeGoldengr()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_goldenrg		= (UINT)(1.0 * (m_goldenr-SENSOROB) / (m_goldengr-SENSOROB)*AWB_FACTOR + 0.5); 
	m_goldenbg		= (UINT)(1.0 * (m_goldenb-SENSOROB) / (m_goldengb-SENSOROB)*AWB_FACTOR + 0.5);
	m_goldengg		= (UINT)(1.0 * (m_goldengb-SENSOROB) / (m_goldengr-SENSOROB)*AWB_FACTOR + 0.5);
	UpdateData(FALSE);
}

void OptionDlg::OnEnChangeGoldenr()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_goldenrg		= (UINT)(1.0 * (m_goldenr-SENSOROB) / (m_goldengr-SENSOROB)*AWB_FACTOR + 0.5); 
	m_goldenbg		= (UINT)(1.0 * (m_goldenb-SENSOROB) / (m_goldengb-SENSOROB)*AWB_FACTOR + 0.5);
	m_goldengg		= (UINT)(1.0 * (m_goldengb-SENSOROB) / (m_goldengr-SENSOROB)*AWB_FACTOR + 0.5);
	UpdateData(FALSE);
}

void OptionDlg::OnEnChangeGoldenb()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_goldenrg		= (UINT)(1.0 * (m_goldenr-SENSOROB) / (m_goldengr-SENSOROB)*AWB_FACTOR + 0.5); 
	m_goldenbg		= (UINT)(1.0 * (m_goldenb-SENSOROB) / (m_goldengb-SENSOROB)*AWB_FACTOR + 0.5);
	m_goldengg		= (UINT)(1.0 * (m_goldengb-SENSOROB) / (m_goldengr-SENSOROB)*AWB_FACTOR + 0.5);
	UpdateData(FALSE);
}

void OptionDlg::OnEnChangeGoldengb()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_goldenrg		= (UINT)(1.0 * (m_goldenr-SENSOROB) / (m_goldengr-SENSOROB)*AWB_FACTOR + 0.5); 
	m_goldenbg		= (UINT)(1.0 * (m_goldenb-SENSOROB) / (m_goldengb-SENSOROB)*AWB_FACTOR + 0.5);
	m_goldengg		= (UINT)(1.0 * (m_goldengb-SENSOROB) / (m_goldengr-SENSOROB)*AWB_FACTOR + 0.5);

	UpdateData(FALSE);
}

void OptionDlg::OnCbnSelchangeShadingItem()
{
	// TODO: 在此添加控件通知处理程序代码
	m_shadingitem = m_ShadingList.GetCurSel();
}

void OptionDlg::AddTestList()
{
	/////////////////////////////////shading////////////////////////////////////
	m_ShadingList.ResetContent();
	m_ShadingList.Clear();

	//pls add shading  item here;
	m_ShadingList.InsertString(0, _T("Shading5"));
	m_ShadingList.InsertString(1, _T("Shading14"));
	m_ShadingList.InsertString(2, _T("Shading49"));

	m_ShadingList.SetCurSel(m_shadingitem);

	///////////////////////////////sensor////////////////////////////////////////
	m_SensorList.ResetContent();
	m_SensorList.Clear();

	//pls add sensor item here;
	m_SensorList.InsertString(0,_T("S5K3L8"));
	m_SensorList.InsertString(1,_T("S5K5E8"));

	m_SensorList.SetCurSel(m_sensoritem);

	//////////////////////////////////QUAL LSC//////////////////////////////////////
	m_cfaList.ResetContent();
	m_cfaList.Clear();

	//pls add cfa item here;
	m_cfaList.InsertString(0, _T("RGGB"));
	m_cfaList.InsertString(1, _T("GRBG"));
	m_cfaList.InsertString(2, _T("BGGR"));
	m_cfaList.InsertString(3, _T("GBRG"));

	m_cfaList.SetCurSel(m_cfaitem);

	//lsc mode
	m_quallsclist.ResetContent();
	m_quallsclist.Clear();

	//pls add cfa item here;
	m_quallsclist.InsertString(0, _T("17x13"));
	m_quallsclist.InsertString(1, _T("9x7"));

	m_quallsclist.SetCurSel(m_quallscitem);

}

void OptionDlg::OnBnClickedGoldenchannel()
{
	// TODO: 在此添加控件通知处理程序代码
    int buttonstatus = ((CButton *)GetDlgItem(IDC_GOLDENCHANNEL))->GetCheck();
	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_GoldenRG))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_GoldenBG))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_GoldenGG))->EnableWindow(FALSE);

		((CButton *)GetDlgItem(IDC_GoldenR))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_GoldenB))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_GoldenGr))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_GoldenGb))->EnableWindow(TRUE);
	}
}


void OptionDlg::OnBnClickedGoldengain()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_GOLDENGAIN))->GetCheck();
	if (buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_GoldenRG))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_GoldenBG))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_GoldenGG))->EnableWindow(TRUE);

		((CButton *)GetDlgItem(IDC_GoldenR))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_GoldenB))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_GoldenGr))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_GoldenGb))->EnableWindow(FALSE);
	}
}

void OptionDlg::OnBnClickedEnlsc()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_ENLSC))->GetCheck();
	if (!buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_ENMTKLSC))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_ENQUALLSC))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_ENSENSORLSC))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_SensorLSCItem))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_LSCTarget))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_SensorLSCGroup))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_CFAItem))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_QUALLSCMODE))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_QUALAWB))->EnableWindow(FALSE);

	}
	else
	{
		((CButton *)GetDlgItem(IDC_ENMTKLSC))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_ENQUALLSC))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_ENSENSORLSC))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_SensorLSCItem))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_LSCTarget))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_SensorLSCGroup))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_CFAItem))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_QUALLSCMODE))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_QUALAWB))->EnableWindow(TRUE);
	}
}


void OptionDlg::OnBnClickedEnawb()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_ENAWB))->GetCheck();
	if (!buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_RGainH))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_RGainL))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_BGainH))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_BGainL))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_RGainDif))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_BGainDif))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_OrgDif))->EnableWindow(FALSE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RGainH))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_RGainL))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_BGainH))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_BGainL))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_RGainDif))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_BGainDif))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_OrgDif))->EnableWindow(TRUE);
	}
}


void OptionDlg::OnBnClickedEnaf()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_ENAF))->GetCheck();
	if (!buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_CHECKINF))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_CHECKMAC))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_CHECKSTA))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_InfH))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_InfL))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_MacH))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_MacL))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_StcH))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_StcL))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_AFDif))->EnableWindow(FALSE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECKINF))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_CHECKMAC))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_CHECKSTA))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_InfH))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_InfL))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_MacH))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_MacL))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_StcH))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_StcL))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_AFDif))->EnableWindow(TRUE);
	}
}


void OptionDlg::OnBnClickedEnpdaf()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton *)GetDlgItem(IDC_ENPDAF))->GetCheck();
	if (!buttonstatus)
	{
		((CButton *)GetDlgItem(IDC_ENMTKPDAF))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_ENQUALPDAF))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_ENQVL))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_MTK_INI_NAME))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_Qual_INI_NAME))->EnableWindow(FALSE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_ENMTKPDAF))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_ENQUALPDAF))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_ENQVL))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_MTK_INI_NAME))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_Qual_INI_NAME))->EnableWindow(TRUE);
	}
}

void OptionDlg::OnBnClickedEnmtklsc()
{
	// TODO: 在此添加控件通知处理程序代码
	int mtkstatus = ((CButton *)GetDlgItem(IDC_ENMTKLSC))->GetCheck();
	int qualstatus = ((CButton *)GetDlgItem(IDC_ENQUALLSC))->GetCheck();
	int sensorstatus = ((CButton *)GetDlgItem(IDC_ENSENSORLSC))->GetCheck();
	if (!sensorstatus)
	{
		((CButton *)GetDlgItem(IDC_SensorLSCItem))->ShowWindow(FALSE);
		((CButton *)GetDlgItem(IDC_LSCTarget))->ShowWindow(FALSE);
		((CButton *)GetDlgItem(IDC_SensorLSCGroup))->ShowWindow(FALSE);
		((CButton *)GetDlgItem(IDC_STATICLSC1))->ShowWindow(FALSE);
		((CButton *)GetDlgItem(IDC_STATICLSC2))->ShowWindow(FALSE);

	}
	else
	{
		((CButton *)GetDlgItem(IDC_SensorLSCItem))->ShowWindow(TRUE);
		((CButton *)GetDlgItem(IDC_LSCTarget))->ShowWindow(TRUE);
		((CButton *)GetDlgItem(IDC_SensorLSCGroup))->ShowWindow(TRUE);
		((CButton *)GetDlgItem(IDC_STATICLSC1))->ShowWindow(TRUE);
		((CButton *)GetDlgItem(IDC_STATICLSC2))->ShowWindow(TRUE);
	}
}

void OptionDlg::OnCbnSelchangeSensorlscitem()
{
	// TODO: 在此添加控件通知处理程序代码
	m_sensoritem = m_SensorList.GetCurSel();
}


void OptionDlg::OnCbnSelchangeCfaitem()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cfaitem = m_cfaList.GetCurSel();
}


void OptionDlg::OnCbnSelchangeQuallscmode()
{
	// TODO: 在此添加控件通知处理程序代码
	m_quallscitem = m_quallsclist.GetCurSel();
}
