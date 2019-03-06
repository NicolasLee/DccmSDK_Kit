#pragma once

#include "DccmBase.h"
#include "OptionDlg.h"
#include "Inspection.h"

#include "OtpCheckBase.h"
#include "OtpCheck_THY8348M.h"
#include "OtpCheck_TKU8341M.h"
#include "OtpCheck_EYD2467M.h"
#include "OtpCheck_TYD8470M.h"
#include "OtpCheck_HLT6014.h"
#include "OtpCheck_FHB6551M.h"
#include "OtpCheck_PLQ8579M.h"
#include "OtpCheck_TRJ8495M.h"

#define sensorOTP

class OtpCheck_Inspection :public Inspection
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

	void MatchProjName(eProjectName projName);
};
