#pragma once


// OptionDlg 对话框
#include "resource.h"

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

	DECLARE_MESSAGE_MAP()

public:
	void AddTestList(void);
	OTP_Inspection* m_pInspection;
	UINT m_sleeptime;
	UINT m_exp_tolerance;
	UINT m_exp_target;
	BOOL m_ExposureEn;	

	UINT m_exp_pdaftarget;
	BOOL m_PDAFExposureEn;	
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

	UINT m_InfH;
	UINT m_InfL;
	UINT m_MacH;
	UINT m_MacL;
	UINT m_StcH;
	UINT m_StcL;
	UINT m_AFDif;

	UINT m_grouplimit;
	CString m_MTK_INI_Name;
	CString m_Qual_INI_Name;

	CComboBox m_ShadingList;
	int m_shadingitem;
	double m_ymax;
	double m_ymin;
	double m_ydiff;
	double m_rgdiff;
	double m_bgdiff;

	BOOL m_goldengain;

	BOOL m_LSCItem;
	CComboBox m_cfaList;
	int m_cfaitem;
	CComboBox m_quallsclist;
	int m_quallscitem;
	BOOL m_qualawb;

	int m_lscTarget;
	int m_lscGroup;
	CComboBox m_SensorList;
	int m_sensoritem;

public:

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
};
