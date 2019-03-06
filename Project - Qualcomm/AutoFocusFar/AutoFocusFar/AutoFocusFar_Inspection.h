#pragma once

#include "DccmBase.h"
#include "OptionDlg.h"
#include "Inspection.h"
#include "AFCurve.h"
#include <deque>

#define AF_EndFlag 100
#define MTF_ROI		5

struct ROI_Info
{
	CRect pos;
	int cx;
	int cy;
	double MTFvalue;
	double HVratio;
	deque<double> MTF_Deque;
	int TestResult;
	int limit1;
	int limit2;
	BOOL Enable;

};

struct _MTF
{
	ROI_Info ROI[MTF_ROI];
	int ROI_height;
	int ROI_width;
	double SurroundDiff;	// surround diff
	int surroundDiffLimit;	// MTF差异(不含中心)
	int HVratioLimit;		// 水平垂直解像力比
	int DequeSize;
};

class AutoFocusFar_Inspection :public Inspection
{
public:

	AutoFocusFar_Inspection(DccmControl_Interface* pInterface,LPCTSTR lpszName);
	~AutoFocusFar_Inspection(void);

	int Initialize();    // 初始化代码放在这里
	int Testing();       // 测试代码放在这里
	int Finalize();      // 结束代码放在这里

	int LoadOption();
	int SaveOption();

	LPCTSTR GetReportHeaders();
	LPCTSTR GetReportLowerLimits();
	LPCTSTR GetReportUpperLimits();
	LPCTSTR GetReportContents();

private:
	OptionDlg* pDlg;

public:
	CString m_szStr;
	int  FramePitch;
	BOOL m_bOneTimeLock;
	AFCurve m_Curve[5];
	double val[5];
	SearchParam  m_SearchParam[4];
	_MTF sMTF;

	CDC* m_pMemDC_Assistant; 
	CDC* m_pMemDC_Display;
	vector<int>* m_pvStorage;

	int m_curSearchStep;
	int m_curStepCode;
	int m_direction;
	int m_AF_Status;
	int m_maxAxisY;
	int m_maxAxisX;
	int m_bestAF_Code;
	double m_bestAF_Val;
	int AutoFocusFarTest();
	int ShowCurve(vector<FocusNode>* Array, int penClr);
	void ShowFocusRect(int index);
	double GetROIValues(int Alg_type, CRect roiRect);
	int DrawBackGround();
	int OnKeyCtrlNum();
	BOOL AF_EndResolutionAndCodeCheck(void);// 判断最清晰解像力是否达标，同时code范围在标准之内
	int  AF_BestCodeCheck(int index);	// 判断EndCode是否是最佳Code
	BOOL AF_CheckSurroundDiff();
	void ClearStorage(BOOL bClear);

	void CalcImageRect(void);

	int BackToInitPos(void);
	BOOL AF_CheckGlue(int afCode);		// 预判粘胶产品
	double m_maxMTF_Center;				// 中心MTF最大值
	double m_minMTF_Center;				// 中心MTF最小值
	BOOL m_bOneTimeLog;					// 只输出一次的log
};

