// OTP.h : OTP DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// OTPApp
// �йش���ʵ�ֵ���Ϣ������� OTP.cpp
//

class OTPApp : public CWinApp
{
public:
	OTPApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
