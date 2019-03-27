#pragma once


// Test1 对话框

class Test1 : public CDialogEx
{
	DECLARE_DYNAMIC(Test1)

public:
	Test1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Test1();

// 对话框数据
	enum { IDD = IDD_DIALOG_ID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
