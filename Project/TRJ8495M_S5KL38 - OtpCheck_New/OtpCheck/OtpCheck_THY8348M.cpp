#include "StdAfx.h"
#include "OtpCheck_THY8348M.h"


OtpCheck_THY8348M::OtpCheck_THY8348M(void)
{
	m_slaveSensor = 0x0;
	m_slaveEeprom = 0xB0;
	m_modeSensor = 0;
	m_modeEeprom = I2CMODE_16_8;
	m_regSize = 2;

	m_flagValid	= 0x01;// 0x01: Valid 其他：数据为空或无效

// 	memset(&m_addrFlag, 0, sizeof(m_addrFlag));
// 	memset(&m_addrSum, 0, sizeof(m_addrSum));
// 	memset(&m_otpSize, 0, sizeof(m_otpSize));
// 	memset(&m_addrCode, 0, sizeof(m_addrCode));
	memset(&m_otpPage, 0, sizeof(m_otpPage));

	m_addrFlag.moduleInfo	= 0x0000;
	m_addrFlag.awb			= 0x0010;
	m_addrFlag.lsc			= 0x0020;
	m_addrFlag.af			= 0x0740;
	m_addrFlag.pdaf			= 0x0780;
	m_addrFlag.pdaf_coef_k	= 0x0b40;

	m_addrSum.moduleInfo	= 0x000b;
	m_addrSum.awb			= 0x001D;
	m_addrSum.lsc			= 0x0709;
	m_addrSum.af			= 0x0745;
	m_addrSum.pdaf			= 0x0b03;
	m_addrSum.pdaf_coef_k	= 0x0b43;

	m_otpSize.moduleInfo	= m_addrSum.moduleInfo - m_addrFlag.moduleInfo - 1;
	m_otpSize.awb			= m_addrSum.awb - m_addrFlag.awb - 1;
	m_otpSize.lsc			= m_addrSum.lsc - m_addrFlag.lsc - 1;
	m_otpSize.af			= m_addrSum.af	- m_addrFlag.af - 1;
	m_otpSize.pdaf			= m_addrSum.pdaf - m_addrFlag.pdaf - 1;
	m_otpSize.pdaf_coef_k	= m_addrSum.pdaf_coef_k - m_addrFlag.pdaf_coef_k/* - 1*/;//Sum(0x0B41 ~ 0x0B43)%256

	m_addrCode.stcL			= 0;
	m_addrCode.stcH			= 0;
	m_addrCode.infL			= 0x0744;
	m_addrCode.infH			= 0x0743;
	m_addrCode.macL			= 0x0742;
	m_addrCode.macH			= 0x0741;

	m_bCheckStc = FALSE;	// 起始Code
	m_bCheckInf = FALSE;	// 远景Code
	m_bCheckMac = FALSE;	// 近景Code

	m_stcLower = 0;	// 起始Code下边界
	m_stcUpper = 0;	// 起始Code上边界
	m_infLower = 0;	//
	m_infUpper = 0;	//
	m_macUpper = 0;	//
	m_macLower = 0;	//
}


OtpCheck_THY8348M::~OtpCheck_THY8348M(void)
{
}


BOOL OtpCheck_THY8348M::Check_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"THY8348M Check_ModuleInfo");

	BOOL bRet = TRUE;
	bRet &= CheckFlag_ModuleInfo();
	bRet &= CheckSum_ModuleInfo();

	return bRet;
}

BOOL OtpCheck_THY8348M::Check_AWB()
{
	//m_pInterface->AddDebugLog(L"THY8348M Check_AWB");

	BOOL bRet = TRUE;
	bRet &= CheckFlag_AWB();
	bRet &= CheckSum_AWB();

	return bRet;
}

BOOL OtpCheck_THY8348M::Check_LSC()
{
	//m_pInterface->AddDebugLog(L"THY8348M Check_LSC");

	BOOL bRet = TRUE;
	bRet &= CheckFlag_LSC();
	bRet &= CheckSum_LSC();
	bRet &= CheckAbnormal();

	return bRet;
}

BOOL OtpCheck_THY8348M::Check_AF()
{
	//m_pInterface->AddDebugLog(L"THY8348M Check_AF");

	BOOL bRet = TRUE;
	bRet &= CheckFlag_AF();
	bRet &= CheckSum_AF();
	bRet &= CheckCode_Stc();
	bRet &= CheckCode_Inf();
	bRet &= CheckCode_Mac();

	return bRet;
}

BOOL OtpCheck_THY8348M::Check_PDAF()
{
	//m_pInterface->AddDebugLog(L"THY8348M Check_PDAF");

	BOOL bRet = TRUE;
	bRet &= CheckFlag_PDAF();
	bRet &= CheckSum_PDAF();
	bRet &= CheckFlag_PDAF_Coef_K();
//	bRet &= CheckSum_PDAF_Coef_K();

	return bRet;
}

BYTE OtpCheck_THY8348M::SumCalc(BYTE* pVal, int valSize)
{
	ULONGLONG sum = 0;
	for (int i = 0; i < valSize; i++)
	{
		sum += pVal[i];
	}
	sum %= 256;
	return (BYTE)sum;
}

#ifdef IIC_SENSOR_THY8348M
BOOL OtpCheck_THY8348M::ReadSensor_Byte(WORD reg, BYTE* val)
{
	//m_pInterface->AddDebugLog(L"THY8348M ReadSensor_Byte");
	return TRUE;
}

BOOL OtpCheck_THY8348M::ReadSensor_Word(WORD reg, WORD* val)
{
	//m_pInterface->AddDebugLog(L"THY8348M ReadSensor_Word");
	return TRUE;
}

BOOL OtpCheck_THY8348M::WriteSensor_Byte(WORD reg, BYTE val)
{
	//m_pInterface->AddDebugLog(L"THY8348M WriteSensor_Byte");
	return TRUE;
}

BOOL OtpCheck_THY8348M::WriteSensor_Word(WORD reg, WORD val)
{
	//m_pInterface->AddDebugLog(L"THY8348M WriteSensor_Word");
	return TRUE;
}

BYTE OtpCheck_THY8348M::ReadSensor_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BOOL bStop /* = FALSE */)
{
	//m_pInterface->AddDebugLog(L"THY8348M ReadSensor_MultiBytes");
	return TRUE;
}

BYTE OtpCheck_THY8348M::WriteSensor_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize)
{
	//m_pInterface->AddDebugLog(L"THY8348M WriteSensor_MultiBytes");
	return TRUE;
}
#else
BOOL OtpCheck_THY8348M::ReadEeprom_Byte(WORD reg, BYTE* val)
{
	//m_pInterface->AddDebugLog(L"THY8348M ReadEeprom_Byte");

	BOOL bRet = TRUE;
	WORD readVal = 0;
	bRet &= m_pInterface->I2C_Read(m_slaveEeprom, reg, &readVal, m_modeEeprom);
	*val = (BYTE)readVal;

	return bRet;
}

BOOL OtpCheck_THY8348M::WriteEeprom_Byte(WORD reg, BYTE val)
{
	//m_pInterface->AddDebugLog(L"THY8348M WriteEeprom_Byte");

	BOOL bRet = TRUE;
	WORD readVal = 0;
	bRet &= m_pInterface->I2C_Write(m_slaveEeprom, reg, val, m_modeEeprom);

	return bRet;
}
/*
BOOL OtpCheck_THY8348M::ReadEeprom_Word(WORD reg, WORD* val)
{
	//m_pInterface->AddDebugLog(L"THY8348M ReadEeprom_Word");

	BOOL bRet = TRUE;
	WORD readVal = 0;
	bRet &= m_pInterface->I2C_Read(m_slaveEeprom, reg, &readVal, m_modeEeprom);
	*val = readVal;

	return bRet;
}

BOOL OtpCheck_THY8348M::WriteEeprom_Word(WORD reg, WORD val)
{
	//m_pInterface->AddDebugLog(L"THY8348M WriteEeprom_Word");

	BOOL bRet = TRUE;
	WORD readVal = 0;
	bRet &= m_pInterface->I2C_Write(m_slaveEeprom, reg, val, m_modeEeprom);

	return TRUE;
}
*/
BYTE OtpCheck_THY8348M::ReadEeprom_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BOOL bStop /* = FALSE */)
{
	//m_pInterface->AddDebugLog(L"THY8348M ReadEeprom_MultiBytes");

	BOOL bRet = TRUE;
	for (int i = 0; i < valSize; i++)
	{
		BYTE val = 0;
		bRet &= ReadEeprom_Byte(reg + i, &val);
		pVal[i] = val;
	}

	return bRet;
}

BYTE OtpCheck_THY8348M::WriteEeprom_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize)
{
	//m_pInterface->AddDebugLog(L"THY8348M WriteEeprom_MultiBytes");

	BOOL bRet = TRUE;
	for (int i = 0; i < valSize; i++)
	{
		BYTE val = 0;
		bRet &= WriteEeprom_Byte(reg + i, pVal[i]);
	}

	return bRet;
}
#endif

BOOL OtpCheck_THY8348M::CheckFlag_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckFlag_ModuleInfo");
	if (!m_bCheckFlag_ModuleInfo) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	bRet &= ReadEeprom_Byte(m_addrFlag.moduleInfo, &readVal);
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return bRet;
}

BOOL OtpCheck_THY8348M::CheckFlag_AWB()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckFlag_AWB");
	if (!m_bCheckFlag_AWB) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	bRet &= ReadEeprom_Byte(m_addrFlag.awb, &readVal);
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return bRet;
}

BOOL OtpCheck_THY8348M::CheckFlag_LSC()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckFlag_LSC");
	if (!m_bCheckFlag_LSC) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	bRet &= ReadEeprom_Byte(m_addrFlag.lsc, &readVal);
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_THY8348M::CheckFlag_AF()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckFlag_AF");
	if (!m_bCheckFlag_AF) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	bRet &= ReadEeprom_Byte(m_addrFlag.af, &readVal);
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_THY8348M::CheckFlag_PDAF()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckFlag_PDAF");
	if (!m_bCheckFlag_PDAF) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	bRet &= ReadEeprom_Byte(m_addrFlag.pdaf, &readVal);
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_THY8348M::CheckFlag_PDAF_Coef_K()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckFlag_PDAF_Coef_K");
	if (!m_bCheckFlag_PDAF) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	bRet &= ReadEeprom_Byte(m_addrFlag.pdaf_coef_k, &readVal);
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_THY8348M::CheckSum_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckSum_ModuleInfo");
	if (!m_bCheckSum_ModuleInfo) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.moduleInfo;
	int valSize = m_otpSize.moduleInfo;
	BYTE* pVal = new BYTE[valSize];
	bRet &= ReadEeprom_MultiBytes(regAddr+1, m_regSize, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	BYTE calcVal = SumCalc(pVal, valSize);
	regAddr = m_addrSum.moduleInfo;
	bRet &= ReadEeprom_Byte(regAddr, &readVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}
	SAFE_DELETE_ARRAY(pVal);

	return bRet;
}

BOOL OtpCheck_THY8348M::CheckSum_AWB()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckSum_AWB");
	if (!m_bCheckSum_AWB) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.awb;
	int valSize = m_otpSize.awb;
	BYTE* pVal = new BYTE[valSize];
	bRet &= ReadEeprom_MultiBytes(regAddr+1, m_regSize, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	BYTE calcVal = SumCalc(pVal, valSize);
	regAddr = m_addrSum.awb;
	bRet &= ReadEeprom_Byte(regAddr, &readVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}
	SAFE_DELETE_ARRAY(pVal);

	return TRUE;
}

BOOL OtpCheck_THY8348M::CheckSum_LSC()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckSum_LSC");
	if (!m_bCheckSum_LSC) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.lsc;
	int valSize = m_otpSize.lsc;
	BYTE* pVal = new BYTE[valSize];
	bRet &= ReadEeprom_MultiBytes(regAddr+1, m_regSize, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	BYTE calcVal = SumCalc(pVal, valSize);
	regAddr = m_addrSum.lsc;
	bRet &= ReadEeprom_Byte(regAddr, &readVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}
	SAFE_DELETE_ARRAY(pVal);

	return TRUE;
}

BOOL OtpCheck_THY8348M::CheckAbnormal()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckSum_LSC");
	if (!m_bCheckSum_LSC) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.lsc;
	int valSize = m_otpSize.lsc;
	BYTE* pVal = new BYTE[valSize];
	bRet &= ReadEeprom_MultiBytes(regAddr+1, m_regSize, pVal, valSize);

	//check 连续为0或ff判断为异常
	for(int i=3; i< valSize; i++)
	{
		if(((pVal[i]==0)&&(pVal[i-3]==0)&&(pVal[i-2]==0)&&(pVal[i-1]==0))
		||((pVal[i]==0xff)&&(pVal[i-3]==0xff)&&(pVal[i-2]==0xff)&&(pVal[i-1]==0xff)))
		{
			m_szInfo.Format(L"LSC Data Abnormal!");
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FALSE;
		}
	}
	SAFE_DELETE_ARRAY(pVal);

	return TRUE;
}

BOOL OtpCheck_THY8348M::CheckSum_AF()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckSum_AF");
	if (!m_bCheckSum_AF) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.af;
	int valSize = m_otpSize.af;
	BYTE* pVal = new BYTE[valSize];
	bRet &= ReadEeprom_MultiBytes(regAddr+1, m_regSize, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	BYTE calcVal = SumCalc(pVal, valSize);
	regAddr = m_addrSum.af;
	bRet &= ReadEeprom_Byte(regAddr, &readVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}
	SAFE_DELETE_ARRAY(pVal);

	return TRUE;
}

BOOL OtpCheck_THY8348M::CheckSum_PDAF()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckSum_PDAF");
	if (!m_bCheckSum_PDAF) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.pdaf;
	int valSize = m_otpSize.pdaf;
	BYTE* pVal = new BYTE[valSize];
	bRet &= ReadEeprom_MultiBytes(regAddr+1, m_regSize, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	BYTE calcVal = SumCalc(pVal, valSize);
	regAddr = m_addrSum.pdaf;
	bRet &= ReadEeprom_Byte(regAddr, &readVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}
	SAFE_DELETE_ARRAY(pVal);

	return TRUE;
}

BOOL OtpCheck_THY8348M::CheckSum_PDAF_Coef_K()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckSum_PDAF_Coef_K");
	if (!m_bCheckSum_PDAF) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.pdaf_coef_k;
	int valSize = m_otpSize.pdaf_coef_k;
	BYTE* pVal = new BYTE[valSize];
	bRet &= ReadEeprom_MultiBytes(regAddr+1, m_regSize, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	BYTE calcVal = SumCalc(pVal, valSize);
	regAddr = m_addrSum.pdaf_coef_k;
	bRet &= ReadEeprom_Byte(regAddr, &readVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}
	SAFE_DELETE_ARRAY(pVal);

	return TRUE;
}

BOOL OtpCheck_THY8348M::CheckCode_Stc()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckCode_Stc");
	if (!m_bCheckStc) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	WORD readVal = 0;
	bRet &= ReadEeprom_Byte(m_addrCode.stcH, &readVal_H);
	bRet &= ReadEeprom_Byte(m_addrCode.stcL, &readVal_L);
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

BOOL OtpCheck_THY8348M::CheckCode_Inf()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckCode_Inf");
	if (!m_bCheckInf) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	WORD readVal = 0;
	bRet &= ReadEeprom_Byte(m_addrCode.infH, &readVal_H);
	bRet &= ReadEeprom_Byte(m_addrCode.infL, &readVal_L);
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

BOOL OtpCheck_THY8348M::CheckCode_Mac()
{
	//m_pInterface->AddDebugLog(L"THY8348M CheckCode_Mac");
	if (!m_bCheckMac) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	WORD readVal = 0;
	bRet &= ReadEeprom_Byte(m_addrCode.macH, &readVal_H);
	bRet &= ReadEeprom_Byte(m_addrCode.macL, &readVal_L);
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