// PD_CALI_Qual_2.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "PDAF_QualL4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// PD_CALI_Qual_2App

BEGIN_MESSAGE_MAP(PD_CALI_Qual_2App, CWinApp)
END_MESSAGE_MAP()


// PD_CALI_Qual_2App 构造

PD_CALI_Qual_2App::PD_CALI_Qual_2App()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 PD_CALI_Qual_2App 对象

PD_CALI_Qual_2App theApp;


// PD_CALI_Qual_2App 初始化

BOOL PD_CALI_Qual_2App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
