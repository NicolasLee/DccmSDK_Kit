#include "StdAfx.h"
#include "ShadingCalc.h"


ShadingCalc::ShadingCalc(DccmControl_Interface* pInterface, const int roiCnt)
{
	m_roiCnt = roiCnt;
	pROI = new ShadingROI_Info[m_roiCnt];
	m_pInterface = pInterface;
}


ShadingCalc::~ShadingCalc(void)
{
	SAFE_DELETE_ARRAY(pROI);
}

void ShadingCalc::InitMember()
{
	switch(m_roiCnt)
	{
	case 5://shading5
		pROI[0].cx = 50;
		pROI[0].cy = 50;
		pROI[0].Enable = 1;

		pROI[1].cx = 5;
		pROI[1].cy = 5;
		pROI[1].Enable = 1;

		pROI[2].cx = 5;
		pROI[2].cy = 95;
		pROI[2].Enable = 1;

		pROI[3].cx = 95;
		pROI[3].cy = 5;
		pROI[3].Enable = 1;

		pROI[4].cx = 95;
		pROI[4].cy = 95;
		pROI[4].Enable = 1;

		m_ROI_height   = 10;
		m_ROI_width    = 10;
		break;
	case 14://shading14
		pROI[0].cx  = 50;
		pROI[0].cy  = 50;
		pROI[1].cx  = 10;
		pROI[1].cy  = 10;
		pROI[2].cx  = 50;
		pROI[2].cy  = 10;
		pROI[3].cx  = 90;
		pROI[3].cy  = 10;
		pROI[4].cx  = 10;
		pROI[4].cy  = 50;
		pROI[5].cx  = 50;
		pROI[5].cy  = 50;
		pROI[6].cx  = 90;
		pROI[6].cy  = 50;
		pROI[7].cx  = 10;
		pROI[7].cy  = 90;
		pROI[8].cx  = 50;
		pROI[8].cy  = 90;
		pROI[9].cx  = 90;
		pROI[9].cy  = 90;
		pROI[10].cx = 30;
		pROI[10].cy = 30;
		pROI[11].cx = 70;
		pROI[11].cy = 30;
		pROI[12].cx = 30;
		pROI[12].cy = 70;
		pROI[13].cx = 70;
		pROI[13].cy = 70;

		m_ROI_height= 10;
		m_ROI_width = 10;
		break;
	case 25:

		break;
	case 49: //shading 49
		pROI[0].cx = 50;
		pROI[0].cy = 50;
		pROI[1].cx  = 2 ;
		pROI[1].cy  = 2 ;
		pROI[2].cx  = 18;
		pROI[2].cy  = 2 ;
		pROI[3].cx  = 34;
		pROI[3].cy  = 2 ;
		pROI[4].cx  = 50;
		pROI[4].cy  = 2 ;
		pROI[5].cx  = 66;
		pROI[5].cy  = 2 ;
		pROI[6].cx  = 82;
		pROI[6].cy  = 2 ;
		pROI[7].cx  = 98;
		pROI[7].cy  = 2 ;
		pROI[8].cx  = 2 ;
		pROI[8].cy  = 18;
		pROI[9].cx  = 18;
		pROI[9].cy  = 18;
		pROI[10].cx = 34;
		pROI[10].cy = 18;
		pROI[11].cx = 50;
		pROI[11].cy = 18;
		pROI[12].cx = 66;
		pROI[12].cy = 18;
		pROI[13].cx = 82;
		pROI[13].cy = 18;
		pROI[14].cx = 98;
		pROI[14].cy = 18;
		pROI[15].cx = 2 ;
		pROI[15].cy = 34;
		pROI[16].cx = 18;
		pROI[16].cy = 34;
		pROI[17].cx = 34;
		pROI[17].cy = 34;
		pROI[18].cx = 50;
		pROI[18].cy = 34;
		pROI[19].cx = 66;
		pROI[19].cy = 34;
		pROI[20].cx = 82;
		pROI[20].cy = 34;    
		pROI[21].cx = 98;
		pROI[21].cy = 34;
		pROI[22].cx = 2 ;
		pROI[22].cy = 50;
		pROI[23].cx = 18;
		pROI[23].cy = 50;
		pROI[24].cx = 34;
		pROI[24].cy = 50;
		pROI[25].cx = 66;
		pROI[25].cy = 50;
		pROI[26].cx = 82;
		pROI[26].cy = 50;
		pROI[27].cx = 98;
		pROI[27].cy = 50;
		pROI[28].cx = 2 ;
		pROI[28].cy = 66;
		pROI[29].cx = 18;
		pROI[29].cy = 66;
		pROI[30].cx = 34;
		pROI[30].cy = 66;
		pROI[31].cx = 50;
		pROI[31].cy = 66;
		pROI[32].cx = 66;
		pROI[32].cy = 66;
		pROI[33].cx = 82;
		pROI[33].cy = 66;
		pROI[34].cx = 98;
		pROI[34].cy = 66;
		pROI[35].cx = 2 ;
		pROI[35].cy = 82;
		pROI[36].cx = 18;
		pROI[36].cy = 82;
		pROI[37].cx = 34;
		pROI[37].cy = 82;
		pROI[38].cx = 50;
		pROI[38].cy = 82;
		pROI[39].cx = 66;
		pROI[39].cy = 82;
		pROI[40].cx = 82;
		pROI[40].cy = 82;
		pROI[41].cx = 98;
		pROI[41].cy = 82;
		pROI[42].cx = 2 ;
		pROI[42].cy = 98;
		pROI[43].cx = 18;
		pROI[43].cy = 98;
		pROI[44].cx = 34;
		pROI[44].cy = 98;
		pROI[45].cx = 50;
		pROI[45].cy = 98;
		pROI[46].cx = 66;
		pROI[46].cy = 98;
		pROI[47].cx = 82;
		pROI[47].cy = 98;
		pROI[48].cx = 98;
		pROI[48].cy = 98;

		for (int i = 0; i < m_roiCnt; i++)
		{
			pROI->Enable = TRUE;
		}

		m_ROI_width= 3;
		m_ROI_height= 3;

	}
	m_deltaY_max = m_ymax;
	m_deltaY_min = m_ymin;
	m_Ydif_limit   = m_ydif;
	m_RGdif_limit  = m_rgdiff;
	m_BGdif_limit  = m_bgdiff;

}

void ShadingCalc::CalcImageRect(void)
{
	ImageInfo img  = m_pInterface->GetImage();
	int width  = img.width;
	int height = img.height;
	BYTE* Y_Buffer = img.Y_Buf;
	for(int i=0; i<m_roiCnt; i++)
	{
		if(pROI[i].Enable)
		{
			//����ת��Ϊʵ�����ص�
			pROI[i].pos = m_pInterface->CalcPercentRect_ToImage(pROI[i].cx,	pROI[i].cy,	m_ROI_width, m_ROI_height);
		}
	}
}

BOOL ShadingCalc::ShadingTest(void)
{
	CalcImageRect();
	ImageInfo img  = m_pInterface->GetImage();
	int width =  img.width;
	int height = img.height;

	BYTE* RGB24_Buffer = img.RGB24_Buf;

	//������
	for(int i=0;i<m_roiCnt;i++)
	{
		pROI[i].aveY = ImageProc::GetAveY_ROI_RGB24(RGB24_Buffer, width, height, pROI[i].pos);
		pROI[i].aveR = ImageProc::GetAveR_ROI_RGB24(RGB24_Buffer, width, height, pROI[i].pos);
		pROI[i].aveG = ImageProc::GetAveG_ROI_RGB24(RGB24_Buffer, width, height, pROI[i].pos);
		pROI[i].aveB = ImageProc::GetAveB_ROI_RGB24(RGB24_Buffer, width, height, pROI[i].pos);
	}

	double Ymax = 0;
	double Ymin = 255;

	double RGdif_max = 0;
	double BGdif_max = 0;

	double PPmin = 255;
	double PPmax = 0;

	pROI[0].deltaY = 100.0;
	pROI[0].deltaR = 100.0;
	pROI[0].deltaG = 100.0;
	pROI[0].deltaB = 100.0;

	pROI[0].deltaRGdif = 100*pROI[0].aveR/pROI[0].aveG; //ֱ����ʾR/Gֵ
	pROI[0].deltaBGdif = 100*pROI[0].aveB/pROI[0].aveG; //ֱ����ʾB/Gֵ
	m_vData.clear();
	for(int i=1;i<m_roiCnt;i++)
	{
		//��¼����
		pROI[i].deltaY = 1.0 * pROI[i].aveY / pROI[0].aveY * 100.0;
		pROI[i].deltaR = 1.0 * pROI[i].aveR / pROI[0].aveR * 100.0;
		pROI[i].deltaG = 1.0 * pROI[i].aveG / pROI[0].aveG * 100.0;
		pROI[i].deltaB = 1.0 * pROI[i].aveB / pROI[0].aveB * 100.0;
		m_vData.push_back(pROI[i].deltaY);

		pROI[i].deltaRGdif = fabs(pROI[i].deltaR - pROI[i].deltaG);
		pROI[i].deltaBGdif = fabs(pROI[i].deltaB - pROI[i].deltaG);
		//�ҵ����Ĳ���
		RGdif_max = max(RGdif_max, pROI[i].deltaRGdif);
		BGdif_max = max(BGdif_max, pROI[i].deltaBGdif);

		Ymax = max(pROI[i].deltaY, Ymax);
		Ymin = min(pROI[i].deltaY, Ymin);

		PPmax = max(PPmax, pROI[i].deltaRGdif);
		PPmax = max(PPmax, pROI[i].deltaBGdif);

		PPmin = min(PPmin, pROI[i].deltaRGdif);
		PPmin = min(PPmin, pROI[i].deltaBGdif);
	}
	//�õ����յ�ֵ
	m_Ydif_max  = Ymax - Ymin;
	m_RGdif_max = RGdif_max;
	m_BGdif_max = BGdif_max;
	m_PPdif_max = PPmax - PPmin;

	//��¼���
	m_deltaY_max = m_ymax;
	m_deltaY_min = m_ymin;
	m_Ydif_limit   = m_ydif;
	m_RGdif_limit  = m_rgdiff;
	m_BGdif_limit  = m_bgdiff;

	for(int i=1;i<m_roiCnt;i++)
	{	
		//�ж�deltaY�Ƿ񳬱�
		if(pROI[i].deltaY < m_deltaY_min || pROI[i].deltaY > m_deltaY_max)
			pROI[i].deltaY_result = FALSE;
		else
			pROI[i].deltaY_result = TRUE;
		//�ж�RG�Ƿ񳬱�
		if (abs(pROI[i].deltaRGdif) > m_RGdif_limit )
			pROI[i].RG_result = FALSE;
		else
			pROI[i].RG_result = TRUE;
		//�ж�BG�Ƿ񳬱�
		if (abs(pROI[i].deltaBGdif) > m_BGdif_limit) 
			pROI[i].BG_result = FALSE;
		else
			pROI[i].BG_result = TRUE;

		if (m_Ydif_max > m_Ydif_limit)
			m_Ydif_result = FALSE;
		else
			m_Ydif_result = TRUE;

		//�����һ�����꣬��pROI������NG��
		if (pROI[i].BG_result == FALSE||
			pROI[i].RG_result == FALSE||
			pROI[i].deltaY_result == FALSE||
			m_Ydif_result == FALSE)
		{
			pROI[i].result = FALSE;
		}
		else
		{
			pROI[i].result = TRUE;
		}
	}

	pROI[0].BG_result = TRUE; //0 Ĭ��OK
	pROI[0].RG_result = TRUE; //0 Ĭ��OK
	pROI[0].deltaY_result = TRUE; //0 Ĭ��OK
	pROI[0].result = TRUE; //0 Ĭ��OK

	//�����һ��pROI���NG,����������Ŀ���NG
	int ROI_FailCnt=0;
	for(int i=0; i<m_roiCnt; i++)
	{
		if (pROI[i].result == FALSE )
		{
			ROI_FailCnt++;
		}
	}

	if (ROI_FailCnt > 0)
	{
		ShowShadingTest(FALSE);
		return FALSE;
	}
	else
	{
		ShowShadingTest(TRUE);
		return TRUE;
	}
}


void ShadingCalc::ShowShadingTest(BOOL bResult)
{
	CRect DisplayWndRect;
	m_pInterface->GetRect_Display(&DisplayWndRect);

	//�õ�ϵͳĬ������
	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);

	//��ʼ��
	CDC* pDisplayWndMemDC = m_pInterface->GetMemDC_Display();
	pDisplayWndMemDC->SelectObject(&ft);	
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
	pDisplayWndMemDC->SetBkMode(TRANSPARENT);
	pDisplayWndMemDC->SetTextColor(RGB(150,255,255));

	COLORREF CurrentColor;

	for(int i=0;i<m_roiCnt;i++)
	{
		if(pROI[i].Enable)
		{
			if (pROI[i].result == FALSE)
				CurrentColor = COLOR_RED;
			else
				CurrentColor = COLOR_GREEN;

			CPen pen(PS_SOLID,2,CurrentColor);
			pDisplayWndMemDC->SelectObject(&pen);

			CRect rect;
			rect.left   =  (pROI[i].cx - m_ROI_width/2)  * DisplayWndRect.Width() /100;
			rect.right  =  (pROI[i].cx + m_ROI_width/2)  * DisplayWndRect.Width() /100;
			rect.top    =  (pROI[i].cy - m_ROI_height/2) * DisplayWndRect.Height()/100;
			rect.bottom =  (pROI[i].cy + m_ROI_height/2) * DisplayWndRect.Height()/100;

			pDisplayWndMemDC->Rectangle(rect);

			//���ַ�
			int x = pROI[i].cx * DisplayWndRect.Width()/100;
			int y = pROI[i].cy * DisplayWndRect.Height()/100;

			CString str;
			str.Format(_T("B%d"),i);
			pDisplayWndMemDC->TextOut(x-10,y-10,str);
		}
	}

	//��ʼ��������
	CDC* pInfoMemDC = m_pInterface->GetMemDC_Assistant();
	CRect rect;
	m_pInterface->GetRect_Assistant(&rect);
	pInfoMemDC->FillRect(&rect,&CBrush(RGB(160,160,160)));
	pInfoMemDC->SetBkMode(TRANSPARENT);
	pInfoMemDC->SelectObject(&ft);

	int x = 10;
	int y = 10;

	CString header = _T("NO.    Y     ��R    ��G    ��B    ��Y   R/Gdif  B/Gdif ");
	pInfoMemDC->SetTextColor(RGB(0,0,0));
	pInfoMemDC->TextOut(x, y, header);


	CPen pen(PS_SOLID,1,RGB(0,0,100));
	pInfoMemDC->SelectObject(&pen);
	pInfoMemDC->MoveTo(0,32);
	pInfoMemDC->LineTo(340,32);

	CString szSpace1 = _T("  ");
	CString szSpace2 = _T("    ");

	for(int i=0; i<m_roiCnt; i++)
	{
		if(pROI[i].Enable)
		{
			x = 10;
			y = 40 + i* 22 ;
			CString data;
			data.Format(_T("B%d    %d%s %.0lf%s  %.0lf%s  %.0lf"),i,
				pROI[i].aveY,
				pROI[i].aveY >= 100 ? szSpace1:szSpace2,
				pROI[i].deltaR,
				pROI[i].deltaR >= 100 ? szSpace1:szSpace2,
				pROI[i].deltaG,
				pROI[i].deltaG >= 100 ? szSpace1:szSpace2,
				pROI[i].deltaB);

			pInfoMemDC->SetTextColor(RGB(0,0,0));
			pInfoMemDC->TextOut(x, y, data);

			//����deltaYֵ��ʾֵ����ɫ
			CString deltaY;
			deltaY.Format(_T("%.1lf"),pROI[i].deltaY);
			if (pROI[i].deltaY_result == FALSE)
				CurrentColor = COLOR_RED;
			else
				CurrentColor = COLOR_GREEN;

			if (i==0)	CurrentColor = COLOR_BLACK; //0����Ĭ��Ϊ��

			pInfoMemDC->SetTextColor(CurrentColor);
			pInfoMemDC->TextOut(x + 190, y, deltaY);

			//����deltaRGdifֵ��ʾֵ����ɫ
			CString RG;
			RG.Format(_T("%.0lf"),pROI[i].deltaRGdif);
			if (pROI[i].RG_result == FALSE)
				CurrentColor = COLOR_RED;
			else
				CurrentColor = COLOR_GREEN;

			if (i==0)	CurrentColor = COLOR_BLACK;

			pInfoMemDC->SetTextColor(CurrentColor);
			pInfoMemDC->TextOut(x+240, y, RG);

			//����deltaBGdifֵ��ʾֵ����ɫ
			CString BG;
			BG.Format(_T("%.0lf"),pROI[i].deltaBGdif);
			if (pROI[i].BG_result == FALSE)
				CurrentColor = COLOR_RED;
			else
				CurrentColor = COLOR_GREEN;

			if (i==0)	CurrentColor = COLOR_BLACK;

			pInfoMemDC->SetTextColor(CurrentColor);
			pInfoMemDC->TextOut(x+285, y, BG);
		}
	}

	//���ָ���	
	pInfoMemDC->SetTextColor(COLOR_BLACK);
	pInfoMemDC->MoveTo(0,160);
	pInfoMemDC->LineTo(340,160);


	CString str;

	//��ʾ����
	str = _T("Max Ydif");
	pInfoMemDC->TextOut(10, 170, str); 
	//����MAX Ydif ֵ��ʾֵ����ɫ
	if (m_Ydif_max > m_Ydif_limit)
		CurrentColor = COLOR_RED;
	else
		CurrentColor = COLOR_GREEN;
	pInfoMemDC->SetTextColor(CurrentColor);
	str.Format(_T("%0.lf"),m_Ydif_max);
	pInfoMemDC->TextOut(90, 170, str);

	//��ʾ����
	str = _T("Max deltaRGdif");
	pInfoMemDC->SetTextColor(COLOR_BLACK);
	pInfoMemDC->TextOut(10, 195, str);
	//����MAX deltaRGdif ֵ��ʾֵ����ɫ
	if (m_RGdif_max > m_RGdif_limit)
		CurrentColor = COLOR_RED;
	else
		CurrentColor = COLOR_GREEN;
	pInfoMemDC->SetTextColor(CurrentColor);
	str.Format(_T("%0.lf"), m_RGdif_max);
	pInfoMemDC->TextOut(90, 195, str);


	//��ʾ����
	str = _T("Max deltaBGdif");	
	pInfoMemDC->SetTextColor(COLOR_BLACK);
	pInfoMemDC->TextOut(10, 220, str);
	//����MAX deltaBGdif ֵ��ʾֵ����ɫ
	if (m_BGdif_max > m_BGdif_limit)
		CurrentColor = COLOR_RED;
	else
		CurrentColor = COLOR_GREEN;
	pInfoMemDC->SetTextColor(CurrentColor);
	str.Format(_T("%0.lf"), m_BGdif_max);
	pInfoMemDC->TextOut(90, 220, str);

 	CString result;
	if (bResult == FALSE)
	{
		CurrentColor = COLOR_RED;
		result = _T("NG");
	}
	else
	{
		CurrentColor = COLOR_GREEN;
		result = _T("OK");
 	}

	CFont font2;
	font2.CreatePointFont(400,_T("Arial"));

	pInfoMemDC->SetTextColor(CurrentColor);
	pInfoMemDC->SelectObject(&font2);
	pInfoMemDC->TextOut(180, 180, result);

	pDisplayWndMemDC->SetTextColor(CurrentColor);
	pDisplayWndMemDC->SelectObject(&font2);
	pDisplayWndMemDC->TextOut(
		DisplayWndRect.Width()/2 - 150, 
		DisplayWndRect.Height()/2 + 50, 
		_T("Shading ") + result);

	//��ʾDC
	m_pInterface->ShowMemDC(pDisplayWndMemDC);
	m_pInterface->ShowMemDC(pInfoMemDC);
}