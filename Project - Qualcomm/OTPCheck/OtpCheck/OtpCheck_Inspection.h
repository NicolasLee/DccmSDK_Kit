#pragma once

#include "DccmBase.h"
#include "OptionDlg.h"
#include "Inspection.h"

#include "OtpCheckBase.h"
#include "OtpCheck_TKU8615M.h"
#include "OtpCheck_TRJ8564.h"
#include "OtpCheck_HLT7038.h"
#include "OtpCheck_HLT7178.h"
#include "OtpCheck_HLT7215.h"
#include "OtpCheck_TYD8592M.h"
#include "OtpCheck_TYD8583M.h"
#include "OtpCheck_HLT7014.h"
#include "OtpCheck_FYD8853M.h"
#include "OtpCheck_CMK8440.h"
#include "OtpCheck_SAA30L2K.h"
#include "OtpCheck_CB801C.h"

class OtpCheck_Inspection :public Inspection //OtpCheck_Inspection里的文件除非必要，尽量不做改动。
{
public:
	OtpCheck_Inspection(DccmControl_Interface* pInterface, LPCTSTR lpszName);
	~OtpCheck_Inspection(void);

	int Testing();       // 测试时 处理代码
	int Initialize();    // 测试前 处理代码
	int Finalize();      // 测试后 处理代码

	int LoadOption();	 // 加载配置文件参数
	int SaveOption();	 // 保存配置文件参数

	
	LPCTSTR GetReportHeaders();		// 本地保存数据：数据文件头
	LPCTSTR GetReportLowerLimits();	// 本地保存数据：数据标准下限
	LPCTSTR GetReportUpperLimits(); // 本地保存数据：数据标准上限
	LPCTSTR GetReportContents() ;	// 本地保存数据：数据内容

private:
	OptionDlg* pDlg;

public:
	OtpCheckBase* m_pOtpCheck;
	int OtpCheck_Test();
	void UpdateOTPCheckUI();

	void MatchProjName(eProjectName projName);
};
