// Test1.cpp : 实现文件
//

#include "stdafx.h"
#include "OTP.h"
#include "Test1.h"
#include "afxdialogex.h"


// Test1 对话框

IMPLEMENT_DYNAMIC(Test1, CDialogEx)

Test1::Test1(CWnd* pParent /*=NULL*/)
	: CDialogEx(Test1::IDD, pParent)
{

}

Test1::~Test1()
{
}

void Test1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Test1, CDialogEx)
END_MESSAGE_MAP()


// Test1 消息处理程序
