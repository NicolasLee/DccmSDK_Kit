#pragma once
#include "OtpCheckBase.h"

class OtpCheck_CMK8440 :public OtpCheckBase
{
public:
	OtpCheck_CMK8440(void);
	~OtpCheck_CMK8440(void);

private:

	int CheckGroup(BYTE flag);
	int CheckSum(BOOL flag,int flagaddr, int sumaddr, int datasize,  int gap,  CString str, int validgroup);	//��һ��checksum�����޷���������checksum�������������������checksum����
	void SaveOTPData();	
	int CheckValue();
	int GetInfAFCode();
	int GetMacAFCode();

};

