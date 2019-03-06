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

	int Initialize();    // ����ǰ �������
	void InitItem();
	void InitShadingItem(); 
	void InitSensorItem(); 
	void InitCFAItem();
	void InitQualLSCItem();
	int Testing();       // ����ʱ �������
	BOOL PDAFExposure();
	BOOL NormalExposure();
	int Finalize();      // ���Ժ� �������

	int LoadOption();	 // ���������ļ�����
	int SaveOption();	 // ���������ļ�����

	LPCTSTR GetReportHeaders();		// ���ر������ݣ������ļ�ͷ
	LPCTSTR GetReportLowerLimits();	// ���ر������ݣ����ݱ�׼����
	LPCTSTR GetReportUpperLimits(); // ���ر������ݣ����ݱ�׼����
	LPCTSTR GetReportContents() ;	// ���ر������ݣ���������

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
