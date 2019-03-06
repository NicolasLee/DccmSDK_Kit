#include "stdafx.h"
#include "..\\OTP\\OtpBase.h"
#include "ShadingOV.h"
#include "..\\ImgProcess.h"


CShadingOV::CShadingOV()
{
	memset(&m_values, 0, sizeof(m_values));	
	memset(&m_params, 0, sizeof(m_params));	
	
	SetOptionName(_T("_SetShadingOV"));
	SetReportFileName(_T("LencOV.csv"));
}

CShadingOV::~CShadingOV()
{
	if (IsOptionLoaded())
	{
		SaveOptions();
	}
}

void CShadingOV::LoadOptions(void)
{
	CShading::LoadOptions();
	
	CString sectionName = GetOptionName();
	
	for (int i=0; i<SHADING_OV_BLOCK_NUM; i++)
	{
		CString keyName;
		keyName.Format(_T("Y_B%d.x"), i);
		m_params.block_Y[i].x = ReadInt(sectionName, keyName, m_params.block_Y[i].x);
		
		keyName.Format(_T("Y_B%d.y"), i);
		m_params.block_Y[i].y = ReadInt(sectionName, keyName, m_params.block_Y[i].y);

		keyName.Format(_T("Col_B%d.x"), i);
		m_params.block_Col[i].x = ReadInt(sectionName, keyName, m_params.block_Col[i].x);
		
		keyName.Format(_T("Col_B%d.y"), i);
		m_params.block_Col[i].y = ReadInt(sectionName, keyName, m_params.block_Col[i].y);
	}
	
	m_params.yWidth = ReadInt(sectionName, _T("YWidth"), m_params.yWidth);
	m_params.yHeight= ReadInt(sectionName, _T("YHeight"), m_params.yHeight);

	m_params.colWidth = ReadInt(sectionName, _T("ColWidth"), m_params.colWidth);
	m_params.colHeight= ReadInt(sectionName, _T("ColHeight"), m_params.colHeight);
	
	m_params.yMax = ReadFloat(sectionName, _T("YMax"), m_params.yMax);
	m_params.yMin = ReadFloat(sectionName, _T("YMin"), m_params.yMin);
	m_params.yDif = ReadFloat(sectionName, _T("YDif"), m_params.yDif);
	m_params.rgMax = ReadFloat(sectionName, _T("RGMax"), m_params.rgMax);
	m_params.bgMax = ReadFloat(sectionName, _T("BGMax"), m_params.bgMax);
}

void CShadingOV::SaveOptions(void)
{
	CShading::SaveOptions();
	
	CString sectionName = GetOptionName();
	
	for (int i=0; i<SHADING_OV_BLOCK_NUM; i++)
	{
		CString keyName;
		keyName.Format(_T("Y_B%d.x"), i);
		WriteInt(sectionName, keyName, m_params.block_Y[i].x);
		
		keyName.Format(_T("Y_B%d.y"), i);
		WriteInt(sectionName, keyName, m_params.block_Y[i].y);

		keyName.Format(_T("Col_B%d.x"), i);
		WriteInt(sectionName, keyName, m_params.block_Col[i].x);
		
		keyName.Format(_T("Col_B%d.y"), i);
		WriteInt(sectionName, keyName, m_params.block_Col[i].y);
	}
		
	WriteInt(sectionName, _T("YWidth"), m_params.yWidth);
	WriteInt(sectionName, _T("YHeight"), m_params.yHeight);

	WriteInt(sectionName, _T("ColWidth"), m_params.colWidth);
	WriteInt(sectionName, _T("ColHeight"), m_params.colHeight);
	
	WriteFloat(sectionName, _T("YMax"), m_params.yMax);
	WriteFloat(sectionName, _T("YMin"), m_params.yMin);
	WriteFloat(sectionName, _T("YDif"), m_params.yDif);
	WriteFloat(sectionName, _T("RGMax"), m_params.rgMax);
	WriteFloat(sectionName, _T("BGMax"), m_params.bgMax);
}

void* CShadingOV::GetValues(void)
{
	return &m_values;
}

void* CShadingOV::GetParams(void)
{
	return &m_params;
}

/*
B1          B4
      B0
B2          B3
*/
void CShadingOV::SetLSC_ROI(UINT imageWidth, UINT imageHeight,BYTE RoiMode_Y,BYTE RoiMode_Col)
{
	UINT blockWidth  ;
	UINT blockHeight ;
	switch(RoiMode_Y)
	{
	case ROI_CALC_5:
		blockWidth  = imageWidth /5;
		blockHeight = imageHeight/5;
		break;
	case ROI_CALC_10_8:
		blockWidth  = imageWidth /10;
		blockHeight = imageHeight/8;
		break;
	case ROI_CALC_12_10:
		blockWidth  = imageWidth /12;
		blockHeight = imageHeight/10;
		break;
	default:
		blockWidth  = imageWidth /5;
		blockHeight = imageHeight/5;
		break;
	}
	m_params.block_Y[0].x = imageWidth /2 - blockWidth/2;
	m_params.block_Y[0].y = imageHeight/2 - blockHeight/2;
	m_params.block_Y[1].x = 0;
	m_params.block_Y[1].y = 0;
	m_params.block_Y[2].x = m_params.block_Y[1].x;
	m_params.block_Y[2].y = imageHeight - m_params.block_Y[1].y - blockHeight;
	m_params.block_Y[3].x = imageWidth - m_params.block_Y[1].x - blockWidth;
	m_params.block_Y[3].y = m_params.block_Y[2].y;
	m_params.block_Y[4].x = m_params.block_Y[3].x;
	m_params.block_Y[4].y = m_params.block_Y[1].y;
	
	m_params.yWidth = blockWidth;
	m_params.yHeight= blockHeight;

	switch(RoiMode_Col)
	{

	case ROI_CALC_5:
		blockWidth  = imageWidth /5;
		blockHeight = imageHeight/5;
		break;
	case ROI_CALC_10_8:
		blockWidth  = imageWidth /10;
		blockHeight = imageHeight/8;
		break;
	case ROI_CALC_12_10:
		blockWidth  = imageWidth /12;
		blockHeight = imageHeight/10;
		break;
	default:
		blockWidth  = imageWidth /5;
		blockHeight = imageHeight/5;
		break;
	}
	m_params.block_Col[0].x = imageWidth /2 - blockWidth/2;
	m_params.block_Col[0].y = imageHeight/2 - blockHeight/2;
	m_params.block_Col[1].x = 0;
	m_params.block_Col[1].y = 0;
	m_params.block_Col[2].x = m_params.block_Col[1].x;
	m_params.block_Col[2].y = imageHeight - m_params.block_Col[1].y - blockHeight;
	m_params.block_Col[3].x = imageWidth - m_params.block_Col[1].x - blockWidth;
	m_params.block_Col[3].y = m_params.block_Col[2].y;
	m_params.block_Col[4].x = m_params.block_Col[3].x;
	m_params.block_Col[4].y = m_params.block_Col[1].y;

	m_params.colWidth = blockWidth;
	m_params.colHeight= blockHeight;

	m_params.yMax = 60;
	m_params.yMin = 54;
	m_params.yDif = 5;
	m_params.rgMax = 5;
	m_params.bgMax = 5;
}
void CShadingOV::SetParams(UINT imageWidth, UINT imageHeight)
{
	LencOtpControlStruct *lencCtrl = COtpBase::GetLencControlParams();
	BYTE Y_RoiMode = lencCtrl->Y_roiCalcMode;
	BYTE Col_ROiMode = lencCtrl->Col_roiCalcMode;
	SetLSC_ROI(imageWidth,imageHeight,Y_RoiMode,Col_ROiMode);
}

void CShadingOV::CalcValues(BYTE *imageData, UINT imageWidth, UINT imageHeight, BYTE OB)
{
	memset(&m_values, 0, sizeof(m_values));	

	for (int i=0; i<SHADING_OV_BLOCK_NUM; i++)
	{
		RGBStruct avg;
		GetRoiAverage_RGB(&avg, CRect(CPoint(m_params.block_Y[i].x, m_params.block_Y[i].y), CSize(m_params.yWidth, m_params.yHeight)), 	
			imageData, imageWidth, imageHeight);
		m_values.avg_Y[i].r = avg.r - OB;
		m_values.avg_Y[i].g = avg.g - OB;
		m_values.avg_Y[i].b = avg.b - OB;

		RGB_Y(m_values.avg_Y[i].y, m_values.avg_Y[i].r, m_values.avg_Y[i].g, m_values.avg_Y[i].b);
		GetRoiAverage_RGB(&avg, CRect(CPoint(m_params.block_Col[i].x, m_params.block_Col[i].y), CSize(m_params.colWidth, m_params.colHeight)), 	
			imageData, imageWidth, imageHeight);
		m_values.avg_Col[i].r = avg.r - OB;
		m_values.avg_Col[i].g = avg.g - OB;
		m_values.avg_Col[i].b = avg.b - OB;
	}
	m_values.yRatio[0] = 100;

	m_values.rg[0] = 1.0f * m_values.avg_Col[0].r / m_values.avg_Col[0].g;
	m_values.bg[0] = 1.0f * m_values.avg_Col[0].b / m_values.avg_Col[0].g;
	
	m_values.yMax  = 0;
	m_values.yMin  = 100;
	m_values.rgMax = 0;
	m_values.bgMax = 0;
	
	for (int j=1; j<SHADING_OV_BLOCK_NUM; j++)
	{
		m_values.yRatio[j] = 100.0f * m_values.avg_Y[j].y / m_values.avg_Y[0].y;
		m_values.rg[j] = 100.0f * (1.0f* m_values.avg_Col[j].r / m_values.avg_Col[j].g) / m_values.rg[0];
		m_values.bg[j] = 100.0f * (1.0f* m_values.avg_Col[j].b / m_values.avg_Col[j].g) / m_values.bg[0];
		
		if (fabs(m_values.rgMax) < fabs(m_values.rg[j] - 100))
		{
			m_values.rgMax = m_values.rg[j] - 100;
		}

		if (fabs(m_values.bgMax) < fabs(m_values.bg[j] - 100))
		{
			m_values.bgMax = m_values.bg[j] - 100;
		}
	}
	
	m_values.yMax = max(m_values.yRatio[1], m_values.yRatio[2]);
	m_values.yMax = max(m_values.yMax, m_values.yRatio[3]);
	m_values.yMax = max(m_values.yMax, m_values.yRatio[4]);

	m_values.yMin = min(m_values.yRatio[1], m_values.yRatio[2]);
	m_values.yMin = min(m_values.yMin, m_values.yRatio[3]);
	m_values.yMin = min(m_values.yMin, m_values.yRatio[4]);
	
	m_values.yDif = m_values.yMax - m_values.yMin;

	m_values.rg[0] = 100;
	m_values.bg[0] = 100;	
}

BYTE CShadingOV::CheckPass(void)
{ 
	CString temp;

	BYTE result = SHADING_RESULT_PASS;
	if (m_values.yMin < m_params.yMin)
	{
		temp.Format(_T("m_values.yMin: %f < m_params.yMin: %f"), m_values.yMin, m_params.yMin);
		AddInfo(temp, LOG_NG);
		result |= SHADING_Y_ERROR;
	}
	if (m_values.yMax > m_params.yMax)
	{
		temp.Format(_T("m_params.yMax: %f < m_params.yMax: %f"), m_values.yMax, m_params.yMax);
		AddInfo(temp, LOG_NG);
		result |= SHADING_Y_ERROR;
	}

	if (m_values.yDif > m_params.yDif)
	{
		temp.Format(_T("m_values.yDif: %f m_params.yDif: %f"), m_values.yDif, m_params.yDif);
		AddInfo(temp, LOG_NG);
		result |= SHADING_YDIF_ERROR;
	}
	
	if (fabs(m_values.rgMax) > m_params.rgMax)
	{
		temp.Format(_T("m_values.rgMax: %f > m_params.rgMax: %f"), fabs(m_values.rgMax), m_params.rgMax);
		AddInfo(temp, LOG_NG);
		result |= SHADING_RG_ERROR;
	}

	if (fabs(m_values.bgMax) > m_params.bgMax)
	{
		temp.Format(_T("m_values.bgMax: %f > m_params.bgMax: %f"), fabs(m_values.bgMax), m_params.bgMax);
		AddInfo(temp, LOG_NG);
		result |= SHADING_BG_ERROR;
	}

	return result;
}

void CShadingOV::ShowTestInfo(void)
{	
	CString info;
	CString pos[5] = {_T("C "), _T("TL"), _T("BL"), _T("BR"), _T("TR")};
	
	AddInfo(_T("      Y      R      G      B"));
	
	for (int i=0; i<SHADING_OV_BLOCK_NUM; i++)
	{
		info.Format(_T("%s: %3d   %3d   %3d   %3d"), 
			pos[i], m_values.avg_Y[i].y, m_values.avg_Col[i].r, m_values.avg_Col[i].g, m_values.avg_Col[i].b);
		AddInfo(info);
	}
	
	AddInfo(_T("    ¡÷Y     ¡÷RG     ¡÷BG"));
	
	for (int j=1; j<SHADING_OV_BLOCK_NUM; j++)
	{
		info.Format(_T("%s: %5.2f   %5.2f   %5.2f"), 
			pos[j], m_values.yRatio[j], m_values.rg[j], m_values.bg[j]);
		AddInfo(info);
	}
	
	info.Format(_T("Ymax: %5.2f,  Ymin: %5.2f"), m_values.yMax, m_values.yMin);
	AddInfo(info);
	
	info.Format(_T("Ydif: %5.2f"), m_values.yDif);
	AddInfo(info);
//	AddInfo(info+_T("%%"));
	
	info.Format(_T("R/G: %5.2f,  B/G: %5.2f"), m_values.rgMax, m_values.bgMax);
	AddInfo(info);
}

BOOL CShadingOV::SaveTestFile(void)
{
	CString fname = GetReportFilePath() + GetReportFileName();				
	CString headers = _T("Module NO., Y_TL, Y_BL, Y_BR, Y_TR, RG_TL, RG_BL, RG_BR, RG_TR, BG_TL, BG_BL, BG_BR, BG_TR, Ydif, R/Gmax, B/Gmax");		
	
	CString contents;
	contents.Format(_T("%s, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f"), 
		GetModuleID(), 
		m_values.yRatio[1],  m_values.yRatio[2],  m_values.yRatio[3],  m_values.yRatio[4], 
		m_values.rg[1], m_values.rg[2], m_values.rg[3], m_values.rg[4], 
		m_values.bg[1], m_values.bg[2], m_values.bg[3], m_values.bg[4],
		m_values.yDif, m_values.rgMax, m_values.bgMax);
	
	if (SaveFileCSV(fname, headers, contents))
	{
		CString info;
		info.Format(_T("Shading Save to: %s"), fname);
		AddInfo(info, LOG_NORMAL);
		
		return TRUE;
	}

	AddInfo(_T("Save Shading File ERROR"), LOG_NG);
	return FALSE;
}



