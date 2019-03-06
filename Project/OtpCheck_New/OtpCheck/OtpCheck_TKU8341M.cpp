#include "StdAfx.h"
#include "OtpCheck_TKU8341M.h"


OtpCheck_TKU8341M::OtpCheck_TKU8341M(void)
{
	m_slaveSensor = 0x0;
	m_slaveEeprom = 0xB0;
	m_modeSensor = 0;
	m_modeEeprom = I2CMODE_16_8;
	m_regSize = 2;
	m_pageSize = 0x40;	// 0x00~0x3f
	m_flagValid	= 0x80;	// valid:0x80 empty: 0x00
	m_flagEmpty	= 0x00;	// valid:0x80 empty: 0x00
	
	m_addrFlag.moduleInfo	= 0x00;		// page 0
	m_addrFlag.awb			= 0x00;		// page 1
	m_addrFlag.lsc			= 0x00;		// page 2
	m_addrFlag.af			= 0x00;		// page 32
	m_addrFlag.pdaf			= 0x00;		// page 33
	m_addrFlag.pdafStep2Start	= 0x31;		// page 40

	m_addrSum.moduleInfo	= 0x15;		// page 0
	m_addrSum.awb			= 0x07;		// page 1
	m_addrSum.lsc			= 0x0d;		// page 31
	m_addrSum.af			= 0x05;		// page 32
	m_addrSum.pdafStep1		= 0x3d;		// page 54
	m_addrSum.pdafStep2		= 0x3e;		// page 54

	m_otpPage.moduleInfo	= 0;
	m_otpPage.awb			= 1;
	m_otpPage.lsc			= 2;	// 2 ~31
	m_otpPage.af			= 32;
	m_otpPage.pdafStep1		= 33;	// 33~40
	m_otpPage.pdafStep1_End = 41;
	m_otpPage.pdafStep2		= 40;	// 40~54
	m_otpPage.pdafStep2_End = 55;	// 55 end

	m_otpSize.moduleInfo	= m_addrSum.moduleInfo - m_addrFlag.moduleInfo - 1;
	m_otpSize.awb			= m_addrSum.awb - m_addrFlag.awb - 1;
	m_otpSize.lsc			= (m_otpPage.af - m_otpPage.lsc - 1)*m_pageSize - (m_addrFlag.lsc+1) + m_addrSum.lsc;//1868
	m_otpSize.af			= m_addrSum.af	- m_addrFlag.af - 1;
	m_otpSize.pdafStep1		= (m_otpPage.pdafStep1_End - m_otpPage.pdafStep1 - 1)*m_pageSize - (m_addrFlag.pdaf+1) + 0x31;//496
	m_otpSize.pdafStep2		= (m_otpPage.pdafStep2_End - m_otpPage.pdafStep2 - 1)*m_pageSize - (m_addrFlag.pdafStep2Start+1) + m_addrSum.pdafStep2;//908

	m_addrCode.stcL			= 0;
	m_addrCode.stcH			= 0;
	m_addrCode.infL			= 0x01;
	m_addrCode.infH			= 0x02;
	m_addrCode.macL			= 0x03;
	m_addrCode.macH			= 0x04;

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


OtpCheck_TKU8341M::~OtpCheck_TKU8341M(void)
{
}


BOOL OtpCheck_TKU8341M::Check_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"TKU8341M Check_ModuleInfo");

	BOOL bRet = TRUE;
	bRet &= CheckFlag_ModuleInfo();
	bRet &= CheckSum_ModuleInfo();

	return bRet;
}

BOOL OtpCheck_TKU8341M::Check_AWB()
{
	//m_pInterface->AddDebugLog(L"TKU8341M Check_AWB");

	BOOL bRet = TRUE;
	bRet &= CheckFlag_AWB();
	bRet &= CheckSum_AWB();

	return bRet;
}

BOOL OtpCheck_TKU8341M::Check_LSC()
{
	//m_pInterface->AddDebugLog(L"TKU8341M Check_LSC");

	BOOL bRet = TRUE;
	bRet &= CheckFlag_LSC();
	bRet &= CheckSum_LSC();

	return bRet;
}

BOOL OtpCheck_TKU8341M::Check_AF()
{
	//m_pInterface->AddDebugLog(L"TKU8341M Check_AF");

	BOOL bRet = TRUE;
	bRet &= CheckFlag_AF();
	bRet &= CheckSum_AF();
	bRet &= CheckCode_Stc();
	bRet &= CheckCode_Inf();
	bRet &= CheckCode_Mac();

	return bRet;
}

BOOL OtpCheck_TKU8341M::Check_PDAF()
{
	//m_pInterface->AddDebugLog(L"TKU8341M Check_PDAF");

	BOOL bRet = TRUE;
	bRet &= CheckFlag_PDAF();
	bRet &= CheckSum_PDAF_Step1();
	bRet &= CheckSum_PDAF_Step2();

	return bRet;
}

BYTE OtpCheck_TKU8341M::SumCalc(BYTE* pVal, int valSize)
{
	ULONGLONG sum = 0;
	for (int i = 0; i < valSize; i++)
	{
		sum += pVal[i];
	}
	sum = sum%255 + 1;
	return (BYTE)sum;
}

#ifdef IIC_SENSOR_TKU8341M
BOOL OtpCheck_TKU8341M::ReadSensor_Byte(WORD reg, BYTE* val)
{
	//m_pInterface->AddDebugLog(L"TKU8341M ReadSensor_Byte");
	return TRUE;
}

BOOL OtpCheck_TKU8341M::ReadSensor_Word(WORD reg, WORD* val)
{
	//m_pInterface->AddDebugLog(L"TKU8341M ReadSensor_Word");
	return TRUE;
}

BOOL OtpCheck_TKU8341M::WriteSensor_Byte(WORD reg, BYTE val)
{
	//m_pInterface->AddDebugLog(L"TKU8341M WriteSensor_Byte");
	return TRUE;
}

BOOL OtpCheck_TKU8341M::WriteSensor_Word(WORD reg, WORD val)
{
	//m_pInterface->AddDebugLog(L"TKU8341M WriteSensor_Word");
	return TRUE;
}

BYTE OtpCheck_TKU8341M::ReadSensor_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BOOL bStop /* = FALSE */)
{
	//m_pInterface->AddDebugLog(L"TKU8341M ReadSensor_MultiBytes");
	return TRUE;
}

BYTE OtpCheck_TKU8341M::WriteSensor_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize)
{
	//m_pInterface->AddDebugLog(L"TKU8341M WriteSensor_MultiBytes");
	return TRUE;
}

#else

BOOL OtpCheck_TKU8341M::ReadEepromPage_Byte(WORD reg, BYTE* val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"TKU8341M ReadEepromPage_Byte");

	BOOL bRet = TRUE;
	WORD readVal = 0;
	bRet &= m_pInterface->I2C_Read(m_slaveEeprom, ((EEPROM_Read<<14)|(page<<6)|(reg&0x3f)), &readVal, m_modeEeprom);
	*val = (BYTE)readVal;
	Sleep(1);

	return bRet;
}

BOOL OtpCheck_TKU8341M::WriteEepromPage_Byte(WORD reg, BYTE val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"TKU8341M WriteEepromPage_Byte");

	BOOL bRet = TRUE;
	bRet &= m_pInterface->I2C_Write(m_slaveEeprom, ((EEPROM_Read<<14)|(page<<6)|(reg&0x3f)), val, m_modeEeprom);

	return bRet;
}
/*
BOOL OtpCheck_TKU8341M::ReadEepromPage_Word(WORD reg, WORD* val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"TKU8341M ReadEepromPage_Word");

	BOOL bRet = TRUE;
	WORD readVal = 0;
	bRet &= m_pInterface->I2C_Read(m_slaveEeprom, ((EEPROM_Read<<14)|(page<<6)|(reg&0x3f)), &readVal, m_modeEeprom);
	*val = readVal;

	return bRet;
}

BOOL OtpCheck_TKU8341M::WriteEepromPage_Word(WORD reg, WORD val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"TKU8341M WriteEeprom_Word");

	BOOL bRet = TRUE;
	bRet &= m_pInterface->I2C_Write(m_slaveEeprom, reg, val, m_modeEeprom);

	return TRUE;
}
// */
BYTE OtpCheck_TKU8341M::ReadEepromPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop /* = FALSE */)
{
	//m_pInterface->AddDebugLog(L"TKU8341M ReadEepromPage_MultiBytes");

	BOOL bRet = TRUE;
	WORD curReg = 0;
	for (int i = 0, j = 0; i < valSize; i++)
	{	
		curReg = (reg + i)%m_pageSize;
		if ((curReg == 0) && 
			(reg != 0))	// 避免Reg从0开始，j进行自+
		{
			j++;
		}
		BYTE val = 0;
		bRet &= ReadEepromPage_Byte(curReg, &val, page+j);
		pVal[i] = val;
//		m_szInfo.Format(L"i:%04d reg:0x%02x, val:0x%02x, page:%04d", i, curReg, val, page+j);
//		m_pInterface->AddLog(m_szInfo);
	}

	return bRet;
}

BYTE OtpCheck_TKU8341M::WriteEepromPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page)
{
	//m_pInterface->AddDebugLog(L"TKU8341M WriteEeprom_MultiBytes");

	BOOL bRet = TRUE;
	WORD curReg = 0;
	for (int i = 0, j = 0; i < valSize; i++)
	{
		curReg = (reg + i)%m_pageSize;
		if ((curReg == 0) && 
			(reg != 0))	// 避免Reg从0开始，j进行自+
		{
			j++;
		}
		bRet &= WriteEepromPage_Byte(curReg, pVal[i], page+j);
	}

	return bRet;
}
#endif

BOOL OtpCheck_TKU8341M::CheckFlag_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"TKU8341M CheckFlag_ModuleInfo");
	if (!m_bCheckFlag_ModuleInfo) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	WORD reg = m_addrFlag.moduleInfo;
	BYTE page = m_otpPage.moduleInfo;
	bRet &= ReadEepromPage_Byte(reg, &readVal, page);
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return bRet;
}

BOOL OtpCheck_TKU8341M::CheckFlag_AWB()
{
	//m_pInterface->AddDebugLog(L"TKU8341M CheckFlag_AWB");
	if (!m_bCheckFlag_AWB) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	WORD reg = m_addrFlag.awb;
	BYTE page = m_otpPage.awb;
	bRet &= ReadEepromPage_Byte(reg, &readVal, page);
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return bRet;
}

BOOL OtpCheck_TKU8341M::CheckFlag_LSC()
{
	//m_pInterface->AddDebugLog(L"TKU8341M CheckFlag_LSC");
	if (!m_bCheckFlag_LSC) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	WORD reg = m_addrFlag.lsc;
	BYTE page = m_otpPage.lsc;
	bRet &= ReadEepromPage_Byte(reg, &readVal, page);
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_TKU8341M::CheckFlag_AF()
{
	//m_pInterface->AddDebugLog(L"TKU8341M CheckFlag_AF");
	if (!m_bCheckFlag_AF) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	WORD reg = m_addrFlag.af;
	BYTE page = m_otpPage.af;
	bRet &= ReadEepromPage_Byte(reg, &readVal, page);
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_TKU8341M::CheckFlag_PDAF()
{
	//m_pInterface->AddDebugLog(L"TKU8341M CheckFlag_PDAF_Step1");
	if (!m_bCheckFlag_PDAF) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	WORD reg = m_addrFlag.pdaf;
	BYTE page = m_otpPage.pdafStep1;
	bRet &= ReadEepromPage_Byte(reg, &readVal, page);
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_TKU8341M::CheckSum_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"TKU8341M CheckSum_ModuleInfo");
	if (!m_bCheckSum_ModuleInfo) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.moduleInfo;
	BYTE page = m_otpPage.moduleInfo;
	int valSize = m_otpSize.moduleInfo;

	BYTE* pVal = new BYTE[valSize];
	bRet &= ReadEepromPage_MultiBytes(regAddr+1, m_regSize, pVal, valSize, page);

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.moduleInfo;
	BYTE calcVal = SumCalc(pVal, valSize);
	bRet &= ReadEepromPage_Byte(regAddr, &readVal, page);
	SAFE_DELETE_ARRAY(pVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return bRet;
}

BOOL OtpCheck_TKU8341M::CheckSum_AWB()
{
	//m_pInterface->AddDebugLog(L"TKU8341M CheckSum_AWB");
	if (!m_bCheckSum_AWB) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.awb;
	BYTE page = m_otpPage.awb;
	int valSize = m_otpSize.awb;
	BYTE* pVal = new BYTE[valSize];
	bRet &= ReadEepromPage_MultiBytes(regAddr+1, m_regSize, pVal, valSize, page);

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.awb;
	BYTE calcVal = SumCalc(pVal, valSize);
	bRet &= ReadEepromPage_Byte(regAddr, &readVal, page);
	SAFE_DELETE_ARRAY(pVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_TKU8341M::CheckSum_LSC()
{
	//m_pInterface->AddDebugLog(L"TKU8341M CheckSum_LSC");
	if (!m_bCheckSum_LSC) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.lsc;
	BYTE page = m_otpPage.lsc;
	int valSize = m_otpSize.lsc;
	BYTE* pVal = new BYTE[valSize];
    m_pInterface->AddLog(L"Reading LSC Data...");
	bRet &= ReadEepromPage_MultiBytes(regAddr+1, m_regSize, pVal, valSize, page);

//	if (!SaveLSCData(pVal, valSize))
// 	{
// 		return FALSE;
// 	}

	if (!CheckAbnormal(pVal, valSize))
	{
		SAFE_DELETE_ARRAY(pVal);
		return FALSE;
	}

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.lsc;
	page = m_otpPage.af-1;
	BYTE calcVal = SumCalc(pVal, valSize);
	bRet &= ReadEepromPage_Byte(regAddr, &readVal, page);
	SAFE_DELETE_ARRAY(pVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

// BOOL OtpCheck_TKU8341M::SaveLSCData(BYTE *pVal, int valSize)
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

BOOL OtpCheck_TKU8341M::CheckAbnormal(BYTE *pVal, int valSize)
{
	//m_pInterface->AddDebugLog(L"TKU8341M CheckSum_LSC");
// 	if (!m_bCheckSum_LSC) return TRUE;
// 
// 	BOOL bRet = TRUE;
// 	// Calc sum
// 	WORD regAddr = m_addrFlag.lsc;
// 	BYTE page = m_otpPage.lsc;
// 	int valSize = m_otpSize.lsc;
// 	BYTE* pVal = new BYTE[valSize];
// 	bRet &= ReadEepromPage_MultiBytes(regAddr+1, m_regSize, pVal, valSize, page);

	//check 连续为0或ff判断为异常
	for(int i=3; i< valSize; i++)
	{
		if (i>68)
		{
			if(((pVal[i]==0)&&(pVal[i-3]==0)&&(pVal[i-2]==0)&&(pVal[i-1]==0))
				||((pVal[i]==0xff)&&(pVal[i-3]==0xff)&&(pVal[i-2]==0xff)&&(pVal[i-1]==0xff)))
			{
				m_szInfo.Format(L"LSC Data Abnormal!");
				m_pInterface->AddLog(m_szInfo, COLOR_RED);
				return FALSE;
			}
		}
	}
//	SAFE_DELETE_ARRAY(pVal);

	return TRUE;
}

BOOL OtpCheck_TKU8341M::CheckSum_AF()
{
	//m_pInterface->AddDebugLog(L"TKU8341M CheckSum_AF");
	if (!m_bCheckSum_AF) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.af;
	BYTE page = m_otpPage.af;
	int valSize = m_otpSize.af;
	BYTE* pVal = new BYTE[valSize];
	bRet &= ReadEepromPage_MultiBytes(regAddr+1, m_regSize, pVal, valSize, page);

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.af;
	BYTE calcVal = SumCalc(pVal, valSize);
	bRet &= ReadEepromPage_Byte(regAddr, &readVal, page);
	SAFE_DELETE_ARRAY(pVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
} 

BOOL OtpCheck_TKU8341M::CheckSum_PDAF_Step1()
{
	//m_pInterface->AddDebugLog(L"TKU8341M CheckSum_PDAF_Step1");
	if (!m_bCheckSum_PDAF) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.pdaf;
	BYTE page = m_otpPage.pdafStep1;
	int valSize = m_otpSize.pdafStep1;
	BYTE* pVal = new BYTE[valSize];
	bRet &= ReadEepromPage_MultiBytes(regAddr+1, m_regSize, pVal, valSize, page);

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.pdafStep1;
	page = m_otpPage.pdafStep2_End-1;
	BYTE calcVal = SumCalc(pVal, valSize);
	bRet &= ReadEepromPage_Byte(regAddr, &readVal, page);
	SAFE_DELETE_ARRAY(pVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_TKU8341M::CheckSum_PDAF_Step2()
{
	//m_pInterface->AddDebugLog(L"TKU8341M CheckSum_PDAF_Step2");
	if (!m_bCheckSum_PDAF) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.pdafStep2Start;
	BYTE page = m_otpPage.pdafStep2;
	int valSize = m_otpSize.pdafStep2;
	BYTE* pVal = new BYTE[valSize];
	bRet &= ReadEepromPage_MultiBytes(regAddr, m_regSize, pVal, valSize, page);

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.pdafStep2;
	page = m_otpPage.pdafStep2_End-1;
	BYTE calcVal = SumCalc(pVal, valSize);
	bRet &= ReadEepromPage_Byte(regAddr, &readVal, page);
	SAFE_DELETE_ARRAY(pVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_TKU8341M::CheckCode_Stc()
{
	//m_pInterface->AddDebugLog(L"TKU8341M CheckCode_Stc");
	if (!m_bCheckStc) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	BYTE page = m_otpPage.af;
	WORD readVal = 0;
	bRet &= ReadEepromPage_Byte(m_addrCode.stcH, &readVal_H, page);
	bRet &= ReadEepromPage_Byte(m_addrCode.stcL, &readVal_L, page);
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

BOOL OtpCheck_TKU8341M::CheckCode_Inf()
{
	//m_pInterface->AddDebugLog(L"TKU8341M CheckCode_Inf");
	if (!m_bCheckInf) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	BYTE page = m_otpPage.af;
	WORD readVal = 0;
	bRet &= ReadEepromPage_Byte(m_addrCode.infH, &readVal_H, page);
	bRet &= ReadEepromPage_Byte(m_addrCode.infL, &readVal_L, page);
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

BOOL OtpCheck_TKU8341M::CheckCode_Mac()
{
	//m_pInterface->AddDebugLog(L"TKU8341M CheckCode_Mac");
	if (!m_bCheckMac) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	BYTE page = m_otpPage.af;
	WORD readVal = 0;
	bRet &= ReadEepromPage_Byte(m_addrCode.macH, &readVal_H, page);
	bRet &= ReadEepromPage_Byte(m_addrCode.macL, &readVal_L, page);
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