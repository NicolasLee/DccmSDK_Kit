#include "stdafx.h"
#include "ShadingQualcomm.h"


CShadingQualcomm::CShadingQualcomm()
{
	memset(&m_values, 0, sizeof(m_values));	
	memset(&m_params, 0, sizeof(m_params));	
	
	SetOptionName(_T("_SetShadingQualcomm"));
	SetReportFileName(_T("LencQualcomm.csv"));
}

CShadingQualcomm::~CShadingQualcomm()
{
	if (IsOptionLoaded())
	{
		SaveOptions();
	}
}

void CShadingQualcomm::LoadOptions(void)
{
	CShading::LoadOptions();
	
	CString sectionName = GetOptionName();
	
	CString keyName;
	int i;

	for (i=0; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
	{
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
	m_params.rgMax = ReadFloat(sectionName, _T("RGMax"), m_params.rgMax);
	m_params.bgMax = ReadFloat(sectionName, _T("BGMax"), m_params.bgMax);

	for (i=1; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
	{
		keyName.Format(_T("GoldenY%d"), i);
		m_params.yGolden[i] = ReadFloat(sectionName, keyName, m_params.yGolden[i]);
	}
	
	for (i=1; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
	{
		keyName.Format(_T("GoldenRG%d"), i);
		m_params.rgGolden[i] = ReadFloat(sectionName, keyName, m_params.rgGolden[i]);
	}
	
	for (i=1; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
	{
		keyName.Format(_T("GoldenBG%d"), i);
		m_params.bgGolden[i] = ReadFloat(sectionName, keyName, m_params.bgGolden[i]);
	}	

	m_params.yRelMax  = ReadFloat(sectionName, _T("Rel_YMax"), m_params.yRelMax);
	m_params.rgRelMax = ReadFloat(sectionName, _T("Rel_RGMax"), m_params.rgRelMax);
	m_params.bgRelMax = ReadFloat(sectionName, _T("Rel_BGMax"), m_params.bgRelMax);
}

void CShadingQualcomm::SaveOptions(void)
{
	CShading::SaveOptions();
	
	CString sectionName = GetOptionName();

	CString keyName;
	int i;

	for (i=0; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
	{
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
	WriteFloat(sectionName, _T("RGMax"), m_params.rgMax);
	WriteFloat(sectionName, _T("BGMax"), m_params.bgMax);
	
	for (i=1; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
	{
		keyName.Format(_T("GoldenY%d"), i);
		WriteFloat(sectionName, keyName, m_params.yGolden[i]);
	}
	
	for (i=1; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
	{
		keyName.Format(_T("GoldenRG%d"), i);
		WriteFloat(sectionName, keyName, m_params.rgGolden[i]);
	}
	
	for (i=1; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
	{
		keyName.Format(_T("GoldenBG%d"), i);
		WriteFloat(sectionName, keyName, m_params.bgGolden[i]);
	}

	WriteFloat(sectionName, _T("Rel_YMax"), m_params.yRelMax);
	WriteFloat(sectionName, _T("Rel_RGMax"), m_params.rgRelMax);
	WriteFloat(sectionName, _T("Rel_BGMax"), m_params.bgRelMax);
}

void* CShadingQualcomm::GetValues(void)
{
	return &m_values;
}

void* CShadingQualcomm::GetParams(void)
{
	return &m_params;
}

/*
B1          B4
      B0
B2          B3
*/
void CShadingQualcomm::SetParams(UINT imageWidth, UINT imageHeight)
{
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

	m_params.width = blockWidth;
	m_params.height= blockHeight;

	m_params.yMax = 100;
	m_params.yMin = 0;
	m_params.yDif = 5;
	m_params.rgMax = 100;
	m_params.bgMax = 100;

	m_params.yGolden[0] = 100;
	m_params.yGolden[1] = 100;
	m_params.yGolden[2] = 100;
	m_params.yGolden[3] = 100;
	m_params.yGolden[4] = 100;

	m_params.rgGolden[0] = 100;
	m_params.rgGolden[1] = 100;
	m_params.rgGolden[2] = 100;
	m_params.rgGolden[3] = 100;
	m_params.rgGolden[4] = 100;

	m_params.bgGolden[0] = 100;
	m_params.bgGolden[1] = 100;
	m_params.bgGolden[2] = 100;
	m_params.bgGolden[3] = 100;
	m_params.bgGolden[4] = 100;
	
	m_params.yRelMax  = 7.5;
	m_params.rgRelMax = 4;
	m_params.bgRelMax = 4;
}

void CShadingQualcomm::CalcValues(BYTE *imageData, UINT imageWidth, UINT imageHeight, BYTE OB)
{
	memset(&m_values, 0, sizeof(m_values));	

	for (int i=0; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
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

	m_values.rg[0] = 1.0f * m_values.avg[0].r / m_values.avg[0].g;
	m_values.bg[0] = 1.0f * m_values.avg[0].b / m_values.avg[0].g;
	
	m_values.yMax  = 0;
	m_values.yMin  = 100;
	m_values.rgMax = 0;
	m_values.bgMax = 0;
	
	for (int j=1; j<SHADING_QUALCOMM_BLOCK_NUM; j++)
	{
		m_values.yRatio[j] = 100.0f * m_values.avg[j].y / m_values.avg[0].y;
		m_values.rg[j] = 100.0f * (1.0f* m_values.avg[j].r / m_values.avg[j].g) / m_values.rg[0];
		m_values.bg[j] = 100.0f * (1.0f* m_values.avg[j].b / m_values.avg[j].g) / m_values.bg[0];
		
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
	
// 	m_values.yMax = max4(m_values.yRatio[1], m_values.yRatio[2], m_values.yRatio[3], m_values.yRatio[4]);
// 	m_values.yMin = min4(m_values.yRatio[1], m_values.yRatio[2], m_values.yRatio[3], m_values.yRatio[4]);	
	m_values.yDif = m_values.yMax - m_values.yMin;

	m_values.rg[0] = 100;
	m_values.bg[0] = 100;	

	m_values.yRelMax  = 0;
	m_values.rgRelMax = 0;
	m_values.bgRelMax = 0;
	
	for (int k=1; k<SHADING_QUALCOMM_BLOCK_NUM; k++)
	{
		m_values.yRel[k] = 100.0f * (m_values.yRatio[k] / m_params.yGolden[k] -1);
		m_values.rgRel[k] = 100.0f * (m_values.rg[k] / m_params.rgGolden[k] -1);
		m_values.bgRel[k] = 100.0f * (m_values.bg[k] / m_params.bgGolden[k] -1);
		
		if (fabs(m_values.yRelMax) < fabs(m_values.yRel[k]))
		{
			m_values.yRelMax = m_values.yRel[k];
		}
		
		if (fabs(m_values.rgRelMax) < fabs(m_values.rgRel[k]))
		{
			m_values.rgRelMax = m_values.rgRel[k];
		}
		
		if (fabs(m_values.bgRelMax) < fabs(m_values.bgRel[k]))
		{
			m_values.bgRelMax = m_values.bgRel[k];
		}
	}
}

BYTE CShadingQualcomm::CheckPass(void)
{ 
	BYTE result = SHADING_RESULT_PASS;
	
	if (m_values.yDif > m_params.yDif)
	{
		result |= SHADING_YDIF_ERROR;
	}
	
	if (fabs(m_values.yRelMax) > m_params.yRelMax)
	{
		result |= SHADING_Y_ERROR;
	}
	
	if (fabs(m_values.rgRelMax) > m_params.rgRelMax)
	{
		result |= SHADING_RG_ERROR;
	}

	if (fabs(m_values.bgRelMax) > m_params.bgRelMax)
	{
		result |= SHADING_BG_ERROR;
	}

	return result;
}

void CShadingQualcomm::ShowTestInfo(void)
{	
	CString info;
	CString pos[5] = {_T("C "), _T("TL"), _T("BL"), _T("BR"), _T("TR")};
	
	AddInfo(_T("      Y      R      G      B"));
	
	for (int i=0; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
	{
		info.Format(_T("%s: %3d   %3d   %3d   %3d"), 
			pos[i], m_values.avg[i].y, m_values.avg[i].r, m_values.avg[i].g, m_values.avg[i].b);
		AddInfo(info);
	}
	
	AddInfo(_T("    ¡÷Y     ¡÷RG     ¡÷BG"));
	
	for (int j=1; j<SHADING_QUALCOMM_BLOCK_NUM; j++)
	{
		info.Format(_T("%s: %5.2f   %5.2f   %5.2f"), 
			pos[j], m_values.yRatio[j], m_values.rg[j], m_values.bg[j]);
		AddInfo(info);
	}
	
	info.Format(_T("Ymax: %5.2f,  Ymin: %5.2f"), m_values.yMax, m_values.yMin);
	AddInfo(info);
	
	info.Format(_T("Ydif: %5.2f"), m_values.yDif);
	AddInfo(info);
	
	info.Format(_T("R/G: %5.2f,  B/G: %5.2f"), m_values.rgMax, m_values.bgMax);
	AddInfo(info);

	AddInfo(_T("    Rel_Y   Rel_RG   Rel_BG"));

	for (int k=1; k<SHADING_QUALCOMM_BLOCK_NUM; k++)
	{
		info.Format(_T("%s: %5.2f   %5.2f   %5.2f"), 
			pos[k], m_values.yRel[k], m_values.rgRel[k], m_values.bgRel[k]);
		AddInfo(info);
	}
	
	info.Format(_T("Rel_Y: %5.2f"), m_values.yRelMax);
	AddInfo(info);
	
	info.Format(_T("Rel_R/G: %5.2f"), m_values.rgRelMax);
	AddInfo(info);

	info.Format(_T("Rel_B/G: %5.2f"), m_values.bgRelMax);
	AddInfo(info);
}

BOOL CShadingQualcomm::SaveTestFile(void)
{
	CString fname = GetReportFilePath() + GetReportFileName();				

	CString headers = _T("Module NO., Y_TL, Y_BL, Y_BR, Y_TR, RG_TL, RG_BL, RG_BR, RG_TR, BG_TL, BG_BL, BG_BR, BG_TR, Ydif, R/Gmax, B/Gmax, \
		Rel_Y_TL, Rel_Y_BL, Rel_Y_BR, Rel_Y_TR, Rel_RG_TL, Rel_RG_BL, Rel_RG_BR, Rel_RG_TR, Rel_BG_TL, Rel_BG_BL, Rel_BG_BR, Rel_BG_TR, \
		Rel_Ymax, Rel_R/Gmax, Rel_B/Gmax");			

	CString contents = GetModuleID() + _T(", ");

	CString tmpContents = _T("");
	int i;

	for (i=1; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
	{
		tmpContents.Format(_T("%.2f, "), m_values.yRatio[i]);
		contents += tmpContents;
	}

	for (i=1; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
	{
		tmpContents.Format(_T("%.2f, "), m_values.rg[i]);
		contents += tmpContents;
	}
	
	for (i=1; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
	{
		tmpContents.Format(_T("%.2f, "), m_values.bg[i]);
		contents += tmpContents;
	}
	
	tmpContents.Format(_T("%.2f, %.2f, %.2f, "), m_values.yDif, m_values.rgMax, m_values.bgMax);
	contents += tmpContents;
	
	for (i=1; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
	{
		tmpContents.Format(_T("%.2f, "), m_values.yRel[i]);
		contents += tmpContents;
	}
	
	for (i=1; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
	{
		tmpContents.Format(_T("%.2f, "), m_values.rgRel[i]);
		contents += tmpContents;
	}
	
	for (i=1; i<SHADING_QUALCOMM_BLOCK_NUM; i++)
	{
		tmpContents.Format(_T("%.2f, "), m_values.bgRel[i]);
		contents += tmpContents;
	}
	
	tmpContents.Format(_T("%.2f, %.2f, %.2f"), m_values.yRelMax, m_values.rgRelMax, m_values.bgRelMax);
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



