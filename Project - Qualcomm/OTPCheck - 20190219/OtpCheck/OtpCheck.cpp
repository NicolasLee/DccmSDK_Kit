// OtpCheck.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "OtpCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// OtpCheckApp

BEGIN_MESSAGE_MAP(OtpCheckApp, CWinApp)
END_MESSAGE_MAP()


// OtpCheckApp ����

OtpCheckApp::OtpCheckApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� OtpCheckApp ����

OtpCheckApp theApp;


// OtpCheckApp ��ʼ��

BOOL OtpCheckApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
