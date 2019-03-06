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
	
public:

	BOOL m_bCheckStc;
	BOOL m_bCheckInf;
	BOOL m_bCheckMac;	// 近景Code

	short m_stcLower;	// 
	short m_stcUpper;	//
	short m_infLower;	//
	short m_infUpper;	//
	short m_macUpper;	//
	short m_macLower;	//

	CComboBox m_comboProjName;
	eProjectName m_projName;

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
	afx_msg void OnBnClickedButton();
};
