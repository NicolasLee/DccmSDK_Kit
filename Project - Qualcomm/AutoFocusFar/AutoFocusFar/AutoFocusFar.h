// AutoFocusFar.h : AutoFocusFar DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// AutoFocusFarApp
// �йش���ʵ�ֵ���Ϣ������� AutoFocusFar.cpp
//

class AutoFocusFarApp : public CWinApp
{
public:
	AutoFocusFarApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
