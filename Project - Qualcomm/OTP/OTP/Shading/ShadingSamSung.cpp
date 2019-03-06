#include "stdafx.h"
#include "ShadingSamSung.h"


CShadingSamSung::CShadingSamSung()
{
	memset(&m_values, 0, sizeof(m_values));	
	memset(&m_params, 0, sizeof(m_params));	
	
	SetOptionName(_T("_SetShadingSamSung"));
	SetReportFileName(_T("LencSamSung.csv"));
}

CShadingSamSung::~CShadingSamSung()
{
	if (IsOptionLoaded())
	{
		SaveOptions();
	}
}

void CShadingSamSung::LoadOptions(void)
{
	CShading::LoadOptions();
	
	CString sectionName = GetOptionName();
	
	for (int i=0; i<SHADING_SAMSUNG_BLOCK_NUM; i++)
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
	m_params.rgMax = ReadFloat(sectionName, _T("RGMax"), m_params.rgMax);
	m_params.bgMax = ReadFloat(sectionName, _T("BGMax"), m_params.bgMax);
	m_params.colorDif = ReadFloat(sectionName, _T("ColorDif"), m_params.colorDif);
}

void CShadingSamSung::SaveOptions(void)
{
	CShading::SaveOptions();
	
	CString sectionName = GetOptionName();
	
	for (int i=0; i<SHADING_SAMSUNG_BLOCK_NUM; i++)
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
	WriteFloat(sectionName, _T("RGMax"), m_params.rgMax);
	WriteFloat(sectionName, _T("BGMax"), m_params.bgMax);
	WriteFloat(sectionName, _T("ColorDif"), m_params.colorDif);
}

void* CShadingSamSung::GetValues(void)
{
	return &m_values;
}

void* CShadingSamSung::GetParams(void)
{
	return &m_params;
}

/*
0    1    2    3    4    5    6
7    8    9    10   11   12   13
14   15   16   17   18   19   20
21   22   23   24   25   26   27
28   29   30   31   32   33   34
35   36   37   38   39   40   41
42   43   44   45   46   47   48
*/
void CShadingSamSung::SetParams(UINT imageWidth, UINT imageHeight)
{
	UINT blockWidth = UINT(min(imageWidth, imageHeight) / 32.0 +0.5);
	if ((blockWidth % 2) != 0)
	{
		blockWidth++;
	}
	
	UINT blockHeight = blockWidth;
		
	m_params.block[0].x  = blockWidth /4 +1;
	m_params.block[0].y  = blockHeight/4 +1;
	m_params.block[1].x  = imageWidth/6 - blockWidth/2;
	m_params.block[1].y  = m_params.block[0].y;
	m_params.block[2].x  = imageWidth/3 - blockWidth/2;
	m_params.block[2].y  = m_params.block[0].y;
	m_params.block[3].x  = imageWidth/2 - blockWidth/2;
	m_params.block[3].y  = m_params.block[0].y;
	m_params.block[4].x  = imageWidth*2/3 - blockWidth/2;
	m_params.block[4].y  = m_params.block[0].y;
	m_params.block[5].x  = imageWidth*5/6 - blockWidth/2;
	m_params.block[5].y  = m_params.block[0].y;
	m_params.block[6].x  = imageWidth - blockWidth - m_params.block[0].x;
	m_params.block[6].y  = m_params.block[0].y;
	m_params.block[7].x  = m_params.block[0].x;		
	m_params.block[7].y  = imageHeight/6 - blockHeight/2;
	m_params.block[8].x  = m_params.block[1].x;
	m_params.block[8].y  = m_params.block[7].y;
	m_params.block[9].x  = m_params.block[2].x;
	m_params.block[9].y  = m_params.block[7].y;
	m_params.block[10].x = m_params.block[3].x;
	m_params.block[10].y = m_params.block[7].y;
	m_params.block[11].x = m_params.block[4].x;
	m_params.block[11].y = m_params.block[7].y;
	m_params.block[12].x = m_params.block[5].x;
	m_params.block[12].y = m_params.block[7].y;
	m_params.block[13].x = m_params.block[6].x;
	m_params.block[13].y = m_params.block[7].y;
	m_params.block[14].x = m_params.block[0].x;
	m_params.block[14].y = imageHeight/3 - blockHeight/2;
	m_params.block[15].x = m_params.block[1].x;
	m_params.block[15].y = m_params.block[14].y;
	m_params.block[16].x = m_params.block[2].x;
	m_params.block[16].y = m_params.block[14].y;
	m_params.block[17].x = m_params.block[3].x;
	m_params.block[17].y = m_params.block[14].y;
	m_params.block[18].x = m_params.block[4].x;
	m_params.block[18].y = m_params.block[14].y;
	m_params.block[19].x = m_params.block[5].x;
	m_params.block[19].y = m_params.block[14].y;
	m_params.block[20].x = m_params.block[6].x;
	m_params.block[20].y = m_params.block[14].y;
	m_params.block[21].x = m_params.block[0].x;		
	m_params.block[21].y = imageHeight/2 - blockHeight/2;
	m_params.block[22].x = m_params.block[1].x;
	m_params.block[22].y = m_params.block[21].y;
	m_params.block[23].x = m_params.block[2].x;
	m_params.block[23].y = m_params.block[21].y;
	m_params.block[24].x = m_params.block[3].x;
	m_params.block[24].y = m_params.block[21].y;
	m_params.block[25].x = m_params.block[4].x;
	m_params.block[25].y = m_params.block[21].y;
	m_params.block[26].x = m_params.block[5].x;
	m_params.block[26].y = m_params.block[21].y;
	m_params.block[27].x = m_params.block[6].x;
	m_params.block[27].y = m_params.block[21].y;
	m_params.block[28].x = m_params.block[0].x;
	m_params.block[28].y = imageHeight*2/3 - blockHeight/2;
	m_params.block[29].x = m_params.block[1].x;
	m_params.block[29].y = m_params.block[28].y;
	m_params.block[30].x = m_params.block[2].x;
	m_params.block[30].y = m_params.block[28].y;
	m_params.block[31].x = m_params.block[3].x;
	m_params.block[31].y = m_params.block[28].y;
	m_params.block[32].x = m_params.block[4].x;
	m_params.block[32].y = m_params.block[28].y;
	m_params.block[33].x = m_params.block[5].x;
	m_params.block[33].y = m_params.block[28].y;
	m_params.block[34].x = m_params.block[6].x;
	m_params.block[34].y = m_params.block[28].y;
	m_params.block[35].x = m_params.block[0].x;
	m_params.block[35].y = imageHeight*5/6 - blockHeight/2;
	m_params.block[36].x = m_params.block[1].x;
	m_params.block[36].y = m_params.block[35].y;
	m_params.block[37].x = m_params.block[2].x;
	m_params.block[37].y = m_params.block[35].y;
	m_params.block[38].x = m_params.block[3].x;
	m_params.block[38].y = m_params.block[35].y;
	m_params.block[39].x = m_params.block[4].x;
	m_params.block[39].y = m_params.block[35].y;
	m_params.block[40].x = m_params.block[5].x;
	m_params.block[40].y = m_params.block[35].y;
	m_params.block[41].x = m_params.block[6].x;
	m_params.block[41].y = m_params.block[35].y;
	m_params.block[42].x = m_params.block[0].x;
	m_params.block[42].y = imageHeight - blockHeight - m_params.block[0].y;
	m_params.block[43].x = m_params.block[1].x;
	m_params.block[43].y = m_params.block[42].y;
	m_params.block[44].x = m_params.block[2].x;
	m_params.block[44].y = m_params.block[42].y;
	m_params.block[45].x = m_params.block[3].x;
	m_params.block[45].y = m_params.block[42].y;
	m_params.block[46].x = m_params.block[4].x;
	m_params.block[46].y = m_params.block[42].y;
	m_params.block[47].x = m_params.block[5].x;
	m_params.block[47].y = m_params.block[42].y;
	m_params.block[48].x = m_params.block[6].x;
	m_params.block[48].y = m_params.block[42].y;

	m_params.width = blockWidth;
	m_params.height= blockHeight;

	m_params.yMax = 100;
	m_params.yMin = 60;
	m_params.yDif = 15;
	m_params.rgMax = 8;
	m_params.bgMax = 8;
	m_params.colorDif = 12;
}

void CShadingSamSung::CalcValues(BYTE *imageData, UINT imageWidth, UINT imageHeight, BYTE OB)
{
	memset(&m_values, 0, sizeof(m_values));	

	for (int i=0; i<SHADING_SAMSUNG_BLOCK_NUM; i++)
	{
		RGBStruct avg;
		GetRoiAverage_RGB(&avg, 
			CRect(CPoint(m_params.block[i].x, m_params.block[i].y), CSize(m_params.width, m_params.height)), 	
			imageData, imageWidth, imageHeight);
		
		m_values.avg[i].r = avg.r;// - OB;
		m_values.avg[i].g = avg.g;// - OB;
		m_values.avg[i].b = avg.b;// - OB;
		
		RGB_Y(m_values.avg[i].y, m_values.avg[i].r, m_values.avg[i].g, m_values.avg[i].b);
	}
	
	m_values.yMax  = 0;
	m_values.yMin  = 100;
	m_values.rgMax = 0;
	m_values.bgMax = 0;

	float colorMax = 0;
	float colorMin = 100;
	
	for (int j=0; j<SHADING_SAMSUNG_BLOCK_NUM; j++)
	{
		m_values.yRatio[j] = 100.0f * m_values.avg[j].y / m_values.avg[24].y;
		m_values.rRatio[j] = 100.0f * m_values.avg[j].r / m_values.avg[24].r;
		m_values.gRatio[j] = 100.0f * m_values.avg[j].g / m_values.avg[24].g;
		m_values.bRatio[j] = 100.0f * m_values.avg[j].b / m_values.avg[24].b;
		
		float rg = m_values.rRatio[j] - m_values.gRatio[j];
		float bg = m_values.bRatio[j] - m_values.gRatio[j];

		if (fabs(m_values.rgMax) < fabs(rg))
		{
			m_values.rgMax = rg;
		}

		if (fabs(m_values.bgMax) < fabs(bg))
		{
			m_values.bgMax = bg;
		}

		colorMax = max(colorMax, rg);
		colorMax = max(colorMax, bg);

		colorMin = min(colorMin, rg);
		colorMin = min(colorMin, bg);
	}

	m_values.colorDif = colorMax - colorMin;
	
	m_values.yMax = max(m_values.yRatio[0], m_values.yRatio[6]);
	m_values.yMax = max(m_values.yMax, m_values.yRatio[42]);
	m_values.yMax = max(m_values.yMax, m_values.yRatio[48]);

	m_values.yMin = min(m_values.yRatio[0], m_values.yRatio[6]);
	m_values.yMin = min(m_values.yMin, m_values.yRatio[42]);
	m_values.yMin = min(m_values.yMin, m_values.yRatio[48]);
	
	m_values.yDif = m_values.yMax - m_values.yMin;
}

BYTE CShadingSamSung::CheckPass(void)
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

	if (fabs(m_values.colorDif) > m_params.colorDif)
	{
		result |= SHADING_COLORDIF_ERROR;
	}
	
	return result;
}

void CShadingSamSung::ShowTestInfo(void)
{	
	CString info;

	AddInfo(_T("      Y      R      G      B"));
	
/*	for (int i=0; i<SHADING_SAMSUNG_BLOCK_NUM; i++)
	{
		info.Format(_T("%d: %3d   %3d   %3d   %3d"), 
			i,m_values.avg[i].y,m_values.avg[i].r,m_values.avg[i].g,m_values.avg[i].b);
		AddInfo(info);
	}
	
	AddInfo(_T("    ¡÷Y     ¡÷R     ¡÷G     ¡÷B"));
	
	for (i=0; i<SHADING_SAMSUNG_BLOCK_NUM; i++)
	{
		info.Format(_T("%d: %5.2f   %5.2f   %5.2f   %5.2f"), 
			i,m_values.yRatio[i],m_values.rRatio[i],m_values.gRatio[i],m_values.bRatio[i]);
		AddInfo(info);
	}
*/	
	info.Format(_T("Ymin=%.2f, Ymax=%.2f"),m_values.yMin,m_values.yMax);
	AddInfo(info);
	
	info.Format(_T("Ydif=%.2f"),m_values.yDif);
	AddInfo(info);
	
	info.Format(_T("R/G=%.2f, B/G=%.2f"),m_values.rgMax,m_values.bgMax);
	AddInfo(info);	
	
	info.Format(_T("P-P=%.2f"),m_values.colorDif);
	AddInfo(info);
}

BOOL CShadingSamSung::SaveTestFile(void)
{
	CString fname = GetReportFilePath() + GetReportFileName();				
	CString headers = _T("Module NO., Ymin, Ymax, Ydif, R/Gmax, B/Gmax, P-P");		
	
	CString contents;
	contents.Format(_T("%s, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f"), 
		GetModuleID(),m_values.yMin,m_values.yMax,m_values.yDif,m_values.rgMax,m_values.bgMax,m_values.colorDif);
	
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



