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

#define sensorOTP

class OtpCheck_Inspection :public Inspection
{
public:
	OtpCheck_Inspection(DccmControl_Interface* pInterface, LPCTSTR lpszName);
	~OtpCheck_Inspection(void);

	int Testing();       // ����ʱ �������
	int Initialize();    // ����ǰ �������
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
	OtpCheckBase* m_pOtpCheck;
	int OtpCheck_Test();

	void MatchProjName(eProjectName projName);
};
