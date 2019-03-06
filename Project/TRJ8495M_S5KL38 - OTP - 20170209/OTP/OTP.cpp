// OTP.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "OTP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// OTPApp

BEGIN_MESSAGE_MAP(OTPApp, CWinApp)
END_MESSAGE_MAP()


// OTPApp 构造

OTPApp::OTPApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 OTPApp 对象

OTPApp theApp;


// OTPApp 初始化

BOOL OTPApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
