// PD_CALI_Qual_2.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "PDAF_QualL4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// PD_CALI_Qual_2App

BEGIN_MESSAGE_MAP(PD_CALI_Qual_2App, CWinApp)
END_MESSAGE_MAP()


// PD_CALI_Qual_2App ����

PD_CALI_Qual_2App::PD_CALI_Qual_2App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� PD_CALI_Qual_2App ����

PD_CALI_Qual_2App theApp;


// PD_CALI_Qual_2App ��ʼ��

BOOL PD_CALI_Qual_2App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
