#pragma once


// Test1 �Ի���

class Test1 : public CDialogEx
{
	DECLARE_DYNAMIC(Test1)

public:
	Test1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Test1();

// �Ի�������
	enum { IDD = IDD_DIALOG_ID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
