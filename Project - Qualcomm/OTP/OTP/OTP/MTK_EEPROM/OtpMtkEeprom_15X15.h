#pragma once

#include "..\\OtpBase.h"
#include <vector>

using namespace std;
#define LSC_MTK_SIZE  1868	//15*15
class COtpMtkEeprom_15X15: public COtpBase
{
public:
	COtpMtkEeprom_15X15(void);
	~COtpMtkEeprom_15X15(void);

	void SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist);
	BOOL create_lsc_group_Mtk(BYTE* image_data,BYTE *lencData,UINT width,UINT height,UINT lsc_size);		
	BOOL save_raw_data(BYTE* image_data, CString raw_name,UINT width,UINT height);

};

