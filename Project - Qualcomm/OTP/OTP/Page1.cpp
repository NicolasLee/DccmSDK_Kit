// Page1.cpp : 实现文件
//

#include "stdafx.h"
#include "OTP.h"
#include "Page1.h"
#include "afxdialogex.h"


// CPage1 对话框

IMPLEMENT_DYNAMIC(CPage1, CDialogEx)

CPage1::CPage1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPage1::IDD, pParent)
{

}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
		
	DDX_Text(pDX, IDC_SensorR_timer, SensorR_timer);
	DDX_Text(pDX, IDC_SensorR_num, SensorR_num);
	DDX_Control(pDX, IDC_SensorR_step1, steplist[0]);
	DDX_Control(pDX, IDC_SensorR_step2, steplist[1]);
	DDX_Control(pDX, IDC_SensorR_step3, steplist[2]);
	DDX_Control(pDX, IDC_SensorR_step4, steplist[3]);
	DDX_Control(pDX, IDC_SensorR_step5, steplist[4]);
	DDX_Control(pDX, IDC_SensorR_step6, steplist[5]);
	DDX_Control(pDX, IDC_SensorR_step7, steplist[6]);
	DDX_Control(pDX, IDC_SensorR_step8, steplist[7]);
	DDX_Control(pDX, IDC_SensorR_step9, steplist[8]);
	DDX_Control(pDX, IDC_SensorR_step10, steplist[9]);
	DDX_Control(pDX, IDC_SensorR_step11, steplist[10]);
	DDX_Control(pDX, IDC_SensorR_step12, steplist[11]);
	DDX_Control(pDX, IDC_SensorR_step13, steplist[12]);
	DDX_Control(pDX, IDC_SensorR_step14, steplist[13]);
	DDX_Control(pDX, IDC_SensorR_step15, steplist[14]);

	DDX_Control(pDX, IDC_SensorR_address1, address[0]);
	DDX_Control(pDX, IDC_SensorR_address2, address[1]);
	DDX_Control(pDX, IDC_SensorR_address3, address[2]);
	DDX_Control(pDX, IDC_SensorR_address4, address[3]);
	DDX_Control(pDX, IDC_SensorR_address5, address[4]);
	DDX_Control(pDX, IDC_SensorR_address6, address[5]);
	DDX_Control(pDX, IDC_SensorR_address7, address[6]);
	DDX_Control(pDX, IDC_SensorR_address8, address[7]);
	DDX_Control(pDX, IDC_SensorR_address9, address[8]);
	DDX_Control(pDX, IDC_SensorR_address10, address[9]);
	DDX_Control(pDX, IDC_SensorR_address11, address[10]);
	DDX_Control(pDX, IDC_SensorR_address12, address[11]);
	DDX_Control(pDX, IDC_SensorR_address13, address[12]);
	DDX_Control(pDX, IDC_SensorR_address14, address[13]);
	DDX_Control(pDX, IDC_SensorR_address15, address[14]);

	DDX_Control(pDX, IDC_SensorR_value1, value[0]);
	DDX_Control(pDX, IDC_SensorR_value2, value[1]);
	DDX_Control(pDX, IDC_SensorR_value3, value[2]);
	DDX_Control(pDX, IDC_SensorR_value4, value[3]);
	DDX_Control(pDX, IDC_SensorR_value5, value[4]);
	DDX_Control(pDX, IDC_SensorR_value6, value[5]);
	DDX_Control(pDX, IDC_SensorR_value7, value[6]);
	DDX_Control(pDX, IDC_SensorR_value8, value[7]);
	DDX_Control(pDX, IDC_SensorR_value9, value[8]);
	DDX_Control(pDX, IDC_SensorR_value10, value[9]);
	DDX_Control(pDX, IDC_SensorR_value11, value[10]);
	DDX_Control(pDX, IDC_SensorR_value12, value[11]);
	DDX_Control(pDX, IDC_SensorR_value13, value[12]);
	DDX_Control(pDX, IDC_SensorR_value14, value[13]);
	DDX_Control(pDX, IDC_SensorR_value15, value[14]);

	DDX_Control(pDX, IDC_SensorR_op1, oplist[0]);
	DDX_Control(pDX, IDC_SensorR_op2, oplist[1]);
	DDX_Control(pDX, IDC_SensorR_op3, oplist[2]);
	DDX_Control(pDX, IDC_SensorR_op4, oplist[3]);
	DDX_Control(pDX, IDC_SensorR_op5, oplist[4]);
	DDX_Control(pDX, IDC_SensorR_op6, oplist[5]);
	DDX_Control(pDX, IDC_SensorR_op7, oplist[6]);
	DDX_Control(pDX, IDC_SensorR_op8, oplist[7]);
	DDX_Control(pDX, IDC_SensorR_op9, oplist[8]);
	DDX_Control(pDX, IDC_SensorR_op10, oplist[9]);
	DDX_Control(pDX, IDC_SensorR_op11, oplist[10]);
	DDX_Control(pDX, IDC_SensorR_op12, oplist[11]);
	DDX_Control(pDX, IDC_SensorR_op13, oplist[12]);
	DDX_Control(pDX, IDC_SensorR_op14, oplist[13]);
	DDX_Control(pDX, IDC_SensorR_op15, oplist[14]);

	


}


BEGIN_MESSAGE_MAP(CPage1, CDialogEx)
END_MESSAGE_MAP()


// CPage1 消息处理程序

