#pragma once


// CPage4 对话框

class CPage4 : public CDialogEx
{
	DECLARE_DYNAMIC(CPage4)

public:
	CPage4(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage4();

// 对话框数据
	enum { IDD = IDD_DIALOG_AWB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
