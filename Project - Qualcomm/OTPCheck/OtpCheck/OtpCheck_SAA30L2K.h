#pragma once
#include "OtpCheckBase.h"

class OtpCheck_SAA30L2K :public OtpCheckBase
{
public:
	OtpCheck_SAA30L2K(void);
	~OtpCheck_SAA30L2K(void);

private:

	int CheckGroup(BYTE flag);
	int CheckSum(BOOL flag,int flagaddr, int sumaddr, int datasize,  int gap,  CString str, int validgroup);	//��һ��checksum�����޷���������checksum�������������������checksum����
	void SaveOTPData();	

};

