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

	virtual int Testing() = 0;   // 子类重载 测试时处理代码
	virtual int Initialize();    // 子类重载 测试前处理代码
	virtual int Finalize();      // 子类重载 测试后处理代码

	virtual int LoadOption();
	virtual int SaveOption();

	virtual LPCTSTR GetReportHeaders() = 0;
	virtual LPCTSTR GetReportLowerLimits() = 0;
	virtual LPCTSTR GetReportUpperLimits() = 0;
	virtual LPCTSTR GetReportContents() = 0;

public:
	void SetType(int type);
	int  GetType(void);
	LPCTSTR GetTypeName(int type);	// 获取测试项类型的字符串名称

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
		RET_CONTIUNE  = -1,   // 继续本阶段循环
		RET_END  	  = 0,    // 结束本阶段循环
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
	int     m_testResult;		// 测试结果
	CString m_inspectionName;	// 测试项名称
	CString m_FuseID;			// Sensor FuseID 
	CString m_configPathName;	// 配置文件名称
	int     m_testType;			// 测试项类型
	BOOL    m_bSave;			// 是否存储数据
	CString m_failName;			// 同一个测试项目，有几种fail类型。例如：particle IC IR
	BOOL	m_bRecvMsg;			// 获取到继续执行的消息
	int		m_inspectionID;		// 测试项ID
};


