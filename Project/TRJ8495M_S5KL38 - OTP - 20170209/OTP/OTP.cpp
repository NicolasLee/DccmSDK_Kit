// OTP.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "OTP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// OTPApp

BEGIN_MESSAGE_MAP(OTPApp, CWinApp)
END_MESSAGE_MAP()


// OTPApp ����

OTPApp::OTPApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� OTPApp ����

OTPApp theApp;


// OTPApp ��ʼ��

BOOL OTPApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
