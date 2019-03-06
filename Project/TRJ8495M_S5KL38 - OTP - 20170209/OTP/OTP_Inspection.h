#pragma once

#include "DccmBase.h"
#include "OptionDlg.h"
#include "Inspection.h"


#include "OTP\OtpBase.h"
#include "otp\TYD8470M\OtpS5K3L8.h"


class OTP_Inspection :public Inspection
{
public:
	OTP_Inspection(DccmControl_Interface* pInterface, LPCTSTR lpszName);
	~OTP_Inspection(void);

	int Initialize();    // 测试前 处理代码
	void InitItem();
	void InitShadingItem(); 
	void InitSensorItem(); 
	void InitCFAItem();
	void InitQualLSCItem();
	int Testing();       // 测试时 处理代码
	BOOL PDAFExposure();
	BOOL NormalExposure();
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
	ShadingCalc* m_pShading;
	COtpBase* m_pOtp;
	CString info;
	int step;
	int pitch;
	BOOL m_Lock1;
	BOOL m_Lock2;
	BOOL m_pdafExpLock;
	BOOL m_normalExpLock;
	BOOL TestShading();
	void MatchProject();
};
