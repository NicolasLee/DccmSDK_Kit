#include "StdAfx.h"
#include "Sensor.h"
CSensor::CSensor(void)
{
}

CSensor::~CSensor(void)
{
}

////////////////////////////////////Sensor 5E8//////////////////////////////////////
BOOL CSensor::S5K5E8ReadOTPpage(int Page, USHORT address, BYTE *Data, int length)
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

BOOL CSensor::S5K5E8WriteOTPpage(int Page, USHORT address, BYTE *Data, int length)
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