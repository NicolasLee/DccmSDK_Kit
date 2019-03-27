#pragma once


// OptionDlg 对话框
#include "resource.h"
#include "OtpBase.h"
#include "Page1.h"
#include "Page2.h"
#include "Page3.h"
#include "Page4.h"
#include "Page5.h"
#include "Page6.h"
#include "Page7.h"


class OTP_Inspection;



class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, OTP_Inspection* pInspection);   // 标准构造函数
	virtual ~OptionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();

	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnCbnSelchangeComboProjName();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeGoldengr();
	afx_msg void OnEnChangeGoldenr();
	afx_msg void OnEnChangeGoldenb();
	afx_msg void OnEnChangeGoldengb();
	afx_msg void OnCbnSelchangeShadingItem();
	afx_msg void OnBnClickedGoldenchannel();
	afx_msg void OnBnClickedGoldengain();
	afx_msg void OnBnClickedEnlsc();
	afx_msg void OnBnClickedEnawb();
	afx_msg void OnBnClickedEnaf();
	afx_msg void OnBnClickedEnpdaf();
	afx_msg void OnBnClickedEnmtklsc();
	afx_msg void OnCbnSelchangeSensorlscitem();
	afx_msg void OnCbnSelchangeCfaitem();
	afx_msg void OnCbnSelchangeQuallscmode();
	afx_msg void OnCbnSelchangeMtkpdafversion();
	afx_msg void OnCbnSelchangeQualpdafversion();
	afx_msg void OnCbnSelchangeQuallscversion();
	afx_msg void OnBnClickedCheckExpouse();
	afx_msg void OnCbnSelchangeMtklscversion();
	afx_msg void OnBnClickedEnmtkpdaf();
	DECLARE_MESSAGE_MAP()

public:
	OTP_Inspection* m_pInspection;

	//通用烧录
	CTabCtrl m_tab;
	BOOL m_General;
	int m_Brunto;
	BYTE m_SlaveID;
	BOOL m_spage;
	int m_totalgroup;
	CPage1 m_page1;
	CPage2 m_page2;
	CPage3 m_page3;
	CPage4 m_page4;
	CPage5 m_page5;
	CPage6 m_page6;
	CPage7 m_page7;
	CDialog* pDialog[7];
	int m_CurSelTab;
	void OnInittab();

	UINT m_sleeptime;
	BOOL m_ExposureEn;	
	BOOL m_PDAFExposureEn;	
	UINT m_exp_tolerance;
	UINT m_exp_pdaftolerance;
	UINT m_exp_target;
	UINT m_exp_pdaftarget;
	UINT m_exp_size;
	UINT m_exp_pdafsize;

	BOOL m_EnBasic;
	BOOL m_EnLSC;
	BOOL m_EnAWB;
	BOOL m_EnAF;
	BOOL m_EnPDAF;
	BOOL m_EnQVL;
	BOOL m_PDAFItem;
	BOOL m_berase;
	BOOL m_bcheckinf;
	BOOL m_bcheckmac;
	BOOL m_bchecksta;
	BOOL m_dummyburn;
	BOOL m_twosetting;
	UINT m_grouplimit;

	//保存AWB应用前后数据
	BOOL m_SaveAWB;
	UINT m_awb_size;
	UINT m_goldenrg;
	UINT m_goldenbg;
	UINT m_goldengg;

	UINT m_rgainh;
	UINT m_rgainl;
	UINT m_bgainh;
	UINT m_bgainl;
	UINT m_goldenr;
	UINT m_goldenb;
	UINT m_goldengr;
	UINT m_goldengb;
	float m_rgaindif;
	float m_bgaindif;
	float m_orgdif;

	BOOL m_afread;
	UINT m_InfH;
	UINT m_InfL;
	UINT m_MacH;
	UINT m_MacL;
	UINT m_StcH;
	UINT m_StcL;
	UINT m_AFDif;

	CComboBox m_comboMtkPdafVerList;
	int m_MTKPDAFitem;
	CComboBox m_comboQualPdafVerList;
	int m_QUALPDAFitem;
	CString m_MTK_INI_Name;
	CString m_Qual_INI_Name;

	CComboBox m_comboShadingList;
	int m_shadingitem;
	double m_ymax;
	double m_ymin;
	double m_ydiff;
	double m_rgdiff;
	double m_bgdiff;
	double m_ymax2;
	double m_ymin2;

	BOOL m_goldengain;

	int m_LSCItem;
	BOOL m_SaveMTKraw;
	CComboBox m_comboMtkLscVerList;
	int m_MTKLSCVerItem;
	CComboBox m_comboQualLscVerList;
	int m_QUALLSCVerItem;
	CComboBox m_comboCfaList;
	int m_cfaitem;
	CComboBox m_comboQualLscList;
	int m_quallscitem;
	BOOL m_qualawb;

	int m_lscTarget;
	int m_lscGroup;
	CComboBox m_comboSensorList;
	int m_sensoritem;

	CComboBox m_comboProjName;
	eProjectName m_projName;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckGeneral();
	afx_msg void OnBnClickedEneeprom();

	CString GetFile();
	CString GetSensorConfigString(CString strSection, CString strKey, CString strFile, CString strDefault);
	int SetSensorConfigInt(CString strSection, CString strKey, int data, CString strFile);
	int SetSensorConfigString(CString strSection, CString strKey, CString strData, CString strFile);
};
