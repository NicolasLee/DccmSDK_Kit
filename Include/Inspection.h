#pragma once

#include "DccmDef.h"
#include "DccmInspectionType.h"

class DccmControl_Interface;
//DLL
#define WM_UPDATE_INSPECITON_OPTION     WM_USER + 0x1000

class Inspection
{
public:
	Inspection();
	virtual ~Inspection();

	virtual int Testing() = 0;   // �������� ����ʱ�������
	virtual int Initialize();    // �������� ����ǰ�������
	virtual int Finalize();      // �������� ���Ժ������

	virtual int LoadOption();
	virtual int SaveOption();

	virtual LPCTSTR GetReportHeaders() = 0;
	virtual LPCTSTR GetReportLowerLimits() = 0;
	virtual LPCTSTR GetReportUpperLimits() = 0;
	virtual LPCTSTR GetReportContents() = 0;

public:
	void SetType(int type);
	int  GetType(void);
	LPCTSTR GetTypeName(int type);	// ��ȡ���������͵��ַ�������

	void SetOptionDlgHandle(HWND hOptionDlg);
	HWND GetOptionDlgHandle(void);

	void SetResult(int testResult);
	int  GetResult(void); 

	BOOL isSaved(void);
	void SetSave(BOOL enable);

	void SetName(LPCTSTR name);
	LPCTSTR GetName(void);

	void SetFuseID(LPCTSTR fuseID);
	LPCTSTR GetFuseID(void);

	void SetFailName(LPCTSTR failName);
	LPCTSTR GetFailName();

	void SetInspectionID(int inspID);
	int GetInspectionID(void);

	BOOL IsRecvMsg(void);
	void SetRecvMsg(BOOL bRecvMsg);

	enum RETURN_STATUS
	{		
		RET_CONTIUNE  = -1,   // �������׶�ѭ��
		RET_END  	  = 0,    // �������׶�ѭ��
	};

	void	SetConfigPath(LPCTSTR pathName);

	CString ReadConfigString(LPCTSTR keyname, LPCTSTR defaultVal);
	void	WriteConfigString(LPCTSTR keyname, LPCTSTR string);

	int		ReadConfigInt(LPCTSTR keyname, int defaultVal);
	void	WriteConfigInt(LPCTSTR keyname, int data,int datatype = DATA_DEC);

	double	ReadConfigDouble(LPCTSTR keyname, double defaultVal);
	void	WriteConfigDouble(LPCTSTR keyname, double data, int precision = 2);

	DccmControl_Interface *m_pInterface;

private:
	HWND    m_hOptionDlg;
	int     m_testResult;		// ���Խ��
	CString m_inspectionName;	// ����������
	CString m_FuseID;			// Sensor FuseID 
	CString m_configPathName;	// �����ļ�����
	int     m_testType;			// ����������
	BOOL    m_bSave;			// �Ƿ�洢����
	CString m_failName;			// ͬһ��������Ŀ���м���fail���͡����磺particle IC IR
	BOOL	m_bRecvMsg;			// ��ȡ������ִ�е���Ϣ
	int		m_inspectionID;		// ������ID
};


