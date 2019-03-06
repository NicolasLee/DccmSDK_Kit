#pragma once
#include "OtpCheckBase.h"

class OtpCheck_FYD8853M :public OtpCheckBase
{
public:
	OtpCheck_FYD8853M(void);
	~OtpCheck_FYD8853M(void);

private:

	int CheckGroup(BYTE flag);
	int CheckSum(BOOL flag,int flagaddr, int sumaddr, int datasize,  int gap,  CString str, int validgroup);	//��һ��checksum�����޷���������checksum�������������������checksum����
	void SaveOTPData();	

};

