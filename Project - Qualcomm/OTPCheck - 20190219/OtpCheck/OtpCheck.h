// OtpCheck.h : OtpCheck DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// OtpCheckApp
// �йش���ʵ�ֵ���Ϣ������� OtpCheck.cpp
//

class OtpCheckApp : public CWinApp
{
public:
	OtpCheckApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
