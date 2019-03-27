// Test2.cpp : 实现文件
//

#include "stdafx.h"
#include "OTP.h"
#include "Test2.h"
#include "afxdialogex.h"


// Test2 对话框

IMPLEMENT_DYNAMIC(Test2, CDialogEx)

Test2::Test2(CWnd* pParent /*=NULL*/)
	: CDialogEx(Test2::IDD, pParent)
{

}

Test2::~Test2()
{
}

void Test2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Test2, CDialogEx)
END_MESSAGE_MAP()


// Test2 消息处理程序
