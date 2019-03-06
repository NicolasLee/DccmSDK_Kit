#pragma once


// OptionDlg 对话框
#include "resource.h"

class PD_CALI_Qual_2_Inspection;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, PD_CALI_Qual_2_Inspection* pInspection);   // 标准构造函数
	virtual ~OptionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	PD_CALI_Qual_2_Inspection* m_pInspection;
	DECLARE_MESSAGE_MAP()
public:
	int		m_Exp_Tolerance;
	int		m_Exp_Target;
	BOOL	m_ExposureEn;
	BOOL  m_QVLEn;
	BOOL  m_negvalue;

	CString m_szDEV_ADDR;


	CString m_szReg_ADDR;
	CString m_szInf_Msb_ADDR;
	CString m_szInf_Lsb_ADDR;
	CString m_szMac_Msb_ADDR;
	CString m_szMac_Lsb_ADDR;

	UINT m_InfH;
	UINT m_InfL;
	UINT m_MacH;
	UINT m_MacL;
	UINT m_MidH;
	UINT m_MidL;
	UINT m_AFDif;

	INT m_KH;
	INT m_KL;

	BOOL	m_bSaveRaw;
	BOOL  m_berase;
};
