#pragma once


// Test3 对话框

class Test3 : public CDialogEx
{
	DECLARE_DYNAMIC(Test3)

public:
	Test3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Test3();

// 对话框数据
	enum { IDD = IDD_DIALOG_LSC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
