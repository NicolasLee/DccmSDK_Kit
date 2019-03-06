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
	SetConfigPath(pInterface->GetConfigPathName());	 // 设置Config路径	
	SetName(lpszName);	                             // 设置测试项目的名字
	pDlg = new OptionDlg(NULL, this);                 // 生成对话框对象
	LoadOption();                                    // 读取参数
	pDlg->Create(OptionDlg::IDD, NULL);               // 创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         // 得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TYPE_IMAGE);

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

	InitItem();
	m_pOtp->InitProgram();
	return 0;
}

void OTP_Inspection::InitItem()
{
	InitShadingItem();
	InitSensorItem();
	InitCFAItem();
	InitQualLSCItem();
}
void OTP_Inspection::InitShadingItem()
{
	if (pDlg->m_shadingitem == 0)
	{
		m_pOtp->m_shadingitem = 5;
	}
	else if(pDlg->m_shadingitem == 1)
	{
		m_pOtp->m_shadingitem = 14;
	}
	else if(pDlg->m_shadingitem == 2)
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
/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int OTP_Inspection::Testing()
{
	//TODO:在此添加测试项代码
	BOOL bRet = TRUE;

	switch(step)
	{
	case 0:	
		bRet &= m_pOtp->InitOTP();
		if (!bRet)
		{break;}
		else
		{
			step++;
			return RET_CONTIUNE;
		}

	case 1:
		if (pDlg->m_EnPDAF)
		{
				if (!PDAFExposure())
				{
					return -1;
				}
		}
		bRet &= m_pOtp->PDAF_Proc1();
		if (!bRet)
		{break;}
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
		bRet &= m_pOtp->GetLSC();
		if (!bRet)
		{break;}
		else
		{

			step++;
			return RET_CONTIUNE;
		}

	case 3:	
		if ((!m_Lock1) && (pDlg->m_LSCItem == 2))//Sensor LSC
		{
			if (pitch < 2)
			{
				pitch++;
				return RET_CONTIUNE;
			}
			else
			{
				m_Lock1 = TRUE;
				pitch = 0;
			}

			if (!TestShading())//Shading After Apply LSC
			{
				bRet = 0;
				m_pInterface->AddLog(_T("After SRAM Test Shading NG!"),COLOR_RED);
				break;
			}
		}
		if (!NormalExposure())
		{
			return -1;
		}

		bRet &= m_pOtp->GetAWB();
		if (!bRet)
		{break;}
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
		{break;}
		else
		{
			step++;
			return RET_CONTIUNE;
		}
	case 5:	
		m_pInterface->PauseGrabFrame();
		bRet &= m_pOtp->WriteOTP();//m_pShading->m_vData
		m_pInterface->ResumeGrabFrame();
		if (!bRet)
		{break;}
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
		m_pOtp->SaveExp = ReadConfigInt(_T("RecordDllExp"), 1);
		m_pInterface->GetImageSensor()->WriteExp(m_pOtp->SaveExp);
		m_pOtp->SaveGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllGain"), 1);
		m_pInterface->GetImageSensor()->WriteAnalogGain(m_pOtp->SaveGain);

		m_pInterface->PauseGrabFrame();
		bRet &= m_pOtp->CheckOTP();
		m_pInterface->ResumeGrabFrame();
		if (!bRet)
		{break;}
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
		{break;}
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
		{break;}
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
		{break;}
		else
		{
			step++;
			return RET_CONTIUNE;
		}

	default:
		break;
	}

	switch(bRet)
	{
	case RESULT_FAIL:
		m_pInterface->AddLog(L"FAIL", COLOR_RED);
		break;
	case RESULT_PASS:
		m_pInterface->AddLog(L"PASS", COLOR_GREEN);
		break;
	default://RESULT_NULL
		bRet = RET_END;
		break;
	}
	SetResult(bRet);
	return RET_END;
}

BOOL OTP_Inspection::PDAFExposure()
{
	if (pDlg->m_PDAFExposureEn)
	{
		if (!m_pdafExpLock)
		{
			m_pOtp->SaveExp = ReadConfigInt(_T("RecordDllPDAFExp"), 1);
			m_pInterface->GetImageSensor()->WriteExp(m_pOtp->SaveExp);
			m_pOtp->SaveGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllGain"), 1);
			m_pInterface->GetImageSensor()->WriteAnalogGain(m_pOtp->SaveGain);
			m_pdafExpLock = TRUE;
		}

		int ret = -1;
		if (pDlg->m_PDAFItem == 0)//MTK
		{	
			ret = m_pInterface->Exposure(pDlg->m_exp_pdaftarget,pDlg->m_exp_tolerance, EXP_G,TRUE,50,50,3.125);//中心1/32
		}
		else if (pDlg->m_PDAFItem == 1)//Qual
		{
			ret = m_pInterface->Exposure(pDlg->m_exp_pdaftarget,pDlg->m_exp_tolerance, EXP_G,TRUE,50,50,3.125);//中心200x200
		}

			if(ret != 0)
		{
			return FALSE;
		}
		else
		{
			m_pInterface->GetImageSensor()->ReadExp(m_pOtp->SavePDAFExp);
			m_pInterface->GetConfigManager()->WriteConfigInt(GetName(),L"RecordDllPDAFExp",m_pOtp->SavePDAFExp);
			m_pOtp->SaveGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllGain"), 1);
			m_pInterface->GetImageSensor()->WriteAnalogGain(m_pOtp->SaveGain);
			m_pdafExpLock =FALSE;
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
			m_pOtp->SaveGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllGain"), 1);
			m_pInterface->GetImageSensor()->WriteAnalogGain(m_pOtp->SaveGain);
			m_normalExpLock = TRUE;
		}
		int ret = -1;
		if (pDlg->m_LSCItem == 0 ||pDlg->m_LSCItem == 1)
		{
			ret = m_pInterface->Exposure(pDlg->m_exp_target,pDlg->m_exp_tolerance,EXP_G,TRUE,50,50,5);//中心1/10
			if(ret != 0)		return FALSE;
		}
		if (pDlg->m_LSCItem == 2)
		{
			if (pDlg->m_sensoritem == 0)//3L8
			{
				ret = m_pInterface->Exposure(pDlg->m_exp_target,pDlg->m_exp_tolerance,EXP_G,TRUE,50,50,10);//1/5
				if(ret != 0)		return FALSE;
			}
			if (pDlg->m_sensoritem == 1)//5E8
			{
				ret = m_pInterface->Exposure(pDlg->m_exp_target,pDlg->m_exp_tolerance,EXP_G,TRUE,50,50,6.25);//1/8
				if(ret != 0)		return FALSE;
			}
		}
		if (ret == 0)
		{
			m_pInterface->GetImageSensor()->ReadExp(m_pOtp->SaveExp);
			m_pInterface->GetConfigManager()->WriteConfigInt(GetName(),L"RecordDllExp",m_pOtp->SaveExp);
			m_pInterface->GetImageSensor()->ReadAnalogGain(m_pOtp->SaveGain);
			m_pInterface->GetConfigManager()->WriteConfigInt(GetName(),L"RecordDllGain",m_pOtp->SaveGain);
			m_normalExpLock = FALSE;
		}
	}
	return TRUE;
}
/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等

返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int OTP_Inspection::Finalize()
{
	//TODO:在此添加回收结束代码
//	m_pOtp->EndProgram();
	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int OTP_Inspection::LoadOption()
{
	//TODO: 在此添加读取代码 

	pDlg->m_ExposureEn    = ReadConfigInt(_T("ExposureEn"), 1);
	pDlg->m_exp_target    = ReadConfigInt(_T("Exp_Target"), 165);
	pDlg->m_PDAFExposureEn   = ReadConfigInt(_T("PDAFExposureEn"), 1);
	pDlg->m_exp_pdaftarget    = ReadConfigInt(_T("Exp_PDAFTarget"), 210);
	pDlg->m_exp_tolerance = ReadConfigInt(_T("Exp_Tolerance"),5);
	pDlg->m_sleeptime = ReadConfigInt(_T("Sleeptime"),1);
	pDlg->m_EnQVL   = ReadConfigInt(_T("EnQVL"), 1);  
	pDlg->m_PDAFItem   = ReadConfigInt(_T("MTKPDAFEn"), 1);
	pDlg->m_EnLSC   = ReadConfigInt(_T("EnLSC"), 1);  
	pDlg->m_LSCItem   = ReadConfigInt(_T("EnMTKLSC"), 0);  
	pDlg->m_lscTarget = ReadConfigInt(_T("LscTarget"),70);
	pDlg->m_lscGroup = ReadConfigInt(_T("LscGroup"),1);
	pDlg->m_EnAWB   = ReadConfigInt(_T("EnAWB"), 1);  
	pDlg->m_EnAF   = ReadConfigInt(_T("EnAF"), 1);  
	pDlg->m_EnPDAF   = ReadConfigInt(_T("EnPDAF"), 1);  
	pDlg->m_berase = ReadConfigInt(_T("EnErase"),0);
	pDlg->m_bcheckinf = ReadConfigInt(_T("EnCheckINF"),1);
	pDlg->m_bcheckmac = ReadConfigInt(_T("EnCheckMAC"),1);
	pDlg->m_bchecksta = ReadConfigInt(_T("EnCheckSTA"),0);
	pDlg->m_dummyburn = ReadConfigInt(_T("DummyBurn"),0);
	pDlg->m_twosetting = ReadConfigInt(_T("TwoSetting"),0);
	pDlg->m_goldengain = ReadConfigInt(_T("EnGoldenGain"), 0);

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
	pDlg->m_cfaitem = ReadConfigInt(_T("CFAItem"),0);
	pDlg->m_quallscitem = ReadConfigInt(_T("QUALLSCMODE"),0);
	pDlg->m_qualawb = ReadConfigInt(_T("ENQUALAWB"),0);

	pDlg->m_goldenrg    = ReadConfigInt(_T("GoldenRGain"), 512);
	pDlg->m_goldenbg    = ReadConfigInt(_T("GoldenBGain"), 512);
	pDlg->m_goldengg    = ReadConfigInt(_T("GoldenGGain"), 1024);
	pDlg->m_MTK_INI_Name = ReadConfigString(_T("MTK_INI_NAME"), _T(".ini")); 
	pDlg->m_Qual_INI_Name = ReadConfigString(_T("Qual_INI_NAME"), _T(".txt")); 
	//........................
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
	WriteConfigInt(_T("ExposureEn      "),pDlg->m_ExposureEn);
	WriteConfigInt(_T("Exp_Target      "),pDlg->m_exp_target);
	WriteConfigInt(_T("PDAFExposureEn      "),pDlg->m_PDAFExposureEn);
	WriteConfigInt(_T("Exp_PDAFTarget      "),pDlg->m_exp_pdaftarget);
	WriteConfigInt(_T("Exp_Tolerance   "),pDlg->m_exp_tolerance);
	WriteConfigInt(_T("Sleeptime   "),pDlg->m_sleeptime);
	WriteConfigInt(_T("EnQVL      "),pDlg->m_EnQVL);  
	WriteConfigInt(_T("MTKPDAFEn      "),pDlg->m_PDAFItem);
	WriteConfigInt(_T("EnLSC      "),pDlg->m_EnLSC);  
	WriteConfigInt(_T("EnMTKLSC      "),pDlg->m_LSCItem);  
	WriteConfigInt(_T("LscTarget      "),pDlg->m_lscTarget);
	WriteConfigInt(_T("LscGroup      "),pDlg->m_lscGroup);
	WriteConfigInt(_T("EnAWB      "),pDlg->m_EnAWB);  
	WriteConfigInt(_T("EnAF      "),pDlg->m_EnAF);  
	WriteConfigInt(_T("EnPDAF      "),pDlg->m_EnPDAF);  
	WriteConfigInt(_T("EnErase      "),		pDlg->m_berase);  
	WriteConfigInt(_T("EnCheckINF      "),		pDlg->m_bcheckinf);  
	WriteConfigInt(_T("EnCheckMAC      "),		pDlg->m_bcheckmac);  
	WriteConfigInt(_T("EnCheckSTA      "),		pDlg->m_bchecksta);  

	WriteConfigInt(_T("DummyBurn      "),		pDlg->m_dummyburn);
	WriteConfigInt(_T("TwoSetting      "), pDlg->m_twosetting);
	WriteConfigInt(_T("EnGoldenGain      "), pDlg->m_goldengain);

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
	WriteConfigInt(_T("CFAItem      "),pDlg->m_cfaitem);
	WriteConfigInt(_T("QUALLSCMODE      "),pDlg->m_quallscitem);
	WriteConfigInt(_T("ENQUALAWB      "),pDlg->m_qualawb);

	WriteConfigString(_T("MTK_INI_NAME      "), pDlg->m_MTK_INI_Name);
	WriteConfigString(_T("Qual_INI_NAME      "), pDlg->m_Qual_INI_Name);
    MatchProject();

	if (((CButton *)pDlg->GetDlgItem(IDC_GOLDENCHANNEL))->GetCheck())
	{
		pDlg->m_goldenrg		= (UINT)(1.0 * (pDlg->m_goldenr-SENSOROB) / (pDlg->m_goldengr-SENSOROB)*AWB_FACTOR + 0.5); 
		pDlg->m_goldenbg		= (UINT)(1.0 * (pDlg->m_goldenb-SENSOROB) / (pDlg->m_goldengb-SENSOROB)*AWB_FACTOR + 0.5);
		pDlg->m_goldengg		= (UINT)(1.0 * (pDlg->m_goldengb-SENSOROB) / (pDlg->m_goldengr-SENSOROB)*AWB_FACTOR + 0.5);
		pDlg->OnBnClickedGoldenchannel();
		pDlg->OnBnClickedGoldengain();
	}

	WriteConfigInt(_T("GoldenRGain      "),pDlg->m_goldenrg);
	WriteConfigInt(_T("GoldenBGain      "),pDlg->m_goldenbg);
	WriteConfigInt(_T("GoldenGGain      "),pDlg->m_goldengg);

	if (((CButton *)pDlg->GetDlgItem(IDC_ENSENSORLSC))->GetCheck())
	{
		m_pOtp->SensorLSCSta = TRUE;
		((CButton *)pDlg->GetDlgItem(IDC_SensorLSCItem))->ShowWindow(TRUE);
		((CButton *)pDlg->GetDlgItem(IDC_LSCTarget))->ShowWindow(TRUE);
		((CButton *)pDlg->GetDlgItem(IDC_SensorLSCGroup))->ShowWindow(TRUE);
		((CButton *)pDlg->GetDlgItem(IDC_STATICLSC1))->ShowWindow(TRUE);
		((CButton *)pDlg->GetDlgItem(IDC_STATICLSC2))->ShowWindow(TRUE);
	}
	else
	{
		m_pOtp->SensorLSCSta = FALSE;
		((CButton *)pDlg->GetDlgItem(IDC_SensorLSCItem))->ShowWindow(FALSE);
		((CButton *)pDlg->GetDlgItem(IDC_LSCTarget))->ShowWindow(FALSE);
		((CButton *)pDlg->GetDlgItem(IDC_SensorLSCGroup))->ShowWindow(FALSE);
 		((CButton *)pDlg->GetDlgItem(IDC_STATICLSC1))->ShowWindow(FALSE);
		((CButton *)pDlg->GetDlgItem(IDC_STATICLSC2))->ShowWindow(FALSE);
	}
	if (((CButton *)pDlg->GetDlgItem(IDC_ENPDAF))->GetCheck())
	{
		((CButton *)pDlg->GetDlgItem(IDC_ENMTKPDAF))->EnableWindow(TRUE);
		((CButton *)pDlg->GetDlgItem(IDC_ENQUALPDAF))->EnableWindow(TRUE);
		((CButton *)pDlg->GetDlgItem(IDC_ENQVL))->EnableWindow(TRUE);
		((CButton *)pDlg->GetDlgItem(IDC_MTK_INI_NAME))->EnableWindow(TRUE);
		((CButton *)pDlg->GetDlgItem(IDC_Qual_INI_NAME))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)pDlg->GetDlgItem(IDC_ENMTKPDAF))->EnableWindow(FALSE);
		((CButton *)pDlg->GetDlgItem(IDC_ENQUALPDAF))->EnableWindow(FALSE);
		((CButton *)pDlg->GetDlgItem(IDC_ENQVL))->EnableWindow(FALSE);
		((CButton *)pDlg->GetDlgItem(IDC_MTK_INI_NAME))->EnableWindow(FALSE);
		((CButton *)pDlg->GetDlgItem(IDC_Qual_INI_NAME))->EnableWindow(FALSE);
	}

	if (m_pShading == NULL)
	{
		InitShadingItem();
		m_pShading = new ShadingCalc(m_pInterface, m_pOtp->m_shadingitem);//m_shadingnum	
	}
	m_pShading->InitMember();
	m_pShading->m_ymax = pDlg->m_ymax;
	m_pShading->m_ymin = pDlg->m_ymin;
	m_pShading->m_ydif = pDlg->m_ydiff;
	m_pShading->m_rgdiff = pDlg->m_rgdiff;
	m_pShading->m_bgdiff = pDlg->m_bgdiff;
// 	MatchProjName(pDlg->m_projName);
// 	pDlg->PostMessage(WM_COMMAND,   MAKEWPARAM(IDC_COMBO_PROJ_NAME, CBN_SELCHANGE), (LPARAM)pDlg->m_hWnd);
	m_pOtp->m_goldenrg				= pDlg->m_goldenrg;
	m_pOtp->m_goldenbg				= pDlg->m_goldenbg;
	m_pOtp->m_goldengg				= pDlg->m_goldengg;

	m_pOtp->m_rgainh				= pDlg->m_rgainh;
	m_pOtp->m_rgainl				= pDlg->m_rgainl;
	m_pOtp->m_bgainh				= pDlg->m_bgainh;
	m_pOtp->m_bgainl				= pDlg->m_bgainl;
	m_pOtp->m_goldenr				= pDlg->m_goldenr;
	m_pOtp->m_goldenb				= pDlg->m_goldenb;
	m_pOtp->m_goldengr				= pDlg->m_goldengr;
	m_pOtp->m_goldengb				= pDlg->m_goldengb;
	m_pOtp->m_rgaindif				= pDlg->m_rgaindif;
	m_pOtp->m_bgaindif				= pDlg->m_bgaindif;
	m_pOtp->m_orgdif				    = pDlg->m_orgdif;
		
	m_pOtp->m_InfH				    	= pDlg->m_InfH;
	m_pOtp->m_InfL				     	= pDlg->m_InfL;
	m_pOtp->m_MacH					= pDlg->m_MacH;
	m_pOtp->m_MacL					= pDlg->m_MacL;
	m_pOtp->m_StcH					= pDlg->m_StcH;
	m_pOtp->m_StcL					= pDlg->m_StcL;
	m_pOtp->m_AFDif					= pDlg->m_AFDif;	 
	m_pOtp->m_grouplimit			= pDlg->m_grouplimit;

    m_pOtp->m_sleeptime			  = pDlg->m_sleeptime;
	m_pOtp->m_PDAFItem       = pDlg->m_PDAFItem;
	m_pOtp->m_EnQVL                = pDlg->m_EnQVL;
	m_pOtp->m_EnLSC				  = pDlg->m_EnLSC;
	m_pOtp->m_LSCItem         = pDlg->m_LSCItem;
	m_pOtp->m_EnAF					  = pDlg->m_EnAF;
	m_pOtp->m_EnAWB              = pDlg->m_EnAWB;
	m_pOtp->m_EnPDAF			  = pDlg->m_EnPDAF;
	m_pOtp->m_berase                = pDlg->m_berase;
	m_pOtp->m_bcheckinf               = pDlg->m_bcheckinf;
	m_pOtp->m_bcheckmac            = pDlg->m_bcheckmac;
	m_pOtp->m_bchecksta              = pDlg->m_bchecksta;

	m_pOtp->m_dummyburn     = pDlg->m_dummyburn;
	m_pOtp->m_qualawb          = pDlg->m_qualawb;
	m_pOtp->m_MTK_INI_Name           = pDlg->m_MTK_INI_Name;
	m_pOtp->m_Qual_INI_Name           = pDlg->m_Qual_INI_Name;
	m_pOtp->m_LscTarget           = pDlg->m_lscTarget;
	m_pOtp->m_LscGroup           = pDlg->m_lscGroup;
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR OTP_Inspection::GetReportHeaders()
{
	static CString header;
	header = _T("");
	return header;
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
	Content = _T("");
	return Content;
}


void OTP_Inspection::MatchProject()
{
	if (m_pOtp == NULL)
	{
		m_pOtp = new COtpS5K3L8();
		m_pOtp->m_pInterface = m_pInterface;
	}
}

BOOL OTP_Inspection::TestShading() // TestShading
{
	BOOL bRet = TRUE;
	m_pShading->CalcImageRect();
	bRet = m_pShading->ShadingTest();

	return bRet;
}
