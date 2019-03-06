#include "stdafx.h"
#include "8858R2A_Shading.h"

C8858R2A_Shading::C8858R2A_Shading()
{
	memset(&m_values, 0, sizeof(m_values));	
	memset(&m_params, 0, sizeof(m_params));	
	
	SetOptionName(_T("_SetShading8858R2A"));
	SetReportFileName(_T("Lenc8858R2A.csv"));
}

C8858R2A_Shading::~C8858R2A_Shading()
{
	if (IsOptionLoaded())
	{
		SaveOptions();
	}
}
void C8858R2A_Shading::LoadOptions(void)
{
	CShading::LoadOptions();
	
	CString sectionName = GetOptionName();
	
	for (int i=0; i<SHADING_8858R2A_BLOCK_NUM; i++)
	{
		CString keyName;
		keyName.Format(_T("B%d.x"), i);
		m_params.block[i].x = ReadInt(sectionName, keyName, m_params.block[i].x);
		
		keyName.Format(_T("B%d.y"), i);
		m_params.block[i].y = ReadInt(sectionName, keyName, m_params.block[i].y);
	}
	
	m_params.width_Y = ReadInt(sectionName, _T("BWidth"), m_params.width_Y);
	m_params.height_Y= ReadInt(sectionName, _T("BHeight"), m_params.height_Y);
	m_params.width_C = ReadInt(sectionName, _T("CWidth"), m_params.width_C);
    m_params.height_C = ReadInt(sectionName, _T("CHeight"), m_params.height_C);

	m_params.yMax = ReadFloat(sectionName, _T("YMax"), m_params.yMax);
	m_params.yMin = ReadFloat(sectionName, _T("YMin"), m_params.yMin);
	m_params.yDif = ReadFloat(sectionName, _T("YDif"), m_params.yDif);
	m_params.rgMax = ReadFloat(sectionName, _T("RGMax"), m_params.rgMax);
	m_params.bgMax = ReadFloat(sectionName, _T("BGMax"), m_params.bgMax);
}
void C8858R2A_Shading::SaveOptions(void)
{
	CShading::SaveOptions();
	
	CString sectionName = GetOptionName();
	
	for (int i=0; i<SHADING_8858R2A_BLOCK_NUM; i++)
	{
		CString keyName;
		keyName.Format(_T("B%d.x"), i);
		WriteInt(sectionName, keyName, m_params.block[i].x);
		
		keyName.Format(_T("B%d.y"), i);
		WriteInt(sectionName, keyName, m_params.block[i].y);
	}
	
	WriteInt(sectionName, _T("BWidth"), m_params.width_Y);
	WriteInt(sectionName, _T("BHeight"), m_params.height_Y);
	WriteInt(sectionName, _T("CWidth"), m_params.width_C);
	WriteInt(sectionName, _T("CHeight"), m_params.height_C);
	
	WriteFloat(sectionName, _T("YMax"), m_params.yMax);
	WriteFloat(sectionName, _T("YMin"), m_params.yMin);
	WriteFloat(sectionName, _T("YDif"), m_params.yDif);
	WriteFloat(sectionName, _T("RGMax"), m_params.rgMax);
	WriteFloat(sectionName, _T("BGMax"), m_params.bgMax);
}
void* C8858R2A_Shading::GetValues(void)
{
	return &m_values;
}

void* C8858R2A_Shading::GetParams(void)
{
	return &m_params;
}

/*
B1(B6)          B4(B9)
        B0(B5)
B2(B7)          B3(B8)
*/
void C8858R2A_Shading::SetParams(UINT imageWidth, UINT imageHeight)
{
	//Y Shading
	UINT blockWidth  = imageWidth /5;
	UINT blockHeight = imageHeight/5;
		
	m_params.block[0].x = imageWidth /2 - blockWidth/2;
	m_params.block[0].y = imageHeight/2 - blockHeight/2;
	m_params.block[1].x = 0;
	m_params.block[1].y = 0;
	m_params.block[2].x = m_params.block[1].x;
	m_params.block[2].y = imageHeight - m_params.block[1].y - blockHeight;
	m_params.block[3].x = imageWidth - m_params.block[1].x - blockWidth;
	m_params.block[3].y = m_params.block[2].y;
	m_params.block[4].x = m_params.block[3].x;
	m_params.block[4].y = m_params.block[1].y;

	m_params.width_Y = blockWidth;
	m_params.height_Y= blockHeight;
	// Color  Shading
    blockWidth  = imageWidth /10;
	blockHeight = imageHeight /8;
	m_params.block[5].x = imageWidth /2 - blockWidth /2;
	m_params.block[5].y = imageHeight /2 - blockHeight /2;
	m_params.block[6].x = 0;
	m_params.block[6].y = 0;
	m_params.block[7].x = m_params.block[6].x;
	m_params.block[7].y = imageHeight - m_params.block[6].y - blockHeight;
	m_params.block[8].x = imageWidth - m_params.block[6].x - blockWidth;
	m_params.block[8].y = m_params.block[7].y;
	m_params.block[9].x = m_params.block[8].x;
	m_params.block[9].y = m_params.block[6].y;
	
	m_params.width_C  = blockWidth;
	m_params.height_C = blockHeight;

	m_params.yMax = 66;
	m_params.yMin = 60;
	m_params.yDif = 5;
	m_params.rgMax = 5;
	m_params.bgMax = 5;

/*	m_params.yMax = 65.5;
	m_params.yMin = 59.5;
	m_params.yDif = 5;
	m_params.rgMax = 7;
	m_params.bgMax = 7;
	
/*	m_params.yMax = 69.3;
	m_params.yMin = 63.3;
	m_params.yDif = 4;
	m_params.rgMax = 5;
	m_params.bgMax = 5;*/
}
void C8858R2A_Shading::CalcValues(BYTE *imageData, UINT imageWidth, UINT imageHeight, BYTE OB)
{
	memset(&m_values, 0, sizeof(m_values));	
	
	for (int i=0; i<SHADING_8858R2A_BLOCK_NUM; i++)
	{
		RGBStruct avg;
		if(i<=4)
		{
			GetRoiAverage_RGB(&avg, 
				CRect(CPoint(m_params.block[i].x, m_params.block[i].y), CSize(m_params.width_Y, m_params.height_Y)), 	
			imageData, imageWidth, imageHeight);

		}
		else
		{
			GetRoiAverage_RGB(&avg, 
				CRect(CPoint(m_params.block[i].x, m_params.block[i].y), CSize(m_params.width_C, m_params.height_C)), 	
				imageData, imageWidth, imageHeight);	
		}
		
		m_values.avg[i].r = avg.r - OB;
		m_values.avg[i].g = avg.g - OB;
		m_values.avg[i].b = avg.b - OB;
		
		RGB_Y(m_values.avg[i].y, m_values.avg[i].r, m_values.avg[i].g, m_values.avg[i].b);
	}
	
	m_values.yRatio[0] = 100;
	
	m_values.rg[5] = 1.0f * m_values.avg[5].r / m_values.avg[5].g;
	m_values.bg[5] = 1.0f * m_values.avg[5].b / m_values.avg[5].g;
	
	m_values.yMax  = 0;
	m_values.yMin  = 100;
	m_values.rgMax = 0;
	m_values.bgMax = 0;
	
	for (int j=1; j<5; j++)
	{
		m_values.yRatio[j] = 100.0f * m_values.avg[j].y / m_values.avg[0].y;
		
	}
	for (j=6;j<SHADING_8858R2A_BLOCK_NUM; j++)
	{
		m_values.rg[j] = 100.0f * (1.0f* m_values.avg[j].r / m_values.avg[j].g) / m_values.rg[5];
		m_values.bg[j] = 100.0f * (1.0f* m_values.avg[j].b / m_values.avg[j].g) / m_values.bg[5];
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
	
	m_values.rg[5] = 100;
	m_values.bg[5] = 100;	
}

BYTE C8858R2A_Shading::CheckPass(void)
{ 
	BYTE result = SHADING_RESULT_PASS;
	
	if (m_values.yMin < m_params.yMin || m_values.yMax > m_params.yMax)
	{
		result |= SHADING_Y_ERROR;
	}
	
	if (m_values.yDif > m_params.yDif)
	{
		result |= SHADING_YDIF_ERROR;
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
void C8858R2A_Shading::ShowTestInfo(void)
{	
	CString info;
	CString pos[5] = {_T("C "), _T("TL"), _T("BL"), _T("BR"), _T("TR")};
	
	AddInfo(_T("      Y      R      G      B"));
	
	for (int i=0; i<5; i++)
	{
		info.Format(_T("%s: %3d   %3d   %3d   %3d"), 
			pos[i], m_values.avg[i].y, m_values.avg[i+5].r, m_values.avg[i+5].g, m_values.avg[i+5].b);
		AddInfo(info);
	}
	
	AddInfo(_T("    ¡÷Y     ¡÷RG     ¡÷BG"));
	
	for (int j=1; j<5; j++)
	{
		info.Format(_T("%s: %5.2f   %5.2f   %5.2f"), 
			pos[j], m_values.yRatio[j], m_values.rg[j+5], m_values.bg[j+5]);
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
BOOL C8858R2A_Shading::SaveTestFile(void)
{
	CString fname = GetReportFilePath() + GetReportFileName();				
	CString headers = _T("Module NO., Y_TL, Y_BL, Y_BR, Y_TR, RG_TL, RG_BL, RG_BR, RG_TR, BG_TL, BG_BL, BG_BR, BG_TR, Ydif, R/Gmax, B/Gmax");		
	
	CString contents;
	contents.Format(_T("%s, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f"), 
		GetModuleID(), 
		m_values.yRatio[1],  m_values.yRatio[2],  m_values.yRatio[3],  m_values.yRatio[4], 
		m_values.rg[6], m_values.rg[7], m_values.rg[8], m_values.rg[9], 
		m_values.bg[6], m_values.bg[7], m_values.bg[8], m_values.bg[9],
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