#include "stdafx.h"
#include "AutoFocusFar_Inspection.h"
#include "ImageProc.h"



#ifndef DCCM_EXP
    #define DCCM_EXP	__declspec(dllexport)
#endif	/* DCCM_EXP */


extern "C" DCCM_EXP Inspection*  CreateInspectionItem(DccmControl_Interface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new AutoFocusFar_Inspection(pInterface, lpszName);
}



AutoFocusFar_Inspection::~AutoFocusFar_Inspection(void)
{

}

AutoFocusFar_Inspection::AutoFocusFar_Inspection(DccmControl_Interface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	SetConfigPath(pInterface->GetConfigPathName());	//����CONFIG·��	
	SetName(lpszName);	                            //���ò�����Ŀ������
	pDlg = new OptionDlg(NULL,this);				//���ɶԻ������
	LoadOption();                                   //��ȡ����
	pDlg->Create(OptionDlg::IDD,NULL);				//�����Ի��򴰿�
	SetOptionDlgHandle(pDlg->GetSafeHwnd());        //�õ��Ի�����

	//TODO: ���ò�����Ŀ������ 
	SetType(TYPE_INFINITY);

	//�������Ա�����ʼ��
	m_bOneTimeLock = FALSE;
	m_pvStorage = NULL;
	ClearStorage(pDlg->ClearRecord);
}

int AutoFocusFar_Inspection::Initialize()
{
	Inspection::Initialize();
	//TODO 

	if(!m_bOneTimeLock)
	{
		for (int i = 0; i < MTF_ROI; i++)
		{		
			sMTF.ROI[i].MTF_Deque.clear();  // ��սڵ���Ϣ
			m_Curve[i].Init();
		}

		if (pDlg->ShowOriginalImg)
		{
			m_pInterface->ShowOriginalImage();
		}
		m_pInterface->GetDriverIC()->WriteAF(0); // code����
		Sleep(50);
		m_pInterface->GetDriverIC()->WriteAF(pDlg->StartCode); // code����
		Sleep(50);
		
		
		m_SearchParam[0] = SearchParam(pDlg->DescendTime1, pDlg->StepDiff1, pDlg->StepCode1);
		m_SearchParam[1] = SearchParam(pDlg->DescendTime2, pDlg->StepDiff2, pDlg->StepCode2);
		m_SearchParam[2] = SearchParam(pDlg->DescendTime3, pDlg->StepDiff3, pDlg->StepCode3);
		m_SearchParam[3] = SearchParam(pDlg->DescendTime4, pDlg->StepDiff4, pDlg->StepCode4);
		
		m_curSearchStep = 0;
		m_AF_Status = 0;
		// ��ѶԽ��㱣��
		m_bestAF_Code = -1;

		// �õ��洢�ռ�
		m_pvStorage = m_pInterface->GetMyStorage(this);
		ClearStorage(TRUE);

		// ͼ����Ϣ
		m_maxAxisY = pDlg->MAX_Y;
		m_maxAxisX = pDlg->MAX_X;
		
		DrawBackGround();
		m_bOneTimeLock = TRUE;

		CalcImageRect();
		m_bestAF_Code = 0;
		m_bestAF_Val = 0.0;
		m_maxMTF_Center = 0.0;
		m_minMTF_Center = 100.0;
		m_bOneTimeLog = TRUE;
	}
	
    FramePitch = 0; //֡�����0
	if (pDlg->ExpEnable)
	{	
		int nRet = m_pInterface->Exposure(pDlg->ExpTarget, pDlg->ExpTolerance);
		if(nRet == 0)
		{
			// �����ع�����
			int expTimeRead = 0;
			m_pInterface->GetImageSensor()->ReadExp(expTimeRead);
			m_pInterface->GetConfigManager()->WriteConfigInt(GetName(), L"RecordDllExp", expTimeRead);
			CString szTemp;
			szTemp.Format(L"dccf Write RecordDllExp: 0x%04X", expTimeRead);
			m_pInterface->AddDebugLog(szTemp, COLOR_BLUE);
			return RET_END;
		}
		else
			return RET_CONTIUNE;
	}
	return RET_END;

}

int AutoFocusFar_Inspection::Testing()
{
	m_bOneTimeLock = FALSE;
	int iRet = AutoFocusFarTest();
	if (iRet == 0)
		return RET_END;
	else
		return RET_CONTIUNE;
}

int AutoFocusFar_Inspection::Finalize()
{
	if (pDlg->SearchArea.Enable)
	{
		m_szStr.Format(_T("����������Code %d"), m_bestAF_Code);
		m_pInterface->AddLog(m_szStr, COLOR_GREEN);
		SetResult(RESULT_PASS);
		return RET_END;
	}

	m_bOneTimeLock = FALSE;
	// ������ѵ�
	if (GetResult() == RESULT_PASS)
	{
		m_pvStorage->push_back(m_bestAF_Code);
		m_pInterface->GetConfigManager()->WriteConfigInt(_T("AFBESTCODE"), _T("InfCode"), m_bestAF_Code);
		m_pInterface->AddLog(_T("Codeֵ�ѱ��浽�洢�ռ�"), COLOR_BLUE);
	}
	// ���� Codeֵ-������ ����
	if (pDlg->SaveAF_Curve)
	{
		CTime time = CTime::GetCurrentTime();
		CString szTime = time.Format(_T("%Y%m%d-%H%M%S"));
		m_szStr = m_pInterface->GetExeDir();
		m_szStr = m_szStr + _T("Report\\AutoFocusFar_Curve_") + szTime + _T(".csv");
		m_Curve[0].SaveCurve(m_szStr);
	}

	if (GetResult()== RESULT_FAIL)
	{
		m_szStr.Format(_T("Զ�� NG"));
		m_pInterface->AddLog(m_szStr, COLOR_RED);
	}
	else
	{
		m_szStr.Format(_T("Զ�� OK"));
		m_pInterface->AddLog(m_szStr, COLOR_GREEN);
	}

	if (pDlg->BackToInit)
		BackToInitPos();

	return RET_END;
}

int AutoFocusFar_Inspection::LoadOption()
{
	sMTF.ROI[0].cx = ReadConfigInt(_T("ROI0_x"),50);
	sMTF.ROI[0].cy = ReadConfigInt(_T("ROI0_y"),50);
	sMTF.ROI[0].Enable = ReadConfigInt(_T("ROI0_Enable"),1);
	sMTF.ROI[0].limit1 = ReadConfigInt(_T("ROI0_Limit1"),48);

	sMTF.ROI[1].cx = ReadConfigInt(_T("ROI1_x"),21);
	sMTF.ROI[1].cy = ReadConfigInt(_T("ROI1_y"),13);
	sMTF.ROI[1].Enable = ReadConfigInt(_T("ROI1_Enable"),0);
	sMTF.ROI[1].limit1 = ReadConfigInt(_T("ROI1_Limit1"),28);

	sMTF.ROI[2].cx = ReadConfigInt(_T("ROI2_x"),21);
	sMTF.ROI[2].cy = ReadConfigInt(_T("ROI2_y"),87);
	sMTF.ROI[2].Enable = ReadConfigInt(_T("ROI2_Enable"),0);
	sMTF.ROI[2].limit1 = ReadConfigInt(_T("ROI2_Limit1"),28);

	sMTF.ROI[3].cx = ReadConfigInt(_T("ROI3_x"),79);
	sMTF.ROI[3].cy = ReadConfigInt(_T("ROI3_y"),87);
	sMTF.ROI[3].Enable = ReadConfigInt(_T("ROI3_Enable"),0);
	sMTF.ROI[3].limit1 = ReadConfigInt(_T("ROI3_Limit1"),28);

	sMTF.ROI[4].cx = ReadConfigInt(_T("ROI4_x"),79);
	sMTF.ROI[4].cy = ReadConfigInt(_T("ROI4_y"),13);
	sMTF.ROI[4].Enable = ReadConfigInt(_T("ROI4_Enable"),0);
	sMTF.ROI[4].limit1 = ReadConfigInt(_T("ROI4_Limit1"),28);

	sMTF.ROI_height= ReadConfigInt(_T("ROI_height"),	10);
	sMTF.ROI_width = ReadConfigInt(_T("ROI_width"),		10);
	sMTF.surroundDiffLimit = ReadConfigInt(_T("SurroundDiffLimit"), 100);
	pDlg->CheckCornerDiff = ReadConfigInt(_T("CheckCornerDiff"), 0);

	pDlg->StartCode    = ReadConfigInt(_T("StartCode"), 200);
	pDlg->CodeLimitL  = ReadConfigInt(_T("CodeLimitL"), 100);
	pDlg->CodeLimitH  = ReadConfigInt(_T("CodeLimitH"), 500);


	pDlg->ExpEnable     = ReadConfigInt(_T("ExpEnable"), 1);
	pDlg->ExpTarget     = ReadConfigInt(_T("ExpTarget"), 130);
	pDlg->ExpTolerance  = ReadConfigInt(_T("ExpTolerance"),4);

	pDlg->AlgorithmType = ReadConfigInt(_T("AlgorithmType"), 0);
	pDlg->MAX_X         = ReadConfigInt(_T("MAX_X"), 1024);   
	pDlg->MAX_Y         = ReadConfigInt(_T("MAX_Y"), 100);

	pDlg->FramePitchLimit = ReadConfigInt(_T("FramePitchLimit"),2);


	pDlg->DescendTime1    = ReadConfigInt(_T("DescendTime1"),2);
	pDlg->DescendTime2    = ReadConfigInt(_T("DescendTime2"),1);
	pDlg->DescendTime3    = ReadConfigInt(_T("DescendTime3"),1);
	pDlg->DescendTime4    = ReadConfigInt(_T("DescendTime4"),1);

	pDlg->StepDiff1       = ReadConfigInt(_T("StepDiff1"),500)/100;
	pDlg->StepDiff2       = ReadConfigInt(_T("StepDiff2"),200)/100;
	pDlg->StepDiff3       = ReadConfigInt(_T("StepDiff3"),100)/100;
	pDlg->StepDiff4       = ReadConfigInt(_T("StepDiff4"),100)/100;

	pDlg->PosPrompt        = ReadConfigInt(_T("PosPrompt"), 0);

	pDlg->SearchArea.Enable		= ReadConfigInt(_T("SearchArea.Enable"),	0);
	pDlg->SearchArea.StartCode	= ReadConfigInt(_T("SearchArea.StartCode"),	100);
	pDlg->SearchArea.EndCode	= ReadConfigInt(_T("SearchArea.EndCode"),	800);
	pDlg->SearchArea.StepCode	= ReadConfigInt(_T("SearchArea.StepCode"),	60);
	pDlg->ClearRecord	   = ReadConfigInt(_T("ClearRecord"), 1);
	pDlg->SaveAF_Curve     = ReadConfigInt(_T("SaveAF_Curve"), 0);
	pDlg->ShowOriginalImg  = ReadConfigInt(_T("ShowOriginalImg"), 0);
	pDlg->ResolutionCheck  = ReadConfigInt(_T("ResolutionCheck"), 0);
	pDlg->BackToInit	   = ReadConfigInt(_T("BackToInit"), 0);
	pDlg->CheckGlue			= ReadConfigInt(_T("CheckGlue"), 0);

	pDlg->SearchTime      = ReadConfigInt(_T("SearchTime"),1);
	pDlg->StepCode1       = ReadConfigInt(_T("StepCode1"),40);
	pDlg->StepCode2       = ReadConfigInt(_T("StepCode2"),12);
	pDlg->StepCode3       = ReadConfigInt(_T("StepCode3"),4);
	pDlg->StepCode4       = ReadConfigInt(_T("StepCode4"),2);

	return Inspection::LoadOption();
}

int AutoFocusFar_Inspection::SaveOption()
{
	if (!Inspection::SaveOption())   return -1;
//TODO

	for(int i=0; i<5; i++)
	{
		CString KeyName;
		KeyName.Format(_T("ROI%d_x"),i);
		WriteConfigInt(KeyName,sMTF.ROI[i].cx);

		KeyName.Format(_T("ROI%d_y"),i);
		WriteConfigInt(KeyName,sMTF.ROI[i].cy);

		KeyName.Format(_T("ROI%d_Enable"),i);
		WriteConfigInt(KeyName,sMTF.ROI[i].Enable);

		KeyName.Format(_T("ROI%d_Limit1"),i);
		WriteConfigInt(KeyName,sMTF.ROI[i].limit1);
	}

	WriteConfigInt(_T("ROI_Width"), sMTF.ROI_width);
	WriteConfigInt(_T("ROI_Height"),sMTF.ROI_height);
	WriteConfigInt(_T("SurroundDiffLimit"),	sMTF.surroundDiffLimit);
	WriteConfigInt(_T("CheckCornerDiff"),pDlg->CheckCornerDiff);
	WriteConfigInt(_T("StartCode"),pDlg->StartCode);
	WriteConfigInt(_T("CodeLimitH"), pDlg->CodeLimitH);
	WriteConfigInt(_T("CodeLimitL"), pDlg->CodeLimitL);
	WriteConfigInt(_T("PosPrompt"), pDlg->PosPrompt);

    WriteConfigInt(_T("ExpEnable"),		pDlg->ExpEnable);
	WriteConfigInt(_T("ExpTarget"),		pDlg->ExpTarget);
	WriteConfigInt(_T("ExpTolerance"),	pDlg->ExpTolerance);
	WriteConfigInt(_T("AlgorithmType"),pDlg->AlgorithmType);
	WriteConfigInt(_T("MAX_X"),pDlg->MAX_X);
	WriteConfigInt(_T("MAX_Y"),pDlg->MAX_Y);
	WriteConfigInt(_T("FramePitchLimit"),pDlg->FramePitchLimit);

	WriteConfigInt(_T("SearchTime"), pDlg->SearchTime);
	WriteConfigInt(_T("StepCode1"), pDlg->StepCode1);
	WriteConfigInt(_T("StepCode2"), pDlg->StepCode2);
	WriteConfigInt(_T("StepCode3"), pDlg->StepCode3);
	WriteConfigInt(_T("StepCode4"), pDlg->StepCode4);

	WriteConfigInt(_T("DescendTime1"), pDlg->DescendTime1);
	WriteConfigInt(_T("DescendTime2"), pDlg->DescendTime2);
	WriteConfigInt(_T("DescendTime3"), pDlg->DescendTime3);
	WriteConfigInt(_T("DescendTime4"), pDlg->DescendTime4);

	WriteConfigInt(_T("StepDiff1"), int(pDlg->StepDiff1*100));
	WriteConfigInt(_T("StepDiff2"), int(pDlg->StepDiff2*100));
	WriteConfigInt(_T("StepDiff3"), int(pDlg->StepDiff3*100));
	WriteConfigInt(_T("StepDiff4"), int(pDlg->StepDiff4*100));

	WriteConfigInt(_T("ShowOriginalImg"), pDlg->ShowOriginalImg);
	WriteConfigInt(_T("ClearRecord"), pDlg->ClearRecord);
	WriteConfigInt(_T("SaveAF_Curve"), pDlg->SaveAF_Curve);
	WriteConfigInt(_T("ResolutionCheck"), pDlg->ResolutionCheck);
	WriteConfigInt(_T("BackToInit"), pDlg->BackToInit);
	WriteConfigInt(_T("CheckGlue"), pDlg->CheckGlue);

	WriteConfigInt(_T("SearchArea.Enable"), pDlg->SearchArea.Enable);
	WriteConfigInt(_T("SearchArea.StartCode"), pDlg->SearchArea.StartCode);
	WriteConfigInt(_T("SearchArea.EndCode"), pDlg->SearchArea.EndCode);
	WriteConfigInt(_T("SearchArea.StepCode"), pDlg->SearchArea.StepCode);
	return 0;
}

LPCTSTR AutoFocusFar_Inspection::GetReportHeaders()
{
	// TODO : Make report headers of this inspection and return that
	static CString header(_T("BestAFCode,B0,B1,B2,B3,B4"));

	return header;
}

LPCTSTR AutoFocusFar_Inspection::GetReportLowerLimits()
{
	// TODO : Make report lower limits of this inspection and return that

	return _T("");
}

LPCTSTR AutoFocusFar_Inspection::GetReportUpperLimits()
{
	// TODO : Make report upper limits of this inspection and return that

	return _T("");
}

LPCTSTR AutoFocusFar_Inspection::GetReportContents()
{
	// TODO : Make report contents of this inspection and return that
	static CString contents;
	contents.Format(_T("%d,%5.2f,%5.2f,%5.2f,%5.2f,%5.2f"),m_bestAF_Code,
		sMTF.ROI[0].MTFvalue,
		sMTF.ROI[1].MTFvalue,
		sMTF.ROI[2].MTFvalue,
		sMTF.ROI[3].MTFvalue,
		sMTF.ROI[4].MTFvalue);
	return contents;
}

int AutoFocusFar_Inspection::AF_BestCodeCheck(int index)
{
	FocusNode BackNode = m_Curve[index].GetBackNode();
	FocusNode MaxNode = m_Curve[index].FindMaxNode();

	// �ж����һ���ڵ��Ƿ�����������ͬһ���ڵ㣬��--����0 �����������������
	if (BackNode.m_Code == MaxNode.m_Code) 
		return 0;

	//������ǣ��ж����һ���ڵ��ֵ�Ƿ�����������ڵ��ֵ��
	if (BackNode.m_Value >= MaxNode.m_Value)
	{
		//��--����0 �����������������
		return 0;
	}
	else
	{
		return -1;
	}
}

int AutoFocusFar_Inspection::AutoFocusFarTest()
{
	USHORT AF_Code;
	USHORT usTempCode;
	if (m_pInterface->GetDriverIC()->ReadAF(&usTempCode))// ����ǰ��DIRIVER ICֵ
	{
		m_pInterface->AddLog(_T("��ȡCodeʧ�ܣ�DriverIC��������"), COLOR_RED);
		SetResult(RESULT_FAIL);
		return 0;
	}
	AF_Code = usTempCode;

	// ֡���                                                                                                                                                                                                    
	if(FramePitch < pDlg->FramePitchLimit)
	{
		FramePitch++;
		for (int i = 0; i < MTF_ROI; i++)
		{
			val[i] = GetROIValues(pDlg->AlgorithmType, sMTF.ROI[i].pos);
			sMTF.ROI[i].MTFvalue = val[i];
			if (sMTF.ROI[i].Enable)					
				ShowFocusRect(i);
		}
		// ��ʾ
		m_pInterface->ShowMemDC(m_pMemDC_Display);
		m_pInterface->ShowMemDC(m_pMemDC_Assistant);
		return 1;
	}
	else
	{
 		FramePitch = 0;
		for (int i = 0; i < MTF_ROI; i++)
		{
			val[i] = GetROIValues(pDlg->AlgorithmType, sMTF.ROI[i].pos);
			sMTF.ROI[i].MTFvalue = val[i];
			m_Curve[i].AddNode(val[i], AF_Code);	// �������ڵ���Ϣ
			if (sMTF.ROI[i].Enable)					
			{
				m_szStr.Format(_T("B%d "), i);
			//	m_pInterface->AddLog(m_szStr + m_Curve[i].OutputNodeInfo());	// ��ʾ��Ϣ
				m_pInterface->AddLog(m_Curve[i].OutputNodeInfo(m_szStr));	// ��ʾ��Ϣ
				// ��ʾAF����
				ShowCurve(&m_Curve[i].m_vFocusNode, i);
				ShowFocusRect(i);
			}
		}
		// ��ʾ
		m_pInterface->ShowMemDC(m_pMemDC_Display);
		m_pInterface->ShowMemDC(m_pMemDC_Assistant);
	}

	int index = 0;	// ������Ϊ��������
	switch(m_AF_Status)
	{
	case 0: // ��һ��д���ʼCODEֵ
		{
			int waitTime = 0;
			if (pDlg->SearchArea.Enable)
			{
				// ��ʼ��
				m_bestAF_Val = 0;

				AF_Code = pDlg->SearchArea.StartCode;
				waitTime = pDlg->SearchArea.StepCode/4;
				m_pInterface->AddLog(_T("�������������� ��ʼ"), COLOR_BLUE);
			}
			else
			{
				m_Curve[index].SetNextSearchTarget(m_SearchParam[m_curSearchStep].m_DescendTime, m_SearchParam[m_curSearchStep].m_Diff);
				m_curStepCode = m_SearchParam[m_curSearchStep].m_StepCode;

				AF_Code = pDlg->StartCode;
				waitTime = pDlg->StartCode/4;

				Sleep(waitTime);// ���ⵯƬ�𵴹������ʱ
			}

			m_AF_Status = 1;
			break;
		}
	case 1:
		{
			if (pDlg->SearchArea.Enable)
			{
				if (AF_Code <= pDlg->SearchArea.EndCode)
				{
					if (sMTF.ROI[0].MTFvalue > m_bestAF_Val)
					{
						m_bestAF_Code = AF_Code;
						m_bestAF_Val = sMTF.ROI[0].MTFvalue;
					}
					AF_Code += pDlg->SearchArea.StepCode;
				}
				else
				{
					m_pInterface->AddLog(_T("�������������� ����"), COLOR_BLUE);
					return RET_END;
				}
			}
			else
			{
				// ��ǰ�������������
				if (m_Curve[index].isSearchTargetReached())
				{
					//��һ���ڵ��Ƿ�����㣬����ǣ�����Խ�����������㡣��������
					if (m_Curve[index].GetPenultimateNode().m_bNewStartPoint)
					{
						m_curStepCode = -m_curStepCode;
						AF_Code += m_curStepCode;
						break;
					}
					// �õ���һ����������
					m_direction = m_Curve[index].GetNextSearchDirection();
					// �õ���һ��������ʼֵ
					AF_Code = m_Curve[index].GetNextSearchStartCode();
					// ������һ�ε�����Ŀ��
					m_curSearchStep++;				
					// �Ƿ������һ������
					if (m_curSearchStep > pDlg->SearchTime) // YES
					{
						AF_Code = m_Curve[index].GetBestCode();
						m_AF_Status = AF_EndFlag;
						break;
					}
					m_Curve[index].SetNextSearchTarget(m_SearchParam[m_curSearchStep].m_DescendTime, m_SearchParam[m_curSearchStep].m_Diff);
					// ������һ�εĲ���ֵ
					m_curStepCode = m_SearchParam[m_curSearchStep].m_StepCode * m_direction;
				}
				else //û��ɣ�������
				{
					AF_Code += m_curStepCode;
				}
			}
			break;
		}

	case AF_EndFlag://AF�����Ķ���
		{
			if (!AF_BestCodeCheck(index))
			{			
				m_bestAF_Code = AF_Code;
				if (!pDlg->ResolutionCheck)
				{
					SetResult(RESULT_PASS);
					return 0;
				}

				// �������������
				BOOL bRet = AF_EndResolutionAndCodeCheck();

				// Delta���
				if (pDlg->CheckCornerDiff)
				{
					if (!AF_CheckSurroundDiff())	return 0;
				}

				if (bRet)
					SetResult(RESULT_PASS);
				else
					SetResult(RESULT_FAIL);
				return 0;
			}
			else
			{
				m_pInterface->AddLog(_T("Fit Code incorrect"));
				AF_Code = m_Curve[index].FindMaxNode().m_Code;
				break;
			}
		}
	}

	if (AF_Code<0)
	{
		AF_Code = 0;
		m_AF_Status = AF_EndFlag;
	}

	if (AF_Code > 1023)
	{
		AF_Code = m_Curve[index].FindMaxNode().m_Code;
		m_AF_Status = AF_EndFlag;
	}

	if (pDlg->CheckGlue && pDlg->ResolutionCheck)
	{
		if (!AF_CheckGlue(AF_Code))
		{
			SetResult(RESULT_FAIL);
			return 0;
		}
	}

	if (m_pInterface->GetDriverIC()->WriteAF((USHORT)AF_Code))
	{
		m_pInterface->AddLog(_T("д��Codeʧ�ܣ�DriverIC��������"), COLOR_RED);
		SetResult(RESULT_FAIL);
		return 0;
	}
	return 1;
}

BOOL AutoFocusFar_Inspection::AF_EndResolutionAndCodeCheck()
{
	BOOL bRet = TRUE;
	BOOL Code_OK = FALSE, Resolution_OK = FALSE;
	CString szTemp;
	for (int i = 0; i < MTF_ROI; i++)
	{
		if (sMTF.ROI[i].Enable)
		{
			if ((m_bestAF_Code >= pDlg->CodeLimitL) && (m_bestAF_Code <= pDlg->CodeLimitH))
				Code_OK = TRUE;

			// ���������
			double val[5];
			val[i] = GetROIValues(pDlg->AlgorithmType, sMTF.ROI[i].pos);//�õ���������Ľ�����ֵ
			m_szStr.Format(_T("B%d ������ֵ�� %5.2lf%%��%d%%��  "), i, val[i], sMTF.ROI[i].limit1);
			if (val[i] >= sMTF.ROI[i].limit1) // �жϽ�����
			{
				Resolution_OK =  TRUE;
				szTemp.Format(_T("���������Խ����B%d OK"), i);
				m_szStr = m_szStr + szTemp;
				m_pInterface->AddLog(m_szStr, COLOR_GREEN);
			}
			else
			{
				Resolution_OK =  FALSE;
				sMTF.ROI[i].TestResult = FALSE;
				szTemp.Format(_T("���������Խ����B%d NG �޷��ﵽ��������׼"), i);
				m_szStr = m_szStr + szTemp;
				m_pInterface->AddLog(m_szStr, COLOR_RED);

				// ��ʾ������
				if (pDlg->PosPrompt)
				{
					if (m_Curve[i].GetBackNode().m_Code < pDlg->CodeLimitL)
						m_pInterface->AddLog(_T("��ͷ������"), COLOR_BLUE);
					if (m_Curve[i].GetBackNode().m_Code > pDlg->CodeLimitH)
						m_pInterface->AddLog(_T("��ͷ������"), COLOR_BLUE);
				}
			}
		}
	
		if (Code_OK && Resolution_OK) //Code���OK�ͽ�����OK ����PASS
			sMTF.ROI[i].TestResult = TRUE;

		bRet &= sMTF.ROI[i].TestResult;	// NGһ����NG
	}

	if (!Code_OK)	// �ж�Codeֵ
	{
		bRet = FALSE;
		m_szStr.Format(_T("����Codeֵ��%d��%d~%d�� Code���Խ���� NG ����Codeֵ��Χ"), m_bestAF_Code, pDlg->CodeLimitL, pDlg->CodeLimitH);
		m_pInterface->AddLog(m_szStr, COLOR_RED);
	}
	else
	{
		m_szStr.Format(_T("����Codeֵ��%d��%d~%d�� Code���Խ���� OK"), m_bestAF_Code, pDlg->CodeLimitL, pDlg->CodeLimitH);
		m_pInterface->AddLog(m_szStr, COLOR_GREEN);
	}

	return bRet;
}

BOOL AutoFocusFar_Inspection::AF_CheckSurroundDiff()
{
	BOOL bCheckAll = TRUE;
	for (int i = 0; i < MTF_ROI; i++)
	{
		if (!sMTF.ROI[i].Enable)
			bCheckAll = FALSE;
	}
	if (!bCheckAll)
	{
		m_pInterface->AddLog(_T("Ҫ�����Ľǲ��졣���ȹ�ѡ�ĽǼ�����������½��в���"), COLOR_RED);
		return FALSE;
	}
	
	double maxVal = 0, minVal = 100;
	for (int i = 1; i < 5; i++)
	{
		if (sMTF.ROI[i].Enable)
		{
			maxVal = max(maxVal, sMTF.ROI[i].MTFvalue);
			minVal = min(minVal, sMTF.ROI[i].MTFvalue);
		}
	}
	sMTF.SurroundDiff = maxVal - minVal;
	if (sMTF.SurroundDiff > sMTF.surroundDiffLimit)
	{
		m_szStr.Format(_T("�Ľǲ��쳬��: %5.2f"), sMTF.SurroundDiff);
		m_pInterface->AddLog(m_szStr, COLOR_RED);
		SetResult(RESULT_FAIL);
		return FALSE;
	}
	return TRUE;
}

int AutoFocusFar_Inspection::ShowCurve(vector<FocusNode>* Array, int penClr)
{
	CPen penY(PS_SOLID, 2, COLOR_YELLOW);
	CPen penB(PS_SOLID, 2, COLOR_BLUE);               
	CPen penO(PS_SOLID, 2, COLOR_ORANGE);   
	CPen penR(PS_SOLID, 2, COLOR_RED); 
	CPen penG(PS_SOLID, 2, COLOR_GREEN);              
	CPen penW(PS_SOLID, 2, COLOR_WHITE); 

	// �����ػ�
	CRect rect;
	m_pInterface->GetRect_Assistant(&rect);

	if (Array->back().m_Value > m_maxAxisY*0.9)
	{
		m_maxAxisY = int(Array->back().m_Value * 1.2);
		DrawBackGround(); // ����������仯�����ػ�����
	}
	if (Array->back().m_Code > m_maxAxisX*0.9)
	{
		m_maxAxisX = int(Array->back().m_Code * 1.2);
		DrawBackGround(); // ����������仯�����ػ�����
	}
//	DrawBackGround();	// ��������״̬�仯

	// ������Χ����ʾ��Ϣ
	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);	// �õ�ϵͳĬ������
	ft.Attach(hfont);
	m_pMemDC_Assistant->SelectObject(ft);
	m_pMemDC_Assistant->SetBkMode(TRANSPARENT);
	m_pMemDC_Assistant->SetTextColor(RGB(150,255,255));
	int fHeight = 15;
	int fWidth = 8;
	m_pMemDC_Assistant->TextOut(1, rect.bottom-fHeight, _T("0"));
	m_szStr.Format(_T("%d"), m_maxAxisY);
	m_pMemDC_Assistant->TextOut(1, 1, m_szStr);
	m_szStr.Format(_T("%d"), m_maxAxisX);
	m_pMemDC_Assistant->TextOut(rect.right-fWidth*4-1, rect.bottom-fHeight, m_szStr);

	switch(penClr)
	{
	case 0:
		m_pMemDC_Assistant->SelectObject(&penY);
		m_pMemDC_Assistant->SetTextColor(COLOR_YELLOW);
		m_pMemDC_Assistant->TextOut(rect.right-fWidth*4-1, 1, _T("- B0"));
		break;
	case 1:
		m_pMemDC_Assistant->SelectObject(&penW);
		m_pMemDC_Assistant->SetTextColor(COLOR_WHITE);
		m_pMemDC_Assistant->TextOut(rect.right-fWidth*4-1, 1+fHeight*1, _T("- B1"));
		break;
	case 2:
		m_pMemDC_Assistant->SelectObject(&penO);
		m_pMemDC_Assistant->SetTextColor(COLOR_ORANGE);
		m_pMemDC_Assistant->TextOut(rect.right-fWidth*4-1, 1+fHeight*2, _T("- B2"));
		break;
	case 3:
		m_pMemDC_Assistant->SelectObject(&penG);
		m_pMemDC_Assistant->SetTextColor(COLOR_GREEN);
		m_pMemDC_Assistant->TextOut(rect.right-fWidth*4-1, 1+fHeight*3, _T("- B3"));
		break;
	case 4:
		m_pMemDC_Assistant->SelectObject(&penB);
		m_pMemDC_Assistant->SetTextColor(COLOR_BLUE);
		m_pMemDC_Assistant->TextOut(rect.right-fWidth*4-1, 1+fHeight*4, _T("- B4"));
		break;
	}

	// ���߻���
	double hRatio = 1.0 * rect.Height()/m_maxAxisY;
	double vRatio = 1.0 * rect.Width()/m_maxAxisX;

	int x1,x2,y1,y2;
	for(UINT i = 1; i < Array->size(); i++)
	{
		//�������һ��������
		 y2 =  int(rect.Height() - Array->at(i).m_Value * hRatio);
		 x2 =  int(Array->at(i).m_Code*vRatio);
		//����ǰһ��������
		 y1 =  int(rect.Height() - Array->at(i-1).m_Value * hRatio);
		 x1 =  int(Array->at(i-1).m_Code*vRatio);

		m_pMemDC_Assistant->MoveTo(x1, y1);
		m_pMemDC_Assistant->LineTo(x2, y2);
	}

	if (m_AF_Status == AF_EndFlag)
	{
		m_pMemDC_Assistant->SelectObject(&penR);
		CBrush br(COLOR_RED);
		m_pMemDC_Assistant->SelectObject(&br);
		m_pMemDC_Assistant->Ellipse(x2-2, y2-2, x2+2, y2+2);
	}

	return 0;
}

void AutoFocusFar_Inspection::ShowFocusRect(int index)
{
	CPen penR(PS_SOLID, 2, COLOR_RED);                 //���û��ʣ��Խ������ɫ�ͱ߿���
	CPen penG(PS_SOLID, 2, COLOR_GREEN); 

	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);	// �õ�ϵͳĬ������
	ft.Attach(hfont);

	//********��ʾ�Խ���************//
	m_pMemDC_Display = m_pInterface->GetMemDC_Display();

	m_pMemDC_Display->SetBkMode(TRANSPARENT);        //����͸��ģʽ
	m_pMemDC_Display->SelectStockObject(NULL_BRUSH); //���ÿ�ˢ

	if (sMTF.ROI[index].MTFvalue > sMTF.ROI[index].limit1)
		m_pMemDC_Display->SelectObject(&penG);            
	else
		m_pMemDC_Display->SelectObject(&penR);           

	m_pMemDC_Display->SelectObject(ft);
	m_pMemDC_Display->SetTextColor(RGB(150,255,255));
	if (sMTF.ROI[index].Enable)
	{
		// ���㷽��Ĵ�С��λ��
		CRect AFRect = m_pInterface->CalcPercentRect_ImageToDisplay(sMTF.ROI[index].cx, sMTF.ROI[index].cy, sMTF.ROI_width, sMTF.ROI_height);
		m_pMemDC_Display->Rectangle(&AFRect);

		CRect rectShow;
		m_pInterface->GetRect_Display(&rectShow);
	
		m_szStr.Format(_T("B%d %.2f%%"), index, sMTF.ROI[index].MTFvalue);
		m_pMemDC_Display->TextOut((sMTF.ROI[index].cx-int(sMTF.ROI_width/2.4))*rectShow.Width()/100, sMTF.ROI[index].cy*rectShow.Height()/100, m_szStr);
	}

}

double AutoFocusFar_Inspection::GetROIValues(int Alg_type, CRect roiRect)
{
	ImageInfo img  = m_pInterface->GetImage();
	int width =  img.width;
	int height = img.height;
	BYTE* Y_Buffer = img.Y_Buf;

	double value = 0;
	switch(Alg_type)
	{
	case 0:
		value = ImageProc::GetMTF(Y_Buffer, width, height, roiRect)*100;
		break;
	case 1:
		value = ImageProc::GetResolution_FV(Y_Buffer, width, height, roiRect);
		break;
	}
	return value;
}

int AutoFocusFar_Inspection::DrawBackGround()
{
	// ͼ����Ϣ
	m_pMemDC_Assistant = m_pInterface->GetMemDC_Assistant();
	CRect rect;
	m_pInterface->GetRect_Assistant(&rect);
	m_pMemDC_Assistant->FillRect(&rect, &CBrush(COLOR_BLACK));
	m_pMemDC_Assistant->SetBkMode(TRANSPARENT);

	// ������
	int height = rect.Height();
	int width  = rect.Width();

	int hPitch = height/10 ;
	CPen ps(PS_DOT, 1, RGB(0,100,0));
	m_pMemDC_Assistant->SelectObject(&ps);

	for(int i= 0 ;i<=10 ;i++)
	{
		m_pMemDC_Assistant->MoveTo(0, height-i*hPitch);
		m_pMemDC_Assistant->LineTo(width, height-i*hPitch);
	}

	// ������
	int vPitch= width/10;
	for(int i=0; i<=10; i++)
	{
		m_pMemDC_Assistant->MoveTo(i*vPitch, height);
		m_pMemDC_Assistant->LineTo(i*vPitch, 0);
	}

	return 0;
}

int AutoFocusFar_Inspection::OnKeyCtrlNum()
{
	m_pInterface->GetDriverIC()->WriteAF(0);
	return 0;
}


void AutoFocusFar_Inspection::ClearStorage(BOOL bClear)
{
	if (bClear)
	{
		if (m_pvStorage != NULL)
		{
			m_pvStorage->clear();
		}
		m_pInterface->GetConfigManager()->ClearConfig(_T("AFBESTCODE"), _T("InfCode"));
	}
}


void AutoFocusFar_Inspection::CalcImageRect(void)
{
	ImageInfo img  = m_pInterface->GetImage();
	int width =  img.width;
	int height = img.height;

	for(int i=0; i<5; i++)
	{
		if(sMTF.ROI[i].Enable)
		{
			//����ת��Ϊʵ�����ص�
			ROI_Info* PROI = &sMTF.ROI[i];

			if (PROI->cy == 0 && PROI->cx == 0)
			{
				PROI->Enable = FALSE;
			}

			PROI->pos.top    = int((PROI->cy - sMTF.ROI_height/2.0)  * height/100);
			PROI->pos.bottom = int((PROI->cy + sMTF.ROI_height/2.0)  * height/100);
			PROI->pos.left   = int((PROI->cx - sMTF.ROI_width/2.0)   * width/100);
			PROI->pos.right  = int((PROI->cx + sMTF.ROI_width/2.0)   * width/100);

// 			if (m_bRotate)
// 			{
// 				if (i>=8)
// 				{
// 					int w = PROI->pos.Width();
// 					int h = PROI->pos.Height();
// 
// 					PROI->pos.top    = (PROI->cy * height/100) - w/2;
// 					PROI->pos.bottom = (PROI->cy * height/100) + w/2;
// 					PROI->pos.left   = (PROI->cx * width/100)  - h/2;
// 					PROI->pos.right  = (PROI->cx * width/100)  + h/2;
// 				}
// 			}

			if(PROI->pos.left < 0)
				PROI->pos.MoveToX(0);
			if(PROI->pos.right >= width)
				PROI->pos.MoveToX(width - 1 - PROI->pos.Width());
			if(PROI->pos.top < 0)
				PROI->pos.MoveToY(0);
			if(PROI->pos.bottom >= height)
				PROI->pos.MoveToY(height - 1 - PROI->pos.Height());

			if (PROI->pos.Width() == 0 || PROI->pos.Height() == 0)
			{
				PROI->Enable = FALSE;
			}
		}
	}

}

int AutoFocusFar_Inspection::BackToInitPos(void)
{
	m_pInterface->GetDriverIC()->WriteAF(0);
	m_pInterface->AddLog(_T("Write Code: 0"));
	Sleep(100);
	m_pInterface->GetDriverIC()->WriteAF(0);
	m_pInterface->AddLog(_T("Write Code: 0"));
	return RET_END;
}

BOOL AutoFocusFar_Inspection::AF_CheckGlue(int afCode)
{
	m_maxMTF_Center = max(sMTF.ROI[0].MTFvalue, m_maxMTF_Center);
	m_minMTF_Center = min(sMTF.ROI[0].MTFvalue, m_minMTF_Center);
	double diffCenter = fabs(m_maxMTF_Center - m_minMTF_Center);

	int tolerance = m_SearchParam[m_curSearchStep].m_DescendTime * m_curStepCode;
	if (afCode > (pDlg->CodeLimitH + tolerance) && 
		m_curSearchStep == 0)
	{
		if (diffCenter > m_SearchParam[m_curSearchStep].m_Diff)
		{
			if (m_bOneTimeLog)
			{
				m_szStr.Format(_T("����MTF��ֵ��%5.2f%% > %5.2f%%(����) �в���"), diffCenter, m_SearchParam[m_curSearchStep].m_Diff);
				m_pInterface->AddLog(m_szStr, COLOR_BLUE);
				m_bOneTimeLog = FALSE;
			}
			return TRUE;
		}
		m_pInterface->AddLog(_T("ģ�������ճ��"), COLOR_RED);
		return FALSE;
	}
	return TRUE;
}