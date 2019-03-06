#include "StdAfx.h"
#include "OtpCheck_EYD2467M.h"

OtpCheck_EYD2467M::OtpCheck_EYD2467M(void)
{
// 	memset(&m_addrFlag, 0, sizeof(m_addrFlag));
// 	memset(&m_addrSum, 0, sizeof(m_addrSum));
// 	memset(&m_otpSize, 0, sizeof(m_otpSize));
// 	memset(&m_addrCode, 0, sizeof(m_addrCode));
	memset(&m_otpPage, 0, sizeof(m_otpPage));

	m_slaveSensor = 0xC0;
	m_slaveEeprom = 0;
	m_modeSensor = I2CMODE_16_8;
	m_modeEeprom = 0;
	m_regSize = 2;

	m_flagEmpty = 0x00;
	m_flagGroup1 = 0x01;
	m_flagGroup2 = 0x13;
	m_flagGroup3 = 0x37;

	m_addrFlag.moduleInfo	= 0x0201;
	m_addrFlag.awb			= 0x0c5f;
	m_addrFlag.lsc			= 0x0235;
	m_addrFlag.af			= 0x0cba;

	m_addrSum.moduleInfo	= 0x0212;
	m_addrSum.awb			= 0x0c7d;
	m_addrSum.lsc			= 0x0598;
	m_addrSum.af			= 0x0cc1;
	m_addrBaseSum.moduleInfo  =  0x020f;
	m_addrBaseSum.lsc = 0x0210;
	m_addrBaseSum.awb = 0x0211;
	m_Sumsize.moduleInfo = 1;
	m_Sumsize.awb=1;
	m_Sumsize.lsc=1;
	m_Sumsize.af=2;

	//有些OTP不是Flag到Checksum寄存器之间就计算sum
	m_redu.moduleInfo=6;
	m_redu.lsc = 0;
	m_redu.awb= 1;
	m_redu.af= 0;

	m_Baseredu.moduleInfo = 6;
	m_Baseredu.lsc = 8;
	m_Baseredu.awb = 1;

	m_otpSize.moduleInfo	= m_addrSum.moduleInfo - m_addrFlag.moduleInfo - 1 - m_redu.moduleInfo;
	m_otpSize.awb			= m_addrSum.awb - m_addrFlag.awb - 1 - m_redu.awb;
	m_otpSize.lsc			= m_addrSum.lsc - m_addrFlag.lsc - 1 - m_redu.lsc;
	m_otpSize.af			= m_addrSum.af	- m_addrFlag.af - 1 - m_redu.af;

	m_addrCode.stcL			= 0;
	m_addrCode.stcH			= 0;
	m_addrCode.infL			= 0x0cbe;
	m_addrCode.infH			= 0x0cbd;
	m_addrCode.macL			= 0x0cc0;
	m_addrCode.macH			= 0x0cbf;

	m_bCheckStc = FALSE;	// 起始Code
	m_bCheckInf = TRUE;	// 远景Code
	m_bCheckMac = TRUE;	// 近景Code

// 	m_stcLower;// = 0;	// 起始Code下边界
// 	m_stcUpper;// = 0;	// 起始Code上边界
// 	m_infLower;// = 0;	//
// 	m_infUpper;// = 0;	//
// 	m_macUpper;// = 0;	//
// 	m_macLower;// = 0;	//

}


OtpCheck_EYD2467M::~OtpCheck_EYD2467M(void)
{
}

BOOL OtpCheck_EYD2467M::Check_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"EYD2467M Check_ModuleInfo");

	int group = -1;
	BOOL bRet = TRUE;
	group = CheckFlag_ModuleInfo();
	if(!(group == 1 || group == 2 || group == 3))
	{
		return FALSE;
	}
	bRet &= checkSum_Module_Base(group);
	Sleep(20);
	bRet &= checkSum_LSC_Base(group);
	Sleep(20);
	bRet &= checkSum_AWB_Base(group);
	Sleep(20);
	bRet &= CheckSum_Module(group);
	Sleep(20);
	return bRet;
}

BOOL OtpCheck_EYD2467M::Check_AWB()
{
	//m_pInterface->AddDebugLog(L"EYD2467M Check_AWB");
	int group = -1;
	BOOL bRet = TRUE;
	group =  CheckFlag_AWB();
	if(!(group == 1 || group == 2 || group == 3))
	{
		return FALSE;
	}
	bRet &= CheckSum_AWB(group);

	return bRet;
}

BOOL OtpCheck_EYD2467M::Check_LSC()
{
	//m_pInterface->AddDebugLog(L"EYD2467M Check_LSC");
	int group = -1;
	BOOL bRet = TRUE;

	group = CheckFlag_LSC();
	if(!(group == 1 || group == 2 || group == 3))
	{
		return FALSE;
	}
	bRet &= CheckSum_LSC(group);

	return bRet;
}

BOOL OtpCheck_EYD2467M::Check_AF()
{
	//m_pInterface->AddDebugLog(L"EYD2467M Check_AF");
	int group = -1;
	BOOL bRet = TRUE;

	group = CheckFlag_AF();
	if(!(group == 1 || group == 2 || group == 3))
	{
		return FALSE;
	}
	bRet &= CheckSum_AF(group);
	bRet &= CheckCode_Stc();
	bRet &= CheckCode_Inf();
	bRet &= CheckCode_Mac();

	return bRet;
}

int OtpCheck_EYD2467M::SumCalc(USHORT* pVal, int valSize, int mod,int add)
{
	ASSERT(pVal != NULL);

	ULONGLONG  sum = 0;

	for (int i = 0; i<valSize; i++)
	{
		sum += pVal[i];
	}

	return sum % mod+add;
}

#ifdef IIC_SENSOR_EYD2467M
BOOL OtpCheck_EYD2467M::ReadSensor(USHORT reg, USHORT* val)
{
	//m_pInterface->AddDebugLog(L"EYD2467M ReadSensor_Byte");
	BOOL bRet = TRUE;
	USHORT readVal = 0;
	bRet &= m_pInterface->I2C_Read(m_slaveSensor, reg, &readVal, m_modeSensor);
	*val = (USHORT)readVal;

	return bRet;
}

BOOL OtpCheck_EYD2467M::WriteSensor(USHORT reg, USHORT val)
{
	//m_pInterface->AddDebugLog(L"EYD2467M WriteSensor_Byte");
	BOOL bRet = TRUE;
	USHORT readVal = 0;
	bRet &= m_pInterface->I2C_Write(m_slaveSensor, reg, val, m_modeSensor);

	return bRet;
}

BYTE OtpCheck_EYD2467M::ReadSensor_MultiBytes(WORD reg, USHORT regSize, USHORT *pVal, WORD valSize, BOOL bStop /* = FALSE */)
{
	//m_pInterface->AddDebugLog(L"EYD2467M ReadSensor_MultiBytes");
	return TRUE;
}

BYTE OtpCheck_EYD2467M::WriteSensor_MultiBytes(WORD reg, USHORT regSize, USHORT *pVal, WORD valSize)
{
	//m_pInterface->AddDebugLog(L"EYD2467M WriteSensor_MultiBytes");
	return TRUE;
}
#else
#endif

INT OtpCheck_EYD2467M::CheckFlag_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"EYD2467M CheckFlag_ModuleInfo");
	if (!m_bCheckFlag_ModuleInfo) return TRUE;

	// Read flag
	USHORT readVal = 0;
	ReadOTPData(m_addrFlag.moduleInfo,m_addrFlag.moduleInfo, &readVal);
	if (readVal == m_flagEmpty)
	{
		m_szInfo.Format(L"模组Basic未烧录!");
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return 0;
	}

	if (readVal == m_flagGroup1)
	{
		return 1;
	}	
	else if (readVal == m_flagGroup2)
	{
		return 2;
	}	
	else if (readVal == m_flagGroup3)
	{
		return 3;
	}
	else
	{
		m_szInfo.Format(L"read flag 0x%02x val is 0x%02x", m_addrFlag.moduleInfo, readVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return 4;
	}
}

BOOL OtpCheck_EYD2467M::CheckFlag_AWB()
{
	//m_pInterface->AddDebugLog(L"EYD2467M CheckFlag_AWB");
	if (!m_bCheckFlag_AWB) return TRUE;

	// Read flag
	USHORT readVal = 0;
	ReadOTPData(m_addrFlag.awb,m_addrFlag.awb, &readVal);
	if (readVal == m_flagEmpty)
	{
		m_szInfo.Format(L"模组AWB未烧录!");
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return 0;
	}
	if (readVal == m_flagGroup1)
	{
		return 1;
	}	
	else if (readVal == m_flagGroup2)
	{
		return 2;
	}	
	else if (readVal == m_flagGroup3)
	{
		return 3;
	}
	else
	{
		m_szInfo.Format(L"read flag 0x%02x val is 0x%02x", m_addrFlag.awb, readVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return -1;
	}
}

BOOL OtpCheck_EYD2467M::CheckFlag_LSC()
{
	//m_pInterface->AddDebugLog(L"EYD2467M CheckFlag_LSC");
	if (!m_bCheckFlag_LSC) return TRUE;

	// Read flag
	USHORT readVal = 0;
	ReadOTPData(m_addrFlag.lsc,m_addrFlag.lsc, &readVal);
	if (readVal == m_flagEmpty)
	{
		m_szInfo.Format(L"模组LSC未烧录!");
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return 0;
	}
	if (readVal == m_flagGroup1)
	{
		return 1;
	}	
	else if (readVal == m_flagGroup2)
	{
		return 2;
	}	
	else if (readVal == m_flagGroup3)
	{
		return 3;
	}
	else
	{
		m_szInfo.Format(L"read flag 0x%02x val is 0x%02x", m_addrFlag.lsc, readVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return -1;
	}
}

INT OtpCheck_EYD2467M::CheckFlag_AF()
{
	//m_pInterface->AddDebugLog(L"EYD2467M CheckFlag_AF");
	if (!m_bCheckFlag_AF) return -1;

	// Read flag
	USHORT readVal = 0;
	ReadOTPData(m_addrFlag.af,m_addrFlag.af, &readVal);
	if (readVal == m_flagEmpty)
	{
		m_szInfo.Format(L"模组AF未烧录!");
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return 0;
	}
	if (readVal == m_flagGroup1)
	{
		return 1;
	}	
	else if (readVal == m_flagGroup2)
	{
		return 2;
	}	
	else if (readVal == m_flagGroup3)
	{
		return 3;
	}
	else
	{
		m_szInfo.Format(L"read flag 0x%02x val is 0x%02x", m_addrFlag.lsc, readVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return -1;
	}
}

BOOL OtpCheck_EYD2467M::checkSum_Module_Base(USHORT group)
{
	//m_pInterface->AddDebugLog(L"EYD2467M CheckSum_ModuleInfo");
	if (!m_bCheckSum_ModuleInfo) return TRUE;

	BOOL bRet = TRUE;
	USHORT regAddr = 0;
	USHORT regAddrH = 0;
	USHORT regAddrL = 0;
	USHORT valSize = 0;
	USHORT readSum = 0;
	USHORT sumSize = 0;
	USHORT calcSum = 0;
	USHORT redu = 0;

	// Calc sum
	regAddr = m_addrFlag.moduleInfo;
	valSize = m_otpSize.moduleInfo;
	redu = m_redu.moduleInfo;
	USHORT* pVal = new USHORT[valSize];

	bRet &= ReadOTPData((regAddr+1)+(group-1)*(valSize+redu+1), (regAddr+1)+(valSize-1)+(group-1)*(valSize+redu+1), pVal);//+1为Flag

	// Read sum
	sumSize = m_Sumsize.moduleInfo;
	calcSum = SumCalc(pVal, valSize, 255, 1);
	regAddr = m_addrBaseSum.moduleInfo;
	bRet &= ReadOTPData(regAddr+((group-1)*(valSize+redu+1)),regAddr+((group-1)*(valSize+redu+1)), &readSum);//+1为CheckSum
	SAFE_DELETE_ARRAY(pVal);

	if (readSum != calcSum)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readSum, calcSum);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return bRet;
}

BOOL OtpCheck_EYD2467M::checkSum_LSC_Base(USHORT group)
{
	//m_pInterface->AddDebugLog(L"EYD2467M CheckSum_ModuleInfo");
	if (!m_bCheckSum_ModuleInfo) return TRUE;
	BOOL bRet = TRUE;
	USHORT regAddr = 0;
	USHORT regAddrH = 0;
	USHORT regAddrL = 0;
	USHORT valSize = 0;
	USHORT readSum = 0;
	USHORT sumSize = 0;
	USHORT calcSum = 0;
	USHORT redu = 0;

	// Calc sum
	regAddr = m_addrFlag.lsc;
	valSize = m_otpSize.lsc;
	redu = m_redu.lsc;
	USHORT* pVal = new USHORT[valSize-m_Baseredu.lsc];

	bRet &= ReadOTPData((regAddr+1)+(group-1)*(valSize+redu+1), (regAddr+1)+(valSize-1)+(group-1)*(valSize+redu+1)-m_Baseredu.lsc, pVal);//+1为Flag

	// Read sum
	sumSize = m_Sumsize.lsc;
	calcSum = SumCalc(pVal, (valSize-m_Baseredu.lsc), 255, 1);
	regAddr = m_addrBaseSum.lsc;
	valSize = m_otpSize.moduleInfo;
	redu = m_redu.moduleInfo;
	bRet &= ReadOTPData(regAddr+((group-1)*(valSize+redu+1)),regAddr+((group-1)*(valSize+redu+1)), &readSum);//+1为CheckSum

	SAFE_DELETE_ARRAY(pVal);
	if (readSum != calcSum)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readSum, calcSum);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return bRet;
}

BOOL OtpCheck_EYD2467M::checkSum_AWB_Base(USHORT group)
{
	//m_pInterface->AddDebugLog(L"EYD2467M CheckSum_ModuleInfo");
	if (!m_bCheckSum_ModuleInfo) return TRUE;
	BOOL bRet = TRUE;
	USHORT regAddr = 0;
	USHORT regAddrH = 0;
	USHORT regAddrL = 0;
	USHORT valSize = 0;
	USHORT readSum = 0;
	USHORT sumSize = 0;
	USHORT calcSum = 0;
	USHORT redu = 0;

	// Calc sum
	regAddr = m_addrFlag.awb;
	valSize = m_otpSize.awb;
	redu = m_redu.awb;
	USHORT* pVal = new USHORT[valSize];

	bRet &= ReadOTPData((regAddr+1)+(group-1)*(valSize+redu+1), (regAddr+1)+(valSize-1)+(group-1)*(valSize+redu+1), pVal);//+1为Flag

	// Read sum
	sumSize = m_Sumsize.moduleInfo;
	calcSum = SumCalc(pVal, (valSize), 255, 1);
	regAddr = m_addrBaseSum.awb;
	valSize = m_otpSize.moduleInfo;
	redu = m_redu.moduleInfo;


	bRet &= ReadOTPData(regAddr+((group-1)*(valSize+redu+1)),regAddr+((group-1)*(valSize+redu+1)), &readSum);//+1为CheckSum

	SAFE_DELETE_ARRAY(pVal);
	if (readSum != calcSum)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readSum, calcSum);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return bRet;
}

BOOL OtpCheck_EYD2467M::CheckSum_Module(USHORT group)
{
	//m_pInterface->AddDebugLog(L"EYD2467M CheckSum_ModuleInfo");
	if (!m_bCheckSum_ModuleInfo) return TRUE;
	BOOL bRet = TRUE;
	USHORT regAddr = 0;
	USHORT regAddrH = 0;
	USHORT regAddrL = 0;
	USHORT valSize = 0;
	USHORT readSum = 0;
	USHORT sumSize = 0;
	USHORT calcSum = 0;
	USHORT redu = 0;

	// Calc sum
	regAddr = m_addrFlag.moduleInfo;
	valSize = m_otpSize.moduleInfo;
	redu = m_redu.moduleInfo;
	USHORT* pVal = new USHORT[valSize];

	bRet &= ReadOTPData((regAddr+1)+(group-1)*(valSize+redu+1), (regAddr+1)+(valSize-1)+(group-1)*(valSize+redu+1), pVal);//+1为Flag
	
	// Read sum
	sumSize = m_Sumsize.moduleInfo;
	calcSum = SumCalc(pVal, valSize, 256, 0);
	regAddr = m_addrSum.moduleInfo;

	SAFE_DELETE_ARRAY(pVal);
	if (sumSize == 1)
	{

		bRet &= ReadOTPData(regAddr+((group-1)*(valSize+redu+1)),regAddr+((group-1)*(valSize+redu+1)), &readSum);//+1为CheckSum
		
		if (readSum != calcSum)
		{
			m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readSum, calcSum);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FALSE;
		}
	}
	else if (sumSize == 2)
	{
		USHORT* pSum = new USHORT[sumSize];
		bRet &= ReadOTPData(regAddr+((group-1)*(valSize+redu+2)),(regAddr+((group-1)*(valSize+redu+2)))+1, &readSum);//+1为CheckSum
		regAddrH = pSum[0];
		regAddrL =  pSum[1];
		readSum = pSum[0]<<8|pSum[1];//高位在前
		SAFE_DELETE_ARRAY(pSum);
		if (readSum != calcSum)
		{
			m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readSum, calcSum);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FALSE;
		}
	}
	else
	{
		m_szInfo.Format(L"Basic sumSize设置错误:%02d", sumSize);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return bRet;
}

BOOL OtpCheck_EYD2467M::CheckSum_AWB(USHORT group)
{
	//m_pInterface->AddDebugLog(L"EYD2467M CheckSum_AWB");
	if (!m_bCheckSum_AWB) return TRUE;
	BOOL bRet = TRUE;
	USHORT regAddr = 0;
	USHORT regAddrH = 0;
	USHORT regAddrL = 0;
	USHORT valSize = 0;
	USHORT readSum = 0;
	USHORT sumSize = 0;
	USHORT calcSum = 0;
	USHORT redu = 0;

	// Calc sum
	regAddr = m_addrFlag.awb;
	valSize = m_otpSize.awb;
	USHORT* pVal = new USHORT[valSize];
	redu = m_redu.awb;

	bRet &= ReadOTPData((regAddr+1)+(group-1)*(valSize+redu+1), (regAddr+1)+(valSize-1)+(group-1)*(valSize+redu+1), pVal);//+1为Flag


	// Read sum
	sumSize = m_Sumsize.awb;
	calcSum = SumCalc(pVal, valSize, 256, 0);
	regAddr = m_addrSum.awb;

	SAFE_DELETE_ARRAY(pVal);
	if (sumSize == 1)
	{
		bRet &= ReadOTPData(regAddr+((group-1)*(valSize+redu+1)),regAddr+((group-1)*(valSize+redu+1)), &readSum);//+1为CheckSum
		
		if (readSum != calcSum)
		{
			m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readSum, calcSum);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FALSE;
		}
	}
	else if (sumSize == 2)
	{
		USHORT* pSum = new USHORT[sumSize];
		bRet &= ReadOTPData(regAddr+((group-1)*(valSize+redu+2)),(regAddr+((group-1)*(valSize+redu+2)))+1, &readSum);//+1为CheckSum
		regAddrH = pSum[0];
		regAddrL =  pSum[1];
		readSum = pSum[0]<<8|pSum[1];//高位在前
		SAFE_DELETE_ARRAY(pSum);
		if (readSum != calcSum)
		{
			m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readSum, calcSum);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FALSE;
		}
	}
	else
	{
		m_szInfo.Format(L"AWB sumSize设置错误:%02d", sumSize);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return bRet;
}

BOOL OtpCheck_EYD2467M::CheckSum_LSC(USHORT group)
{
	//m_pInterface->AddDebugLog(L"EYD2467M CheckSum_LSC");
	if (!m_bCheckSum_LSC) return TRUE;

	BOOL bRet = TRUE;
	USHORT regAddr = 0;
	USHORT regAddrH = 0;
	USHORT regAddrL = 0;
	USHORT valSize = 0;
	USHORT readSum = 0;
	USHORT sumSize = 0;
	USHORT calcSum = 0;
	USHORT redu = 0;

	// Calc sum
	regAddr = m_addrFlag.lsc;
	valSize = m_otpSize.lsc;
	redu = m_redu.lsc;
	USHORT* pVal = new USHORT[valSize];

	bRet &= ReadOTPData((regAddr+1)+(group-1)*(valSize+redu+1), (regAddr+1)+(valSize-1)+(group-1)*(valSize+redu+1), pVal);//+1为Flag
	
// 	if (!SaveLSCData(pVal, valSize))
// 	{
// 		return FALSE;
// 	}

	if (!CheckAbnormal(pVal, valSize+1))			
	{
		SAFE_DELETE_ARRAY(pVal);
		return FALSE;
	}

	// Read sum
	sumSize = m_Sumsize.lsc;
	calcSum = SumCalc(pVal, valSize, 256, 0);
	regAddr = m_addrSum.lsc;
	SAFE_DELETE_ARRAY(pVal);
	if (sumSize == 1)
	{

		bRet &= ReadOTPData(regAddr+((group-1)*(valSize+redu+1)),regAddr+((group-1)*(valSize+redu+1)), &readSum);//+1为CheckSum
		if (readSum != calcSum)
		{
			m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readSum, calcSum);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FALSE;
		}
	}
	else if (sumSize == 2)
	{
		USHORT* pSum = new USHORT[sumSize];
		bRet &= ReadOTPData(regAddr+((group-1)*(valSize+redu+2)),(regAddr+((group-1)*(valSize+redu+2)))+1, &readSum);//+1为CheckSum
		regAddrH = pSum[0];
		regAddrL = pSum[1];
		readSum  = pSum[0]<<8 | pSum[1];//高位在前
		SAFE_DELETE_ARRAY(pSum);
		if (readSum != calcSum)
		{
			m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readSum, calcSum);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FALSE;
		}
	}
	else
	{
		m_szInfo.Format(L"LSC sumSize设置错误:%02d", sumSize);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return bRet;
}

// BOOL OtpCheck_EYD2467M::SaveLSCData(USHORT *pVal, int valSize)
// {
// 	CString DirPath = m_pInterface->GetExeDir();
// 	CString Path ;
// 	Path.Format(_T("LSC Data_%d.txt"),m_pInterface->GetModuleNum());
// 	Path = DirPath+Path;
// 	FILE* file = NULL;
// 
// 	CStringA temp, num;
// 	for (int i = 0; i<valSize; i++)
// 	{
// 		num.Format("NO.%04d\t0x%02x\n", i+1,(unsigned char)pVal[i]);
// 		temp += num;
// 	}
// 
// 	char* p = temp.GetBuffer();
// 	if (_wfopen_s(&file,Path.GetBuffer(),_T("wt+"))==0)
// 	{
// 		fwrite(p,1,strlen(p),file);
// 		fclose(file);
// 		Path = Path+_T("saved");
// 		m_pInterface->AddLog(Path,COLOR_BLUE);
// 	}
// 	return TRUE;
// }


BOOL OtpCheck_EYD2467M::CheckAbnormal(USHORT *pVal, int valSize)
{
	//m_pInterface->AddDebugLog(L"EYD2467M CheckSum_LSC");
// 	if (!m_bCheckSum_LSC) return TRUE;
// 
// 	BOOL bRet = TRUE;
// 	// Calc sum
// 	USHORT regAddr = m_addrFlag.lsc;
// 	int valSize = m_otpSize.lsc;
// 	USHORT* pVal = new USHORT[valSize];
// 	bRet &= ReadSensor_MultiBytes(regAddr+1, m_regSize, pVal, valSize);

	//check 连续为0或ff判断为异常
	for(int i=4; i< valSize; i++)
	{
		if (i<=valSize-8)
		{	
			if (((pVal[i] == 0x00)&&(pVal[i-3] == 0x00)&&(pVal[i-2] == 0x00)&&(pVal[i-1] == 0x00)&&(pVal[i-4] == 0x00)) ||
				((pVal[i] == 0xff)&&(pVal[i-3] == 0xff)&&(pVal[i-2] == 0xff)&&(pVal[i-1] == 0xff)&&(pVal[i-4] == 0xff)))
			{
				m_szInfo.Format(L"LSC Data Abnormal!");
				m_pInterface->AddLog(m_szInfo, COLOR_RED);
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL OtpCheck_EYD2467M::CheckSum_AF(USHORT group)
{
	//m_pInterface->AddDebugLog(L"EYD2467M CheckSum_AF");
	if (!m_bCheckSum_AF) return TRUE;
	BOOL bRet = TRUE;
	USHORT regAddr = 0;
	USHORT regAddrH = 0;
	USHORT regAddrL = 0;
	USHORT valSize = 0;
	USHORT readSum = 0;
	USHORT sumSize = 0;
	USHORT calcSum = 0;
	USHORT redu = 0;

	// Calc sum
	regAddr = m_addrFlag.af;
	valSize = m_otpSize.af;
	redu = m_redu.af;
	USHORT* pVal = new USHORT[valSize];

	bRet &= ReadOTPData((regAddr+1)+(group-1)*(valSize+redu+2), (regAddr+1)+(valSize-1)+(group-1)*(valSize+redu+2), pVal);//+1为Flag

	// Read sum
	sumSize = m_Sumsize.af;
	calcSum = SumCalc(pVal, valSize,0xFFFF,1);
	regAddr = m_addrSum.af;

	SAFE_DELETE_ARRAY(pVal);
	if (sumSize == 1)
	{

		bRet &= ReadOTPData(regAddr+((group-1)*(valSize+redu+1)),regAddr+((group-1)*(valSize+redu+1)), &readSum);//+1为CheckSum
		
		if (readSum != calcSum)
		{
			m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readSum, calcSum);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FALSE;
		}
	}
	else if (sumSize == 2)
	{
		USHORT* pSum = new USHORT[sumSize];

		bRet &= ReadOTPData(regAddr+((group-1)*(valSize+redu+2)),(regAddr+((group-1)*(valSize+redu+2)))+1, pSum);//+1为CheckSum
		
		regAddrH = pSum[0];
		regAddrL =  pSum[1];
		readSum = pSum[0]<<8|pSum[1];//高位在前
		SAFE_DELETE_ARRAY(pSum);
		if (readSum != calcSum)
		{
			m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readSum, calcSum);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FALSE;
		}
	}
	else
	{
		m_szInfo.Format(L"AF sumSize设置错误:%02d", sumSize);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}
	
	return bRet;
}

BOOL OtpCheck_EYD2467M::CheckCode_Stc()
{
	//m_pInterface->AddDebugLog(L"EYD2467M CheckCode_Stc");
	if (!m_bCheckStc) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	USHORT readVal_H = 0, readVal_L = 0;
	USHORT readVal = 0;
	bRet &= ReadSensor(m_addrCode.stcH, &readVal_H);
	bRet &= ReadSensor(m_addrCode.stcL, &readVal_L);
	readVal = readVal_L | (readVal_H << 8);	// 三星格式
	if (readVal < m_stcLower ||
		readVal > m_stcUpper)
	{
		m_szInfo.Format(L"Code %d 不在范围 (%d~%d)", readVal, m_stcLower, m_stcUpper);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_EYD2467M::CheckCode_Inf()
{
	//m_pInterface->AddDebugLog(L"EYD2467M CheckCode_Inf");
	if (!m_bCheckInf) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	USHORT readVal_H = 0, readVal_L = 0;
	USHORT readVal = 0;
	bRet &= ReadOTPData(m_addrCode.infH,m_addrCode.infH, &readVal_H);
	bRet &= ReadOTPData(m_addrCode.infL,m_addrCode.infL, &readVal_L);
	readVal = readVal_L | (readVal_H << 8);	// 三星格式
	if (readVal < m_infLower ||
		readVal > m_infUpper)
	{
		m_szInfo.Format(L"Code %d 不在范围 (%d~%d)", readVal, m_infLower, m_infUpper);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_EYD2467M::CheckCode_Mac()
{
	//m_pInterface->AddDebugLog(L"EYD2467M CheckCode_Mac");
	if (!m_bCheckMac) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	USHORT readVal_H = 0, readVal_L = 0;
	USHORT readVal = 0;
	bRet &= ReadOTPData(m_addrCode.macH,m_addrCode.macH, &readVal_H);
	bRet &= ReadOTPData(m_addrCode.macL,m_addrCode.macL, &readVal_L);
	readVal = readVal_L | (readVal_H << 8);	// 三星格式
	if (readVal < m_macLower ||
		readVal > m_macUpper)
	{
		m_szInfo.Format(L"Code %d 不在范围 (%d~%d)", readVal, m_macLower, m_macUpper);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_EYD2467M::OTPInit()
{
	BOOL bRet = TRUE;
	Sleep(5);
	bRet &= WriteSensor( 0x0a02, 0x01 ); //Fast Sleep on
	bRet &= WriteSensor( 0x0a00, 0x00 ); //stand by on
	Sleep(15);
	bRet &= WriteSensor( 0x0f02, 0x00); //pll disable
	bRet &= WriteSensor( 0x071a, 0x01); //CP TRIM_H
	bRet &= WriteSensor( 0x071b, 0x09); //IPGM TRIM_H
	bRet &= WriteSensor( 0x0d04, 0x01); //Fsync(OTP busy)Output Enable
	bRet &= WriteSensor( 0x0d00, 0x07); //Fsync(OTP bust)Output Drivability
	bRet &= WriteSensor( 0x003e, 0x10); //OTP R/W mode
	bRet &= WriteSensor( 0x0a00, 0x01); //stand by off
	Sleep(5);

	return bRet;
}

BOOL OtpCheck_EYD2467M::OTPRelease()
{
	BOOL bRet = TRUE;
	bRet &= WriteSensor( 0x0a00, 0x00); //stand by on
	Sleep(15);
	bRet &= WriteSensor( 0x003e, 0x00); //display mode
	bRet &= WriteSensor( 0x0a00, 0x01); //stand by off
	Sleep(100);

	return bRet;
}

BOOL OtpCheck_EYD2467M::WriteOTPData(USHORT StartAddr,USHORT EndAddr, USHORT *Data)
{
	BOOL bRet = TRUE;
	int i=0, j=0;
	USHORT CheckData = 0;
	USHORT StartAddrH = (StartAddr>>8)&0xff;
	USHORT StartAddrL = StartAddr & 0xff;

	OTPInit();
	bRet &= WriteSensor( 0x070a, StartAddrH);
	bRet &= WriteSensor( 0x070b, StartAddrL);
	//Write mode设置5次机会
	for (i=0; i<5; i++)
	{
		if (WriteSensor(0x0702, 0x02) == DT_ERROR_OK)break;//Write Mode
	}
	if(i>=5)
	{
		m_szInfo.Format(L"Set Write mode 0x0702 0x02 Fail!");
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		OTPRelease();
		return FALSE;
	}
	//每个寄存器写入3次
	for (int i=0;  i<=(EndAddr-StartAddr); i++)
	{
		for (j=0; j<3;j++)
		{
			bRet &= WriteSensor(0x0706, Data[i]);
			Sleep(5);
			ReadSensor(0x070c,  &CheckData);
		}
		if (Data[i] == CheckData)
		{
			break;
		}
		else
		{
			//挽救
			USHORT Addr,AddrH,AddrL;
			Addr = StartAddr+i;              //重写
			AddrH = (Addr>>8)&0xFF;
			AddrL = Addr&0xFF;
			bRet &= WriteSensor(0x070a,AddrH);    
			bRet &= WriteSensor(0x070b,AddrL);
			bRet &= WriteSensor(0x0702,0x02);     //write mode
		}
		if (j>=3)
		{
			m_szInfo.Format(L"Write Register0x%2x fail!",StartAddr+i);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			OTPRelease();
			return FALSE;
		}
	}
	OTPRelease();
	return bRet;
}

BOOL OtpCheck_EYD2467M::ReadOTPData(USHORT StartAddr, USHORT EndAddr, USHORT *Data)
{
	CString info;
	
	BOOL bRet = TRUE;
	USHORT StartAddrH = (StartAddr>>8)&0xff;
	USHORT StartAddrL = StartAddr & 0xff;

	OTPInit();
	bRet &= WriteSensor( 0x070a, StartAddrH);
	bRet &= WriteSensor( 0x070b, StartAddrL);

	bRet &= WriteSensor( 0x0702, 0x01);	//Read Mode
	for (int i = 0; i<=EndAddr-StartAddr; i++)
	{
		ReadSensor(0x0708, Data+i);
// 		info.Format(_T("Read OTP Reg %04x, %02x"),StartAddr+i,Data[i]);
// 		m_pInterface->AddLog(info,COLOR_LIGHTSALMON);
	}
	OTPRelease();
	return bRet;
}

