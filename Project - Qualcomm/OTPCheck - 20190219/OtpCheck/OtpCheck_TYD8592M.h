#pragma once
#include "OtpCheckBase.h"

class OtpCheck_TYD8592M :public OtpCheckBase
{
public:
	OtpCheck_TYD8592M(void);
	~OtpCheck_TYD8592M(void);

private:
	int InitProject();
    int EndProject();

	int CheckGroup(BYTE flag);
	int CheckSum(BOOL flag,int flagaddr, int sumaddr, int datasize,  int gap,  CString str, int validgroup);	//��һ��checksum�����޷���������checksum�������������������checksum����
	void SaveOTPData();	

};

