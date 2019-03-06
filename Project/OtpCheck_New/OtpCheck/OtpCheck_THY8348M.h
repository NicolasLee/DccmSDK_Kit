#pragma once

#include "OtpCheckBase.h"


class OtpCheck_THY8348M : public OtpCheckBase
{
public:
	OtpCheck_THY8348M(void);
	~OtpCheck_THY8348M(void);

	virtual BOOL Check_ModuleInfo();
	virtual BOOL Check_AWB();
	virtual BOOL Check_LSC();
	virtual BOOL Check_AF();
	virtual BOOL Check_PDAF();

#ifdef IIC_SENSOR_THY8348M
	virtual BOOL ReadSensor_Byte(WORD reg, BYTE* val);	// ��Sensorָ���Ĵ�����ȡһ��BYTE
	virtual BOOL WriteSensor_Byte(WORD reg, BYTE val);	// ��Sensorд��Ĵ���д��һ��BYTE
	virtual BOOL ReadSensor_Word(WORD reg, WORD* val);	// ��Sensorָ���Ĵ�����ȡ����BYTE
	virtual BOOL WriteSensor_Word(WORD reg, WORD val);		// ��Sensorд��Ĵ���д�����BYTE
	virtual BYTE ReadSensor_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BOOL bStop = FALSE);
	virtual BYTE WriteSensor_MultiBytes(WORD reg, BYTE regSize, BYTE *pVal, WORD valSize);
#else
	virtual BOOL ReadEeprom_Byte(WORD reg, BYTE* val); //��SENSORָ���Ĵ�����ȡһ��BYTE
	virtual BOOL WriteEeprom_Byte(WORD reg, BYTE val); //��SENSORд��Ĵ���д��һ��BYTE
/*	virtual BOOL ReadEeprom_Word(WORD reg, WORD* val); //��SENSORָ���Ĵ�����ȡ����BYTE
	virtual BOOL WriteEeprom_Word(WORD reg, WORD val); //��SENSORд��Ĵ���д�����BYTE
*/	virtual BYTE ReadEeprom_MultiBytes(USHORT reg, BYTE regSize, BYTE *pVal, USHORT valSize, BOOL bStop = FALSE);
	virtual BYTE WriteEeprom_MultiBytes(USHORT reg, BYTE regSize, BYTE *pVal, USHORT valSize);
#endif

private:
	virtual	BOOL CheckFlag_ModuleInfo();	// 0x01: Valid ����������Ϊ�ջ���Ч
	virtual	BOOL CheckFlag_AWB();			// 0x01: Valid ����������Ϊ�ջ���Ч
	virtual	BOOL CheckFlag_LSC();			// 0x01: Valid ����������Ϊ�ջ���Ч
	virtual	BOOL CheckFlag_AF();			// 0x01: Valid ����������Ϊ�ջ���Ч
	virtual	BOOL CheckFlag_PDAF();			// 0x01: Valid ����������Ϊ�ջ���Ч
	BOOL CheckFlag_PDAF_Coef_K();			// 0x01: Valid ����������Ϊ�ջ���Ч

	virtual	BOOL CheckSum_ModuleInfo();		// Sum (from 0x0001 to 0x000A)%256
	virtual	BOOL CheckSum_AWB();			// Sum (from 0x0011 to 0x001C)%256
	virtual	BOOL CheckSum_LSC();			// valSize of LSC is 1768 Bytes 17 x 13 x 4 (channel) x 2 (bytes)= 1768 Sum(0x0021 ~ 0x0708)%25
	virtual	BOOL CheckAbnormal();           // ��������Ƿ�����Ϊ0��ff
	virtual	BOOL CheckSum_AF();				// Sum(0x0741 ~ 0x0744)%256
	virtual	BOOL CheckSum_PDAF();			// Sum(0x0781 ~ 0x0B02)%256
	BOOL CheckSum_PDAF_Coef_K();			// Sum(0x0B41 ~ 0x0B43)%256
	virtual BYTE SumCalc(BYTE* pVal, int valSize);// Sum()%256

	virtual	BOOL CheckCode_Stc();				
	virtual	BOOL CheckCode_Inf();
	virtual	BOOL CheckCode_Mac();
};

