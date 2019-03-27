#pragma once

#include "DccmBase.h"
#include "OptionDlg.h"
#include "Inspection.h"
#include "OtpBase.h"

#include "project\TRJ8654M_S5K3L8\Otp_TRJ8564.h"
#include "Project\FHB6551M_S5K3P3\Otp_FHB6551M.h"
#include "Project\PLQ8579M_S5K5E8\Otp_PLQ8579M.h"
#include "Project\HLTE1_S5K4H8\Otp_HLTE1M.h"
#include "Project\HLT7017_S5K3L8\Otp_HLT7017.h"
#include "Project\TYD8635M_OV13855\Otp_TYD8635M.h"
#include "Project\TYD8583M_Hi1332\Otp_TYD8583M.h"
#include "Project\HLT7012_S5K3L8\Otp_HLT7012.h"
#include "Project\HLT7014_S5K3L8\Otp_HLT7014.h"
#include "Project\HLT7109_OV13855\Otp_HLT7109.h"
#include "Project\HLT7025_S5K3L8_MTK_NOPD\Otp_HLT7025.h"
#include "Project\SAA3_0L2K_S5K3L8XXM3_Qual\Otp_SAA3_0L2K.h"
#include "Project\CB801C_S5K4H7\Otp_CB801C.h"

class OTP_Inspection :public Inspection
{
public:
	OTP_Inspection(DccmControl_Interface* pInterface, LPCTSTR lpszName);
	~OTP_Inspection(void);

	int Initialize();    // 测试前 处理代码
	int Testing();       // 测试时 处理代码
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
	void InitShadingItem(); 
	void InitSensorItem(); 
	void InitCFAItem();
	void InitQualLSCItem();
	
	int Exposure_Interface(UINT size);//1/size
	int PDAFExposure_Interface(UINT size);//1/size
	BOOL PDAFExposure();
	BOOL NormalExposure();
	
	ShadingCalc* m_pShading;
	COtpBase* m_pOtp;
	CString info;
	int step;
	int pitch;
	BOOL m_Lock1;
	BOOL m_Lock2;
	BOOL m_pdafExpLock;
	BOOL m_normalExpLock;
	BOOL m_shadingflag;
	BOOL m_proc1explock;

	BOOL TestShading();
	void MatchProjName(eProjectName projName);
};
