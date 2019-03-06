// PD_CALI_Qual_2.h : PD_CALI_Qual_2 DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// PD_CALI_Qual_2App
// 有关此类实现的信息，请参阅 PD_CALI_Qual_2.cpp
//

class PD_CALI_Qual_2App : public CWinApp
{
public:
	PD_CALI_Qual_2App();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
