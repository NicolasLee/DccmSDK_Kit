#pragma once


// Test3 �Ի���

class Test3 : public CDialogEx
{
	DECLARE_DYNAMIC(Test3)

public:
	Test3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Test3();

// �Ի�������
	enum { IDD = IDD_DIALOG_LSC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
