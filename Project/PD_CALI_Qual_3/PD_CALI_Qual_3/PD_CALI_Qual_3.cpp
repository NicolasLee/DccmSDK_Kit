// PD_CALI_Qual_3.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "PD_CALI_Qual_3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// PD_CALI_Qual_3App

BEGIN_MESSAGE_MAP(PD_CALI_Qual_3App, CWinApp)
END_MESSAGE_MAP()


// PD_CALI_Qual_3App 构造

PD_CALI_Qual_3App::PD_CALI_Qual_3App()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 PD_CALI_Qual_3App 对象

PD_CALI_Qual_3App theApp;


// PD_CALI_Qual_3App 初始化

BOOL PD_CALI_Qual_3App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
