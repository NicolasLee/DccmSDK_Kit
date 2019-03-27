#pragma once


// Test2 对话框

class Test2 : public CDialogEx
{
	DECLARE_DYNAMIC(Test2)

public:
	Test2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Test2();

// 对话框数据
	enum { IDD = IDD_DIALOG_AWB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
