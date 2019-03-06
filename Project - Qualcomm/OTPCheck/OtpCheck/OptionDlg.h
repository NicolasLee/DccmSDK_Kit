#pragma once

// OptionDlg 对话框
#include "resource.h"
#include "afxwin.h"

#include "OtpCheckBase.h"

class OtpCheck_Inspection;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, OtpCheck_Inspection* pInspection);   // 标准构造函数
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
	afx_msg void OnCbnSelchangeComboProjName();
	DECLARE_MESSAGE_MAP()

public:
	OtpCheck_Inspection* m_pInspection;

	CComboBox m_comboProjName;
	eProjectName m_projName;
	int CurTestType;	// 测试项目测试类型
	BOOL m_OtpType;//0 Sensor烧录，1 Eeprom烧录

	BOOL m_bCompareAF;
	BOOL m_bCheckStc;
	BOOL m_bCheckInf;
	BOOL m_bCheckMac;	
	BOOL m_bCheckMid;

	UINT m_bAFCompareLimit;
	UINT m_stcLower;
	UINT m_stcUpper;
	UINT m_infLower;
	UINT m_infUpper;
	UINT m_macUpper;	
	UINT m_macLower;	
	UINT m_midUpper;
	UINT m_midLower;

	BOOL m_bCheckRG;
	BOOL m_bCheckBG;
	BOOL m_bCheckGG;
	BOOL m_bCheckR;
	BOOL m_bCheckGr;
	BOOL m_bCheckGb;
	BOOL m_bCheckB;

	UINT m_RGLower;
	UINT m_RGUpper;
	UINT m_BGLower;
	UINT m_BGUpper;
	UINT m_GGLower;
	UINT m_GGUpper;
	UINT m_RLower;
	UINT m_RUpper;
	UINT m_GrLower;
	UINT m_GrUpper;
	UINT m_GbLower;
	UINT m_GbUpper;
	UINT m_BLower;
	UINT m_BUpper;

	BOOL m_bCheckGoldRG;
	BOOL m_bCheckGoldBG;
	BOOL m_bCheckGoldGG;
	BOOL m_bCheckGoldR;
	BOOL m_bCheckGoldGr;
	BOOL m_bCheckGoldGb;
	BOOL m_bCheckGoldB;

	UINT m_GoldRG;
	UINT m_GoldBG;
	UINT m_GoldGG;
	UINT m_GoldR;
	UINT m_GoldGr;
	UINT m_GoldGb;
	UINT m_GoldB;

	BOOL m_bCheckFlag_ModuleInfo;
	BOOL m_bCheckFlag_AWB;
	BOOL m_bCheckFlag_LSC;
	BOOL m_bCheckFlag_AF;
	BOOL m_bCheckFlag_PDAF;

	BOOL m_bCheckSum_ModuleInfo;
	BOOL m_bCheckSum_AWB;
	BOOL m_bCheckSum_LSC;
	BOOL m_bCheckSum_AF;
	BOOL m_bCheckSum_PDAF;

	afx_msg void OnBnClickedSaveOTPData();
	afx_msg void OnBnClickedCheckCompareaf();
	afx_msg void OnBnClickedCheckStc();
	afx_msg void OnBnClickedCheckInf();
	afx_msg void OnBnClickedCheckMac();
	afx_msg void OnBnClickedCheckMid();
	afx_msg void OnBnClickedCheckGoldrg();
	afx_msg void OnBnClickedCheckGoldbg();
	afx_msg void OnBnClickedCheckGoldr();
	afx_msg void OnBnClickedCheckGoldgr();
	afx_msg void OnBnClickedCheckGoldgb();
	afx_msg void OnBnClickedCheckGoldb();
	afx_msg void OnBnClickedCheckRg();
	afx_msg void OnBnClickedCheckBg();
	afx_msg void OnBnClickedCheckR();
	afx_msg void OnBnClickedCheckGr();
	afx_msg void OnBnClickedCheckGb();
	afx_msg void OnBnClickedCheckB();
	afx_msg void OnCbnSelchangeComboSetType();
};
