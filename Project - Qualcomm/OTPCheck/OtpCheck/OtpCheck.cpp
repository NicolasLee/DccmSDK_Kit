// OtpCheck.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "OtpCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// OtpCheckApp

BEGIN_MESSAGE_MAP(OtpCheckApp, CWinApp)
END_MESSAGE_MAP()


// OtpCheckApp 构造

OtpCheckApp::OtpCheckApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 OtpCheckApp 对象

OtpCheckApp theApp;


// OtpCheckApp 初始化

BOOL OtpCheckApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
