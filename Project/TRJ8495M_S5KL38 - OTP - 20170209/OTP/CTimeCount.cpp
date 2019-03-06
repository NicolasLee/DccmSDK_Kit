#include "StdAfx.h"
#include "CTimeCount.h"
// ��ʱ��ʼ

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

// ��ʱ������������ 
void CTimeCount::End(void)
{
	QueryPerformanceCounter(&new_time);
	use_time = (double)(new_time.QuadPart - old_time.QuadPart) / (double)frequency.QuadPart * 1000;
}

// ����㷨����ʱ�� (��λ: ms)
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
