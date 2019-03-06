#pragma once
#include "OtpCheckBase.h"

class OtpCheck_TYD8583M :public OtpCheckBase
{
public:
	OtpCheck_TYD8583M(void);
	~OtpCheck_TYD8583M(void);

private:
	int InitProject();
    int EndProject();

	int CheckGroup(BYTE flag);
	int CheckSum(BOOL flag,int flagaddr, int sumaddr, int datasize,  int gap,  CString str, int validgroup);	//��һ��checksum�����޷���������checksum�������������������checksum����
	int CheckSum_Word(BOOL flag,int flagaddr, int sumaddr, int datasize,  int gap,  CString str, int validgroup);
	void SaveOTPData();	

};

