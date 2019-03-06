#include "stdafx.h"
#include "OtpBase.h"


OtpSensorStruct COtpBase::m_otpSensor;
OtpPowerStruct COtpBase::m_otpPower;
tagOtpCtrl COtpBase::m_otpCtrl;

OtpIDStruct	COtpBase::m_IDGroup;

LencOtpControlStruct COtpBase::m_lencCtrl;
//LencRegStruct COtpBase::m_lencReg;

tagAwbOtpCtrl COtpBase::m_wbCtrl;
WBGainRegStruct COtpBase::m_wbGainReg;
tagAwbRatio COtpBase::m_wbGolden;	
ExpControlStruct COtpBase::m_expCtrl;
ExpRegisterStruct COtpBase::m_expRegister;
ExpRangeStruct COtpBase::m_expRange;
MTKPDAFStruct COtpBase::m_mtkpdaf;


CString COtpBase::SetOtpName(CString otpName)
{
	CString oldName = m_otpName;
	m_otpName = otpName;
	return oldName;
}

CString COtpBase::GetOtpName(void)
{
	return m_otpName;
}

OtpSensorStruct* COtpBase::GetOtpSensorParams(void)
{
	return &m_otpSensor;
}

OtpPowerStruct* COtpBase::GetOtpPowerParams(void)
{
	return &m_otpPower;
}

tagOtpCtrl* COtpBase::GetControlParams(void)
{
	return &m_otpCtrl;
}

void COtpBase::SetEepromParams(EepromStruct *eeprom)
{
	m_Eeprom.deviceID = eeprom->deviceID;
	m_Eeprom.addrByteSize = eeprom->addrByteSize;
	m_Eeprom.pageSize = eeprom->pageSize;
	m_Eeprom.msWriteTime = eeprom->msWriteTime;
}
ExpControlStruct* COtpBase::GetExpControlParams(void) 
{
	return &m_expCtrl;
}

ExpRegisterStruct* COtpBase::GetExpRegister(void)				 
{
	return &m_expRegister;
}

ExpRangeStruct* COtpBase::GetExpRangeParams(void)
{
	return &m_expRange;
}

OtpIDStruct* COtpBase::GetIDGroup(void)
{
	return &m_IDGroup;
}

OtpIDStruct* COtpBase::GetIDCurrent(void)					 
{
	return &m_IDCurrent;
}

MTKPDAFStruct* COtpBase::GetMTKPDAFParams(void)
{
	return &m_mtkpdaf;
}


void COtpBase::SetSensorOB(BYTE OB)
{
	m_sensorOB = OB;
}

BYTE COtpBase::GetSensorOB(void) const	
{
	return m_sensorOB;
}

tagAwbOtpCtrl* COtpBase::GetWBControlParams(void)
{
	return &m_wbCtrl;
}

WBGainRegStruct* COtpBase::GetWBGainRegister(void)
{
	return &m_wbGainReg;
}		

tagAwbRatio* COtpBase::GetWBGoldenValue(void)					 
{
	return &m_wbGolden;
}

tagAwbRatio* COtpBase::GetWBCurrentValue(void)					 
{
	return &m_wbCurrent;
}

LencOtpControlStruct* COtpBase::GetLencControlParams(void)
{
	return &m_lencCtrl;
}


BOOL COtpBase::GT24C64_Read(USHORT address,unsigned char *Data,int size)
{
	int i=0;
	int group=0;
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
		group=0;
		num=size;
		Count=0;
	}
    if(Count>0)
		m_pInterface->I2C_MultiBytesRead(0xA0,address,2,Data,Count);
	Sleep(m_sleeptime*30);
	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,Data+(Count+i*32),32);
		Sleep(m_sleeptime*10);
	}
	m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,Data+(Count+i*32),num);
	return TRUE;
	
}

BOOL COtpBase::GT24C64_Programming(USHORT address,unsigned char *Data,int size)
{
	int i=0;
	int group=0;
	int num=0;
	BYTE* CheckData=new BYTE[size]();
	BOOL bRet = TRUE;

	do 
	{	
		Sleep(m_sleeptime*10);
		int Count= address%32;
		Count=32-Count;
		if(size-Count>0)
		{
			group = (size-Count)/32;
			num=(size-Count)%32;
		}
		else
		{
			group=0;
			num=size;
			Count=0;
		}
		m_pInterface->I2C_MultiBytesWrite(0xA0,address,2,Data,Count);
		Sleep(m_sleeptime*10);

		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesWrite(0xA0,address+(Count+i*32),2,Data+(Count+i*32),32);
			Sleep(m_sleeptime*10);
		}
		m_pInterface->I2C_MultiBytesWrite(0xA0,address+(Count+i*32),2,Data+(Count+i*32),num);
		Sleep(m_sleeptime*10);
		if(Count>0)
			m_pInterface->I2C_MultiBytesRead(0xA0,address,2,CheckData,Count);

		Sleep(m_sleeptime*10);
		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,CheckData+(Count+i*32),32);
			Sleep(m_sleeptime*10);
		}
		m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,CheckData+(Count+i*32),num);
		for (i=0;i<size;i++)
		{
			if (CheckData[i]!=Data[i])
			{
				bRet = FALSE;
				break;
			}
		}
	} while (0);

	SAFE_DELETE_ARRAY(CheckData);
	return bRet;
}

BOOL COtpBase::ZC533_Page_Erase(BYTE page)
{
	//m_pInterface->AddDebugLog(L"TKU8341M WriteEepromPage_Byte");
	ZC533_Erase_lock = ZC533_LastPage - page;
	ZC533_LastPage = page;
	BOOL bRet = TRUE;
	if (ZC533_Erase_lock != 0)//不允许连续两次erase 同一page
	{
		bRet &= m_pInterface->I2C_Write(0xB0, ((ZC533_PAGE_ERASE<<14)|(page<<6)), 0, I2CMODE_16_8);
		Sleep(6*m_sleeptime);
	}

	return bRet;
}

BOOL COtpBase::ZC533_All_Erase()
{
	BOOL bRet = TRUE;
	bRet &= m_pInterface->I2C_Write(0xB0, (ZC533_ALL_ERASE<<14), 0, I2CMODE_16_8);
	Sleep(6*m_sleeptime);
	return bRet;
}

BOOL COtpBase::ZC533_Read(USHORT address,unsigned char *Data,int size)
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
		Sleep(m_sleeptime*10);

		for (i=0;i<retgroup;i++)
		{
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),pagesize);//|(reg&0x3f)   +(Count+i*pagesize)
			Sleep(m_sleeptime*10);
		}
		m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),retnum);//|(reg&0x3f)   +(Count+i*pagesize)
	}
	else
	{
		m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page)<<6)|(reg&0x3f))+(Count+i*32),2,Data+(Count+i*32),num);
		Sleep(m_sleeptime*10);
	}

	return TRUE;
}

BOOL COtpBase::ZC533_Programming(USHORT address,unsigned char *Data,int size)//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page
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
	BYTE* CheckData=new BYTE[size]();
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

	BOOL bRet = TRUE;
	do 
	{
		if (ret>0)
		{
			if (!ZC533_Page_Erase(page))	
			{
				bRet = FALSE;
				break;
			}
			m_pInterface->I2C_MultiBytesWrite(0xB0,((ZC533_WRITE<<14)|(page<<6)|(reg&0x3f)),2,Data,retsize);
			Sleep(m_sleeptime*10);

			for (i=0;i<retgroup;i++)
			{
				if (!ZC533_Page_Erase(page+i+1))	
				{
					bRet = FALSE;
					break;
				}
// 			for (j=0; j<pagesize; j++)//如果64个连续写不进，可以考虑改成32个连续写
// 			{
					m_pInterface->I2C_MultiBytesWrite(0xB0,((ZC533_WRITE<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),pagesize);//|(j&0x3f)
					Sleep(m_sleeptime*10);
//				}
			}
			if (!ZC533_Page_Erase(page+i+1))	
			{
				bRet = FALSE;
				break;
			}
			m_pInterface->I2C_MultiBytesWrite(0xB0,((ZC533_WRITE<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),retnum);//|(reg&0x3f))+(Count+i*pagesize)
			Sleep(m_sleeptime*10);
		}
		else
		{
			if (!ZC533_Page_Erase(page))	
			{
				bRet = FALSE;
				break;
			}
			m_pInterface->I2C_MultiBytesWrite(0xB0,((ZC533_WRITE<<14)|(page<<6)|(reg&0x3f))+(Count+i*pagesize),2,Data+(Count+i*pagesize),num);
			Sleep(m_sleeptime*10);
		}
		////////////////////////////////Read//////////////////////////////////////////
		if (ret>0)
		{
			if(Count>0)
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|(page<<6)|(reg&0x3f)),2,CheckData,retsize);
			Sleep(m_sleeptime*5);

			for (i=0;i<retgroup;i++)
			{
				m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,CheckData+(retsize+i*pagesize),pagesize);//|(reg&0x3f)   +(Count+i*pagesize)
				Sleep(m_sleeptime*5);
			}
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,CheckData+(retsize+i*pagesize),retnum);//|(reg&0x3f)   +(Count+i*pagesize)
			Sleep(m_sleeptime*5);
			for (i=0;i<size;i++)
			{
				if (CheckData[i]!=Data[i])
				{
					m_pInterface->AddLog(_T("读写对比失败!"),COLOR_RED);
					bRet = FALSE;
					break;
				}
			}
		}
		else
		{
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page)<<6)|(reg&0x3f))+(Count+i*32),2,CheckData+(Count+i*32),num);
			Sleep(m_sleeptime*5);
			for (i=0;i<size;i++)
			{
				if (CheckData[i]!=Data[i])
				{
					m_pInterface->AddLog(_T("读写对比失败!"),COLOR_RED);
					bRet = FALSE;
					break;
				}
			}
		}
	} while (0);

	SAFE_DELETE_ARRAY(CheckData);
	return bRet;
}

BOOL COtpBase::DW9763_Page_Erase(BYTE page)
{
	BOOL bRet = TRUE;

	bRet &= m_pInterface->I2C_Write(0xB0, ((1<<7)|(page<<2)|(0x02)), 0, I2CMODE_8_8);
	Sleep(4*m_sleeptime);

	return bRet;
}

BOOL COtpBase::DW9763_All_Erase()
{
	BOOL bRet = TRUE;
	bRet &= m_pInterface->I2C_Write(0xB0, 0x81, 0xee, I2CMODE_8_8);
	Sleep(15*m_sleeptime);

	m_pInterface->AddLog(_T("DW9763 All Data Erased!"),COLOR_LIGHTGOLDENRODYELLOW);
	return bRet;
}

BOOL COtpBase::DW9763_Read(USHORT address,unsigned char *Data,int size)
{
	int i=0;
	int group=0;
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
		group=0;
		num=size;
		Count=0;
	}
	if(Count>0)
		m_pInterface->I2C_MultiBytesRead(0xB0,address,2,Data,Count);
	Sleep(m_sleeptime*30);
	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,Data+(Count+i*32),32);
		Sleep(m_sleeptime*10);
	}
	m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,Data+(Count+i*32),num);
	return TRUE;
}

BOOL COtpBase::DW9763_Programming(USHORT address,unsigned char *Data,int size)//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page
{
	int i=0;
	int group=0;
	int num=0;
	BYTE* CheckData=new BYTE[size]();
	BOOL bRet = TRUE;

	do 
	{	
		Sleep(m_sleeptime*10);
		int Count= address%32;
		Count=32-Count;
		if(size-Count>0)
		{
			group = (size-Count)/32;
			num=(size-Count)%32;
		}
		else
		{
			group=0;
			num=size;
			Count=0;
		}
		m_pInterface->I2C_MultiBytesWrite(0xB0,address,2,Data,Count);
		Sleep(m_sleeptime*10);

		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesWrite(0xB0,address+(Count+i*32),2,Data+(Count+i*32),32);
			Sleep(m_sleeptime*10);
		}
		m_pInterface->I2C_MultiBytesWrite(0xB0,address+(Count+i*32),2,Data+(Count+i*32),num);
		Sleep(m_sleeptime*10);
		if(Count>0)
			m_pInterface->I2C_MultiBytesRead(0xB0,address,2,CheckData,Count);

		Sleep(m_sleeptime*10);
		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,CheckData+(Count+i*32),32);
			Sleep(m_sleeptime*10);
		}
		m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,CheckData+(Count+i*32),num);
		for (i=0;i<size;i++)
		{
			if (CheckData[i]!=Data[i])
			{
				bRet = FALSE;
				break;
			}
		}
	} while (0);

	SAFE_DELETE_ARRAY(CheckData);
	return bRet;
}

BOOL COtpBase::eeprom_write(USHORT addr, USHORT reg, BYTE val)
{
	BOOL ret = i2c_multi_write(addr, reg, m_Eeprom.addrByteSize, &val, 1);
	Sleep(m_Eeprom.msWriteTime);
	return ret;
}

BOOL COtpBase::eeprom_read(USHORT addr, USHORT reg, BYTE *pval)
{
	return i2c_multi_read(addr, reg, m_Eeprom.addrByteSize, pval, 1);
}

BOOL COtpBase::eeprom_multi_write(USHORT addr, USHORT reg, BYTE *pval, USHORT length)
{
	BOOL ret = TRUE;

	if (length < m_Eeprom.pageSize)
	{
		ret = i2c_multi_write(addr, reg, m_Eeprom.addrByteSize, pval, length);
		Sleep(m_Eeprom.msWriteTime);
		return ret;
	}

	BYTE *dst = pval;
	int num = length;

	if (reg % m_Eeprom.pageSize != 0)
	{
		int mod = m_Eeprom.pageSize - reg % m_Eeprom.pageSize;

		ret = i2c_multi_write(addr, reg, m_Eeprom.addrByteSize, dst, mod);
		Sleep(m_Eeprom.msWriteTime);

		dst += mod;
		reg += mod;

		num = length - mod;
	}

	int cycles = num / m_Eeprom.pageSize;

	for (int i=0; i<cycles; i++)
	{
		ret = i2c_multi_write(addr, reg, m_Eeprom.addrByteSize, dst, m_Eeprom.pageSize);
		Sleep(m_Eeprom.msWriteTime);

		reg += m_Eeprom.pageSize;
		dst += m_Eeprom.pageSize;
	}

	int mod = num - cycles * m_Eeprom.pageSize;
	if (mod != 0)
	{		
		ret = i2c_multi_write(addr, reg, m_Eeprom.addrByteSize, dst, mod);
		Sleep(m_Eeprom.msWriteTime);		
	}

	return ret;
}

BOOL COtpBase::eeprom_multi_read(USHORT addr, USHORT reg, BYTE *pval, USHORT length)
{
	if (length < m_Eeprom.pageSize)
	{
		return i2c_multi_read(addr, reg, m_Eeprom.addrByteSize, pval, length);
	}

	BOOL ret = TRUE;

	BYTE *dst = pval;
	int num = length;

	if (reg % m_Eeprom.pageSize != 0)
	{
		int mod = m_Eeprom.pageSize - reg % m_Eeprom.pageSize;

		ret = i2c_multi_read(addr, reg, m_Eeprom.addrByteSize, dst, mod);		
		dst += mod;
		reg += mod;

		num = length - mod;
	}

	int cycles = num / m_Eeprom.pageSize;

	for (int i=0; i<cycles; i++)
	{
		ret = i2c_multi_read(addr, reg, m_Eeprom.addrByteSize, dst, m_Eeprom.pageSize);		
		reg += m_Eeprom.pageSize;
		dst += m_Eeprom.pageSize;
	}

	int mod = num - cycles * m_Eeprom.pageSize;
	if (mod != 0)
	{		
		ret = i2c_multi_read(addr, reg, m_Eeprom.addrByteSize, dst, mod);
	}

	return ret;
}

BOOL COtpBase::eeprom_write(USHORT reg, BYTE val)
{
	return eeprom_write(m_Eeprom.deviceID, reg, val);
}

BOOL COtpBase::eeprom_read(USHORT reg, BYTE *pval)
{
	return eeprom_read(m_Eeprom.deviceID, reg, pval);
}

BOOL COtpBase::eeprom_multi_write(USHORT reg, BYTE *pval, USHORT length)
{
	return eeprom_multi_write(m_Eeprom.deviceID, reg, pval, length);
}

BOOL COtpBase::eeprom_multi_read(USHORT reg, BYTE *pval, USHORT length)
{
	return eeprom_multi_read(m_Eeprom.deviceID, reg, pval, length);
}
BOOL COtpBase::i2c_multi_read(USHORT addr, USHORT reg, BYTE regSize, BYTE *pval, USHORT length, BOOL noStop/*=FALSE*/)
{
	if ((pval == NULL))	return FALSE;

	for (int i=0; i<I2C_RETRY_NUM; i++)
	{
		if (m_pInterface->I2C_MultiBytesRead(addr, reg, regSize, pval, length, noStop))
			return TRUE;

		m_szStr.Format(_T("I2C Multi Read ERROR: %02X, %X, %d bytes"), addr, reg, length);
		m_pInterface->AddLog(m_szStr);

		Sleep(I2C_RETRY_WAIT);
	}

	m_szStr.Format(_T("I2C Multi Read ERROR: %02X, %X, %d bytes"), addr, reg, length);
	m_pInterface->AddLog(m_szStr, COLOR_RED);

	return FALSE;
}

BOOL COtpBase::i2c_write(USHORT reg, USHORT val)
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

BOOL COtpBase::i2c_read(USHORT reg, USHORT *pval)
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

BOOL COtpBase::i2c_write_byte(USHORT reg, BYTE val)
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

BOOL COtpBase::i2c_read_byte(USHORT reg, BYTE *pval)
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

BOOL COtpBase::i2c_multi_write(USHORT reg, BYTE *pval, USHORT length)
{
	if ((pval == NULL))
	{
		return FALSE;
	}

	for (int i=0; i<I2C_RETRY_NUM; i++)
	{
		if (i2c_multi_write(reg, pval, length))
		{
			return TRUE;
		}

		CString info;
		info.Format(_T("I2C Multi Write ERROR: %X, %d bytes"), reg, length);
		m_pInterface->AddLog(info);

		Sleep(I2C_RETRY_WAIT);
	}

	CString info;
	info.Format(_T("I2C Multi Write ERROR: %X, %d bytes"), reg, length);
	m_pInterface->AddLog(info, COLOR_RED);

	return FALSE;
}

BOOL COtpBase::i2c_multi_write(USHORT addr, USHORT reg, BYTE regSize, BYTE *pval, USHORT length)
{
	if ( (pval == NULL))
	{
		return FALSE;
	}

	for (int i=0; i<I2C_RETRY_NUM; i++)
	{
		if (m_pInterface->I2C_MultiBytesWrite(addr, reg, regSize, pval, length))
		{
			return TRUE;
		}

		CString info;
		info.Format(_T("I2C Multi Write ERROR: %02X, %X, %d bytes"), addr, reg, length);
		m_pInterface->AddLog(info);

		Sleep(I2C_RETRY_WAIT);
	}

	CString info;
	info.Format(_T("I2C Multi Write ERROR: %02X, %X, %d bytes"), addr, reg, length);
	m_pInterface->AddLog(info, COLOR_RED);

	return FALSE;
}

BOOL COtpBase::i2c_multi_read(USHORT reg, BYTE *pval, USHORT length, BOOL noStop/*=FALSE*/)
{
	if ((pval == NULL))
	{
		return FALSE;
	}

	for (int i=0; i<I2C_RETRY_NUM; i++)
	{
		if (i2c_multi_read(reg, pval, length, noStop))
		{
			return TRUE;
		}

		CString info;
		info.Format(_T("I2C Multi Read ERROR: %X, %d bytes"), reg, length);
		m_pInterface->AddLog(info);		

		Sleep(I2C_RETRY_WAIT);
	}

	CString info;
	info.Format(_T("I2C Multi Read ERROR: %X, %d bytes"), reg, length);
	m_pInterface->AddLog(info, COLOR_RED);		

	return FALSE;
}


int COtpBase::SumCalc(BYTE *data, int num, int mod,int add)
{
	ASSERT(data != NULL);

	ULONG  sum = 0;

	for (int i=0; i<num; i++)
	{
		sum += data[i];
	}

	return sum % mod+add;
}

BOOL COtpBase::InitProgram(void)
{
	CString path;
	CString strWorkPath  = GetModulePath()+"\\LSC";
	CreateDirectory(strWorkPath, NULL);
	if (m_LSCItem == 0)
	{
		path = strWorkPath+_T("\\MTK");
	}
	else if(m_LSCItem == 1)
	{
		path = strWorkPath+_T("\\Qual");
	}
	else if (m_LSCItem == 2)
	{
		path = strWorkPath+_T("\\Sensor");
	}
	CreateDirectory(path, NULL);

	m_proc1data = NULL;
	m_mtklscdata = NULL;
	m_quallscdata = NULL;
	ZC533_Erase_lock = 0;
	ZC533_LastPage = -1;
	SaveExp = 0;
	if (m_PDAFItem == 1)
	{
		InitQualParam();
	}
	return TRUE;
}

BOOL COtpBase::EndProgram(void)
{
	FreeDll();
	SAFE_DELETE(m_pShading);
	SAFE_DELETE_ARRAY(m_proc1data);
	SAFE_DELETE_ARRAY(m_mtklscdata);
	SAFE_DELETE_ARRAY(m_quallscdata);
	return TRUE;
}

BOOL COtpBase::FreeDll(void)
{
	return TRUE;
}

BOOL COtpBase::InitOTP(void)
{
    if(!LoadDll()) 	return FALSE; //此处调用动态dll 库

	if (m_berase && !SensorLSCSta )//sensor lsc一定属于sensor otp，一定无法erase
	{
		if (!EraseAllData())  return FALSE;
	}

	if (m_EnAWB)
	{
		SetWBDefaultGain();	
	}

	if (m_EnLSC && SensorLSCSta)  //For Sensor LSC
	{
		DisableLenc();
	}

	if (m_EnAF)
	{
		if (!GetAFData()) 		return FALSE;
	}

	if (m_EnPDAF)
	{
		if (m_PDAFItem == 0)
		{
			PreProc1();
		}

	}
	return TRUE;
}

BOOL COtpBase :: LoadDll()
{
	return TRUE;
}

BOOL COtpBase :: EraseAllData()
{
	return TRUE;
}

BOOL COtpBase :: GetAFData()
{
	m_pInterface ->AddLog(_T("Get AF Data Start"));

	m_infCode = 0, m_macCode = 0,m_stcCode = 0;//init

	vector<int>* vpStcAF = m_pInterface->GetStorage(_T("AutoFocusStart"));
	vector<int>* vpMacAF = m_pInterface->GetStorage(_T("AutoFocusNear"));
	vector<int>* vpInfAF = m_pInterface->GetStorage(_T("AutoFocusFar"));

#ifndef  TestVersion
	if (m_bcheckinf)
	{
		if (vpInfAF == NULL || vpInfAF->size() == 0)
		{
			m_infCode = 0;
			info = _T("远景Code值为空");
			m_pInterface->AddLog(info, COLOR_RED);
			return FALSE;
		}
		else
			m_infCode = vpInfAF->at(0);

		if(m_infCode > m_InfH || m_infCode < m_InfL)
		{
			info.Format(_T("远景Code(%d)超标,请重测远景"), m_infCode);
			m_pInterface->AddLog(info, COLOR_RED);
			return FALSE;
		}
	}


	if (m_bcheckmac)
	{
		if (vpMacAF == NULL || vpMacAF->size() == 0)
		{
			m_macCode = 0;
			info = _T("近景Code值为空");
			m_pInterface->AddLog(info, COLOR_RED);
			return FALSE;
		}
		else
			m_macCode = vpMacAF->at(0);

		if (m_macCode > m_MacH || m_macCode < m_MacL)
		{
			info.Format(_T("近景Code(%d)超标,请重测近景"), m_macCode);
			m_pInterface->AddLog(info, COLOR_RED);
			return FALSE;
		}
	}

	if (m_bchecksta)
	{
		if (vpStcAF == NULL || vpStcAF->size() == 0)
		{
			m_stcCode = 0;
			info = _T("起始Code值为空");
			m_pInterface->AddLog(info, COLOR_RED);
			return FALSE;
		}
		else
			m_stcCode = vpStcAF->at(0);

		if (m_stcCode > m_StcH || m_macCode < m_StcL)
		{
			info.Format(_T("起始Code(%d)超标,请重测远景"), m_stcCode);
			m_pInterface->AddLog(info, COLOR_RED);
			return FALSE;
		}
	}

	if (m_bcheckmac && m_bcheckinf)
	{
		if (m_infCode >= m_macCode-m_AFDif || m_infCode>m_macCode)
		{
			info.Format(_T("远景Code(%d)近景Code(%d)太接近，请重新对焦"), m_infCode, m_macCode);
			m_pInterface->AddLog(info, COLOR_RED);
			return FALSE;
		}
	}

	info.Format(_T("远景Code: %d\n近景Code: %d"), m_infCode, m_macCode);
	m_pInterface->AddLog(info, COLOR_BLUE);
#endif

	/////////////////////////only for test///////////////////////////////////////////
#ifdef  TestVersion
	m_pInterface->AddLog(_T("This is Test Version!"),COLOR_RED);
	m_infCode = 180;
	m_macCode = 432;
#endif
	//////////////////////////////////////////////////////////////////////////

	m_pInterface ->AddLog(_T("Get AF Data End"));
	return TRUE;
}

void COtpBase :: PreProc1()
{
	m_pInterface->GetDriverIC()->WriteAF(m_macCode);
	info.Format(L"Prepare Proc1,Lens Peak to: %d", m_macCode);
	m_pInterface->AddLog(info);
	return;
}

BOOL COtpBase ::CheckAF()
{
	return TRUE;
}

BOOL COtpBase::MTK_GetProc1(void)
{
	CString Path;//Raw
	CString str;

	CString Path2;//QVL Raw
	CString Path3;//QVL Raw2


	//创建保存路径
	FileFolderPath = "";
	Path = m_pInterface->GetExeDir();
	FileFolderPath += Path + _T("PD_CALI_MTK_FILE\\");
	CreateDirectory(FileFolderPath, NULL);

	if (!m_EnQVL)
	{
		Path =FileFolderPath+_T("\\PD_CALI_MTK_STEP1.raw");
	}
	else
	{
		CString FileFolderPath1;// ../Proc1/x.raw
		str.Format(_T("%d\\"),m_pInterface->GetModuleNum());
		FileFolderPath1 = FileFolderPath + str;
		CreateDirectory(FileFolderPath1,NULL);
		FileFolderPath1 = FileFolderPath1+_T("proc1\\");
		CreateDirectory(FileFolderPath1,NULL);
		str.Format(_T("%d.raw"),m_pInterface->GetModuleNum());
		Path = FileFolderPath1 + str;


		CString FileFolderPath2;// ../5000K/x.raw 
		str.Format(_T("%d\\"),m_pInterface->GetModuleNum());
		FileFolderPath2 = FileFolderPath + str;
		CreateDirectory(FileFolderPath2,NULL);
		FileFolderPath2 = FileFolderPath2+_T("5000K\\");
		CreateDirectory(FileFolderPath2,NULL);
		str.Format(_T("%d.raw"),m_pInterface->GetModuleNum());
		Path2 = FileFolderPath2 + str;

		CString FileFolderPath3;// ../../5000K/x.raw  just  for QVL count
		str.Format(_T("%d\\"),m_pInterface->GetModuleNum());
		FileFolderPath3 = FileFolderPath+_T("5000K\\");
		CreateDirectory(FileFolderPath3,NULL);
		str.Format(_T("%d.raw"),m_pInterface->GetModuleNum());
		Path3 = FileFolderPath3 + str;
	}

	USHORT *rawbuff = m_pInterface->GetImage().Raw_Buf;
	int width = m_pInterface->GetImage().width;
	int height = m_pInterface->GetImage().height;
	//////////////////////////////////////////////////////////////////////////
#ifndef  TestVersion
	if (PathFileExists(Path)) 
	{
		if (!DeleteFile(Path))
		{
			m_pInterface->AddLog(_T("无法删除原Raw图!"), COLOR_RED);
			return FALSE;
		}
	}

	SaveCurrentRaw10(Path,rawbuff,width, height);//保存Raw图  
	if (m_EnQVL)
	{
		if (PathFileExists(Path2)) 
		{
			if (!DeleteFile(Path2))
			{
				m_pInterface->AddLog(_T("无法删除原Raw图!"), COLOR_RED);
				return FALSE;
			}
		}
		SaveCurrentRaw10(Path2,rawbuff,width, height);
		if (PathFileExists(Path3)) 
		{
			if (!DeleteFile(Path3))
			{
				m_pInterface->AddLog(_T("无法删除原Raw图!"), COLOR_RED);
				return FALSE;
			}
		}
		SaveCurrentRaw10(Path3,rawbuff,width, height);
	}
//	SAFE_DELETE(rawbuff);
	m_pInterface->AddLog(_T("PDAF Proc1 Raw Saved"));
#endif
	//////////////////////////////////////////////////////////////////////////

	m_pInterface->GetDriverIC()->WriteAF(0);//AF回归
	m_pInterface->AddLog(_T("Lens Code set Zero!"));

	FILE* fp;
	if (_wfopen_s(&fp,  Path, _T("rb")))  
	{
		info.Format(_T("找不到文件%s!"),Path);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}

	if (fp)
	{
		raw_seq = new unsigned short[width*height];
		fread((void*)raw_seq,2,width*height,fp);//读取Raw图
		fclose(fp);
	}

	int max_size = 0;
	int check_width=0;
	int check_height=0;
	int i = 0;
	CString settingfilepath = GetModulePath()+"\\PDAF\\MTK"+m_MTK_INI_Name;
	BOOL result = TRUE;

	int errornum = pd_cali_init(CT2A(settingfilepath));
	if (errornum != 0)
	{
		m_pInterface->AddLog(_T("Setting sample init failed!"), COLOR_RED);
		SAFE_DELETE_ARRAY(raw_seq);
		return FALSE;
	}
	errornum = pd_cali_get_max_output_size(max_size);
	if (errornum != 0)
	{
		m_pInterface->AddLog(_T("Get MaxSize failed!"), COLOR_RED);
		SAFE_DELETE_ARRAY(raw_seq);
		return FALSE;
	}
	errornum= pd_cali_get_raw_width_height(check_width,check_height);
	if (errornum != 0)
	{
		m_pInterface->AddLog(_T("pd_cali_get_raw_width_height运算出错!"), COLOR_RED);
		SAFE_DELETE_ARRAY(raw_seq);
		return FALSE;
	}

	int output_size = -1;
	char* output = new char[max_size];
	memset(output,0,max_size); 
	do 
	{
		errornum = pd_cali_proc1(raw_seq,output,max_size,output_size);//Step1 运算

		if (errornum!=0)
		{
			if (errornum == 3)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1载入参数错误!"), COLOR_RED);
				result &= FALSE;
				break;
			}
			if (errornum == 4)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1生成数据size错误!"), COLOR_RED);
				result &= FALSE;
				break;
			}
			if (errornum == 5)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1之前未进行初始化!"), COLOR_RED);
				result &= FALSE;
				break;
			}
			if (errornum == 6)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1 Raw图异常!"), COLOR_RED);
				result &= FALSE;
				break;
			}
			if (errornum == 7)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1生成数据异常!"), COLOR_RED);
				result &= FALSE;
				break;
			}
			if (errornum == 8)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1 quality check fail!"), COLOR_RED);
				result &= FALSE;
				break;
			}
		}
		else
		{
			m_pInterface->AddLog(_T("pd_cali_proc1 success!"), COLOR_BLUE);
		}

		m_proc1data = new unsigned char[output_size];
		for (i=0; i<output_size; i++)
		{
			m_proc1data[i] = output[i];
		}
	} while (0);


	SAFE_DELETE_ARRAY(raw_seq);
	if (!result)
	{
		SAFE_DELETE_ARRAY(output);
		return result;
	}
	//////////////////////////////Verify//////////////////////////////////////
	BOOL bRet = TRUE;
	BYTE* Proc2_Read =  new BYTE[m_mtkpdaf.Proc2Size];
	BYTE *PDAF_Data = new BYTE[m_mtkpdaf.TotalSize];
	char *PdafBin = new char[m_mtkpdaf.TotalSize];
	do 
	{
		errornum = pd_cali_init(CT2A(settingfilepath));
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("PDAF Ini Init Fail!"), COLOR_RED);
			bRet &= FALSE;		
			break;
		}
		errornum = pd_cali_verify_proc1(m_proc1data);
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("PDAF Proc1 Verify Fail!"), COLOR_RED);
			bRet &= FALSE;	
			break;
		}
		PDAFSaveProc1Bin(output, output_size);


		if (!GT24C64_Read(m_mtkpdaf.Proc2_FirstAddr, Proc2_Read, m_mtkpdaf.Proc2Size))
		{
			m_pInterface->AddLog(_T("PDAF Data Read Fail!"), COLOR_RED);
			bRet &= FALSE;
			break;	
		}


		for (i = 0; i < m_mtkpdaf.Proc1Size; i++)
		{
			PdafBin[i] = output[i];
		}
		for(i = 0; i<m_mtkpdaf.Proc2Size; i++)
		{
			PdafBin[i+m_mtkpdaf.Proc1Size] = Proc2_Read[i];
		}

		for (i=0; i<m_mtkpdaf.TotalSize; i++)
		{
			PDAF_Data[i] = PdafBin[i];
		}
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("PDAF Ini Init Fail!"), COLOR_RED);
			bRet &= FALSE;	
			break;
		}
		errornum = pd_cali_verify(PDAF_Data);
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("PDAF Data Verify Fail!"), COLOR_RED);
			bRet &= FALSE;	
			break;
		}
		PDAFSaveBin(PdafBin, m_mtkpdaf.TotalSize);
	} while (0);

	SAFE_DELETE_ARRAY(output);
	SAFE_DELETE_ARRAY(Proc2_Read);
	SAFE_DELETE_ARRAY(PDAF_Data);
	SAFE_DELETE_ARRAY(PdafBin);
	if (!bRet)
	{
		SAFE_DELETE_ARRAY(m_proc1data);
	}
	return bRet;
}

void COtpBase::PDAFSaveProc1Bin(char* data, int size)
{
	CString str;
	str.Format(_T("Proc1 output bytes size %d"), size);
	m_pInterface->AddLog(str);

	// 存储bin文件
	CString info;
	CString Path;
// 	if (!m_QVLEn)
// 	{
	Path.Format(_T("PD_CALI_MTK_STEP1_%d.bin"),m_pInterface->GetModuleNum());
	Path = FileFolderPath+Path;
// 	}
// 	else
// 	{
// 		CString FileFolderPath1;
// 		str.Format(_T("bin\\"));//
// 		FileFolderPath1 = FileFolderPath + str;
// 		CreateDirectory(FileFolderPath1,NULL);
// 		Path.Format(_T("%d.bin"),m_pInterface->GetModuleNum());
// 		Path = FileFolderPath1 + Path;// str + _T("\\")+
// 	}

	if (PathFileExists(Path))
	{
		if(!DeleteFile(Path.GetBuffer(Path.GetLength())))
		{
			m_pInterface->AddLog(_T("原bin档删除失败!"),COLOR_RED);
			return ;
		}
	}

	FILE* file1;
	if (_wfopen_s(&file1, Path.GetBuffer() , _T("wb+")) == 0)
	{
		fwrite(data, 1, size, file1);
		fclose(file1);
		info.Format(_T("%s saved"),Path);
		m_pInterface->AddLog(info);
	}


	// 保存txt文本
	Path.Format(_T("PD_CALI_MTK_STEP1_%d.txt"),m_pInterface->GetModuleNum());
	Path = FileFolderPath+Path;
	if (PathFileExists(Path))
	{
		if(!DeleteFile(Path.GetBuffer(Path.GetLength())))
		{
			m_pInterface->AddLog(_T("原txt档删除失败!"),COLOR_RED);
			return ;
		}
	}

	CStringA temp, num;
	for (int i=0; i<size; i++)
	{
		num.Format("%02hhX\n", (unsigned char)data[i]);
		temp += num;
	}

	char* p = temp.GetBuffer();
	FILE* file2;
	if (_wfopen_s(&file2, Path.GetBuffer() , _T("wt+")) == 0)
	{
		fwrite(p, 1, strlen(p), file2);

		fclose(file2);
		info.Format(_T("%s saved"),Path);
		m_pInterface->AddLog(info);
	}

	return;
}

void COtpBase::PDAFSaveBin(char* data, int size)
{
	CString str;
	str.Format(_T("PDAF output bytes size %d"), size);
	m_pInterface->AddLog(str);

	// 存储bin文件
	CString info;
	CString Path;
	CString Path2;
	if (!m_EnQVL)
	{
		Path.Format(_T("PD_CALI_MTK_%d.bin"),m_pInterface->GetModuleNum());
		Path = FileFolderPath+Path;
	}
	else
	{
		CString FileFolderPath1; 
		str.Format(_T("bin\\"));// ../../bin/x.bin
		FileFolderPath1 = FileFolderPath + str;
		CreateDirectory(FileFolderPath1,NULL);
		str.Format(_T("%d.bin"),m_pInterface->GetModuleNum());
		Path = FileFolderPath1 + str;// str + _T("\\")+

		CString FileFolderPath2;// ../out/x.raw
		str.Format(_T("%d\\"),m_pInterface->GetModuleNum());
		FileFolderPath2 = FileFolderPath + str;
		CreateDirectory(FileFolderPath2,NULL);
		FileFolderPath2 = FileFolderPath2+_T("out\\");
		CreateDirectory(FileFolderPath2,NULL);
		str.Format(_T("%d.bin"),m_pInterface->GetModuleNum());
		Path2 = FileFolderPath2 + str;
	}

	if (PathFileExists(Path))
	{
		if(!DeleteFile(Path.GetBuffer(Path.GetLength())))
		{
			m_pInterface->AddLog(_T("原bin档删除失败!"),COLOR_RED);
			return ;
		}
	}

	FILE* file1;
	if (_wfopen_s(&file1, Path.GetBuffer() , _T("wb+")) == 0)
	{
		fwrite(data, 1, size, file1);
		fclose(file1);
		info.Format(_T("%s saved"),Path);
		m_pInterface->AddLog(info);
	}

	FILE* file2;
	if (_wfopen_s(&file2, Path2.GetBuffer() , _T("wb+")) == 0)
	{
		fwrite(data, 1, size, file2);
		fclose(file2);
		info.Format(_T("%s saved"),Path2);
		m_pInterface->AddLog(info);
	}
	return;
}

BOOL COtpBase::PDAF_Proc1(void)
{
	if (m_EnPDAF)
	{
		if (m_PDAFItem == 0)//MTK
		{
			if (!MTK_GetProc1()) 
			{
				return FALSE;
			}
		}
		//Qualcomm
		if (m_PDAFItem == 1)
		{
			if (!Qual_GainMapCal())
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL COtpBase::GetLSC(void)
{
	if (m_EnLSC)
	{
		if (m_LSCItem == 0)
		{
			if (!MTK_GetLSC())	return FALSE; 		//MTK
		}
		else if (m_LSCItem == 1)
		{
			if (!Qual_GetLSC())	return FALSE; 			//Qual
		}
		else if (m_LSCItem == 2)
		{
			if (m_sensoritem == 0)//3l8
			{
				if (!S5K3L8_GetLSC())	return FALSE;
			}
			if (m_sensoritem == 1)//5E8
			{
				if (!S5K5E8_GetLSC())	return FALSE;
			}

		}
	}

	return TRUE;
}

BOOL COtpBase::GetAWB(void)
{
	return TRUE;
}

BOOL COtpBase::CheckOTPAWB(void)
{
	return TRUE;
}

BOOL COtpBase::WriteOTP(void)
{
	return TRUE;
}


BOOL COtpBase::VerifyLSC(void)
{
	return TRUE;
}

BOOL COtpBase::VerifyPreWB(void)
{
	return TRUE;
}

BOOL COtpBase::VerifyAWB(void)
{
	return TRUE;
}

BOOL COtpBase::CheckOTP(void)
{
	return TRUE;
}

void COtpBase::SaveCurrentRaw10(CString filename, USHORT *buff, int width, int height)
{
	FILE *file;

		if (_wfopen_s(&file, filename.GetBuffer() , _T("wb+")) == 0)
		{
				int cnt = fwrite( (void*)buff, 2, width*height, file);	// 返回实际写入的数据项个数count
				fclose(file);
				CString str;
				if (cnt == width*height)
				{
					str = filename + _T(" saved");
					m_pInterface->AddLog(str);
				}
				else
				{
					str = filename + _T(" Written Error");
					m_pInterface->AddLog(str, COLOR_RED);
				}
		}
		else 
		{
			CString str = filename + _T(" Written Error");
			m_pInterface->AddLog(str, COLOR_RED);
		}
		return;
}

void COtpBase::SaveCurrentRaw8(CString filename, BYTE *buff, int width, int height)
{
	FILE *file;

	if (_wfopen_s(&file, filename.GetBuffer() , _T("wb+")) == 0)
	{
		int cnt = fwrite( (void*)buff, 1, width*height, file);	// 返回实际写入的数据项个数count
		fclose(file);
		CString str;
		if (cnt == width*height)
		{
			str = filename + _T(" saved");
			m_pInterface->AddLog(str);
		}
		else
		{
			str = filename + _T(" Written Error");
			m_pInterface->AddLog(str, COLOR_RED);
		}
	}
	else 
	{
		CString str = filename + _T(" Written Error");
		m_pInterface->AddLog(str, COLOR_RED);
	}

	return;
}

BOOL COtpBase::StartProgram()
{

	return TRUE;
}

void COtpBase::SetWBDefaultGain(void)					 
{
	return;	
}

void COtpBase::DisableLenc()
{
	return;
}

void COtpBase::WriteWBReg(USHORT addrHigh, USHORT addrLow, USHORT gain)
{
	//HIBYTE
	if ((addrHigh != REG_INVALID) && (addrLow != REG_INVALID))
	{
		i2c_write_byte(addrHigh, HIBYTE(gain));
		i2c_write_byte(addrLow,  LOBYTE(gain));
	}
	else if (addrHigh != REG_INVALID)
	{
		i2c_write(addrHigh, gain);
	}
	else if (addrLow != REG_INVALID)
	{
		i2c_write(addrLow, gain);
	}
	
}

BOOL COtpBase::InitAWB(void)
{
	memset(&m_wbCurrent, 0, sizeof(m_wbCurrent));

	ImageInfo imgInfo = m_pInterface->GetImage();//此处仅用于获取图像长宽

	if (m_wbCtrl.roiCalcMode != ROI_CALC_USER)
	{
		SetRoi(m_wbCtrl.roi, imgInfo.width, imgInfo.height, m_wbCtrl.roiCalcMode);
	}
	FormatRoiRect(m_wbCtrl.roi, imgInfo.width, imgInfo.height);

	return TRUE;
}


BOOL COtpBase::GetAWBInfo(tagAwbRatio* wb)
{
	int SENSOROB = 64;
	int AWB_FACTOR = 1024;
	// getwb
	ImageInfo imgInfo = m_pInterface->GetImage();
	//建议此处保存Applywb前的bmp图片
	int width = imgInfo.width;
	int height= imgInfo.height;

	wb->r = ImageProc::GetAveR_ROI_Raw(imgInfo.Raw_Buf, imgInfo.width, imgInfo.height, m_wbCtrl.roi, imgInfo.RawFormat);
	wb->g = ImageProc::GetAveG_ROI_Raw(imgInfo.Raw_Buf, imgInfo.width, imgInfo.height, m_wbCtrl.roi, imgInfo.RawFormat);
	wb->b = ImageProc::GetAveB_ROI_Raw(imgInfo.Raw_Buf, imgInfo.width, imgInfo.height, m_wbCtrl.roi, imgInfo.RawFormat);
	wb->gb = ImageProc::GetAveGb_ROI_Raw(imgInfo.Raw_Buf, imgInfo.width, imgInfo.height, m_wbCtrl.roi, imgInfo.RawFormat);
	wb->gr = ImageProc::GetAveGr_ROI_Raw(imgInfo.Raw_Buf, imgInfo.width, imgInfo.height, m_wbCtrl.roi, imgInfo.RawFormat);

	wb->rg		= (UINT)(1.0 * (wb->r-SENSOROB) / (wb->g-SENSOROB)*AWB_FACTOR + 0.5); 
	wb->bg		= (UINT)(1.0 * (wb->b-SENSOROB) / (wb->g-SENSOROB)*AWB_FACTOR + 0.5);
	wb->gb_gr	= (UINT)(1.0 * (wb->gb-SENSOROB)/ (wb->gr-SENSOROB)*AWB_FACTOR+ 0.5);

	m_szStr.Format(_T("R/G: %d B/G: %d"), wb->rg, wb->bg);
	m_pInterface->AddLog(m_szStr, COLOR_BLUE);
	return TRUE;
}

BOOL COtpBase::Qual_GetAWBInfo(tagAwbRatio* wb)
{
	int SENSOROB = 64;
	int AWB_FACTOR = 1024;
	// getwb //Qual LSC自减OB
	wb->rg		= (UINT)(1.0 * Qual_wb_r / Qual_wb_gr*AWB_FACTOR + 0.5); 
	wb->bg		= (UINT)(1.0 * Qual_wb_b / Qual_wb_gr*AWB_FACTOR + 0.5);
	wb->gb_gr	= (UINT)(1.0 * Qual_wb_gb/ Qual_wb_gr*AWB_FACTOR+ 0.5);

	m_szStr.Format(_T("R/G: %d B/G: %d"), wb->rg, wb->bg);
	m_pInterface->AddLog(m_szStr, COLOR_BLUE);
	return TRUE;
}
void COtpBase::SetRoi(CRect &roi, UINT width, UINT height, BYTE roiCalcMode, UINT x/*=0*/, UINT y/*=0*/, UINT roiWidth/*=0*/, UINT roiHeight/*=0*/)
{
	if (roiCalcMode == ROI_CALC_USER)
	{
		roi.left = x;
		roi.top  = y;
		roi.right  = x + roiWidth;
		roi.bottom = y + roiHeight;

		FormatRoiRect(roi, width, height);
		return;
	}

	switch (roiCalcMode)
	{
	case ROI_CALC_CENTER:
		roiWidth  = width;
		roiHeight = 2;
		break;

	case ROI_CALC_1:		
	case ROI_CALC_2:	
	case ROI_CALC_4:		
	case ROI_CALC_5:		
		roiWidth  = width  / roiCalcMode;     
		roiHeight = height / roiCalcMode;     
		break;

	case ROI_CALC_8:		
		roiWidth  = width  / roiCalcMode;     
		roiHeight = height / roiCalcMode;     
		break;

	case ROI_CALC_10:		
		roiWidth  = width  / roiCalcMode;     
		roiHeight = height / roiCalcMode;     
		break;

	default:
		break;
	}
	roi.left = (width - roiWidth) / 2;
	roi.top  = (height - roiHeight) / 2;
	roi.right = roi.left + roiWidth;
	roi.bottom = roi.top + roiHeight;

	FormatRoiRect(roi, width, height);
}


void COtpBase::FormatRoiRect(CRect &rt, UINT width, UINT height)
{
	if (rt.left > (int)width)
	{
		rt.left = width - 2;
	}

	if (rt.top > (int)height)
	{
		rt.top = height - 2;
	}

	if (rt.left + rt.Width() > (int)width)
	{
		rt.right = width - 1;
	}

	if (rt.top + rt.Height() > (int)height)
	{
		rt.bottom = height - 1;
	}

	if ((rt.left % 2) != 0)
	{
		rt.left--;
	}

	if ((rt.top % 2) != 0)
	{
		rt.top--;
	}

	if ((rt.Width() % 2) != 0)
	{
		rt.right--;
	}

	if ((rt.Height() % 2) != 0)
	{
		rt.bottom--;
	}	
}

BOOL COtpBase::CheckEqual(BYTE *data1, BYTE *data2, int num)
{
	CString info;
	if (data1 == NULL || data2 == NULL)
	{
		return FALSE;
	}

	if (num <= 0)
	{
		return FALSE;
	}	

	for (int i=0; i<num; i++)
	{
		if (data1[i] != data2[i])
		{
			info.Format(_T("Data %d Check Error! 0x%x != 0x%x"),i,data1[i],data2[i]);
			m_pInterface->AddLog(info, COLOR_RED);
			return FALSE;
		}
	}

	return TRUE;
}

VOID COtpBase::Raw10toRaw8(USHORT *Src, BYTE *Dest, long number)//Raw10
{
	long DesCount = 0;

	for (long i = 0; i < number; i++ )//i = i + 5
	{
		    Dest[i] = ((Src[i]>>2) & 0xff);
	}
}

CString COtpBase::GetModulePath()
{
	int  i    = 0;
	int  len  = 0;
	bool flag = false;
	CString myStr;
	TCHAR ModulePath[MAX_PATH] = {0};
	char path[512] = {0};
	GetModuleFileNameW(NULL, ModulePath, MAX_PATH);//return  real  lenghth
	PathRemoveFileSpec(ModulePath);
	myStr.Format(ModulePath);
	return myStr;
}

BOOL COtpBase::MTK_GetLSC(void)
{
	m_pInterface->AddLog(_T("Get LSC Data Start！"));
	CString sfilename = GetModulePath()+"\\LSC\\MTK\\Capture_4208x3120.raw";

#ifndef  TestVersion
	if (PathFileExists(sfilename))
	{
		if (!DeleteFile(sfilename))
		{
			m_pInterface->AddLog(_T("无法删除原Raw图!"), COLOR_RED);
			return FALSE;
		}
	}
#endif

	int width = m_pInterface->GetImage().width;
	int height = m_pInterface->GetImage().height;

	BYTE *raw8buff = new BYTE [width*height];
	memset(raw8buff, 0, width*height);

	Raw10toRaw8(m_pInterface->GetImage().Raw_Buf,raw8buff,width*height);

#ifndef  TestVersion
	SaveCurrentRaw8(sfilename,raw8buff,width, height);//保存Raw8图
#endif

	SAFE_DELETE_ARRAY(raw8buff);

	m_mtklscdata = new BYTE[SIZE_MTK_LSC];
	memset(m_mtklscdata, 0 ,sizeof(m_mtklscdata));
	CString m_LscDirectory=GetModulePath()+"\\LSC\\MTK";
	info.Format(_T("Taskkill /IM %s\\1_ShadingCalibrationSample.exe"),m_LscDirectory);
	BOOL result = TRUE;
	do 
	{
		if (MTK_GetLSCData())
		{
			m_pInterface->AddLog(_T("Get MTK LSC Success!"), COLOR_BLUE);
			WinExec(CT2A(info), SW_HIDE);
		}
		else
		{
			WinExec(CT2A(info), SW_HIDE);
			m_pInterface->AddLog(_T("Get MTK LSC Fail!"), COLOR_RED);
			result &= FALSE;
			break;
		}

		info.Format(_T("Taskkill /IM %s\\2_ShadingCorrectionSample.exe"),m_LscDirectory);
		if (MTK_ApplyLSC())
		{
			m_pInterface->AddLog(_T("Apply LSC Pass!"), COLOR_BLUE);
			WinExec(CT2A(info), SW_HIDE);
		}
		else
		{
			WinExec(CT2A(info), SW_HIDE);
			m_pInterface->AddLog(_T("Apply LSC Fail!"), COLOR_RED);
			result &= FALSE;
			break;
		}
	} while (0);

	if (!result)
	{
		SAFE_DELETE_ARRAY(m_mtklscdata);
	}
	return result;
}

BOOL COtpBase::MTK_GetLSCData(void)
{
	CString m_LscDirectory=GetModulePath()+"\\LSC\\MTK";
	SetCurrentDirectory(m_LscDirectory);

	CString capture_Image=m_LscDirectory+"\\"+"Capture_4208x3120.raw";
	CString slim_param_capture=m_LscDirectory+"\\"+"slim_param_capture.txt";
	CString ShadingCalibration_Slim=m_LscDirectory+"\\"+"1_ShadingCalibrationSample.exe";
	CString ShadingCalibrate=m_LscDirectory+"\\"+"ShadingCalibrate.dll";
	CString eeprom_slim=m_LscDirectory+"\\"+"eeprom_slim.txt";

	int Timeout=100;
	HANDLE hFile;
	if (!PathFileExists(capture_Image))
	{
		m_pInterface->AddLog(_T("Raw图不存在!"),COLOR_RED);
		return FALSE;
	}
	if(!PathFileExists(ShadingCalibrate))
	{
		m_pInterface->AddLog(_T("ShadingCalibrate.dll文件不存在!"),COLOR_RED);
		return FALSE;
	}
	if(!PathFileExists(ShadingCalibration_Slim))
	{
		m_pInterface->AddLog(_T("1_ShadingCalibrationSample.exe文件不存在!"),COLOR_RED);
		return FALSE;
	}
	if(!PathFileExists(slim_param_capture))
	{
		m_pInterface->AddLog(_T("slim_param_capture.txt文件不存在!"),COLOR_RED);
		return FALSE;
	}
	if (PathFileExists(eeprom_slim))
	{
		if (!DeleteFile(eeprom_slim))
		{
			m_pInterface->AddLog(_T("无法删除旧eeprom_slim文件!"),COLOR_RED);
			return FALSE;			
		}
	}
	WinExec(CT2A(ShadingCalibration_Slim),SW_HIDE);
	while (Timeout!=0)
	{
		hFile = CreateFile(eeprom_slim,GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
		if( hFile != INVALID_HANDLE_VALUE )
		{
			DWORD nBytesRead;
			unsigned char BIN[2000] = {0};
			ReadFile(hFile, BIN, 2000, &nBytesRead, NULL );
			CloseHandle( hFile );

			if (nBytesRead != SIZE_MTK_LSC)
			{
				m_pInterface->AddLog(_T("eeprom_slim大小不为1868 Byte!"),COLOR_RED);
				return FALSE;
			}

			for (int i = 0;i < SIZE_MTK_LSC; i++)
			{
				m_mtklscsum += m_mtklscdata[i] = BIN[i];
//				m_mtklscsum += m_mtklscdata[i];
//				m_pInterface->AddDebugLog(_T("Output LSCData----->%d=0x%02X"),i,LSCData[i]);
			}
			break;
		}
		Sleep(100);
		Timeout--;
	}

	return TRUE;
}

BOOL COtpBase::MTK_ApplyLSC()
{
	CString m_LscDirectory=GetModulePath()+"\\LSC\\MTK";
	SetCurrentDirectory(m_LscDirectory);
	CString eeprom_slim=m_LscDirectory+"\\"+"eeprom_slim.txt";
	CString after_ob=m_LscDirectory+"\\"+"after_ob.raw";
	CString ShadingCorrectionSample=m_LscDirectory+"\\"+"2_ShadingCorrectionSample.exe";
	CString resultFile=m_LscDirectory+"\\"+"result.txt";
	CString lsv_param_capture=m_LscDirectory+"\\"+"lsv_param_capture.txt";

	if(!PathFileExists(ShadingCorrectionSample))
	{
		m_pInterface->AddLog(_T("2_ShadingCorrectionSample.exe文件不存在!"),COLOR_RED);
		return FALSE;
	}
	if(PathFileExists(resultFile))
	{
		if(!DeleteFile(resultFile))
		{
			m_pInterface->AddLog(_T("无法删除旧result.txt文件!"),COLOR_RED);
			return FALSE;
		}
	}
	if(!PathFileExists(lsv_param_capture))
	{
		m_pInterface->AddLog(_T("lsv_param_capture.txt文件不存在!!"),COLOR_RED);
		return FALSE;
	}
	if(!PathFileExists(eeprom_slim))
	{
		m_pInterface->AddLog(_T("eeprom_slim.txt文件不存在!"),COLOR_RED);
		return FALSE;
	}
	if(!PathFileExists(after_ob))
	{
		m_pInterface->AddLog(_T("after_ob.raw文件不存在!"),COLOR_RED);
		return FALSE;
	}
	WinExec(CT2A(ShadingCorrectionSample),SW_HIDE);
	CString FileContent;

	int Timeout=100;
	CString line = _T("");
	CStdioFile file;
	while (Timeout!=0)
	{
		if(!file.Open(resultFile, CFile::modeRead | CFile::typeText))
		{
			Sleep(50);
			Timeout--;
		}
		else
		{
			break;
		}

		if (Timeout == 0)
		{
			m_pInterface->AddLog(_T("打开result.txt失败!"),COLOR_RED);
			return FALSE;
		}
	}
	file.ReadString(line);
	while (file.ReadString(line))
	{
		if (line.Left(4) != "pass")
		{
			file.Close();
			m_pInterface->AddLog(_T("LSC result fail!"),COLOR_RED);
			return 0;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////Qual///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void COtpBase::InitQualParam()
{
	//Qual LSC
	Qual_LSC_R_Max = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_R_Max"),102);
	Qual_LSC_R_Min = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_R_Min"),0);
	Qual_LSC_Gr_Max = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_Gr_Max"),102);
	Qual_LSC_Gr_Min = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_Gr_Min"),0);
	Qual_LSC_Gb_Max = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_Gb_Max"),102);
	Qual_LSC_Gb_Min = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_Gb_Min"),0);
	Qual_LSC_B_Max = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_B_Max"),102);
	Qual_LSC_B_Min = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_B_Min"),0);
	Qual_LSCDistanceSpec = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSCDistanceSpec"),60);

	//Qual PDAF
	memset(Qual_GainMapData,0,sizeof(Qual_GainMapData));       
	MinPixelAvg = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("MinPixelAvg"),64);
	MaxPixelAvg = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("MaxPixelAvg"),102);
	MinPDAvg = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("MinPDAvg"),64);
	MaxPDAvg = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("MaxPDAvg"),102);
	MinGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("MinGain"),64);
	MaxGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("MaxGain"),1024);
	MaxPixelThresh = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("MaxPixelThresh"),256);
	GainDiff = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("GainDiff"),256);

	return;
}

void COtpBase::SaveQualParam()
{
	//Qual LSC
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("Qual_LSC_R_Max      "),Qual_LSC_R_Max);
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("Qual_LSC_R_Min      "),Qual_LSC_R_Min);
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("Qual_LSC_Gr_Max      "),Qual_LSC_Gr_Max);
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("Qual_LSC_Gr_Min      "),Qual_LSC_Gr_Min);
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("Qual_LSC_Gb_Max      "),Qual_LSC_Gb_Max);
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("Qual_LSC_Gb_Min      "),Qual_LSC_Gb_Min);
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("Qual_LSC_B_Max      "),Qual_LSC_B_Max);
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("Qual_LSC_B_Min      "),Qual_LSC_B_Min);
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("Qual_LSCDistanceSpec      "),Qual_LSCDistanceSpec);
	
	//Qual PDAF
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("MinPixelAvg      "),MinPixelAvg);
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("MaxPixelAvg      "),MaxPixelAvg);
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("MinPDAvg      "),MinPDAvg);
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("MaxPDAvg      "),MaxPDAvg);
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("MinGain      "),MinGain);
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("MaxGain      "),MaxGain);
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("MaxPixelThresh      "),MaxPixelThresh);
	m_pInterface->GetConfigManager()->WriteConfigInt(L"OTP",_T("GainDiff      "),GainDiff);

	return;
}

BOOL COtpBase::Qual_GetLSC()
{
	m_pInterface->AddLog(_T("Get Qual LSC..."));
	typedef int (*lpFun)(unsigned char * pImage, int nWidth, int nHeight,                                                              
		int CFA_pattern, short R_black_offset, short Gr_black_offset,                                                                    
		short Gb_black_offset, short B_black_offset, BOOL bMode9x7);    

	CString WorkingPath = GetModulePath()+"\\LSC";
	CreateDirectory(WorkingPath,NULL);
	WorkingPath += "\\Qual";
	CreateDirectory(WorkingPath,NULL);
	SetCurrentDirectory(WorkingPath);
	WorkingPath += "\\LSCCalibrationDll.dll";
	HINSTANCE hQualLSCDll = LoadLibrary(WorkingPath);

	BOOL bRet = TRUE;
	do 
	{
		if (NULL == hQualLSCDll)
		{
			info.Format(_T("找不到LSCCalibrationDll.dll，请将此Dll放入程式目录:\n%s！"),WorkingPath);
			m_pInterface->AddLog(info,COLOR_RED);
			bRet = FALSE;
			break;
		}
		lpFun LensCorrection = (lpFun)GetProcAddress(hQualLSCDll,"LensCorrectionLib");
		if (!LensCorrection)
		{
			m_pInterface->AddLog(_T("找不到入口函数LensCorrectionLib！"),COLOR_RED);
			bRet = FALSE;
			break;
		}


		ImageInfo lscimageinfo = m_pInterface->GetImage();
		int size = lscimageinfo.width*lscimageinfo.height;
		BYTE* lscbuff = new BYTE[size];
		Raw10toRaw8(lscimageinfo.Raw_Buf,lscbuff,size);
		LensCorrection(lscbuff,lscimageinfo.width,lscimageinfo.height,m_cfaitem,m_otpCtrl.sensorob/4,m_otpCtrl.sensorob/4,m_otpCtrl.sensorob/4,m_otpCtrl.sensorob/4,m_quallscitem);


		//////////////////////////////////Read LSC//////////////////////////////////
		float R_Channel[221] = {0};
		float Gr_Channel[221] = {0};
		float Gb_channel[221] = {0};
		float B_Channel[221] = {0};
		int R_Hex,Gr_Hex,Gb_Hex,B_Hex;
		if (!ReadQualLSCDataFromTXT(R_Channel,Gr_Channel,Gb_channel,B_Channel))    return FALSE;

		int i = 0;
		if (m_otpCtrl.quallsccompress)
		{
			m_quallscdata = new BYTE[1105];//17x13x5
			for (i = 0; i<221; i++)
			{
				R_Hex = (int)(R_Channel[i] * 4 +0.5);
				Gr_Hex = (int)(Gr_Channel[i] * 4 +0.5);
				Gb_Hex = (int)(Gb_channel[i] * 4 +0.5);
				B_Hex = (int)(B_Channel[i] * 4 +0.5);
				m_quallscdata[5*i + 0] = R_Hex&0xff;
				m_quallscdata[5*i + 1] = Gr_Hex&0xff;
				m_quallscdata[5*i + 2] = Gb_Hex&0xff;
				m_quallscdata[5*i + 3] = B_Hex&0xff;
				m_quallscdata[5*i + 4] = ((R_Hex>>2)&0xc0) + ((Gr_Hex>>4)&0x30) + ((Gb_Hex>>6)&0x0c) + ((B_Hex>>8)&0x0);	
			}
			for (i=0; i<1105; i++)
			{
				m_quallscsum += m_quallscdata[i];
			}
		}
		else
		{
			m_quallscdata = new BYTE[1768];//17x13x8
			for (i=0; i<221; i++)
			{
				R_Hex = (int)(R_Channel[i] * 4 +0.5);
				Gr_Hex = (int)(Gr_Channel[i] * 4 +0.5);
				Gb_Hex = (int)(Gb_channel[i] * 4 +0.5);
				B_Hex = (int)(B_Channel[i] * 4 +0.5);
				m_quallscdata[8*i + 0] = (R_Hex>>8)&0x0f;
				m_quallscdata[8*i + 1] = R_Hex&0xff;
				m_quallscdata[8*i + 2] = (Gr_Hex>>8)&0x0f;
				m_quallscdata[8*i + 3] = Gr_Hex&0xff;
				m_quallscdata[8*i + 4] = (Gb_Hex>>8)&0x0f;
				m_quallscdata[8*i + 5] = Gb_Hex&0xff;
				m_quallscdata[8*i + 6] = (B_Hex>>8)&0x0f;
				m_quallscdata[8*i + 7] = B_Hex&0xff;
			}
			for (i=0; i<1768; i++)
			{
				m_quallscsum += m_quallscdata[i];
			}
		}

	} while (0);

	FreeLibrary(hQualLSCDll);

	if (!bRet)	return bRet;

	if (!QualLSCDataCheck(m_quallscdata))
	{
		m_pInterface->AddLog(_T("LSC烧录环境不良！"),COLOR_RED);
		return FALSE;
	}
	m_pInterface->AddLog(_T("Get Qual LSC success"),COLOR_BLUE);
	return TRUE;
}

BOOL COtpBase::ReadQualLSCDataFromTXT(float *R_Channel, float *Gr_Channel, float *Gb_channel, float *B_Channel)
{
	int mycharlength = 0;
	int strlength = 0;
	char mycharbuff[512] = {0};
	CStdioFile file;
	CString Path = GetModulePath();
	CString str;
	str.Format(_T("\\LSC\\Qual\\AWB_LSC_CALIBRATION_DATA.txt"));
	Path = Path + str;

	int r_count =0;
	int gr_count = 0;
	int gb_count = 0;
	int b_count = 0;
	int channel_count = 0;
	int end_count =0;
	float tempnum;
	int i;
	int q = 0;

	if(file.Open(Path , CFile::modeRead | CFile::typeText ))
	{
		CString strTemp;
		CString a;
		while(file.ReadString(strTemp))
		{
			if (strTemp.Find(_T("}"),0) != -1)
			{
				end_count++;
				continue;
			}
			if (end_count >= 4)
			{
				break;
			}
			if ( strTemp.Find( _T("{"), 0 ) != -1 )
			{	
				channel_count++;
				if ( channel_count == 1 )
				{
					strTemp = strTemp.Mid(38,strTemp.GetLength()-38);
				}
				if ( (channel_count == 2) || ( channel_count == 3 ) )
				{
					strTemp = strTemp.Mid(14,strTemp.GetLength()-14);
				}
				if ( channel_count == 4 )
				{
					strTemp = strTemp.Mid(13,strTemp.GetLength()-1);
				}

				int num = 0;
				if (m_quallscitem == 0)
				{
					num = 17;
				}
				else if (m_quallscitem == 1)
				{
					num = 9;
				}
				for (i = 0;i < num; i++)
				{
					switch (channel_count)
					{
					case 1:
						//						a = strTemp.Mid(38+i*11,9);	
						q = 0;
						while ( (strTemp[q] != ' ')&&(q<13) )
						{
							mycharlength++;
							q++;
						}
						a = strTemp.Mid(0,mycharlength);
						strlength = strTemp.GetLength();
						if ( (strlength-mycharlength) != 1 )
						{
							strTemp = strTemp.Mid(mycharlength+2,strlength-(mycharlength+2));
						}
						else
						{
							a = strTemp.Mid(0,strlength-1);
						}
						tempnum = (float)atof(CT2A(a));
						mycharlength = 0;
						R_Channel[r_count++] = tempnum;
						break;
					case 2:
						//						a = strTemp.Mid(14+i*11,9);
						q = 0;
						while ( (strTemp[q] != ' ')&&(q<13) )
						{
							mycharlength++;
							q++;
						}
						a = strTemp.Mid(0,mycharlength);
						strlength = strTemp.GetLength();
						if ( (strlength-mycharlength) != 1 )
						{
							strTemp = strTemp.Mid(mycharlength+2,strlength-(mycharlength+2));
						}
						else
						{
							a = strTemp.Mid(0,strlength-1);
						}
						tempnum = (float)atof(CT2A(a));
						mycharlength = 0;
						Gr_Channel[gr_count++] = tempnum;
						break;
					case 3:
						//						a = strTemp.Mid(14+i*11,9);
						q = 0;
						while ( (strTemp[q] != ' ')&&(q<13) )
						{
							mycharlength++;
							q++;
						}
						a = strTemp.Mid(0,mycharlength);
						strlength = strTemp.GetLength();
						if ( (strlength-mycharlength) != 1 )
						{
							strTemp = strTemp.Mid(mycharlength+2,strlength-(mycharlength+2));
						}
						else
						{
							a = strTemp.Mid(0,strlength-1);
						}
						tempnum = (float)atof(CT2A(a));
						mycharlength = 0;
						Gb_channel[gb_count++] = tempnum;
						break;
					case 4:
						//						a = strTemp.Mid(13+i*11,9);
						q = 0;
						while ( (strTemp[q] != ' ')&&(q<13) )
						{
							mycharlength++;
							q++;
						}
						a = strTemp.Mid(0,mycharlength);
						strlength = strTemp.GetLength();
						if ( (strlength-mycharlength) != 1 )
						{
							strTemp = strTemp.Mid(mycharlength+2,strlength-(mycharlength+2));
						}
						else
						{
							a = strTemp.Mid(0,strlength-1);
						}
						tempnum = (float)atof(CT2A(a));
						mycharlength = 0;
						B_Channel[b_count++] = tempnum;
						break;
					}
				}
			}
			else
			{
				strTemp = strTemp.Mid(1,strTemp.GetLength()-1);
				for (i = 0;i < 17; i++)
				{
					//					strTemp.TrimLeft(1);
					int k = 0;
					//					a = strTemp.Mid(1+i*11,9);
					while ( (strTemp[k] != ' ')&&(k<13) )
					{
						mycharlength++;
						k++;
					}
					a = strTemp.Mid(0,mycharlength);
					strlength = strTemp.GetLength();
					if ( (strlength-mycharlength) != 1 )
					{
						strTemp = strTemp.Mid(mycharlength+2,strlength-(mycharlength+2));
					}
					else
					{
						a = strTemp.Mid(0,strlength-1);
					}


					tempnum = (float)atof(CT2A(a));
					mycharlength = 0;
					switch (channel_count)
					{
					case 1:
						R_Channel[r_count++] = tempnum;
						break;
					case 2:
						Gr_Channel[gr_count++] = tempnum;
						break;
					case 3:
						Gb_channel[gb_count++] = tempnum;
						break;
					case 4:
						B_Channel[b_count++] = tempnum;
						break;
					}
				}
			}
		}
		file.Close();
	}
	else
	{
		info.Format(_T("找不到文件%s!"),Path);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL COtpBase::QualLSCDataCheck(BYTE *data)
{
	FILE *fp = NULL;

	CString sfilename;
	CString sTmp;
	CString sfileext;
	CString sfilter = _T("Ini File(*.txt)|*.txt");

	CFileFind ff;

	CString   SensorINI = GetModulePath() + "\\LSC\\Qual\\Standard.ini";  // 设定参数文档路径

	sfilename.Format(SensorINI);

	if (_wfopen_s(&fp,sfilename,_T("a")))
	{
		info.Format(_T("找不到文件%s!"),sfilename);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}
	fclose(fp);

	CStdioFile MyFile(sfilename, CFile::modeRead | CFile::typeText);  

	int StandardBuf[17*13][4] = {0};
	int i = 0;
	for(i = 0; i < 221; i++ )
	{
		MyFile.ReadString( sTmp);
		StandardBuf[i][0]=atoi(CT2A(sTmp));

		MyFile.ReadString( sTmp);
		StandardBuf[i][1]=atoi(CT2A(sTmp));

		MyFile.ReadString( sTmp);
		StandardBuf[i][2]=atoi(CT2A(sTmp));
		MyFile.ReadString( sTmp);
		StandardBuf[i][3]=atoi(CT2A(sTmp));
	}

	MyFile.Close();


	int tempBuf[17*13][4] = {0};

	int cnt= 0;
	int LSCData_R_Max,LSCData_Gr_Max,LSCData_Gb_Max,LSCData_B_Max;
	LSCData_R_Max=LSCData_Gr_Max=LSCData_Gb_Max=LSCData_B_Max=0;
	int LSCData_R_Min,LSCData_Gr_Min,LSCData_Gb_Min,LSCData_B_Min;
	LSCData_R_Min=LSCData_Gr_Min=LSCData_Gb_Min=LSCData_B_Min=1023;

	if (m_otpCtrl.quallsccompress)
	{
		for(  i = 0; i < 221; i++ )
		{
			tempBuf[i][0] = data[cnt]+ (data[cnt+4]>>6)*256;
			tempBuf[i][1] = data[cnt+1] + ( (data[cnt+4]>>4)&0x03)*256;
			tempBuf[i][2] = data[cnt+2] + ( (data[cnt+4]>>2)&0x03)*256;
			tempBuf[i][3] = data[cnt+3] + (data[cnt+4]&0x03)*256;
			cnt = cnt+5; 
		}
	}
	else
	{
		for(  i = 0; i < 221; i++ )
		{
			tempBuf[i][0] = data[cnt]<<8 | data[cnt+1];
			tempBuf[i][1] = data[cnt+2]<<8 | data[cnt+3];
			tempBuf[i][2] = data[cnt+4]<<8 | data[cnt+5];
			tempBuf[i][3] = data[cnt+6]<<8 | data[cnt+7];
			cnt = cnt+8; 
		}
	}

	for (i=0; i<221; i++)
	{
		if (tempBuf[i][0]<LSCData_R_Min)
		{
			LSCData_R_Min=tempBuf[i][0];
		}
		if (tempBuf[i][0]>LSCData_R_Max)
		{
			LSCData_R_Max=tempBuf[i][0];
		}
		if (tempBuf[i][1]<LSCData_Gr_Min)
		{
			LSCData_Gr_Min=tempBuf[i][1];
		}
		if (tempBuf[i][1]>LSCData_Gr_Max)
		{
			LSCData_Gr_Max=tempBuf[i][1];
		}
		if (tempBuf[i][2]<LSCData_Gb_Min)
		{
			LSCData_Gb_Min=tempBuf[i][2];
		}
		if (tempBuf[i][2]>LSCData_Gb_Max)
		{
			LSCData_Gb_Max=tempBuf[i][2];
		}
		if(tempBuf[i][3]<LSCData_B_Min)
		{
			LSCData_B_Min=tempBuf[i][3];
		}
		if (tempBuf[i][3]>LSCData_B_Max)
		{
			LSCData_B_Max=tempBuf[i][3];
		}
	}

	///////////////Save the Data///////////
	FILE  *fpn  = NULL;
	CString FileName;

	FileName.Format(_T("\\LSC\\Qual\\LSCData.txt"));
//	int SaveLSCData=GetPrivateProfileInt("NORMAL","SaveLSCData",1, m_szSetup);
// 	if(SaveLSCData)
// 		FileName = GetModulePath() +FuseID+FileName;
// 	else 
		FileName = GetModulePath() +FileName;

	_wfreopen_s(&fpn,FileName,_T("at"),stdout);
	if(fpn==NULL)  
	{
		//	AfxMessageBox("open file fail!");
		return 0;
	}

	int tempMax = 0;
	int tempDistance = 0;

	for( i = 0; i < 17*13; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			tempDistance = abs(tempBuf[i][j] - StandardBuf[i][j]);

			if( tempDistance > tempMax )
			{
				tempMax = tempDistance;
			}		 

		}

	}
	for(  i = 0; i < 221; i++ )
	{
		printf("%d\n",tempBuf[i][0]);
		printf("%d\n",tempBuf[i][1]);
		printf("%d\n",tempBuf[i][2]);
		printf("%d\n",tempBuf[i][3]);
	}
	printf("R 最小为:%d R 最大为:%d",LSCData_R_Min,LSCData_R_Max);
	printf("\n");
	printf("Gr最小为:%d Gr最大为:%d",LSCData_Gr_Min,LSCData_Gr_Max);
	printf("\n");
	printf("Gb最小为:%d Gb最大为:%d",LSCData_Gb_Min,LSCData_Gb_Max);
	printf("\n");
	printf("B 最小为:%d B 最大为:%d",LSCData_B_Min,LSCData_B_Max);
	printf("\n");
	printf("LSCDistanceSpec为：%d",tempMax);
	printf("\n\n");
	fclose(fpn); 

	/////////////////////////////////////
	if (LSCData_R_Max>Qual_LSC_R_Max||LSCData_R_Min<Qual_LSC_R_Min||LSCData_Gr_Max>Qual_LSC_Gr_Max||LSCData_Gr_Min<Qual_LSC_Gr_Min
		||LSCData_Gb_Max>Qual_LSC_Gb_Max||LSCData_Gb_Min<Qual_LSC_Gb_Min||LSCData_B_Max>Qual_LSC_B_Max||LSCData_B_Min<Qual_LSC_B_Min)
	{	
		return FALSE;
	}

	if( tempMax > Qual_LSCDistanceSpec )
	{		
		return FALSE;
	} 

	return TRUE;
}


BOOL COtpBase::Qual_GetAWBData()
{
	CStdioFile file;
	CString Path = GetModulePath();
	Path +=  "\\LSC\\Qual\\AWB_LSC_CALIBRATION_DATA.txt";

	if(file.Open(Path , CFile::modeRead | CFile::typeText ))
	{
		CString strTemp;
		CString a;
		while(file.ReadString(strTemp))
		{
			if (strTemp.Find(_T("r_oc_ave"),0) == -1)
			{
				continue;
			}
			else
			{
				double temp=0.00;
				a = strTemp.Mid(10,5);
				temp=atof(CT2A(a.GetBuffer(a.GetLength())));
				Qual_wb_r=(BYTE)(temp+0.5); //R


				file.ReadString(strTemp);
				a = strTemp.Mid(10,5);
				temp=atof(CT2A(a.GetBuffer(a.GetLength())));
				Qual_wb_b=(BYTE)(temp+0.5); //B

				file.ReadString(strTemp);
				a = strTemp.Mid(11,5);
				temp=atof(CT2A(a.GetBuffer(a.GetLength())));
				Qual_wb_gr=(BYTE)(temp+0.5); //Gr

				file.ReadString(strTemp);
				a = strTemp.Mid(11,5);
				temp=atof(CT2A(a.GetBuffer(a.GetLength())));
				Qual_wb_gb=(BYTE)(temp+0.5); //Gb
				break;

			}	
		}
		file.Close();
	}
	else 
	{
		m_pInterface->AddLog(_T("打开AWB_LSC_CALIBRATION_DATA.txt失败!"),COLOR_RED);
		return FALSE;
	}
	return TRUE;
}

BOOL COtpBase::Qual_GainMapCal()
{
	CString FileFolderPath = GetModulePath()+"\\PDAF";
	CreateDirectory(FileFolderPath, NULL);
	FileFolderPath += "\\Qual";
	CreateDirectory(FileFolderPath, NULL);
	SetCurrentDirectory(FileFolderPath);
	CString Rawfilename =  FileFolderPath + "\\Qual_Step1.raw";
	if (PathFileExists(Rawfilename)) 
	{
		if (!DeleteFile(Rawfilename))
		{
			m_pInterface->AddLog(_T("删除原Raw图失败!"), COLOR_RED);
			return FALSE;
		}
	}
	ImageInfo imgInfo = m_pInterface->GetImage();
	int width = imgInfo.width;
	int height= imgInfo.height;
	SaveCurrentRaw10(Rawfilename,imgInfo.Raw_Buf,width,height);

	FILE *fp;
	int i = 0;
	short *GmCalibImage;
	PDAFPattern PDAFPatternValue;
	GainMap1DDataStruct GainMapData1D;
	GainMap2DDataStruct GainMapData2D;
	char CurImgFilename[MAX_PATH];
	char GainMapFilename[MAX_PATH];
	char GainMapBaseFilename[MAX_PATH];
	int ImageExistFlag;

	// Calibration Verification Parameter settings:
	GainMapVerifyStruct gmvs;
    //gmvs.MinPixelAvg  :for 10-bit data, Pixel average value shouldn't smaller than this
    //gmvs.MaxPixelAvg  : ... shouldn't bigger than this
    //gmvs.MinPDAvg     : PD Signal average shouldn't smaller than this
    //gmvs.MaxPDAvg     : PD Signal average shouldn't bigger than this
    //gmvs.MinGain      : PD Gainmap shouldn't smaller than this
    //gmvs.MaxGain      : PD Gainmap value shouldn't bigger than this
    //gmvs.MaxPixelThresh   : max signal of normal pixel should be higher than this
        /* indicator of bit-width error: 
            if max < 256, then could be too dark for a flat image of 10-bit.
        */
	gmvs.MinPixelAvg = MinPixelAvg;
	gmvs.MaxPixelAvg = MaxPixelAvg;//disabled
	gmvs.MinPDAvg = MinPDAvg;
	gmvs.MaxPDAvg = MaxPDAvg;//disabled
	gmvs.MinGain  = MinGain;//0.25x gain
	gmvs.MaxGain = MaxGain;// 4x gain
	gmvs.MaxPixelThresh = MaxPixelThresh;// 1/4  of max range[1023]
	gmvs.GainDiff = GainDiff;

	//print string
	char version_info[4096];
	PDAF_Cal_get_lib_version(version_info);
	info.Format(_T("%s"),CA2T(version_info));
	m_pInterface->AddLog(info);

	char feature_info[4096];
	PDAF_Cal_get_lib_features(feature_info);
	info.Format(_T("%s"),CA2T(feature_info));
	m_pInterface->AddLog(info);

	//Sensor Basic Information: S5K3P3
	PDAFPatternValue.GlobalShift_x = 12; // PDAF Start position x
	PDAFPatternValue.GlobalShift_y = 12; // PDAF Start position y
	PDAFPatternValue.x_step = 64; //PDAF Block Width  
	PDAFPatternValue.y_step = 64; //PDAF Block Height
	PDAFPatternValue.ImageBits = 10; // Image Bit Width 10
	PDAFPatternValue.BlackLevel = 64; // Image Black Level
	PDAFPatternValue.SensorGain = 1;//1
	PDAFPatternValue.DSRatio = 4;//待确认 4
	PDAFPatternValue.PDAFonG = FALSE;//FALSE

	//Define OTP Format: Choose from 4 Different Types
	//0 OTPFORMAT1D: Old 1D Gain Map
	//1 OTPFORMAT2D: Old 2D Gain Map
	//2 OTPFORMATUnited_1D: New 1D Gainm Map
	//3 OTPFORMATUnited_2D: New 2D Gainm Map
	OTPFormat_Type = 3;

	CString PatternFilename = GetModulePath() + "\\PDAF\\Qual\\"+m_Qual_INI_Name;
	strcpy_s(PDAFPatternValue.PDAFPatternFilename,CT2A(PatternFilename.GetBuffer(PatternFilename.GetLength())));//File to store PDAF Pattern
	if (!PathFileExists(PatternFilename))
	{
		info.Format(_T("Cannot Find %s."),PatternFilename);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}
	strcpy_s(CurImgFilename,CT2A(Rawfilename.GetBuffer(Rawfilename.GetLength())));

	CString OutPutFilename = GetModulePath() + "\\PDAF2DGm_S5K3P3_";
	strcpy_s (GainMapBaseFilename, CT2A(OutPutFilename.GetBuffer(OutPutFilename.GetLength())));
	/*******************************************
	First Part: Gain Map Calibration 
	********************************************/
	m_pInterface->AddLog(_T("Now it is Calibrating Gain Map."));
	//Load Image used for Gmap Calibration
	GmCalibImage = (short *)calloc(width*height,sizeof(short));

	ImageExistFlag = Qual_ReadSrcData(CurImgFilename, GmCalibImage, width, height);
	if (ImageExistFlag<0)
	{
		info.Format(_T("Cannot Find Gmap Caibration DATA or Image Size is Error %s."),CurImgFilename);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}

	//Gain Map Calibration
	uint16_t GainMapFlag = -1;
	if (OTPFormat_Type == 0 || OTPFormat_Type == 2)
	{
		GainMapFlag = PDAF_Cal_GainMap(GmCalibImage,width,height,PDAFPatternValue,NULL,&GainMapData1D,gmvs);
	}
	if (OTPFormat_Type == 1 || OTPFormat_Type == 3)
	{
		GainMapFlag = PDAF_Cal_GainMap(GmCalibImage, width, height, PDAFPatternValue,&GainMapData2D,NULL,gmvs);
	}

	int quit = 0;
	quit  = Qual_Display_GmCheckResult(GainMapFlag);
	if(quit == -1)
	{
		m_pInterface->AddLog(_T("Some Error in GainMap, Stop."),COLOR_RED);
		return FALSE;
	}

    if(GainMapFlag & 0x8000)//2D
	{
		if (OTPFormat_Type == 1)
		{
			sprintf_s(GainMapFilename,"%s2D.txt",GainMapBaseFilename);
		} 
		else
		{
			sprintf_s(GainMapFilename,"%sOTPUnitedFormat_2D.txt",GainMapBaseFilename);
		}

		fopen_s(&fp,GainMapFilename,"wb");
		if (OTPFormat_Type == 3)
		{
			fprintf(fp, "%d\n", 2);
			GainMapData2D.VersionNum = 2;
		}
		fprintf(fp,"%d\n",GainMapData2D.OffsetX);
		fprintf(fp,"%d\n",GainMapData2D.OffsetY);
		fprintf(fp,"%d\n",GainMapData2D.RatioX);
		fprintf(fp,"%d\n",GainMapData2D.RatioY);
		fprintf(fp,"%d\n",GainMapData2D.MapWidth);
		fprintf(fp,"%d\n",GainMapData2D.MapHeight);
		for (i = 0; i < GainMapData2D.MapHeight*GainMapData2D.MapWidth; i ++)
			fprintf(fp,"%d\n",GainMapData2D.Left_GainMap[i]);
		for (i = 0; i < GainMapData2D.MapHeight*GainMapData2D.MapWidth; i ++)
			fprintf(fp,"%d\n",GainMapData2D.Right_GainMap[i]);
		fclose(fp);
	}

    if(GainMapFlag & 0x4000)//1D
	{
		if (OTPFormat_Type == 0)
		{
			sprintf_s(GainMapFilename,"%s1D.txt",GainMapBaseFilename);
		} 
		else
		{
			sprintf_s(GainMapFilename,"%sOTPUnitedFormat_1D.txt",GainMapBaseFilename);
		}
		
		fopen_s(&fp,GainMapFilename,"wb");
		if (OTPFormat_Type == 2)
		{
			fprintf(fp, "%d\n", 1);
		}
		fprintf(fp,"%d\n",GainMapData1D.DSRatio);
		fprintf(fp,"%d\n",GainMapData1D.ActualLength);
		fprintf(fp,"%d\n",GainMapData1D.DSLength);
		for (i = 0; i < GainMapData1D.DSLength; i ++)
			fprintf(fp,"%d\n",GainMapData1D.Left_GainMap[i]);
		for (i = 0; i < GainMapData1D.DSLength; i ++)
		fprintf(fp,"%d\n",GainMapData1D.Right_GainMap[i]);
		fclose(fp);
	}
	m_pInterface->AddLog(_T("PDAF Gain Map Calibration Success."),COLOR_BLUE);
	free(GmCalibImage);

	int gap = 0;
	if (OTPFormat_Type == 3)
	{
		Qual_GainMapData[0]=GainMapData2D.VersionNum>>8;
		Qual_GainMapData[1]=GainMapData2D.VersionNum&0xff;
		gap = 2;
	}
	Qual_GainMapData[0+gap]=GainMapData2D.OffsetX>>8;
	Qual_GainMapData[1+gap]=GainMapData2D.OffsetX&0xff;
	Qual_GainMapData[2+gap]=GainMapData2D.OffsetY>>8;
	Qual_GainMapData[3+gap]=GainMapData2D.OffsetY&0xff;

	Qual_GainMapData[4+gap]=GainMapData2D.RatioX>>8;
	Qual_GainMapData[5+gap]=GainMapData2D.RatioX&0xff;
	Qual_GainMapData[6+gap]=GainMapData2D.RatioY>>8;
	Qual_GainMapData[7+gap]=GainMapData2D.RatioY&0xff;

	Qual_GainMapData[8+gap]=GainMapData2D.MapWidth>>8;	
	Qual_GainMapData[9+gap]=GainMapData2D.MapWidth&0xff;

	Qual_GainMapData[10+gap]=GainMapData2D.MapHeight>>8;
	Qual_GainMapData[11+gap]=GainMapData2D.MapHeight&0xff;

	
	for (i=0;i<GainMapData2D.MapWidth*GainMapData2D.MapHeight;i++)
	{	
		Qual_GainMapData[12+gap+i*2]=GainMapData2D.Left_GainMap[i]>>8;
		Qual_GainMapData[13+gap+i*2]=GainMapData2D.Left_GainMap[i]&0xff;

		Qual_GainMapData[544+gap+i*2]=GainMapData2D.Right_GainMap[i]>>8;
		Qual_GainMapData[545+gap+i*2]=GainMapData2D.Right_GainMap[i]&0xff;
	}

	return TRUE;

	/*******************************************
	End of Gain Map Calibration 
	********************************************/
}

int COtpBase::Qual_ReadSrcData(char* SrcRAWFilename, short *RAWDATA, int width, int height)
{
	int i,j;
	FILE *fp;
	//	unsigned char readval1,readval2;
	unsigned short ReadData;

	fopen_s(&fp,SrcRAWFilename,"rb");
	if (fp == NULL)
		return -1;

	for (i = 0; i < height; i ++)
	{
		for (j = 0; j < width; j ++)
		{
			fread(&ReadData,sizeof(unsigned short),1,fp);
			RAWDATA[i * width + j] = short(ReadData);
		}
	}
	fclose(fp);
	return 0;
}

int COtpBase::Qual_Display_GmCheckResult(int GainMapFlag)
{
	if (GainMapFlag & 0x0200)
	{
		m_pInterface->AddLog(_T("0x0200: GainMap Calibration, Memory error. "),COLOR_RED);
		return -1;
	}
	if (GainMapFlag & 0x0100)
	{
		m_pInterface->AddLog(_T("0x0100: GainMap Calibration, Input Parameter Error. "),COLOR_RED);
		return -1;
	}
	if (GainMapFlag & 0x0080)
	{
		m_pInterface->AddLog(_T("0x0080: Error, GainMap Calibration, PD signal too weak. "),COLOR_RED);
		return -1;
	}
	if (GainMapFlag & 0x0040)
	{
		m_pInterface->AddLog(_T("0x0040: Error, GainMap Calibration, PD signal too strong. "),COLOR_RED);
		return -1;
	}
	if (GainMapFlag & 0x0020)
	{
		m_pInterface->AddLog(_T("0x0020: Error, GainMap Calibration, Normal Pixel too weak. "),COLOR_RED);
		return -1;
	}
	if (GainMapFlag & 0x0010)
	{
		m_pInterface->AddLog(_T("0x0010: Error, GainMap Calibration, Normal Pixel too strong. "),COLOR_RED);
		return -1;
	}
	if (GainMapFlag & 0x0008)
	{
		m_pInterface->AddLog(_T("0x0008: Error, GainMap Calibration, max of Normal Pixel too weak. "),COLOR_RED);
		return -1;
	}
	if (GainMapFlag & 0x0004)
	{
		m_pInterface->AddLog(_T("0x0004: Error, GainMap Calibration, pattern file error. "),COLOR_RED);
		return -1;
	}

	if (GainMapFlag & 0x0002)
	{
		m_pInterface->AddLog(_T("0x0002: Warning, GainMap Calibration, GM value too small. "),COLOR_RED);
		return 0;
	}
	if (GainMapFlag & 0x0001)
	{
		m_pInterface->AddLog(_T("0x0001: Warning, GainMap Calibration, GM value too big. "),COLOR_RED);
		return 0;
	}
	m_pInterface->AddLog(_T("The Gainmap is Normal. "),COLOR_BLUE);

	return 0;
}
//////////////////////////////////////////Sensor////////////////////////////////////////////////////////////////////////////////////////////
BOOL COtpBase::S5K3L8_GetLSC()
{
	//only for test
	// 	BYTE _LSCData[417] = {0};
	// 	if (m_LscGroup == 1)
	// 	{
	// 	    S5K3L8_ReadOTPPage(1, _LSCData);
	// 		if (m_LscGroup == 1)
	// 		{
	// 			S5K3L8_ReadOTPPage(0x19,_LSCData);
	// 			Sleep(100);
	// 			S5K3L8_ReadOTPPage(0x1A,_LSCData+64);
	// 			Sleep(100);
	// 			S5K3L8_ReadOTPPage(0x1B,_LSCData+128);
	// 			Sleep(100);
	// 			S5K3L8_ReadOTPPage(0x1C,_LSCData+192);
	// 			Sleep(100);
	// 			S5K3L8_ReadOTPPage(0x1D,_LSCData+256);
	// 			Sleep(100);
	// 			S5K3L8_ReadOTPPage(0x1E,_LSCData+320);
	// 			Sleep(100);
	// 			S5K3L8_ReadOTPPage(0x1F,_LSCData+384);
	// 			Sleep(100);	
	// 		}
	// 
	// 		if (m_LscGroup == 2)
	// 		{
	// 			S5K3L8_ReadOTPPage(0x12,_LSCData);
	// 			Sleep(100);
	// 			S5K3L8_ReadOTPPage(0x13,_LSCData+64);
	// 			Sleep(100);
	// 			S5K3L8_ReadOTPPage(0x14,_LSCData+128);
	// 			Sleep(100);
	// 			S5K3L8_ReadOTPPage(0x15,_LSCData+192);
	// 			Sleep(100);
	// 			S5K3L8_ReadOTPPage(0x16,_LSCData+256);
	// 			Sleep(100);
	// 			S5K3L8_ReadOTPPage(0x17,_LSCData+320);
	// 			Sleep(100);
	// 			S5K3L8_ReadOTPPage(0x18,_LSCData+384);
	// 			Sleep(100);
	// 			S5K3L8_ReadOTPPage(0x19,_LSCData+448);
	// 			Sleep(100);
	// 		}
	// 		S5K3L8_ReadOTPPage(26, _LSCData);
	// 	}
	// 	if (m_LscGroup == 2)
	// 	{
	// 		S5K3L8_ReadOTPPage(19, _LSCData);
	// 	}
	// 	if ( (_LSCData[8]|_LSCData[7]|_LSCData[6]|_LSCData[5]|_LSCData[4]|_LSCData[3]|_LSCData[2]|_LSCData[1]|_LSCData[0]) != 0)
	// 	{
	// 		m_pInterface->AddLog(_T("OTPÄÚÒÑÓÐLSCÊý¾Ý£¡"), COLOR_RED);
	// 		return FALSE;
	// 	}
	// 	return TRUE;
	//////////////////////////////////////////////////////////////////////////
	m_pInterface->AddLog(_T("Get 3L8 Sensor LSC start..."));
	CString Path;
	Path = m_pInterface->GetExeDir();
	Path += L"LensRI.txt";

	FILE* fp;
	if (_wfopen_s(&fp,  Path, _T("rt")))  
	{
		info.Format(_T("ÕÒ²»µ½ÎÄ¼þ%s!"),Path);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}

	double *pLensRI = new double[11];
	char strData[256];
	for (int i=0 ;i<11; ++i)
	{
		fscanf_s(fp,"%s",strData,5);
		sscanf_s(strData, "%lf", pLensRI+i);
	}

	SetUserLensRI_3L8(pLensRI[0], pLensRI[1], pLensRI[2], pLensRI[3], pLensRI[4], pLensRI[5], pLensRI[6], pLensRI[7], pLensRI[8], pLensRI[9], pLensRI[10]);
	SAFE_DELETE_ARRAY(pLensRI);
	fclose(fp);

	ImageInfo imgInfo = m_pInterface->GetImage();
	int width = imgInfo.width;
	int height= imgInfo.height;

	SetParameter_3L8(64, width, height,  m_LscTarget, m_LscTarget, m_LscTarget, m_LscTarget, 0.3, 0.01, 1, 0);
	Sleep(100);
	BOOL SetGroup;
	if (m_LscGroup == 1)
	{
		SetGroup = TRUE;
	}
	if (m_LscGroup == 2)
	{
		SetGroup = FALSE;
	}

	CString strWorkPath  = GetModulePath()+"\\3L8";
	CreateDirectory(strWorkPath, NULL);
	CString sfilename = strWorkPath + "\\LSC.raw";
	CString strOTPSetFile = strWorkPath + "\\3L8LSCOTP.sset";
	CString strSRAMSetFile = strWorkPath + "\\3L8LSCSRAM.sset"; 
#ifdef  TestVersion
	if (PathFileExists(sfilename)) 
	{
		if (!DeleteFile(sfilename))
		{
			m_pInterface->AddLog(_T("文件删除失败!"), COLOR_RED);
			return FALSE;
		}
	}
	SaveCurrentRaw10(sfilename,imgInfo.Raw_Buf,width,height);
#endif
	FILE* fp1;
	if (_wfopen_s(&fp1,  sfilename, _T("rb")))  
	{
		info.Format(_T("打开文件失败%s!"),Path);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}

	BYTE* raw_buffer = NULL;
	if (fp1)
	{
		raw_buffer = new BYTE[width*height*2];
		fread((void*)raw_buffer,2,width*height,fp1);
		fclose(fp1);
	}

	BOOL flag = Run_3L8(raw_buffer, raw_buffer, FALSE, SetGroup, strWorkPath);
	if (!flag)
	{
		SAFE_DELETE_ARRAY(raw_buffer);
		m_pInterface->AddLog(_T("Fail to run 3L8 LSC!"),COLOR_RED);
		return FALSE;		
	}

	SAFE_DELETE_ARRAY(raw_buffer);
	SaveOTPSetfile_3L8(CT2A(strOTPSetFile.GetBuffer(MAX_PATH)));
	strOTPSetFile.ReleaseBuffer();

	SaveSRAMSetfile_3L8(CT2A(strSRAMSetFile.GetBuffer(MAX_PATH)));
	strSRAMSetFile.ReleaseBuffer();

	if (!S5K3L8_ApplyLSC())  return FALSE;

	m_pInterface->AddLog(_T("Get 3L8 Sensor LSC pass!"),COLOR_BLUE);
	return TRUE;
}


///////////////////////////////////s5k3l8///////////////////////////////////////
BOOL COtpBase::S5K3L8_ApplyLSC()
{
	m_pInterface->RestartCamera(1);//close camera & stream off 
	SaveExp = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllExp"), 1);
	m_pInterface->GetImageSensor()->WriteExp(SaveExp);
	SaveGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllGain"), 1);
	m_pInterface->GetImageSensor()->WriteAnalogGain(SaveGain);
	// 	USHORT Val;
	// 	i2c_read(0x0100, &Val);
	// 	CString str;
	// 	str.Format(_T("0100val:0x%x"),Val);
	// 	m_pInterface->AddLog(str);

	CString strfile,tmp;
	CString strWorkPath  = GetModulePath()+"\\3L8";
	CreateDirectory(strWorkPath, NULL);
	tmp.Format(_T("\\3L8LSCSRAM.sset"));
	strfile = strWorkPath + tmp;

	CStdioFile file;
	if (!file.Open((strfile), CFile::modeRead | CFile::typeText))
	{
		m_pInterface->AddLog(_T("打开文件失败!"),COLOR_RED);
		return FALSE;		
	}

	CString strLine;
	int count1 =0; 
	int count2 = 0;

	i2c_write(0x6028, 0x2000);
	i2c_write(0x602a, 0x0cf4);
	i2c_write_byte(0x6f12,0x00);
	Sleep(100);

	i2c_write(0x6028, 0x4000);
	i2c_write(0x602a, 0x0b00);
	i2c_write_byte(0x6f12, 0x01);
	Sleep(200);

	while(file.ReadString(strLine))
	{
		if (strLine.Find(_T("WRITE #gisp_gras_LutDataWriteH_"),0) != -1)
		{
			strLine.Replace(_T("WRITE #gisp_gras_LutDataWriteH_"),_T(""));
			CString strval = strLine.Right(2);
			int val = strtoul(CT2A(strval), NULL, 16);

			int temppos = strLine.ReverseFind('_');
			strLine = strLine.Left(temppos);
			int num = strtoul(CT2A(strLine), NULL, 10);
			if (num == 0)
			{
				i2c_write(0x6028, 0x2000);
			}
			i2c_write(0x602a, 0x0d05+num);
			i2c_write_byte(0x6f12, val&0xff);
			count1++;
		}
		else if (strLine.Find(_T("WRITE #gisp_gras_LutDataWriteL_"),0) != -1)
		{
			strLine.Replace(_T("WRITE #gisp_gras_LutDataWriteL_"),_T(""));
			CString strval = strLine.Right(4);
			int val = strtoul(CT2A(strval), NULL, 16);

			int temppos = strLine.ReverseFind('_');
			strLine = strLine.Left(temppos);
			int num = strtoul(CT2A(strLine), NULL, 10);

			i2c_write(0x602a, 0x0d96+num*2);
			i2c_write(0x6f12, val&0xffff);
			count2++;
		}
		// 		if (strLine.Find(_T("s"),0) != -1)
		// 		{
		// 			int nlength = strLine.GetLength();
		// 			nlength--;
		// 			strLine = strLine.Right(nlength);
		// 			intVal = strtoul(CT2A(strLine), NULL, 16);
		// 			if (nlength == 6)
		// 			{
		// 				intLSCAddr = intVal>>8;
		// 				intLSCValue = intVal % 256;
		// 				i2c_write_byte(intLSCAddr,(BYTE)intLSCValue);
		// 				count++;
		// 			}
		// 			else if (nlength == 8)
		// 			{
		// 				intLSCAddr = intVal>>16;
		// 				intLSCValue = (intVal&0xffff);
		// 				i2c_write(intLSCAddr,intLSCValue);
		// 				count++;
		// 			}
		// 		}
	}

	file.Close();
	i2c_write(0x6028, 0x4000);
	i2c_write(0x602a, 0x0100);
	i2c_write_byte(0x6f12, 0x01);
	Sleep(100);

	i2c_write(0x6028, 0x4000);
	i2c_write(0x602a, 0x305A);
	i2c_write_byte(0x6f12, 0x08);

	return TRUE;
}

void COtpBase::S5K3L8_ReadOTPPage(int page, BYTE *Buff)
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

void COtpBase::S5K3L8_WriteOTPPage(int page, BYTE *Buff)
{
	// Set control register
	i2c_write(0x0100,0x0100);//stream   on 
	i2c_write(0x0A02,page*256); //set page
	//WRITE otp data to buff
	Sleep(10);
	for(int i=0;i<64;i++)
	{
		i2c_write_byte(0x0a04+i,Buff[i]);
		Sleep(10);
	}
	i2c_write(0x0a00,0x0300);//write start
	Sleep(100);
	i2c_write(0x0a00,0x0000);//make initial state
	return;
}

void COtpBase::Sensor3L8_StreamOn()
{
	BYTE stram_flag = 0;
	i2c_read_byte(0x0100, &stram_flag);
	if (stram_flag == 0)
	{
		i2c_write_byte(0x0100, 0x01);
		i2c_write(0x305A, 0x0008);
	}
	return;
}

////////////////////////////////////Sensor 5E8//////////////////////////////////////
BOOL COtpBase::S5K5E8ReadOTPpage(int Page, USHORT address, BYTE *Data, int length)
{  

	BOOL bRet = TRUE;
	i2c_write_byte(0x0100, 0x00);
	Sleep(5*m_sleeptime);
	i2c_write_byte(0x0a00, 0x04);
	i2c_write_byte(0x0a02, Page);
	i2c_write_byte(0x0a00, 0x01);
	Sleep(1*m_sleeptime);
	for ( int i=0; i<length; i++ )
	{
		bRet &= i2c_read_byte((address+i), &Data[i]);
	}
	i2c_write_byte(0x0a00, 0x04);
	i2c_write_byte(0x0a00, 0x00);
 	i2c_write_byte(0x0100, 0x01);
 	Sleep(5*m_sleeptime);
	if (!bRet)
	{
		info.Format(_T("Write Page%d Data Fail!"),Page);
		m_pInterface->AddLog(info,COLOR_RED);		
	}
	return bRet;
}

BOOL COtpBase::S5K5E8WriteOTPpage(int Page, USHORT address, BYTE *Data, int length)
{
	if (length <= 32)
	{
		i2c_write_byte(0x0100, 0x00);
		Sleep(5*m_sleeptime);
		i2c_write_byte(0x3b42, 0x68);
		i2c_write_byte(0x3b41, 0x01);
		i2c_write_byte(0x3b40, 0x00);
		i2c_write_byte(0x0a00, 0x04);
		i2c_write_byte(0x0a00, 0x03);
		i2c_write_byte(0x3b42, 0x00);
		i2c_write_byte(0x3b45, 0x01);
		i2c_write_byte(0x0a02, Page);
		i2c_write_byte(0x0a00, 0x03);
		Sleep(5*m_sleeptime);
		for ( int i=0; i<length; i++ )
		{
			BYTE WriteEnding;
			i2c_read_byte(0x0A01, &WriteEnding);
			do 
			{
				Sleep(0*m_sleeptime);
			} while ((WriteEnding & 0x02));
			i2c_write_byte((address+i), Data[i]);
		}
		BYTE WriteEnding;
		i2c_read_byte(0x0A01, &WriteEnding);
		do 
		{
			Sleep(0*m_sleeptime);
		} while ((WriteEnding & 0x02));
		Sleep(5*m_sleeptime);
		i2c_write_byte(0x0a00, 0x04);
		i2c_write_byte(0x0a00, 0x00);
		i2c_write_byte(0x3b40, 0x01);
		Sleep(5*m_sleeptime);
		i2c_write_byte(0x0100, 0x01);
		Sleep(10*m_sleeptime);
	}
	else
	{
		i2c_write_byte(0x0100, 0x00);
		Sleep(5*m_sleeptime);
		i2c_write_byte(0x3b42, 0x68);
		i2c_write_byte(0x3b41, 0x01);
		i2c_write_byte(0x3b40, 0x00);
		i2c_write_byte(0x0a00, 0x04);
		i2c_write_byte(0x0a00, 0x03);
		i2c_write_byte(0x3b42, 0x00);
		i2c_write_byte(0x3b45, 0x01);
		i2c_write_byte(0x0a02, Page);
		i2c_write_byte(0x0a00, 0x03);
		Sleep(5*m_sleeptime);
		for ( int i=0; i<length/2; i++ )
		{
			BYTE WriteEnding;
			i2c_read_byte(0x0A01, &WriteEnding);
			do 
			{
				Sleep(0*m_sleeptime);
			} while ((WriteEnding & 0x02));
			i2c_write_byte((address+i), Data[i]);
		}
		BYTE WriteEnding;
		i2c_read_byte(0x0A01, &WriteEnding);
		do 
		{
			Sleep(0*m_sleeptime);
		} while ((WriteEnding & 0x02));
		Sleep(5*m_sleeptime);
		i2c_write_byte(0x0a00, 0x04);
		i2c_write_byte(0x0a00, 0x00);
		i2c_write_byte(0x3b40, 0x01);
		Sleep(5*m_sleeptime);
		i2c_write_byte(0x0100, 0x01);
		Sleep(10*m_sleeptime);

		i2c_write_byte(0x0100, 0x00);
		Sleep(5*m_sleeptime);
		i2c_write_byte(0x3b42, 0x68);
		i2c_write_byte(0x3b41, 0x01);
		i2c_write_byte(0x3b40, 0x00);
		i2c_write_byte(0x0a00, 0x04);
		i2c_write_byte(0x0a00, 0x03);
		i2c_write_byte(0x3b42, 0x00);
		i2c_write_byte(0x3b45, 0x01);
		i2c_write_byte(0x0a02, Page);
		i2c_write_byte(0x0a00, 0x03);
		Sleep(5*m_sleeptime);
		for ( int i=0; i<length/2; i++ )
		{
			BYTE WriteEnding;
			i2c_read_byte(0x0A01, &WriteEnding);
			do 
			{
				Sleep(0*m_sleeptime);
			} while ((WriteEnding & 0x02));
			i2c_write_byte((address+length/2+i), Data[i+length/2]);
		}
		i2c_read_byte(0x0A01, &WriteEnding);
		do 
		{
			Sleep(0*m_sleeptime);
		} while ((WriteEnding & 0x02));
		Sleep(5*m_sleeptime);
		i2c_write_byte(0x0a00, 0x04);
		i2c_write_byte(0x0a00, 0x00);
		i2c_write_byte(0x3b40, 0x01);
		Sleep(5*m_sleeptime);
		i2c_write_byte(0x0100, 0x01);
		Sleep(10*m_sleeptime);
	}

	return TRUE;
}

BOOL COtpBase::S5K5E8_GetLSC()
{
	m_pInterface->AddLog(_T("Get 5E8 Sensor LSC start..."));
	CString Path;
	Path = m_pInterface->GetExeDir();

	ImageInfo imgInfo = m_pInterface->GetImage();
	int width = imgInfo.width;
	int height= imgInfo.height;

	SetParameter_5E8(64, width, height,  m_LscTarget, m_LscTarget, m_LscTarget, m_LscTarget, 0.3, 0.01, 1, 0);
	Sleep(100*m_sleeptime);

	CString strWorkPath  = GetModulePath()+"\\LSC\\Sensor\\5E8";
	CreateDirectory(strWorkPath, NULL);
	CString sfilename = strWorkPath + "\\LSC.raw";
	CString strOTPSetFile = strWorkPath + "\\5E8LSCOTP.sset";
	CString strSRAMSetFile = strWorkPath + "\\5E8LSCSRAM.sset"; 
#ifndef  TestVersion
	if (PathFileExists(sfilename)) 
	{
		if (!DeleteFile(sfilename))
		{
			m_pInterface->AddLog(_T("文件删除失败!"), COLOR_RED);
			return FALSE;
		}
	}
	SaveCurrentRaw10(sfilename,imgInfo.Raw_Buf,width,height);
#endif
	FILE* fp;
	if (_wfopen_s(&fp,  sfilename, _T("rb")))  
	{
		info.Format(_T("打开文件失败%s!"),sfilename);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}

	BYTE* raw_buffer = NULL;
	if (fp)
	{
		raw_buffer = new BYTE[width*height*2];
		fread((void*)raw_buffer,2,width*height,fp);
		fclose(fp);
	}

	BOOL flag = Run_5E8(raw_buffer, raw_buffer, FALSE, strWorkPath);
	if (!flag)
	{
		SAFE_DELETE_ARRAY(raw_buffer);
		m_pInterface->AddLog(_T("Fail to run 5E8 LSC!"),COLOR_RED);
		return FALSE;		
	}

	SAFE_DELETE_ARRAY(raw_buffer);
	SaveOTPSetfile_5E8(CT2A(strOTPSetFile.GetBuffer(MAX_PATH)));
	strOTPSetFile.ReleaseBuffer();

	SaveSRAMSetfile_5E8(CT2A(strSRAMSetFile.GetBuffer(MAX_PATH)));
	strSRAMSetFile.ReleaseBuffer();

	if (!S5K5E8_ApplyLSC())  return FALSE;

	m_pInterface->AddLog(_T("Get 5E8 Sensor LSC pass!"),COLOR_BLUE);
	return TRUE;
}

BOOL COtpBase::S5K5E8_ApplyLSC()
{
	m_pInterface->RestartCamera(1);//close camera & stream off 
	SaveExp = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllExp"), 1);
	m_pInterface->GetImageSensor()->WriteExp(SaveExp);
	SaveGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllGain"), 1);
	m_pInterface->GetImageSensor()->WriteAnalogGain(SaveGain);

	CString strfile,tmp;
	CString strWorkPath  = GetModulePath()+"\\LSC\\Sensor\\5E8";
	CreateDirectory(strWorkPath, NULL);
	tmp.Format(_T("\\5E8LSCSRAM.sset"));
	strfile = strWorkPath + tmp;

	CStdioFile file;
	if (!file.Open((strfile), CFile::modeRead | CFile::typeText))
	{
		m_pInterface->AddLog(_T("打开文件失败!"),COLOR_RED);
		return FALSE;		
	}

	CString strLine;

	int	 intVal;
	int  intLSCAddr;
	int  intLSCValue;

// 	BYTE value1=0;
// 	BYTE value2=0;
// 	i2c_read_byte(0x0202,&value1);
// 	i2c_read_byte(0x0203,&value2);
// 
// 	i2c_write_byte(0x0100,0x00);
// 	Sleep(10);
// 
// 	i2c_write_byte(0x0101,0x00);             
// 	i2c_write_byte(0x0136,0x18);             
// 	i2c_write_byte(0x3303,0x02);             
// 	i2c_write_byte(0x3400,0x01);             
// 	i2c_write_byte(0x3906,0xBE);             
// 	i2c_write_byte(0x3C01,0x0F);             
// 	i2c_write_byte(0x3C14,0x00);             
// 	i2c_write_byte(0x3235,0x08);             
// 	i2c_write_byte(0x3063,0x2E);             
// 	i2c_write_byte(0x307A,0x40);             
// 	i2c_write_byte(0x307B,0x11);             
// 	i2c_write_byte(0x3079,0x20);             
// 	i2c_write_byte(0x3070,0x05);             
// 	i2c_write_byte(0x3067,0x06);             
// 	i2c_write_byte(0x3071,0x62);             
// 	i2c_write_byte(0x3203,0x4);             
// 	i2c_write_byte(0x3205,0x4);             
// 	i2c_write_byte(0x320b,0x42);             
// 	i2c_write_byte(0x3007,0x00);             
// 	i2c_write_byte(0x3008,0x14);             
// 	i2c_write_byte(0x3020,0x58);             
// 	i2c_write_byte(0x300D,0x34);             
// 	i2c_write_byte(0x300E,0x17);             
// 	i2c_write_byte(0x3021,0x02);             
// 	i2c_write_byte(0x3010,0x59);             
// 	i2c_write_byte(0x3002,0x01);             
// 	i2c_write_byte(0x3005,0x01);             
// 	i2c_write_byte(0x3008,0x04);             
// 	i2c_write_byte(0x300F,0x70);             
// 	i2c_write_byte(0x3010,0x69);             
// 	i2c_write_byte(0x3017,0x10);             
// 	i2c_write_byte(0x3019,0x19);             
// 	i2c_write_byte(0x300C,0x62);             
// 	i2c_write_byte(0x3064,0x10);             
// 	i2c_write_byte(0x323D,0x01);             
// 	i2c_write_byte(0x323E,0x3E);             
// 	i2c_write_byte(0x0305,0x04);             
// 	i2c_write_byte(0x0307,0xa6);             
// 	i2c_write_byte(0x0340,0x0f);             
// 	i2c_write_byte(0x0341,0x82);  
// 
// 	i2c_write_byte(0x0202,value1);             
// 	i2c_write_byte(0x0203,value2);             
// 
// 	i2c_write_byte(0x3400,0x00); // lsc on   
// 
// 	i2c_write_byte(0x0100,0x00); //stream off

	int k=0;
	while(file.ReadString(strLine))
	{
		k++;
		strLine = strLine.Left(9);
		strLine.TrimRight();
		if (strLine.Find(_T("s"),0) != -1)
		{
			int nlength = strLine.GetLength();
			nlength--;
			strLine = strLine.Right(nlength);
			intVal = strtoul(CT2A(strLine), NULL, 16);
			if (nlength == 6)
			{
				intLSCAddr = intVal>>8;
				intLSCValue = intVal % 256;
				if ((k>2) && intLSCAddr == 0x3457)
				{
					i2c_write_byte(intLSCAddr,intLSCValue);
					Sleep(10*m_sleeptime);
				}
				else
				{
					i2c_write_byte(intLSCAddr,intLSCValue);
				}
			}
			else if (nlength == 8)
			{
				intLSCAddr = intVal>>16;
				intLSCValue = ( intVal&0xffff );			
				i2c_write(intLSCAddr, intLSCValue); 
			}
		}
	}
	file.Close();

	i2c_write_byte(0x0100, 0x01);//stream on
	Sleep(10*m_sleeptime);

	return TRUE;
}