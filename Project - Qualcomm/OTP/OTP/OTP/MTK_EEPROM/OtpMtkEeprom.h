#ifndef OTP_MTK_H
#define OTP_MTK_H
//B/Gr/Gb/R:0¡¢1¡¢2¡¢3
#pragma once

#include "..\\OtpBase.h"
#include <vector>
using namespace std;

class COtpMtkEeprom : public COtpBase
{
public:	
	COtpMtkEeprom();
	virtual ~COtpMtkEeprom();
	
	void SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist);
	BOOL create_lsc_group_Mtk(BYTE* image_data,BYTE *lencData,UINT width,UINT height);		
	BOOL save_raw_data(BYTE* image_data, CString raw_name,UINT width,UINT height);
	void Init(CString RawName,int nSize);
private:
	int nLscSize;
	CString input_rawname;
	CString shading_cal;
	CString shading_ver;
	CString shading_val;
	CString shading_result;
};

#endif  /* OTP_MTK_H */
