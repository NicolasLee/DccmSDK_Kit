#include "StdAfx.h"
#include "OtpCheckBase.h"


OtpCheckBase::OtpCheckBase(void)
{
	memset(&m_flag, 0, sizeof(m_flag));
	memset(&m_addrFlag, 0, sizeof(m_addrFlag));
	memset(&m_addrSum, 0, sizeof(m_addrSum));
	memset(&m_otpSize, 0, sizeof(m_otpSize));
	memset(&m_afAddrCode, 0, sizeof(m_afAddrCode));
	memset(&m_afValue, 0, sizeof(m_afValue));
	memset(&m_otpPage, 0, sizeof(m_otpPage));
	memset(&m_gapsize, 0 ,sizeof(m_gapsize));
	memset(&m_addrAWB, 0 ,sizeof(m_addrAWB));
	memset(&m_valueAWB, 0 ,sizeof(m_valueAWB));
	memset(&m_calcCheckSum, 0 ,sizeof(m_calcCheckSum));
}


OtpCheckBase::~OtpCheckBase(void)
{
}

int OtpCheckBase::InitProject()//对于有些Sensor 比如Hynix系列 读写OTP时需要做Init和Release
{
	return FUNCTION_PASS;
}

int OtpCheckBase::EndProject()
{
	return FUNCTION_PASS;
}

int OtpCheckBase::PauseGrabFrame()
{
	return m_pInterface->PauseGrabFrame();
}

int OtpCheckBase::ResumeGrabFrame()
{
	return m_pInterface->ResumeGrabFrame();
}

char* OtpCheckBase::GetFuseID()
{
	return m_pInterface->GetImageSensor()->GetFuseID();
}

int OtpCheckBase::InitOTP()
{
	return m_pInterface->GetImageSensor()->	InitOTP();
}

int OtpCheckBase::ReleaseOTP()
{
	return m_pInterface->GetImageSensor()->ReleaseOTP();
}


BOOL OtpCheckBase::S5K4H7ReadOTPByte(UINT Page, USHORT startAddr, BYTE *data, int length)
{
	BOOL bResult = TRUE;
	BYTE bPage = Page & 0xff;

	bResult &= i2c_write_byte(0x0A02, bPage); //page 21
	bResult &= i2c_write_byte(0x0A00, 0x01);//read enable
	Sleep(50);
	for (int i = 0; i < length; i++)
	{
		bResult &= i2c_read_byte(startAddr + i, &data[i]);
	}
	bResult &= i2c_write_byte(0x0A00, 0x00);

	return bResult;
}

BOOL OtpCheckBase::S5K4H7WriteMultiByte(UINT Page, USHORT startAddr, BYTE *data, int nstart, int length)
{
	BOOL bResult = TRUE;
	BYTE ErrFlag;
	BYTE bPage = Page & 0xff;

	bResult &= i2c_write_byte(0x3B3F, 0x00);   //OTP write disable
	bResult &= i2c_write_byte(0x0A02, bPage);    //write page 21
	bResult &= i2c_write_byte(0x0A00, 0x03);   //write command

	for (int i = 0; i < length; i++)
	{
		bResult &= i2c_write_byte(startAddr + i, data[nstart + i]);
	}
	Sleep(100);
	bResult &= i2c_read_byte(0x0A01, &ErrFlag);
	if (((ErrFlag >> 1) & 0x01) == 1)
	{
		bResult &= i2c_write_byte(0x0A00, 0x00);
	}
	else if (((ErrFlag >> 3) & 0x01) == 1)
	{
		bResult &= FALSE;
	}
	else
		Sleep(100);
	bResult &= i2c_write_byte(0x0A00, 0x00);

	return bResult;

}

BOOL OtpCheckBase::S5K4H7WriteSingleByte(UINT Page, USHORT startAddr, BYTE data)
{
	BOOL bResult = TRUE;
	BYTE ErrFlag;
	BYTE bPage = Page & 0xff;

	bResult &= i2c_write_byte(0x3B3F, 0x00);   //OTP write disable
	bResult &= i2c_write_byte(0x0A02, bPage);    //write page 21
	bResult &= i2c_write_byte(0x0A00, 0x03);   //write command

	bResult &= i2c_write_byte(startAddr, data);
	Sleep(100);
	bResult &= i2c_read_byte(0x0A01, &ErrFlag);
	if (((ErrFlag >> 1) & 0x01) == 1)
	{
		bResult &= i2c_write_byte(0x0A00, 0x00);
	}
	else if (((ErrFlag >> 3) & 0x01) == 1)
	{
		bResult &= FALSE;
	}
	else
		Sleep(100);
	bResult &= i2c_write_byte(0x0A00, 0x00);

	return bResult;

}


int OtpCheckBase::Read_OTP_BYTE(USHORT addr, BYTE* data, int PageNo/* = 0 */)
{
	return m_pInterface->GetImageSensor()->Read_OTP_BYTE(addr,data,PageNo);
}

int OtpCheckBase::Read_OTP_Multi_BYTE(USHORT addr, BYTE* data, int Length, int PageNo /* = 0 */)
{
	return m_pInterface->GetImageSensor()->Read_OTP_Multi_BYTE(addr,data,Length,PageNo);
}

int OtpCheckBase::Write_OTP_BYTE(USHORT addr, BYTE data, int PageNo/* = 0 */)
{
	return m_pInterface->GetImageSensor()->Write_OTP_BYTE(addr,data,PageNo);
}

int OtpCheckBase::Write_OTP_Multi_BYTE(USHORT addr, BYTE* data, int Length, int PageNo /* = 0 */)
{
	return m_pInterface->GetImageSensor()->Write_OTP_Multi_BYTE(addr,data,Length,PageNo);
}

int OtpCheckBase::Eeprom_All_Erase()
{
	return m_pInterface->GetEepromIC()->Eeprom_All_Erase();
}

int OtpCheckBase::Eeprom_Page_Erase(BYTE page)
{
	return m_pInterface->GetEepromIC()->Eeprom_Page_Erase(page);
}

int OtpCheckBase::Eeprom_Programing(USHORT address,unsigned char *Data,int size)
{
	return m_pInterface->GetEepromIC()->Eeprom_Programing(address,Data,size);
}

int OtpCheckBase::Eeprom_Programing_BYTE(USHORT address, unsigned char Data)
{
	return m_pInterface->GetEepromIC()->Eeprom_Programing(address,&Data,1);
}

int OtpCheckBase::Eeprom_Read(USHORT address,unsigned char *Data,int size)
{
	return m_pInterface->GetEepromIC()->Eeprom_Read(address,Data,size);
}

int OtpCheckBase::Eeprom_Read_BYTE(USHORT address,unsigned char *Data)
{
	return m_pInterface->GetEepromIC()->Eeprom_Read(address, Data, 1);
}

int OtpCheckBase::Programing(USHORT address, unsigned char *Data, int size, int PageNo/* = 0 */)
{
	if (m_OtpType == 0)
	{
		return Write_OTP_Multi_BYTE(address, Data, size, PageNo);
	}
	else if (m_OtpType == 1)
	{
		return Eeprom_Programing(address, Data,size);
	}

	return FUNCTION_FAIL;
}

int OtpCheckBase::Programing_BYTE(USHORT addr, BYTE data, int PageNo/* = 0 */)
{
	if (m_OtpType == 0)
	{
		return Write_OTP_BYTE(addr, data, PageNo);
	}
	else if (m_OtpType == 1)
	{
		return Eeprom_Programing_BYTE(addr, data);
	}
	return FUNCTION_FAIL;
}

int OtpCheckBase::Read(USHORT address, unsigned char *Data, int size, int PageNo/* = 0 */)
{
	if (m_OtpType == 0)
	{
		return Read_OTP_Multi_BYTE(address, Data, size, PageNo);
	}
	else if (m_OtpType == 1)
	{
		return Eeprom_Read(address, Data,size);
	}
	return FUNCTION_FAIL;
}

int OtpCheckBase::Read_BYTE(USHORT addr, BYTE *data, int PageNo/* = 0 */)
{
	if (m_OtpType == 0)
	{
		return Read_OTP_BYTE(addr, data, PageNo);
	}
	else if (m_OtpType == 1)
	{
		return Eeprom_Read_BYTE(addr, data);
	}
	return FUNCTION_FAIL;
}


BYTE OtpCheckBase::SumCalc(BYTE *data, int num, int mod,int add)
{
	ASSERT(data != NULL);

	ULONG  sum = 0;

	for (int i=0; i<num; i++)
	{
		sum += data[i];
	}

	return (BYTE)(sum % mod+add);
}

WORD OtpCheckBase::SumCalc_WORD(BYTE *data, int num, int mod,int add)
{
	ASSERT(data != NULL);

	ULONG  sum = 0;

	for (int i=0; i<num; i++)
	{
		sum += data[i];
	}

	return (WORD)(sum % mod+add);
}

//i2c通用读写函数
BOOL OtpCheckBase::i2c_write(USHORT reg, USHORT val)
{
	for (int i=0; i<I2C_RETRY_NUM; i++)
	{
		if (m_pInterface->I2C_WriteSensor_Word(reg, val))
		{
			return TRUE;
		}

		CString info;
		info.Format(_T("I2C Write ERROR: %X, %X"), reg, val);
		m_pInterface->AddLog(info);		

		Sleep(I2C_RETRY_WAIT);
	}

	CString info;
	info.Format(_T("I2C Write ERROR: %X, %X"), reg, val);
	m_pInterface->AddLog(info, COLOR_RED);		

	return FALSE;
}

BOOL OtpCheckBase::i2c_read(USHORT reg, USHORT *pval)
{
	if ((pval == NULL))
	{
		return FALSE;
	}

	for (int i=0; i<I2C_RETRY_NUM; i++)
	{
		if (m_pInterface->I2C_ReadSensor_Word(reg, pval))
		{
			return TRUE;
		}

		CString info;
		info.Format(_T("I2C Read ERROR: %X"), reg);
		m_pInterface->AddLog(info);

		Sleep(I2C_RETRY_WAIT);
	}

	CString info;
	info.Format(_T("I2C Read ERROR: %X"), reg);
	m_pInterface->AddLog(info, COLOR_RED);

	return FALSE;
}

BOOL OtpCheckBase::i2c_write_byte(USHORT reg, BYTE val)
{
	for (int i=0; i<I2C_RETRY_NUM; i++)
	{	
		if (m_pInterface->I2C_WriteSensor_Byte(reg, val))
		{
			return TRUE;
		}

		CString info;
		info.Format(_T("I2C Write ERROR: 0x%04X, 0x%02X"), reg, val);
		m_pInterface->AddLog(info);		

		Sleep(I2C_RETRY_WAIT);
	}

	CString info;
	info.Format(_T("I2C Write ERROR: %X, %X"), reg, val);
	m_pInterface->AddLog(info, COLOR_RED);		

	return FALSE;
}

BOOL OtpCheckBase::i2c_read_byte(USHORT reg, BYTE *pval)
{
	if ((pval == NULL))
	{
		return FALSE;
	}

	for (int i=0; i<I2C_RETRY_NUM; i++)
	{
		if (m_pInterface->I2C_ReadSensor_Byte(reg, pval))
		{
			return TRUE;
		}

		CString info;
		info.Format(_T("I2C Read ERROR: %X"), reg);
		m_pInterface->AddLog(info);
	}

	CString info;
	info.Format(_T("I2C Read ERROR: %X"), reg);
	m_pInterface->AddLog(info, COLOR_RED);

	return FALSE;
}


int OtpCheckBase::Check_S5K4H7ModuleInfo()
{
	int bRet = FUNCTION_PASS;
	bRet |= S5K4H7CheckFlag(m_addrFlag.basicpage,m_bCheckFlag_ModuleInfo, m_addrFlag.basic, _T("Basic"), &m_group.basic);
	bRet |= CheckSum(m_bCheckSum_ModuleInfo, m_addrFlag.basic, m_addrSum.basic, m_otpSize.basic, m_gapsize.basic, _T("Basic"), m_group.basic);

	return bRet;
}

int OtpCheckBase::Check_S5K4H7AWB()
{
	int bRet = FUNCTION_PASS;
	bRet |= S5K4H7CheckFlag(m_addrFlag.awbpage, m_bCheckFlag_AWB, m_addrFlag.awb, _T("AWB"), &m_group.awb);
	bRet |= Check_AWBValue();
	bRet |= CheckSum(m_bCheckSum_AWB, m_addrFlag.awb, m_addrSum.awb, m_otpSize.awb, m_gapsize.awb, _T("AWB"), m_group.awb);

	return bRet;
}

int OtpCheckBase::Check_S5K4H7LSC()
{
	int bRet = FUNCTION_PASS;
	bRet |= S5K4H7CheckFlag(m_addrFlag.lscpage, m_bCheckFlag_LSC, m_addrFlag.lsc, _T("LSC"), &m_group.lsc);
	bRet |= CheckSum(m_bCheckSum_LSC, m_addrFlag.lsc, m_addrSum.lsc, m_otpSize.lsc, m_gapsize.lsc, _T("LSC"), m_group.lsc);

	return bRet;
}

int OtpCheckBase::S5K4H7CheckFlag(int page,BOOL flag, int addr, CString str, int *validgroup)//检查模块Flag
{
	if (!flag) return FUNCTION_PASS;

	// Read flag
	BYTE readVal = { 0 };
	WORD reg = addr;
	
	S5K4H7ReadOTPByte(page, reg, &readVal, 1);
	if (0x11 == readVal)
	{
		readVal |= 0x02;
	}

	*validgroup = CheckGroup(readVal);
	int bRet = FUNCTION_PASS;
	do
	{
		if (*validgroup == m_group.empty)
		{
			m_szInfo.Format(L"模组%s未烧录!", str);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			bRet = FUNCTION_FAIL;
			break;
		}
		else if (*validgroup == m_group.invalid)
		{
			m_szInfo.Format(L"%s Flag标志位无效!", str);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			bRet = FUNCTION_FAIL;
			break;
		}

		m_szInfo.Format(L"%s Flag第%d组有效!", str, *validgroup);
		m_pInterface->AddLog(m_szInfo, COLOR_BLUE);
	} while (0);

	return bRet;
}



int OtpCheckBase::Check_ModuleInfo()
{
	int bRet = FUNCTION_PASS;
	bRet |= CheckFlag(m_bCheckFlag_ModuleInfo,m_addrFlag.basic,_T("Basic"),&m_group.basic);
	bRet |= CheckSum(m_bCheckSum_ModuleInfo,m_addrFlag.basic,m_addrSum.basic,m_otpSize.basic, m_gapsize.basic, _T("Basic"),m_group.basic);

	return bRet;
}

int OtpCheckBase::Check_AWB()
{
	int bRet = FUNCTION_PASS;
	bRet |= CheckFlag(m_bCheckFlag_AWB,m_addrFlag.awb,_T("AWB"),&m_group.awb);
	bRet |= Check_AWBValue();
	bRet |= CheckSum(m_bCheckSum_AWB,m_addrFlag.awb,m_addrSum.awb,m_otpSize.awb, m_gapsize.awb, _T("AWB"),m_group.awb);

	return bRet;
}


int OtpCheckBase::Check_LSC()
{
	int bRet = FUNCTION_PASS;
	bRet |= CheckFlag(m_bCheckFlag_LSC,m_addrFlag.lsc,_T("LSC"),&m_group.lsc);
	bRet |= CheckSum(m_bCheckSum_LSC,m_addrFlag.lsc,m_addrSum.lsc,m_otpSize.lsc, m_gapsize.lsc, _T("LSC"),m_group.lsc);

	return bRet;
}

int OtpCheckBase::Check_AF()
{
	int bRet = FUNCTION_PASS;
	bRet |= CheckFlag(m_bCheckFlag_AF,m_addrFlag.af,_T("AF"),&m_group.af);
	bRet |= CheckSum(m_bCheckSum_AF,m_addrFlag.af,m_addrSum.af,m_otpSize.af, m_gapsize.af, _T("AF"),m_group.af);
	bRet |= CheckCode_Stc();
	bRet |= CheckCode_Inf();
	bRet |= CheckCode_Mac();
	bRet |= CheckCode_Mid();
	bRet |= CompareAFCode();//AF二次比对

	return bRet;
}

int OtpCheckBase::Check_PDAF()
{
	int bRet = FUNCTION_PASS;
	bRet |= CheckFlag(m_bCheckFlag_PDAF,m_addrFlag.pdaf1,_T("PDAF"),&m_group.pdaf);
	bRet |= CheckSum(m_bCheckSum_PDAF,m_addrFlag.pdaf1,m_addrSum.pdaf1,m_otpSize.pdaf1, m_gapsize.pdaf1, _T("PDAF1"),m_group.pdaf);
	bRet |= CheckSum(m_bCheckSum_PDAF,m_addrFlag.pdaf2,m_addrSum.pdaf2,m_otpSize.pdaf2, m_gapsize.pdaf2, _T("PDAF2"),m_group.pdaf);
	return bRet;
}

int OtpCheckBase::CheckFlag(BOOL flag,int addr, CString str, int *validgroup)//检查模块Flag
{
	if (!flag) return FUNCTION_PASS;

	// Read flag
	BYTE readVal = {0};
	WORD reg = addr;
	Read_BYTE(reg,&readVal);

	*validgroup = CheckGroup(readVal);
	int bRet = FUNCTION_PASS;
	do 
	{
		if (*validgroup == m_group.empty)
		{
			m_szInfo.Format(L"模组%s未烧录!",str);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			bRet = FUNCTION_FAIL;
			break;
		}
		else if (*validgroup == m_group.invalid)
		{
			m_szInfo.Format(L"%s Flag标志位无效!",str );
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			bRet = FUNCTION_FAIL;
			break;
		}

		m_szInfo.Format(L"%s Flag第%d组有效!",str, *validgroup );
		m_pInterface->AddLog(m_szInfo, COLOR_BLUE);
	} while (0);

	return bRet;
}

int OtpCheckBase::CheckGroup(BYTE flag)
{
	m_pInterface->AddLog(L"该案子CheckGroup函数未找到",COLOR_RED);
	return FUNCTION_FAIL;
}

int OtpCheckBase::CheckSum(BOOL flag,int flagaddr, int sumaddr, int datasize,  int gap,  CString str, int validgroup)//检查模块Checksum
{
	m_pInterface->AddLog(L"该案子CheckSum函数未找到",COLOR_RED);
	return FUNCTION_FAIL;
}

int OtpCheckBase::CheckSum_Word(BOOL flag,int flagaddr, int sumaddr, int datasize,  int gap,  CString str, int validgroup)//检查模块Checksum
{
	m_pInterface->AddLog(L"该案子CheckSum_Word函数未找到",COLOR_RED);
	return FUNCTION_FAIL;
}

int OtpCheckBase::GetStcAFCode()
{
	int bRet = FUNCTION_PASS;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	USHORT page = m_otpPage.af;
	bRet |= Read_BYTE(m_afAddrCode.stcH+(m_group.af-1)*m_gapsize.af, &readVal_H);
	bRet |= Read_BYTE(m_afAddrCode.stcL+(m_group.af-1)*m_gapsize.af, &readVal_L);
	m_afValue.m_bStcCode = readVal_L | (readVal_H << 8);

	if(bRet)
	{
		m_pInterface->AddLog(L"读取起始code值失败");
	}
	return bRet;
}

int OtpCheckBase::CheckCode_Stc()
{
	if (!m_bCheckStc) return FUNCTION_PASS;

	if (GetStcAFCode())	return FUNCTION_FAIL;

	if (m_afValue.m_bStcCode < m_infLower ||
		m_afValue.m_bStcCode > m_infUpper)
	{
		m_szInfo.Format(L"远景Code %d 不在范围 (%d~%d)", m_afValue.m_bStcCode, m_infLower, m_infUpper);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FUNCTION_FAIL;
	}

	return FUNCTION_PASS;
}

int OtpCheckBase::GetInfAFCode()
{
	int bRet = FUNCTION_PASS;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	USHORT page = m_otpPage.af;
	bRet |= Read_BYTE(m_afAddrCode.infH+(m_group.af-1)*m_gapsize.af, &readVal_H);
	bRet |= Read_BYTE(m_afAddrCode.infL+(m_group.af-1)*m_gapsize.af, &readVal_L);
	m_afValue.m_bInfCode = readVal_L | (readVal_H << 8);	

	if(bRet)
	{
		m_pInterface->AddLog(L"读取远景code值失败");
	}
	return bRet;
}

int OtpCheckBase::CheckCode_Inf()
{
	if (!m_bCheckInf) return FUNCTION_PASS;

	if (GetInfAFCode())	return FUNCTION_FAIL;

	if (m_afValue.m_bInfCode < m_infLower ||
		m_afValue.m_bInfCode > m_infUpper)
	{
		m_szInfo.Format(L"远景Code %d 不在范围 (%d~%d)", m_afValue.m_bInfCode, m_infLower, m_infUpper);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FUNCTION_FAIL;
	}

	return FUNCTION_PASS;
}

int OtpCheckBase::GetMacAFCode()
{
	int bRet = FUNCTION_PASS;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	USHORT page = m_otpPage.af;
	bRet |= Read_BYTE(m_afAddrCode.macH+(m_group.af-1)*m_gapsize.af, &readVal_H);
	bRet |= Read_BYTE(m_afAddrCode.macL+(m_group.af-1)*m_gapsize.af, &readVal_L);
	m_afValue.m_bMacCode = readVal_L | (readVal_H << 8);

	if(bRet)
	{
		m_pInterface->AddLog(L"读取近景code值失败");
	}
	return bRet;
}

int OtpCheckBase::CheckCode_Mac()
{
	if (!m_bCheckMac) return FUNCTION_PASS;

	if (GetMacAFCode())	return FUNCTION_FAIL;

	if (m_afValue.m_bMacCode < m_macLower ||
		m_afValue.m_bMacCode > m_macUpper)
	{
		m_szInfo.Format(L"近景Code %d 不在范围 (%d~%d)", m_afValue.m_bMacCode, m_macLower, m_macUpper);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FUNCTION_FAIL;
	}

	return FUNCTION_PASS;
}

int OtpCheckBase::GetMidAFCode()
{
	int bRet = FUNCTION_PASS;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	USHORT page = m_otpPage.af;
	bRet |= Read_BYTE(m_afAddrCode.midH+(m_group.af-1)*m_gapsize.af, &readVal_H);
	bRet |= Read_BYTE(m_afAddrCode.midL+(m_group.af-1)*m_gapsize.af, &readVal_L);
	m_afValue.m_bMidCode = readVal_L | (readVal_H << 8);

	if(bRet)
	{
		m_pInterface->AddLog(L"读取中景code值失败");
	}
	return bRet;
}

int OtpCheckBase::CheckCode_Mid()
{
	if (!m_bCheckMid) return FUNCTION_PASS;

	if (GetMidAFCode())	return FUNCTION_FAIL;

	if (m_afValue.m_bMidCode < m_midLower ||
		m_afValue.m_bMidCode > m_midUpper)
	{
		m_szInfo.Format(L"中景Code %d 不在范围 (%d~%d)", m_afValue.m_bMidCode, m_midLower, m_midUpper);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		return FUNCTION_FAIL;
	}

	return FUNCTION_PASS;
}

int OtpCheckBase::CompareAFCode()//AF值二次比对，用于测二程式检测烧录AF值与当前获取AF值差异
{
	if (!m_bCompareAF)	return FUNCTION_PASS;
	USHORT m_infCode = 0;
	USHORT m_macCode = 0;
	USHORT m_stcCode = 0;
	USHORT m_midCode = 0;
	CString info;

	vector<double>* vpStcAF = m_pInterface->GetStorage(_T("AutoFocusStart"));
	vector<double>* vpMacAF = m_pInterface->GetStorage(_T("AutoFocusNear"));
	vector<double>* vpInfAF = m_pInterface->GetStorage(_T("AutoFocusFar"));
	vector<double>* vpMidAF = m_pInterface->GetStorage(_T("AutoFocusMiddle"));

	if (m_bCheckInf)
	{
		if (vpInfAF == NULL || vpInfAF->size() == 0)
		{
			m_infCode = 0;
			info = _T("远景Code值为空");
			m_pInterface->AddLog(info, COLOR_RED);
			return FUNCTION_FAIL;
		}  
		else
			m_infCode = (USHORT)(vpInfAF->at(0));

		if ((UINT)abs(m_infCode-m_afValue.m_bInfCode) >= m_bAFCompareLimit)
		{
			m_szInfo.Format(L"OTP内远景 Code %d 与二次对焦AF Code %d 超出允许范围 : %d", m_afValue.m_bInfCode, m_infCode, m_bAFCompareLimit);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckMac)
	{
		if (vpMacAF == NULL || vpMacAF->size() == 0)
		{
			m_macCode = 0;
			info = _T("近景Code值为空");
			m_pInterface->AddLog(info, COLOR_RED);
			return FUNCTION_FAIL;
		}  
		else
			m_macCode = (USHORT)(vpMacAF->at(0));

		if ((UINT)abs(m_macCode-m_afValue.m_bMacCode) >= m_bAFCompareLimit)
		{
			m_szInfo.Format(L"OTP内近景 Code %d 与二次对焦AF Code %d 超出允许范围 : %d", m_afValue.m_bMacCode, m_macCode, m_bAFCompareLimit);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckStc)
	{
		if (vpStcAF == NULL || vpStcAF->size() == 0)
		{
			m_stcCode = 0;
			info = _T("起始Code值为空");
			m_pInterface->AddLog(info, COLOR_RED);
			return FUNCTION_FAIL;
		}  
		else
			m_stcCode = (USHORT)(vpStcAF->at(0));

		if ((UINT)abs(m_stcCode-m_afValue.m_bStcCode) >= m_bAFCompareLimit)
		{
			m_szInfo.Format(L"OTP内起始 Code %d 与二次对焦AF Code %d 超出允许范围 : %d", m_afValue.m_bStcCode, m_stcCode, m_bAFCompareLimit);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckMid)
	{
		if (vpMidAF == NULL || vpMidAF->size() == 0)
		{
			m_midCode = 0;
			info = _T("中景Code值为空");
			m_pInterface->AddLog(info, COLOR_RED);
			return FUNCTION_FAIL;
		}  
		else
			m_midCode = (USHORT)(vpMidAF->at(0));

		if ((UINT)abs(m_midCode-m_afValue.m_bMidCode) >= m_bAFCompareLimit)
		{
			m_szInfo.Format(L"OTP内中景 Code %d 与二次对焦AF Code %d 超出允许范围 : %d", m_afValue.m_bMidCode, m_midCode, m_bAFCompareLimit);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	return FUNCTION_PASS;
}

int OtpCheckBase::Check_AWBValue()//检查AWB值，分为两个函数，GetValue为读取AWB值，不同案子不同写法，CheckValue为检测AWB值，为通用函数。
{
	if (GetValue())
	{
		m_pInterface->AddLog(_T("读取AWB值异常"),COLOR_RED);		
		return FUNCTION_FAIL;
	}
	if (CheckValue())	return FUNCTION_FAIL;
	return FUNCTION_PASS;
}

int OtpCheckBase::GetValue()//读取AWB值
{
	if (Read_BYTE(m_addrAWB.RGL+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.RGL) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.RGH+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.RGH) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.BGL+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.BGL) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.BGH+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.BGH) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GGL+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GGL) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GGH+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GGH) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.RL+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.RL) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.RH+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.RH) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GrL+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GrL) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GrH+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GrH) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GbL+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GbL) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GbH+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GbH) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.BL+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.BL) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.BH+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.BH) != FUNCTION_PASS) return FUNCTION_FAIL;

	if (Read_BYTE(m_addrAWB.GOLDRGL+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GOLDRGL) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GOLDRGH+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GOLDRGH) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GOLDBGL+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GOLDBGL) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GOLDBGH+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GOLDBGH) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GOLDGGL+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GOLDGGL) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GOLDGGH+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GOLDGGH) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GOLDRL+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GOLDRL) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GOLDRH+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GOLDRH) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GOLDGrL+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GOLDGrL) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GOLDGrH+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GOLDGrH) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GOLDGbL+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GOLDGbL) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GOLDGbH+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GOLDGbH) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GOLDBL+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GOLDBL) != FUNCTION_PASS) return FUNCTION_FAIL;
	if (Read_BYTE(m_addrAWB.GOLDBH+(m_group.awb-1)*m_gapsize.awb, &m_valueAWB.GOLDBH) != FUNCTION_PASS) return FUNCTION_FAIL;

	return FUNCTION_PASS;
}

int OtpCheckBase::CheckValue()//检查AWB值是否满足设定范围
{
	if (m_addrAWB.RGH == UNEXCEPT_REG)
	{
		m_valueAWB.RG = m_valueAWB.RGL;
	}
	else
	{
		m_valueAWB.RG = m_valueAWB.RGH<<8 |m_valueAWB.RGL;
	}

	if (m_addrAWB.BGH == UNEXCEPT_REG)
	{
		m_valueAWB.BG = m_valueAWB.BGL;
	}
	else
	{
		m_valueAWB.BG = m_valueAWB.BGH<<8 |m_valueAWB.BGL;
	}

	if (m_addrAWB.GGH == UNEXCEPT_REG)
	{
		m_valueAWB.GG = m_valueAWB.GGL;
	}
	else
	{
		m_valueAWB.GG = m_valueAWB.GGH<<8 |m_valueAWB.GGL;
	}

	if (m_addrAWB.RH == UNEXCEPT_REG)
	{
		m_valueAWB.R = m_valueAWB.RL;
	}
	else
	{
		m_valueAWB.R = m_valueAWB.RH<<8 |m_valueAWB.RL;
	}

	if (m_addrAWB.BH == UNEXCEPT_REG)
	{
		m_valueAWB.B = m_valueAWB.BL;
	}
	else
	{
		m_valueAWB.B = m_valueAWB.BH<<8 |m_valueAWB.BL;
	}

	if (m_addrAWB.GrH == UNEXCEPT_REG)
	{
		m_valueAWB.Gr = m_valueAWB.GrL;
	}
	else
	{
		m_valueAWB.Gr = m_valueAWB.GrH<<8 |m_valueAWB.GrL;
	}

	if (m_addrAWB.GbH == UNEXCEPT_REG)
	{
		m_valueAWB.Gb = m_valueAWB.GbL;
	}
	else
	{
		m_valueAWB.Gb = m_valueAWB.GbH<<8 |m_valueAWB.GbL;
	}

	if (m_addrAWB.GOLDRGH == UNEXCEPT_REG)
	{
		m_valueAWB.GOLDRG = m_valueAWB.GOLDRGL;
	}
	else
	{
		m_valueAWB.GOLDRG = m_valueAWB.GOLDRGH<<8 |m_valueAWB.GOLDRGL;
	}

	if (m_addrAWB.GOLDBGH == UNEXCEPT_REG)
	{
		m_valueAWB.GOLDBG = m_valueAWB.GOLDBGL;
	}
	else
	{
		m_valueAWB.GOLDBG = m_valueAWB.GOLDBGH<<8 |m_valueAWB.GOLDBGL;
	}

	if (m_addrAWB.GOLDGGH == UNEXCEPT_REG)
	{
		m_valueAWB.GOLDGG = m_valueAWB.GOLDGGL;
	}
	else
	{
		m_valueAWB.GOLDGG = m_valueAWB.GOLDGGH<<8 |m_valueAWB.GOLDGGL;
	}

	if (m_addrAWB.GOLDRH == UNEXCEPT_REG)
	{
		m_valueAWB.GOLDR = m_valueAWB.GOLDRL;
	}
	else
	{
		m_valueAWB.GOLDR = m_valueAWB.GOLDRH<<8 |m_valueAWB.GOLDRL;
	}

	if (m_addrAWB.GOLDBH == UNEXCEPT_REG)
	{
		m_valueAWB.GOLDB = m_valueAWB.GOLDBL;
	}
	else
	{
		m_valueAWB.GOLDB = m_valueAWB.GOLDBH<<8 |m_valueAWB.GOLDBL;
	}

	if (m_addrAWB.GOLDGrH == UNEXCEPT_REG)
	{
		m_valueAWB.GOLDGr = m_valueAWB.GOLDGrL;
	}
	else
	{
		m_valueAWB.GOLDGr = m_valueAWB.GOLDGrH<<8 |m_valueAWB.GOLDGrL;
	}

	if (m_addrAWB.GOLDGbH == UNEXCEPT_REG)
	{
		m_valueAWB.GOLDGb = m_valueAWB.GOLDGbL;
	}
	else
	{
		m_valueAWB.GOLDGb = m_valueAWB.GOLDGbH<<8 |m_valueAWB.GOLDGbL;
	}

	if (m_bCheckRG)
	{
		if (m_valueAWB.RG<m_RGLower || m_valueAWB.RG>m_RGUpper)
		{
			m_szInfo.Format(L"RG %d 不在范围 (%d~%d)", m_valueAWB.RG, m_RGLower, m_RGUpper);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL; 
		}
	}

	if (m_bCheckBG)
	{
		if (m_valueAWB.BG<m_BGLower ||m_valueAWB.BG>m_BGUpper)
		{
			m_szInfo.Format(L"BG %d 不在范围 (%d~%d)", m_valueAWB.BG, m_BGLower, m_BGUpper);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL; 
		}
	}

	if (m_bCheckGG)
	{
		if (m_valueAWB.GG<m_GGLower ||m_valueAWB.GG>m_GGUpper)
		{
			m_szInfo.Format(L"GG %d 不在范围 (%d~%d)", m_valueAWB.GG, m_GGLower, m_GGUpper);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL; 
		}
	}

	if (m_bCheckR)
	{
		if (m_valueAWB.R<m_RLower || m_valueAWB.R>m_RUpper)
		{
			m_szInfo.Format(L"R %d 不在范围 (%d~%d)", m_valueAWB.R, m_RLower, m_RUpper);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGr)
	{
		if (m_valueAWB.Gr<m_GrLower ||m_valueAWB.Gr>m_GrUpper)
		{
			m_szInfo.Format(L"Gr %d 不在范围 (%d~%d)", m_valueAWB.Gr, m_GrLower, m_GrUpper);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}
	if (m_bCheckGb)
	{
		if (m_valueAWB.Gb<m_GbLower || m_valueAWB.Gb>m_GbUpper)
		{
			m_szInfo.Format(L"Gb %d 不在范围 (%d~%d)", m_valueAWB.Gb, m_GbLower, m_GbUpper);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckB)
	{
		if (m_valueAWB.B<m_BLower ||m_valueAWB.B>m_BUpper)
		{
			m_szInfo.Format(L"B %d 不在范围 (%d~%d)", m_valueAWB.B, m_BLower, m_BUpper);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGoldRG)
	{
		if (m_valueAWB.GOLDRG != m_GoldRG)
		{
			m_szInfo.Format(L"Golden RG %d 与 设定值 %d 不一致!", m_valueAWB.GOLDRG, m_GoldRG);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGoldBG)
	{
		if (m_valueAWB.GOLDBG != m_GoldBG)
		{
			m_szInfo.Format(L"Golden BG %d 与 设定值 %d 不一致!", m_valueAWB.GOLDBG, m_GoldBG);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGoldGG)
	{
		if (m_valueAWB.GOLDGG != m_GoldGG)
		{
			m_szInfo.Format(L"Golden GG %d 与 设定值 %d 不一致!", m_valueAWB.GOLDGG, m_GoldGG);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGoldR)
	{
		if (m_valueAWB.GOLDR != m_GoldR)
		{
			m_szInfo.Format(L"Golden R %d 与 设定值 %d 不一致!", m_valueAWB.GOLDR, m_GoldR);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGoldGr)
	{
		if (m_valueAWB.GOLDGr != m_GoldGr)
		{
			m_szInfo.Format(L"Golden Gr %d 与 设定值 %d 不一致!", m_valueAWB.GOLDGr, m_GoldGr);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGoldGb)
	{
		if (m_valueAWB.GOLDGb != m_GoldGb)
		{
			m_szInfo.Format(L"Golden Gb %d 与 设定值 %d 不一致!", m_valueAWB.GOLDGb, m_GoldGb);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGoldB)
	{
		if (m_valueAWB.GOLDB != m_GoldB)
		{
			m_szInfo.Format(L"Golden B %d 与 设定值 %d 不一致!", m_valueAWB.GOLDB, m_GoldB);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	return FUNCTION_PASS;
}

void OtpCheckBase::SaveOTPData()//保存当前OTP数据
{
	m_pInterface->AddLog(_T("此项目尚未添加读取OTP功能！"),COLOR_RED);
	return;
}