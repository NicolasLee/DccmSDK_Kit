#pragma once


// CPage7 �Ի���

class CPage7 : public CDialogEx
{
	DECLARE_DYNAMIC(CPage7)

public:
	CPage7(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPage7();

// �Ի�������
	enum { IDD = IDD_DIALOG_PDAF1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
