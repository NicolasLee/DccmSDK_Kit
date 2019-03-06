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

class OtpCheck_Inspection :public Inspection //OtpCheck_Inspection����ļ����Ǳ�Ҫ�����������Ķ���
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
	void UpdateOTPCheckUI();

	void MatchProjName(eProjectName projName);
};
