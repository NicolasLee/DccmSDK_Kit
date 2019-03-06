#pragma once
#include "OTP\OtpBase.h"

class CSensor : public COtpBase
{
public:
	CSensor(void);
	~CSensor(void);

	//5E8
	BOOL S5K5E8ReadOTPpage(int Page, USHORT address, BYTE *Data, int length);
	BOOL S5K5E8WriteOTPpage(int Page, USHORT address, BYTE *Data, int length);
};