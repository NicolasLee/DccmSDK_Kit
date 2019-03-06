#pragma once

#include "DccmBase.h"
#include "OptionDlg.h"
#include "Inspection.h"
#include "StringProc.h"
#include "PDAFCalibrationTools_Dll.h"

#pragma comment(lib,"PDAFCalibrationTools_RevL_Dll.lib")

//#define  TestVersion 1
enum ZC533_Type
{
	ZC533_READ		 = 0, //0x00
	ZC533_WRITE	 = 1, //0x01
	ZC533_PAGE_ERASE= 2, //0x10
	ZC533_ALL_ERASE = 3, //0x11
};

typedef struct _QCdccmap_t
{
	unsigned short q_format;
	unsigned short width;
	unsigned short height;
	unsigned short dm[DCC_MAP_MAX_SZ];
	unsigned long checksum;
} QCdccmap_t;

class PD_CALI_Qual_2_Inspection :public Inspection
{
public:
	PD_CALI_Qual_2_Inspection(DccmControl_Interface* pInterface, LPCTSTR lpszName);
	~PD_CALI_Qual_2_Inspection(void);

	int Testing();       //子类重载测试代码放放在此函数
	int Initialize();      //子类重载 初始化代码
	int Finalize();      //子类重载 初始化

	int LoadOption();
	int SaveOption();

	LPCTSTR GetReportHeaders();
	LPCTSTR GetReportLowerLimits();
	LPCTSTR GetReportUpperLimits() ;
	LPCTSTR GetReportContents() ;

private:
	OptionDlg* pDlg;

public:
	CString szStr;
	int FrameCnt;
	int CaptureSize;
	BOOL bInit;
	UINT Inf_code;
	UINT Mac_code;
	UINT Mid_code;
	BOOL Lock_OneTime;
	CString FileFolderPath;
//	short ** RawArray;
	int* lensPos;
	int CaptureFrame;
	int inf_msb_reg;
	int inf_lsb_reg;
	int mac_msb_reg;
	int mac_lsb_reg;
	CString m_Ini_Name;
	int m_PageSize;
	int m_Dev_Addr;
	int m_Reg_Addr;
	int m_SleepTime;
	int m_I2cMode;
	int m_CodeSPC;
	CString info;

	int ZC533_Erase_lock;
	int ZC533_LastPage;

	//Qual PDAF
	int MinPixelAvg;
	int MaxPixelAvg;
	int MinPDAvg;
	int MaxPDAvg;
	int MinGain;
	int MaxGain;
	int MaxPixelThresh;
	int GainDiff;
	int OTPFormat_Type; //0 --> Previous 1D, 1 ---> Previous 2D, 2 ---> Unitied Version 1D or 2 ---> Unitied Version 2D

// 	PDAFPattern PDAFPatternValue;
// 	GainMap1DDataStruct GainMapData1D;
// 	GainMap2DDataStruct GainMapData2D;
// 	GainMapStruct GainMap;
// 	// Calibration Verification Parameter settings:
// 	GainMapVerifyStruct gmvs;
	CString Imagefilename[10];

	int InfinityCode;
	int MacroCode;

	gainmap_t       gain_map;

	int  GetAFcode(void);
	void SaveRawImage(USHORT* raw_buf, int width, int height, CString& path);
	int  PD_CALI_Proc2(void);
	int  PD_CALI_Verify_Proc2(int size);	// call dll for data verify
	void GetErrorCString(int code);

	int  EEPROM_S5K3M2(UCHAR* data, int size);
	int  EEPROM_S5K3L8(UCHAR* data, int size);

	BOOL GT24C64_Read(USHORT address,unsigned char *Data,int size);
	BOOL GT24C64_Programming(USHORT address,unsigned char *Data,int size);

	BOOL ZC533_Page_Erase(BYTE page);
	BOOL ZC533_All_Erase();
	BOOL ZC533_Read(USHORT address,unsigned char *Data,int size);
	BOOL ZC533_Programming(USHORT address,unsigned char *Data,int size);

	BOOL DW9763_Page_Erase(BYTE page);
	BOOL DW9763_All_Erase();
	BOOL DW9763_Read(USHORT address,unsigned char *Data,int size);
	BOOL DW9763_Programming(USHORT address,unsigned char *Data,int size);

	BOOL DW9767_Page_Erase(BYTE page);
	BOOL DW9767_All_Erase();
	BOOL DW9767_Read(USHORT address,unsigned char *Data,int size);//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop /* = FALSE */
	BOOL DW9767_Programming(USHORT address,unsigned char *Data,int size);//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page

	BOOL FM24C64_Read(USHORT address,unsigned char *Data,int size);
	BOOL FM24C64_Programming(USHORT address,unsigned char *Data,int size);

	BOOL FP5518_All_Erase();
	BOOL FP5518_Read(USHORT address, BYTE *value, int length);
	BOOL FP5518_Programming(USHORT address, BYTE *value, int length);

	BOOL CheckEqual(BYTE *data1, BYTE *data2, int num);

	BOOL CheckOTPIsBurning();
	int  Display_GmCheckResult(int GainMapFlag);
	void print_rc(unsigned int rc);

	///////////////////////////////////Normal///////////////////////////////////////
	void SaveCurrentRaw10(CString filename,USHORT *buff, int width, int height);
	void SaveCurrentRaw8(CString filename,BYTE *buff, int width, int height);
	void Raw10toRaw8(USHORT *Src, BYTE *Dest, long number);
	CString GetModulePath();
	BYTE SumCalc(BYTE* pVal, int valSize, int mod,int add);
};
