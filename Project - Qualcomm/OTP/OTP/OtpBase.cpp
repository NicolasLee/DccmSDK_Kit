#include "stdafx.h"
#include "Rver_LSCstdafx.h"
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

COtpBase::COtpBase()
{
	m_OTPType = L"";
}

COtpBase::~COtpBase()
{

}

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

BOOL COtpBase::GT24C64_All_Erase(int size)
{
	int i = 0;
	int group = 0;
	int num = 0;
	BYTE* eraseData = new BYTE[32]();
	memset(eraseData, 0xff, 32*sizeof(BYTE));//注意初始EEPROM是00还是ff

	if(size-32>0)
	{
		group = size/32;
		num=size%32;
		if (num != 0)
		{
			group += 1;
		}
		num = 32;
	}
	else
	{
		group=0;
		num=size;
	}

	for (i=0; i<group; i++)
	{
		if (!GT24C64_Programming(0x0000+i*num,eraseData,num))//erase all
		{
			m_pInterface->AddLog(_T("OTP数据擦除失败!"),COLOR_RED);
			SAFE_DELETE_ARRAY(eraseData);
			return FALSE;
		}
	}

	m_pInterface->AddLog(_T("GT24C64 All Data Erased!"),COLOR_LIGHTGOLDENRODYELLOW);
	SAFE_DELETE_ARRAY(eraseData);
	return TRUE;
}

BOOL COtpBase::GT24C64_Read(USHORT address,unsigned char *Data,int size)
{
	UCHAR slaveid = 0xA0;//cnw
	int i=0;
	int group=0;
	int num=0;

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
		m_pInterface->I2C_MultiBytesRead(slaveid, address, 2, Data, Count);
	msleep(30);
	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesRead(slaveid, address + (Count + i * 32), 2, Data + (Count + i * 32), 32);
		msleep(10);
	}
	m_pInterface->I2C_MultiBytesRead(slaveid, address + (Count + i * 32), 2, Data + (Count + i * 32), num);
	return TRUE;
	
}

BOOL COtpBase::GT24C64_Programming(USHORT address,unsigned char *Data,int size)
{
	UCHAR slaveid = 0xA0;//cnw
	int i=0;
	int group=0;
	int num=0;
	BYTE* CheckData=new BYTE[size]();
	BOOL bRet = TRUE;

	do 
	{	
		msleep(10);
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
		m_pInterface->I2C_MultiBytesWrite(slaveid, address, 2, Data, Count);
		msleep(10);

		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesWrite(slaveid, address + (Count + i * 32), 2, Data + (Count + i * 32), 32);
			msleep(10);	
		}
		m_pInterface->I2C_MultiBytesWrite(slaveid, address + (Count + i * 32), 2, Data + (Count + i * 32), num);
		msleep(10);
		if(Count>0)
			m_pInterface->I2C_MultiBytesRead(slaveid, address, 2, CheckData, Count);

		msleep(10);
		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesRead(slaveid, address + (Count + i * 32), 2, CheckData + (Count + i * 32), 32);
			msleep(10);
		}
		m_pInterface->I2C_MultiBytesRead(slaveid, address + (Count + i * 32), 2, CheckData + (Count + i * 32), num);
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

BOOL COtpBase::FM24C64_Read(USHORT address,unsigned char *Data,int size)
{
	int i=0;
	int group=0;
	int num=0;

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
	msleep(30);
	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,Data+(Count+i*32),32);
		msleep(10);
	}
	m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,Data+(Count+i*32),num);
	return TRUE;

}

BOOL COtpBase::FM24C64_Programming(USHORT address,unsigned char *Data,int size)
{
	int i=0;
	int group=0;
	int num=0;
	BYTE* CheckData=new BYTE[size]();
	BOOL bRet = TRUE;

	do 
	{	
		msleep(10);
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
		msleep(10);

		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesWrite(0xA0,address+(Count+i*32),2,Data+(Count+i*32),32);
			msleep(10);
		}
		m_pInterface->I2C_MultiBytesWrite(0xA0,address+(Count+i*32),2,Data+(Count+i*32),num);
		msleep(10);
		if(Count>0)
			m_pInterface->I2C_MultiBytesRead(0xA0,address,2,CheckData,Count);

		msleep(10);
		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesRead(0xA0,address+(Count+i*32),2,CheckData+(Count+i*32),32);
			msleep(10);
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
	ZC533_Erase_lock = ZC533_LastPage - page;
	ZC533_LastPage = page;
	BOOL bRet = TRUE;
	if (ZC533_Erase_lock != 0)//不允许连续两次erase 同一page
	{
		bRet &= m_pInterface->I2C_Write(0xB0, ((ZC533_PAGE_ERASE<<14)|(page<<6)), 0, I2CMODE_16_8);
		msleep(6);
	}

	return bRet;
}

BOOL COtpBase::ZC533_All_Erase()
{
	BOOL bRet = TRUE;
	bRet &= m_pInterface->I2C_Write(0xB0, (ZC533_ALL_ERASE<<14), 0, I2CMODE_16_8);
	msleep(6);
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
		msleep(10);

		for (i=0;i<retgroup;i++)
		{
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),pagesize);//|(reg&0x3f)   +(Count+i*pagesize)
			msleep(10);
		}
		m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),retnum);//|(reg&0x3f)   +(Count+i*pagesize)
	}
	else
	{
		m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page)<<6)|(reg&0x3f))+(Count+i*32),2,Data+(Count+i*32),num);
		msleep(10);
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
			msleep(10);

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
					msleep(10);
//				}
			}
			if (!ZC533_Page_Erase(page+i+1))	
			{
				bRet = FALSE;
				break;
			}
			m_pInterface->I2C_MultiBytesWrite(0xB0,((ZC533_WRITE<<14)|((page+i+1)<<6)),2,Data+(retsize+i*pagesize),retnum);//|(reg&0x3f))+(Count+i*pagesize)
			msleep(10);
		}
		else
		{
			if (!ZC533_Page_Erase(page))	
			{
				bRet = FALSE;
				break;
			}
			m_pInterface->I2C_MultiBytesWrite(0xB0,((ZC533_WRITE<<14)|(page<<6)|(reg&0x3f))+(Count+i*pagesize),2,Data+(Count+i*pagesize),num);
			msleep(10);
		}
		////////////////////////////////Read//////////////////////////////////////////
		if (ret>0)
		{
			if(Count>0)
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|(page<<6)|(reg&0x3f)),2,CheckData,retsize);
			msleep(5);

			for (i=0;i<retgroup;i++)
			{
				m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,CheckData+(retsize+i*pagesize),pagesize);//|(reg&0x3f)   +(Count+i*pagesize)
				msleep(5);
			}
			m_pInterface->I2C_MultiBytesRead(0xB0,((ZC533_READ<<14)|((page+i+1)<<6)),2,CheckData+(retsize+i*pagesize),retnum);//|(reg&0x3f)   +(Count+i*pagesize)
			msleep(5);
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
			msleep(5);
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

	bRet &= m_pInterface->I2C_Write(0xB0, ((1<<7)|(page<<2)|(0x02)), 0xee, I2CMODE_8_8);
	msleep(4);

	return bRet;
}

BOOL COtpBase::DW9763_All_Erase()
{
	BOOL bRet = TRUE;
	bRet &= m_pInterface->I2C_Write(0xB0, 0x81, 0xee, I2CMODE_8_8);
	msleep(15);

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
	msleep(30);
	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,Data+(Count+i*32),32);
		msleep(10);
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
		msleep(10);
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
		msleep(10);

		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesWrite(0xB0,address+(Count+i*32),2,Data+(Count+i*32),32);
			msleep(10);
		}
		m_pInterface->I2C_MultiBytesWrite(0xB0,address+(Count+i*32),2,Data+(Count+i*32),num);
		msleep(10);
		if(Count>0)
			m_pInterface->I2C_MultiBytesRead(0xB0,address,2,CheckData,Count);

		msleep(10);
		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,CheckData+(Count+i*32),32);
			msleep(10);
		}
		m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,CheckData+(Count+i*32),num);
		for (i=0;i<size;i++)
		{
			if (CheckData[i]!=Data[i])
			{
				info.Format(_T("DW9761/63/67读写对比失败，序号:%d, 读取值:%d, 写入值:%d"), i, CheckData[i], Data[i]);
				m_pInterface->AddLog(info);
				bRet = FALSE;
				break;
			}
		}
	} while (0);

	SAFE_DELETE_ARRAY(CheckData);
	return bRet;
}

BOOL COtpBase::DW9767_All_Erase()
{
	BOOL bRet = TRUE;

	bRet &= m_pInterface->I2C_Write(0x18, 0x02, 0x10, I2CMODE_8_8);
	bRet &= m_pInterface->I2C_Write(0xB0, 0x81, 0xee, I2CMODE_8_8);
	msleep(15);
	bRet &= m_pInterface->I2C_Write(0x18, 0x02, 0x00, I2CMODE_8_8);

	m_pInterface->AddLog(_T("DW9767 All Data Erased!"),COLOR_LIGHTGOLDENRODYELLOW);
	return bRet;
}

BOOL COtpBase::DW9767_Page_Erase(BYTE page)
{
	BOOL bRet = TRUE;
	bRet &= m_pInterface->I2C_Write(0x18, 0x02, 0x10, I2CMODE_8_8);
	bRet &= m_pInterface->I2C_Write(0xB0, ((1<<7)|(page<<2)|(0x02)), 0xee, I2CMODE_8_8);
	msleep(4);
	bRet &= m_pInterface->I2C_Write(0x18, 0x02, 0x00, I2CMODE_8_8);

	return bRet;
}

BOOL COtpBase::DW9767_Read(USHORT address,unsigned char *Data,int size)
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
	msleep(30);
	for (i=0;i<group;i++)
	{
		m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,Data+(Count+i*32),32);
		msleep(10);
	}
	m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,Data+(Count+i*32),num);
	return TRUE;
}

BOOL COtpBase::DW9767_Programming(USHORT address,unsigned char *Data,int size)//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page
{
	m_pInterface->I2C_Write(0x18, 0x02, 0x10, I2CMODE_8_8);
	int i=0;
	int group=0;
	int num=0;
	BYTE* CheckData=new BYTE[size]();
	BOOL bRet = TRUE;

	do 
	{	
		msleep(10);
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
		msleep(10);

		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesWrite(0xB0,address+(Count+i*32),2,Data+(Count+i*32),32);
			msleep(10);
		}
		m_pInterface->I2C_MultiBytesWrite(0xB0,address+(Count+i*32),2,Data+(Count+i*32),num);
		msleep(10);
		if(Count>0)
			m_pInterface->I2C_MultiBytesRead(0xB0,address,2,CheckData,Count);

		msleep(10);
		for (i=0;i<group;i++)
		{
			m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,CheckData+(Count+i*32),32);
			msleep(10);
		}
		m_pInterface->I2C_MultiBytesRead(0xB0,address+(Count+i*32),2,CheckData+(Count+i*32),num);
		m_pInterface->I2C_Write(0x18, 0x02, 0x10, I2CMODE_8_8);
		for (i=0;i<size;i++)
		{
			if (CheckData[i]!=Data[i])
			{
				info.Format(_T("DW9767读写对比失败，序号:%d, 读取值:%d, 写入值:%d"), i, CheckData[i], Data[i]);
				m_pInterface->AddLog(info);
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
	msleep(m_Eeprom.msWriteTime);
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
		msleep(m_Eeprom.msWriteTime);
		return ret;
	}

	BYTE *dst = pval;
	int num = length;

	if (reg % m_Eeprom.pageSize != 0)
	{
		int mod = m_Eeprom.pageSize - reg % m_Eeprom.pageSize;

		ret = i2c_multi_write(addr, reg, m_Eeprom.addrByteSize, dst, mod);
		msleep(m_Eeprom.msWriteTime);

		dst += mod;
		reg += mod;

		num = length - mod;
	}

	int cycles = num / m_Eeprom.pageSize;

	for (int i=0; i<cycles; i++)
	{
		ret = i2c_multi_write(addr, reg, m_Eeprom.addrByteSize, dst, m_Eeprom.pageSize);
		msleep(m_Eeprom.msWriteTime);

		reg += m_Eeprom.pageSize;
		dst += m_Eeprom.pageSize;
	}

	int mod = num - cycles * m_Eeprom.pageSize;
	if (mod != 0)
	{		
		ret = i2c_multi_write(addr, reg, m_Eeprom.addrByteSize, dst, mod);
		msleep(m_Eeprom.msWriteTime);		
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

		msleep(I2C_RETRY_WAIT);
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

		msleep(I2C_RETRY_WAIT);
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

		msleep(I2C_RETRY_WAIT);
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

		msleep(I2C_RETRY_WAIT);
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

//		msleep(I2C_RETRY_WAIT);
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

		msleep(I2C_RETRY_WAIT);
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

		msleep(I2C_RETRY_WAIT);
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

		msleep(I2C_RETRY_WAIT);
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
	memset(Applyawbdata,0,7*sizeof(int));
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
	m_sensorotpisempty= FALSE;
	SaveExp = 0;
	//L3
	LockJVer = FALSE;
	FrameCnt = 0;
	CaptureFrame = 0;
	CaptureSize = 3;
	RawArray = new USHORT *[CaptureSize];
	memset(RawArray, 0, CaptureSize* sizeof(USHORT*));
	if (m_PDAFItem == 1)
	{
		InitQualParam();
	}
	return TRUE;
}

BOOL COtpBase::EndProgram(void)
{
	FreeDll();
	if (m_EnPDAF)
	{
		if (m_PDAFItem == 0)
		{
			SAFE_DELETE_ARRAY(m_proc1data);
		}
		else if (m_PDAFItem == 1)
		{
			SaveQualParam();
		}
	}

	if (m_EnLSC)
	{
		if (m_LSCItem == 0)
		{
			SAFE_DELETE_ARRAY(m_mtklscdata);
		}
		else if (m_LSCItem == 1)
		{
			SAFE_DELETE_ARRAY(m_quallscdata);
			//L3
			for (int i=0; i<CaptureSize; i++)
			{
				SAFE_DELETE_ARRAY(RawArray[i]);
			}
			SAFE_DELETE_ARRAY(RawArray);
		}
		else if (m_LSCItem == 2)
		{

		}
	}
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
		if (!m_afread)
		{
			if (!GetAFData()) 	return FALSE;
		}
		else
		{
			if (!GetOTPAF()) 		return FALSE;	
		}
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
	m_pInterface->AddLog(_T("未找到擦除EEPROM函数！"),COLOR_RED);
	return FALSE;
}

BOOL COtpBase :: GetAFData()
{
	m_pInterface ->AddLog(_T("Get AF Data Start"));

	m_infCode = 0, m_macCode = 0,m_stcCode = 0;//init

	vector<double>* vpStcAF = m_pInterface->GetStorage(_T("AutoFocusStart"));
	vector<double>* vpMacAF = m_pInterface->GetStorage(_T("AutoFocusNear"));
	vector<double>* vpInfAF = m_pInterface->GetStorage(_T("AutoFocusFar"));

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
			m_infCode = (USHORT)(vpInfAF->at(0));

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
			m_macCode = (USHORT)(vpMacAF->at(0));

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
			m_stcCode = (USHORT)(vpStcAF->at(0));

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
	m_infCode =220;
	m_macCode = 410;
	m_stcCode = 200;
#endif
	//////////////////////////////////////////////////////////////////////////

	m_pInterface ->AddLog(_T("Get AF Data End"));
	return TRUE;
}

BOOL COtpBase :: GetOTPAF()
{
	m_pInterface ->AddLog(_T("Read OTP AF Data Start"));

	m_infCode = 0, m_macCode = 0,m_stcCode = 0;//init


#ifndef  TestVersion
	if (m_bcheckinf)
	{	
		m_infCode = 180;//otp af add this

		if(m_infCode > m_InfH || m_infCode < m_InfL)
		{
			info.Format(_T("远景Code(%d)超标,请重测远景"), m_infCode);
			m_pInterface->AddLog(info, COLOR_RED);
			return FALSE;
		}
	}


	if (m_bcheckmac)
	{
			m_macCode = 500;//otp af add this

		if (m_macCode > m_MacH || m_macCode < m_MacL)
		{
			info.Format(_T("近景Code(%d)超标,请重测近景"), m_macCode);
			m_pInterface->AddLog(info, COLOR_RED);
			return FALSE;
		}
	}

	if (m_bchecksta)
	{
			m_stcCode = 100;//otp af add this

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
			info.Format(_T("远景Code(%d)近景Code(%d)太接近，请检查OTP内函数"), m_infCode, m_macCode);
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
	m_infCode = 270;
	m_macCode = 540;
#endif
	//////////////////////////////////////////////////////////////////////////

	m_pInterface ->AddLog(_T("Read OTP AF Data End"));
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


BOOL COtpBase::MTK_Ver201_GetProc1(void)
{
	m_pInterface->AddLog(_T("Get MTK PDAF Ver 2.0.1.1023..."));

	CString Path;//Raw
	CString str;

	CString Path2;//QVL Raw
	CString Path3;//QVL Raw2

	//创建保存路径
	FileFolderPath = "";
	Path = m_pInterface->GetExeDir();

	FileFolderPath = Path + _T("PDAF\\");
	CreateDirectory(FileFolderPath, NULL);
	FileFolderPath += _T("MTK\\");
	CreateDirectory(FileFolderPath, NULL);

	if (!m_EnQVL)
	{
		Path =FileFolderPath+_T("\\STEP1.raw");
	}
	else
	{
		FileFolderPath += _T("QVL\\");
		CreateDirectory(FileFolderPath, NULL);
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

	raw_seq = NULL;
	if (fp)
	{
		raw_seq = new unsigned short[width*height];
		fread((void*)raw_seq,2,width*height,fp);//读取Raw图
		fclose(fp);
	}

	typedef PD_ERR_CODE (*PDInit)(char *filename);    
	typedef PD_ERR_CODE (*PDGetsize)(int& max_size);
	typedef PD_ERR_CODE (*PDGetwidthheight)(int& width, int& height);    
	typedef PD_ERR_CODE (*PDProc1)(unsigned short*raw, char *output, int max_output_size, int& output_size);
	typedef PD_ERR_CODE (*PDVerpro1)(unsigned char *eeprom_buf_proc1);    
	typedef PD_ERR_CODE (*PDVerpro2)(unsigned char *eeprom_buf_proc2);
	typedef PD_ERR_CODE (*PDVerpro)(unsigned char *eeprom_buf);    

	CString WorkingPath = GetModulePath()+"\\PDAFDLL\\MTK\\2.0.1.1023";
	SetCurrentDirectory(WorkingPath);
	WorkingPath += "\\pd_cali_dll.dll";
	HINSTANCE hPDAFDll = LoadLibrary(WorkingPath);

	int max_size = 1;
	char* output = NULL; 
	int check_width=0;
	int check_height=0;
	BYTE* Proc2_Read =  new BYTE[m_mtkpdaf.Proc2Size];
	BYTE *PDAF_Data = new BYTE[m_mtkpdaf.TotalSize];
	char *PdafBin = new char[m_mtkpdaf.TotalSize];
	int i = 0;
	BOOL bRet = TRUE;
	do 
	{
		if (NULL == hPDAFDll)
		{
			info.Format(_T("找不到pd_cali_dll.dll，请将此Dll放入程式目录:\n%s！"),WorkingPath);
			m_pInterface->AddLog(info,COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDInit pd_cali_init = (PDInit)GetProcAddress(hPDAFDll, (char *)8);
		if (!pd_cali_init)
		{
			m_pInterface->AddLog(_T("找不到入口函数pd_cali_init！"),COLOR_RED);
			bRet = FALSE;
			break;
		}

		CString settingfilepath = GetModulePath()+"\\PDAFSetting\\MTK_SettingSample\\"+m_MTK_INI_Name;
		if (!PathFileExists(settingfilepath))
		{
			info.Format(_T("找不到文件%s!"),settingfilepath);
			m_pInterface->AddLog(info,COLOR_RED);
			bRet = FALSE;
			break;
		}

		int errornum = pd_cali_init(CT2A(settingfilepath));
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("Setting sample init failed!"), COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDGetsize pd_cali_get_max_output_size = (PDGetsize)GetProcAddress(hPDAFDll,(char *)6);
		errornum = pd_cali_get_max_output_size(max_size);
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("Get MaxSize failed!"), COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDGetwidthheight pd_cali_get_raw_width_height = (PDGetwidthheight)GetProcAddress(hPDAFDll,(char *)7);
		errornum= pd_cali_get_raw_width_height(check_width,check_height);
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("pd_cali_get_raw_width_height运算出错!"), COLOR_RED);
			bRet = FALSE;
			break;
		}

		int output_size = -1;
		output = new char[max_size];
		memset(output,0,max_size); 
		PDProc1 pd_cali_proc1 = (PDProc1)GetProcAddress(hPDAFDll,(char *)9);
		errornum = pd_cali_proc1(raw_seq,output,max_size,output_size);//Step1 运算
		if (errornum!=0)
		{
			if (errornum == 3)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1载入参数错误!"), COLOR_RED);
				bRet = FALSE;
				break;
			}
			if (errornum == 4)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1生成数据size错误!"), COLOR_RED);
				bRet = FALSE;
				break;
			}
			if (errornum == 5)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1之前未进行初始化!"), COLOR_RED);
				bRet = FALSE;
				break;
			}
			if (errornum == 6)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1 Raw图异常!"), COLOR_RED);
				bRet = FALSE;
				break;
			}
			if (errornum == 7)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1生成数据异常!"), COLOR_RED);
				bRet = FALSE;
				break;
			}
			if (errornum == 8)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1 quality check fail!"), COLOR_RED);
				bRet = FALSE;
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

		//////////////////////////////Verify////////////////////////////////////// //
		errornum = pd_cali_init(CT2A(settingfilepath));
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("PDAF Ini Init Fail!"), COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDVerpro1 pd_cali_verify_proc1 = (PDVerpro1)GetProcAddress(hPDAFDll,(char *)12);
		errornum = pd_cali_verify_proc1(m_proc1data);
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("PDAF Proc1 Verify Fail!"), COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDAFSaveProc1Bin(output, output_size);

		if (!DW9763_Read(m_mtkpdaf.Proc2_FirstAddr, Proc2_Read, m_mtkpdaf.Proc2Size))
		{
			m_pInterface->AddLog(_T("PDAF Data Read Fail!"), COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDVerpro2 pd_cali_verify_proc2 = (PDVerpro2)GetProcAddress(hPDAFDll,(char *)13);
		errornum = pd_cali_verify_proc2(Proc2_Read);
		if (errornum != 0)
		{
			info.Format(_T("PDAF Proc2 Verify Fail!Errornum: %d"),errornum);
			m_pInterface->AddLog(info);
			bRet = FALSE;
			break;
		}

		for (i = 0; i < m_mtkpdaf.Proc1Size; i++)
		{
			PDAF_Data[i] = m_proc1data[i];
		}
		for(i = 0; i<m_mtkpdaf.Proc2Size; i++)
		{
			PDAF_Data[i+m_mtkpdaf.Proc1Size] = Proc2_Read[i];
		}

		for (i=0; i<m_mtkpdaf.TotalSize; i++)
		{
			PdafBin[i] = PDAF_Data[i];
		}

		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("PDAF Ini Init Fail!"), COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDVerpro pd_cali_verify = (PDVerpro)GetProcAddress(hPDAFDll,(char *)11);
		errornum = pd_cali_verify(PDAF_Data);
		if (errornum != 0)
		{
			info.Format(_T("PDAF Data Verify Fail!Errornum: %d"),errornum);
			m_pInterface->AddLog(info);
			bRet = FALSE;
			break;
		}
		PDAFSaveBin(PdafBin, m_mtkpdaf.TotalSize);
	} while (0);

	FreeLibrary(hPDAFDll);
	SAFE_DELETE_ARRAY(raw_seq);
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

BOOL COtpBase::MTK_Ver300_GetProc1(void)
{
	m_pInterface->AddLog(_T("Get MTK PDAF Ver 3.0.0.525..."));

	CString Path;//Raw
	CString str;

	CString Path2;//QVL Raw
	CString Path3;//QVL Raw2

	//创建保存路径
	FileFolderPath = "";
	Path = m_pInterface->GetExeDir();

	FileFolderPath = Path + _T("PDAF\\");
	CreateDirectory(FileFolderPath, NULL);
	FileFolderPath += _T("MTK\\");
	CreateDirectory(FileFolderPath, NULL);

	if (!m_EnQVL)
	{
		Path =FileFolderPath+_T("\\STEP1.raw");
	}
	else
	{
		FileFolderPath += _T("QVL\\");
		CreateDirectory(FileFolderPath, NULL);
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

	raw_seq = NULL;
	if (fp)
	{
		raw_seq = new unsigned short[width*height];
		fread((void*)raw_seq,2,width*height,fp);//读取Raw图
		fclose(fp);
	}

	typedef PD_ERR_CODE (*PDInit)(char *filename);    
	typedef PD_ERR_CODE (*PDGetsize)(int& max_size);
	typedef PD_ERR_CODE (*PDGetwidthheight)(int& width, int& height);    
	typedef PD_ERR_CODE (*PDProc1)(unsigned short*raw, char *output, int max_output_size, int& output_size);
	typedef PD_ERR_CODE (*PDVerpro1)(unsigned char *eeprom_buf_proc1);    
	typedef PD_ERR_CODE (*PDVerpro2)(unsigned char *eeprom_buf_proc2);
	typedef PD_ERR_CODE (*PDVerpro)(unsigned char *eeprom_buf);    

	CString WorkingPath = GetModulePath()+"\\PDAFDLL\\MTK\\3.0.0.525";
	SetCurrentDirectory(WorkingPath);
	WorkingPath += "\\pd_cali_dll.dll";
	HINSTANCE hPDAFDll = LoadLibrary(WorkingPath);

	int max_size = 1;
	char* output = NULL; 
	int check_width=0;
	int check_height=0;
	BYTE* Proc2_Read =  new BYTE[m_mtkpdaf.Proc2Size];
	BYTE *PDAF_Data = new BYTE[m_mtkpdaf.TotalSize];
	char *PdafBin = new char[m_mtkpdaf.TotalSize];
	int i = 0;
	BOOL bRet = TRUE;
	do 
	{
		if (NULL == hPDAFDll)
		{
			info.Format(_T("找不到pd_cali_dll.dll，请将此Dll放入程式目录:\n%s！"),WorkingPath);
			m_pInterface->AddLog(info,COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDInit pd_cali_init = (PDInit)GetProcAddress(hPDAFDll, (char *)9);
		if (!pd_cali_init)
		{
			m_pInterface->AddLog(_T("找不到入口函数pd_cali_init！"),COLOR_RED);
			bRet = FALSE;
			break;
		}

		CString settingfilepath = GetModulePath()+"\\PDAFSetting\\MTK_SettingSample\\"+m_MTK_INI_Name;
		if (!PathFileExists(settingfilepath))
		{
			info.Format(_T("找不到文件%s!"),settingfilepath);
			m_pInterface->AddLog(info,COLOR_RED);
			bRet = FALSE;
			break;
		}

		int errornum = pd_cali_init(CT2A(settingfilepath));
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("Setting sample init failed!"), COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDGetsize pd_cali_get_max_output_size = (PDGetsize)GetProcAddress(hPDAFDll,(char *)6);
		errornum = pd_cali_get_max_output_size(max_size);
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("Get MaxSize failed!"), COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDGetwidthheight pd_cali_get_raw_width_height = (PDGetwidthheight)GetProcAddress(hPDAFDll,(char *)7);
		errornum= pd_cali_get_raw_width_height(check_width,check_height);
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("pd_cali_get_raw_width_height运算出错!"), COLOR_RED);
			bRet = FALSE;
			break;
		}

		int output_size = -1;
		output = new char[max_size];
		memset(output,0,max_size); 
		PDProc1 pd_cali_proc1 = (PDProc1)GetProcAddress(hPDAFDll,(char *)10);
		errornum = pd_cali_proc1(raw_seq,output,max_size,output_size);//Step1 运算
		if (errornum!=0)
		{
			if (errornum == 3)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1载入参数错误!"), COLOR_RED);
				bRet = FALSE;
				break;
			}
			if (errornum == 4)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1生成数据size错误!"), COLOR_RED);
				bRet = FALSE;
				break;
			}
			if (errornum == 5)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1之前未进行初始化!"), COLOR_RED);
				bRet = FALSE;
				break;
			}
			if (errornum == 6)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1 Raw图异常!"), COLOR_RED);
				bRet = FALSE;
				break;
			}
			if (errornum == 7)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1生成数据异常!"), COLOR_RED);
				bRet = FALSE;
				break;
			}
			if (errornum == 8)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1 quality check fail!"), COLOR_RED);
				bRet = FALSE;
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

		//////////////////////////////Verify////////////////////////////////////// //
		errornum = pd_cali_init(CT2A(settingfilepath));
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("PDAF Ini Init Fail!"), COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDVerpro1 pd_cali_verify_proc1 = (PDVerpro1)GetProcAddress(hPDAFDll,(char *)14);
		errornum = pd_cali_verify_proc1(m_proc1data);
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("PDAF Proc1 Verify Fail!"), COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDAFSaveProc1Bin(output, output_size);

		if (!FM24C64_Read(m_mtkpdaf.Proc2_FirstAddr, Proc2_Read, m_mtkpdaf.Proc2Size))//!DW9763_Read(m_mtkpdaf.Proc2_FirstAddr, Proc2_Read, m_mtkpdaf.Proc2Size)
		{
			m_pInterface->AddLog(_T("PDAF Data Read Fail!"), COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDVerpro2 pd_cali_verify_proc2 = (PDVerpro2)GetProcAddress(hPDAFDll,(char *)15);
		errornum = pd_cali_verify_proc2(Proc2_Read);
		if (errornum != 0)
		{
			info.Format(_T("PDAF Proc2 Verify Fail!Errornum: %d"),errornum);
			m_pInterface->AddLog(info);
			bRet = FALSE;
			break;
		}

		for (i = 0; i < m_mtkpdaf.Proc1Size; i++)
		{
			PDAF_Data[i] = m_proc1data[i];
		}
		for(i = 0; i<m_mtkpdaf.Proc2Size; i++)
		{
			PDAF_Data[i+m_mtkpdaf.Proc1Size] = Proc2_Read[i];
		}

		for (i=0; i<m_mtkpdaf.TotalSize; i++)
		{
			PdafBin[i] = PDAF_Data[i];
		}

		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("PDAF Ini Init Fail!"), COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDVerpro pd_cali_verify = (PDVerpro)GetProcAddress(hPDAFDll,(char *)13);
		errornum = pd_cali_verify(PDAF_Data);
		if (errornum != 0)
		{
			info.Format(_T("PDAF Data Verify Fail!Errornum: %d"),errornum);
			m_pInterface->AddLog(info);
			bRet = FALSE;
			break;
		}
		PDAFSaveBin(PdafBin, m_mtkpdaf.TotalSize);
	} while (0);

	FreeLibrary(hPDAFDll);
	SAFE_DELETE_ARRAY(raw_seq);
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

BOOL COtpBase::MTK_Ver301_GetProc1(void)
{
	m_pInterface->AddLog(_T("Get MTK PDAF Ver 3.0.1.0322..."));

	CString Path;//Raw
	CString str;

	CString Path2;//QVL Raw
	CString Path3;//QVL Raw2

	//创建保存路径
	FileFolderPath = "";
	Path = m_pInterface->GetExeDir();

	FileFolderPath = Path + _T("PDAF\\");
	CreateDirectory(FileFolderPath, NULL);
	FileFolderPath += _T("MTK\\");
	CreateDirectory(FileFolderPath, NULL);

	if (!m_EnQVL)
	{
		Path =FileFolderPath+_T("\\STEP1.raw");
	}
	else
	{
		FileFolderPath += _T("QVL\\");
		CreateDirectory(FileFolderPath, NULL);
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

	raw_seq = NULL;
	if (fp)
	{
		raw_seq = new unsigned short[width*height];
		fread((void*)raw_seq,2,width*height,fp);//读取Raw图
		fclose(fp);
	}

	typedef PD_ERR_CODE (*PDInit)(char *filename);    
	typedef PD_ERR_CODE (*PDGetsize)(int& max_size);
	typedef PD_ERR_CODE (*PDGetwidthheight)(int& width, int& height);    
	typedef PD_ERR_CODE (*PDProc1)(unsigned short*raw, char *output, int max_output_size, int& output_size);
	typedef PD_ERR_CODE (*PDVerpro1)(unsigned char *eeprom_buf_proc1);    
	typedef PD_ERR_CODE (*PDVerpro2)(unsigned char *eeprom_buf_proc2);
	typedef PD_ERR_CODE (*PDVerpro)(unsigned char *eeprom_buf);    

	CString WorkingPath = GetModulePath()+"\\PDAFDLL\\MTK\\3.0.1.0322";
	SetCurrentDirectory(WorkingPath);
	WorkingPath += "\\pd_cali_dll.dll";
	HINSTANCE hPDAFDll = LoadLibrary(WorkingPath);

	int max_size = 1;
	char* output = NULL; 
	int check_width=0;
	int check_height=0;
	BYTE* Proc2_Read =  new BYTE[m_mtkpdaf.Proc2Size];
	BYTE *PDAF_Data = new BYTE[m_mtkpdaf.TotalSize];
	char *PdafBin = new char[m_mtkpdaf.TotalSize];
	int i = 0;
	BOOL bRet = TRUE;
	do 
	{
		if (NULL == hPDAFDll)
		{
			info.Format(_T("找不到pd_cali_dll.dll，请将此Dll放入程式目录:\n%s！"),WorkingPath);
			m_pInterface->AddLog(info,COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDInit pd_cali_init = (PDInit)GetProcAddress(hPDAFDll, (char *)9);
		if (!pd_cali_init)
		{
			m_pInterface->AddLog(_T("找不到入口函数pd_cali_init！"),COLOR_RED);
			bRet = FALSE;
			break;
		}

		CString settingfilepath = GetModulePath()+"\\PDAFSetting\\MTK_SettingSample\\"+m_MTK_INI_Name;
		if (!PathFileExists(settingfilepath))
		{
			info.Format(_T("找不到文件%s!"),settingfilepath);
			m_pInterface->AddLog(info,COLOR_RED);
			bRet = FALSE;
			break;
		}

		int errornum = pd_cali_init(CT2A(settingfilepath));
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("Setting sample init failed!"), COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDGetsize pd_cali_get_max_output_size = (PDGetsize)GetProcAddress(hPDAFDll,(char *)6);
		errornum = pd_cali_get_max_output_size(max_size);
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("Get MaxSize failed!"), COLOR_RED);
			bRet = FALSE;
			break;
		}
		PDGetwidthheight pd_cali_get_raw_width_height = (PDGetwidthheight)GetProcAddress(hPDAFDll,(char *)7);
		errornum= pd_cali_get_raw_width_height(check_width,check_height);
		if (errornum != 0)
		{
			m_pInterface->AddLog(_T("pd_cali_get_raw_width_height运算出错!"), COLOR_RED);
			bRet = FALSE;
			break;
		}

		int output_size = -1;
		output = new char[max_size];
		memset(output,0,max_size); 
		PDProc1 pd_cali_proc1 = (PDProc1)GetProcAddress(hPDAFDll,(char *)10);
		errornum = pd_cali_proc1(raw_seq,output,max_size,output_size);//Step1 运算
		if (errornum!=0)
		{
			if (errornum == 3)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1载入参数错误!"), COLOR_RED);
				bRet = FALSE;
				break;
			}
			if (errornum == 4)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1生成数据size错误!"), COLOR_RED);
				bRet = FALSE;
				break;
			}
			if (errornum == 5)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1之前未进行初始化!"), COLOR_RED);
				bRet = FALSE;
				break;
			}
			if (errornum == 6)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1 Raw图异常!"), COLOR_RED);
				bRet = FALSE;
				break;
			}
			if (errornum == 7)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1生成数据异常!"), COLOR_RED);
				bRet = FALSE;
				break;
			}
			if (errornum == 8)
			{
				m_pInterface->AddLog(_T("pd_cali_proc1 quality check fail!"), COLOR_RED);
				bRet = FALSE;
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

		//////////////////////////////Verify////////////////////////////////////// //
// 		errornum = pd_cali_init(CT2A(settingfilepath));
// 		if (errornum != 0)
// 		{
// 			m_pInterface->AddLog(_T("PDAF Ini Init Fail!"), COLOR_RED);
// 			bRet = FALSE;
// 			break;
// 		}
// 		PDVerpro1 pd_cali_verify_proc1 = (PDVerpro1)GetProcAddress(hPDAFDll,(char *)14);
// 		errornum = pd_cali_verify_proc1(m_proc1data);
// 		if (errornum != 0)
// 		{
// 			m_pInterface->AddLog(_T("PDAF Proc1 Verify Fail!"), COLOR_RED);
// 			bRet = FALSE;
// 			break;
// 		}
// 		PDAFSaveProc1Bin(output, output_size);
// 
// 		if (!GT24C64_Read(m_mtkpdaf.Proc2_FirstAddr, Proc2_Read, m_mtkpdaf.Proc2Size))//!DW9763_Read(m_mtkpdaf.Proc2_FirstAddr, Proc2_Read, m_mtkpdaf.Proc2Size)
// 		{
// 			m_pInterface->AddLog(_T("PDAF Data Read Fail!"), COLOR_RED);
// 			bRet = FALSE;
// 			break;
// 		}
// 		PDVerpro2 pd_cali_verify_proc2 = (PDVerpro2)GetProcAddress(hPDAFDll,(char *)15);
// 		errornum = pd_cali_verify_proc2(Proc2_Read);
// 		if (errornum != 0)
// 		{
// 			info.Format(_T("PDAF Proc2 Verify Fail!Errornum: %d"),errornum);
// 			m_pInterface->AddLog(info);
// 			bRet = FALSE;
// 			break;
// 		}

		for (i = 0; i < m_mtkpdaf.Proc1Size; i++)
		{
			PDAF_Data[i] = m_proc1data[i];
		}
		for(i = 0; i<m_mtkpdaf.Proc2Size; i++)
		{
			PDAF_Data[i+m_mtkpdaf.Proc1Size] = Proc2_Read[i];
		}

		for (i=0; i<m_mtkpdaf.TotalSize; i++)
		{
			PdafBin[i] = PDAF_Data[i];
		}

// 		if (errornum != 0)
// 		{
// 			m_pInterface->AddLog(_T("PDAF Ini Init Fail!"), COLOR_RED);
// 			bRet = FALSE;
// 			break;
// 		}
// 		PDVerpro pd_cali_verify = (PDVerpro)GetProcAddress(hPDAFDll,(char *)13);
// 		errornum = pd_cali_verify(PDAF_Data);
// 		if (errornum != 0)
// 		{
// 			info.Format(_T("PDAF Data Verify Fail!Errornum: %d"),errornum);
// 			m_pInterface->AddLog(info);
// 			bRet = FALSE;
// 			break;
// 		}
// 		PDAFSaveBin(PdafBin, m_mtkpdaf.TotalSize);
	} while (0);

	FreeLibrary(hPDAFDll);
	SAFE_DELETE_ARRAY(raw_seq);
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

int COtpBase::PDAF_Proc1(void)
{
	if (m_EnPDAF)
	{
		if (m_PDAFItem == 0)//MTK
		{
			if (m_MTKPDAFitem == 0)//2.0.1.1023
			{
				if (!MTK_Ver201_GetProc1()) 				return 1;
			}
			else if (m_MTKPDAFitem == 1)//3.0.0.525
			{
				if (!MTK_Ver300_GetProc1()) 				return 1;
			}
			else if (m_MTKPDAFitem == 2)
			{
				if (!MTK_Ver301_GetProc1()) 				return 1;
			}
		}
		//Qualcomm
		if (m_PDAFItem == 1)
		{
			if (m_QUALPDAFitem == 0)//J
			{
				if (!Qual_GainMapCal_J()) 				return 1;
			}
			else if (m_QUALPDAFitem == 1)//L3
			{
				return Qual_GainMapCal_L3();
			}
			else if (m_QUALPDAFitem == 2)//L4
			{
				return Qual_GainMapCal_L4();
			}
			else if (m_QUALPDAFitem == 3)//L4
			{
				return Qual_GainMapCal_L5();
			}
// 			if (!LockJVer) //先做J再做L3用的  例如7012
// 			{
// 				if (!Qual_GainMapCal())//Qual_GainMapCal_L3()
// 				{
// 					return 1;
// 				}
// 				else
// 				{
// 					LockJVer = TRUE;
// 				}
// 			}
		}
		//Sensor
		if (m_PDAFItem == 2)
		{
			;//待添加
		}
	}
	return 0;
}

BOOL COtpBase::GetLSC(void)
{
	if (m_EnLSC)
	{
		if (m_LSCItem == 0)
		{
			if (m_MTKLSCVerItem == 0)
			{
				m_MTKLSCBasePath =GetModulePath()+L"\\LSC\\MTK\\14";
			}
			else if (m_MTKLSCVerItem == 1)
			{
				m_MTKLSCBasePath =GetModulePath()+L"\\LSC\\MTK\\15";
			}
			else if (m_MTKLSCVerItem == 2)
			{
				m_MTKLSCBasePath =GetModulePath()+L"\\LSC\\MTK\\16RGB";
			}
			else if (m_MTKLSCVerItem == 3)
			{
				m_MTKLSCBasePath =GetModulePath()+L"\\LSC\\MTK\\16MONO";
			}

			if (!MTK_GetLSC())	return FALSE; 
		}
		else if (m_LSCItem == 1)
		{
			m_QualLSCBasePath = GetModulePath()+ _T("\\LSC\\Qual\\");
			if (m_QUALLSCVerItem == 0)//P
			{
				if (!Qual_GetLSC_P()) 				return FALSE;
			}
			else if (m_QUALLSCVerItem == 1)//R
			{
				if (!Qual_GetLSC_R()) 				return FALSE;
			}
		}
		else if (m_LSCItem == 2)
		{
			if (m_sensoritem == 0)//3l8
			{
				if (!S5K3L8_GetLSC())	return FALSE;
			}
			else if (m_sensoritem == 1)//5E8
			{
				if (!S5K5E8_GetLSC())	return FALSE;
			}
			else if (m_sensoritem == 2)//4H8
			{
				if (!S5K4H8_GetLSC())	return FALSE;
			}
			else if (m_sensoritem == 3)//Hi1332
			{
				if (!HI1332_GetLSC())	return FALSE;
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

void COtpBase::Raw10toRaw8(USHORT *Src, BYTE *Dest, long number)//Raw10
{
	long DesCount = 0;

	for (long i = 0; i < number; i++ )//i = i + 5
	{
		Dest[i] = ((Src[i]>>2) & 0xff);
	}
}

BOOL COtpBase :: Raw8ToBMP(CString sfilename, BYTE *pSrcBuffer, UINT width, UINT height, UINT RawFormat)
{
	//Bayer Conversion Flip
	BYTE *pDstBuffer = new BYTE[width*height*3];

	UINT x = 0; 
	UINT y = 0;
	UINT i = 0;

	switch( RawFormat )
	{
	case 0: //RGGB
		{
			//////////////////////////////			
			///Corner case;
			x = 0;
			y = 0;
			pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[(height-1)*width+1]; //B
			pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[ (height-2)*width+1] + pSrcBuffer[(height-1)*width])/2; //G
			pDstBuffer[ y * width * 3 + x * 3 + 2 ] = pSrcBuffer[ (height-2)*width]; // R

			x = width - 1;
			y = 0;
			pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[ height*width - 1 ]; 
			pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[height*width -2] + pSrcBuffer[(height-1)*width-1] )/2;
			pDstBuffer[ y * width * 3 + x * 3 + 2 ] = pSrcBuffer[(height-1)*width - 2];

			x = 0;
			y = height - 1;
			pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[width+1];
			pDstBuffer[ y * width * 3 + x * 3 + 1 ] = (pSrcBuffer[1] + pSrcBuffer[width] )/2;
			pDstBuffer[ y * width * 3 + x * 3 + 2 ] = pSrcBuffer[0];

			x = width - 1;
			y = height - 1;
			pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[width*2-1];
			pDstBuffer[ y * width * 3 + x * 3 + 1 ]  = ( pSrcBuffer[width*2 -2] + pSrcBuffer[width -1 ] )/2;
			pDstBuffer[ y * width * 3 + x * 3 + 2 ] = pSrcBuffer[width-2];

			//for boundary
			for( x = 1; x < width -1; x++ )
			{ 	
				if( x%2 == 1 )
				{
					y = 0;
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[(height-1)*width+x]; //B
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[(height-1)*width+x-1] + pSrcBuffer[(height-1)*width+x+1] + pSrcBuffer[(height-2)*width+x] )/3; //G
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] = ( pSrcBuffer[(height-2)*width+x-1] +  pSrcBuffer[(height-2)*width+x+1] )/2; //R

					y = height -1;
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[width+x];
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[x] + pSrcBuffer[width+x-1] + pSrcBuffer[width+x+1] ) /3 ;
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] = ( pSrcBuffer[x-1] + pSrcBuffer[x+1] )/2; 
				}
				else
				{
					y = 0;
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = ( pSrcBuffer[(height-1)*width+x-1] + pSrcBuffer[(height-1)*width+x+1] )/2; //B
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[(height-1)*width+x] + pSrcBuffer[(height-2)*width+x-1] + pSrcBuffer[(height-2)*width + x +1])/3; //G //Here
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[(height-2)*width+x];	//R

					y = height -1;
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[width+x-1] + pSrcBuffer[width+x+1] )/2;
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[width+x] + pSrcBuffer[x-1] + pSrcBuffer[x+1] )/3 ;
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] = pSrcBuffer[x] ; 

				} 
			}

			for( y = 1;  y < height - 1; y++ )
			{
				if( y%2 == 1 )
				{
					x = 0;
					i = ( height -1 - y )*width;
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = ( pSrcBuffer[i-width+1] + pSrcBuffer[i+width+1] )/2;
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i+1] )/3;
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] = pSrcBuffer[i];


					x = width -1;
					i = ( height - 1 - y)* width + x;
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i+width-1])/3;  //here 
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] = pSrcBuffer[i-1];  
				}
				else
				{
					x = 0;                                             
					i = ( height -1 - y )*width;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[i+1];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[i] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width+1])/3;          
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;         


					x = width -1;                                      
					i = ( height - 1 - y)* width + x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[i];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] )/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i+width-1] )/2;  //Here

				}
			}


			//other area
			for( y = 1; y < height-1; y++ )
			{
				for( x=1; x < width-1; x++ )
				{
					i = (height-1-y)*width + x;

					if( y%2 == 1 )
					{
						if( x%2 == 1 ) //G1
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 1] =  ( pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 2] =  (pSrcBuffer[i-1] + pSrcBuffer[i+1])/2;//Here

						}
						else  //R
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] ) / 4;
							pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] + pSrcBuffer[i+width]) / 4;
							pDstBuffer[y * width * 3 + x * 3 + 2] =  pSrcBuffer[i]; 
						}
					}
					else
					{
						if( x%2  == 1)  //B
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i];
							pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] ) /4;//Here
							pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1]) /4;
						}
						else  //G2
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 2] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
						}

					}
				}
			}
		}
		break;

	case 1: //GRBG
		{ 
			//corner case
			x = 0;
			y = 0;
			i = (height-1)*width;
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i]; //B
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i+1] )/2; //G
			pDstBuffer[y * width * 3 + x * 3 + 2] =   pSrcBuffer[i-width+1]; 

			x = width -1;
			y = 0;			 
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[height*width-2];
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[(height-1)*width-2] + pSrcBuffer[height*width-1] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] = pSrcBuffer[(height-1)*width-1];

			x = 0;   
			y = height -1;
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[width];
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[0] + pSrcBuffer[width+1] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] = pSrcBuffer[1];

			x = width - 1;
			y = height - 1;
			pDstBuffer[y * width * 3 + x * 3 + 0] =  pSrcBuffer[width*2-2];
			pDstBuffer[y * width * 3 + x * 3 + 1] =  ( pSrcBuffer[width*2 -1] +  pSrcBuffer[width-2] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] =  pSrcBuffer[width -1];


			//for boundary case
			for( x = 1; x < width -1; x++ )
			{
				if( x%2 == 1 )
				{
					y = 0;                                             
					i = ( height -1  )*width + x;  //G                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = (  pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] )/3;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i-width]; 


					y = height - 1;                                      
					i = x;       //R               
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] )/2;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+width] + pSrcBuffer[i+1])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i];    

				}
				else
				{
					y = 0;                                             
					i = ( height -1  )*width + x;  //B                     
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  (  pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1])/3 ;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] )/2; 


					y = height - 1;                                      
					i = x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i+width] ;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] )/3 ;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;    
				}
			}

			for( y = 1; y < height - 1; y++ )
			{
				if( y%2 == 1 )
				{
					x = 0;                                             
					i = ( height -1 - y )*width;  //G                     
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = (  pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width+1] )/3;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i+1]; 


					x = width -1;                                      
					i = ( height - 1 - y)* width + x; //R                     
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i-width-1] + pSrcBuffer[i+width-1] )/2;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i];   

				}
				else
				{
					x = 0;                                             
					i = ( height -1 - y )*width;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  (  pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i+1])/3 ;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-width+1] + pSrcBuffer[i+width+1] )/2; 


					x = width -1;                                      
					i = ( height - 1 - y)* width + x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i-1] ;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = (  pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i+width-1])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;  

				}
			}

			//other area
			for( y = 1; y < height -1; y++ )
			{
				for( x = 1; x < width -1; x++ )
				{
					i = ( height -1 -y)*width + x;

					if( y%2 == 1 )
					{
						if( x%2 == 1 ) //R
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] )/4;
							pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1])/4;
							pDstBuffer[y * width * 3 + x * 3 + 2] =  pSrcBuffer[i];

						}
						else     //G1
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-1] + pSrcBuffer[i+1])/2; 
						}
					}
					else
					{
						if( x%2 == 1 ) //G2
						{   
							pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-1] + pSrcBuffer[i+1])/2;     
							pDstBuffer[ y * width * 3 + x * 3 + 1 ] =    ( pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;  
							pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width])/2;

						}
						else //B
						{
							pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   pSrcBuffer[i];     
							pDstBuffer[ y * width * 3 + x * 3 + 1 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] )/4;  
							pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] )/4; 
						}

					}
				}

			} 
		}
		break;

	case 2:   //GBRG
		{
			//corner case
			x = 0;
			y = 0;
			i = (height-1)*width;
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i-width+1]; //B
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i+1] )/2; //G
			pDstBuffer[y * width * 3 + x * 3 + 2] =   pSrcBuffer[i]; //R

			x = width -1;
			y = 0;			 
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[(height - 1)*width-1];
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[(height-1)*width-2] + pSrcBuffer[height*width-1] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] = pSrcBuffer[ height*width-2];

			x = 0;   
			y = height -1;
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[1];
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[0] + pSrcBuffer[width +1 ] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] = pSrcBuffer[width];

			x = width -1;
			y = height -1;
			pDstBuffer[y * width * 3 + x * 3 + 0] =  pSrcBuffer[width-1];
			pDstBuffer[y * width * 3 + x * 3 + 1] =  ( pSrcBuffer[width-2] +  pSrcBuffer[width*2-1] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] =  pSrcBuffer[width*2 -2];


			//for boundary case
			for( x = 1; x < width -1; x++ )
			{
				if( x%2 == 1 )
				{
					y = 0;                                             
					i = ( height -1  )*width + x;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[i-width];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i] )/3;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2; 


					y = height - 1;                                      
					i = x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i];          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] + pSrcBuffer[i+width])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] )/2;    

				}
				else
				{
					y = 0;                                             
					i = ( height -1  )*width + x;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] )/2;      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  (  pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1])/3 ;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i]; 


					y = height - 1;                                      
					i = x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1]  )/2 ;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] )/3 ;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i+width];    
				}
			}

			for( y = 1; y < height - 1; y++ )
			{
				if( y%2 == 1 )
				{
					x = 0;                                             
					i = ( height -1 - y )*width;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i+1];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i] + pSrcBuffer[i-width+1]+ pSrcBuffer[i+width+1])/3;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width])/2;//Here 


					x = width -1;                                      
					i = ( height - 1 - y)* width + x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i];          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+width])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-width-1] + pSrcBuffer[i+width-1] )/2;   

				}
				else
				{
					x = 0;                                             
					i = ( height -1 - y )*width;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i-width+1] + pSrcBuffer[i+width+1] )/2;      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  (  pSrcBuffer[i-width] + pSrcBuffer[i+1] + pSrcBuffer[i+width])/3 ;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i]; 


					x = width -1;                                      
					i = ( height - 1 - y)* width + x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2 ;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = (  pSrcBuffer[i-width-1] + pSrcBuffer[i] + pSrcBuffer[i+width-1])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i-1];  

				}
			}

			//other area
			for( y = 1; y < height -1; y++ )
			{
				for( x = 1; x < width -1; x++ )
				{
					i = ( height -1 -y)*width + x;

					if( y%2 == 1 )
					{
						if( x%2 == 1 )  
						{
							//B
							pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i];
							pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] + pSrcBuffer[i+width])/4;
							pDstBuffer[y * width * 3 + x * 3 + 2] = (pSrcBuffer[i-width-1] +pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/4;

						}
						else      
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-width] +  + pSrcBuffer[i+width])/2; 
						}
					}
					else
					{
						if( x%2 == 1 )  
						{   
							pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width])/2;     
							pDstBuffer[ y * width * 3 + x * 3 + 1 ] =     ( pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;  
							pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;

						}
						else  
						{
							pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-width-1] +pSrcBuffer[i-width+1]+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/4;     
							pDstBuffer[ y * width * 3 + x * 3 + 1 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width]   + pSrcBuffer[i-1] + pSrcBuffer[i+1] )/4;  
							pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   pSrcBuffer[i]; 
						}

					}
				}

			}

		}
		break;



	case 3:  //BGGR		
		//////////////////////////////////////
		{
			//corner case
			x = 0;
			y = 0;
			i = (height-1)*width;
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i-width]; //B
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i] + pSrcBuffer[i-width+1] )/2;  
			pDstBuffer[y * width * 3 + x * 3 + 2] =   pSrcBuffer[i+1]; //R

			x = width -1;
			y = 0;			 
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[(height - 1)*width-2];
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[height*width-2] + pSrcBuffer[(height-1)*width-1] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] = pSrcBuffer[ height*width-1];

			x = 0;   
			y = height -1;
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[0];
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[1] + pSrcBuffer[width] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] = pSrcBuffer[width+1];

			x = width -1;
			y = height -1;
			pDstBuffer[y * width * 3 + x * 3 + 0] =  pSrcBuffer[width-2];
			pDstBuffer[y * width * 3 + x * 3 + 1] =  ( pSrcBuffer[width-1] +  pSrcBuffer[width*2-2] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] =  pSrcBuffer[width*2 -1];


			//for boundary case
			for( x = 1; x < width -1; x++ )
			{
				if( x%2 == 1 )
				{
					y = 0;                                             
					i = ( height -1  )*width + x;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = (  pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] )/2;      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] )/3;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i]; 


					y = height - 1;                                      
					i = x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i+width];    

				}
				else
				{
					y = 0;                                             
					i = ( height -1  )*width + x;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i-width];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  (  pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i])/3 ;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2; 


					y = height - 1;                                      
					i = x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i] ;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] + pSrcBuffer[i+width] )/3 ;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] )/2;    
				}
			}

			for( y = 1; y < height - 1; y++ )
			{
				if( y%2 == 1 )
				{
					x = 0;                                             
					i = ( height -1 - y )*width;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[i];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+1] + pSrcBuffer[i+width] )/3;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  (pSrcBuffer[i-width+1] + pSrcBuffer[i+width+1])/2; 


					x = width -1;                                      
					i = ( height - 1 - y)* width + x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i-1];          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-width-1] + pSrcBuffer[i] + pSrcBuffer[i+width-1])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;   

				}
				else
				{
					x = 0;                                             
					i = ( height -1 - y )*width;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  (  pSrcBuffer[i-width+1] + pSrcBuffer[i] + pSrcBuffer[i+width+1])/3 ;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i+1]; 


					x = width -1;                                      
					i = ( height - 1 - y)* width + x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i-width-1] + pSrcBuffer[i+width-1] )/2 ;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = (  pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+width])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i];  

				}
			}

			//other area
			for( y = 1; y < height -1; y++ )
			{
				for( x = 1; x < width -1; x++ )
				{
					i = ( height -1 -y)*width + x;

					if( y%2 == 1 )
					{
						if( x%2 == 1 )  
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 2] = (pSrcBuffer[i-width] + pSrcBuffer[i+width])/2;

						}
						else      
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i];
							pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] + pSrcBuffer[i+width ] )/4;
							pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/4; 
						}
					}
					else
					{
						if( x%2 == 1 )  
						{   
							pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/4;     
							pDstBuffer[ y * width * 3 + x * 3 + 1 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] )/4;  
							pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   pSrcBuffer[i];

						}
						else  
						{
							pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;     
							pDstBuffer[ y * width * 3 + x * 3 + 1 ] =   ( pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;  
							pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2; 
						}

					}
				}

			} 

		}
		break;
	} 

	int OffBits;
	HFILE bmpFile;
	BITMAPFILEHEADER bmpHeader;// Header for Bitmap file
	BITMAPINFO bmpInfo;

	OffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	DWORD dwSizeImage = width * height *3;//IMAGESIZE_X * IMAGESIZE_Y *3

	//VeriFlipBuf(pBuffer,width * 3, height);//to correct the vertical flip problem.
	bmpHeader.bfType = ((WORD)('M'<<8)|'B');
	bmpHeader.bfSize = OffBits + dwSizeImage;
	bmpHeader.bfReserved1 = 0;
	bmpHeader.bfReserved2 = 0;
	bmpHeader.bfOffBits = OffBits;

	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = width;
	bmpInfo.bmiHeader.biHeight = height;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biSizeImage = 0;
	bmpInfo.bmiHeader.biXPelsPerMeter = 0;
	bmpInfo.bmiHeader.biYPelsPerMeter = 0;
	bmpInfo.bmiHeader.biClrUsed = 0;
	bmpInfo.bmiHeader.biClrImportant = 0;

	bmpFile = _lcreat(CT2A(sfilename), FALSE);

	if (bmpFile < 0)
	{
		return FALSE;
	}

	UINT len = 0;
	len = _lwrite(bmpFile, (LPSTR)&bmpHeader, sizeof(BITMAPFILEHEADER));
	len = _lwrite(bmpFile,(LPSTR)&bmpInfo, sizeof(BITMAPINFOHEADER));
	len = _lwrite(bmpFile, (LPSTR)pDstBuffer,bmpHeader.bfSize - sizeof(bmpHeader)- sizeof(bmpInfo)+4);//4

	SAFE_DELETE_ARRAY(pDstBuffer);
	_lclose(bmpFile);

	return TRUE;
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

	m_szStr.Format(_T("R:%d Gr:%d Gb:%d B:%d"),Qual_wb_r,Qual_wb_gr,Qual_wb_gb,Qual_wb_b);
	m_pInterface->AddLog(m_szStr, COLOR_BLUE);
	m_szStr.Format(_T("R/G: %d B/G: %d G/G:%d"), wb->rg, wb->bg,wb->gb_gr);
	m_pInterface->AddLog(m_szStr, COLOR_BLUE);
	return TRUE;
}

BOOL COtpBase::SaveAwbData(tagAwbRatio* wb,int type)
{
	CTime time =  CTime::GetCurrentTime();
	CString DirPath = _T("D:\\Report\\");
	CString Path ;
	if (type == 0)
	{
		Path.Format(_T("AWB Data Befor Apply_%04d%02d%02d.xls"),time.GetYear(), time.GetMonth(), time.GetDay());
	}
	else if (type == 1)
	{
		Path.Format(_T("AWB Data After Apply_%04d%02d%02d.xls"),time.GetYear(), time.GetMonth(), time.GetDay());
	}
	else if (type == 2)
	{
		Path.Format(_T("AWB Data in OTP_%04d%02d%02d.xls"),time.GetYear(), time.GetMonth(), time.GetDay());
	}
	Path = DirPath+Path;

	FILE *fp = NULL;
	if(_wfreopen_s(&fp,Path,_T("at"),stdout))  //
	{
		info.Format(_T("无法打开文件%s"),Path);
		m_pInterface->AddLog(info,COLOR_RED);
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	long filesize = ftell(fp);
	if (filesize == 0)
	{
		printf("NUM\tTest Time\tR\tGr\tGb\tB\tRGain\tBGain\tGGain\tGoldenRG\tGoldenBG");
	}
	printf("\n");

	printf("%d\t", m_pInterface->GetModuleNum());
	printf("%02d:%02d:%02d\t", time.GetHour(), time.GetMinute(), time.GetSecond());

	printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", wb->r,wb->gr,wb->gb,wb->b,wb->rg,wb->bg,wb->gb_gr,m_wbGolden.rg,m_wbGolden.bg);

	fclose(fp);
	info = Path + _T(" saved");
	m_pInterface->AddLog(info);
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

CString COtpBase::GetModulePath()
{
	CString myStr;
	TCHAR ModulePath[MAX_PATH] = {0};
	GetModuleFileNameW(NULL, ModulePath, MAX_PATH);//return  real  lenghth
	PathRemoveFileSpec(ModulePath);
	myStr.Format(ModulePath);
	return myStr;
}

BOOL COtpBase::MTK_GetLSC(void)
{
	m_pInterface->AddLog(_T("Get MTK LSC Data Start！"));

	int width = m_pInterface->GetImage().width;
	int height = m_pInterface->GetImage().height;

	info.Format(L"\\Capture_%dx%d.raw",width,height);
	CString sfilename =m_MTKLSCBasePath+info;

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



	BYTE *raw8buff = new BYTE [width*height];
	memset(raw8buff, 0, width*height);

	Raw10toRaw8(m_pInterface->GetImage().Raw_Buf,raw8buff,width*height);

#ifndef  TestVersion
	SaveCurrentRaw8(sfilename,raw8buff,width, height);//保存Raw8图
#endif

	SAFE_DELETE_ARRAY(raw8buff);

	m_mtklscdata = new BYTE[SIZE_MTK_LSC];
	memset(m_mtklscdata, 0 ,sizeof(m_mtklscdata));

	info.Format(_T("Taskkill /IM %s\\1_ShadingCalibrationSample.exe"),m_MTKLSCBasePath);
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

		info.Format(_T("Taskkill /IM %s\\2_ShadingCorrectionSample.exe"),m_MTKLSCBasePath);
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
	SetCurrentDirectory(m_MTKLSCBasePath);

	int width = m_pInterface->GetImage().width;
	int height = m_pInterface->GetImage().height;

	info.Format(L"\\Capture_%dx%d.raw",width,height);
	CString capture_Image =m_MTKLSCBasePath+info;

	CString slim_param_capture=m_MTKLSCBasePath+"\\"+"slim_param_capture.txt";
	CString ShadingCalibration_Slim;
	if (m_MTKLSCVerItem == 0 ||m_MTKLSCVerItem == 1 )
	{
		ShadingCalibration_Slim=m_MTKLSCBasePath+L"\\1_ShadingCalibrationSample.exe";
	}
	else if (m_MTKLSCVerItem == 2 || m_MTKLSCVerItem == 3)
	{
		ShadingCalibration_Slim=m_MTKLSCBasePath+L"\\ShadingCalibrationSample.exe";
	}

	CString ShadingCalibrate=m_MTKLSCBasePath+"\\"+"ShadingCalibrate.dll";
	CString eeprom_slim=m_MTKLSCBasePath+"\\"+"eeprom_slim.txt";

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
		msleep(100);
		Timeout--;
	}

	return TRUE;
}

BOOL COtpBase::MTK_ApplyLSC()
{
	SetCurrentDirectory(m_MTKLSCBasePath);
	CString eeprom_slim=m_MTKLSCBasePath+"\\"+"eeprom_slim.txt";
	CString after_ob=m_MTKLSCBasePath+"\\"+"after_ob.raw";
	CString ShadingCorrectionSample;
	if (m_MTKLSCVerItem == 0 ||m_MTKLSCVerItem == 1)
	{
		ShadingCorrectionSample=m_MTKLSCBasePath+L"\\2_ShadingCorrectionSample.exe";
	}
	else if (m_MTKLSCVerItem == 2 || m_MTKLSCVerItem == 3)
	{
		ShadingCorrectionSample=m_MTKLSCBasePath+L"\\ShadingCorrectionSample.exe";
	}
	CString resultFile=m_MTKLSCBasePath+"\\result.txt";
	CString lsv_param_capture=m_MTKLSCBasePath+"\\lsv_param_capture.txt";
	CString corrected = m_MTKLSCBasePath + "\\after_ob_correct_lo.raw";

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
	if (PathFileExists(corrected))
	{
		if (m_SaveMTKraw)
		{
			MTK_SaveCorrectedRaw();
		}
	}
	WinExec(CT2A(ShadingCorrectionSample),SW_HIDE);
	CString FileContent;

	int Timeout=200;
	CString line = _T("");
	CStdioFile file;
	while (Timeout!=0)
	{
		if(!file.Open(resultFile, CFile::modeRead | CFile::typeText))
		{
			msleep(50);
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

BOOL COtpBase::MTK_SaveCorrectedRaw()
{
	char Pathsrc[MAX_PATH];
	char Pathdest[MAX_PATH];

	CString corrected = m_MTKLSCBasePath + "\\" + "after_ob_correct_lo.raw";

	int c;
	FILE *fpSrc, *fpDest;

	strcpy_s (Pathsrc, CT2A(corrected.GetBuffer(corrected.GetLength())));
	fopen_s(&fpSrc, Pathsrc, "rb");
	if (fpSrc == NULL)
	{
		info.Format(_T("找不到文件%s"),corrected);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}

	CString m_SaveDirectory = m_MTKLSCBasePath + _T("\\MTKCorrectedRaw");
	CreateDirectory(m_SaveDirectory,NULL);
	info.Format(_T("\\%d"),m_pInterface->GetModuleNum());
	CString m_savePath = m_SaveDirectory + info+_T(".raw");
	strcpy_s (Pathdest, CT2A(m_savePath.GetBuffer(m_savePath.GetLength())));
	fopen_s(&fpDest, Pathdest, "wb");
	if (fpDest == NULL)
	{
		info.Format(_T("创建文件失败%s"),m_savePath);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}
	while((c = fgetc(fpSrc)) != EOF)//复制源文件到目标文件
	{
		fputc(c,fpDest);
	}
	fclose(fpSrc);
	fclose(fpDest);

	FILE *fp;
	fopen_s(&fp, Pathdest, "rb");
	if (fp == NULL)
	{
		info.Format(_T("找不到文件%s"),Pathdest);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}
	int width = (m_pInterface->GetImage().width)/2;
	int height = (m_pInterface->GetImage().height)/2;
	int Format = m_pInterface->GetImage().RawFormat;

	BYTE *rawbuff = new BYTE[width*height];
	fread(rawbuff, 1, width*height, fp);	// 返回实际写入的数据项个数count
	fclose(fp);

	CString m_savebmpPath = m_SaveDirectory + info+_T(".bmp");
	Raw8ToBMP(m_savebmpPath, rawbuff, width,height,Format);
	SAFE_DELETE_ARRAY(rawbuff);

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////Qual///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void COtpBase::InitQualParam()
{
	//Qual LSC
	Qual_LSC_R_Max = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_R_Max"),1024);
	Qual_LSC_R_Min = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_R_Min"),0);
	Qual_LSC_Gr_Max = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_Gr_Max"),1024);
	Qual_LSC_Gr_Min = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_Gr_Min"),0);
	Qual_LSC_Gb_Max = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_Gb_Max"),1024);
	Qual_LSC_Gb_Min = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_Gb_Min"),0);
	Qual_LSC_B_Max = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_B_Max"),1024);
	Qual_LSC_B_Min = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSC_B_Min"),0);
	Qual_LSCDistanceSpec = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("Qual_LSCDistanceSpec"),240);

	//Qual PDAF
	memset(Qual_J_GainMapData,0,sizeof(Qual_J_GainMapData));       
	memset(Qual_L3_GainMapData,0,sizeof(Qual_L3_GainMapData));
	memset(Qual_L4_GainMapData,0,sizeof(Qual_L4_GainMapData));
	memset(Qual_L5_GainMapData, 0, sizeof(Qual_L5_GainMapData));
	MinPixelAvg = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("MinPixelAvg"),64);
	MaxPixelAvg = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("MaxPixelAvg"),1024);
	MinPDAvg = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("MinPDAvg"),64);
	MaxPDAvg = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("MaxPDAvg"),1024);
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

BOOL COtpBase::Qual_GetLSC_P()
{
	m_pInterface->AddLog(_T("Get Qual P LSC..."));
	typedef int (*lpFun)(unsigned char * pImage, int nWidth, int nHeight,                                                              
		int CFA_pattern, short R_black_offset, short Gr_black_offset,                                                                    
		short Gb_black_offset, short B_black_offset, BOOL bMode9x7);    

	CString WorkingPath = m_QualLSCBasePath+_T("P");
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
#if 0
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
				m_quallscdata[5*i + 4] = ((R_Hex>>2)&0xc0) + ((Gr_Hex>>4)&0x30) + ((Gb_Hex>>6)&0x0c) + ((B_Hex>>8)&0x03);	
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
// 				m_quallscdata[8*i + 0] = (R_Hex>>8)&0x0f;
// 				m_quallscdata[8*i + 1] = R_Hex&0xff;
// 				m_quallscdata[8*i + 2] = (Gr_Hex>>8)&0x0f;
// 				m_quallscdata[8*i + 3] = Gr_Hex&0xff;
// 				m_quallscdata[8*i + 4] = (Gb_Hex>>8)&0x0f;
// 				m_quallscdata[8*i + 5] = Gb_Hex&0xff;
// 				m_quallscdata[8*i + 6] = (B_Hex>>8)&0x0f;
// 				m_quallscdata[8*i + 7] = B_Hex&0xff;

				m_quallscdata[8*i + 0] = R_Hex&0xff;
				m_quallscdata[8*i + 1] = (R_Hex>>8)&0x0f;
				m_quallscdata[8*i + 2] = Gr_Hex&0xff;
				m_quallscdata[8*i + 3] = (Gr_Hex>>8)&0x0f;
				m_quallscdata[8*i + 4] = Gb_Hex&0xff;
				m_quallscdata[8*i + 5] = (Gb_Hex>>8)&0x0f;
				m_quallscdata[8*i + 6] = B_Hex&0xff;
				m_quallscdata[8*i + 7] = (B_Hex>>8)&0x0f;
			}
		}
#else
		m_quallscdata = new BYTE[1768];//17x13x8
		if (!ReadQualOnlyLSCLSCDataFromTXT())    return FALSE;
#endif
			for ( int i=0; i<1768; i++)
			{
				m_quallscsum += m_quallscdata[i];
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
	CString Path = m_QualLSCBasePath;//GetModulePath();
	CString str;
	str.Format(_T("P\\AWB_LSC_CALIBRATION_DATA.txt"));
	Path = Path + str;
	if (!PathFileExists(Path))
	{
		info.Format(_T("找不到文件%s!"),Path);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}
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

BOOL COtpBase::ReadQualOnlyLSCLSCDataFromTXT()
{
	CString Path = GetModulePath();
	CString str;
	str.Format(_T("\\LSC\\Qual\\P\\ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt"));
	CString eeprom_slim = Path + str;
	int Timeout=100;
	HANDLE hFile;

	while (Timeout!=0)
	{
		hFile = CreateFile(eeprom_slim,GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
		if( hFile != INVALID_HANDLE_VALUE )
		{
			DWORD nBytesRead;
			unsigned char BIN[2000] = {0};
			ReadFile(hFile, BIN, 2000, &nBytesRead, NULL );
			CloseHandle( hFile );

			if (nBytesRead != 1768)
			{
				m_pInterface->AddLog(_T("eeprom_slim大小不为1868 Byte!"),COLOR_RED);
				return FALSE;
			}

			for (int i = 0;i < 1768; i++)
			{
				m_quallscdata[i] = BIN[i];
			}
			break;
		}
		Sleep(100);
		Timeout--;
	}

	return TRUE;
}

BOOL COtpBase::ReadQualRverLSCDataFromTXT(float *R_Channel, float *Gr_Channel, float *Gb_channel, float *B_Channel)
{
	int mycharlength = 0;
	int strlength = 0;
	char mycharbuff[512] = {0};
	CStdioFile file;
	CString Path;// = GetModulePath();
	CString str;
	str.Format(_T("R\\AWB_LSC_CALIBRATION_DATA.txt"));
	Path = m_QualLSCBasePath+str;

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
			if ( strTemp.Find( _T("Channel"), 0 ) != -1 ||strTemp.Find( _T("LEFT LENS SHADING DATA"), 0 ) != -1)
			{	
				continue;
			}
			else if (strTemp.Find( _T("{"), 0 ) != -1)
			{
				channel_count++;
				continue;
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

	CString SensorINI;
	if (m_QUALLSCVerItem == 0)//P
	{
		SensorINI = m_QualLSCBasePath+ _T("P\\Standard.ini");  // 设定参数文档路径
	}
	else if (m_QUALLSCVerItem == 1)//R
	{
		SensorINI = m_QualLSCBasePath + _T("R\\Standard.ini");  // 设定参数文档路径
	}


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
			tempBuf[i][0] = data[i*2]+(data[i*2+1]<<8) ;			//R
			tempBuf[i][1] = data[i*2+442]+(data[i*2+1+442]<<8) ;	//Gr
			tempBuf[i][2] = data[i*2+884]+(data[i*2+1+884]<<8);		//Gb
			tempBuf[i][3] = data[i*2+1326]+(data[i*2+1+1326]<<8) ;	//B
		}

		for (i=0; i<221; i++)
		{
			m_quallscdata[cnt++]=(tempBuf[i][0]&0xff);		//R_L
			m_quallscdata[cnt++]=(tempBuf[i][1]&0xff);		//Gr_L
			m_quallscdata[cnt++]=(tempBuf[i][2]&0xff);		//Gb_L
			m_quallscdata[cnt++]=(tempBuf[i][3]&0xff);		//B_L
			m_quallscdata[cnt++]=(((tempBuf[i][0]>>8)&0x03)<<6)+(((tempBuf[i][1]>>8)&0x03)<<4)+(((tempBuf[i][2]>>8)&0x03)<<2)+((tempBuf[i][3]>>8)&0x03);//R_H Gr_H Gb_H B_H
		}
	}
	else
	{
		for(  i = 0; i < 221; i++ )
		{
// 			tempBuf[i][0] = data[i*2]+(data[i*2+1]<<8) ;			//R
// 			tempBuf[i][1] = data[i*2+442]+(data[i*2+1+442]<<8) ;	//Gr
// 			tempBuf[i][2] = data[i*2+884]+(data[i*2+1+884]<<8);		//Gb
// 			tempBuf[i][3] = data[i*2+1326]+(data[i*2+1+1326]<<8) ;	//B
			tempBuf[i][0] = data[i*2+1]+(data[i*2]<<8) ;			//R  //高-低
			tempBuf[i][1] = data[i*2+1+442]+(data[i*2+442]<<8) ;	//Gr
			tempBuf[i][2] = data[i*2+1+884]+(data[i*2+884]<<8);		//Gb
			tempBuf[i][3] = data[i*2+1+1326]+(data[i*2+1326]<<8) ;	//B
		}
		for(  i = 0; i < 221; i++ )
		{
// 			m_quallscdata[cnt++] = (tempBuf[i][0]&0xff);	
// 			m_quallscdata[cnt++] = (tempBuf[i][0]>>8) & 0xff;
// 			m_quallscdata[cnt++] = (tempBuf[i][1]&0xff);	
// 			m_quallscdata[cnt++] = (tempBuf[i][1]>>8) & 0xff;
// 			m_quallscdata[cnt++] = (tempBuf[i][2]&0xff);	
// 			m_quallscdata[cnt++] = (tempBuf[i][2]>>8) & 0xff;
// 			m_quallscdata[cnt++] = (tempBuf[i][3]&0xff);	
// 			m_quallscdata[cnt++] = (tempBuf[i][3]>>8) & 0xff;
			
			m_quallscdata[cnt++] = (tempBuf[i][0] >> 8) & 0xff;
			m_quallscdata[cnt++] = (tempBuf[i][0] & 0xff);
			
			m_quallscdata[cnt++] = (tempBuf[i][1] >> 8) & 0xff;
			m_quallscdata[cnt++] = (tempBuf[i][1] & 0xff);
			
			m_quallscdata[cnt++] = (tempBuf[i][2] >> 8) & 0xff;
			m_quallscdata[cnt++] = (tempBuf[i][2] & 0xff);
			
			m_quallscdata[cnt++] = (tempBuf[i][3] >> 8) & 0xff;
			m_quallscdata[cnt++] = (tempBuf[i][3] & 0xff);
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

	if (m_QUALLSCVerItem == 0)
	{
		FileName.Format(_T("P\\LSCData.txt"));
	}
	else
	{
		FileName.Format(_T("R\\LSCData.txt"));
	}

	FileName = m_QualLSCBasePath +FileName;

	if(_wfreopen_s(&fpn,FileName,_T("at"),stdout))  //
	{
		m_pInterface->AddLog(_T("无法打开LSCData.txt文件!"),COLOR_RED);
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

	//可以封装使用
	int h;  //height
	int w;  //width
	int R_Channel[221] = {0};
	int B_Channel[221] = {0};
	int Gr_Channel[221] = {0};
	int Gb_Channel[221] = {0};

	if (m_otpCtrl.quallsccompress)
	{
		for (i=0; i<221; i++)
		{
			R_Channel[i] = m_quallscdata[5*i] + ((m_quallscdata[5*i+4] & 0xC0) << 2);    //LSCReadData是从eeprom里读出的LSC数据，共有1105个
			Gr_Channel[i] = m_quallscdata[5*i+1] + ((m_quallscdata[5*i+4] & 0x30) << 4);
			Gb_Channel[i] = m_quallscdata[5*i+2] + ((m_quallscdata[5*i+4] & 0x0c) << 6);
			B_Channel[i] = m_quallscdata[5*i+3] + ((m_quallscdata[5*i+4] & 0x03) << 8);
		}
	}
	else
	{
		for (i=0; i<221; i++)
		{
			R_Channel[i] = (m_quallscdata[i*8]<<8) + m_quallscdata[i*8+1];
			Gr_Channel[i] = (m_quallscdata[i*8+2]<<8) + m_quallscdata[i*8+3];
			Gb_Channel[i] = (m_quallscdata[i*8+4]<<8 )+ m_quallscdata[i*8+5];
			B_Channel[i] = (m_quallscdata[i*8+6]<<8) + m_quallscdata[i*8+7];
		}
	}


/*	CString LSCFileName = L"D:\\Report\\LSCReadData.xls";
/	FILE *fpn2 = NULL;
	if(_wfreopen_s(&fpn2,LSCFileName,_T("at"),stdout))  //
	{
		m_pInterface->AddLog(_T("无法打开LSCReadData.xls文件!"),COLOR_RED);
		return 0;
	}*/

	printf("R_Channel:\n");
	for (h=0;h<13;h++)
	{
		for (w=0;w<17;w++)
		{
			printf("%d\t",R_Channel[w+h*17]);
		}
		printf("\n");
	}
	printf("\n");

	printf("Gr_Channel:\n");
	for (h=0;h<13;h++)
	{
		for (w=0;w<17;w++)
		{
			printf("%d\t",Gr_Channel[w+h*17]);
		}
		printf("\n");
	}
	printf("\n");

	printf("Gb_Channel:\n");
	for (h=0;h<13;h++)
	{
		for (w=0;w<17;w++)
		{
			printf("%d\t",Gb_Channel[w+h*17]);
		}
		printf("\n");
	}
	printf("\n");

	printf("B_Channel:\n");
	for (h=0;h<13;h++)
	{
		for (w=0;w<17;w++)
		{
			printf("%d\t",B_Channel[w+h*17]);
		}
		printf("\n");
	}
	printf("--------------------------------------\n");
	/*fclose(fpn2);*/
	/////////////////////////////////////
	if (LSCData_R_Max>Qual_LSC_R_Max||LSCData_R_Min<Qual_LSC_R_Min||LSCData_Gr_Max>Qual_LSC_Gr_Max||LSCData_Gr_Min<Qual_LSC_Gr_Min
		||LSCData_Gb_Max>Qual_LSC_Gb_Max||LSCData_Gb_Min<Qual_LSC_Gb_Min||LSCData_B_Max>Qual_LSC_B_Max||LSCData_B_Min<Qual_LSC_B_Min)
	{	
		m_pInterface->AddLog(_T("四通道值超出Standard允许范围！"),COLOR_RED);
		return FALSE;
	}

	if( tempMax > Qual_LSCDistanceSpec )
	{	
		m_pInterface->AddLog(_T("最大误差值超出Standard允许范围！"),COLOR_RED);
		return FALSE;
	} 

	return TRUE;
}



BOOL COtpBase::Qual_GetAWBData()
{
	CStdioFile file;
	CString Path = m_QualLSCBasePath;// = GetModulePath();
	if (m_QUALLSCVerItem)
	{
		Path +=  "R\\AWB_LSC_CALIBRATION_DATA.txt";
	}
	else
	{
		Path +=  "P\\AWB_LSC_CALIBRATION_DATA.txt";
	}


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

BOOL COtpBase::Qual_GetLSC_R()
{
	m_pInterface->AddLog(_T("Get Qual R LSC..."));
	typedef int (*lpFun)(unsigned char * pImage, int nWidth, int nHeight,                                                              
		int CFA_pattern, short R_black_offset, short Gr_black_offset,                                                                    
		short Gb_black_offset, short B_black_offset, BOOL bMode9x7,CalibrationDataStruct *pCalData);    

	CString WorkingPath = m_QualLSCBasePath+_T("R");
	SetCurrentDirectory(WorkingPath);
	WorkingPath += "\\LSCCalibrationDll_RevR.dll";
	HINSTANCE hQualLSCDll = LoadLibrary(WorkingPath);

	BOOL bRet = TRUE;
	do 
	{
		if (NULL == hQualLSCDll)
		{
			info.Format(_T("找不到LSCCalibrationDll_RevR.dll，请将此Dll放入程式目录:\n%s！"),WorkingPath);
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

//			CalibrationDataStruct Rver_Data[221];
//		CalibrationDataStruct * Rver_Data = new CalibrationDataStruct[221];
		CalibrationDataStruct Rver_Data;
		Rver_Data.R_LSC = (unsigned short *)malloc(221*sizeof(UINT));
		Rver_Data.Gr_LSC = (unsigned short *)malloc(221*sizeof(UINT));
		Rver_Data.Gb_LSC = (unsigned short *)malloc(221*sizeof(UINT));
		Rver_Data.B_LSC = (unsigned short *)malloc(221*sizeof(UINT));
		Rver_Data.AWB = (unsigned short *)malloc(221*sizeof(UINT));

		LensCorrection(lscbuff,lscimageinfo.width,lscimageinfo.height,m_cfaitem,m_otpCtrl.sensorob/4,m_otpCtrl.sensorob/4,m_otpCtrl.sensorob/4,m_otpCtrl.sensorob/4,m_quallscitem,&Rver_Data);

		free(Rver_Data.R_LSC);
		free(Rver_Data.Gr_LSC);
		free(Rver_Data.Gb_LSC);
		free(Rver_Data.B_LSC);
		free(Rver_Data.AWB);
		//////////////////////////////////Read LSC//////////////////////////////////
		float R_Channel[221] = {0};
		float Gr_Channel[221] = {0};
		float Gb_channel[221] = {0};
		float B_Channel[221] = {0};
		int R_Hex,Gr_Hex,Gb_Hex,B_Hex;
		if (!ReadQualRverLSCDataFromTXT(R_Channel,Gr_Channel,Gb_channel,B_Channel))    return FALSE;

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
				m_Rver_quallscsum += m_quallscdata[i];
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
				// 				m_quallscdata[8*i + 0] = (R_Hex>>8)&0x0f;
				// 				m_quallscdata[8*i + 1] = R_Hex&0xff;
				// 				m_quallscdata[8*i + 2] = (Gr_Hex>>8)&0x0f;
				// 				m_quallscdata[8*i + 3] = Gr_Hex&0xff;
				// 				m_quallscdata[8*i + 4] = (Gb_Hex>>8)&0x0f;
				// 				m_quallscdata[8*i + 5] = Gb_Hex&0xff;
				// 				m_quallscdata[8*i + 6] = (B_Hex>>8)&0x0f;
				// 				m_quallscdata[8*i + 7] = B_Hex&0xff;

				m_quallscdata[8*i + 1] = R_Hex&0xff;//高-低
				m_quallscdata[8*i + 0] = (R_Hex>>8)&0x0f;
				m_quallscdata[8*i + 3] = Gr_Hex&0xff;
				m_quallscdata[8*i + 2] = (Gr_Hex>>8)&0x0f;
				m_quallscdata[8*i + 5] = Gb_Hex&0xff;
				m_quallscdata[8*i + 4] = (Gb_Hex>>8)&0x0f;
				m_quallscdata[8*i + 7] = B_Hex&0xff;
				m_quallscdata[8*i + 6] = (B_Hex>>8)&0x0f;
			}
			for (i=0; i<1768; i++)
			{
				m_Rver_quallscsum += m_quallscdata[i];
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
///////////////////////////////////Qual PDAF///////////////////////////////////////
BOOL COtpBase::Qual_GainMapCal_J()
{
	CString FileFolderPath = GetModulePath()+"\\PDAF";
	CreateDirectory(FileFolderPath, NULL);
	FileFolderPath += "\\Qual";
	CreateDirectory(FileFolderPath, NULL);
	SetCurrentDirectory(FileFolderPath);
	CString Rawfilename =  FileFolderPath + "\\Qual_Step1.raw";

	ImageInfo imgInfo = m_pInterface->GetImage();
	int width = imgInfo.width;
	int height= imgInfo.height;
//#ifndef TestVersion 
	if (PathFileExists(Rawfilename)) 
	{
		if (!DeleteFile(Rawfilename))
		{
			m_pInterface->AddLog(_T("删除原Raw图失败!"), COLOR_RED);
			return FALSE;
		}
	}
	SaveCurrentRaw10(Rawfilename,imgInfo.Raw_Buf,width,height);
//#endif

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

	//////////////////////////////////////////////////////////////////////////
	//Sensor Basic Information: S5K3P3
// 	PDAFPatternValue.GlobalShift_x = 12; // PDAF Start position x
// 	PDAFPatternValue.GlobalShift_y = 12; // PDAF Start position y
// 	PDAFPatternValue.x_step = 64; //PDAF Block Width  
// 	PDAFPatternValue.y_step = 64; //PDAF Block Height
// 	PDAFPatternValue.ImageBits = 10; // Image Bit Width 10
// 	PDAFPatternValue.BlackLevel = 64; // Image Black Level
// 	PDAFPatternValue.SensorGain = 1;//1
// 	PDAFPatternValue.DSRatio = 2;//
// 	PDAFPatternValue.PDAFonG = FALSE;//FALSE
	//////////////////////////////////////////////////////////////////////////
	//Sensor Basic Information: S5K3L8  
	PDAFPatternValue.GlobalShift_x = 24; // PDAF Start position x
	PDAFPatternValue.GlobalShift_y = 24; // PDAF Start position y
	PDAFPatternValue.x_step = 64; //PDAF Block Width  
	PDAFPatternValue.y_step = 64; //PDAF Block Height
	PDAFPatternValue.ImageBits = 10; // Image Bit Width 10
	PDAFPatternValue.BlackLevel = 64; // Image Black Level
	PDAFPatternValue.SensorGain = 1;//1
	PDAFPatternValue.DSRatio = 2;//
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
		info.Format(_T("Cannot Find Gmap Caibration DATA %s or  This Image Size is Error ."),CA2T(CurImgFilename));
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
	m_pInterface->AddLog(_T("PDAF Gain Map Calibration Success.\n"),COLOR_BLUE);
	free(GmCalibImage);

	if (GainMapFlag & 0x8000)
	{	
		int gap = 0;
		if (OTPFormat_Type == 3)
		{
			Qual_J_GainMapData[0]=GainMapData2D.VersionNum&0xff;
			Qual_J_GainMapData[1]=GainMapData2D.VersionNum>>8;
			gap = 2;

// 			Qual_J_GainMapData[0]=GainMapData2D.VersionNum>>8;
// 			Qual_J_GainMapData[1]=GainMapData2D.VersionNum&0xff;
// 			gap = 2;

// 			Qual_J_GainMapData[0]=GainMapData2D.VersionNum;
// 			gap = 1;
		}
// 		Qual_J_GainMapData[0+gap]=GainMapData2D.OffsetX>>8;
// 		Qual_J_GainMapData[1+gap]=GainMapData2D.OffsetX&0xff;
// 		Qual_J_GainMapData[2+gap]=GainMapData2D.OffsetY>>8;
// 		Qual_J_GainMapData[3+gap]=GainMapData2D.OffsetY&0xff;
// 
// 		Qual_J_GainMapData[4+gap]=GainMapData2D.RatioX>>8;
// 		Qual_J_GainMapData[5+gap]=GainMapData2D.RatioX&0xff;
// 		Qual_J_GainMapData[6+gap]=GainMapData2D.RatioY>>8;
// 		Qual_J_GainMapData[7+gap]=GainMapData2D.RatioY&0xff;
// 
// 		Qual_J_GainMapData[8+gap]=GainMapData2D.MapWidth>>8;	
// 		Qual_J_GainMapData[9+gap]=GainMapData2D.MapWidth&0xff;
// 
// 		Qual_J_GainMapData[10+gap]=GainMapData2D.MapHeight>>8;
// 		Qual_J_GainMapData[11+gap]=GainMapData2D.MapHeight&0xff;

		Qual_J_GainMapData[0+gap]=GainMapData2D.OffsetX&0xff;
		Qual_J_GainMapData[1+gap]=GainMapData2D.OffsetX>>8;
		Qual_J_GainMapData[2+gap]=GainMapData2D.OffsetY&0xff;
		Qual_J_GainMapData[3+gap]=GainMapData2D.OffsetY>>8;

		Qual_J_GainMapData[4+gap]=GainMapData2D.RatioX&0xff;
		Qual_J_GainMapData[5+gap]=GainMapData2D.RatioX>>8;
		Qual_J_GainMapData[6+gap]=GainMapData2D.RatioY&0xff;
		Qual_J_GainMapData[7+gap]=GainMapData2D.RatioY>>8;

		Qual_J_GainMapData[8+gap]=GainMapData2D.MapWidth&0xff;
		Qual_J_GainMapData[9+gap]=GainMapData2D.MapWidth>>8;	

		Qual_J_GainMapData[10+gap]=GainMapData2D.MapHeight&0xff;
		Qual_J_GainMapData[11+gap]=GainMapData2D.MapHeight>>8;


		for (i=0;i<GainMapData2D.MapWidth*GainMapData2D.MapHeight;i++)
		{	
// 			Qual_J_GainMapData[12+gap+i*2]=GainMapData2D.Left_GainMap[i]>>8;
// 			Qual_J_GainMapData[13+gap+i*2]=GainMapData2D.Left_GainMap[i]&0xff;
// 
// 			Qual_J_GainMapData[GainMapData2D.MapWidth*GainMapData2D.MapHeight*2+12+gap+i*2]=GainMapData2D.Right_GainMap[i]>>8;
// 			Qual_J_GainMapData[GainMapData2D.MapWidth*GainMapData2D.MapHeight*2+12+gap+i*2+1]=GainMapData2D.Right_GainMap[i]&0xff;


			Qual_J_GainMapData[12+gap+i*2]=GainMapData2D.Left_GainMap[i]&0xff;
			Qual_J_GainMapData[13+gap+i*2]=GainMapData2D.Left_GainMap[i]>>8;

			Qual_J_GainMapData[GainMapData2D.MapWidth*GainMapData2D.MapHeight*2+12+gap+i*2]=GainMapData2D.Right_GainMap[i]&0xff;
			Qual_J_GainMapData[GainMapData2D.MapWidth*GainMapData2D.MapHeight*2+12+gap+i*2+1]=GainMapData2D.Right_GainMap[i]>>8;
		}
	}

	if (GainMapFlag & 0x4000)
	{
		Qual_J_GainMapData[0]=GainMapData1D.DSRatio>>8;
		Qual_J_GainMapData[1]=GainMapData1D.DSRatio&0xff;
		Qual_J_GainMapData[2]=GainMapData1D.ActualLength>>8;
		Qual_J_GainMapData[3]=GainMapData1D.ActualLength&0xff;

		Qual_J_GainMapData[4]=GainMapData1D.DSLength>>8;
		Qual_J_GainMapData[5]=GainMapData1D.DSLength&0xff;

		for (i=0;i<GainMapData1D.DSLength;i++)
		{	
			Qual_J_GainMapData[6+i*2]=GainMapData1D.Left_GainMap[i]>>8;
			Qual_J_GainMapData[7+i*2]=GainMapData1D.Left_GainMap[i]&0xff;

			Qual_J_GainMapData[GainMapData1D.DSLength*2+6+i*2]=GainMapData1D.Right_GainMap[i]>>8;
			Qual_J_GainMapData[GainMapData1D.DSLength*2+6+i*2+1]=GainMapData1D.Right_GainMap[i]&0xff;
		}
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

int32_t COtpBase::Qual_GainMapCal_L3()
{
	ImageInfo imgInfo = m_pInterface->GetImage();
	// 获取RAW BUF
	USHORT* raw_buf = m_pInterface->GetImage().Raw_Buf;
	int width = m_pInterface->GetImage().width;
	int height = m_pInterface->GetImage().height;

	CString FileFolderPath = GetModulePath()+"\\PDAF";
	CreateDirectory(FileFolderPath, NULL);
	FileFolderPath += "\\Qual";
	CreateDirectory(FileFolderPath, NULL);
	FileFolderPath += "\\L3";
	CreateDirectory(FileFolderPath, NULL);
	SetCurrentDirectory(FileFolderPath);
	if (CaptureFrame<CaptureSize)
	{
		RawArray[CaptureFrame] = new USHORT[width*height];

		info.Format(_T("\\Qual_L3Step1_%d.raw"),CaptureFrame+1);
		CString Rawfilename =  FileFolderPath + info;
#ifndef TestVersion 
		if (PathFileExists(Rawfilename)) 
		{
			if (!DeleteFile(Rawfilename))
			{
				m_pInterface->AddLog(_T("删除原Raw图失败!"), COLOR_RED);
				return 1;
			}
		}
		SaveCurrentRaw10(Rawfilename,raw_buf,width,height);
#endif
		FILE* fp_r;
		char CurImgFilename[MAX_PATH];
		strcpy_s(CurImgFilename,CT2A(Rawfilename.GetBuffer(Rawfilename.GetLength())));
		if (_wfopen_s(&fp_r,  Rawfilename,_T("rb")))   //CT2A(sfilename)
		{
			info.Format(_T("Cannot Find raw: %s"),CurImgFilename);
			m_pInterface->AddLog(info,COLOR_RED);
			return 1;
		}
		if (fp_r)
		{
			fread((void*)RawArray[CaptureFrame],2,width*height,fp_r);//读取Raw图
			fclose(fp_r);
		}
		else
		{
			info.Format(_T("Cannot Find FP"));
			m_pInterface->AddLog(info,COLOR_RED);
			return 1;
		}

		CaptureFrame++;
		return -1;//continue
	}

	USHORT* flatfieldbuffer;
	flatfieldbuffer = (USHORT*) calloc(width*height,sizeof(USHORT));
	for (int i=0; i<width*height; i++)
	{
		flatfieldbuffer[i] = USHORT((RawArray[0][i]+RawArray[1][i]+RawArray[2][i])/3.0+0.5);
	}

	CString Rawfilename =  FileFolderPath + _T("\\Step1_Average.raw");
#ifndef TestVersion
	if (PathFileExists(Rawfilename)) 
	{
		if (!DeleteFile(Rawfilename))
		{
			m_pInterface->AddLog(_T("删除原Raw图失败!"), COLOR_RED);
			return 1;
		}
	}
	SaveCurrentRaw10(Rawfilename,flatfieldbuffer,width,height);
	free(flatfieldbuffer);
#endif

	typedef int (*lpFun)(char *s);    
	typedef void*(*PdafCalCreate)(dll_config_t_l3 *dll_cfg);
	typedef int32_t(*PdafCalGetGainmap)(void *p, 
		uint16_t *img, sensor_config_t *sensor_cfg, gainmap_t *gain_map, 
		gainmap_limits_t *gm_limits);
	typedef int32_t(*PdafCalDestroy)(void* p);

	CString WorkingPath = FileFolderPath + _T("\\PDAFCalibrationTools_RevL_Dll.dll");
	HINSTANCE hPDAFDll = LoadLibrary(WorkingPath);

	BOOL bRet = TRUE;

	if (NULL == hPDAFDll)
	{
		info.Format(_T("找不到PDAFCalibrationTools_RevL_Dll.dll，请将此Dll放入程式目录:\n%s！"),WorkingPath);
		m_pInterface->AddLog(info,COLOR_RED);
		return 1;
	}
	lpFun GetVersion = (lpFun)GetProcAddress(hPDAFDll,"PDAF_Cal_get_lib_version");
	if (!GetVersion)
	{
		m_pInterface->AddLog(_T("找不到入口函数PDAF_Cal_get_lib_version！"),COLOR_RED);
		m_pInterface->AddLog(info,COLOR_RED);
		return 1;
	}
	lpFun GetFeature = (lpFun)GetProcAddress(hPDAFDll,"PDAF_Cal_get_lib_features");
	if (!GetFeature)
	{
		m_pInterface->AddLog(_T("找不到入口函数PDAF_Cal_get_lib_features！"),COLOR_RED);
		m_pInterface->AddLog(info,COLOR_RED);
		return 1;
	}

	BOOL Is2pd = FALSE;
	if(!Is2pd)
	{
		void *p; 
		char dll_version_info[1024];
		char dll_feature_info[2048];
		char path_raw_file[MAX_PATH];
		char path_gainmap_file[MAX_PATH];

		dll_config_t_l3     dll_cfg;
		sensor_config_t  sensor_cfg;
		gainmap_t        gain_map;
		gainmap_limits_t gm_limits;

		uint16_t *img_raw;
		uint16_t raw_width, raw_height;
		int32_t  rc = 0;

		// set test limits
		// calibration DLL will still return gain map even if limits are exceeded
		gm_limits.pd_max_limit   = 950;  // Max PD pixel value after LPF must be below 950
		gm_limits.pd_min_limit   = 100;  // Min PD pixel value after LPF must exceed 100
		gm_limits.gain_max_limit = (uint16_t) (7.999f * (1<<GAIN_MAP_Q_FMT));  // max gain of 8x

		// load dll config parameters
		get_dll_cfg_L3( &dll_cfg );

		/////////////////////////////////////////////////////////////////////////
		// INIT RUN ONLY ONCE 

		// allocate scratch buffer for calibration dll
		PdafCalCreate PDAF_Cal_create = (PdafCalCreate)GetProcAddress(hPDAFDll,"PDAF_Cal_create");
		p = PDAF_Cal_create( &dll_cfg ); 
		if( p == NULL ) {
			m_pInterface->AddLog(_T("dll memory allocation error."),COLOR_RED);
			return 1;  // dll memory allocation error
		}


		// calibration DLL for version number and version description
		GetVersion(dll_version_info);
		info.Format(_T("%s"),CA2T(dll_version_info));
		m_pInterface->AddLog(info);
		GetFeature(dll_feature_info);
		info.Format(_T("%s"),CA2T(dll_feature_info));
		m_pInterface->AddLog(info);



		// specify information about pdaf sensor
// 		raw_width   = sensor_cfg.image_width;
// 		raw_height  = sensor_cfg.image_height;
// 		black_lvl   = sensor_cfg.black_lvl;
// 		cfa         = sensor_cfg.cfa;
// 		img_width   = raw_width  >> 2; // tail mode pd image width=1/4 raw width
// 		img_height  = raw_height >> 2; // tail mode pd image height=1/4 raw height

		// specify sensor gain = sensor analog gain * sensor digital gain used
		// analog gain of dual-photodiode PD sensor must be between 2.0x and 3.0x 
		// digital gain of dual-photodiode PD sensor must be 1x or disabled
		// this is to prevent pixel from entering nonlinear region
//		sensor_gain = 1.0f;   


		// obtain new sensor config file from sensor manufacturer
		// add extern declaration to PDAFCalibrationTools_Dll.h and call sensor cfg here
		//3L8
		sensor_cfg.image_width      = 4208;
		sensor_cfg.image_height     = 3120;
		sensor_cfg.bit_depth        = 10;
		sensor_cfg.black_lvl        = 64;
		sensor_cfg.cfa              = GRBG;
		sensor_cfg.pd_block_width=64;//65;
		sensor_cfg.pd_block_height=64;//48;
		sensor_cfg.pd_block_start_x=24;//28;
		sensor_cfg.pd_block_start_y=24;//31;
		sensor_cfg.pd_block_n_pd_pairs=16;
		uint16_t pd_block_l_x[] =
		{ 4, 4, 8, 8,20,20,24,24,36,36,40,40,52,52,56,56};

		uint16_t pd_block_l_y[] = 
		{11,59,23,47,15,55,27,43,27,43,15,55,23,47,11,59};

		uint16_t pd_block_r_x[] =
		{ 4, 4, 8, 8,20,20,24,24,36,36,40,40,52,52,56,56};

		uint16_t pd_block_r_y[] =
		{ 7,63,27,43,11,59,31,39,31,39,11,59,27,43, 7,63};

		memcpy(sensor_cfg.pd_block_l_x, pd_block_l_x, 16*sizeof(uint16_t));
		memcpy(sensor_cfg.pd_block_l_y, pd_block_l_y, 16*sizeof(uint16_t)); 
		memcpy(sensor_cfg.pd_block_r_x, pd_block_r_x, 16*sizeof(uint16_t)); 
		memcpy(sensor_cfg.pd_block_r_y, pd_block_r_y, 16*sizeof(uint16_t)); 

		// allocate buffer for reading flat field image
		// incorrect buffer size may terminate DLL with invalid memory access
		raw_width = imgInfo.width;
		raw_height= imgInfo.height;

		img_raw = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));

		/////////////////////////////////////////////////////////////////////////
		// PER MODULE CALIBRATION
		// RECOMMEND USING SAME FLAT FIELD AS LENS SHADING CALIBRATION   

		// load flat field image
		strcpy_s(path_raw_file,CT2A(Rawfilename.GetBuffer(Rawfilename.GetLength())));//File to store gainmap
		if (read_raw_file(path_raw_file, img_raw, raw_width, raw_height) ) {
			//if (read_bin_file(space_conc(ROOTPATH,FLATPATH), img_raw, &raw_width, &raw_height) ) {
			info.Format(_T("cannot find raw: %s."),path_raw_file);
			m_pInterface->AddLog(_T("info"),COLOR_RED);
			return 1;
		}


		PdafCalGetGainmap PDAF_Cal_get_gainmap = (PdafCalGetGainmap)GetProcAddress(hPDAFDll, "PDAF_Cal_get_gainmap");
		// gain map calibration using parsed left and right images
		rc = PDAF_Cal_get_gainmap(p, img_raw, &sensor_cfg, &gain_map, &gm_limits );

		// display errors
		print_return_code_L3(rc);

		// do not write NVM if return code indicates error
		if ( rc != 0 ) {
			print_return_code_L3(rc);
			return 1;
		}

		CString PatternFilename = GetModulePath() + "\\PDAF\\Qual\\L3\\gainmap.txt";
		strcpy_s(path_gainmap_file,CT2A(PatternFilename.GetBuffer(PatternFilename.GetLength())));//File to store gainmap
		// store gain map in OTP/NVM
		write_gainmap_file(path_gainmap_file, &gain_map);

		/////////////////////////////////////////////////////////////////////////
		// DE-INIT
		free(img_raw);
		PdafCalDestroy PDAF_Cal_destroy = (PdafCalDestroy)GetProcAddress(hPDAFDll, "PDAF_Cal_destroy");
		PDAF_Cal_destroy( p ); 
		m_pInterface->AddLog(_T("PDAF Gain Map L ver Calibration Success.\n"),COLOR_BLUE);

		Qual_L3_GainMapData[0] = gain_map.cal_version>>8;
		Qual_L3_GainMapData[1] = gain_map.cal_version&0xff;
		Qual_L3_GainMapData[2] = gain_map.width>>8;
		Qual_L3_GainMapData[3] = gain_map.width&0xff;
		Qual_L3_GainMapData[4] = gain_map.height>>8;
		Qual_L3_GainMapData[5] = gain_map.height&0xff;
		for (int i = 0; i < gain_map.width * gain_map.height;i++)
		{
			Qual_L3_GainMapData[6+i*2] = gain_map.gm_l[i]>>8;
			Qual_L3_GainMapData[7+i*2] = gain_map.gm_l[i]&0xff;

			Qual_L3_GainMapData[6+gain_map.width*gain_map.height*2+i*2] = gain_map.gm_r[i]>>8;
			Qual_L3_GainMapData[7+gain_map.width*gain_map.height*2+i*2] = gain_map.gm_r[i]&0xff;
		}
		////////////////////////////////////Only Temp Burning//////////////////////////////////////
		//J Ver 
// 		BYTE Flag[1] = {0x01};
// 		DW9763_Programming(0x0711, Flag, 1);
// 		DW9763_Programming(0x0712, Qual_J_GainMapData, sizeof(Qual_J_GainMapData));
// 		ULONG Sum = 0;
// 		for (int i = 0; i<sizeof(Qual_J_GainMapData); i++)
// 		{
// 			Sum += Qual_J_GainMapData[i];
// 		}
// 		Sum = Sum % 256;
// 		BYTE WriteSum[1] = {0};
// 		WriteSum[0] = Sum;
// 		DW9763_Programming(0x0A93,WriteSum,1);
// 		//L3 Ver
// 		DW9763_Programming(0x0AA0, Flag, 1);
// 		DW9763_Programming(0x0AA1, Qual_L_GainMapData,sizeof(Qual_L_GainMapData));
// 		Sum = 0;
// 		for (int i = 0; i<sizeof(Qual_L_GainMapData); i++)
// 		{
// 			Sum += Qual_L_GainMapData[i];
// 		}
// 		Sum = Sum % 256;
// 		WriteSum[0] = Sum;
// 		DW9763_Programming(0x0E1B,WriteSum,1);
		return 0;

	}
	else
	{

// 		void *p; 
// 		char dll_version_info[1024];
// 		char dll_feature_info[2048];
// 
// 		dll_config_t     dll_cfg;
// 		sensor_config_t  sensor_cfg;
// 		gainmap_t        gain_map;
// 		gainmap_limits_t gm_limits;
// 
// 		uint16_t *img_l, *img_r, black_lvl;
// 		uint16_t raw_width, raw_height, pd_width, pd_height;
// 		cfa_t    cfa;
// 		float    sensor_gain;
// 
// 		int32_t  rc = 0;
// 
// 		// set test limits
// 		// calibration DLL will still return gain map even if limits are exceeded
// 		gm_limits.pd_max_limit   = 950;  // Max PD pixel value after LPF must be below 950
// 		gm_limits.pd_min_limit   = 100;   // Min PD pixel value after LPF must exceed 300
// 		gm_limits.gain_max_limit = (uint16_t) (7.999f * (1<<GAIN_MAP_Q_FMT));  // max gain of 6x
// 
// 		// specify sensor gain = sensor analog gain * sensor digital gain used
// 		// analog gain of dual-photodiode PD sensor must be between 2.0x and 3.0x 
// 		// digital gain of dual-photodiode PD sensor must be 1x or disabled
// 		// this is to prevent pixel from entering nonlinear region  
// 		sensor_gain = 2.0f; 
// 
// 		// load dll config parameters
// 		get_dll_cfg( &dll_cfg );
// 
// 		/////////////////////////////////////////////////////////////////////////
// 		// INIT RUN ONLY ONCE 
// 
// 		// allocate scratch buffer for calibration dll
// 		p = PDAF_Cal_create( &dll_cfg ); 
// 		if( p == NULL ) {
// 			return -1;  // dll memory allocation error
// 		}
// 
// 		// calibration DLL for version number and version description
// 		PDAF_Cal_get_lib_version(dll_version_info);  printf("%s", dll_version_info);
// 		PDAF_Cal_get_lib_features(dll_feature_info); printf("%s", dll_feature_info);
// 
// 		// obtain new sensor config file from sensor manufacturer
// 		// add extern declaration to PDAFCalibrationTools_Dll.h and call sensor cfg here
// 		get_sensor_cfg_imx362(&sensor_cfg);
// 
// 		// specify information about pdaf sensor
// 		raw_width  = sensor_cfg.image_width;
// 		raw_height = sensor_cfg.image_height;
// 		black_lvl  = sensor_cfg.black_lvl;
// 		cfa        = sensor_cfg.cfa;
// 		pd_width   = raw_width  >> 2; // pd image width is 1/4 raw image width
// 		pd_height  = raw_height >> 2; // pd image height is 1/4 raw image height
// 
// 
// 		// allocate buffer for reading flat field image
// 		// incorrect buffer size may terminate DLL with invalid memory access
// 		img_l = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));
// 		img_r = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));
// 		if (img_l == NULL || img_r == NULL) 
// 			return -1;
// 
// 
// 		/////////////////////////////////////////////////////////////////////////
// 		// PER MODULE CALIBRATION
// 		// RECOMMEND USING SAME FLAT FIELD AS LENS SHADING CALIBRATION 
// 
// 		// load flat field image
// 		if (read_2pd_raw_file(space_conc(ROOTPATH,FLATPATH), 
// 			img_l, img_r, raw_width, raw_height, cfa) ) {
// 				return -1;
// 		}
// 
// 		// gain map calibration using parsed left and right images
// 		rc = PDAF_Cal_get_gainmap_2pd(p, img_l, img_r, pd_width, pd_height,
// 			black_lvl, &gain_map, &gm_limits, sensor_gain);
// 
// 		// display errors
// 		print_return_code(rc);
// 
// 		// do not write NVM if return code indicates error
// 		if ( rc != 0 ) {
// 			return rc;
// 		}
// 
// 		// store gain map in OTP/NVM
// 		write_gainmap_file(space_conc(OUTPUTPATH,GAINMAPFN), &gain_map);
// 
// 		/////////////////////////////////////////////////////////////////////////
// 		// DE-INIT
// 		free(img_l);
// 		free(img_r);
// 		PDAF_Cal_destroy( p );
// 
// 		return rc;
		return 0;
	}
	return 0;
}

int32_t COtpBase::Qual_GainMapCal_L4()
{
	ImageInfo imgInfo = m_pInterface->GetImage();
	// 获取RAW BUF
	USHORT* raw_buf = m_pInterface->GetImage().Raw_Buf;
	int width = m_pInterface->GetImage().width;
	int height = m_pInterface->GetImage().height;

	CString FileFolderPath = GetModulePath()+"\\PDAF";
	CreateDirectory(FileFolderPath, NULL);
	FileFolderPath += "\\Qual";
	CreateDirectory(FileFolderPath, NULL);
	FileFolderPath += "\\L4";
	CreateDirectory(FileFolderPath, NULL);
	SetCurrentDirectory(FileFolderPath);
	if (CaptureFrame<CaptureSize)
	{
		RawArray[CaptureFrame] = new USHORT[width*height];

		info.Format(_T("\\Qual_L4Step1_%d.raw"),CaptureFrame+1);
		CString Rawfilename =  FileFolderPath + info;
//#ifndef TestVersion 
		if (PathFileExists(Rawfilename)) 
		{
			if (!DeleteFile(Rawfilename))
			{
				m_pInterface->AddLog(_T("删除原Raw图失败!"), COLOR_RED);
				return 1;
			}
		}
		SaveCurrentRaw10(Rawfilename,raw_buf,width,height);
//#endif
		FILE* fp_r;
		char CurImgFilename[MAX_PATH];
		strcpy_s(CurImgFilename,CT2A(Rawfilename.GetBuffer(Rawfilename.GetLength())));
		if (_wfopen_s(&fp_r,  Rawfilename,_T("rb")))   //CT2A(sfilename)
		{
			info.Format(_T("Cannot Find raw: %s"),CA2T(CurImgFilename));
			m_pInterface->AddLog(info,COLOR_RED);
			return 1;
		}
		if (fp_r)
		{
			fread((void*)RawArray[CaptureFrame],2,width*height,fp_r);//读取Raw图
			fclose(fp_r);
		}
		else
		{
			info.Format(_T("Cannot Find FP"));
			m_pInterface->AddLog(info,COLOR_RED);
			return 1;
		}

		CaptureFrame++;
		return -1;//continue
	}

	USHORT* flatfieldbuffer;
	flatfieldbuffer = (USHORT*) calloc(width*height,sizeof(USHORT));
	for (int i=0; i<width*height; i++)
	{
		flatfieldbuffer[i] = USHORT((RawArray[0][i]+RawArray[1][i]+RawArray[2][i])/3.0+0.5);
	}

	CString Rawfilename =  FileFolderPath + _T("\\Step1_Average.raw");
//#ifndef TestVersion
	if (PathFileExists(Rawfilename)) 
	{
		if (!DeleteFile(Rawfilename))
		{
			m_pInterface->AddLog(_T("删除原Raw图失败!"), COLOR_RED);
			return 1;
		}
	}
	SaveCurrentRaw10(Rawfilename,flatfieldbuffer,width,height);
	free(flatfieldbuffer);
//#endif

	typedef void*(*PdafCalCreate)(dll_config_t_l4 *dll_cfg);
	typedef int32_t(*PdafCalGetGainmap)(void *p, 
		uint16_t *img, sensor_config_t *sensor_cfg, gainmap_t *gain_map, 
		gainmap_limits_t *gm_limits);
	typedef int32_t(*PdafCalDestroy)(void* p);
	typedef int (*lpFun)(char *s);    
	CString WorkingPath = FileFolderPath + _T("\\PDAFCalibrationTools_RevL_Dll.dll");
	HINSTANCE hPDAFDll = LoadLibrary(WorkingPath);

	BOOL bRet = TRUE;

	if (NULL == hPDAFDll)
	{
		info.Format(_T("找不到PDAFCalibrationTools_RevL_Dll.dll，请将此Dll放入程式目录:\n%s！"),WorkingPath);
		m_pInterface->AddLog(info,COLOR_RED);
		return 1;
	}
	lpFun GetVersion = (lpFun)GetProcAddress(hPDAFDll,"PDAF_Cal_get_lib_version");
	if (!GetVersion)
	{
		m_pInterface->AddLog(_T("找不到入口函数PDAF_Cal_get_lib_version！"),COLOR_RED);
		m_pInterface->AddLog(info,COLOR_RED);
		return 1;
	}
	lpFun GetFeature = (lpFun)GetProcAddress(hPDAFDll,"PDAF_Cal_get_lib_features");
	if (!GetFeature)
	{
		m_pInterface->AddLog(_T("找不到入口函数PDAF_Cal_get_lib_features！"),COLOR_RED);
		m_pInterface->AddLog(info,COLOR_RED);
		return 1;
	}


	BOOL Is2pd = FALSE;
	if(!Is2pd)
	{
		void *p; 
		char dll_version_info[1024];
		char dll_feature_info[2048];
		char path_raw_file[MAX_PATH];
		char path_gainmap_file[MAX_PATH];

		dll_config_t_l4     dll_cfg;
		sensor_config_t  sensor_cfg;
		gainmap_t        gain_map;
		gainmap_limits_t gm_limits;

		uint16_t *img_raw;
		uint16_t raw_width, raw_height;
		int32_t  rc = 0;

		// set test limits
		// calibration DLL will still return gain map even if limits are exceeded
		gm_limits.pd_max_limit   = 950;  // Max PD pixel value after LPF must be below 950
		gm_limits.pd_min_limit   = 100;  // Min PD pixel value after LPF must exceed 100
		gm_limits.gain_max_limit = (uint16_t) (7.999f * (1<<GAIN_MAP_Q_FMT));  // max gain of 8x

		// load dll config parameters
		get_dll_cfg_L4( &dll_cfg );

		/////////////////////////////////////////////////////////////////////////
		// INIT RUN ONLY ONCE 

		// allocate scratch buffer for calibration dll
		PdafCalCreate PDAF_Cal_create = (PdafCalCreate)GetProcAddress(hPDAFDll, "PDAF_Cal_create");
		p = PDAF_Cal_create( &dll_cfg ); 
		if( p == NULL ) {
			m_pInterface->AddLog(_T("dll memory allocation error."),COLOR_RED);
			return 1;  // dll memory allocation error
		}


		// calibration DLL for version number and version description
		GetVersion(dll_version_info);
		info.Format(_T("%s"),CA2T(dll_version_info));
		m_pInterface->AddLog(info);
		GetFeature(dll_feature_info);
		info.Format(_T("%s"),CA2T(dll_feature_info));
		m_pInterface->AddLog(info);



		// specify information about pdaf sensor
		// 		raw_width   = sensor_cfg.image_width;
		// 		raw_height  = sensor_cfg.image_height;
		// 		black_lvl   = sensor_cfg.black_lvl;
		// 		cfa         = sensor_cfg.cfa;
		// 		img_width   = raw_width  >> 2; // tail mode pd image width=1/4 raw width
		// 		img_height  = raw_height >> 2; // tail mode pd image height=1/4 raw height

		// specify sensor gain = sensor analog gain * sensor digital gain used
		// analog gain of dual-photodiode PD sensor must be between 2.0x and 3.0x 
		// digital gain of dual-photodiode PD sensor must be 1x or disabled
		// this is to prevent pixel from entering nonlinear region
		//		sensor_gain = 1.0f;   


		// obtain new sensor config file from sensor manufacturer
		// add extern declaration to PDAFCalibrationTools_Dll.h and call sensor cfg here
		//3L8
		sensor_cfg.image_width      = 4208;
		sensor_cfg.image_height     = 3120;
		sensor_cfg.bit_depth        = 10;
		sensor_cfg.black_lvl        = 64;
		sensor_cfg.cfa              = GRBG;
		sensor_cfg.pd_block_width=64;//65;
		sensor_cfg.pd_block_height=64;//48;
		sensor_cfg.pd_block_start_x=24;//28;
		sensor_cfg.pd_block_start_y=24;//31;
		sensor_cfg.pd_block_n_pd_pairs=16;
		uint16_t pd_block_l_x[] =
		{ 4, 4, 8, 8,20,20,24,24,36,36,40,40,52,52,56,56};

		uint16_t pd_block_l_y[] = 
		{11,59,23,47,15,55,27,43,27,43,15,55,23,47,11,59};

		uint16_t pd_block_r_x[] =
		{ 4, 4, 8, 8,20,20,24,24,36,36,40,40,52,52,56,56};

		uint16_t pd_block_r_y[] =
		{ 7,63,27,43,11,59,31,39,31,39,11,59,27,43, 7,63};

		memcpy(sensor_cfg.pd_block_l_x, pd_block_l_x, 16*sizeof(uint16_t));
		memcpy(sensor_cfg.pd_block_l_y, pd_block_l_y, 16*sizeof(uint16_t)); 
		memcpy(sensor_cfg.pd_block_r_x, pd_block_r_x, 16*sizeof(uint16_t)); 
		memcpy(sensor_cfg.pd_block_r_y, pd_block_r_y, 16*sizeof(uint16_t)); 

		// allocate buffer for reading flat field image
		// incorrect buffer size may terminate DLL with invalid memory access
		raw_width = imgInfo.width;
		raw_height= imgInfo.height;

		img_raw = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));

		/////////////////////////////////////////////////////////////////////////
		// PER MODULE CALIBRATION
		// RECOMMEND USING SAME FLAT FIELD AS LENS SHADING CALIBRATION   

		// load flat field image
		strcpy_s(path_raw_file,CT2A(Rawfilename.GetBuffer(Rawfilename.GetLength())));//File to store gainmap
		if (read_raw_file(path_raw_file, img_raw, raw_width, raw_height) ) {
			//if (read_bin_file(space_conc(ROOTPATH,FLATPATH), img_raw, &raw_width, &raw_height) ) {
			info.Format(_T("cannot find raw: %s."),path_raw_file);
			m_pInterface->AddLog(_T("info"),COLOR_RED);
			return 1;
		}


		// gain map calibration using parsed left and right images
		PdafCalGetGainmap PDAF_Cal_get_gainmap = (PdafCalGetGainmap)GetProcAddress(hPDAFDll, "PDAF_Cal_get_gainmap");
		rc = PDAF_Cal_get_gainmap(p, img_raw, &sensor_cfg, &gain_map, &gm_limits );

		// display errors
		print_return_code_L4(rc);

		// do not write NVM if return code indicates error
		if ( rc != 0 ) {
			print_return_code_L4(rc);
			return 1;
		}

		CString PatternFilename = GetModulePath() + "\\PDAF\\Qual\\L4\\gainmap.txt";
		strcpy_s(path_gainmap_file,CT2A(PatternFilename.GetBuffer(PatternFilename.GetLength())));//File to store gainmap
		// store gain map in OTP/NVM
		write_gainmap_file(path_gainmap_file, &gain_map);

		/////////////////////////////////////////////////////////////////////////
		// DE-INIT
		free(img_raw);
		PdafCalDestroy PDAF_Cal_destroy = (PdafCalDestroy)GetProcAddress(hPDAFDll, "PDAF_Cal_destroy");
		PDAF_Cal_destroy( p ); 
		m_pInterface->AddLog(_T("PDAF Gain Map L ver Calibration Success.\n"),COLOR_BLUE);

// 		Qual_L4_GainMapData[0] = gain_map.cal_version&0xff;
// 		Qual_L4_GainMapData[1] = gain_map.cal_version>>8;
// 		Qual_L4_GainMapData[2] = gain_map.width&0xff;
// 		Qual_L4_GainMapData[3] = gain_map.width>>8;
// 		Qual_L4_GainMapData[4] = gain_map.height&0xff;
// 		Qual_L4_GainMapData[5] = gain_map.height>>8;

		Qual_L4_GainMapData[1] = gain_map.cal_version & 0xff;//高-低
		Qual_L4_GainMapData[0] = gain_map.cal_version >> 8;
		Qual_L4_GainMapData[3] = gain_map.width & 0xff;
		Qual_L4_GainMapData[2] = gain_map.width >> 8;
		Qual_L4_GainMapData[5] = gain_map.height & 0xff;
		Qual_L4_GainMapData[4] = gain_map.height >> 8;

		for (int i = 0; i < gain_map.width * gain_map.height;i++)
		{
// 			Qual_L4_GainMapData[6+i*2] = gain_map.gm_l[i]&0xff;
// 			Qual_L4_GainMapData[7+i*2] = gain_map.gm_l[i]>>8;
// 
// 			Qual_L4_GainMapData[6+gain_map.width*gain_map.height*2+i*2] = gain_map.gm_r[i]&0xff;
// 			Qual_L4_GainMapData[7+gain_map.width*gain_map.height*2+i*2] = gain_map.gm_r[i]>>8;

			Qual_L4_GainMapData[7 + i * 2] = gain_map.gm_l[i] & 0xff;//高-低
			Qual_L4_GainMapData[6 + i * 2] = gain_map.gm_l[i] >> 8;

			Qual_L4_GainMapData[7 + gain_map.width*gain_map.height * 2 + i * 2] = gain_map.gm_r[i] & 0xff;
			Qual_L4_GainMapData[6 + gain_map.width*gain_map.height * 2 + i * 2] = gain_map.gm_r[i] >> 8;

		}
		return 0;
	}
	else
	{

		// 		void *p; 
		// 		char dll_version_info[1024];
		// 		char dll_feature_info[2048];
		// 
		// 		dll_config_t     dll_cfg;
		// 		sensor_config_t  sensor_cfg;
		// 		gainmap_t        gain_map;
		// 		gainmap_limits_t gm_limits;
		// 
		// 		uint16_t *img_l, *img_r, black_lvl;
		// 		uint16_t raw_width, raw_height, pd_width, pd_height;
		// 		cfa_t    cfa;
		// 		float    sensor_gain;
		// 
		// 		int32_t  rc = 0;
		// 
		// 		// set test limits
		// 		// calibration DLL will still return gain map even if limits are exceeded
		// 		gm_limits.pd_max_limit   = 950;  // Max PD pixel value after LPF must be below 950
		// 		gm_limits.pd_min_limit   = 100;   // Min PD pixel value after LPF must exceed 300
		// 		gm_limits.gain_max_limit = (uint16_t) (7.999f * (1<<GAIN_MAP_Q_FMT));  // max gain of 6x
		// 
		// 		// specify sensor gain = sensor analog gain * sensor digital gain used
		// 		// analog gain of dual-photodiode PD sensor must be between 2.0x and 3.0x 
		// 		// digital gain of dual-photodiode PD sensor must be 1x or disabled
		// 		// this is to prevent pixel from entering nonlinear region  
		// 		sensor_gain = 2.0f; 
		// 
		// 		// load dll config parameters
		// 		get_dll_cfg( &dll_cfg );
		// 
		// 		/////////////////////////////////////////////////////////////////////////
		// 		// INIT RUN ONLY ONCE 
		// 
		// 		// allocate scratch buffer for calibration dll
		// 		p = PDAF_Cal_create( &dll_cfg ); 
		// 		if( p == NULL ) {
		// 			return -1;  // dll memory allocation error
		// 		}
		// 
		// 		// calibration DLL for version number and version description
		// 		PDAF_Cal_get_lib_version(dll_version_info);  printf("%s", dll_version_info);
		// 		PDAF_Cal_get_lib_features(dll_feature_info); printf("%s", dll_feature_info);
		// 
		// 		// obtain new sensor config file from sensor manufacturer
		// 		// add extern declaration to PDAFCalibrationTools_Dll.h and call sensor cfg here
		// 		get_sensor_cfg_imx362(&sensor_cfg);
		// 
		// 		// specify information about pdaf sensor
		// 		raw_width  = sensor_cfg.image_width;
		// 		raw_height = sensor_cfg.image_height;
		// 		black_lvl  = sensor_cfg.black_lvl;
		// 		cfa        = sensor_cfg.cfa;
		// 		pd_width   = raw_width  >> 2; // pd image width is 1/4 raw image width
		// 		pd_height  = raw_height >> 2; // pd image height is 1/4 raw image height
		// 
		// 
		// 		// allocate buffer for reading flat field image
		// 		// incorrect buffer size may terminate DLL with invalid memory access
		// 		img_l = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));
		// 		img_r = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));
		// 		if (img_l == NULL || img_r == NULL) 
		// 			return -1;
		// 
		// 
		// 		/////////////////////////////////////////////////////////////////////////
		// 		// PER MODULE CALIBRATION
		// 		// RECOMMEND USING SAME FLAT FIELD AS LENS SHADING CALIBRATION 
		// 
		// 		// load flat field image
		// 		if (read_2pd_raw_file(space_conc(ROOTPATH,FLATPATH), 
		// 			img_l, img_r, raw_width, raw_height, cfa) ) {
		// 				return -1;
		// 		}
		// 
		// 		// gain map calibration using parsed left and right images
		// 		rc = PDAF_Cal_get_gainmap_2pd(p, img_l, img_r, pd_width, pd_height,
		// 			black_lvl, &gain_map, &gm_limits, sensor_gain);
		// 
		// 		// display errors
		// 		print_return_code(rc);
		// 
		// 		// do not write NVM if return code indicates error
		// 		if ( rc != 0 ) {
		// 			return rc;
		// 		}
		// 
		// 		// store gain map in OTP/NVM
		// 		write_gainmap_file(space_conc(OUTPUTPATH,GAINMAPFN), &gain_map);
		// 
		// 		/////////////////////////////////////////////////////////////////////////
		// 		// DE-INIT
		// 		free(img_l);
		// 		free(img_r);
		// 		PDAF_Cal_destroy( p );
		// 
		// 		return rc;
		return 0;
	}
	return 0;
}

int32_t COtpBase::Qual_GainMapCal_L5()
{
	ImageInfo imgInfo = m_pInterface->GetImage();
	// 获取RAW BUF
	USHORT* raw_buf = m_pInterface->GetImage().Raw_Buf;
	int width = m_pInterface->GetImage().width;
	int height = m_pInterface->GetImage().height;

	CString FileFolderPath = GetModulePath() + "\\PDAF";
	CreateDirectory(FileFolderPath, NULL);
	FileFolderPath += "\\Qual";
	CreateDirectory(FileFolderPath, NULL);
	FileFolderPath += "\\L5";
	CreateDirectory(FileFolderPath, NULL);
	SetCurrentDirectory(FileFolderPath);
	if (CaptureFrame < CaptureSize)
	{
		RawArray[CaptureFrame] = new USHORT[width*height];

		info.Format(_T("\\Qual_L5Step1_%d.raw"), CaptureFrame + 1);
		CString Rawfilename = FileFolderPath + info;
		//#ifndef TestVersion 
		if (PathFileExists(Rawfilename))
		{
			if (!DeleteFile(Rawfilename))
			{
				m_pInterface->AddLog(_T("删除原Raw图失败!"), COLOR_RED);
				return 1;
			}
		}
		SaveCurrentRaw10(Rawfilename, raw_buf, width, height);
		//#endif
		FILE* fp_r;
		char CurImgFilename[MAX_PATH];
		strcpy_s(CurImgFilename, CT2A(Rawfilename.GetBuffer(Rawfilename.GetLength())));
		if (_wfopen_s(&fp_r, Rawfilename, _T("rb")))   //CT2A(sfilename)
		{
			info.Format(_T("Cannot Find raw: %s"), CA2T(CurImgFilename));
			m_pInterface->AddLog(info, COLOR_RED);
			return 1;
		}
		if (fp_r)
		{
			fread((void*)RawArray[CaptureFrame], 2, width*height, fp_r);//读取Raw图
			fclose(fp_r);
		}
		else
		{
			info.Format(_T("Cannot Find FP"));
			m_pInterface->AddLog(info, COLOR_RED);
			return 1;
		}

		CaptureFrame++;
		return -1;//continue
	}

	USHORT* flatfieldbuffer;
	flatfieldbuffer = (USHORT*)calloc(width*height, sizeof(USHORT));
	for (int i = 0; i < width*height; i++)
	{
		flatfieldbuffer[i] = USHORT((RawArray[0][i] + RawArray[1][i] + RawArray[2][i]) / 3.0 + 0.5);
	}

	CString Rawfilename = FileFolderPath + _T("\\Step1_Average.raw");
	//#ifndef TestVersion
	if (PathFileExists(Rawfilename))
	{
		if (!DeleteFile(Rawfilename))
		{
			m_pInterface->AddLog(_T("删除原Raw图失败!"), COLOR_RED);
			return 1;
		}
	}
	SaveCurrentRaw10(Rawfilename, flatfieldbuffer, width, height);
	free(flatfieldbuffer);
	//#endif

	typedef void*(*PdafCalCreate)(dll_config_t_l5 *dll_cfg);
	typedef int32_t(*PdafCalGetGainmap)(void *p,
		uint16_t *img, sensor_config_t *sensor_cfg, gainmap_t *gain_map,
		gainmap_limits_t *gm_limits);
	typedef int32_t(*PdafCalDestroy)(void* p);
	typedef int(*lpFun)(char *s);
	CString WorkingPath = FileFolderPath + _T("\\PDAFCalibrationTools_RevL_Dll.dll");
	HINSTANCE hPDAFDll = LoadLibrary(WorkingPath);

	BOOL bRet = TRUE;

	if (NULL == hPDAFDll)
	{
		info.Format(_T("找不到PDAFCalibrationTools_RevL_Dll.dll，请将此Dll放入程式目录:\n%s！"), WorkingPath);
		m_pInterface->AddLog(info, COLOR_RED);
		return 1;
	}
	lpFun GetVersion = (lpFun)GetProcAddress(hPDAFDll, "PDAF_Cal_get_lib_version");
	if (!GetVersion)
	{
		m_pInterface->AddLog(_T("找不到入口函数PDAF_Cal_get_lib_version！"), COLOR_RED);
		m_pInterface->AddLog(info, COLOR_RED);
		return 1;
	}
	lpFun GetFeature = (lpFun)GetProcAddress(hPDAFDll, "PDAF_Cal_get_lib_features");
	if (!GetFeature)
	{
		m_pInterface->AddLog(_T("找不到入口函数PDAF_Cal_get_lib_features！"), COLOR_RED);
		m_pInterface->AddLog(info, COLOR_RED);
		return 1;
	}


	BOOL Is2pd = FALSE;
	if (!Is2pd)
	{
		void *p;
		char dll_version_info[1024];
		char dll_feature_info[2048];
		char path_raw_file[MAX_PATH];
		char path_gainmap_file[MAX_PATH];

		dll_config_t_l5     dll_cfg;
		sensor_config_t  sensor_cfg;
		gainmap_t        gain_map;
		gainmap_limits_t gm_limits;

		uint16_t *img_raw;
		uint16_t raw_width, raw_height;
		int32_t  rc = 0;

		// set test limits
		// calibration DLL will still return gain map even if limits are exceeded
		gm_limits.pd_max_limit = 950;  // Max PD pixel value after LPF must be below 950
		gm_limits.pd_min_limit = 100;  // Min PD pixel value after LPF must exceed 100
		gm_limits.gain_max_limit = (uint16_t)(7.999f * (1 << GAIN_MAP_Q_FMT));  // max gain of 8x

		// load dll config parameters
		get_dll_cfg_L5(&dll_cfg);

		/////////////////////////////////////////////////////////////////////////
		// INIT RUN ONLY ONCE 

		// allocate scratch buffer for calibration dll
		PdafCalCreate PDAF_Cal_create = (PdafCalCreate)GetProcAddress(hPDAFDll, "PDAF_Cal_create");
		p = PDAF_Cal_create(&dll_cfg);
		if (p == NULL) {
			m_pInterface->AddLog(_T("dll memory allocation error."), COLOR_RED);
			return 1;  // dll memory allocation error
		}


		// calibration DLL for version number and version description
		GetVersion(dll_version_info);
		info.Format(_T("%s"), CA2T(dll_version_info));
		m_pInterface->AddLog(info);
		GetFeature(dll_feature_info);
		info.Format(_T("%s"), CA2T(dll_feature_info));
		m_pInterface->AddLog(info);



		// specify information about pdaf sensor
		// 		raw_width   = sensor_cfg.image_width;
		// 		raw_height  = sensor_cfg.image_height;
		// 		black_lvl   = sensor_cfg.black_lvl;
		// 		cfa         = sensor_cfg.cfa;
		// 		img_width   = raw_width  >> 2; // tail mode pd image width=1/4 raw width
		// 		img_height  = raw_height >> 2; // tail mode pd image height=1/4 raw height

		// specify sensor gain = sensor analog gain * sensor digital gain used
		// analog gain of dual-photodiode PD sensor must be between 2.0x and 3.0x 
		// digital gain of dual-photodiode PD sensor must be 1x or disabled
		// this is to prevent pixel from entering nonlinear region
		//		sensor_gain = 1.0f;   


		// obtain new sensor config file from sensor manufacturer
		// add extern declaration to PDAFCalibrationTools_Dll.h and call sensor cfg here
		//3L8
		sensor_cfg.image_width = 4208;
		sensor_cfg.image_height = 3120;
		sensor_cfg.bit_depth = 10;
		sensor_cfg.black_lvl = 64;
		sensor_cfg.cfa = GRBG;
		sensor_cfg.pd_block_width = 64;//65;
		sensor_cfg.pd_block_height = 64;//48;
		sensor_cfg.pd_block_start_x = 24;//28;
		sensor_cfg.pd_block_start_y = 24;//31;
		sensor_cfg.pd_block_n_pd_pairs = 16;
		uint16_t pd_block_l_x[] =
		{ 4, 4, 8, 8, 20, 20, 24, 24, 36, 36, 40, 40, 52, 52, 56, 56 };

		uint16_t pd_block_l_y[] =
		{ 11, 59, 23, 47, 15, 55, 27, 43, 27, 43, 15, 55, 23, 47, 11, 59 };

		uint16_t pd_block_r_x[] =
		{ 4, 4, 8, 8, 20, 20, 24, 24, 36, 36, 40, 40, 52, 52, 56, 56 };

		uint16_t pd_block_r_y[] =
		{ 7, 63, 27, 43, 11, 59, 31, 39, 31, 39, 11, 59, 27, 43, 7, 63 };

		memcpy(sensor_cfg.pd_block_l_x, pd_block_l_x, 16 * sizeof(uint16_t));
		memcpy(sensor_cfg.pd_block_l_y, pd_block_l_y, 16 * sizeof(uint16_t));
		memcpy(sensor_cfg.pd_block_r_x, pd_block_r_x, 16 * sizeof(uint16_t));
		memcpy(sensor_cfg.pd_block_r_y, pd_block_r_y, 16 * sizeof(uint16_t));

		// allocate buffer for reading flat field image
		// incorrect buffer size may terminate DLL with invalid memory access
		raw_width = imgInfo.width;
		raw_height = imgInfo.height;

		img_raw = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));

		/////////////////////////////////////////////////////////////////////////
		// PER MODULE CALIBRATION
		// RECOMMEND USING SAME FLAT FIELD AS LENS SHADING CALIBRATION   

		// load flat field image
		strcpy_s(path_raw_file, CT2A(Rawfilename.GetBuffer(Rawfilename.GetLength())));//File to store gainmap
		if (read_raw_file(path_raw_file, img_raw, raw_width, raw_height)) {
			//if (read_bin_file(space_conc(ROOTPATH,FLATPATH), img_raw, &raw_width, &raw_height) ) {
			info.Format(_T("cannot find raw: %s."), path_raw_file);
			m_pInterface->AddLog(_T("info"), COLOR_RED);
			return 1;
		}


		// gain map calibration using parsed left and right images
		PdafCalGetGainmap PDAF_Cal_get_gainmap = (PdafCalGetGainmap)GetProcAddress(hPDAFDll, "PDAF_Cal_get_gainmap");
		rc = PDAF_Cal_get_gainmap(p, img_raw, &sensor_cfg, &gain_map, &gm_limits);

		// display errors
		print_return_code_L5(rc);

		// do not write NVM if return code indicates error
		if (rc != 0) {
			print_return_code_L5(rc);
			return 1;
		}

		CString PatternFilename = GetModulePath() + "\\PDAF\\Qual\\L5\\gainmap.txt";
		strcpy_s(path_gainmap_file, CT2A(PatternFilename.GetBuffer(PatternFilename.GetLength())));//File to store gainmap
		// store gain map in OTP/NVM
		write_gainmap_file(path_gainmap_file, &gain_map);

		/////////////////////////////////////////////////////////////////////////
		// DE-INIT
		free(img_raw);
		PdafCalDestroy PDAF_Cal_destroy = (PdafCalDestroy)GetProcAddress(hPDAFDll, "PDAF_Cal_destroy");
		PDAF_Cal_destroy(p);
		m_pInterface->AddLog(_T("PDAF Gain Map L ver Calibration Success.\n"), COLOR_BLUE);

		// 		Qual_L4_GainMapData[0] = gain_map.cal_version&0xff;
		// 		Qual_L4_GainMapData[1] = gain_map.cal_version>>8;
		// 		Qual_L4_GainMapData[2] = gain_map.width&0xff;
		// 		Qual_L4_GainMapData[3] = gain_map.width>>8;
		// 		Qual_L4_GainMapData[4] = gain_map.height&0xff;
		// 		Qual_L4_GainMapData[5] = gain_map.height>>8;

		Qual_L5_GainMapData[1] = gain_map.cal_version & 0xff;//高-低
		Qual_L5_GainMapData[0] = gain_map.cal_version >> 8;
		Qual_L5_GainMapData[3] = gain_map.width & 0xff;
		Qual_L5_GainMapData[2] = gain_map.width >> 8;
		Qual_L5_GainMapData[5] = gain_map.height & 0xff;
		Qual_L5_GainMapData[4] = gain_map.height >> 8;

		for (int i = 0; i < gain_map.width * gain_map.height; i++)
		{
			// 			Qual_L4_GainMapData[6+i*2] = gain_map.gm_l[i]&0xff;
			// 			Qual_L4_GainMapData[7+i*2] = gain_map.gm_l[i]>>8;
			// 
			// 			Qual_L4_GainMapData[6+gain_map.width*gain_map.height*2+i*2] = gain_map.gm_r[i]&0xff;
			// 			Qual_L4_GainMapData[7+gain_map.width*gain_map.height*2+i*2] = gain_map.gm_r[i]>>8;

			Qual_L5_GainMapData[7 + i * 2] = gain_map.gm_l[i] & 0xff;//高-低
			Qual_L5_GainMapData[6 + i * 2] = gain_map.gm_l[i] >> 8;

			Qual_L5_GainMapData[7 + gain_map.width*gain_map.height * 2 + i * 2] = gain_map.gm_r[i] & 0xff;
			Qual_L5_GainMapData[6 + gain_map.width*gain_map.height * 2 + i * 2] = gain_map.gm_r[i] >> 8;

		}
		return 0;
	}
	else
	{

	
		return 0;
	}
	return 0;
}

int COtpBase::write_gainmap_file(char *filename, gainmap_t *gain_map) 
{
	int i;
	FILE *fp;
	if (_wfopen_s(&fp,CA2T(filename),_T("w")))
	{
		printf("Cannot open file for write: %s\n", filename);
		return -1;
	}  

	fprintf(fp, "%d\n", gain_map->cal_version);

	fprintf(fp, "%d\n", gain_map->width);     // from PDAF CAL version K and up 
	fprintf(fp, "%d\n", gain_map->height);    // gain map size is fixed to 17x13

	for (i = 0; i < gain_map->height*gain_map->width; i++)
		fprintf(fp, "%d\n",gain_map->gm_l[i]);
	for (i = 0; i < gain_map->height*gain_map->width; i++)
		fprintf(fp, "%d\n", gain_map->gm_r[i]);
	fclose(fp);

	return 0;
}

void COtpBase::print_return_code_L3(int32_t rc)
{
	// error codes from gain map calibration

	// 0x0001: Memory allocation error
	// 0x0002: Invalid sensor configuration or sensor gain
	// 0x0004: Invalid pd pixel pattern within block
	// 0x0008: Gain Map: pixel values too high for calibration
	// 0x0010: Gain Map: pixel values too low for calibration
	// 0x0020: Gain Map: gain map shows maximum value that is too high
	// 0x0040: DCC: phase disparity values out of bound. invalid input image 
	// 0x0080: DCC: phase disparity values out of order. LEFT and RIGHT pixels may be swapped 
	// 0x0100: DCC: insufficient number of lens sweep prior to dcc calculation
	// 0x0200: DCC: insufficient match between zero phase disparity focus peak
	// 0x0400: DCC: insufficient range of pd values observed from INF to MACRO
	// 0x1000: DCC: Please check line chart condition.
	// 0x2000: DCC: object distance close to fit boundary.

	if (rc & 0x0001)
		printf("\nError code 0x0001: Memory allocation error.");
	if (rc & 0x0002)
		printf("\nError code 0x0002: Invalid sensor configuration or sensor gain.");
	if (rc & 0x0004)
		printf("\nError code 0x0004: Invalid pd pixel pattern within block.");
	if (rc & 0x0008)
		printf("\nError code 0x0008: Gain Map: pd pixel values too high for calibration.");
	if (rc & 0x0010)
		printf("\nError code 0x0010: Gain Map: pd pixel values too low for calibration.");
	if (rc & 0x0020)
		printf("\nError code 0x0020: Gain Map: gain map shows maximum value that is too high.");
	if (rc & 0x0040)
		printf("\nError code 0x0040: DCC: phase disparity values out of bound. invalid input image.");
	if (rc & 0x0080)
		printf("\nError code 0x0080: DCC: phase disparity values out of order. LEFT and RIGHT pixels may be swapped.");
	if (rc & 0x0100)
		printf("\nError code 0x0100: DCC: insufficient number of lens sweep prior to dcc calculation.");
	if (rc & 0x0200)
		printf("\nError code 0x0200: DCC: insufficient match between zero phase disparity focus peak.");
	if (rc & 0x0400)
		printf("\nError code 0x0400: DCC: insufficient range of pd values from INF to MACRO.");
	if (rc & 0x1000)
		printf("\nError code 0x1000: DCC: Please check line chart condition.");
	if (rc & 0x2000)
		printf("\nError code 0x2000: DCC: object distance is too close to stack fit boundary.");
}

void COtpBase::print_return_code_L4(int32_t rc)
{
	// error codes from gain map calibration

	// 0x0001: Memory allocation error
	// 0x0002: Invalid sensor configuration or sensor gain
	// 0x0004: Invalid pd pixel pattern within block
	// 0x0008: Gain Map: pixel values too high for calibration
	// 0x0010: Gain Map: pixel values too low for calibration
	// 0x0020: Gain Map: gain map shows maximum value that is too high
	// 0x0040: DCC: phase disparity values out of bound. invalid input image 
	// 0x0080: DCC: phase disparity values out of order. LEFT and RIGHT pixels may be swapped 
	// 0x0100: DCC: insufficient number of lens sweep prior to dcc calculation
	// 0x0200: DCC: insufficient match between zero phase disparity focus peak
	// 0x0400: DCC: insufficient range of pd values observed from INF to MACRO
	// 0x1000: DCC: Please check line chart condition.
	// 0x2000: DCC: object distance close to fit boundary.

	if (rc & 0x0001)
		printf("\nError code 0x0001: Memory allocation error.");
	if (rc & 0x0002)
		printf("\nError code 0x0002: Invalid sensor configuration or sensor gain.");
	if (rc & 0x0004)
		printf("\nError code 0x0004: Invalid pd pixel pattern within block.");
	if (rc & 0x0008)
		printf("\nError code 0x0008: Gain Map: pd pixel values too high for calibration.");
	if (rc & 0x0010)
		printf("\nError code 0x0010: Gain Map: pd pixel values too low for calibration.");
	if (rc & 0x0020)
		printf("\nError code 0x0020: Gain Map: gain map shows maximum value that is too high.");
	if (rc & 0x0040)
		printf("\nError code 0x0040: DCC: phase disparity values out of bound. invalid input image.");
	if (rc & 0x0080)
		printf("\nError code 0x0080: DCC: phase disparity values out of order. LEFT and RIGHT pixels may be swapped.");
	if (rc & 0x0100)
		printf("\nError code 0x0100: DCC: insufficient number of lens sweep prior to dcc calculation.");
	if (rc & 0x0200)
		printf("\nError code 0x0200: DCC: insufficient match between zero phase disparity focus peak.");
	if (rc & 0x0400)
		printf("\nError code 0x0400: DCC: insufficient range of pd values from INF to MACRO.");
	if (rc & 0x1000)
		printf("\nError code 0x1000: DCC: Please check line chart condition.");
	if (rc & 0x2000)
		printf("\nError code 0x2000: DCC: focus peak is out of boundary.");
}

void COtpBase::print_return_code_L5(int32_t rc)
{


	if (rc & 0x0001)
		printf("\nError code 0x0001: Memory allocation error.");
	if (rc & 0x0002)
		printf("\nError code 0x0002: Invalid sensor configuration or sensor gain.");
	if (rc & 0x0004)
		printf("\nError code 0x0004: Invalid pd pixel pattern within block.");
	if (rc & 0x0008)
		printf("\nError code 0x0008: Gain Map: pd pixel values too high for calibration.");
	if (rc & 0x0010)
		printf("\nError code 0x0010: Gain Map: pd pixel values too low for calibration.");
	if (rc & 0x0020)
		printf("\nError code 0x0020: Gain Map: gain map shows maximum value that is too high.");
	if (rc & 0x0040)
		printf("\nError code 0x0040: DCC: phase disparity values out of bound. invalid input image.");
	if (rc & 0x0080)
		printf("\nError code 0x0080: DCC: phase disparity values out of order. LEFT and RIGHT pixels may be swapped.");
	if (rc & 0x0100)
		printf("\nError code 0x0100: DCC: insufficient number of lens sweep prior to dcc calculation.");
	if (rc & 0x0200)
		printf("\nError code 0x0200: DCC: insufficient match between zero phase disparity focus peak.");
	if (rc & 0x0400)
		printf("\nError code 0x0400: DCC: insufficient range of pd values from INF to MACRO.");
	if (rc & 0x1000)
		printf("\nError code 0x1000: DCC: Please check line chart condition.");
	if (rc & 0x2000)
		printf("\nError code 0x2000: DCC: focus peak is out of boundary.");
}

int COtpBase::read_raw_file(char *filename, uint16_t *img, int width, int height)
{
	int i, j;
	FILE *fp;
	uint16_t pix;

// 	fp = fopen(filename, "rb");
// 	if (fp == NULL)
	if (_wfopen_s(&fp,CA2T(filename),_T("rb")))
	{
		return -1;
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			fread(&pix, sizeof(uint16_t), 1, fp);
			img[i*width + j] = uint16_t(pix);
		}
	}
	fclose(fp);
	return 0;
}

void COtpBase::get_dll_cfg_L3(dll_config_t_l3 *dll_cfg )
{
	int8_t dcc_fir[]  = DCC_FIR;

	dll_cfg->cal_version         = CAL_VERSION;
	dll_cfg->chart_type          = LINECHART;
	dll_cfg->verify_type         = NORMALIZE;
	dll_cfg->gain_map_height     = GAIN_MAP_HEIGHT;
	dll_cfg->gain_map_width      = GAIN_MAP_WIDTH;
	dll_cfg->gain_map_q_fmt      = GAIN_MAP_Q_FMT;	
	dll_cfg->dcc_map_height      = DCC_MAP_HEIGHT;
	dll_cfg->dcc_map_width       = DCC_MAP_WIDTH;
	dll_cfg->dcc_map_q_fmt       = DCC_MAP_Q_FMT;
	memcpy(dll_cfg->dcc_fir, dcc_fir, sizeof(int8_t)*DCC_FIR_SZ);
	dll_cfg->dcc_fir_sz          = DCC_FIR_SZ;
	dll_cfg->dcc_stack_sz        = DCC_STACK_SZ_L3;
	dll_cfg->dcc_search_range    = DCC_SEARCH_RANGE;
	dll_cfg->dcc_fit_exclusion   = DCC_FIT_EXCLUSION;
	dll_cfg->dcc_min_pd_range    = DCC_MIN_PD_RANGE_L3;
	dll_cfg->dcc_map_overlap     = (float)DCC_MAP_OVERLAP_L3;
	dll_cfg->dpd_pixel_unit      = DPD_PIXEL_UNIT;
	dll_cfg->dpd_min_sensor_gain = (float)DPD_MIN_SENSOR_GAIN;
	dll_cfg->dpd_max_sensor_gain = (float)DPD_MAX_SENSOR_GAIN;
}

void COtpBase::get_dll_cfg_L4(dll_config_t_l4 *dll_cfg )
{
	 float dcc_fir[]  = DCC_FIR;

	dll_cfg->cal_version         = CAL_VERSION;
	dll_cfg->chart_type          = LINECHART;
	dll_cfg->verify_type         = NORMALIZE;
	dll_cfg->gain_map_height     = GAIN_MAP_HEIGHT;
	dll_cfg->gain_map_width      = GAIN_MAP_WIDTH;
	dll_cfg->gain_map_q_fmt      = GAIN_MAP_Q_FMT;	
	dll_cfg->dcc_map_height      = DCC_MAP_HEIGHT;
	dll_cfg->dcc_map_width       = DCC_MAP_WIDTH;
	dll_cfg->dcc_map_q_fmt       = DCC_MAP_Q_FMT;
	memcpy(dll_cfg->dcc_fir, dcc_fir, sizeof(float)*DCC_FIR_SZ);
	dll_cfg->dcc_fir_sz          = DCC_FIR_SZ;
	dll_cfg->dcc_stack_sz        = DCC_STACK_SZ_L4;
	dll_cfg->dcc_search_range    = DCC_SEARCH_RANGE;
	dll_cfg->dcc_fit_exclusion   = DCC_FIT_EXCLUSION;
	dll_cfg->dcc_min_pd_range    = DCC_MIN_PD_RANGE_L4;
	dll_cfg->dcc_map_overlap     = (float)DCC_MAP_OVERLAP_L4;
//    dll_cfg->dcc_linearity_level_theshold = DCC_LINEARITY_LEVEL_THRESHOLD;
	dll_cfg->dpd_pixel_unit      = DPD_PIXEL_UNIT;
	dll_cfg->dpd_min_sensor_gain = (float)DPD_MIN_SENSOR_GAIN;
	dll_cfg->dpd_max_sensor_gain = (float)DPD_MAX_SENSOR_GAIN;
}

void COtpBase::get_dll_cfg_L5(dll_config_t_l5 *dll_cfg)
{
	float dcc_fir[] = DCC_FIR;

	dll_cfg->cal_version = CAL_VERSION;
	dll_cfg->chart_type = LINECHART;
	dll_cfg->verify_type = NORMALIZE;
	dll_cfg->gain_map_height = GAIN_MAP_HEIGHT;
	dll_cfg->gain_map_width = GAIN_MAP_WIDTH;
	dll_cfg->gain_map_q_fmt = GAIN_MAP_Q_FMT;
	dll_cfg->dcc_map_height = DCC_MAP_HEIGHT;
	dll_cfg->dcc_map_width = DCC_MAP_WIDTH;
	dll_cfg->dcc_map_q_fmt = DCC_MAP_Q_FMT;
	memcpy(dll_cfg->dcc_fir, dcc_fir, sizeof(float)*DCC_FIR_SZ);
	dll_cfg->dcc_fir_sz = DCC_FIR_SZ;
	dll_cfg->dcc_stack_sz = DCC_STACK_SZ_L4;
	dll_cfg->dcc_search_range = DCC_SEARCH_RANGE;
	dll_cfg->dcc_fit_exclusion = DCC_FIT_EXCLUSION;
	dll_cfg->dcc_min_pd_range = DCC_MIN_PD_RANGE_L4;
	dll_cfg->dcc_map_overlap = (float)DCC_MAP_OVERLAP_L4;
	//    dll_cfg->dcc_linearity_level_theshold = DCC_LINEARITY_LEVEL_THRESHOLD;
	dll_cfg->dpd_pixel_unit = DPD_PIXEL_UNIT;
	dll_cfg->dpd_min_sensor_gain = (float)DPD_MIN_SENSOR_GAIN;
	dll_cfg->dpd_max_sensor_gain = (float)DPD_MAX_SENSOR_GAIN;
}
//////////////////////////////////////////Sensor////////////////////////////////////////////////////////////////////////////////////////////
BOOL COtpBase::S5K3L8_GetLSC()
{
//only for test
// 	BYTE _LSCData[512] = {0};
// 	if (m_LscGroup == 1)
// 	{
// 	    S5K3L8_ReadOTPPage(1, _LSCData);
// 		if (m_LscGroup == 1)
// 		{
// 			S5K3L8_ReadOTPPage(0x19,_LSCData);
// 			msleep(100);
// 			S5K3L8_ReadOTPPage(0x1A,_LSCData+64);
// 			msleep(100);
// 			S5K3L8_ReadOTPPage(0x1B,_LSCData+128);
// 			msleep(100);
// 			S5K3L8_ReadOTPPage(0x1C,_LSCData+192);
// 			msleep(100);
// 			S5K3L8_ReadOTPPage(0x1D,_LSCData+256);
// 			msleep(100);
// 			S5K3L8_ReadOTPPage(0x1E,_LSCData+320);
// 			msleep(100);
// 			S5K3L8_ReadOTPPage(0x1F,_LSCData+384);
// 			msleep(100);	
// 		}
// 	}
// 		if (m_LscGroup == 2)
// 		{
// 			S5K3L8_ReadOTPPage(0x12,_LSCData);
// 			msleep(100);
// 			S5K3L8_ReadOTPPage(0x13,_LSCData+64);
// 			msleep(100);
// 			S5K3L8_ReadOTPPage(0x14,_LSCData+128);
// 			msleep(100);
// 			S5K3L8_ReadOTPPage(0x15,_LSCData+192);
// 			msleep(100);
// 			S5K3L8_ReadOTPPage(0x16,_LSCData+256);
// 			msleep(100);
// 			S5K3L8_ReadOTPPage(0x17,_LSCData+320);
// 			msleep(100);
// 			S5K3L8_ReadOTPPage(0x18,_LSCData+384);
// 			msleep(100);
// 			S5K3L8_ReadOTPPage(0x19,_LSCData+448);
// 			msleep(100);
// 		}
// 		S5K3L8_ReadOTPPage(26, _LSCData);
// 
// 	if (m_LscGroup == 2)
// 	{
// 		S5K3L8_ReadOTPPage(19, _LSCData);
// 	}
// 	if ( (_LSCData[8]|_LSCData[7]|_LSCData[6]|_LSCData[5]|_LSCData[4]|_LSCData[3]|_LSCData[2]|_LSCData[1]|_LSCData[0]) != 0)
// 	{
// 		m_pInterface->AddLog(_T("OTP内已有数据！"), COLOR_RED);
// 		return FALSE;
// 	}
// 	return TRUE;
	//////////////////////////////////////////////////////////////////////////
	m_pInterface->AddLog(_T("Get 3L8 Sensor LSC start..."));
	CString Path;
	Path = GetModulePath()+_T("\\LSC\\Sensor\\3L8");
	SetCurrentDirectory(Path);

	CString DLLPath = Path+"\\LSCDLL_3L8_V21.dll";
	HINSTANCE h3L8LSCDll = LoadLibrary(DLLPath);

	typedef void (*SetParam)( int nPedestal, int nWidth, int nHeight, int nGr, int nR, 
		int nB, int nGb, double dbAlpha, double dbSeed, BOOL bLensModeling, BOOL bScooby );
	typedef void (*SetLensRI)( double RI_1, double RI_2, double RI_3, double RI_4,
		double RI_5, double RI_6, double RI_7, double RI_8,
		double RI_9, double RI_10, double RI_11);
	typedef BOOL(*RunFunc)(  BYTE *pRawBuffer, BYTE *pTrgBuffer, BOOL bSimulatedResults, BOOL b1SetUse, CString strWorkDir );
	typedef BOOL(*SaveOTPfile)( char *szFile );
	typedef BOOL(*SaveSRAMfile)( char *szFile );

	if (NULL == h3L8LSCDll)
	{
		info.Format(_T("找不到LSCDLL_3L8_V21.dll，请将此Dll放入程式目录:\n%s！"),DLLPath);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE ;
	}
	SetParam SetParameter_3L8 = (SetParam)GetProcAddress(h3L8LSCDll, "SetParameter_3L8");
	if (!SetParameter_3L8)
	{
		m_pInterface->AddLog(_T("找不到入口函数SetParam！"),COLOR_RED);
		return FALSE ;
	}


	//////////////////////////////////////////////////////////////////////////
	//合力泰不用LensRI,其他客户可以打开这段代码
	Path += L"\\LensRI.txt";
	FILE* fp;
	if (_wfopen_s(&fp,  Path, _T("rt")))  
	{
		info.Format(_T("%s内找不到LensRI.txt!"),Path);
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

	SetLensRI SetUserLensRI_3L8 = (SetLensRI)GetProcAddress(h3L8LSCDll, "SetUserLensRI_3L8");
	SetUserLensRI_3L8(pLensRI[0], pLensRI[1], pLensRI[2], pLensRI[3], pLensRI[4], pLensRI[5], pLensRI[6], pLensRI[7], pLensRI[8], pLensRI[9], pLensRI[10]);
	SAFE_DELETE_ARRAY(pLensRI);
	fclose(fp);
	//////////////////////////////////////////////////////////////////////////

	ImageInfo imgInfo = m_pInterface->GetImage();
	int width = imgInfo.width;
	int height= imgInfo.height;

	SetParameter_3L8(64, width, height,  m_LscTarget, m_LscTarget, m_LscTarget, m_LscTarget, 0.3, 0.01, 1, 0);
	msleep(100);
	BOOL SetGroup;
	if (m_LscGroup == 1)
	{
		SetGroup = TRUE;
	}
	if (m_LscGroup == 2)
	{
		SetGroup = FALSE;
	}

	CString strWorkPath  = GetModulePath()+"\\LSC\\Sensor\\3L8";
	CreateDirectory(strWorkPath, NULL);
	CString sfilename = strWorkPath + "\\LSC.raw";
	CString strOTPSetFile = strWorkPath + "\\3L8LSCOTP.sset";
	CString strSRAMSetFile = strWorkPath + "\\3L8LSCSRAM.sset"; 
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

	RunFunc Run_3L8 = (RunFunc)GetProcAddress(h3L8LSCDll, "Run_3L8");
	BOOL flag = Run_3L8(raw_buffer, raw_buffer, FALSE, SetGroup, strWorkPath);
	if (!flag)
	{
		SAFE_DELETE_ARRAY(raw_buffer);
		m_pInterface->AddLog(_T("Fail to run 3L8 LSC!"),COLOR_RED);
		return FALSE;		
	}

	SAFE_DELETE_ARRAY(raw_buffer);
	SaveOTPfile SaveOTPSetfile_3L8 = (SaveOTPfile)GetProcAddress(h3L8LSCDll, "SaveOTPSetfile_3L8");
	SaveOTPSetfile_3L8(CT2A(strOTPSetFile.GetBuffer(MAX_PATH)));
	strOTPSetFile.ReleaseBuffer();

	SaveSRAMfile SaveSRAMSetfile_3L8 = (SaveSRAMfile)GetProcAddress(h3L8LSCDll, "SaveSRAMSetfile_3L8");
	SaveSRAMSetfile_3L8(CT2A(strSRAMSetFile.GetBuffer(MAX_PATH)));
	strSRAMSetFile.ReleaseBuffer();

	if (!S5K3L8_ApplySRAM())  return FALSE;

	m_pInterface->AddLog(_T("Get 3L8 Sensor LSC pass!"),COLOR_BLUE);
	return TRUE;
}


///////////////////////////////////s5k3l8///////////////////////////////////////
BOOL COtpBase::S5K3L8_ApplySRAM()
{
	m_pInterface->RestartCamera(1);//close camera & stream off 
	SaveExp = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllExp"), 1);
	m_pInterface->GetImageSensor()->WriteExp(SaveExp);
	SaveGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"RecordExp",_T("ExpGain"), 1);
	m_pInterface->GetImageSensor()->WriteAnalogGain(SaveGain);

	CString strfile,tmp;
	CString strWorkPath  = GetModulePath()+"\\LSC\\Sensor\\3L8";
//	CreateDirectory(strWorkPath, NULL);
	tmp.Format(_T("\\3L8LSCSRAM.sset"));
	strfile = strWorkPath + tmp;

	CStdioFile file;
	if (!file.Open((strfile), CFile::modeRead | CFile::typeText))
	{
		m_pInterface->AddLog(_T("打开 3L8LSCSRAM.sset文件失败!"),COLOR_RED);
		return FALSE;		
	}

	CString strLine;
	int count1 =0; 
	int count2 = 0;

	i2c_write(0x6028, 0x2000);
	i2c_write(0x602a, 0x0cf4);
	i2c_write_byte(0x6f12,0x00);
	msleep(100);

	i2c_write(0x6028, 0x4000);
	i2c_write(0x602a, 0x0b00);
	i2c_write_byte(0x6f12, 0x01);
	msleep(200);

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
	msleep(100);

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

	msleep(100);
	i2c_write(0x0a00,0x0000);//make initial state
	return;
}

void COtpBase::S5K3L8_WriteOTPPage(int page, BYTE *Buff)
{
	// Set control register
	i2c_write(0x0100,0x0100);//stream   on 
	i2c_write(0x0A02,page*256); //set page
	//WRITE otp data to buff
	msleep(10);
	for(int i=0;i<64;i++)
	{
		i2c_write_byte(0x0a04+i,Buff[i]);
		msleep(10);
	}
	i2c_write(0x0a00,0x0300);//write start
	msleep(100);
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

void COtpBase::S5K3L8_SetDefaultAwb()
{
	USHORT defaultgain = m_wbGainReg.dataDefaultGain;
	i2c_write( 0x6028, 0x4000);
	i2c_write( 0x602a, 0x0100);
	i2c_write_byte( 0x6f12, 0x00);// Stream off
	msleep(100);

	i2c_write(m_wbGainReg.addrGr,defaultgain);
	i2c_write(m_wbGainReg.addrR,defaultgain);
	i2c_write(m_wbGainReg.addrB,defaultgain);
	i2c_write(m_wbGainReg.addrGb,defaultgain);

	i2c_write( 0x6028, 0x4000);
	i2c_write( 0x602a, 0x0100);
	i2c_write_byte( 0x6f12, 0x01 );// Stream on

	return;
}

void COtpBase::S5K3L8_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG)
{
	USHORT r_ratio, b_ratio;
	USHORT GAIN_DEFAULT_VAL = m_wbGainReg.dataDefaultGain;

	r_ratio = 512 * (m_goldenrg) /(wb.rg);
	b_ratio = 512 * (m_goldenbg) /(wb.bg);

	if( !r_ratio || !b_ratio)
	{
		m_pInterface ->AddLog(_T("请先Get AWB!"),COLOR_RED);
		return;
	}
	USHORT R_GAIN;
	USHORT B_GAIN;
	USHORT Gr_GAIN;
	USHORT Gb_GAIN;
	USHORT G_GAIN;


	if(r_ratio >= 512 )
	{
		if(b_ratio>=512) 
		{
			R_GAIN = (USHORT)(GAIN_DEFAULT_VAL * r_ratio / 512);
			G_GAIN = GAIN_DEFAULT_VAL;	
			B_GAIN = (USHORT)(GAIN_DEFAULT_VAL * b_ratio / 512);
		}
		else
		{
			R_GAIN =  (USHORT)(GAIN_DEFAULT_VAL * r_ratio / b_ratio);
			G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio);
			B_GAIN = GAIN_DEFAULT_VAL;	
		}
	}
	else 			
	{
		if(b_ratio >= 512)
		{
			R_GAIN = GAIN_DEFAULT_VAL;	
			G_GAIN =(USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio);
			B_GAIN =(USHORT)(GAIN_DEFAULT_VAL *  b_ratio / r_ratio);

		} 
		else 
		{
			Gr_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio );
			Gb_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio );

			if(Gr_GAIN >= Gb_GAIN)
			{
				R_GAIN = GAIN_DEFAULT_VAL;
				G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio );
				B_GAIN = (USHORT)(GAIN_DEFAULT_VAL * b_ratio / r_ratio);
			} 
			else
			{
				R_GAIN =  (USHORT)(GAIN_DEFAULT_VAL * r_ratio / b_ratio );
				G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio );
				B_GAIN = GAIN_DEFAULT_VAL;
			}
		}	
	}
	i2c_write( 0x6028, 0x4000);
	i2c_write( 0x602a, 0x0100);
	i2c_write_byte( 0x6f12, 0x00);// Stream off
	msleep(100);

	i2c_write(0x6028, 0x4000);	
	i2c_write(0x602a, 0x3058);
	i2c_write_byte(0x6f12, 0x01);

	i2c_write(0x602a, 0x0210);
	i2c_write(0x6f12, R_GAIN);

	i2c_write(0x602a, 0x0212);
	i2c_write(0x6f12, B_GAIN);

	i2c_write(0x602a, 0x020e);
	i2c_write(0x6f12, G_GAIN);

	i2c_write(0x602a, 0x0214);
	i2c_write(0x6f12, G_GAIN);

	i2c_write( 0x6028, 0x4000);
	i2c_write( 0x602a, 0x0100);
	i2c_write_byte( 0x6f12, 0x01 );// Stream on
	msleep(500);
	return;
}
//////////////////////////////Sensor 5E8//////////////////////////////////////
BOOL COtpBase::S5K5E8ReadOTPpage(int Page, USHORT address, BYTE *Data, int length)
{  

	BOOL bRet = TRUE;
	i2c_write_byte(0x0100, 0x00);
	msleep(5);
	i2c_write_byte(0x0a00, 0x04);
	i2c_write_byte(0x0a02, Page);
	i2c_write_byte(0x0a00, 0x01);
	msleep(1);
	for ( int i=0; i<length; i++ )
	{
		bRet &= i2c_read_byte((address+i), &Data[i]);
	}
	i2c_write_byte(0x0a00, 0x04);
	i2c_write_byte(0x0a00, 0x00);
 	i2c_write_byte(0x0100, 0x01);
 	msleep(5);
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
		msleep(5);
		i2c_write_byte(0x3b42, 0x68);
		i2c_write_byte(0x3b41, 0x01);
		i2c_write_byte(0x3b40, 0x00);
		i2c_write_byte(0x0a00, 0x04);
		i2c_write_byte(0x0a00, 0x03);
		i2c_write_byte(0x3b42, 0x00);
		i2c_write_byte(0x3b45, 0x01);
		i2c_write_byte(0x0a02, Page);
		i2c_write_byte(0x0a00, 0x03);
		msleep(5);
		for ( int i=0; i<length; i++ )
		{
			BYTE WriteEnding;
			i2c_read_byte(0x0A01, &WriteEnding);
			do 
			{
				msleep(0);
			} while ((WriteEnding & 0x02));
			i2c_write_byte((address+i), Data[i]);
		}
		BYTE WriteEnding;
		i2c_read_byte(0x0A01, &WriteEnding);
		do 
		{
			msleep(0);
		} while ((WriteEnding & 0x02));
		msleep(5);
		i2c_write_byte(0x0a00, 0x04);
		i2c_write_byte(0x0a00, 0x00);
		i2c_write_byte(0x3b40, 0x01);
		msleep(5);
		i2c_write_byte(0x0100, 0x01);
		msleep(10);
	}
	else
	{
		i2c_write_byte(0x0100, 0x00);
		msleep(5);
		i2c_write_byte(0x3b42, 0x68);
		i2c_write_byte(0x3b41, 0x01);
		i2c_write_byte(0x3b40, 0x00);
		i2c_write_byte(0x0a00, 0x04);
		i2c_write_byte(0x0a00, 0x03);
		i2c_write_byte(0x3b42, 0x00);
		i2c_write_byte(0x3b45, 0x01);
		i2c_write_byte(0x0a02, Page);
		i2c_write_byte(0x0a00, 0x03);
		msleep(5);
		for ( int i=0; i<length/2; i++ )
		{
			BYTE WriteEnding;
			i2c_read_byte(0x0A01, &WriteEnding);
			do 
			{
				msleep(0);
			} while ((WriteEnding & 0x02));
			i2c_write_byte((address+i), Data[i]);
		}
		BYTE WriteEnding;
		i2c_read_byte(0x0A01, &WriteEnding);
		do 
		{
			msleep(0);
		} while ((WriteEnding & 0x02));
		msleep(5);
		i2c_write_byte(0x0a00, 0x04);
		i2c_write_byte(0x0a00, 0x00);
		i2c_write_byte(0x3b40, 0x01);
		msleep(5);
		i2c_write_byte(0x0100, 0x01);
		msleep(10);

		i2c_write_byte(0x0100, 0x00);
		msleep(5);
		i2c_write_byte(0x3b42, 0x68);
		i2c_write_byte(0x3b41, 0x01);
		i2c_write_byte(0x3b40, 0x00);
		i2c_write_byte(0x0a00, 0x04);
		i2c_write_byte(0x0a00, 0x03);
		i2c_write_byte(0x3b42, 0x00);
		i2c_write_byte(0x3b45, 0x01);
		i2c_write_byte(0x0a02, Page);
		i2c_write_byte(0x0a00, 0x03);
		msleep(5);
		for ( int i=0; i<length/2; i++ )
		{
			BYTE WriteEnding;
			i2c_read_byte(0x0A01, &WriteEnding);
			do 
			{
				msleep(0);
			} while ((WriteEnding & 0x02));
			i2c_write_byte((address+length/2+i), Data[i+length/2]);
		}
		i2c_read_byte(0x0A01, &WriteEnding);
		do 
		{
			msleep(0);
		} while ((WriteEnding & 0x02));
		msleep(5);
		i2c_write_byte(0x0a00, 0x04);
		i2c_write_byte(0x0a00, 0x00);
		i2c_write_byte(0x3b40, 0x01);
		msleep(5);
		i2c_write_byte(0x0100, 0x01);
		msleep(10);
	}

	return TRUE;
}

BOOL COtpBase::S5K5E8_GetLSC()
{
	m_pInterface->AddLog(_T("Get 5E8 Sensor LSC start..."));
	CString Path;
	Path = GetModulePath()+_T("\\LSC\\Sensor\\5E8");
	SetCurrentDirectory(Path);

	CString DLLPath = Path+"\\LSCDLL_5E8_v22.dll";
	HINSTANCE h5E8LSCDll = LoadLibrary(DLLPath);

	typedef void (*SetParam)( int nPedestal, int nWidth, int nHeight, int nGr, int nR, 
		int nB, int nGb, double dbAlpha, double dbSeed, BOOL bLensModeling, BOOL bScooby );
	typedef void (*SetLensRI)( double RI_1, double RI_2, double RI_3, double RI_4,
		double RI_5, double RI_6, double RI_7, double RI_8,
		double RI_9, double RI_10, double RI_11);
	typedef BOOL(*RunFunc)( BYTE *pRawBuffer, BYTE *pTrgBuffer, BOOL bSimulatedResults, CString strWorkDir );
	typedef BOOL(*SaveOTPfile)( char *szFile );
	typedef BOOL(*SaveSRAMfile)( char *szFile );

	if (NULL == h5E8LSCDll)
	{
		info.Format(_T("找不到LSCDLL_5E8_v22.dll，请将此Dll放入程式目录:\n%s！"),DLLPath);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE ;
	}
	SetParam SetParameter_5E8 = (SetParam)GetProcAddress(h5E8LSCDll, "SetParameter_5E8");
	if (!SetParameter_5E8)
	{
		m_pInterface->AddLog(_T("找不到入口函数SetParam！"),COLOR_RED);
		return FALSE ;
	}

	Path += L"\\LensRI.txt";

	FILE* fp;
	if (_wfopen_s(&fp,  Path, _T("rt")))  
	{
		info.Format(_T("%s内找不到LensRI.txt!"),Path);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}

	double *pLensRI = new double[11];
	char strData[256];
	for (int i=0 ;i<11; ++i)
	{
		fscanf_s(fp,"%s",strData,6);
		sscanf_s(strData, "%lf", pLensRI+i);
	}
	SetLensRI SetUserLensRI_5E8 = (SetLensRI)GetProcAddress(h5E8LSCDll, "SetUserLensRI_5E8");
	SetUserLensRI_5E8(pLensRI[0], pLensRI[1], pLensRI[2], pLensRI[3],pLensRI[4], pLensRI[5], pLensRI[6], pLensRI[7],pLensRI[8], pLensRI[9], pLensRI[10]);
	delete[] pLensRI;
	fclose(fp);

	ImageInfo imgInfo = m_pInterface->GetImage();
	int width = imgInfo.width;
	int height= imgInfo.height;

	SetParameter_5E8(64, width, height,  m_LscTarget, m_LscTarget, m_LscTarget, m_LscTarget, 0.3, 0.01, 1, 0);
	msleep(100);

	CString strWorkPath  = GetModulePath()+"\\LSC\\Sensor\\5E8";
	CreateDirectory(strWorkPath, NULL);
	CString sfilename = strWorkPath + "\\LSC.raw";
	CString strOTPSetFile = strWorkPath + "\\5E8LSCOTP.sset";
	CString strSRAMSetFile = strWorkPath + "\\5E8LSCSRAM.sset"; 
//#ifdef  TestVersion 
	if (PathFileExists(sfilename)) 
	{
		if (!DeleteFile(sfilename))
		{
			m_pInterface->AddLog(_T("文件删除失败!"), COLOR_RED);
			return FALSE;
		}
	}
	SaveCurrentRaw10(sfilename,imgInfo.Raw_Buf,width,height);
//#endif
	FILE* fp_raw;
	if (_wfopen_s(&fp_raw,  sfilename, _T("rb")))  
	{
		info.Format(_T("打开文件失败%s!"),sfilename);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}

	BYTE* raw_buffer = NULL;
	if (fp_raw)
	{
		raw_buffer = new BYTE[width*height*2];
		fread((void*)raw_buffer,2,width*height,fp_raw);
		fclose(fp_raw);
	}

	RunFunc Run_5E8 = (RunFunc)GetProcAddress(h5E8LSCDll, "Run_5E8");
	BOOL flag = Run_5E8(raw_buffer, raw_buffer, FALSE, strWorkPath);
	if (!flag)
	{
		SAFE_DELETE_ARRAY(raw_buffer);
		m_pInterface->AddLog(_T("Fail to run 5E8 LSC!"),COLOR_RED);
		return FALSE;		
	}

	SAFE_DELETE_ARRAY(raw_buffer);
	SaveOTPfile SaveOTPSetfile_5E8 = (SaveOTPfile)GetProcAddress(h5E8LSCDll, "SaveOTPSetfile_5E8");
	SaveOTPSetfile_5E8(CT2A(strOTPSetFile.GetBuffer(MAX_PATH)));
	strOTPSetFile.ReleaseBuffer();

	SaveOTPfile SaveSRAMSetfile_5E8 = (SaveOTPfile)GetProcAddress(h5E8LSCDll, "SaveSRAMSetfile_5E8");
	SaveSRAMSetfile_5E8(CT2A(strSRAMSetFile.GetBuffer(MAX_PATH)));
	strSRAMSetFile.ReleaseBuffer();

	if (!S5K5E8_ApplySRAM())  return FALSE;

	m_pInterface->AddLog(_T("Get 5E8 Sensor LSC pass!"),COLOR_BLUE);
	return TRUE;
}

BOOL COtpBase::S5K5E8_ApplySRAM()
{
	m_pInterface->RestartCamera(1);//close camera & stream off  
	SaveExp = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllExp"), 1);
	m_pInterface->GetImageSensor()->WriteExp(SaveExp);
	SaveGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"RecordExp",_T("ExpGain"), 1);
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
// 	msleep(10);
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
					msleep(10);
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
// 	msleep(1000);
// 	BYTE readdata = 0;
// 
// 
// 	int i = 0;
// 	do 
// 	{
// 		i2c_read_byte(0x0100, &readdata);
// 		info.Format(L"0x%x",readdata);
// 		m_pInterface->AddLog(info,COLOR_RED);
// 		i2c_write_byte(0x0100, 0x01);//stream on
// 		msleep(10);
// 		i++;
// 	} while (readdata == 0 && i<20);
	msleep(10);

	return TRUE;
}

void COtpBase::S5K5E8_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG)
{
	USHORT r_ratio, b_ratio;

	r_ratio = 1024 * (m_goldenrg) /(wb.rg);
	b_ratio = 1024 * (m_goldenbg) /(wb.bg);

	if( !r_ratio || !b_ratio)
	{
		m_pInterface ->AddLog(_T("请先Get AWB!"),COLOR_RED);
		return;
	}
	USHORT R_GAIN;
	USHORT B_GAIN;
	USHORT Gr_GAIN;
	USHORT Gb_GAIN;
	USHORT G_GAIN;

	USHORT GAIN_DEFAULT_VAL = 0x0100;
	if(r_ratio >= 1024 )
	{
		if(b_ratio>=1024) 
		{
			R_GAIN = (USHORT)(GAIN_DEFAULT_VAL * r_ratio / 1024);
			G_GAIN = GAIN_DEFAULT_VAL;	
			B_GAIN = (USHORT)(GAIN_DEFAULT_VAL * b_ratio / 1024);
		}
		else
		{
			R_GAIN =  (USHORT)(GAIN_DEFAULT_VAL * r_ratio / b_ratio);
			G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 1024 / b_ratio);
			B_GAIN = GAIN_DEFAULT_VAL;	
		}
	}
	else 			
	{
		if(b_ratio >= 1024)
		{
			R_GAIN = GAIN_DEFAULT_VAL;	
			G_GAIN =(USHORT)(GAIN_DEFAULT_VAL * 1024 / r_ratio);
			B_GAIN =(USHORT)(GAIN_DEFAULT_VAL *  b_ratio / r_ratio);

		} 
		else 
		{
			Gr_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 1024 / r_ratio );
			Gb_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 1024 / b_ratio );

			if(Gr_GAIN >= Gb_GAIN)
			{
				R_GAIN = GAIN_DEFAULT_VAL;
				G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 1024 / r_ratio );
				B_GAIN = (USHORT)(GAIN_DEFAULT_VAL * b_ratio / r_ratio);
			} 
			else
			{
				R_GAIN =  (USHORT)(GAIN_DEFAULT_VAL * r_ratio / b_ratio );
				G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 1024 / b_ratio );
				B_GAIN = GAIN_DEFAULT_VAL;
			}
		}	
	}
	i2c_write_byte(0x0210, R_GAIN>>8);
	i2c_write_byte(0x0210+1, R_GAIN&0xff);

	i2c_write_byte(0x0212, B_GAIN>>8);
	i2c_write_byte(0x0212+1, B_GAIN&0xff);

	i2c_write_byte(0x020e, G_GAIN>>8);
	i2c_write_byte(0x020e+1, G_GAIN&0xff);

	i2c_write_byte(0x0214, G_GAIN>>8);
	i2c_write_byte(0x0214+1, G_GAIN&0xff);		
	msleep(500);
	return;
}

/////////////////////////////////////4H8/////////////////////////////////////
BOOL COtpBase::S5K4H8WriteOTPpage(int Page, USHORT address,BYTE *Data, int length)
{
	int writeresult = 0;

	// H/W Reset
	m_pInterface->bSensorEnable(3 ^ 3, 1);
	msleep(10);
	m_pInterface->bSensorEnable(3, 1);
	msleep(10);

	BYTE stram_flag = 0;
	i2c_read_byte(0x0100,&stram_flag);
	if (stram_flag == 0)
	{
		i2c_write_byte(0x0100,0x01);
	}

	i2c_write_byte( 0x0a00, 0x04 );
	i2c_write_byte( 0x0a02, Page );
	//otp enable and read start

	for (int i = 0; i < length; i++)
	{
		i2c_write_byte(  address+i, Data[i]);
	}
	msleep(5);
	i2c_write( 0x0a00, 0x0300);
	msleep(10);
	i2c_write_byte(0x0a00, 0x00);

	return TRUE;
}

BOOL COtpBase::S5K4H8ReadOTPpage(int Page, USHORT address,BYTE *Data, int length)
{
	BYTE stram_flag = 0;
	i2c_read_byte(0x0100,&stram_flag);
	if (stram_flag == 0)
	{
		i2c_write_byte(0x0100,0x01);
	}

	i2c_write_byte(0x0a02,Page);
	i2c_write( 0x0a00,0x0100); //otp enable and read start
	//read otp data to buff
	msleep(100 * m_sleeptime);
	for( int i = 0; i < length; i++ )
	{
		i2c_read_byte( address+i, Data+i );
	}
	msleep(50 * m_sleeptime);
	i2c_write(0x0a00,0x0000); //otp enable and read end	
	return TRUE;
}

BOOL COtpBase::S5K4H8_GetLSC()
{
	m_pInterface->AddLog(_T("Get 4H8 Sensor LSC start..."));
	CString Path;
	Path = GetModulePath()+_T("\\LSC\\Sensor\\4H8");
	SetCurrentDirectory(Path);

	CString DLLPath = Path+"\\LSCDLL_4H8_v24.dll";
	HINSTANCE h4H8LSCDll = LoadLibrary(DLLPath);

	typedef void (*SetParam)( int nPedestal, int nWidth, int nHeight, 
		int nGr, int nR, int nB, int nGb, double dbAlpha, double dbSeed, BOOL bLensModeling, BOOL bScooby );
	typedef void (*SetLensRI)( double RI_1, double RI_2, double RI_3, double RI_4,
		double RI_5, double RI_6, double RI_7, double RI_8,
		double RI_9, double RI_10, double RI_11);
	typedef BOOL(*RunFunc)( BYTE *pRawBuffer, BYTE *pTrgBuffer, BOOL bSimulatedResults, CString strWorkDir, BOOL b2ndSet, BOOL *is_SINGULAR, BOOL *bSEED_overflow );
	typedef BOOL(*SaveOTPfile)( char *szFile );
	typedef BOOL(*SaveSRAMfile)( char *szFile );

	if (NULL == h4H8LSCDll)
	{
		info.Format(_T("找不到LSCDLL_4H8.dll，请将此Dll放入程式目录:\n%s！"),DLLPath);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE ;
	}
	SetParam SetParameter = (SetParam)GetProcAddress(h4H8LSCDll, "SetParameter");
	if (!SetParameter)
	{
		m_pInterface->AddLog(_T("找不到入口函数SetParam！"),COLOR_RED);
		return FALSE ;
	}

	Path += L"\\LensRI.txt";
	FILE* fp;
	if (_wfopen_s(&fp,  Path, _T("rt")))  
	{
		info.Format(_T("%s内找不到LensRI.txt!"),Path);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}

	double *pLensRI = new double[11];
	char strData[256];
	for (int i=0 ;i<11; ++i)
	{
		fscanf_s(fp,"%s",strData,6);
		sscanf_s(strData, "%lf", pLensRI+i);
	}
	SetLensRI SetUserLensRI = (SetLensRI)GetProcAddress(h4H8LSCDll, "SetUserLensRI");
	SetUserLensRI(pLensRI[0], pLensRI[1], pLensRI[2], pLensRI[3],pLensRI[4], pLensRI[5], pLensRI[6], pLensRI[7],pLensRI[8], pLensRI[9], pLensRI[10]);
	delete[] pLensRI;
	fclose(fp);

	ImageInfo imgInfo = m_pInterface->GetImage();
	int width = imgInfo.width;
	int height= imgInfo.height;

	SetParameter(64, width, height,  m_LscTarget, m_LscTarget, m_LscTarget, m_LscTarget, 0.3, 0.01, TRUE, FALSE);
	msleep(100);

	CString strWorkPath  = GetModulePath()+"\\LSC\\Sensor\\4H8";
	CreateDirectory(strWorkPath, NULL);
	CString sfilename = strWorkPath + "\\LSC.raw";
	CString strOTPSetFile = strWorkPath + "\\4H8LSCOTP.ini";
	CString strSRAMSetFile = strWorkPath + "\\4H8LSCSRAM.ini"; 
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
	FILE* fp_raw;
	if (_wfopen_s(&fp_raw,  sfilename, _T("rb")))  
	{
		info.Format(_T("打开文件失败%s!"),sfilename);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}

	BYTE* raw_buffer = NULL;
	if (fp_raw)
	{
		raw_buffer = new BYTE[width*height*2];
		fread((void*)raw_buffer,2,width*height,fp_raw);
		fclose(fp_raw);
	}

	BOOL SetGroup = 0;
	if (m_LscGroup == 1)
	{
		SetGroup = FALSE;
	}
	else if (m_LscGroup == 2)
	{
		SetGroup = TRUE;
	}

	BOOL is_SINGULAR;
	BOOL bSEED_overflow;
	BOOL flag = FALSE;
	do 
	{
		RunFunc Run = (RunFunc)GetProcAddress(h4H8LSCDll, "Run");
	    flag = Run(raw_buffer, raw_buffer, FALSE, strWorkPath,SetGroup,&is_SINGULAR, &bSEED_overflow);
		if (!flag)
		{
			m_pInterface->AddLog(_T("Fail to run 4H8 LSC!"),COLOR_RED);
			break;	
		}
		if (is_SINGULAR == TRUE)
		{ 
			m_pInterface->AddLog(_T("It is SINGULAR(not invertible)!"),COLOR_RED);
			break;	
		}
		if( bSEED_overflow == 1 )
		{
			m_pInterface->AddLog(_T("Fail for LSC seed overflow!!"),COLOR_RED);
			break;	
		}

	} while (0);
	SAFE_DELETE_ARRAY(raw_buffer);
	if (!flag)
	{
		return FALSE;
	}

	SaveOTPfile SaveOTPSetfile = (SaveOTPfile)GetProcAddress(h4H8LSCDll, "SaveOTPSetfile");
    SaveOTPSetfile(CT2A(strOTPSetFile.GetBuffer(MAX_PATH)));
	strOTPSetFile.ReleaseBuffer();

	SaveSRAMfile SaveSRAMSetfile = (SaveSRAMfile)GetProcAddress(h4H8LSCDll, "SaveSRAMSetfile");
	SaveSRAMSetfile(CT2A(strSRAMSetFile.GetBuffer(MAX_PATH)));
	strSRAMSetFile.ReleaseBuffer();
	if (!S5K4H8_ApplySRAM())  return FALSE;
	m_pInterface->AddLog(_T("Get 4H8 Sensor LSC pass!"),COLOR_BLUE);
	return TRUE;
}

BOOL COtpBase::S5K4H8_ApplySRAM()
{
	m_pInterface->RestartCamera(1);//close camera & stream off  
	SaveExp = m_pInterface->GetConfigManager()->ReadConfigInt(L"OTP",_T("RecordDllExp"), 1);
	m_pInterface->GetImageSensor()->WriteExp(SaveExp);
	SaveGain = m_pInterface->GetConfigManager()->ReadConfigInt(L"RecordExp",_T("ExpGain"), 1);
	m_pInterface->GetImageSensor()->WriteAnalogGain(SaveGain);

	CString strfile,tmp;
	CString strWorkPath  = GetModulePath()+"\\LSC\\Sensor\\4H8";
	CreateDirectory(strWorkPath, NULL);
	tmp.Format(_T("\\4H8LSCSRAM.ini"));
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

	i2c_write(0x6028, 0x4000); 
	i2c_write(0x602A, 0x0B00);  //0B00 01,Auto lsc on
	i2c_write_byte( 0x6F12, 0x01 );
	msleep(100);
	i2c_write( 0x6028, 0x2000); 
	i2c_write( 0x602A, 0x0C40 );
	i2c_write_byte( 0x6F12, 0x00 );
	msleep(500);

	int k=0;
	while(file.ReadString(strLine))
	{
// 		k++;
// 		strLine = strLine.Left(9);
// 		strLine.TrimRight();
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
// 				if ((k>2) && intLSCAddr == 0x3457)
// 				{
// 					i2c_write_byte(intLSCAddr,intLSCValue);
// 					msleep(10);
// 				}
// 				else
// 				{
					i2c_write_byte(intLSCAddr,intLSCValue);
					k++;
//				}
			}
			else if (nlength == 8)
			{
				intLSCAddr = intVal>>16;
				intLSCValue = ( intVal&0xffff );			
				i2c_write(intLSCAddr, intLSCValue); 
				k++;
			}
		}
	}
	file.Close();

	i2c_write(0x6028,0x4000);
	i2c_write(0x602A,0x0100);
	i2c_write_byte(0x6f12, 0x01);
	msleep(10);
	return TRUE;
}

void COtpBase::S5K4H8_SetDefaultAwb()
{
	USHORT defaultgain = m_wbGainReg.dataDefaultGain;

	i2c_write(0x6028,0x4000);
	i2c_write(0x602a,0x3058);
	i2c_write_byte(0x6f12,0x01);

	i2c_write(m_wbGainReg.addrGr,defaultgain);
	i2c_write(m_wbGainReg.addrR,defaultgain);
	i2c_write(m_wbGainReg.addrB,defaultgain);
	i2c_write(m_wbGainReg.addrGb,defaultgain);

	return;
}

void COtpBase::S5K4H8_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG)
{
	USHORT r_ratio, b_ratio;

	r_ratio = 512 * (m_goldenrg) /(wb.rg);
	b_ratio = 512 * (m_goldenbg) /(wb.bg);

	if( !r_ratio || !b_ratio)
	{
		m_pInterface ->AddLog(_T("请先Get AWB!"),COLOR_RED);
		return;
	}
	USHORT R_GAIN;
	USHORT B_GAIN;
	USHORT Gr_GAIN;
	USHORT Gb_GAIN;
	USHORT G_GAIN;

	USHORT GAIN_DEFAULT_VAL = 0x0100;
	if(r_ratio >= 512 )
	{
		if(b_ratio>=512) 
		{
			R_GAIN = (USHORT)(GAIN_DEFAULT_VAL * r_ratio / 512);
			G_GAIN = GAIN_DEFAULT_VAL;	
			B_GAIN = (USHORT)(GAIN_DEFAULT_VAL * b_ratio / 512);
		}
		else
		{
			R_GAIN =  (USHORT)(GAIN_DEFAULT_VAL * r_ratio / b_ratio);
			G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio);
			B_GAIN = GAIN_DEFAULT_VAL;	
		}
	}
	else 			
	{
		if(b_ratio >= 512)
		{
			R_GAIN = GAIN_DEFAULT_VAL;	
			G_GAIN =(USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio);
			B_GAIN =(USHORT)(GAIN_DEFAULT_VAL *  b_ratio / r_ratio);

		} 
		else 
		{
			Gr_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio );
			Gb_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio );

			if(Gr_GAIN >= Gb_GAIN)
			{
				R_GAIN = GAIN_DEFAULT_VAL;
				G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio );
				B_GAIN = (USHORT)(GAIN_DEFAULT_VAL * b_ratio / r_ratio);
			} 
			else
			{
				R_GAIN =  (USHORT)(GAIN_DEFAULT_VAL * r_ratio / b_ratio );
				G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio );
				B_GAIN = GAIN_DEFAULT_VAL;
			}
		}	
	}
	i2c_write(0x6028,0x4000);
	i2c_write(0x602a,0x3058);
	i2c_write_byte(0x6f12,0x01);

	i2c_write(0x0210, R_GAIN);
	i2c_write(0x0212, B_GAIN);
	i2c_write(0x020e, G_GAIN);
	i2c_write(0x0214, G_GAIN);
	msleep(500);

	return;
}
////////////////////////////////////S5K3P3//////////////////////////////////////
void COtpBase::S5K3P3_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG)
{
	USHORT r_ratio, b_ratio;

	r_ratio = 512 * (m_goldenrg) /(wb.rg);
	b_ratio = 512 * (m_goldenbg) /(wb.bg);

	if( !r_ratio || !b_ratio)
	{
		m_pInterface ->AddLog(_T("请先Get AWB!"),COLOR_RED);
		return;
	}
	USHORT R_GAIN;
	USHORT B_GAIN;
	USHORT Gr_GAIN;
	USHORT Gb_GAIN;
	USHORT G_GAIN;

	USHORT GAIN_DEFAULT_VAL = 0x0100;
	if(r_ratio >= 512 )
	{
		if(b_ratio>=512) 
		{
			R_GAIN = (USHORT)(GAIN_DEFAULT_VAL * r_ratio / 512);
			G_GAIN = GAIN_DEFAULT_VAL;	
			B_GAIN = (USHORT)(GAIN_DEFAULT_VAL * b_ratio / 512);
		}
		else
		{
			R_GAIN =  (USHORT)(GAIN_DEFAULT_VAL * r_ratio / b_ratio);
			G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio);
			B_GAIN = GAIN_DEFAULT_VAL;	
		}
	}
	else 			
	{
		if(b_ratio >= 512)
		{
			R_GAIN = GAIN_DEFAULT_VAL;	
			G_GAIN =(USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio);
			B_GAIN =(USHORT)(GAIN_DEFAULT_VAL *  b_ratio / r_ratio);

		} 
		else 
		{
			Gr_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio );
			Gb_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio );

			if(Gr_GAIN >= Gb_GAIN)
			{
				R_GAIN = GAIN_DEFAULT_VAL;
				G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio );
				B_GAIN = (USHORT)(GAIN_DEFAULT_VAL * b_ratio / r_ratio);
			} 
			else
			{
				R_GAIN =  (USHORT)(GAIN_DEFAULT_VAL * r_ratio / b_ratio );
				G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio );
				B_GAIN = GAIN_DEFAULT_VAL;
			}
		}	
	}
	i2c_write( 0x6028, 0x4000);
	i2c_write( 0x602a, 0x0100);
	i2c_write_byte( 0x6f12, 0x00);// Stream off
	msleep(100);

	i2c_write(0x6028, 0x4000);	
	i2c_write(0x602a, 0x305c);
	i2c_write_byte(0x6f12, 0x01);

	i2c_write(0x602a, 0x0210);
	i2c_write(0x6f12, R_GAIN);

	i2c_write(0x602a, 0x0212);
	i2c_write(0x6f12, B_GAIN);

	i2c_write(0x602a, 0x020e);
	i2c_write(0x6f12, G_GAIN);

	i2c_write(0x602a, 0x0214);
	i2c_write(0x6f12, G_GAIN);

	i2c_write( 0x6028, 0x4000);
	i2c_write( 0x602a, 0x0100);
	i2c_write_byte( 0x6f12, 0x01 );// Stream on
	msleep(500);
	return;
}

//////////////////////////////////////OV13855////////////////////////////////////
void COtpBase::OV13855_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG)
{
	USHORT r_ratio, b_ratio;

	r_ratio = 512 * (m_goldenrg) /(wb.rg);
	b_ratio = 512 * (m_goldenbg) /(wb.bg);

	if( !r_ratio || !b_ratio)
	{
		m_pInterface ->AddLog(_T("请先Get AWB!"),COLOR_RED);
		return;
	}
	USHORT R_GAIN;
	USHORT B_GAIN;
	USHORT Gr_GAIN;
	USHORT Gb_GAIN;
	USHORT G_GAIN;

	USHORT GAIN_DEFAULT_VAL = 0x0400;
	if(r_ratio >= 512 )
	{
		if(b_ratio>=512) 
		{
			R_GAIN = (USHORT)(GAIN_DEFAULT_VAL * r_ratio / 512);
			G_GAIN = GAIN_DEFAULT_VAL;	
			B_GAIN = (USHORT)(GAIN_DEFAULT_VAL * b_ratio / 512);
		}
		else
		{
			R_GAIN =  (USHORT)(GAIN_DEFAULT_VAL * r_ratio / b_ratio);
			G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio);
			B_GAIN = GAIN_DEFAULT_VAL;	
		}
	}
	else 			
	{
		if(b_ratio >= 512)
		{
			R_GAIN = GAIN_DEFAULT_VAL;	
			G_GAIN =(USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio);
			B_GAIN =(USHORT)(GAIN_DEFAULT_VAL *  b_ratio / r_ratio);

		} 
		else 
		{
			Gr_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio );
			Gb_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio );

			if(Gr_GAIN >= Gb_GAIN)
			{
				R_GAIN = GAIN_DEFAULT_VAL;
				G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio );
				B_GAIN = (USHORT)(GAIN_DEFAULT_VAL * b_ratio / r_ratio);
			} 
			else
			{
				R_GAIN =  (USHORT)(GAIN_DEFAULT_VAL * r_ratio / b_ratio );
				G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio );
				B_GAIN = GAIN_DEFAULT_VAL;
			}
		}	
	}
	i2c_write_byte(0x5000, 0xeb);

	i2c_write_byte(0x5104, R_GAIN>>8);
	i2c_write_byte(0x5105, R_GAIN&0xff);

	i2c_write_byte(0x5102, G_GAIN>>8);
	i2c_write_byte(0x5103, G_GAIN&0xff);

	i2c_write_byte(0x5100, B_GAIN>>8);
	i2c_write_byte(0x5101, B_GAIN&0xff);

	msleep(500);
	return;
}

void COtpBase::OV13855_SetDefaultAwb()
{


	i2c_write_byte(0x5104, 0x04);
	i2c_write_byte(0x5105, 0x00);

	i2c_write_byte(0x5102, 0x04);
	i2c_write_byte(0x5103, 0x00);

	i2c_write_byte(0x5100, 0x04);
	i2c_write_byte(0x5101, 0x00);

	return;
}

////////////////////////////////////Hi1332//////////////////////////////////////
void COtpBase::HI1332InitOTP()
{
	i2c_write_byte(0x0a02,0x01);
	i2c_write_byte(0x0a00,0x00);
	msleep(100);
	i2c_write_byte(0x0f02,0x00);
	i2c_write_byte(0x071a,0x01);
	i2c_write_byte(0x071b,0x09);
	i2c_write_byte(0x0d04,0x01);
	i2c_write_byte(0x0d00,0x07);
	i2c_write_byte(0x003e,0x10);
	i2c_write_byte(0x0a00,0x01);
}

void COtpBase::HI1332ReleaseOTP()
{
	i2c_write_byte(0x0a00,0x00);
	msleep(100);
	i2c_write_byte(0x003e,0x00);
	i2c_write_byte(0x0a00,0x01);
}

void COtpBase::HI1332SingleRead(BYTE *pBuf, int addr_start,int addr_end)
{
	BYTE temp;
	for (int i=0;i<=addr_end-addr_start; i++)
	{
		i2c_write_byte(0x70a,((addr_start+i)>>8)&0xff);
		i2c_write_byte(0x70b,(addr_start+i)&0xff);
		i2c_write_byte(0x702,0x01);
		i2c_read_byte(0x708,&temp);
		pBuf[i] = temp;
	}	
}

void COtpBase::HI1332ContinuousRead(BYTE *pBuf, int addr_start,int addr_end)
{
	BYTE temp;
	i2c_write_byte(0x70a,((addr_start)>>8)&0xff);
	i2c_write_byte(0x70b,(addr_start)&0xff);
	i2c_write_byte(0x702,0x01);
	for (int i=0;i<=addr_end-addr_start; i++)
	{
		i2c_read_byte(0x708,&temp);
		pBuf[i] =temp;
	}
}

void COtpBase::HI1332SingleWrite(BYTE *pBuf, int addr_start,int addr_end)
{
	for (int i=0;i<=addr_end-addr_start; i++)
	{
		i2c_write_byte(0x70a,((addr_start+i)>>8)&0xff);
		i2c_write_byte(0x70b,(addr_start+i)&0xff);
		i2c_write_byte(0x702,0x02);
		i2c_write_byte(0x706,pBuf[i]);
		msleep(6);
	}
}

void COtpBase::HI1332ContinuousWrite(BYTE *pBuf, int addr_start,int addr_end)
{
	i2c_write_byte(0x70a,((addr_start)>>8)&0xff);
	i2c_write_byte(0x70b,(addr_start)&0xff);
	i2c_write_byte(0x702,0x02);
	for (int i=0;i<=addr_end-addr_start; i++)
	{

		i2c_write_byte(0x706,pBuf[i]);
		msleep(6);
	}
}

BOOL COtpBase::HI1332LargeWrite(BYTE *pBuf, int addr_start,int addr_end)
{
	int Count = 0;
	int Addr = 0;
	int EndCount = 0;
	BYTE DataRead = 0;
	BYTE Err[867] = {0};
	i2c_write_byte(0x070f, 0x03);
	i2c_write_byte(0x70a,((addr_start)>>8)&0xff);
	i2c_write_byte(0x70b,(addr_start)&0xff);
	i2c_write_byte(0x702,0x02);
	for (int i=0;i<=addr_end-addr_start; i++)
	{
		i2c_write_byte(0x706,pBuf[i]);
		msleep(1);

		i2c_read_byte(0x070c, &DataRead);
		if (DataRead != pBuf[i])
		{
			Err[Count] = i;
			Count++;
			i2c_write_byte(0x070c, 0);
		}
	}
	EndCount = Count;
	i2c_write_byte(0x070f, 0x14);
	for (Count=0; Count<EndCount; Count++)
	{
		i2c_write_byte(Err[Count],pBuf[Err[Count]]);
		msleep(5);

		i2c_read_byte(0x070c, &DataRead);
		if (DataRead != pBuf[Err[Count]])
		{
			info.Format(_T("Buff第%d值为%d仍然写入失败！"),Err[Count]+addr_start, pBuf[Err[Count]]);
			m_pInterface->AddLog(info,COLOR_RED);
			return FALSE;
		}
	}
	return TRUE;
}
 
void COtpBase::HI1332_SetDefaultAwb()
{
	USHORT defaultgain = m_wbGainReg.dataDefaultGain;

	WriteWBReg(m_wbGainReg.addrGrHigh, m_wbGainReg.addrGrLow, defaultgain);
	WriteWBReg(m_wbGainReg.addrRHigh,  m_wbGainReg.addrRLow,  defaultgain);
	WriteWBReg(m_wbGainReg.addrBHigh,  m_wbGainReg.addrBLow,  defaultgain);
	WriteWBReg(m_wbGainReg.addrGbHigh, m_wbGainReg.addrGbLow, defaultgain);

	return;	
}

void COtpBase::HI1332_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG)
{
	USHORT r_ratio, b_ratio;

	r_ratio = 512 * (m_goldenrg) /(wb.rg);
	b_ratio = 512 * (m_goldenbg) /(wb.bg);

	if( !r_ratio || !b_ratio)
	{
		m_pInterface ->AddLog(_T("请先Get AWB!"),COLOR_RED);
		return;
	}
	USHORT R_GAIN;
	USHORT B_GAIN;
	USHORT Gr_GAIN;
	USHORT Gb_GAIN;
	USHORT G_GAIN;

	USHORT GAIN_DEFAULT_VAL = m_wbGainReg.dataDefaultGain;
	if(r_ratio >= 512 )
	{
		if(b_ratio>=512) 
		{
			R_GAIN = (USHORT)(GAIN_DEFAULT_VAL * r_ratio / 512);
			G_GAIN = GAIN_DEFAULT_VAL;	
			B_GAIN = (USHORT)(GAIN_DEFAULT_VAL * b_ratio / 512);
		}
		else
		{
			R_GAIN =  (USHORT)(GAIN_DEFAULT_VAL * r_ratio / b_ratio);
			G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio);
			B_GAIN = GAIN_DEFAULT_VAL;	
		}
	}
	else 			
	{
		if(b_ratio >= 512)
		{
			R_GAIN = GAIN_DEFAULT_VAL;	
			G_GAIN =(USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio);
			B_GAIN =(USHORT)(GAIN_DEFAULT_VAL *  b_ratio / r_ratio);

		} 
		else 
		{
			Gr_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio );
			Gb_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio );

			if(Gr_GAIN >= Gb_GAIN)
			{
				R_GAIN = GAIN_DEFAULT_VAL;
				G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / r_ratio );
				B_GAIN = (USHORT)(GAIN_DEFAULT_VAL * b_ratio / r_ratio);
			} 
			else
			{
				R_GAIN =  (USHORT)(GAIN_DEFAULT_VAL * r_ratio / b_ratio );
				G_GAIN = (USHORT)(GAIN_DEFAULT_VAL * 512 / b_ratio );
				B_GAIN = GAIN_DEFAULT_VAL;
			}
		}	
	}
	i2c_write(0x0A04,0x015E);

	i2c_write_byte(0x050c,R_GAIN>>8);
	i2c_write_byte(0x050d,R_GAIN&0xff);
	i2c_write_byte(0x050e,B_GAIN>>8); 
	i2c_write_byte(0x050f,B_GAIN&0xff); 
	i2c_write_byte(0x0508,G_GAIN>>8); 
	i2c_write_byte(0x0509,G_GAIN&0xff); 
	i2c_write_byte(0x050a,G_GAIN>>8); 
	i2c_write_byte(0x050b,G_GAIN&0xff); 

	Hi1332_LSCData[858] = G_GAIN>>8;
	Hi1332_LSCData[859] = G_GAIN&0xff;
	Hi1332_LSCData[860] = G_GAIN>>8;
	Hi1332_LSCData[861] = G_GAIN&0xff;
	Hi1332_LSCData[862] = R_GAIN>>8;
	Hi1332_LSCData[863] = R_GAIN&0xff;
	Hi1332_LSCData[864] = B_GAIN>>8;
	Hi1332_LSCData[865] = B_GAIN&0xff;

	msleep(500);
	return;
}

BOOL COtpBase::HI1332_GetLSC()
{
	BOOL (* GetLscParaGain)(HI1332_IMG_INFO Img, DWORD* LscPara); 
	BOOL (* GetColorRatio)(HI1332_IMG_INFO Img, WORD* wColorRatio, float* qColorRatio);

	m_pInterface->AddLog(_T("Get HI1332 Sensor LSC start..."));

	HI1332_IMG_INFO m_img;
	m_img.bRawBuf = NULL;
	m_img.Width = 4224;
	m_img.Height = 3120;
	m_img.RoiSize[0] = 844;
	m_img.RoiSize[1] = 624;

	m_img.PxlOrder = 2;
	m_img.LscCorRatio[0] = m_LscTarget;
	m_img.LscCorRatio[1] = m_LscTarget;
	m_img.LscCorRatio[2] = m_LscTarget;
	m_img.LscCorRatio[3] = m_LscTarget;
	m_img.RefModule = 0;

	m_img.RoiPos[0] = ((int)((m_img.Width / 2) - (m_img.RoiSize[0] / 2)) / 2) * 2;	//StrX
	m_img.RoiPos[1] = ((int)((m_img.Height / 2) - (m_img.RoiSize[1] / 2)) / 2) * 2;	//StrY
	m_img.RoiPos[2] = ((int)((m_img.Width / 2) + (m_img.RoiSize[0] / 2)) / 2) * 2;	//EndX
	m_img.RoiPos[3] = ((int)((m_img.Height / 2) + (m_img.RoiSize[1] / 2)) / 2) * 2;	//EndY

	CString Path;
	Path = GetModulePath()+_T("\\LSC\\Sensor\\HI1332");
	SetCurrentDirectory(Path);

	CString DLLPath = Path+"\\Hi1332_OtpCal_DLL.dll";
	HINSTANCE hHI1332LSCDll = LoadLibrary(DLLPath);

	if (NULL == hHI1332LSCDll)
	{
		info.Format(_T("找不到Hi1332_OtpCal_DLL.dll，请将此Dll放入程式目录:\n%s！"),DLLPath);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE ;
	}

	GetLscParaGain = (BOOL(*)(HI1332_IMG_INFO, DWORD*))GetProcAddress(hHI1332LSCDll, "fnHi1332_GetLscParaGain");
	if (GetLscParaGain == NULL)
	{
		info.Format(L"找不到函数入口fnHi1332_GetLscParaGain"); 
		m_pInterface->AddLog(info, COLOR_RED);
		return FALSE;
	}

	GetColorRatio = (BOOL(*)(HI1332_IMG_INFO, WORD*, float*))GetProcAddress(hHI1332LSCDll, "fnHi1332_GetColorRatio");
	if (GetLscParaGain == NULL)
	{
		info.Format(L"找不到函数入口fnHi1332_GetColorRatio");
		m_pInterface->AddLog(info, COLOR_RED);
		return FALSE;
	}

	ImageInfo imgInfo = m_pInterface->GetImage();
	int width = imgInfo.width;
	int height= imgInfo.height;

	CString strWorkPath  = GetModulePath()+"\\LSC\\Sensor\\HI1332";
	CreateDirectory(strWorkPath, NULL);
	CString sfilename = strWorkPath + "\\LSC.raw";
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
	FILE* fp_raw;
	if (_wfopen_s(&fp_raw,  sfilename, _T("rb")))  
	{
		info.Format(_T("打开文件失败%s!"),sfilename);
		m_pInterface->AddLog(info,COLOR_RED);
		return FALSE;
	}

	BYTE* raw_buffer = NULL;
	if (fp_raw)
	{
		raw_buffer = new BYTE[width*height*2];
		fread((void*)raw_buffer,2,width*height,fp_raw);
		fclose(fp_raw);
	}

	m_img.bRawBuf = raw_buffer;

	Hi1332CaliBuffer m_Hi1332CalBuff;
	ZeroMemory(&m_Hi1332CalBuff,sizeof(Hi1332CaliBuffer));  

	GetColorRatio(m_img, m_Hi1332CalBuff.wColorRatio, m_Hi1332CalBuff.qColorRatio);

	//Get LSC para
	GetLscParaGain(m_img, m_Hi1332CalBuff.dwLscParaGain);
	
	SAFE_DELETE_ARRAY(raw_buffer);

	int CntRGr=0,CntBGb = 429;
	for (int i = 0; i<143; i += 1) //LSC_PARA_SIZE 13*11*6=143*6= 858 otp_lsc 8Reserved 1 CheckSum
	{
		Hi1332_LSCData[CntRGr++] = ((m_Hi1332CalBuff.dwLscParaGain[i + 143]) & 0xff);			// R
		Hi1332_LSCData[CntRGr++] = ((((m_Hi1332CalBuff.dwLscParaGain[i + 143]) >> 8) & 0x0f) | ((m_Hi1332CalBuff.dwLscParaGain[i]) & 0x0f) << 4);		// R | Gr
		Hi1332_LSCData[CntRGr++] = ((m_Hi1332CalBuff.dwLscParaGain[i] >> 4) & 0xff);			// Gr

		// OTP 429
		Hi1332_LSCData[CntBGb++] = ((m_Hi1332CalBuff.dwLscParaGain[i + 286]) & 0xff);			// B
		Hi1332_LSCData[CntBGb++] = ((((m_Hi1332CalBuff.dwLscParaGain[i + 286]) >> 8) & 0x0f) | ((m_Hi1332CalBuff.dwLscParaGain[i + 429]) & 0x0f) << 4); // B | Gb
		Hi1332_LSCData[CntBGb++] = ((m_Hi1332CalBuff.dwLscParaGain[i + 429] >> 4) & 0xff);		// Gb
	}

	if (!HI1332_ApplySRAM(m_Hi1332CalBuff))  return FALSE;
	m_pInterface->AddLog(_T("Get HI1332 Sensor LSC pass!"),COLOR_BLUE);
	return TRUE;
}

BOOL COtpBase::HI1332_ApplySRAM(Hi1332CaliBuffer &Hi1332LSCCaliBuffer)
{
	Hi1332PreLSCBuffer m_Hi1332PreLSCBuff;
	ZeroMemory(&m_Hi1332PreLSCBuff,sizeof(Hi1332PreLSCBuffer));  

	int i = 0;
	//Transfer LSC para to sram reg format
	for (i = 0; i<143; i++)
	{
		m_Hi1332PreLSCBuff.bSramLscParaGain_Gr_0x6000[i] =	(USHORT)Hi1332LSCCaliBuffer.dwLscParaGain[i + 143 * 0];			//Gr
		m_Hi1332PreLSCBuff.bSramLscParaGain_R_0x5000[i] =	(USHORT)Hi1332LSCCaliBuffer.dwLscParaGain[i  + 143 * 1];			//R
		m_Hi1332PreLSCBuff.bSramLscParaGain_B_0x5000[i] =	(USHORT)Hi1332LSCCaliBuffer.dwLscParaGain[i + 143 * 2];			//B
		m_Hi1332PreLSCBuff.bSramLscParaGain_Gb_0x6000[i] =	(USHORT)Hi1332LSCCaliBuffer.dwLscParaGain[i  + 143 * 3];			//Gb		
	}

	for(i=0;i<143;i++)
	{

		if(i2c_write(0x5000+2*i,m_Hi1332PreLSCBuff.bSramLscParaGain_R_0x5000[i])!=TRUE)
		{
			m_pInterface ->AddLog(_T("Write SRAM NG"),COLOR_RED);
			return FALSE;
		}

		if(i2c_write(0x5000+2*i+286,m_Hi1332PreLSCBuff.bSramLscParaGain_B_0x5000[i])!=TRUE)
		{
			m_pInterface ->AddLog(_T("Write SRAM NG"),COLOR_RED);
			return FALSE;
		}

		if(i2c_write(0x6000+2*i,m_Hi1332PreLSCBuff.bSramLscParaGain_Gr_0x6000[i])!=TRUE)
		{
			m_pInterface ->AddLog(_T("Write SRAM NG"),COLOR_RED);
			return FALSE;
		}

		if(i2c_write(0x6000+2*i+286,m_Hi1332PreLSCBuff.bSramLscParaGain_Gb_0x6000[i])!=TRUE)
		{
			m_pInterface ->AddLog(_T("Write SRAM NG"),COLOR_RED);
			return FALSE;
		}

	}

	i2c_write(0x0A04,0x015E);
	msleep(100);
	m_pInterface ->AddLog(_T("Apply Hi1332 SRAM End."),COLOR_BLACK);
	return TRUE;


}


