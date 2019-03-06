#include "stdafx.h"
#include "Shading.h"


CShading::CShading()
{
	m_imageWidth = 0; 
	m_imageHeight = 0;
	m_sensorOB = 0;
	m_result = SHADING_RESULT_PASS;
	m_isOptionLoaded = FALSE;	
}

CShading::~CShading()
{
}

void CShading::Init(UINT imageWidth, UINT imageHeight, BYTE OB, HWND displayWnd, HWND logWnd)
{
	SetDisplayWindow(displayWnd);
	SetLogWindow(logWnd);

	m_imageWidth = imageWidth; 
	m_imageHeight = imageHeight;
	m_sensorOB = OB;

	m_result = SHADING_RESULT_PASS;
	
	SetParams(imageWidth, imageHeight);

	LoadOptions();
	m_isOptionLoaded = TRUE;
}

BOOL CShading::Test(BYTE *imageData)
{
	CalcValues(imageData, m_imageWidth, m_imageHeight, m_sensorOB);
	m_result = CheckPass();
	
	if (m_result == SHADING_RESULT_PASS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BYTE CShading::GetResult(void)
{
	return m_result;
}

void* CShading::GetValues(void)
{
	return NULL;
}

void* CShading::GetParams(void)
{
	return NULL;
}

void CShading::ShowTestInfo(void)
{	
}

BOOL CShading::SaveTestFile(void)
{
	return TRUE;
}

BOOL CShading::IsOptionLoaded(void)
{
	return m_isOptionLoaded;
}
