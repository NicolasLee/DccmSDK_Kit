#include "StdAfx.h"
#include "OtpCheck_TRJ8495M.h"


OtpCheck_TRJ8495M::OtpCheck_TRJ8495M(void)
{
	m_slaveSensor = 0x0;
	m_slaveEeprom = 0xB0;
	m_modeSensor = 0;
	m_modeEeprom = I2CMODE_16_8;
	m_regSize = 2;
//	m_pageSize = 0x40;	// 0x00~0x3f
	m_flagValid	= 0x80;	// valid:0x80 empty: 0x00
	m_flagEmpty	= 0x00;	// valid:0x80 empty: 0x00
	
	m_addrFlag.moduleInfo	= 0x0000;		
	m_addrFlag.awb			= 0x0040;		
	m_addrFlag.lsc			= 0x0080;		
	m_addrFlag.af			= 0x0800;		
	m_addrFlag.pdaf			= 0x0840;		
	m_addrFlag.pdafStep2Start	= 0x0A31;		

	m_addrSum.moduleInfo	= 0x0015;		
	m_addrSum.awb			= 0x0055;		
	m_addrSum.lsc			= 0x07cd;		
	m_addrSum.af			= 0x0805;		
	m_addrSum.pdafStep1		= 0x0dbd;		
	m_addrSum.pdafStep2		= 0x0dbe;		

// 	m_otpPage.moduleInfo	= 0;
// 	m_otpPage.awb			= 1;
// 	m_otpPage.lsc			= 2;	// 2 ~31
// 	m_otpPage.af			= 32;
// 	m_otpPage.pdafStep1		= 33;	// 33~40
// 	m_otpPage.pdafStep1_End = 41;
// 	m_otpPage.pdafStep2		= 40;	// 40~54
// 	m_otpPage.pdafStep2_End = 55;	// 55 end

	m_otpSize.moduleInfo	= m_addrSum.moduleInfo - m_addrFlag.moduleInfo - 1;
	m_otpSize.awb			= m_addrSum.awb - m_addrFlag.awb - 1;
	m_otpSize.lsc			= m_addrSum.lsc - m_addrFlag.lsc - 1;
	m_otpSize.af			= m_addrSum.af	- m_addrFlag.af - 1;
	m_otpSize.pdafStep1		= 496;//496
	m_otpSize.pdafStep2		= 908;//908

	m_addrCode.stcL			= 0;
	m_addrCode.stcH			= 0;
	m_addrCode.infL			= 0x0801;
	m_addrCode.infH			= 0x0802;
	m_addrCode.macL		= 0x0803;
	m_addrCode.macH		= 0x0804;

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


OtpCheck_TRJ8495M::~OtpCheck_TRJ8495M(void)
{
}


BOOL OtpCheck_TRJ8495M::Check_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M Check_ModuleInfo");

	BOOL bRet = TRUE;
	bRet &= CheckFlag(m_bCheckFlag_ModuleInfo,m_addrFlag.moduleInfo,_T("Basic"));
//	bRet &= CheckFlag_ModuleInfo();
	bRet &= CheckSum_ModuleInfo();

	return bRet;
}

BOOL OtpCheck_TRJ8495M::Check_AWB()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M Check_AWB");

	BOOL bRet = TRUE;
	bRet &= CheckFlag(m_bCheckFlag_AWB,m_addrFlag.awb,_T("AWB"));
//	bRet &= CheckFlag_AWB();
	bRet &= CheckSum_AWB();

	return bRet;
}

BOOL OtpCheck_TRJ8495M::Check_LSC()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M Check_LSC");

	BOOL bRet = TRUE;
	bRet &= CheckFlag(m_bCheckFlag_LSC,m_addrFlag.lsc,_T("LSC"));
//	bRet &= CheckFlag_LSC();
	bRet &= CheckSum_LSC();

	return bRet;
}

BOOL OtpCheck_TRJ8495M::Check_AF()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M Check_AF");

	BOOL bRet = TRUE;
	bRet &= CheckFlag(m_bCheckFlag_AF,m_addrFlag.af,_T("AF"));
//	bRet &= CheckFlag_AF();
	bRet &= CheckSum_AF();
	bRet &= CheckCode_Stc();
	bRet &= CheckCode_Inf();
	bRet &= CheckCode_Mac();

	return bRet;
}

BOOL OtpCheck_TRJ8495M::Check_PDAF()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M Check_PDAF");

	BOOL bRet = TRUE;
	bRet &= CheckFlag(m_bCheckFlag_PDAF,m_addrFlag.pdaf,_T("PDAF"));
//	bRet &= CheckFlag_PDAF();
	bRet &= CheckSum_PDAF_Step1();
	bRet &= CheckSum_PDAF_Step2();

	return bRet;
}

BYTE OtpCheck_TRJ8495M::SumCalc(BYTE* pVal, int valSize)
{
	ULONGLONG sum = 0;
	for (int i = 0; i < valSize; i++)
	{
		sum += pVal[i];
	}
	sum = sum%255 + 1;
	return (BYTE)sum;
}

#ifdef IIC_SENSOR_TRJ8495M
BOOL OtpCheck_TRJ8495M::ReadSensor_Byte(WORD reg, BYTE* val)
{
	//m_pInterface->AddDebugLog(L"TRJ8495M ReadSensor_Byte");
	return TRUE;
}

BOOL OtpCheck_TRJ8495M::ReadSensor_Word(WORD reg, WORD* val)
{
	//m_pInterface->AddDebugLog(L"TRJ8495M ReadSensor_Word");
	return TRUE;
}

BOOL OtpCheck_TRJ8495M::WriteSensor_Byte(WORD reg, BYTE val)
{
	//m_pInterface->AddDebugLog(L"TRJ8495M WriteSensor_Byte");
	return TRUE;
}

BOOL OtpCheck_TRJ8495M::WriteSensor_Word(WORD reg, WORD val)
{
	//m_pInterface->AddDebugLog(L"TRJ8495M WriteSensor_Word");
	return TRUE;
}

BYTE OtpCheck_TRJ8495M::ReadSensor_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BOOL bStop /* = FALSE */)
{
	//m_pInterface->AddDebugLog(L"TRJ8495M ReadSensor_MultiBytes");
	return TRUE;
}

BYTE OtpCheck_TRJ8495M::WriteSensor_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize)
{
	//m_pInterface->AddDebugLog(L"TRJ8495M WriteSensor_MultiBytes");
	return TRUE;
}

#else
BOOL OtpCheck_TRJ8495M::ReadEeprom_Byte(WORD reg, BYTE* val)
{
	//m_pInterface->AddDebugLog(L"THY8348M ReadEeprom_Byte");

	BOOL bRet = TRUE;
	WORD readVal = 0;
	bRet &= m_pInterface->I2C_Read(m_slaveEeprom, reg, &readVal, m_modeEeprom);
	*val = (BYTE)readVal;

	return bRet;
}

BOOL OtpCheck_TRJ8495M::WriteEeprom_Byte(WORD reg, BYTE val)
{
	//m_pInterface->AddDebugLog(L"THY8348M WriteEeprom_Byte");

	BOOL bRet = TRUE;
	WORD readVal = 0;
	bRet &= m_pInterface->I2C_Write(m_slaveEeprom, reg, val, m_modeEeprom);

	return bRet;
}
// BOOL OtpCheck_TRJ8495M::ReadEeprom_Byte(WORD reg, BYTE* val, BYTE page)
// {
// 	//m_pInterface->AddDebugLog(L"TRJ8495M ReadEeprom_Byte");
// 
// 	BOOL bRet = TRUE;
// 	WORD readVal = 0;
// 	bRet &= m_pInterface->I2C_Read(m_slaveEeprom, ((EEPROM_Read<<14)|(page<<6)|(reg&0x3f)), &readVal, m_modeEeprom);
// 	*val = (BYTE)readVal;
// 	Sleep(1);
// 
// 	return bRet;
// }
// 
// BOOL OtpCheck_TRJ8495M::WriteEepromPage_Byte(WORD reg, BYTE val, BYTE page)
// {
// 	//m_pInterface->AddDebugLog(L"TRJ8495M WriteEepromPage_Byte");
// 
// 	BOOL bRet = TRUE;
// 	bRet &= m_pInterface->I2C_Write(m_slaveEeprom, ((EEPROM_Read<<14)|(page<<6)|(reg&0x3f)), val, m_modeEeprom);
// 
// 	return bRet;
// }
/*
BOOL OtpCheck_TRJ8495M::ReadEepromPage_Word(WORD reg, WORD* val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"TRJ8495M ReadEepromPage_Word");

	BOOL bRet = TRUE;
	WORD readVal = 0;
	bRet &= m_pInterface->I2C_Read(m_slaveEeprom, ((EEPROM_Read<<14)|(page<<6)|(reg&0x3f)), &readVal, m_modeEeprom);
	*val = readVal;

	return bRet;
}

BOOL OtpCheck_TRJ8495M::WriteEepromPage_Word(WORD reg, WORD val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"TRJ8495M WriteEeprom_Word");

	BOOL bRet = TRUE;
	bRet &= m_pInterface->I2C_Write(m_slaveEeprom, reg, val, m_modeEeprom);

	return TRUE;
}
*/
// BYTE OtpCheck_TRJ8495M::ReadEepromPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop /* = FALSE */)
// {
// 	//m_pInterface->AddDebugLog(L"TRJ8495M ReadEepromPage_MultiBytes");
// 
// 	BOOL bRet = TRUE;
// 	WORD curReg = 0;
// 	for (int i = 0, j = 0; i < valSize; i++)
// 	{	
// 		curReg = (reg + i)%m_pageSize;
// 		if ((curReg == 0) && 
// 			(reg != 0))	// 避免Reg从0开始，j进行自+
// 		{
// 			j++;
// 		}
// 		BYTE val = 0;
// 		bRet &= ReadEeprom_Byte(curReg, &val, page+j);
// 		pVal[i] = val;
// //		m_szInfo.Format(L"i:%04d reg:0x%02x, val:0x%02x, page:%04d", i, curReg, val, page+j);
// //		m_pInterface->AddLog(m_szInfo);
// 	}
// 
// 	return bRet;
// }

// BYTE OtpCheck_TRJ8495M::WriteEepromPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page)
// {
// 	//m_pInterface->AddDebugLog(L"TRJ8495M WriteEeprom_MultiBytes");
// 
// 	BOOL bRet = TRUE;
// 	WORD curReg = 0;
// 	for (int i = 0, j = 0; i < valSize; i++)
// 	{
// 		curReg = (reg + i)%m_pageSize;
// 		if ((curReg == 0) && 
// 			(reg != 0))	// 避免Reg从0开始，j进行自+
// 		{
// 			j++;
// 		}
// 		bRet &= WriteEepromPage_Byte(curReg, pVal[i], page+j);
// 	}
// 
// 	return bRet;
// }
#endif

BOOL OtpCheck_TRJ8495M::CheckFlag_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M CheckFlag_ModuleInfo");
	if (!m_bCheckFlag_ModuleInfo) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	WORD reg = m_addrFlag.moduleInfo;
	bRet &= ReadEeprom_Byte(reg, &readVal);
	if (readVal == m_flagEmpty)
	{
		m_szInfo.Format(L"模组OTP未烧录!");
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return bRet;
}

BOOL OtpCheck_TRJ8495M::CheckFlag_AWB()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M CheckFlag_AWB");
	if (!m_bCheckFlag_AWB) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	WORD reg = m_addrFlag.awb;
	bRet &= ReadEeprom_Byte(reg, &readVal);
	if (readVal == m_flagEmpty)
	{
		m_szInfo.Format(L"模组AWB未烧录!");
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return bRet;
}

BOOL OtpCheck_TRJ8495M::CheckFlag_LSC()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M CheckFlag_LSC");
	if (!m_bCheckFlag_LSC) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	WORD reg = m_addrFlag.lsc;
	bRet &= ReadEeprom_Byte(reg, &readVal);
	if (readVal == m_flagEmpty)
	{
		m_szInfo.Format(L"模组LSC未烧录!");
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_TRJ8495M::CheckFlag_AF()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M CheckFlag_AF");
	if (!m_bCheckFlag_AF) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	WORD reg = m_addrFlag.af;
	bRet &= ReadEeprom_Byte(reg, &readVal);
	if (readVal == m_flagEmpty)
	{
		m_szInfo.Format(L"模组AF未烧录!");
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_TRJ8495M::CheckFlag_PDAF()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M CheckFlag_PDAF_Step1");
	if (!m_bCheckFlag_PDAF) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	WORD reg = m_addrFlag.pdaf;
	bRet &= ReadEeprom_Byte(reg, &readVal);
	if (readVal == m_flagEmpty)
	{
		m_szInfo.Format(L"模组PDAF未烧录!");
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_TRJ8495M::CheckFlag(BOOL flag,int addr,CString str)
{
	//m_pInterface->AddDebugLog(L"TRJ8495M CheckFlag_PDAF_Step1");
	if (!flag) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal = 0;
	WORD reg = addr;
	bRet &= ReadEeprom_Byte(reg, &readVal);
	if (readVal == m_flagEmpty)
	{
		m_szInfo.Format(L"模组%s未烧录!",str);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}
	if (readVal != m_flagValid)
	{
		m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_TRJ8495M::CheckSum_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M CheckSum_ModuleInfo");
	if (!m_bCheckSum_ModuleInfo) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.moduleInfo;
	int valSize = m_otpSize.moduleInfo;

	BYTE* pVal = new BYTE[valSize];
	bRet &= DW9763_Read(regAddr+1, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.moduleInfo;
	BYTE calcVal = SumCalc(pVal, valSize);
	bRet &= ReadEeprom_Byte(regAddr, &readVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		SAFE_DELETE_ARRAY(pVal);
		return FALSE;
	}
	SAFE_DELETE_ARRAY(pVal);

	return bRet;
}

BOOL OtpCheck_TRJ8495M::CheckSum_AWB()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M CheckSum_AWB");
	if (!m_bCheckSum_AWB) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.awb;
	int valSize = m_otpSize.awb;

	BYTE* pVal = new BYTE[valSize];
	bRet &= DW9763_Read(regAddr+1, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.awb;
	BYTE calcVal = SumCalc(pVal, valSize);
	bRet &= ReadEeprom_Byte(regAddr, &readVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		SAFE_DELETE_ARRAY(pVal);
		return FALSE;
	}
	SAFE_DELETE_ARRAY(pVal);

	return TRUE;
}

BOOL OtpCheck_TRJ8495M::CheckSum_LSC()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M CheckSum_LSC");
	if (!m_bCheckSum_LSC) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.lsc;
	int valSize = m_otpSize.lsc;
	BYTE* pVal = new BYTE[valSize];
    m_pInterface->AddLog(L"Reading LSC Data...");
	bRet &= DW9763_Read(regAddr+1, pVal, valSize);

// 	if (!SaveLSCData(pVal, valSize))
// 	{
// 		return FALSE;
// 	}

	if (!CheckAbnormal(pVal, valSize))
	{
		return FALSE;
	}

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.lsc;
	BYTE calcVal = SumCalc(pVal, valSize);
	bRet &= ReadEeprom_Byte(regAddr, &readVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		SAFE_DELETE_ARRAY(pVal);
		return FALSE;
	}
	SAFE_DELETE_ARRAY(pVal);

	return TRUE;
}

BOOL OtpCheck_TRJ8495M::SaveLSCData(BYTE *pVal, int valSize)
{
	CString DirPath = m_pInterface->GetExeDir();
	CString Path ;
	Path.Format(_T("LSC Data_%d.txt"),m_pInterface->GetModuleNum());
	Path = DirPath+Path;
	FILE* file = NULL;

	CStringA temp, num;
	for (int i = 0; i<valSize; i++)
	{
		num.Format("NO.%04d\t0x%02x\n", i+1,(unsigned char)pVal[i]);
		temp += num;
	}

	char* p = temp.GetBuffer();
	if (_wfopen_s(&file,Path.GetBuffer(),_T("wt+"))==0)
	{
		fwrite(p,1,strlen(p),file);
		fclose(file);
		Path = Path+_T("saved");
		m_pInterface->AddLog(Path,COLOR_BLUE);
	}
	return TRUE;
}

BOOL OtpCheck_TRJ8495M::SaveOTPData()
{
	m_pInterface->AddLog(_T("Read OTP Start!"));
	///////////////////////////////Read OTP////////////////////////////////////
	int valSize = 2054;
	BYTE* pVal = new BYTE[valSize];
	if (!DW9763_Read(0000, pVal, valSize))
	{
		m_pInterface->AddLog(_T("Read OTP Fail!"),COLOR_RED);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	CString DirPath = m_pInterface->GetExeDir();
	CString Path ;
	Path.Format(_T("OTP Data_%d.txt"),m_pInterface->GetModuleNum());
	Path = DirPath+Path;
	FILE* file = NULL;

	CStringA temp, num;
	for (int i = 0; i<valSize; i++)
	{
		num.Format("%04d\t0x%04x,0x%02x\n", i+1,i,(unsigned char)pVal[i]);
		temp += num;
	}

	char* p = temp.GetBuffer();
	if (_wfopen_s(&file,Path.GetBuffer(),_T("wt+"))==0)
	{
		fwrite(p,1,strlen(p),file);
		fclose(file);
		Path = Path+_T(" saved");
		m_pInterface->AddLog(Path,COLOR_BLUE);
	}

	m_pInterface->AddLog(_T("Read OTP End!"));
	return TRUE;
}

BOOL OtpCheck_TRJ8495M::CheckAbnormal(BYTE *pVal, int valSize)
{
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

	return TRUE;
}

BOOL OtpCheck_TRJ8495M::CheckSum_AF()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M CheckSum_AF");
	if (!m_bCheckSum_AF) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.af;

	int valSize = m_otpSize.af;
	BYTE* pVal = new BYTE[valSize];
	bRet &= DW9763_Read(regAddr+1, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.af;
	BYTE calcVal = SumCalc(pVal, valSize);
	bRet &= ReadEeprom_Byte(regAddr, &readVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		SAFE_DELETE_ARRAY(pVal);
		return FALSE;
	}
	SAFE_DELETE_ARRAY(pVal);

	return TRUE;
} 

BOOL OtpCheck_TRJ8495M::CheckSum_PDAF_Step1()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M CheckSum_PDAF_Step1");
	if (!m_bCheckSum_PDAF) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.pdaf;
	int valSize = m_otpSize.pdafStep1;
	BYTE* pVal = new BYTE[valSize];
	bRet &= DW9763_Read(regAddr+1, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.pdafStep1;
	BYTE calcVal = SumCalc(pVal, valSize);
	bRet &= ReadEeprom_Byte(regAddr, &readVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		SAFE_DELETE_ARRAY(pVal);
		return FALSE;
	}
	SAFE_DELETE_ARRAY(pVal);

	return TRUE;
}

BOOL OtpCheck_TRJ8495M::CheckSum_PDAF_Step2()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M CheckSum_PDAF_Step2");
	if (!m_bCheckSum_PDAF) return TRUE;

	BOOL bRet = TRUE;
	// Calc sum
	WORD regAddr = m_addrFlag.pdafStep2Start;
	int valSize = m_otpSize.pdafStep2;
	BYTE* pVal = new BYTE[valSize];
	bRet &= DW9763_Read(regAddr, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.pdafStep2;
	BYTE calcVal = SumCalc(pVal, valSize);
	bRet &= ReadEeprom_Byte(regAddr, &readVal);
	if (readVal != calcVal)
	{
		m_szInfo.Format(L"read sum 0x%02x != calc sum 0x%02x", readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		SAFE_DELETE_ARRAY(pVal);
		return FALSE;
	}
	SAFE_DELETE_ARRAY(pVal);

	return TRUE;
}

BOOL OtpCheck_TRJ8495M::CheckCode_Stc()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M CheckCode_Stc");
	if (!m_bCheckStc) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	BYTE page = m_otpPage.af;
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

BOOL OtpCheck_TRJ8495M::CheckCode_Inf()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M CheckCode_Inf");
	if (!m_bCheckInf) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	BYTE page = m_otpPage.af;
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

BOOL OtpCheck_TRJ8495M::CheckCode_Mac()
{
	//m_pInterface->AddDebugLog(L"TRJ8495M CheckCode_Mac");
	if (!m_bCheckMac) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	BYTE page = m_otpPage.af;
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
