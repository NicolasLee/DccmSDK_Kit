#include "StdAfx.h"
#include "OtpCheck_HLT6014.h"


OtpCheck_HLT6014::OtpCheck_HLT6014(void)
{
	m_slaveSensor = 0x5A;
	m_slaveEeprom = 0;
	m_modeSensor = I2CMODE_16_8;
	m_modeEeprom = 0;
	m_regSize = 2;
//	m_pageSize = 0x1F;	
	m_flagValid	= 0x01;	
	m_flagEmpty	= 0x00;	
	
	m_addrFlag.moduleInfo	= 0x0A04;		
	m_addrFlag.awb			= 0x0A0D;		
	m_addrFlag.lsc			= 0x0A1A;		
	m_addrFlag.af			= 0x0A14;		
	m_addrFlag.pdaf			= UNEXCEPT_REG;		
	m_addrFlag.pdafStep2Start	= UNEXCEPT_REG;		

	m_addrSum.moduleInfo	= 0x0A05;		
	m_addrSum.awb			= UNEXCEPT_REG;		
	m_addrSum.lsc			= 0x0A1B;		
	m_addrSum.af			= 0X0A15;		
	m_addrSum.pdafStep1		= UNEXCEPT_REG;		
	m_addrSum.pdafStep2		= UNEXCEPT_REG;

// 	m_otpPage.moduleInfo	= 0;
// 	m_otpPage.awb			= 1;
// 	m_otpPage.lsc			= 2;	// 2 ~31
// 	m_otpPage.af			= 32;
// 	m_otpPage.pdafStep1		= 33;	// 33~40
// 	m_otpPage.pdafStep1_End = 41;
// 	m_otpPage.pdafStep2		= 40;	// 40~54
// 	m_otpPage.pdafStep2_End = 55;	// 55 end

	m_otpSize.moduleInfo	= 14;//m_addrSum.moduleInfo - m_addrFlag.moduleInfo - 1;
	m_otpSize.awb			= UNEXCEPT_VALUE;//m_addrSum.awb - m_addrFlag.awb - 1;
	m_otpSize.lsc			= 512;//m_addrSum.lsc - m_addrFlag.lsc - 1;
	m_otpSize.af			= 4;//m_addrSum.af	- m_addrFlag.af - 1;
	m_otpSize.pdafStep1		= UNEXCEPT_VALUE;//496
	m_otpSize.pdafStep2		= UNEXCEPT_VALUE;//908

	m_addrCode.stcL			= UNEXCEPT_VALUE;
	m_addrCode.stcH			= UNEXCEPT_VALUE;
	m_addrCode.infL			= 0x0A19;
	m_addrCode.infH			= 0x0A18;
	m_addrCode.macL		= 0x0A17;
	m_addrCode.macH		= 0x0A16;

	m_bCheckStc = FALSE;	// 起始Code
	m_bCheckInf = TRUE;	// 远景Code
	m_bCheckMac = TRUE;	// 近景Code

	m_stcLower = 0;	// 起始Code下边界
	m_stcUpper = 0;	// 起始Code上边界
	m_infLower = 0;	//
	m_infUpper = 0;	//
	m_macUpper = 0;	//
	m_macLower = 0;	//
}


OtpCheck_HLT6014::~OtpCheck_HLT6014(void)
{
}


BOOL OtpCheck_HLT6014::Check_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"HLT6014 Check_ModuleInfo");

	BOOL bRet = TRUE;
	bRet &= CheckFlag(m_bCheckFlag_ModuleInfo,m_addrFlag.moduleInfo,_T("Basic"));
	bRet &= CheckSum_ModuleInfo();

	return bRet;
}

BOOL OtpCheck_HLT6014::Check_AWB()
{
	//m_pInterface->AddDebugLog(L"HLT6014 Check_AWB");

	BOOL bRet = TRUE;
	bRet &= CheckFlag(m_bCheckFlag_AWB,m_addrFlag.awb,_T("AWB"));
	bRet &= CheckSum_AWB();

	return bRet;
}

BOOL OtpCheck_HLT6014::Check_LSC()
{
	//m_pInterface->AddDebugLog(L"HLT6014 Check_LSC");

	BOOL bRet = TRUE;
	bRet &= CheckFlag(m_bCheckFlag_LSC,m_addrFlag.lsc,_T("LSC"));
	bRet &= CheckSum_LSC();

	return bRet;
}

BOOL OtpCheck_HLT6014::Check_AF()
{
	//m_pInterface->AddDebugLog(L"HLT6014 Check_AF");

	BOOL bRet = TRUE;
	bRet &= CheckFlag(m_bCheckFlag_AF,m_addrFlag.af,_T("AF"));
	bRet &= CheckSum_AF();
	bRet &= CheckCode_Stc();
	bRet &= CheckCode_Inf();
	bRet &= CheckCode_Mac();

	return bRet;
}

BOOL OtpCheck_HLT6014::Check_PDAF()
{
	//m_pInterface->AddDebugLog(L"HLT6014 Check_PDAF");

	BOOL bRet = TRUE;
	bRet &= CheckFlag(m_bCheckFlag_PDAF,m_addrFlag.pdaf,_T("PDAF"));
	bRet &= CheckSum_PDAF_Step1();
	bRet &= CheckSum_PDAF_Step2();

	return bRet;
}

BYTE OtpCheck_HLT6014::SumCalc(BYTE* pVal, int valSize)
{
	ULONGLONG sum = 0;
	for (int i = 0; i < valSize; i++)
	{
		sum += pVal[i];
	}
	sum = sum%255 + 1;
	return (BYTE)sum;
}

#ifdef IIC_SENSOR_HLT6014
// BOOL OtpCheck_HLT6014::ReadSensor_Byte(WORD reg, BYTE* val)
// {
// 	//m_pInterface->AddDebugLog(L"HLT6014 ReadSensor_Byte");
// 	return TRUE;
// }
// 
// BOOL OtpCheck_HLT6014::ReadSensor_Word(WORD reg, WORD* val)
// {
// 	//m_pInterface->AddDebugLog(L"HLT6014 ReadSensor_Word");
// 	return TRUE;
// }
// 
// BOOL OtpCheck_HLT6014::WriteSensor_Byte(WORD reg, BYTE val)
// {
// 	//m_pInterface->AddDebugLog(L"HLT6014 WriteSensor_Byte");
// 	return TRUE;
// }
// 
// BOOL OtpCheck_HLT6014::WriteSensor_Word(WORD reg, WORD val)
// {
// 	//m_pInterface->AddDebugLog(L"HLT6014 WriteSensor_Word");
// 	return TRUE;
// }
// 
// BYTE OtpCheck_HLT6014::ReadSensor_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BOOL bStop /* = FALSE */)
// {
// 	//m_pInterface->AddDebugLog(L"HLT6014 ReadSensor_MultiBytes");
// 	return TRUE;
// }
// 
// BYTE OtpCheck_HLT6014::WriteSensor_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize)
// {
// 	//m_pInterface->AddDebugLog(L"HLT6014 WriteSensor_MultiBytes");
// 	return TRUE;
// }

#else
BOOL OtpCheck_HLT6014::ReadEeprom_Byte(WORD reg, BYTE* val)
{
	//m_pInterface->AddDebugLog(L"THY8348M Read3L8Sensor_Byte");

	BOOL bRet = TRUE;
	WORD readVal = 0;
	bRet &= m_pInterface->I2C_Read(m_slaveEeprom, reg, &readVal, m_modeEeprom);
	*val = (BYTE)readVal;

	return bRet;
}

BOOL OtpCheck_HLT6014::WriteEeprom_Byte(WORD reg, BYTE val)
{
	//m_pInterface->AddDebugLog(L"THY8348M WriteEeprom_Byte");

	BOOL bRet = TRUE;
	WORD readVal = 0;
	bRet &= m_pInterface->I2C_Write(m_slaveEeprom, reg, val, m_modeEeprom);

	return bRet;
}
// BOOL OtpCheck_HLT6014::Read3L8Sensor_Byte(WORD reg, BYTE* val, BYTE page)
// {
// 	//m_pInterface->AddDebugLog(L"HLT6014 Read3L8Sensor_Byte");
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
// BOOL OtpCheck_HLT6014::WriteEepromPage_Byte(WORD reg, BYTE val, BYTE page)
// {
// 	//m_pInterface->AddDebugLog(L"HLT6014 WriteEepromPage_Byte");
// 
// 	BOOL bRet = TRUE;
// 	bRet &= m_pInterface->I2C_Write(m_slaveEeprom, ((EEPROM_Read<<14)|(page<<6)|(reg&0x3f)), val, m_modeEeprom);
// 
// 	return bRet;
// }
/*
BOOL OtpCheck_HLT6014::ReadEepromPage_Word(WORD reg, WORD* val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"HLT6014 ReadEepromPage_Word");

	BOOL bRet = TRUE;
	WORD readVal = 0;
	bRet &= m_pInterface->I2C_Read(m_slaveEeprom, ((EEPROM_Read<<14)|(page<<6)|(reg&0x3f)), &readVal, m_modeEeprom);
	*val = readVal;

	return bRet;
}

BOOL OtpCheck_HLT6014::WriteEepromPage_Word(WORD reg, WORD val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"HLT6014 WriteEeprom_Word");

	BOOL bRet = TRUE;
	bRet &= m_pInterface->I2C_Write(m_slaveEeprom, reg, val, m_modeEeprom);

	return TRUE;
}
*/
// BYTE OtpCheck_HLT6014::ReadEepromPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop /* = FALSE */)
// {
// 	//m_pInterface->AddDebugLog(L"HLT6014 ReadEepromPage_MultiBytes");
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
// 		Read3L8Sensor_Byte(curReg, &val, page+j);
// 		pVal[i] = val;
// //		m_szInfo.Format(L"i:%04d reg:0x%02x, val:0x%02x, page:%04d", i, curReg, val, page+j);
// //		m_pInterface->AddLog(m_szInfo);
// 	}
// 
// 	return bRet;
// }

// BYTE OtpCheck_HLT6014::WriteEepromPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page)
// {
// 	//m_pInterface->AddDebugLog(L"HLT6014 WriteEeprom_MultiBytes");
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

BOOL OtpCheck_HLT6014::CheckValidGroup()
{
	BYTE _data[64] = {0};
	S5K3L8_ReadOTPPage(1, _data);

	if (_data[0] == m_flagEmpty && _data[OTP_GAP]== m_flagEmpty)
	{
		m_szInfo.Format(L"模组OTP未烧录!");
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}
	else if (_data[OTP_GAP] == m_flagValid)
	{
		OTPGroup = 2;
	}
	else if (_data[0] == m_flagValid)
	{
		OTPGroup =1;
	}
	else
	{
		m_szInfo.Format(L"获取有效OTP Group失败!");
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheck_HLT6014::CheckFlag_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"HLT6014 CheckFlag_ModuleInfo");
	if (!m_bCheckFlag_ModuleInfo) return TRUE;

	// Read flag
	BYTE readVal = 0;
	WORD reg = m_addrFlag.moduleInfo;
	Read3L8Sensor_Byte(reg, &readVal);

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

	return TRUE;
}

BOOL OtpCheck_HLT6014::CheckFlag_AWB()
{
	//m_pInterface->AddDebugLog(L"HLT6014 CheckFlag_AWB");
	if (!m_bCheckFlag_AWB) return TRUE;

	// Read flag
	BYTE readVal = 0;
	WORD reg = m_addrFlag.awb;
	Read3L8Sensor_Byte(reg, &readVal);
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

	return TRUE;
}

BOOL OtpCheck_HLT6014::CheckFlag_LSC()
{
	//m_pInterface->AddDebugLog(L"HLT6014 CheckFlag_LSC");
	if (!m_bCheckFlag_LSC) return TRUE;

	// Read flag
	BYTE readVal = 0;
	WORD reg = m_addrFlag.lsc;
	Read3L8Sensor_Byte(reg, &readVal);
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

BOOL OtpCheck_HLT6014::CheckFlag_AF()
{
	//m_pInterface->AddDebugLog(L"HLT6014 CheckFlag_AF");
	if (!m_bCheckFlag_AF) return TRUE;

	// Read flag
	BYTE readVal = 0;
	WORD reg = m_addrFlag.af;
	Read3L8Sensor_Byte(reg, &readVal);
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

BOOL OtpCheck_HLT6014::CheckFlag_PDAF()
{
	//m_pInterface->AddDebugLog(L"HLT6014 CheckFlag_PDAF_Step1");
	if (!m_bCheckFlag_PDAF) return TRUE;

	// Read flag
	BYTE readVal = 0;
	WORD reg = m_addrFlag.pdaf;
	Read3L8Sensor_Byte(reg, &readVal);
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

BOOL OtpCheck_HLT6014::CheckFlag(BOOL flag,int addr,CString str)
{
	if (!flag) return TRUE;

	// Read flag
	BYTE *readVal = new BYTE[1];
	WORD reg = addr;
	Read3L8Sensor_Byte(reg, readVal);

	BOOL bRet = TRUE;
	do 
	{
		if (readVal[0] == m_flagEmpty)
		{
			m_szInfo.Format(L"模组%s未烧录!",str);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
//			bRet = FALSE;//cnw
			break;
		}
		if (readVal[0] != m_flagValid)
		{
			m_szInfo.Format(L"read flag 0x%02x != 0x%02x", readVal, m_flagValid);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			bRet = FALSE;
			break;
		}
	} while (0);

	SAFE_DELETE_ARRAY(readVal);

	return bRet;
}

BOOL OtpCheck_HLT6014::CheckSum_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"HLT6014 CheckSum_ModuleInfo");
	if (!m_bCheckSum_ModuleInfo) return TRUE;

	// Calc sum
	WORD regAddr = m_addrFlag.moduleInfo;
	int valSize = m_otpSize.moduleInfo;

	BYTE* pVal = new BYTE[valSize];
	Read3L8Sensor_Group(regAddr+2, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.moduleInfo;
	BYTE calcVal = SumCalc(pVal, valSize);
	Read3L8Sensor_Byte(regAddr, &readVal);
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

BOOL OtpCheck_HLT6014::CheckSum_AWB()
{
	//m_pInterface->AddDebugLog(L"HLT6014 CheckSum_AWB");
	if (!m_bCheckSum_AWB) return TRUE;

	// Calc sum
	WORD regAddr = m_addrFlag.awb;
	int valSize = m_otpSize.awb;

	BYTE* pVal = new BYTE[valSize];
	Read3L8Sensor_Group(regAddr+1, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.awb;
	BYTE calcVal = SumCalc(pVal, valSize);
	Read3L8Sensor_Byte(regAddr, &readVal);
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

BOOL OtpCheck_HLT6014::CheckSum_LSC()
{
	//m_pInterface->AddDebugLog(L"HLT6014 CheckSum_LSC");
	if (!m_bCheckSum_LSC) return TRUE;

	// Calc sum
	WORD regAddr = m_addrFlag.lsc;
	int valSize = m_otpSize.lsc;
	BYTE* pVal = new BYTE[valSize];
	long int sum = 0;
	int i=0;
    m_pInterface->AddLog(L"Reading LSC Data...");
	if (OTPGroup == 2)
	{
		S5K3L8_ReadOTPPage(0x12,pVal);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x13,pVal+64);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x14,pVal+128);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x15,pVal+192);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x16,pVal+256);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x17,pVal+320);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x18,pVal+384);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x19,pVal+448);
		Sleep(100);
		for (i=62;i<(417+62);i++)
		{	
			sum += pVal[i];
		}
	}
	else
	{
		S5K3L8_ReadOTPPage(0x19,pVal);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1A,pVal+64);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1B,pVal+128);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1C,pVal+192);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1D,pVal+256);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1E,pVal+320);
		Sleep(100);
		S5K3L8_ReadOTPPage(0x1F,pVal+384);
		Sleep(100);	
		for (i=31;i<(417+31);i++)
		{	
			sum += pVal[i];
		}
	}
//	Read3L8Sensor_Group(regAddr+1, pVal, valSize);

// 	if (!SaveLSCData(pVal, valSize))
// 	{
// 		return FALSE;
// 	}

// 	if (!CheckAbnormal(pVal, valSize))
// 	{
// 		return FALSE;
// 	}

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.lsc;
//	BYTE calcVal = SumCalc(pVal, valSize);
	BYTE calcVal = sum%255 + 1;
	Read3L8Sensor_Byte(regAddr, &readVal);
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

BOOL OtpCheck_HLT6014::SaveLSCData(BYTE *pVal, int valSize)
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

BOOL OtpCheck_HLT6014::SaveOTPData()
{
	m_pInterface->AddLog(_T("Read OTP Start!"));
	///////////////////////////////Read OTP////////////////////////////////////
	int valSize = 1024;
	BYTE* pVal = new BYTE[valSize];
	S5K3L8_ReadOTPPage(0x01, pVal);
	Sleep(100);
	S5K3L8_ReadOTPPage(0x12, pVal+64);
	Sleep(100);
	S5K3L8_ReadOTPPage(0x13, pVal+64*2);
	Sleep(100);
	S5K3L8_ReadOTPPage(0x14, pVal+64*3);
	Sleep(100);
	S5K3L8_ReadOTPPage(0x15, pVal+64*4);
	Sleep(100);
	S5K3L8_ReadOTPPage(0x16, pVal+64*5);
	Sleep(100);
	S5K3L8_ReadOTPPage(0x17, pVal+64*6);
	Sleep(100);
	S5K3L8_ReadOTPPage(0x01, pVal+64*7);
	Sleep(100);
	S5K3L8_ReadOTPPage(0x18, pVal+64*8);
	Sleep(100);
	S5K3L8_ReadOTPPage(0x19, pVal+64*9);
	Sleep(100);
	S5K3L8_ReadOTPPage(0x1A, pVal+64*10);
	Sleep(100);
	S5K3L8_ReadOTPPage(0x1B, pVal+64*11);
	Sleep(100);
	S5K3L8_ReadOTPPage(0x1C, pVal+64*12);
	Sleep(100);
	S5K3L8_ReadOTPPage(0x1D, pVal+64*13);
	Sleep(100);
	S5K3L8_ReadOTPPage(0x1E, pVal+64*14);
	Sleep(100);
	S5K3L8_ReadOTPPage(0x1F, pVal+64*15);
	Sleep(100);
	//////////////////////////////////////////////////////////////////////////
	CString DirPath = m_pInterface->GetExeDir();
	CString Path ;
	Path.Format(_T("OTP Data_%d.txt"),m_pInterface->GetModuleNum());
	Path = DirPath+Path;
	FILE* file = NULL;

	CStringA temp, num;
	int i=0; 
	int j=0;
	int k=0;
	for (i = 0; i<valSize; i+=64)
	{
		if (i == 0)
		{
			num.Format("Page: 0x%x\n", 1);
			temp += num;
		}
		else if (i%64 == 0)
		{
			num.Format("Page: 0x%x\n", 0x12+k);
			temp += num;
			k++;
		}

		for(j=0; j<64; j++)
		{
			num.Format("%04d\t0x%04x,0x%02x\n", i+j,0x0A04+j,(unsigned char)pVal[i+j]);
			temp += num;
		}
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

BOOL OtpCheck_HLT6014::CheckAbnormal(BYTE *pVal, int valSize)
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

BOOL OtpCheck_HLT6014::CheckSum_AF()
{
	//m_pInterface->AddDebugLog(L"HLT6014 CheckSum_AF");
	if (!m_bCheckSum_AF) return TRUE;

	// Calc sum
	WORD regAddr = m_addrFlag.af;

	int valSize = m_otpSize.af;
	BYTE* pVal = new BYTE[valSize];
	Read3L8Sensor_Group(regAddr+2, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.af;
	BYTE calcVal = SumCalc(pVal, valSize);
	Read3L8Sensor_Byte(regAddr, &readVal);
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

BOOL OtpCheck_HLT6014::CheckSum_PDAF_Step1()
{
	//m_pInterface->AddDebugLog(L"HLT6014 CheckSum_PDAF_Step1");
	if (!m_bCheckSum_PDAF) return TRUE;

	// Calc sum
	WORD regAddr = m_addrFlag.pdaf;
	int valSize = m_otpSize.pdafStep1;
	BYTE* pVal = new BYTE[valSize];
	Read3L8Sensor_Group(regAddr+1, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.pdafStep1;
	BYTE calcVal = SumCalc(pVal, valSize);
	Read3L8Sensor_Byte(regAddr, &readVal);
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

BOOL OtpCheck_HLT6014::CheckSum_PDAF_Step2()
{
	//m_pInterface->AddDebugLog(L"HLT6014 CheckSum_PDAF_Step2");
	if (!m_bCheckSum_PDAF) return TRUE;

	// Calc sum
	WORD regAddr = m_addrFlag.pdafStep2Start;
	int valSize = m_otpSize.pdafStep2;
	BYTE* pVal = new BYTE[valSize];
	Read3L8Sensor_Group(regAddr, pVal, valSize);

	// Read sum
	BYTE readVal = 0;
	regAddr = m_addrSum.pdafStep2;
	BYTE calcVal = SumCalc(pVal, valSize);
	Read3L8Sensor_Byte(regAddr, &readVal);
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

BOOL OtpCheck_HLT6014::CheckCode_Stc()
{
	//m_pInterface->AddDebugLog(L"HLT6014 CheckCode_Stc");
	if (!m_bCheckStc) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	BYTE page = m_otpPage.af;
	WORD readVal = 0;
	Read3L8Sensor_Byte(m_addrCode.stcH, &readVal_H);
	Read3L8Sensor_Byte(m_addrCode.stcL, &readVal_L);
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

BOOL OtpCheck_HLT6014::CheckCode_Inf()
{
	//m_pInterface->AddDebugLog(L"HLT6014 CheckCode_Inf");
	if (!m_bCheckInf) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	BYTE page = m_otpPage.af;
	WORD readVal = 0;
	Read3L8Sensor_Byte(m_addrCode.infH, &readVal_H);
	Read3L8Sensor_Byte(m_addrCode.infL, &readVal_L);
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

BOOL OtpCheck_HLT6014::CheckCode_Mac()
{
	//m_pInterface->AddDebugLog(L"HLT6014 CheckCode_Mac");
	if (!m_bCheckMac) return TRUE;

	BOOL bRet = TRUE;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	BYTE page = m_otpPage.af;
	WORD readVal = 0;
	Read3L8Sensor_Byte(m_addrCode.macH, &readVal_H);
	Read3L8Sensor_Byte(m_addrCode.macL, &readVal_L);
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

//////////////////////////////////////////////////////////////////////////
void OtpCheck_HLT6014::S5K3L8_ReadOTPPage(int page, BYTE *Buff)
{
	i2c_write(0x0100,0x0100);
	i2c_write_byte(0x0a02,page);
	i2c_write(0x0a00,0x0100);
	for (int i=0;i<64;i++)
	{
		i2c_read_byte(0x0a04+i,Buff+i);
	}

	Sleep(100);
	i2c_write(0x0a00,0x0000);//make initial state
	return;
}

void OtpCheck_HLT6014::Read3L8Sensor_Byte(WORD reg, BYTE* val)
{
	BYTE _data[64] = {0};
	S5K3L8_ReadOTPPage(1,_data);

	if (OTPGroup == 2)
	{
		val[0] =  _data[reg-0x0A04+OTP_GAP];
	}
	else
	{
		val[0] =  _data[reg-0x0A04];
	}

	return;
}

void OtpCheck_HLT6014::Read3L8Sensor_Group(USHORT address,BYTE *Data,int size)
{
	BYTE _data[64] = {0};
	S5K3L8_ReadOTPPage(1,_data);

	if (OTPGroup == 2)
	{
		for (int i=0; i<size; i++)
		{
			Data[i] = _data[address-0x0A04+i+OTP_GAP];
		}
	}
	else
	{
		for (int i=0; i<size; i++)
		{
			Data[i] = _data[address-0x0A04+i];
		}
	}

	return;
}