// PD_CALI_Qual_2.h : PD_CALI_Qual_2 DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// PD_CALI_Qual_2App
// �йش���ʵ�ֵ���Ϣ������� PD_CALI_Qual_2.cpp
//

class PD_CALI_Qual_2App : public CWinApp
{
public:
	PD_CALI_Qual_2App();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
