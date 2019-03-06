#pragma once

#include "DccmBase.h"
#include "OptionDlg.h"
#include "Inspection.h"
#include "StringProc.h"
#include "PDAFCalibrationTools_RevJ.h"

#pragma comment(lib,"PDAFCalibrationTools_RevJ_Dll.lib")

//#define  TestVersion 1
enum ZC533_Type
{
	ZC533_READ		 = 0, //0x00
	ZC533_WRITE	 = 1, //0x01
	ZC533_PAGE_ERASE= 2, //0x10
	ZC533_ALL_ERASE = 3, //0x11
};

class PD_CALI_Qual_3_Inspection :public Inspection
{
public:
	PD_CALI_Qual_3_Inspection(DccmControl_Interface* pInterface, LPCTSTR lpszName);
	~PD_CALI_Qual_3_Inspection(void);

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
	short ** RawArray;
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
		 
	PDAFPattern PDAFPatternValue;
	GainMap1DDataStruct GainMapData1D;
	GainMap2DDataStruct GainMapData2D;
	GainMapStruct GainMap;
	// Calibration Verification Parameter settings:
	GainMapVerifyStruct gmvs;
	CString Imagefilename[4];
	BOOL verifylock;

	int InfinityCode;
	int MacroCode;
	int vericode;

	int  GetAFcode(void);
	BOOL LoadMtkIni(void);
	void PDAFSaveProc3Bin(char* data, int size);
	void SaveRawImage(USHORT* raw_buf, int width, int height, CString& path);
	int  PD_CALI_Proc3(void);
	int  PD_CALI_Verify_Proc3(int size);	// call dll for data verify
	void GetErrorCString(int code);
	BOOL SaveDAC(int *lensPos);

	int  EEPROM_S5K3M2(UCHAR* data, int size);
	int  EEPROM_S5K3L8(UCHAR* data, int size);

	BOOL GT24C64_Read(USHORT address,unsigned char *Data,int size);
	BOOL GT24C64_Programming(USHORT address,unsigned char *Data,int size);

	BOOL ZC533_Page_Erase(BYTE page);
	BOOL ZC533_All_Erase();
	BOOL ZC533_Read(USHORT address,unsigned char *Data,int size);
	BOOL ZC533_Programming(USHORT address,unsigned char *Data,int size);

	BOOL CheckEqual(BYTE *data1, BYTE *data2, int num);

	BOOL CheckOTPIsBurning();
	int  Display_GmCheckResult(int GainMapFlag);

	///////////////////////////////////Normal///////////////////////////////////////
	void SaveCurrentRaw10(CString filename,USHORT *buff, int width, int height);
	void SaveCurrentRaw8(CString filename,BYTE *buff, int width, int height);
	void Raw10toRaw8(USHORT *Src, BYTE *Dest, long number);
	CString GetModulePath();
};
