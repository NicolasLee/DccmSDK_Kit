#pragma once


// CPage4 �Ի���

class CPage4 : public CDialogEx
{
	DECLARE_DYNAMIC(CPage4)

public:
	CPage4(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPage4();

// �Ի�������
	enum { IDD = IDD_DIALOG_AWB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
