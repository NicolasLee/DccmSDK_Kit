#pragma once


// Test2 �Ի���

class Test2 : public CDialogEx
{
	DECLARE_DYNAMIC(Test2)

public:
	Test2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Test2();

// �Ի�������
	enum { IDD = IDD_DIALOG_AWB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
