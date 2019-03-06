#include "StdAfx.h"
#include "CTimeCount.h"
// 计时开始

CTimeCount::CTimeCount()
{
	use_time = 0.0;
}

void CTimeCount::Start(void) 
{
	use_time = 0.0;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&old_time);
}

// 计时结束 
void CTimeCount::End(void)
{
	QueryPerformanceCounter(&new_time);
	use_time = (double)(new_time.QuadPart - old_time.QuadPart) / (double)frequency.QuadPart * 1000;
}

// 获得算法处理时间 (单位: ms)
double CTimeCount::GetUseTime(void) 
{
	return use_time;
}

void CTimeCount::MessageBoxTip(void)
{
	CString str;
	str.Format(_T("%.2lf ms"), use_time);
	AfxMessageBox(str);
}
