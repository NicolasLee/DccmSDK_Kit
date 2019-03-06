#pragma once
#include "resource.h"
// OptionDlg 对话框
#include "DccmBase.h"

class AutoFocusFar_Inspection;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, AutoFocusFar_Inspection* pInspection);   // 标准构造函数
	virtual ~OptionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnKeyCtrlNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedFrameOffsetEnable();
	afx_msg void OnBnClickedCheckResolution();
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();

	AutoFocusFar_Inspection  *pInspection;

public:
	BOOL PosPrompt;
	BOOL ShowOriginalImg;
	BOOL ResolutionCheck;
	BOOL SaveAF_Curve;
	BOOL BackToInit;
	BOOL CheckGlue;
	BOOL ClearRecord;

	// 搜索最清晰区间
	struct sSearchBestArea
	{
		BOOL Enable;
		int StartCode;
		int EndCode;
		int StepCode;
	} SearchArea;

	int StartCode;

	BOOL ExpEnable;
	int ExpTolerance;
	int ExpTarget;

	int AlgorithmType;
	int MAX_Y;
	int MAX_X;

	int FramePitchLimit;
	int SearchTime;	// 次数

	int StepCode1;
	int StepCode2;
	int StepCode3;
	int StepCode4;

	double StepDiff1;
	double StepDiff2;
	double StepDiff3;
	double StepDiff4;

	int DescendTime1;
	int DescendTime2;
	int DescendTime3;
	int DescendTime4;

	int CodeLimitL;
	int CodeLimitH;
	BOOL CheckCornerDiff;
};
