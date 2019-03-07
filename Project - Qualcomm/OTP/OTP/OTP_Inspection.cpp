#include "StdAfx.h"
#include "OTP_Inspection.h"
#include "ImageProc.h"



#ifndef DCCM_EXP
    #define DCCM_EXP	__declspec(dllexport)
#endif	/* DCCM_EXP */


extern "C" DCCM_EXP Inspection * CreateInspectionItem(DccmControl_Interface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new OTP_Inspection(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
OTP_Inspection::OTP_Inspection(DccmControl_Interface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	SetConfigPath(pInterface->GetConfigPath());		// 设置Config路径	
	SetName(lpszName);	                            // 设置测试项目的名字
	pDlg = new OptionDlg(NULL, this);               // 生成对话框对象
	LoadOption();                                   // 读取参数
	pDlg->Create(OptionDlg::IDD, NULL);             // 创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());        // 得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TET_WHITE);

	//TODO: 其他类成员构造初始化
	//.....
	m_pOtp = NULL;
	m_pShading = NULL;

	
}

OTP_Inspection::~OTP_Inspection(void)
{

}

/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int OTP_Inspection::Initialize()
{
	Inspection::Initialize();
	//TODO:在此添加初始化代码
	step = 0;//for test item
	pitch = 0;//for skip frame
	m_Lock1 = FALSE;//for lock one time
	m_Lock2 = FALSE;//for lock one time
	m_normalExpLock = FALSE;
	m_pdafExpLock = FALSE;
	m_shadingflag = FALSE;
	m_proc1explock = FALSE;

	InitShadingItem();
	InitSensorItem();
	InitCFAItem();
	InitQualLSCItem();
	m_pOtp->InitProgram();
	return RET_END;
}


/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int OTP_Inspection::Testing()
{
    //TODO:在此添加测试项代码
	BOOL bRet = TRUE;
	int nResult = RESULT_INSPECTION_NULL;
	switch(step)
	{
	case 0:	
		bRet &= m_pOtp->InitOTP();
		if (!bRet)
		{
			nResult = RESULT_INSPECTION_NG;
			break;
		}
		else
		{
			step++;//= 5;// cnw
			return RET_CONTIUNE;
		}

	case 1:
		if (pDlg->m_EnPDAF)
		{	
			if (!m_proc1explock)
			{
				if (!PDAFExposure())
				{
					return -1;
				}
				else
				{
					m_proc1explock = TRUE; 
				}
			}
		}

		if (pitch < 2)
		{
			pitch++;
			return RET_CONTIUNE;
		}
		else
			pitch = 0;

		bRet = m_pOtp->PDAF_Proc1();//1 fail -1 continue 0 pass
		if (bRet == -1)
		{
			return bRet;
		}
		else if (bRet == 1)
		{
			nResult = RESULT_INSPECTION_NG;
			break;
		}
		else
		{
			step++;
			return RET_CONTIUNE;
		}

	case 2:	
		if (!NormalExposure())
		{
			return -1;
		}
		m_pInterface->PauseGrabFrame();
		bRet &= m_pOtp->GetLSC();
		m_pInterface->ResumeGrabFrame();
		if (!bRet)
		{
			nResult = RESULT_INSPECTION_NG;
			break;
		}
		else
		{
			step++;
			return RET_CONTIUNE;
		}

	case 3:	
		if ((!m_Lock1) && (pDlg->m_LSCItem == 2))// Sensor LSC
		{
			if (pitch < 3)
			{
				pitch++;
				return RET_CONTIUNE;
			}
			else
			{
				m_Lock1 = TRUE;
				pitch = 0;
			}
			if (pDlg->m_EnLSC)
			{
				if (!TestShading())//Shading After Apply LSC
				{
					bRet = 0;
					m_pInterface->AddLog(_T("After SRAM Test Shading NG!"),COLOR_RED);
					break;
				}
			}
		}
		else if ((!m_Lock1) &&(pDlg->m_LSCItem == 0))//MTK
		{
			if (pitch < 3)
			{
				pitch++;
				return RET_CONTIUNE;
			}
			else
			{
				m_Lock1 = TRUE;
				pitch = 0;
			}
			if (pDlg->m_EnLSC)
			{
				if (!TestShading())//Shading for MTK LSC Correctded raw
				{
					bRet = 0;
					m_pInterface->AddLog(_T("MTK Test Shading NG!"),COLOR_RED);
					break;
				}
			}
		}

		if (!NormalExposure())
		{
			return -1;
		}

		bRet &= m_pOtp->GetAWB();
		if (!bRet)
		{
			nResult = RESULT_INSPECTION_NG;
			break;
		}
		else
		{
			step++;
			return RET_CONTIUNE;
		}

	case 4:	
		if (pitch < 2)
		{
			pitch++;
			return RET_CONTIUNE;
		}
		else
			pitch = 0;

		bRet &= m_pOtp->CheckOTPAWB();
		if (!bRet)
		{
			nResult = RESULT_INSPECTION_NG;
			break;
		}
		else
		{
			step++;
			return RET_CONTIUNE;
		}
	case 5:	
		m_pInterface->PauseGrabFrame();
		if (pDlg->m_LSCItem == 1)
		{
			bRet &= m_pOtp->WriteOTP();
		}
		else
		{
			bRet &= m_pOtp->WriteOTP(m_pShading->m_vData);//
		}
		m_pInterface->ResumeGrabFrame();
		if (!bRet)
		{
			nResult = RESULT_INSPECTION_NG;
			break;
		}
		else
		{
			if (!pDlg->m_dummyburn)
			{
				step++;
				return RET_CONTIUNE;
			}
			else
			{
				step = 100;//Dummy Burn到此直接PASS
				return RET_CONTIUNE;
			}
		}
	case 6:	
		if (pDlg->m_twosetting)
		{
			m_pInterface->RestartCamera(1);
		}
		else
		{
			m_pInterface->RestartCamera(0);
		}
		m_pOtp->SaveExp = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllExp"), 1);
		m_pInterface->GetImageSensor()->WriteExp(m_pOtp->SaveExp);
		m_pOtp->SaveGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"RecordExp",_T("ExpGain"), 1);
		m_pInterface->GetImageSensor()->WriteAnalogGain(m_pOtp->SaveGain);

		m_pInterface->PauseGrabFrame();
		bRet &= m_pOtp->CheckOTP();
		m_pInterface->ResumeGrabFrame();
		if (!bRet)
		{
			nResult = RESULT_INSPECTION_NG;
			break;
		}
		else
		{
			step++;
			return RET_CONTIUNE;
		}
	case 7:	
		if (!NormalExposure())
		{
			return -1;
		}
		m_pInterface->PauseGrabFrame();
		bRet &= m_pOtp->VerifyLSC();
		m_pInterface->ResumeGrabFrame();
		if (!bRet)
		{
			nResult = RESULT_INSPECTION_NG;
			break;
		}
		else
		{
			step++;
			return RET_CONTIUNE;
		}
	case 8:	
		if ((!m_Lock2) && (pDlg->m_LSCItem == 2))
		{
			if (pitch < 2)
			{
				pitch++;
				return RET_CONTIUNE;
			}
			else
			{
				m_Lock2 = TRUE;
				pitch = 0;
			}
			m_shadingflag = TRUE;
			if (!TestShading())	
			{ 
				bRet = 0;
				m_pInterface->AddLog(_T("After OTP Test Shading NG!"),COLOR_RED);
				break;
			}
		}
		if (!NormalExposure())
		{
			return -1;
		}
		bRet &= m_pOtp->VerifyPreWB();
		if (!bRet)
		{
			nResult = RESULT_INSPECTION_NG;
			break;
		}
		else
		{
			step++;
			return RET_CONTIUNE;
		}
	case 9:
		if (pitch < 2)
		{
			pitch++;
			return RET_CONTIUNE;
		}
		else
			pitch = 0;
		bRet &= m_pOtp->VerifyAWB();
		if (!bRet)
		{
			nResult = RESULT_INSPECTION_NG;
			break;
		}
		else
		{
			step++;
			return RET_CONTIUNE;
		}
	default:
		break;
	}

	switch (nResult)
	{
	case RESULT_INSPECTION_NG:
		m_pInterface->AddLog(L"FAIL", COLOR_RED);
		break;
	case RESULT_INSPECTION_OK:
		m_pInterface->AddLog(L"PASS", COLOR_GREEN);
		break;
	default://RESULT_INSPECTION_NULL
		bRet = RET_END;
		break;
	}
	SetResult(bRet);
    return RET_END;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等

返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int OTP_Inspection::Finalize()
{
	//TODO:在此添加回收结束代码
    m_pOtp->EndProgram();
	return RET_END;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int OTP_Inspection::LoadOption()
{
	//TODO: 在此添加读取代码 

	pDlg->m_projName = (eProjectName)ReadConfigInt(L"ProjectName", -1);
	pDlg->m_ExposureEn    = ReadConfigInt(_T("ExposureEn"), 1);
	pDlg->m_exp_target    = ReadConfigInt(_T("Exp_Target"), 165);
	pDlg->m_PDAFExposureEn   = ReadConfigInt(_T("PDAFExposureEn"), 1);
	pDlg->m_exp_pdaftarget    = ReadConfigInt(_T("Exp_PDAFTarget"), 210);
	pDlg->m_exp_tolerance = ReadConfigInt(_T("Exp_Tolerance"),5);
	pDlg->m_exp_pdaftolerance = ReadConfigInt(_T("PDAFExp_Tolerance"),5);
	pDlg->m_exp_size = ReadConfigInt(_T("Exp_Size"),10);
	pDlg->m_exp_pdafsize = ReadConfigInt(_T("PDAFExp_Size"),16);
	pDlg->m_sleeptime = ReadConfigInt(_T("Sleeptime"),1);
	pDlg->m_EnBasic   = ReadConfigInt(_T("EnBasic"), 1);  
	pDlg->m_EnQVL   = ReadConfigInt(_T("EnQVL"), 1);  
	pDlg->m_PDAFItem   = ReadConfigInt(_T("MTKPDAFEn"), 1);
	pDlg->m_EnLSC   = ReadConfigInt(_T("EnLSC"), 1);  
	pDlg->m_LSCItem   = ReadConfigInt(_T("EnMTKLSC"), 0);  
	pDlg->m_lscTarget = ReadConfigInt(_T("LscTarget"),70);
	pDlg->m_lscGroup = ReadConfigInt(_T("LscGroup"),1);
	pDlg->m_EnAWB   = ReadConfigInt(_T("EnAWB"), 1);  
	pDlg->m_SaveAWB = ReadConfigInt(_T("SaveAWB"), 0);  
	pDlg->m_EnAF   = ReadConfigInt(_T("EnAF"), 1);  
	pDlg->m_EnPDAF   = ReadConfigInt(_T("EnPDAF"), 1);  
	pDlg->m_berase = ReadConfigInt(_T("EnErase"),0);
	pDlg->m_bcheckinf = ReadConfigInt(_T("EnCheckINF"),1);
	pDlg->m_bcheckmac = ReadConfigInt(_T("EnCheckMAC"),1);
	pDlg->m_bchecksta = ReadConfigInt(_T("EnCheckSTA"),0);
	pDlg->m_dummyburn = ReadConfigInt(_T("DummyBurn"),0);
	pDlg->m_twosetting = ReadConfigInt(_T("TwoSetting"),0);
	pDlg->m_goldengain = ReadConfigInt(_T("EnGoldenGain"), 0);

	pDlg->m_awb_size = ReadConfigInt(_T("AWB_Size"),10);
	pDlg->m_rgainh    = ReadConfigInt(_T("RGainH"), 1023);
	pDlg->m_rgainl   = ReadConfigInt(_T("RGainL"), 0);
	pDlg->m_bgainh    = ReadConfigInt(_T("BGainH"), 1023);
	pDlg->m_bgainl    = ReadConfigInt(_T("BGainL"), 0);
	pDlg->m_goldenr    = ReadConfigInt(_T("GoldenR"), 512);
	pDlg->m_goldenb    = ReadConfigInt(_T("GoldenB"), 512);
	pDlg->m_goldengr    = ReadConfigInt(_T("GoldenGr"), 512);
	pDlg->m_goldengb    = ReadConfigInt(_T("GoldenGb"), 512);
	pDlg->m_rgaindif = (float)ReadConfigDouble(_T("RGainDif"), 15.0);
	pDlg->m_bgaindif = (float)ReadConfigDouble(_T("BGainDif"), 15.0);
	pDlg->m_orgdif = (float)ReadConfigDouble(_T("OrgDif"), 30.0);

	pDlg->m_afread = ReadConfigInt(_T("AFRead"), 0);
	pDlg->m_InfH    = ReadConfigInt(_T("InfH"), 1023);
	pDlg->m_InfL    = ReadConfigInt(_T("InfL"), 0);
	pDlg->m_MacH    = ReadConfigInt(_T("MacH"), 1023);
	pDlg->m_MacL    = ReadConfigInt(_T("MacL"), 0);
	pDlg->m_StcH    = ReadConfigInt(_T("StcH"), 1023);
	pDlg->m_StcL    = ReadConfigInt(_T("StcL"), 0);
	pDlg->m_AFDif    = ReadConfigInt(_T("AFDif"), 30);

	pDlg->m_grouplimit    = ReadConfigInt(_T("GroupLimit"), 3);
	pDlg->m_shadingitem = ReadConfigInt(_T("ShadingItem"),0);
	pDlg->m_sensoritem = ReadConfigInt(_T("SensorItem"),0);
	pDlg->m_ydiff = ReadConfigDouble(_T("Shading_YDif"),15);
	pDlg->m_ymax = ReadConfigDouble(_T("Shading_YMAX"),65);
	pDlg->m_ymin = ReadConfigDouble(_T("Shading_YMIN"),60);
	pDlg->m_rgdiff = ReadConfigDouble(_T("Shading_RGDiff"),10);
	pDlg->m_bgdiff = ReadConfigDouble(_T("Shading_BGDiff"),10);
	pDlg->m_ymax2 = ReadConfigDouble(_T("After OTP Shading_YMAX"),65);
	pDlg->m_ymin2 = ReadConfigDouble(_T("After OTP Shading_YMIN"),60);

	pDlg->m_SaveMTKraw = ReadConfigInt(_T("SaveMTKRaw"),0);
	pDlg->m_MTKLSCVerItem = ReadConfigInt(_T("MTKLSCVersion"), 0);
	pDlg->m_QUALLSCVerItem = ReadConfigInt(_T("QUALLSCVersion"), 0);
	pDlg->m_cfaitem = ReadConfigInt(_T("CFAItem"),0);
	pDlg->m_quallscitem = ReadConfigInt(_T("QUALLSCMODE"),0);
	pDlg->m_qualawb = ReadConfigInt(_T("ENQUALAWB"),0);

	pDlg->m_goldenrg    = ReadConfigInt(_T("GoldenRGain"), 512);
	pDlg->m_goldenbg    = ReadConfigInt(_T("GoldenBGain"), 512);
	pDlg->m_goldengg    = ReadConfigInt(_T("GoldenGGain"), 1024);
	pDlg->m_MTKPDAFitem = ReadConfigInt(_T("MTKPDAFVersion"), 0);
	pDlg->m_QUALPDAFitem = ReadConfigInt(_T("QUALPDAFVersion"), 0);
	pDlg->m_MTK_INI_Name = ReadConfigString(_T("MTK_INI_NAME"), _T(".ini")); 
	pDlg->m_Qual_INI_Name = ReadConfigString(_T("Qual_INI_NAME"), _T(".txt")); 
	//.......................
	return Inspection::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int OTP_Inspection::SaveOption()
{
	if(!Inspection::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	WriteConfigInt(L"ProjectName", pDlg->m_projName);
	WriteConfigInt(_T("ExposureEn      "),pDlg->m_ExposureEn);
	WriteConfigInt(_T("Exp_Target      "),pDlg->m_exp_target);
	WriteConfigInt(_T("PDAFExposureEn      "),pDlg->m_PDAFExposureEn);
	WriteConfigInt(_T("Exp_PDAFTarget      "),pDlg->m_exp_pdaftarget);
	WriteConfigInt(_T("Exp_Tolerance   "),pDlg->m_exp_tolerance);
	WriteConfigInt(_T("PDAFExp_Tolerance   "),pDlg->m_exp_pdaftolerance);
	WriteConfigInt(_T("Exp_Size   "),pDlg->m_exp_size);
	WriteConfigInt(_T("PDAFExp_Size   "),pDlg->m_exp_pdafsize);

	WriteConfigInt(_T("Sleeptime   "),pDlg->m_sleeptime);
	WriteConfigInt(_T("EnBasic      "),pDlg->m_EnBasic);  
	WriteConfigInt(_T("EnQVL      "),pDlg->m_EnQVL);  
	WriteConfigInt(_T("MTKPDAFEn      "),pDlg->m_PDAFItem);
	WriteConfigInt(_T("EnLSC      "),pDlg->m_EnLSC);  
	WriteConfigInt(_T("EnMTKLSC      "),pDlg->m_LSCItem);  
	WriteConfigInt(_T("LscTarget      "),pDlg->m_lscTarget);
	WriteConfigInt(_T("LscGroup      "),pDlg->m_lscGroup);
	WriteConfigInt(_T("EnAWB      "),pDlg->m_EnAWB);  
	WriteConfigInt(_T("SaveAWB      "),pDlg->m_SaveAWB);  
	WriteConfigInt(_T("EnAF      "),pDlg->m_EnAF);  
	WriteConfigInt(_T("EnPDAF      "),pDlg->m_EnPDAF);  
	WriteConfigInt(_T("EnErase      "),		pDlg->m_berase);  
	WriteConfigInt(_T("EnCheckINF      "),		pDlg->m_bcheckinf);  
	WriteConfigInt(_T("EnCheckMAC      "),		pDlg->m_bcheckmac);  
	WriteConfigInt(_T("EnCheckSTA      "),		pDlg->m_bchecksta);  

	WriteConfigInt(_T("DummyBurn      "),		pDlg->m_dummyburn);
	WriteConfigInt(_T("TwoSetting      "), pDlg->m_twosetting);
	WriteConfigInt(_T("EnGoldenGain      "), pDlg->m_goldengain);

	WriteConfigInt(_T("AWB_Size   "),pDlg->m_awb_size);
	WriteConfigInt(_T("RGainH      "),pDlg->m_rgainh);
	WriteConfigInt(_T("RGainL      "),pDlg->m_rgainl);
	WriteConfigInt(_T("BGainH      "),pDlg->m_bgainh);
	WriteConfigInt(_T("BGainL      "),pDlg->m_bgainl);
	WriteConfigInt(_T("GoldenR      "),pDlg->m_goldenr);
	WriteConfigInt(_T("GoldenB      "),pDlg->m_goldenb);
	WriteConfigInt(_T("GoldenGr      "),pDlg->m_goldengr);
	WriteConfigInt(_T("GoldenGb      "),pDlg->m_goldengb);
	WriteConfigDouble(_T("RGainDif      "), pDlg->m_rgaindif);
	WriteConfigDouble(_T("BGainDif      "), pDlg->m_bgaindif);
	WriteConfigDouble(_T("OrgDif      "), pDlg->m_orgdif);

	WriteConfigInt(_T("AFRead      "),pDlg->m_afread);
	WriteConfigInt(_T("InfH      "),pDlg->m_InfH);
	WriteConfigInt(_T("InfL      "),pDlg->m_InfL);
	WriteConfigInt(_T("MacH      "),pDlg->m_MacH);
	WriteConfigInt(_T("MacL      "),pDlg->m_MacL);
	WriteConfigInt(_T("StcH      "),pDlg->m_StcH);
	WriteConfigInt(_T("StcL      "),pDlg->m_StcL);
	WriteConfigInt(_T("AFDif      "),pDlg->m_AFDif);
	WriteConfigInt(_T("GroupLimit      "),pDlg->m_grouplimit);
	WriteConfigInt(_T("ShadingItem      "),pDlg->m_shadingitem);
	WriteConfigInt(_T("SensorItem      "),pDlg->m_sensoritem);
	WriteConfigDouble(_T("Shading_YDif      "),pDlg->m_ydiff);
	WriteConfigDouble(_T("Shading_YMAX      "),pDlg->m_ymax);
	WriteConfigDouble(_T("Shading_YMIN      "),pDlg->m_ymin);
	WriteConfigDouble(_T("Shading_RGDiff      "),pDlg->m_rgdiff);
	WriteConfigDouble(_T("Shading_BGDiff      "),pDlg->m_bgdiff);
	WriteConfigDouble(_T("After OTP Shading_YMAX      "),pDlg->m_ymax2);
	WriteConfigDouble(_T("After OTP Shading_YMIN      "),pDlg->m_ymin2);

	WriteConfigInt(_T("SaveMTKRaw      "),pDlg->m_SaveMTKraw);
	WriteConfigInt(_T("QUALLSCVersion      "),pDlg->m_QUALLSCVerItem);
	WriteConfigInt(_T("MTKLSCVersion      "),pDlg->m_MTKLSCVerItem);
	WriteConfigInt(_T("CFAItem      "),pDlg->m_cfaitem);
	WriteConfigInt(_T("QUALLSCMODE      "),pDlg->m_quallscitem);
	WriteConfigInt(_T("ENQUALAWB      "),pDlg->m_qualawb);

	WriteConfigInt(_T("MTKPDAFVersion      "),pDlg->m_MTKPDAFitem);
	WriteConfigInt(_T("QUALPDAFVersion      "),pDlg->m_QUALPDAFitem);
	WriteConfigString(_T("MTK_INI_NAME      "), pDlg->m_MTK_INI_Name);
	WriteConfigString(_T("Qual_INI_NAME      "), pDlg->m_Qual_INI_Name);

	WriteConfigInt(_T("GoldenRGain      "),pDlg->m_goldenrg);
	WriteConfigInt(_T("GoldenBGain      "),pDlg->m_goldenbg);
	WriteConfigInt(_T("GoldenGGain      "),pDlg->m_goldengg);

	MatchProjName(pDlg->m_projName);
	pDlg->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_COMBO_PROJ_NAME, CBN_SELCHANGE), (LPARAM)pDlg->m_hWnd);

	if (((CButton*)pDlg->GetDlgItem(IDC_GOLDENCHANNEL))->GetCheck())
	{
		UINT SENSOROB = m_pOtp->m_otpSensor.OB;
		UINT AWB_FACTOR = m_pOtp->m_wbCtrl.factorAmplify;
		pDlg->m_goldenrg = (UINT)(1.0 * (pDlg->m_goldenr - SENSOROB) / (pDlg->m_goldengr - SENSOROB)*AWB_FACTOR + 0.5);
		pDlg->m_goldenbg = (UINT)(1.0 * (pDlg->m_goldenb - SENSOROB) / (pDlg->m_goldengb - SENSOROB)*AWB_FACTOR + 0.5);
		pDlg->m_goldengg = (UINT)(1.0 * (pDlg->m_goldengb - SENSOROB) / (pDlg->m_goldengr - SENSOROB)*AWB_FACTOR + 0.5);
		pDlg->OnBnClickedGoldenchannel();
		pDlg->OnBnClickedGoldengain();
	}
	pDlg->OnBnClickedEnlsc();
	pDlg->OnBnClickedEnaf();
	pDlg->OnBnClickedEnawb();
	pDlg->OnBnClickedEnpdaf();

	if (((CButton*)pDlg->GetDlgItem(IDC_ENSENSORLSC))->GetCheck())
	{
		m_pOtp->SensorLSCSta = TRUE;
	}
	else
	{
		m_pOtp->SensorLSCSta = FALSE;
	}
	if (((CButton*)pDlg->GetDlgItem(IDC_CHECK_PDAF))->GetCheck())
	{
		pDlg->GetDlgItem(IDC_ENMTKPDAF)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_ENQUALPDAF)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_CHECK_QVL)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_MTK_INI_NAME)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_Qual_INI_NAME)->EnableWindow(TRUE);
	}
	else
	{
		pDlg->GetDlgItem(IDC_ENMTKPDAF)->EnableWindow(FALSE);
		pDlg->GetDlgItem(IDC_ENQUALPDAF)->EnableWindow(FALSE);
		pDlg->GetDlgItem(IDC_CHECK_QVL)->EnableWindow(FALSE);
		pDlg->GetDlgItem(IDC_MTK_INI_NAME)->EnableWindow(FALSE);
		pDlg->GetDlgItem(IDC_Qual_INI_NAME)->EnableWindow(FALSE);
	}
	//........................


	m_pOtp->m_goldenrg = pDlg->m_goldenrg;
	m_pOtp->m_goldenbg = pDlg->m_goldenbg;
	m_pOtp->m_goldengg = pDlg->m_goldengg;

	m_pOtp->m_awb_size = pDlg->m_awb_size;
	m_pOtp->m_rgainh = pDlg->m_rgainh;
	m_pOtp->m_rgainl = pDlg->m_rgainl;
	m_pOtp->m_bgainh = pDlg->m_bgainh;
	m_pOtp->m_bgainl = pDlg->m_bgainl;
	m_pOtp->m_goldenr = pDlg->m_goldenr;
	m_pOtp->m_goldenb = pDlg->m_goldenb;
	m_pOtp->m_goldengr = pDlg->m_goldengr;
	m_pOtp->m_goldengb = pDlg->m_goldengb;
	m_pOtp->m_rgaindif = pDlg->m_rgaindif;
	m_pOtp->m_bgaindif = pDlg->m_bgaindif;
	m_pOtp->m_orgdif = pDlg->m_orgdif;

	m_pOtp->m_afread = pDlg->m_afread;
	m_pOtp->m_InfH = pDlg->m_InfH;
	m_pOtp->m_InfL = pDlg->m_InfL;
	m_pOtp->m_MacH = pDlg->m_MacH;
	m_pOtp->m_MacL = pDlg->m_MacL;
	m_pOtp->m_StcH = pDlg->m_StcH;
	m_pOtp->m_StcL = pDlg->m_StcL;
	m_pOtp->m_AFDif = pDlg->m_AFDif;
	m_pOtp->m_grouplimit = pDlg->m_grouplimit;

	m_pOtp->m_sleeptime = pDlg->m_sleeptime;
	m_pOtp->m_EnBasic = pDlg->m_EnBasic;
	m_pOtp->m_PDAFItem = pDlg->m_PDAFItem;
	m_pOtp->m_EnQVL = pDlg->m_EnQVL;
	m_pOtp->m_EnLSC = pDlg->m_EnLSC;
	m_pOtp->m_LSCItem = pDlg->m_LSCItem;
	m_pOtp->m_EnAF = pDlg->m_EnAF;
	m_pOtp->m_EnAWB = pDlg->m_EnAWB;
	m_pOtp->m_SaveAWB = pDlg->m_SaveAWB;
	m_pOtp->m_EnPDAF = pDlg->m_EnPDAF;
	m_pOtp->m_berase = pDlg->m_berase;
	m_pOtp->m_bcheckinf = pDlg->m_bcheckinf;
	m_pOtp->m_bcheckmac = pDlg->m_bcheckmac;
	m_pOtp->m_bchecksta = pDlg->m_bchecksta;

	m_pOtp->m_SaveMTKraw = pDlg->m_SaveMTKraw;
	m_pOtp->m_MTKLSCVerItem = pDlg->m_MTKLSCVerItem;
	m_pOtp->m_QUALLSCVerItem = pDlg->m_QUALLSCVerItem;
	m_pOtp->m_dummyburn = pDlg->m_dummyburn;
	m_pOtp->m_qualawb = pDlg->m_qualawb;

	m_pOtp->m_MTKPDAFitem = pDlg->m_MTKPDAFitem;
	m_pOtp->m_QUALPDAFitem = pDlg->m_QUALPDAFitem;
	m_pOtp->m_MTK_INI_Name = pDlg->m_MTK_INI_Name;
	m_pOtp->m_Qual_INI_Name = pDlg->m_Qual_INI_Name;
	m_pOtp->m_LscTarget = pDlg->m_lscTarget;
	m_pOtp->m_LscGroup = pDlg->m_lscGroup;

	m_pOtp->PreSubclassOtp();

	// 	if (m_pShading == NULL)
	// 	{
	InitShadingItem();
	SAFE_DELETE(m_pShading);
	m_pShading = new ShadingCalc(m_pInterface, m_pOtp->m_shadingitem);//m_shadingnum	
	//  }
	m_pShading->InitMember();
	m_pShading->m_shadingflag = m_shadingflag;
	m_pShading->m_ymax = pDlg->m_ymax;
	m_pShading->m_ymin = pDlg->m_ymin;
	m_pShading->m_ydif = pDlg->m_ydiff;
	m_pShading->m_rgdiff = pDlg->m_rgdiff;
	m_pShading->m_bgdiff = pDlg->m_bgdiff;

	// 	pDlg->m_ymax2 = pDlg->m_ymax;//前后shading标准一致。不一致仅用于5E8，或其他SRAM与LSC不一致的Sensor
	// 	pDlg->m_ymin2 = pDlg->m_ymin;//前后shading标准一致。不一致仅用于5E8，或其他SRAM与LSC不一致的Sensor
	m_pShading->m_ymax2 = pDlg->m_ymax2;
	m_pShading->m_ymin2 = pDlg->m_ymin2;

	m_pShading->m_MTKLSCVerItem = pDlg->m_MTKLSCVerItem;
	//////////////////////////////////////////////////////////////////////////
	
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR OTP_Inspection::GetReportHeaders()
{
	// TODO : Make report headers of this inspection and return that
	static CString Header(_T("ModuleNum,StcCode,InfCode,MacCode,R,Gr,Gb,B,RGain,BGain,GGain,GoldenR,GoldenGr,GoldenGb,GoldenB,GoldenRG,GoldenBG,OTPType"));

	return Header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR OTP_Inspection::GetReportLowerLimits()
{
	static CString LowerLimit;
	LowerLimit = _T("");
	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR OTP_Inspection::GetReportUpperLimits()
{
	static CString UpperLimit;
	UpperLimit = _T("");
	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR OTP_Inspection::GetReportContents()
{
	static CString Content;
	Content.Format(L"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s",
		m_pInterface->GetModuleNum(),
		m_pOtp->m_stcCode,
		m_pOtp->m_infCode,
		m_pOtp->m_macCode,
		m_pOtp->Applyawbdata[0],
		m_pOtp->Applyawbdata[1],
		m_pOtp->Applyawbdata[2],
		m_pOtp->Applyawbdata[3],
		m_pOtp->Applyawbdata[4],
		m_pOtp->Applyawbdata[5],
		m_pOtp->Applyawbdata[6],
		m_pOtp->m_goldenr,
		m_pOtp->m_goldengr,
		m_pOtp->m_goldengb,
		m_pOtp->m_goldenb,
		m_pOtp->m_goldenrg,
		m_pOtp->m_goldenbg,
		m_pOtp->m_OTPType);
	return Content;
}

void OTP_Inspection::InitShadingItem()
{
	if (pDlg->m_shadingitem == SHADING_ITEM_5)
	{
		m_pOtp->m_shadingitem = 5;
	}
	else if (pDlg->m_shadingitem == SHADING_ITEM_14)
	{
		m_pOtp->m_shadingitem = 14;
	}
	else if (pDlg->m_shadingitem == SHADING_ITEM_49)
	{
		m_pOtp->m_shadingitem = 49;
	}
}

void OTP_Inspection::InitSensorItem()
{
	m_pOtp->m_sensoritem = pDlg->m_sensoritem;
}

void OTP_Inspection::InitCFAItem()
{
	m_pOtp->m_cfaitem = pDlg->m_cfaitem;
}

void OTP_Inspection::InitQualLSCItem()
{
	m_pOtp->m_quallscitem = pDlg->m_quallscitem;
}
int OTP_Inspection::Exposure_Interface(UINT size)
{
	// 	if (pDlg->m_exp_target >= pDlg->m_exp_pdaftarget)
	// 	{
	// 		m_pInterface->AddLog(L"1", COLOR_RED);
	// 	}
	return m_pInterface->Exposure(pDlg->m_exp_target, pDlg->m_exp_tolerance, EXP_G, TRUE, 50, 50, 100 / (size * 2));
}

int OTP_Inspection::PDAFExposure_Interface(UINT size)
{
	return m_pInterface->Exposure(pDlg->m_exp_pdaftarget, pDlg->m_exp_pdaftolerance, EXP_G, TRUE, 50, 50, 100 / (size * 2));
}

BOOL OTP_Inspection::PDAFExposure()
{
	if (pDlg->m_PDAFExposureEn)
	{
		if (!m_pdafExpLock)
		{
			m_pOtp->SaveExp = ReadConfigInt(_T("RecordDllPDAFExp"), 1);
			m_pInterface->GetImageSensor()->WriteExp(m_pOtp->SaveExp);
			m_pdafExpLock = TRUE;
		}

		int ret = -1;

		ret = PDAFExposure_Interface(pDlg->m_exp_pdafsize);

		if (ret != 0)
		{
			return FALSE;
		}
		else
		{
			Sleep(200);
			m_pInterface->GetImageSensor()->ReadExp(m_pOtp->SavePDAFExp);
			m_pInterface->GetConfigManager()->WriteConfigInt(GetName(), L"RecordDllPDAFExp", m_pOtp->SavePDAFExp);
			m_pdafExpLock = FALSE;
		}
	}
	return TRUE;
}

BOOL OTP_Inspection::NormalExposure()
{
	if (pDlg->m_ExposureEn)
	{
		if (!m_normalExpLock)
		{
			m_pOtp->SaveExp = ReadConfigInt(_T("RecordDllExp"), 1);
			m_pInterface->GetImageSensor()->WriteExp(m_pOtp->SaveExp);
			m_normalExpLock = TRUE;
		}
		int ret = -1;

		ret = Exposure_Interface(pDlg->m_exp_size);

		if (ret != 0)
		{
			return FALSE;
		}
		else
		{
			Sleep(200);
			m_pInterface->GetImageSensor()->ReadExp(m_pOtp->SaveExp);
			m_pInterface->GetConfigManager()->WriteConfigInt(GetName(), L"RecordDllExp", m_pOtp->SaveExp);
			m_normalExpLock = FALSE;
		}
	}
	return TRUE;
}

void OTP_Inspection::MatchProjName(eProjectName projName)
{
	SAFE_DELETE(m_pOtp);
	CString szName;
	switch(projName)
	{
	case PN_FHB6551M:
		m_pOtp = new COtpS5K3P3;
		szName = L"FHB6551M";
		break;
	case PN_PLQ8579M:
		m_pOtp = new COtpPLQ8579M;
		szName = L"PLQ8579M";
		break;
	case PN_TRJ8564M:
		m_pOtp = new COtpTRJ8564M;
		szName = L"TRJ8564M";
		break;
	case PN_HLTE1M:
		m_pOtp = new COtpHLTE1M;
		szName = L"HLTE1M";
		break;
	case PN_HLT7017:
		m_pOtp = new COtpHLT7017;
		szName = L"HLT7017";
		break;
	case PN_TYD8635M:
		m_pOtp = new COtpTYD8635M;
		szName = L"TYD8635M";
	case PN_TYD8583M:
		m_pOtp = new COtpTYD8583M;
		szName = L"TYD8583M";
		break;
	case PN_HLT7014:
		m_pOtp = new COtpHLT7014;
		szName = L"HLT7014";
		break;
	case PN_HLT7109:
		m_pOtp = new COtpHLT7109;
		szName = L"HLT7109";
		break;
	case PN_HLT7025:
		m_pOtp = new COtpHLT7025;
		szName = L"HLT7025";
		break;
	case PN_SAA3_0L2K:
		m_pOtp = new COtpSAA3_0L2K;
		szName = L"SAA3_0L2K"; // 华天项目
		break;
	default:
		m_pInterface->AddLog(L"未知项目名称，请先选择项目名!");
		m_pOtp = new COtpHLT7017;
		break;
	}

	m_pOtp->m_pInterface = m_pInterface;
	m_pInterface->AddLog(L"Otp项目名称：" + szName, COLOR_BLUE);
}


BOOL OTP_Inspection::TestShading()
{
	BOOL bRet = TRUE;
	m_pShading->m_shadingflag = m_shadingflag;
	m_pShading->CalcImageRect();

	if (pDlg->m_LSCItem == 2)
	{
		bRet = m_pShading->ShadingTest();
	}
	else if (pDlg->m_LSCItem == 0)
	{
		bRet = m_pShading->MTKShadingTest();
	}

	return bRet;
}
