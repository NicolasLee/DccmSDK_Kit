#pragma once


// CPage5 对话框

class CPage5 : public CDialogEx
{
	DECLARE_DYNAMIC(CPage5)

public:
	CPage5(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage5();

// 对话框数据
	enum { IDD = IDD_DIALOG_LSC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
