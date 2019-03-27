#pragma once
#include "OtpCheckBase.h"

class OtpCheck_CB801C :public OtpCheckBase
{
public:
	OtpCheck_CB801C(void);
	~OtpCheck_CB801C(void);

private:

	int CheckGroup(BYTE flag);
	
	int CheckSum(BOOL flag,int flagaddr, int sumaddr, int datasize,  int gap,  CString str, int validgroup);	//若一个checksum函数无法满足所有checksum情况，请添加其他情况的checksum函数
	void SaveOTPData();	
	int CheckValue();
	int GetInfAFCode();
	int GetMacAFCode();



};

