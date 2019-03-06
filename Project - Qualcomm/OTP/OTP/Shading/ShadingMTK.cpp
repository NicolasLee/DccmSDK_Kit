#include "stdafx.h"
#include "ShadingMTK.h"


CShadingMTK::CShadingMTK()
{
	memset(&m_values, 0, sizeof(m_values));	
	memset(&m_params, 0, sizeof(m_params));	

	SetOptionName(_T("_SetShadingMTK"));
	SetReportFileName(_T("LencMTK.csv"));
}

CShadingMTK::~CShadingMTK()
{
	if (IsOptionLoaded())
	{
		SaveOptions();
	}
}

void CShadingMTK::LoadOptions(void)
{
	CShading::LoadOptions();
	
	CString sectionName = GetOptionName();
	
	for (int i=0; i<SHADING_MTK_BLOCK_NUM; i++)
	{
		CString keyName;
		keyName.Format(_T("B%d.x"), i);
		m_params.block[i].x = ReadInt(sectionName, keyName, m_params.block[i].x);

		keyName.Format(_T("B%d.y"), i);
		m_params.block[i].y = ReadInt(sectionName, keyName, m_params.block[i].y);
	}

	m_params.width = ReadInt(sectionName, _T("BWidth"), m_params.width);
	m_params.height= ReadInt(sectionName, _T("BHeight"), m_params.height);
	
	m_params.yMax = ReadFloat(sectionName, _T("YMax"), m_params.yMax);
	m_params.yMin = ReadFloat(sectionName, _T("YMin"), m_params.yMin);
	m_params.yDif = ReadFloat(sectionName, _T("YDif"), m_params.yDif);
	m_params.gMax = ReadFloat(sectionName, _T("GMax"), m_params.gMax);
	m_params.gMin = ReadFloat(sectionName, _T("GMin"), m_params.gMin);
	m_params.rgMax = ReadFloat(sectionName, _T("RGMax"), m_params.rgMax);
	m_params.bgMax = ReadFloat(sectionName, _T("BGMax"), m_params.bgMax);
}

void CShadingMTK::SaveOptions(void)
{
	CShading::SaveOptions();
	
	CString sectionName = GetOptionName();
	
	for (int i=0; i<SHADING_MTK_BLOCK_NUM; i++)
	{
		CString keyName;
		keyName.Format(_T("B%d.x"), i);
		WriteInt(sectionName, keyName, m_params.block[i].x);
		
		keyName.Format(_T("B%d.y"), i);
		WriteInt(sectionName, keyName, m_params.block[i].y);
	}
	
	WriteInt(sectionName, _T("BWidth"), m_params.width);
	WriteInt(sectionName, _T("BHeight"), m_params.height);
	
	WriteFloat(sectionName, _T("YMax"), m_params.yMax);
	WriteFloat(sectionName, _T("YMin"), m_params.yMin);
	WriteFloat(sectionName, _T("YDif"), m_params.yDif);
	WriteFloat(sectionName, _T("GMax"), m_params.gMax);
	WriteFloat(sectionName, _T("GMin"), m_params.gMin);
	WriteFloat(sectionName, _T("RGMax"), m_params.rgMax);
	WriteFloat(sectionName, _T("BGMax"), m_params.bgMax);
}

void* CShadingMTK::GetValues(void)
{
	return &m_values;
}

void* CShadingMTK::GetParams(void)
{
	return &m_params;
}

/*
B1      B2      B3
    B4      B5
B6      B0      B7
    B8      B9
B10     B11     B12
*/
void CShadingMTK::SetParams(UINT imageWidth, UINT imageHeight)
{
	UINT blockWidth  = imageWidth /10;
	UINT blockHeight = imageHeight/10;
		
	m_params.block[0].x  = imageWidth /2 - blockWidth/2;
	m_params.block[0].y  = imageHeight/2 - blockHeight/2;
	m_params.block[1].x  = 0;
	m_params.block[1].y  = 0;
	m_params.block[2].x  = m_params.block[0].x;
	m_params.block[2].y  = m_params.block[1].y;
	m_params.block[3].x  = imageWidth - m_params.block[1].x - blockWidth;
	m_params.block[3].y  = m_params.block[1].y;
	m_params.block[4].x  = imageWidth/4  - blockWidth/2;
	m_params.block[4].y  = imageHeight/4 - blockHeight/2;
	m_params.block[5].x  = imageWidth - m_params.block[4].x - blockWidth;
	m_params.block[5].y  = m_params.block[4].y;
	m_params.block[6].x  = m_params.block[1].x;
	m_params.block[6].y  = imageHeight/2 - blockHeight/2;
	m_params.block[7].x  = m_params.block[3].x;		
	m_params.block[7].y  = m_params.block[6].y;	
	m_params.block[8].x  = m_params.block[4].x;
	m_params.block[8].y  = imageHeight - m_params.block[4].y - blockHeight;
	m_params.block[9].x  = m_params.block[5].x;
	m_params.block[9].y  = m_params.block[8].y;
	m_params.block[10].x = m_params.block[1].x;
	m_params.block[10].y = imageHeight - m_params.block[1].y - blockHeight;
	m_params.block[11].x = m_params.block[2].x;
	m_params.block[11].y = m_params.block[10].y;
	m_params.block[12].x = m_params.block[3].x;
	m_params.block[12].y = m_params.block[10].y;

	m_params.width = blockWidth;
	m_params.height= blockHeight;

	m_params.yMax = 64;
	m_params.yMin = 58;
	m_params.yDif = 5;
	m_params.gMax = 64;
	m_params.gMin = 58;
	m_params.rgMax = 5;
	m_params.bgMax = 5;
}

void CShadingMTK::CalcValues(BYTE *imageData, UINT imageWidth, UINT imageHeight, BYTE OB)
{
	memset(&m_values, 0, sizeof(m_values));	

	for (int i=0; i<SHADING_MTK_BLOCK_NUM; i++)
	{
		RGBStruct avg;
		GetRoiAverage_RGB(&avg, 
			CRect(CPoint(m_params.block[i].x, m_params.block[i].y), CSize(m_params.width, m_params.height)),
			imageData, imageWidth, imageHeight);	

		m_values.avg[i].r = avg.r - OB;
		m_values.avg[i].g = avg.g - OB;
		m_values.avg[i].b = avg.b - OB;

		RGB_Y(m_values.avg[i].y, m_values.avg[i].r, m_values.avg[i].g, m_values.avg[i].b);
	}
	
	m_values.yRatio[0] = 100;
	m_values.gRatio[0] = 100;

	m_values.rg[0] = 1.0f * m_values.avg[0].r / m_values.avg[0].g;
	m_values.bg[0] = 1.0f * m_values.avg[0].b / m_values.avg[0].g;
	
	m_values.yMax  = 0;
	m_values.yMin  = 100;
	m_values.gMax  = 0;
	m_values.gMin  = 100;
	m_values.rgMax = 0;
	m_values.bgMax = 0;
	
	for (int j=1; j<SHADING_MTK_BLOCK_NUM; j++)
	{
		if (m_values.avg[0].y==0)
		{
			m_values.yRatio[j] = 0;
		}
		else
		{
			m_values.yRatio[j] = 100.0f * m_values.avg[j].y / m_values.avg[0].y;
		}
		if (m_values.avg[0].g==0)
		{
			m_values.gRatio[j] = 0;
		}
		else
		{
			m_values.gRatio[j] = 100.0f * m_values.avg[j].g / m_values.avg[0].g;
		}
		if (m_values.avg[j].g==0||m_values.rg[0]==0)
		{
			m_values.rg[j] = 100;
		}
		else
		{
			m_values.rg[j] = 100.0f * ((1.0f* m_values.avg[j].r / m_values.avg[j].g) / m_values.rg[0] -1);
		}
		if (m_values.avg[j].g==0||m_values.bg[0]==0)
		{
			m_values.bg[j] = 100;
		}
		else
		{
			m_values.bg[j] = 100.0f * ((1.0f* m_values.avg[j].b / m_values.avg[j].g) / m_values.bg[0] -1);
		}
		if (fabs(m_values.rgMax) < fabs(m_values.rg[j]))
		{
			m_values.rgMax = m_values.rg[j];
		}

		if (fabs(m_values.bgMax) < fabs(m_values.bg[j]))
		{
			m_values.bgMax = m_values.bg[j];
		}
	}
	m_values.gMax = max(m_values.gRatio[1], m_values.gRatio[3]);
	m_values.gMax = max(m_values.gMax, m_values.gRatio[10]);
	m_values.gMax = max(m_values.gMax, m_values.gRatio[12]);
	
	m_values.gMin = min(m_values.gRatio[1], m_values.gRatio[3]);
	m_values.gMin = min(m_values.gMin, m_values.gRatio[10]);
	m_values.gMin = min(m_values.gMin, m_values.gRatio[12]);
	
	m_values.yMax = max(m_values.yRatio[1], m_values.yRatio[3]);
	m_values.yMax = max(m_values.yMax, m_values.yRatio[10]);
	m_values.yMax = max(m_values.yMax, m_values.yRatio[12]);

	m_values.yMin = min(m_values.yRatio[1], m_values.yRatio[3]);
	m_values.yMin = min(m_values.yMin, m_values.yRatio[10]);
	m_values.yMin = min(m_values.yMin, m_values.yRatio[12]);
	
	m_values.yDif = m_values.yMax - m_values.yMin;

	m_values.rg[0] = 0;
	m_values.bg[0] = 0;	
}

BYTE CShadingMTK::CheckPass(void)
{ 
	BYTE result = SHADING_RESULT_PASS;
	
	if (((fabs(m_values.yMin)) < m_params.yMin) || ((fabs(m_values.yMax)) > m_params.yMax))
	{
		result |= SHADING_Y_ERROR;
	}

	if (((fabs(m_values.yDif)) > m_params.yDif))
	{
		result |= SHADING_YDIF_ERROR;
	}
	
	if ((fabs(m_values.gMin)) < m_params.gMin || (fabs(m_values.gMax) > m_params.gMax))
	{
		result |= SHADING_G_ERROR;
	}
	
	if (fabs(m_values.rgMax) > m_params.rgMax)
	{
		result |= SHADING_RG_ERROR;
	}

	if (fabs(m_values.bgMax) > m_params.bgMax)
	{
		result |= SHADING_BG_ERROR;
	}

	return result;
}

void CShadingMTK::ShowTestInfo(void)
{	
	CString info;
	CString pos[5] = {_T("C "), _T("TL"), _T("BL"), _T("BR"), _T("TR")};

	AddInfo(_T("    ¡÷Y    ¡÷G    ¡÷RG    ¡÷BG"));
	
	info.Format(_T("%s: %5.2f  %5.2f  %5.2f  %5.2f"), pos[1], m_values.yRatio[1], m_values.gRatio[1], m_values.rg[1], m_values.bg[1]);
	AddInfo(info);

	info.Format(_T("%s: %5.2f  %5.2f  %5.2f  %5.2f"), pos[2], m_values.yRatio[10], m_values.gRatio[10], m_values.rg[10], m_values.bg[10]);
	AddInfo(info);

	info.Format(_T("%s: %5.2f  %5.2f  %5.2f  %5.2f"), pos[3], m_values.yRatio[12], m_values.gRatio[12], m_values.rg[12], m_values.bg[12]);
	AddInfo(info);
	
	info.Format(_T("%s: %5.2f  %5.2f  %5.2f  %5.2f"), pos[4], m_values.yRatio[3], m_values.gRatio[3], m_values.rg[3], m_values.bg[3]);
	AddInfo(info);		

	info.Format(_T("Ymin=%.2f, Ymax=%.2f"), m_values.yMin, m_values.yMax);
	AddInfo(info);
	
	info.Format(_T("YDif=%.2f"), m_values.yDif);
	AddInfo(info);
	
	info.Format(_T("Gmin=%.2f, Gmax=%.2f"), m_values.gMin, m_values.gMax);
	AddInfo(info);
	
	info.Format(_T("R/G=%.2f, B/G=%.2f"), m_values.rgMax, m_values.bgMax);
	AddInfo(info);
}

BOOL CShadingMTK::SaveTestFile(void)
{
	CString fname = GetReportFilePath() + GetReportFileName();				
	CString headers = _T("Module NO., ");			
	CString contents = GetModuleID() + _T(", ");
	
	CString tmpHeaders = _T("");
	CString tmpContents = _T("");
	
	for (int i=0; i<SHADING_MTK_BLOCK_NUM; i++)
	{
		tmpHeaders.Format(_T("Y%d, "), i);
		headers += tmpHeaders;
		
		tmpContents.Format(_T("%.2f, "), m_values.yRatio[i]);
		contents += tmpContents;
	}
	
	for (int g=0; g<SHADING_MTK_BLOCK_NUM; g++)
	{
		tmpHeaders.Format(_T("G%d, "), g);
		headers += tmpHeaders;
		
		tmpContents.Format(_T("%.2f, "), m_values.gRatio[g]);
		contents += tmpContents;
	}
	
	for (int j=0; j<SHADING_MTK_BLOCK_NUM; j++)
	{
		tmpHeaders.Format(_T("R%d, "), j);
		headers += tmpHeaders;
		
		tmpContents.Format(_T("%.2f, "), m_values.rg[j]+100);
		contents += tmpContents;
	}
	
	for (int k=0; k<SHADING_MTK_BLOCK_NUM; k++)
	{
		tmpHeaders.Format(_T("B%d, "), k);
		headers += tmpHeaders;
		
		tmpContents.Format(_T("%.2f, "), m_values.bg[k]+100);
		contents += tmpContents;
	}
	
	headers += _T("Ymin, Ymax, Ydif, Gmin, Gmax, R/Gmax, B/Gmax");		
	
	tmpContents.Format(_T("%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f"), 
		m_values.yMin, m_values.yMax, m_values.yDif, m_values.gMin, m_values.gMax, m_values.rgMax, m_values.bgMax);
	
	contents += tmpContents;
	
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



