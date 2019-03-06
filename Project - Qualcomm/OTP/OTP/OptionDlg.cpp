// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OTP.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "OTP_Inspection.h"

// OptionDlg 对话框
#define  MaxGroup 3
IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, OTP_Inspection* pInspection)
	: CDialog(OptionDlg::IDD, pParent)
	, m_projName(PN_NULL)
{
	m_goldengain = 0;
	m_LSCItem = 0;
	m_afread = 0;
	m_PDAFItem = 0;
	m_afread = 0;
	m_pInspection = pInspection;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_BASIC, m_EnBasic);
	DDX_Text(pDX, IDC_EDIT_SLEEP_TIME, m_sleeptime);
	DDV_MinMaxUInt(pDX, m_sleeptime, 0, 10);
	DDX_Text(pDX, IDC_SD_EXPTOLERANCE, m_exp_tolerance);
	DDV_MinMaxUInt(pDX, m_exp_tolerance, 0, 100);
	DDX_Text(pDX, IDC_SD_EXPTARGET, m_exp_target);
	DDV_MinMaxUInt(pDX, m_exp_target, 1, 255);
	DDX_Check(pDX, IDC_CHECK_EXPOUSE, m_ExposureEn);
	DDX_Text(pDX, IDC_SD_PDAFEXPTARGET, m_exp_pdaftarget);
	DDV_MinMaxUInt(pDX, m_exp_pdaftarget, 1, 255);
	DDX_Text(pDX, IDC_SD_PDAFEXPTOLERANCE, m_exp_pdaftolerance);
	DDV_MinMaxUInt(pDX, m_exp_pdaftolerance, 0, 100);
	DDX_Text(pDX, IDC_SD_EXPSIZE, m_exp_size);
	DDV_MinMaxUInt(pDX, m_exp_size, 1, 50);
	DDX_Text(pDX, IDC_SD_PDAFEXPSIZE, m_exp_pdafsize);
	DDV_MinMaxUInt(pDX, m_exp_pdafsize, 1, 50);
	DDX_Check(pDX, IDC_CHECK_PDAFEXPOUSE, m_PDAFExposureEn);
	DDX_Check(pDX, IDC_CHECK_LSC, m_EnLSC);
	DDX_Check(pDX, IDC_CHECK_AWB, m_EnAWB);
	DDX_Check(pDX, IDC_CHECK_AF, m_EnAF);
	DDX_Check(pDX, IDC_CHECK_PDAF, m_EnPDAF);
	DDX_Check(pDX, IDC_CHECK_QVL, m_EnQVL);
	DDX_Check(pDX, IDC_CHECK_ERASE, m_berase);      
	DDX_Check(pDX, IDC_CHECK_AF_INF, m_bcheckinf);
	DDX_Check(pDX, IDC_CHECK_AF_MAC, m_bcheckmac);
	DDX_Check(pDX, IDC_CHECK_AF_STA, m_bchecksta);
	DDX_Check(pDX, IDC_DummyBurn, m_dummyburn); 
	DDX_Check(pDX, IDC_TwoSettings,m_twosetting);

	DDX_Text(pDX, IDC_GoldenRG, m_goldenrg);
	DDX_Text(pDX, IDC_GoldenBG, m_goldenbg);
	DDX_Text(pDX, IDC_GoldenGG, m_goldengg);

	DDX_Text(pDX, IDC_AWB_SIZE, m_awb_size);
	DDV_MinMaxUInt(pDX, m_awb_size, 1, 50);
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

	DDX_Radio(pDX, IDC_AF_FOCUSREAD, m_afread); //0 focus 1 otp
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

	DDX_Control(pDX, IDC_ShadingItem, m_comboShadingList);
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
	DDX_Text(pDX, IDC_YMAX2, m_ymax2);
	DDV_MinMaxDouble(pDX, m_ymax2, 1, 100);	
	DDX_Text(pDX, IDC_YMIN2, m_ymin2);
	DDV_MinMaxDouble(pDX, m_ymin2, 1, 100);	

	DDX_Radio(pDX, IDC_GOLDENGAIN, m_goldengain); //0 gain 1 channel
	DDX_Radio(pDX, IDC_ENMTKLSC, m_LSCItem); // 0 MTK 1 Qual 2 Sensor
	DDX_Check(pDX,IDC_CHECK_MTK_RAW,m_SaveMTKraw);
	DDX_Control(pDX,IDC_MTKLSCVERSION,m_comboMtkLscVerList);
	DDX_Control(pDX,IDC_QUALLSCVERSION,m_comboQualLscVerList);
	DDX_Control(pDX,IDC_CFAItem,m_comboCfaList);
	DDX_Control(pDX,IDC_QUALLSCMODE,m_comboQualLscList);
	DDX_Check(pDX,IDC_CHECK_QUAL_AWB,m_qualawb);
	DDX_Control(pDX, IDC_SensorLSCItem, m_comboSensorList);
	DDX_Text(pDX, IDC_LSCTarget, m_lscTarget);
	DDX_Text(pDX, IDC_SensorLSCGroup, m_lscGroup);
	DDV_MinMaxUInt(pDX, m_lscGroup, 1, MaxGroup);

	DDX_Check(pDX, IDC_CHECK_SAVE_AWB, m_SaveAWB);

	DDX_Radio(pDX, IDC_ENMTKPDAF, m_PDAFItem); // 0 MTK 1 Qual 2 Sensor
	DDX_Control(pDX,IDC_MTKPDAFVERSION,m_comboMtkPdafVerList);
	DDX_Control(pDX,IDC_QUALPDAFVERSION,m_comboQualPdafVerList);

	DDX_Control(pDX, IDC_COMBO_PROJ_NAME, m_comboProjName);

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
	ON_BN_CLICKED(IDC_CHECK_LSC, &OptionDlg::OnBnClickedEnlsc)
	ON_BN_CLICKED(IDC_CHECK_AWB, &OptionDlg::OnBnClickedEnawb)
	ON_BN_CLICKED(IDC_CHECK_AF, &OptionDlg::OnBnClickedEnaf)
	ON_BN_CLICKED(IDC_CHECK_PDAF, &OptionDlg::OnBnClickedEnpdaf)
	ON_BN_CLICKED(IDC_ENSENSORLSC, &OptionDlg::OnBnClickedEnmtklsc)
	ON_BN_CLICKED(IDC_ENMTKLSC, &OptionDlg::OnBnClickedEnmtklsc)
	ON_BN_CLICKED(IDC_ENQUALLSC, &OptionDlg::OnBnClickedEnmtklsc)
	ON_BN_CLICKED(IDC_ENMTKPDAF, &OptionDlg::OnBnClickedEnmtkpdaf)
	ON_BN_CLICKED(IDC_ENQUALPDAF, &OptionDlg::OnBnClickedEnmtkpdaf)
	ON_BN_CLICKED(IDC_ENSENSORPDAF, &OptionDlg::OnBnClickedEnmtkpdaf)
	ON_CBN_SELCHANGE(IDC_SensorLSCItem, &OptionDlg::OnCbnSelchangeSensorlscitem)
	ON_CBN_SELCHANGE(IDC_CFAItem, &OptionDlg::OnCbnSelchangeCfaitem)
	ON_CBN_SELCHANGE(IDC_QUALLSCMODE, &OptionDlg::OnCbnSelchangeQuallscmode)
	ON_CBN_SELCHANGE(IDC_COMBO_PROJ_NAME, &OptionDlg::OnCbnSelchangeComboProjName)
	ON_CBN_SELCHANGE(IDC_MTKPDAFVERSION, &OptionDlg::OnCbnSelchangeMtkpdafversion)
	ON_CBN_SELCHANGE(IDC_QUALPDAFVERSION, &OptionDlg::OnCbnSelchangeQualpdafversion)
	ON_CBN_SELCHANGE(IDC_QUALLSCVERSION, &OptionDlg::OnCbnSelchangeQuallscversion)
	ON_CBN_SELCHANGE(IDC_MTKLSCVERSION, &OptionDlg::OnCbnSelchangeMtklscversion)
	ON_BN_CLICKED(IDC_ENMTKPDAF, &OptionDlg::OnBnClickedEnmtkpdaf)
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

	return 0;
}

// 响应模组关闭消息
LRESULT OptionDlg::OnCameraStop(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

BOOL OptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	///////////////////////////Project///////////////////////////////////////////////
	m_comboProjName.InsertString(0, L"FHB6551M");
	m_comboProjName.InsertString(1, L"PLQ8579M");
	m_comboProjName.InsertString(2, L"TRJ8564M");
	m_comboProjName.InsertString(3, L"HLTE1M");
	m_comboProjName.InsertString(4, L"HLT7017");
	m_comboProjName.InsertString(5, L"TYD8635M");
	m_comboProjName.InsertString(6, L"TYD8583M");
	m_comboProjName.InsertString(7, L"HLT7012");
	m_comboProjName.InsertString(8, L"HLT7014");
	m_comboProjName.InsertString(9, L"HLT7109");
	m_comboProjName.InsertString(10, L"HLT7025");
	m_comboProjName.InsertString(11, L"SAA3_0L2K"); // 华天慧创
	m_comboProjName.SetCurSel(m_projName);

	/////////////////////////////////shading////////////////////////////////////
	m_comboShadingList.ResetContent();
	m_comboShadingList.Clear();
	m_comboShadingList.InsertString(0, _T("Shading5"));
	m_comboShadingList.InsertString(1, _T("Shading14"));
	m_comboShadingList.InsertString(2, _T("Shading49"));
	m_comboShadingList.SetCurSel(m_shadingitem);

	///////////////////////////////sensor////////////////////////////////////////
	m_comboSensorList.ResetContent();
	m_comboSensorList.Clear();
	m_comboSensorList.InsertString(0, _T("S5K3L8"));
	m_comboSensorList.InsertString(1, _T("S5K5E8"));
	m_comboSensorList.InsertString(2, _T("S5K4H8"));
	m_comboSensorList.InsertString(3, _T("HI1332"));
	m_comboSensorList.SetCurSel(m_sensoritem);

	/////////////////////////////////////MTK LSC/////////////////////////////////////
	m_comboMtkLscVerList.ResetContent();
	m_comboMtkLscVerList.Clear();

	m_comboMtkLscVerList.InsertString(0, _T("14"));
	m_comboMtkLscVerList.InsertString(1, _T("15"));
	m_comboMtkLscVerList.InsertString(2, _T("16_RGB"));
	m_comboMtkLscVerList.InsertString(3, _T("16_MONO"));
	m_comboMtkLscVerList.SetCurSel(m_MTKLSCVerItem);

	//////////////////////////////////QUAL LSC//////////////////////////////////////
	m_comboQualLscVerList.ResetContent();
	m_comboQualLscVerList.Clear();
	m_comboQualLscVerList.InsertString(0, _T("P"));
	m_comboQualLscVerList.InsertString(1, _T("R"));
	m_comboQualLscVerList.SetCurSel(m_QUALLSCVerItem);


	m_comboCfaList.ResetContent();
	m_comboCfaList.Clear();
	m_comboCfaList.InsertString(0, _T("RGGB"));
	m_comboCfaList.InsertString(1, _T("GRBG"));
	m_comboCfaList.InsertString(2, _T("BGGR"));
	m_comboCfaList.InsertString(3, _T("GBRG"));

	m_comboCfaList.SetCurSel(m_cfaitem);

	//lsc mode
	m_comboQualLscList.ResetContent();
	m_comboQualLscList.Clear();

	//pls add cfa item here;
	m_comboQualLscList.InsertString(0, _T("17x13"));
	m_comboQualLscList.InsertString(1, _T("9x7"));

	m_comboQualLscList.SetCurSel(m_quallscitem);
	///////////////////////////////MTK PDAF Ver////////////////////////////////////////
	m_comboMtkPdafVerList.ResetContent();
	m_comboMtkPdafVerList.Clear();

	//pls add MTK VER item here;
	//	m_comboMtkPdafVerList.InsertString(0,_T("1.3.0.529"));
	m_comboMtkPdafVerList.InsertString(0, _T("2.0.1.1023"));
	m_comboMtkPdafVerList.InsertString(1, _T("3.0.0.0525"));
	m_comboMtkPdafVerList.InsertString(2, _T("3.0.1.0322"));
	m_comboMtkPdafVerList.SetCurSel(m_MTKPDAFitem);

	///////////////////////////////QUAL PDAF Ver////////////////////////////////////////
	m_comboQualPdafVerList.ResetContent();
	m_comboQualPdafVerList.Clear();
	m_comboQualPdafVerList.InsertString(0, _T("J"));
	m_comboQualPdafVerList.InsertString(1, _T("L3"));
	m_comboQualPdafVerList.InsertString(2, _T("L4"));
	m_comboQualPdafVerList.InsertString(3, _T("L5"));
	m_comboQualPdafVerList.InsertString(4, _T("L6"));
	m_comboQualPdafVerList.SetCurSel(m_QUALPDAFitem);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void OptionDlg::OnEnChangeGoldengr()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UINT AWB_FACTOR = 1024;
	m_goldenrg		= (UINT)(1.0 * (m_goldenr) / (m_goldengr)*AWB_FACTOR + 0.5); 
	m_goldenbg		= (UINT)(1.0 * (m_goldenb) / (m_goldengb)*AWB_FACTOR + 0.5);
	m_goldengg		= (UINT)(1.0 * (m_goldengb) / (m_goldengr)*AWB_FACTOR + 0.5);
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
	UINT AWB_FACTOR = 1024;
	m_goldenrg		= (UINT)(1.0 * (m_goldenr) / (m_goldengr)*AWB_FACTOR + 0.5); 
	m_goldenbg		= (UINT)(1.0 * (m_goldenb) / (m_goldengb)*AWB_FACTOR + 0.5);
	m_goldengg		= (UINT)(1.0 * (m_goldengb) / (m_goldengr)*AWB_FACTOR + 0.5);
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
	UINT AWB_FACTOR = 1024;
	m_goldenrg		= (UINT)(1.0 * (m_goldenr) / (m_goldengr)*AWB_FACTOR + 0.5); 
	m_goldenbg		= (UINT)(1.0 * (m_goldenb) / (m_goldengb)*AWB_FACTOR + 0.5);
	m_goldengg		= (UINT)(1.0 * (m_goldengb) / (m_goldengr)*AWB_FACTOR + 0.5);
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
	UINT AWB_FACTOR = 1024;
	m_goldenrg		= (UINT)(1.0 * (m_goldenr) / (m_goldengr)*AWB_FACTOR + 0.5); 
	m_goldenbg		= (UINT)(1.0 * (m_goldenb) / (m_goldengb)*AWB_FACTOR + 0.5);
	m_goldengg		= (UINT)(1.0 * (m_goldengb) / (m_goldengr)*AWB_FACTOR + 0.5);

	UpdateData(FALSE);
}

void OptionDlg::OnCbnSelchangeShadingItem()
{
	// TODO: 在此添加控件通知处理程序代码
	m_shadingitem = m_comboShadingList.GetCurSel();
}

void OptionDlg::OnBnClickedGoldenchannel()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton*)GetDlgItem(IDC_GOLDENCHANNEL))->GetCheck();
	if (buttonstatus)
	{
		GetDlgItem(IDC_GoldenRG)->EnableWindow(FALSE);
		GetDlgItem(IDC_GoldenBG)->EnableWindow(FALSE);
		GetDlgItem(IDC_GoldenGG)->EnableWindow(FALSE);

		GetDlgItem(IDC_GoldenR)->EnableWindow(TRUE);
		GetDlgItem(IDC_GoldenB)->EnableWindow(TRUE);
		GetDlgItem(IDC_GoldenGr)->EnableWindow(TRUE);
		GetDlgItem(IDC_GoldenGb)->EnableWindow(TRUE);
	}
}


void OptionDlg::OnBnClickedGoldengain()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton*)GetDlgItem(IDC_GOLDENGAIN))->GetCheck();
	if (buttonstatus)
	{
		GetDlgItem(IDC_GoldenRG)->EnableWindow(TRUE);
		GetDlgItem(IDC_GoldenBG)->EnableWindow(TRUE);
		GetDlgItem(IDC_GoldenGG)->EnableWindow(TRUE);

		GetDlgItem(IDC_GoldenR)->EnableWindow(FALSE);
		GetDlgItem(IDC_GoldenB)->EnableWindow(FALSE);
		GetDlgItem(IDC_GoldenGr)->EnableWindow(FALSE);
		GetDlgItem(IDC_GoldenGb)->EnableWindow(FALSE);
	}
}

void OptionDlg::OnBnClickedEnlsc()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton*)GetDlgItem(IDC_CHECK_LSC))->GetCheck();
	if (!buttonstatus)
	{
		GetDlgItem(IDC_ENMTKLSC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_MTK_RAW)->EnableWindow(FALSE);
		GetDlgItem(IDC_ENQUALLSC)->EnableWindow(FALSE);
		GetDlgItem(IDC_ENSENSORLSC)->EnableWindow(FALSE);
		GetDlgItem(IDC_SensorLSCItem)->EnableWindow(FALSE);
		GetDlgItem(IDC_LSCTarget)->EnableWindow(FALSE);
		GetDlgItem(IDC_SensorLSCGroup)->EnableWindow(FALSE);
		GetDlgItem(IDC_CFAItem)->EnableWindow(FALSE);
		GetDlgItem(IDC_QUALLSCMODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_QUALLSCVERSION)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_QUAL_AWB)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_ENMTKLSC)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_MTK_RAW)->EnableWindow(TRUE);
		GetDlgItem(IDC_ENQUALLSC)->EnableWindow(TRUE);
		GetDlgItem(IDC_ENSENSORLSC)->EnableWindow(TRUE);
		GetDlgItem(IDC_SensorLSCItem)->EnableWindow(TRUE);
		GetDlgItem(IDC_LSCTarget)->EnableWindow(TRUE);
		GetDlgItem(IDC_SensorLSCGroup)->EnableWindow(TRUE);
		GetDlgItem(IDC_CFAItem)->EnableWindow(TRUE);
		GetDlgItem(IDC_QUALLSCMODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_QUALLSCVERSION)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_QUAL_AWB)->EnableWindow(TRUE);
	}
}


void OptionDlg::OnBnClickedEnawb()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton*)GetDlgItem(IDC_CHECK_AWB))->GetCheck();
	if (!buttonstatus)
	{
		GetDlgItem(IDC_RGainH)->EnableWindow(FALSE);
		GetDlgItem(IDC_RGainL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BGainH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BGainL)->EnableWindow(FALSE);
		GetDlgItem(IDC_RGainDif)->EnableWindow(FALSE);
		GetDlgItem(IDC_BGainDif)->EnableWindow(FALSE);
		GetDlgItem(IDC_OrgDif)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SAVE_AWB)->EnableWindow(FALSE);
		GetDlgItem(IDC_AWB_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_GOLDENCHANNEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_GOLDENGAIN)->EnableWindow(FALSE);


		GetDlgItem(IDC_GoldenRG)->EnableWindow(FALSE);
		GetDlgItem(IDC_GoldenBG)->EnableWindow(FALSE);
		GetDlgItem(IDC_GoldenGG)->EnableWindow(FALSE);
		GetDlgItem(IDC_GoldenR)->EnableWindow(FALSE);
		GetDlgItem(IDC_GoldenB)->EnableWindow(FALSE);
		GetDlgItem(IDC_GoldenGr)->EnableWindow(FALSE);
		GetDlgItem(IDC_GoldenGb)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_RGainH)->EnableWindow(TRUE);
		GetDlgItem(IDC_RGainL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BGainH)->EnableWindow(TRUE);
		GetDlgItem(IDC_BGainL)->EnableWindow(TRUE);
		GetDlgItem(IDC_RGainDif)->EnableWindow(TRUE);
		GetDlgItem(IDC_BGainDif)->EnableWindow(TRUE);
		GetDlgItem(IDC_OrgDif)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SAVE_AWB)->EnableWindow(TRUE);
		GetDlgItem(IDC_AWB_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_GOLDENCHANNEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_GOLDENGAIN)->EnableWindow(TRUE);

		int buttonstatus = ((CButton*)GetDlgItem(IDC_GOLDENCHANNEL))->GetCheck();
		if (buttonstatus)
		{
			GetDlgItem(IDC_GoldenRG)->EnableWindow(FALSE);
			GetDlgItem(IDC_GoldenBG)->EnableWindow(FALSE);
			GetDlgItem(IDC_GoldenGG)->EnableWindow(FALSE);

			GetDlgItem(IDC_GoldenR)->EnableWindow(TRUE);
			GetDlgItem(IDC_GoldenB)->EnableWindow(TRUE);
			GetDlgItem(IDC_GoldenGr)->EnableWindow(TRUE);
			GetDlgItem(IDC_GoldenGb)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_GoldenRG)->EnableWindow(TRUE);
			GetDlgItem(IDC_GoldenBG)->EnableWindow(TRUE);
			GetDlgItem(IDC_GoldenGG)->EnableWindow(TRUE);

			GetDlgItem(IDC_GoldenR)->EnableWindow(FALSE);
			GetDlgItem(IDC_GoldenB)->EnableWindow(FALSE);
			GetDlgItem(IDC_GoldenGr)->EnableWindow(FALSE);
			GetDlgItem(IDC_GoldenGb)->EnableWindow(FALSE);
		}
	}
}


void OptionDlg::OnBnClickedEnaf()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton*)GetDlgItem(IDC_CHECK_AF))->GetCheck();
	if (!buttonstatus)
	{
		GetDlgItem(IDC_CHECK_AF_INF)->EnableWindow(FALSE);
		GetDlgItem(IDC_AF_FOCUSREAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_AF_OTPREAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_AF_MAC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_AF_STA)->EnableWindow(FALSE);
		GetDlgItem(IDC_InfH)->EnableWindow(FALSE);
		GetDlgItem(IDC_InfL)->EnableWindow(FALSE);
		GetDlgItem(IDC_MacH)->EnableWindow(FALSE);
		GetDlgItem(IDC_MacL)->EnableWindow(FALSE);
		GetDlgItem(IDC_StcH)->EnableWindow(FALSE);
		GetDlgItem(IDC_StcL)->EnableWindow(FALSE);
		GetDlgItem(IDC_AFDif)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_AF_INF)->EnableWindow(TRUE);
		GetDlgItem(IDC_AF_FOCUSREAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_AF_OTPREAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_AF_MAC)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_AF_STA)->EnableWindow(TRUE);
		GetDlgItem(IDC_InfH)->EnableWindow(TRUE);
		GetDlgItem(IDC_InfL)->EnableWindow(TRUE);
		GetDlgItem(IDC_MacH)->EnableWindow(TRUE);
		GetDlgItem(IDC_MacL)->EnableWindow(TRUE);
		GetDlgItem(IDC_StcH)->EnableWindow(TRUE);
		GetDlgItem(IDC_StcL)->EnableWindow(TRUE);
		GetDlgItem(IDC_AFDif)->EnableWindow(TRUE);
	}
}


void OptionDlg::OnBnClickedEnpdaf()
{
	// TODO: 在此添加控件通知处理程序代码
	int buttonstatus = ((CButton*)GetDlgItem(IDC_CHECK_PDAF))->GetCheck();
	if (!buttonstatus)
	{
		GetDlgItem(IDC_ENMTKPDAF)->EnableWindow(FALSE);
		GetDlgItem(IDC_MTKPDAFVERSION)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_QVL)->EnableWindow(FALSE);
		GetDlgItem(IDC_MTK_INI_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_ENQUALPDAF)->EnableWindow(FALSE);
		GetDlgItem(IDC_QUALPDAFVERSION)->EnableWindow(FALSE);
		GetDlgItem(IDC_Qual_INI_NAME)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_ENMTKPDAF)->EnableWindow(TRUE);
		GetDlgItem(IDC_MTKPDAFVERSION)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_QVL)->EnableWindow(TRUE);
		GetDlgItem(IDC_MTK_INI_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_ENQUALPDAF)->EnableWindow(TRUE);
		GetDlgItem(IDC_QUALPDAFVERSION)->EnableWindow(TRUE);
		GetDlgItem(IDC_Qual_INI_NAME)->EnableWindow(TRUE);
	}
}

void OptionDlg::OnBnClickedEnmtklsc()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	switch(m_LSCItem)
	{
	case 0:
 		GetDlgItem(IDC_CHECK_MTK_RAW)->EnableWindow(TRUE);
		GetDlgItem(IDC_MTKLSCVERSION)->EnableWindow(TRUE);

		GetDlgItem(IDC_CHECK_QUAL_AWB)->EnableWindow(FALSE);
		GetDlgItem(IDC_QUALLSCVERSION)->EnableWindow(FALSE);
		GetDlgItem(IDC_CFAItem)->EnableWindow(FALSE);
		GetDlgItem(IDC_QUALLSCMODE)->EnableWindow(FALSE);

		GetDlgItem(IDC_SensorLSCItem)->EnableWindow(FALSE);
		GetDlgItem(IDC_LSCTarget)->EnableWindow(FALSE);
		GetDlgItem(IDC_SensorLSCGroup)->EnableWindow(FALSE);
		break;
	case 1:
		GetDlgItem(IDC_CHECK_MTK_RAW)->EnableWindow(FALSE);
		GetDlgItem(IDC_MTKLSCVERSION)->EnableWindow(FALSE);

		GetDlgItem(IDC_CHECK_QUAL_AWB)->EnableWindow(TRUE);
		GetDlgItem(IDC_QUALLSCVERSION)->EnableWindow(TRUE);
		GetDlgItem(IDC_CFAItem)->EnableWindow(TRUE);
		GetDlgItem(IDC_QUALLSCMODE)->EnableWindow(TRUE);

		GetDlgItem(IDC_SensorLSCItem)->EnableWindow(FALSE);
		GetDlgItem(IDC_LSCTarget)->EnableWindow(FALSE);
		GetDlgItem(IDC_SensorLSCGroup)->EnableWindow(FALSE);
		break;
	case 2:
		GetDlgItem(IDC_CHECK_MTK_RAW)->EnableWindow(FALSE);
		GetDlgItem(IDC_MTKLSCVERSION)->EnableWindow(FALSE);

		GetDlgItem(IDC_CHECK_QUAL_AWB)->EnableWindow(FALSE);
		GetDlgItem(IDC_QUALLSCVERSION)->EnableWindow(FALSE);
		GetDlgItem(IDC_CFAItem)->EnableWindow(FALSE);
		GetDlgItem(IDC_QUALLSCMODE)->EnableWindow(FALSE);

		GetDlgItem(IDC_SensorLSCItem)->EnableWindow(TRUE);
		GetDlgItem(IDC_LSCTarget)->EnableWindow(TRUE);
		GetDlgItem(IDC_SensorLSCGroup)->EnableWindow(TRUE);
		break;
	default:

		break;
	}
}

void OptionDlg::OnCbnSelchangeSensorlscitem()
{
	// TODO: 在此添加控件通知处理程序代码
	m_sensoritem = m_comboSensorList.GetCurSel();
}


void OptionDlg::OnCbnSelchangeCfaitem()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cfaitem = m_comboCfaList.GetCurSel();
}


void OptionDlg::OnCbnSelchangeQuallscmode()
{
	// TODO: 在此添加控件通知处理程序代码
	m_quallscitem = m_comboQualLscList.GetCurSel();
}

void OptionDlg::OnCbnSelchangeComboProjName()
{
	// TODO: 在此添加控件通知处理程序代码
	m_projName = (eProjectName)m_comboProjName.GetCurSel();
	switch(m_projName)
	{
	case PN_FHB6551M:
		break;
	case PN_PLQ8579M:
		break;
	case PN_TRJ8564M:
		break;
	case PN_HLTE1M:
		break;
	case PN_HLT7017:
		break;
	case PN_TYD8635M:
		 break;
	case PN_TYD8583M:
		break;
	case PN_HLT7012:
		break;
	case PN_HLT7014:
		break;
	case PN_HLT7109:
		break;
	case PN_HLT7025:
		break;
	default:
		break;
	}
}

void OptionDlg::OnCbnSelchangeMtkpdafversion()
{
	// TODO: 在此添加控件通知处理程序代码
	m_MTKPDAFitem = m_comboMtkPdafVerList.GetCurSel();
}


void OptionDlg::OnCbnSelchangeQualpdafversion()
{
	// TODO: 在此添加控件通知处理程序代码
	m_QUALPDAFitem = m_comboQualPdafVerList.GetCurSel();
}


void OptionDlg::OnCbnSelchangeQuallscversion()
{
	// TODO: 在此添加控件通知处理程序代码
	m_QUALLSCVerItem = m_comboQualLscVerList.GetCurSel();
}


void OptionDlg::OnCbnSelchangeMtklscversion()
{
	// TODO: 在此添加控件通知处理程序代码
	m_MTKLSCVerItem = m_comboMtkLscVerList.GetCurSel();
}

void OptionDlg::OnBnClickedEnmtkpdaf()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	switch (m_PDAFItem)
	{
	case 0:
		GetDlgItem(IDC_MTKPDAFVERSION)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_QVL)->EnableWindow(TRUE);
		GetDlgItem(IDC_MTK_INI_NAME)->EnableWindow(TRUE);

		GetDlgItem(IDC_QUALPDAFVERSION)->EnableWindow(FALSE);
		GetDlgItem(IDC_Qual_INI_NAME)->EnableWindow(FALSE);

		GetDlgItem(IDC_SensorPDAFItem)->EnableWindow(FALSE);
		break;
	case 1:
		GetDlgItem(IDC_MTKPDAFVERSION)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_QVL)->EnableWindow(FALSE);
		GetDlgItem(IDC_MTK_INI_NAME)->EnableWindow(FALSE);

		GetDlgItem(IDC_QUALPDAFVERSION)->EnableWindow(TRUE);
		GetDlgItem(IDC_Qual_INI_NAME)->EnableWindow(TRUE);

		GetDlgItem(IDC_SensorPDAFItem)->EnableWindow(FALSE);
		break;
	case 2:
		GetDlgItem(IDC_MTKPDAFVERSION)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_QVL)->EnableWindow(FALSE);
		GetDlgItem(IDC_MTK_INI_NAME)->EnableWindow(FALSE);

		GetDlgItem(IDC_QUALPDAFVERSION)->EnableWindow(FALSE);
		GetDlgItem(IDC_Qual_INI_NAME)->EnableWindow(FALSE);

		GetDlgItem(IDC_SensorPDAFItem)->EnableWindow(TRUE);
		break;
	default:

		break;
	}
}
