#include "StdAfx.h"
#include "OtpCheckBase.h"


OtpCheckBase::OtpCheckBase(void)
{
	memset(&m_addrFlag, 0, sizeof(m_addrFlag));
	memset(&m_addrSum, 0, sizeof(m_addrSum));
	memset(&m_otpSize, 0, sizeof(m_otpSize));
	memset(&m_addrCode, 0, sizeof(m_addrCode));
	memset(&m_otpPage, 0, sizeof(m_otpPage));

	m_slaveSensor = 0x0;
	m_slaveEeprom = 0x0;
	m_modeSensor = 0;
	m_modeEeprom = 0;
	m_regSize	= 0;

	m_flagValid = 0x00;
	m_flagEmpty = 0x00;
	m_bCheckStc = FALSE;	// 起始Code
	m_bCheckInf = FALSE;	// 远景Code
	m_bCheckMac = FALSE;	// 近景Code

// 	m_stcLower = 0;	// 起始Code下边界
// 	m_stcUpper = 0;	// 起始Code上边界
// 	m_infLower = 0;	//
// 	m_infUpper = 0;	//
// 	m_macUpper = 0;	//
// 	m_macLower = 0;	//
}


OtpCheckBase::~OtpCheckBase(void)
{
}

BOOL OtpCheckBase::CheckValidGroup()
{
	return TRUE;
}

BOOL OtpCheckBase::Check_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase Check_ModuleInfo");
	return TRUE;
}

BOOL OtpCheckBase::Check_AWB()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase Check_AWB");
	return TRUE;
}

BOOL OtpCheckBase::Check_LSC()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase Check_LSC");
	return TRUE;
}

BOOL OtpCheckBase::Check_AF()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase Check_AF");
	return TRUE;
}

BOOL OtpCheckBase::Check_PDAF()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase Check_PDAF");
	return TRUE;
}

BYTE OtpCheckBase::SumCalc(BYTE* pVal, int valSize)
{
	ULONGLONG sum = 0;
	for (int i = 0; i < valSize; i++)
	{
		sum += pVal[i];
	}
	sum %= 256;
	return (BYTE)sum;
}

BOOL OtpCheckBase::ReadSensor_Byte(WORD reg, BYTE* val)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase ReadSensor_Byte");
	return TRUE;
}

BOOL OtpCheckBase::ReadSensor_Word(WORD reg, WORD* val)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase ReadSensor_Word");
	return TRUE;
}

BOOL OtpCheckBase::WriteSensor_Byte(WORD reg, BYTE val)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase WriteSensor_Byte");
	return TRUE;
}

BOOL OtpCheckBase::WriteSensor_Word(WORD reg, WORD val)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase WriteSensor_Word");
	return TRUE;
}

BYTE OtpCheckBase::ReadSensor_MultiBytes(USHORT reg, BYTE regSize, BYTE *pVal, USHORT valSize, BOOL bStop /* = FALSE */)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase ReadSensor_MultiBytes");
	return TRUE;
}

BYTE OtpCheckBase::WriteSensor_MultiBytes(USHORT reg, BYTE regSize, BYTE *pVal, USHORT valSize)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase WriteSensor_MultiBytes");
	return TRUE;
}

BOOL OtpCheckBase::ReadEeprom_Byte(WORD reg, BYTE* val)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase ReadEeprom_Byte");
	return TRUE;
}

BOOL OtpCheckBase::ReadEeprom_Word(WORD reg, WORD* val)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase ReadEeprom_Word");
	return TRUE;
}

BOOL OtpCheckBase::WriteEeprom_Byte(WORD reg, BYTE val)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase WriteEeprom_Byte");
	return TRUE;
}

BOOL OtpCheckBase::WriteEeprom_Word(WORD reg, WORD val)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase WriteEeprom_Word");
	return TRUE;
}

BYTE OtpCheckBase::ReadEeprom_MultiBytes(USHORT reg, BYTE regSize, BYTE *pVal, USHORT valSize, BOOL bStop /* = FALSE */)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase ReadEeprom_MultiBytes");
	return TRUE;
}

BYTE OtpCheckBase::WriteEeprom_MultiBytes(USHORT reg, BYTE regSize, BYTE *pVal, USHORT valSize)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase WriteEeprom_MultiBytes");
	return TRUE;
}


BOOL OtpCheckBase::ReadSensorPage_Byte(WORD reg, BYTE* val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase ReadSensorPage_Byte");
	return TRUE;
}

BOOL OtpCheckBase::ReadSensorPage_Word(WORD reg, WORD* val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase ReadSensorPage_Word");
	return TRUE;
}

BOOL OtpCheckBase::WriteSensorPage_Byte(WORD reg, BYTE val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase WriteSensorPage_Byte");
	return TRUE;
}

BOOL OtpCheckBase::WriteSensorPage_Word(WORD reg, WORD val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase WriteSensorPage_Word");
	return TRUE;
}

BYTE OtpCheckBase::ReadSensorPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop /* = FALSE */)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase ReadSensorPage_MultiBytes");
	return TRUE;
}

BYTE OtpCheckBase::WriteSensorPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase WriteSensorPage_MultiBytes");
	return TRUE;
}

BOOL OtpCheckBase::ReadEepromPage_Byte(WORD reg, BYTE* val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase ReadEepromPage_Byte");
	return TRUE;
}

BOOL OtpCheckBase::ReadEepromPage_Word(WORD reg, WORD* val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase ReadEepromPage_Word");
	return TRUE;
}

BOOL OtpCheckBase::WriteEepromPage_Byte(WORD reg, BYTE val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase WriteEepromPage_Byte");
	return TRUE;
}

BOOL OtpCheckBase::WriteEepromPage_Word(WORD reg, WORD val, BYTE page)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase WriteEepromPage_Word");
	return TRUE;
}

BYTE OtpCheckBase::ReadEepromPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop /* = FALSE */)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase ReadEepromPage_MultiBytes");
	return TRUE;
}

BYTE OtpCheckBase::WriteEepromPage_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page)
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase WriteEepromPage_MultiBytes");
	return TRUE;
}

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

		//		Sleep(I2C_RETRY_WAIT);
	}

	CString info;
	info.Format(_T("I2C Read ERROR: %X"), reg);
	m_pInterface->AddLog(info, COLOR_RED);

	return FALSE;
}

BOOL OtpCheckBase::CheckFlag_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase CheckFlag_ModuleInfo");
	return TRUE;
}

BOOL OtpCheckBase::CheckFlag_AWB()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase CheckFlag_AWB");
	return TRUE;
}

BOOL OtpCheckBase::CheckFlag_LSC()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase CheckFlag_LSC");
	return TRUE;
}

BOOL OtpCheckBase::CheckFlag_AF()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase CheckFlag_AF");
	return TRUE;
}

BOOL OtpCheckBase::CheckFlag_PDAF()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase CheckFlag_PDAF");
	return TRUE;
}

BOOL OtpCheckBase::CheckSum_ModuleInfo()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase CheckSum_ModuleInfo");
	return TRUE;
}

BOOL OtpCheckBase::CheckSum_AWB()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase CheckSum_AWB");
	return TRUE;
}

BOOL OtpCheckBase::CheckSum_LSC()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase CheckSum_LSC");
	return TRUE;
}

BOOL OtpCheckBase::CheckSum_AF()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase CheckSum_AF");
	return TRUE;
}

BOOL OtpCheckBase::CheckSum_PDAF()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase CheckSum_PDAF");
	return TRUE;
}

BOOL OtpCheckBase::CheckCode_Stc()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase CheckCode_Stc");
	return TRUE;
}

BOOL OtpCheckBase::CheckCode_Inf()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase CheckCode_Inf");
	return TRUE;
}

BOOL OtpCheckBase::CheckCode_Mac()
{
	//m_pInterface->AddDebugLog(L"OtpCheckBase CheckCode_Mac");
	return TRUE;
}

BOOL OtpCheckBase::SaveOTPData()
{
	m_pInterface->AddLog(_T("此项目尚未添加读取OTP功能！"),COLOR_RED);
	return TRUE;
}

BOOL OtpCheckBase::GT24C64_Read(USHORT address,unsigned char *Data,int size)
{
	
	int i = 0;
	int group =0;
	int num=0;
	//	byte *CheckData=new byte[size];
	int Count= address%32;
	Count=32-Count;
	if(size-Count>0)
	{
		group = (size-Count)/32;
		num=(size-Count)%32;
	}
	else
	{
		group =0;
		num=size;
		Count=0;
	}
    if(Count>0)
		m_pInterface->I2C_MultiBytesRead(0xA0,address,2,Data,Count);
	Sleep(30);
	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,Data+(Count+i*32),32);
		Sleep(10);
	}
	m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,Data+(Count+i*32),num);
	return TRUE;
	
}

BOOL OtpCheckBase::GT24C64_Programming(USHORT address,unsigned char *Data,int size)
{
	int i = 0;

	/*
	for (i=0;i<1098;i++)
	{
	WriteSensorI2c(0xB0,0x0000+i*3,2,Data+i*3,3);
	}
	WriteSensorI2c(0xB0,0x0000+i*3,2,Data+i*3,1);
	*/
	for (i=0;i<size;i++)
	{
		USHORT WData=Data[i];
        m_pInterface->I2C_Write(0xA0,i+address,WData,3);
	}
	
	BYTE ReadData[4384]={0};
	Sleep(100);   
	for (int k=0;k<137;k++)
	{
		m_pInterface->I2C_MultiBytesRead(0xA0,0x0000+k*32,2,ReadData+k*32,32);
		Sleep(10);
	}
	for (i=address;i<(address+size);i++)
	{
		if(ReadData[i]!=Data[i-address])
			
		return FALSE;
		
	}
	return TRUE;
}

BOOL OtpCheckBase::ZC533_Read(USHORT address,unsigned char *Data,int size)
{
	int pagesize = 64;
	int i=0;
	int j=0;
	int group = 0;
	int num = 0;
	int retgroup = 0;
	int retnum = 0;
	int retsize = 0;
	int Totalretsize = 0;
	int page = address/pagesize;
	int reg =address%pagesize;

	int Count=pagesize-page;
	int ret = size-Count;
	if(ret>0)
	{
		group = (size-Count)/pagesize;
		num=(size-Count)%pagesize;
	}
	else
	{
		group=0;
		num=size;
		Count=0;
	}
	retsize = pagesize-reg;
	Totalretsize = size- retsize;
	retgroup = Totalretsize/pagesize;
	retnum = Totalretsize%pagesize;

	if (ret>0)
	{
		if(Count>0)
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|(page<<6)|(reg&0x3f)),2,Data,retsize);
		Sleep(10);

		for (i=0;i<retgroup;i++)
		{
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),pagesize);//|(reg&0x3f)   +(Count+i*pagesize)
			Sleep(10);
		}
		m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),retnum);//|(reg&0x3f)   +(Count+i*pagesize)
	}
	else
	{
		m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page)<<6)|(reg&0x3f))+(Count+i*32),2,Data+(Count+i*32),num);
		Sleep(10);
	}

	return TRUE;
}