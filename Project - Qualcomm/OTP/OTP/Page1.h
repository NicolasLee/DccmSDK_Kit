#pragma once


// CPage1 �Ի���

class CPage1 : public CDialogEx
{
	DECLARE_DYNAMIC(CPage1)

public:
	CPage1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPage1();
	
// �Ի�������
	enum { IDD = IDD_DIALOG_SensorR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	CComboBox steplist[15];
	CComboBox address[15];
	CComboBox value[15];
	CComboBox oplist[15];

	int SensorR_timer;
	int SensorR_num;
	int SensorR_step[15];
	int SensorR_address[15];
	int SensorR_value[15];
	int SensorR_bit[15];
	int SensorR_op[15];
	int SensorR_gvalue[15];
	int SensorR_sleep[15];
};
