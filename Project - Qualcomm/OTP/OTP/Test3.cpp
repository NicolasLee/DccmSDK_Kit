// Test3.cpp : 实现文件
//

#include "stdafx.h"
#include "OTP.h"
#include "Test3.h"
#include "afxdialogex.h"


// Test3 对话框

IMPLEMENT_DYNAMIC(Test3, CDialogEx)

Test3::Test3(CWnd* pParent /*=NULL*/)
	: CDialogEx(Test3::IDD, pParent)
{

}

Test3::~Test3()
{
}

void Test3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Test3, CDialogEx)
END_MESSAGE_MAP()


// Test3 消息处理程序
