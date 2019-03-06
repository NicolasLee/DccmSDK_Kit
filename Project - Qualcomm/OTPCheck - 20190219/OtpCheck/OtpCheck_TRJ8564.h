#pragma once
#include "OtpCheckBase.h"

class OtpCheck_TRJ8564M :public OtpCheckBase
{
public:
	OtpCheck_TRJ8564M(void);
	~OtpCheck_TRJ8564M(void);

private:

	int CheckGroup(BYTE flag);
	int CheckSum(BOOL flag,int flagaddr, int sumaddr, int datasize,  int gap,  CString str, int validgroup);	//��һ��checksum�����޷���������checksum�������������������checksum����
	void SaveOTPData();	

};

