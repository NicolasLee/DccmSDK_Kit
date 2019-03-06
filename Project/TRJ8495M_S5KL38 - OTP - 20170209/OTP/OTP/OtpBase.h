#pragma once

#include "OtpDef.h"
#include "DccmBase.h"
#include "ImageProc.h"
#include "..\ShadingCalc.h"


#define  TestVersion 
#define Raw8Data

enum MTK_SIZE	
{
	//MTK
	SIZE_MTK_LSC = 1868,
	SIZE_MTK_PDAF_PROC1 = 496,
	SIZE_MTK_PDAF_PROC2 = 908,

	//Qual
	SIZE_QUAL_LSC = 1768,
	SIZE_QUAL_PDAF_PROC1 = 1078,
	SIZE_QUAL_PDAF_PROC2 = 2,
};

enum ZC533_Type
{
	ZC533_READ		 = 0, //0x00
	ZC533_WRITE	 = 1, //0x01
	ZC533_PAGE_ERASE= 2, //0x10
	ZC533_ALL_ERASE = 3, //0x11
};
// base otp class of PlatfomrTool and inner Sensor

#define I2C_RETRY_NUM    4
#define I2C_RETRY_WAIT   1  // 1ms

class COtpBase
{
public:
	DccmControl_Interface* m_pInterface;
	CString m_szStr;

	void SetRoi(CRect &roi, UINT width, UINT height, BYTE roiCalcMode, UINT x=0, UINT y=0, UINT roiWidth=0, UINT roiHeight=0);
	void FormatRoiRect(CRect &rt, UINT width, UINT height);
	BOOL CheckEqual(BYTE *data1, BYTE *data2, int num);
public:	
	virtual BOOL InitProgram(void);
	virtual BOOL StartProgram(void);
	virtual BOOL EndProgram(void);
	///////////////////////////ÉÕÂ¼Á÷³Ì/////////////////////////////////////////
	virtual BOOL LoadDll(void);
	virtual BOOL EraseAllData(void);
	virtual void SetWBDefaultGain(void);
	virtual BOOL InitOTP(void);

	BOOL GetLSC(void);
	virtual BOOL GetAWB(void) = 0 ;
	virtual BOOL CheckOTPAWB(void) = 0 ;
	virtual BOOL WriteOTP(void) = 0;
	virtual BOOL WriteOTP(vector<double>& vData) = 0;
	virtual void DisableLenc(void);
	virtual BOOL CheckOTP(void) = 0;
	virtual BOOL VerifyLSC(void) = 0;
	virtual BOOL VerifyPreWB(void) = 0;
	virtual BOOL VerifyAWB(void) = 0;	
    virtual BOOL FreeDll(void);

	////////////////////////////////AWB///////////////////////////////////////
	virtual BOOL InitAWB(void);
     virtual BOOL GetAWBInfo(tagAwbRatio* wb);
	void WriteWBReg(USHORT addrHigh, USHORT addrLow, USHORT gain);

	//Qual
	BOOL Qual_GetAWBData();
	BOOL Qual_GetAWBInfo(tagAwbRatio* wb);
	///////////////////////////////LSC/////////////////////////////////////////
	//MTK
    BOOL MTK_GetLSC(void);
	BOOL MTK_GetLSCData(void);
	BOOL MTK_ApplyLSC(void);

	//Qual
	BOOL Qual_GetLSC(void);
    BOOL ReadQualLSCDataFromTXT(float *R_Channel,float *Gr_Channel,float *Gb_channel,float *B_Channel);
	BOOL QualLSCDataCheck(BYTE *data);

	//Sensor LSC
	//S5K3L8
	BOOL S5K3L8_GetLSC(void);
	BOOL S5K3L8_ApplyLSC(void);

	//S5K5E8
	BOOL S5K5E8_GetLSC(void);
	BOOL S5K5E8_ApplyLSC(void);
	//////////////////////////////Basic///////////////////////////////////////

	///////////////////////////////AF////////////////////////////////////////
	//af
	virtual BOOL CheckAF();
	BOOL GetAFData(void);

	/////////////////////////////////PDAF////////////////////////////////////
	//MTK
	void PreProc1(void);
    BOOL PDAF_Proc1(void);
	BOOL MTK_GetProc1(void);
	void PDAFSaveProc1Bin(char* data, int size);
	void PDAFSaveBin(char* data, int size);

	//Qual
	void InitQualParam();
	void SaveQualParam();
	BOOL Qual_GainMapCal();
	int Qual_ReadSrcData(char* SrcRAWFilename, short *RAWDATA, int width, int height);
	int Qual_Display_GmCheckResult(int GainMapFlag);
	///////////////////////////////////Normal///////////////////////////////////////
	virtual void SaveCurrentRaw10(CString filename,USHORT *buff, int width, int height);
	virtual void SaveCurrentRaw8(CString filename,BYTE *buff, int width, int height);
	virtual void Raw10toRaw8(USHORT *Src, BYTE *Dest, long number);
	virtual CString GetModulePath();

	////////////////////////////////EEPROM////////////////////////////////
	virtual BOOL GT24C64_Read(USHORT address,unsigned char *Data,int size);
	virtual BOOL GT24C64_Programming(USHORT address,unsigned char *Data,int size);

	virtual BOOL ZC533_Page_Erase(BYTE page);
	virtual BOOL ZC533_All_Erase();
	virtual BOOL ZC533_Read(USHORT address,unsigned char *Data,int size);//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop /* = FALSE */
	virtual BOOL ZC533_Programming(USHORT address,unsigned char *Data,int size);//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page

	virtual BOOL DW9763_Page_Erase(BYTE page);
	virtual BOOL DW9763_All_Erase();
	virtual BOOL DW9763_Read(USHORT address,unsigned char *Data,int size);//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop /* = FALSE */
	virtual BOOL DW9763_Programming(USHORT address,unsigned char *Data,int size);//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page
	//////////////////////////////////3L8/////////////////////////////////////
	void S5K3L8_ReadOTPPage(int page, BYTE *Buff);
	void S5K3L8_WriteOTPPage(int page, BYTE *Buff);
	void Sensor3L8_StreamOn(void);

	BOOL S5K5E8ReadOTPpage(int Page, USHORT address, BYTE *Data, int length);
	BOOL S5K5E8WriteOTPpage(int Page, USHORT address, BYTE *Data, int length);
public:

	virtual void PreSubclassOtp(void) = 0;
	virtual void SetOtpSensorParams(void) = 0;


public:	// 
	CString m_otpName;
	CString SetOtpName(CString otpName);
	CString GetOtpName(void);

	int SumCalc(BYTE *data, int num, int mod,int add);
	void SetSensorOB(BYTE OB);	
	BYTE GetSensorOB(void) const;

	static OtpSensorStruct m_otpSensor;
	static OtpPowerStruct m_otpPower;
	static tagOtpCtrl m_otpCtrl;
	static OtpSensorStruct* GetOtpSensorParams(void);
	static OtpPowerStruct* GetOtpPowerParams(void);
	static tagOtpCtrl* GetControlParams(void);

	static OtpIDStruct m_IDGroup;
	OtpIDStruct m_IDCurrent;
	static OtpIDStruct* GetIDGroup(void);	
	OtpIDStruct* GetIDCurrent(void);		

	BYTE m_sensorOB;
	static tagAwbOtpCtrl m_wbCtrl;
	static WBGainRegStruct m_wbGainReg;
	static tagAwbRatio m_wbGolden;
// tagAwbRatio m_wbCurrentGolden;
// 	tagAwbRatio m_wbCorrect;
	tagAwbRatio m_wbCurrent;

	static tagAwbOtpCtrl* GetWBControlParams(void);
	static WBGainRegStruct* GetWBGainRegister(void);					 
	static tagAwbRatio* GetWBGoldenValue(void);	
	tagAwbRatio* GetWBCurrentGoldenValue(void);					 
	tagAwbRatio* GetWBCurrentValue(void);		

	static LencOtpControlStruct m_lencCtrl;
//	static LencRegStruct m_lencReg;
	BYTE *m_lencData;
	static LencOtpControlStruct* GetLencControlParams(void);
//	static LencRegStruct* GetLencRegister(void);	

	static ExpControlStruct m_expCtrl;
	static ExpRegisterStruct m_expRegister;
	static ExpRangeStruct m_expRange;
	ExpStatusStruct m_expStatus;
	static MTKPDAFStruct m_mtkpdaf;
	static ExpControlStruct* GetExpControlParams(void); 
	static ExpRegisterStruct* GetExpRegister(void);					 
	static ExpRangeStruct* GetExpRangeParams(void); 
	static MTKPDAFStruct* GetMTKPDAFParams(void);
	ShadingCalc* m_pShading;
	EepromStruct m_Eeprom;
	void SetEepromParams(EepromStruct *eeprom);

	BOOL eeprom_write(USHORT addr, USHORT reg, BYTE val);
	BOOL eeprom_read(USHORT addr, USHORT reg, BYTE *pval);	
	BOOL eeprom_multi_write(USHORT addr, USHORT reg, BYTE *pval, USHORT length);
	BOOL eeprom_multi_read(USHORT addr, USHORT reg, BYTE *pval, USHORT length);

	BOOL eeprom_write(USHORT reg, BYTE val);
	BOOL eeprom_read(USHORT reg, BYTE *pval);	
	BOOL eeprom_multi_write(USHORT reg, BYTE *pval, USHORT length);
	BOOL eeprom_multi_read(USHORT reg, BYTE *pval, USHORT length);

	BOOL i2c_write(USHORT reg, USHORT val);
	BOOL i2c_read(USHORT reg, USHORT *pval);
	BOOL i2c_write_byte(USHORT reg, BYTE val);
	BOOL i2c_read_byte(USHORT reg, BYTE *pval);
	BOOL i2c_multi_write(USHORT reg, BYTE *pval, USHORT length);
	BOOL i2c_multi_read(USHORT reg, BYTE *pval, USHORT length, BOOL noStop=FALSE);
	BOOL i2c_write(USHORT addr, USHORT reg, USHORT val, BYTE mode);
	BOOL i2c_read(USHORT addr, USHORT reg, USHORT *pval, BYTE mode);
	BOOL i2c_multi_write(USHORT addr, USHORT reg, BYTE regSize, BYTE* pval, USHORT length);
	BOOL i2c_multi_read(USHORT addr, USHORT reg, BYTE regSize, BYTE* pval, USHORT length, BOOL noStop=FALSE);

	CString info;

	UINT m_goldenrg;
	UINT m_goldenbg;
	UINT m_goldengg;

	UINT m_rgainh;
	UINT m_rgainl;
	UINT m_bgainh;
	UINT m_bgainl;
	UINT m_goldenr;
	UINT m_goldenb;
	UINT m_goldengr;
	UINT m_goldengb;
	float m_rgaindif;
	float m_bgaindif;
	float m_orgdif;

	UINT m_InfH;
	UINT m_InfL;
	UINT m_MacH;
	UINT m_MacL;
	UINT m_StcH;
	UINT m_StcL;
	UINT m_AFDif;
	USHORT m_infCode;
	USHORT m_macCode;
	USHORT m_stcCode;

	UINT m_sleeptime;

	UINT m_grouplimit;
	BOOL m_PDAFItem;
	BOOL m_EnQVL;
	BOOL m_EnLSC;
	BOOL m_LSCItem;
	BOOL m_EnAWB;
	BOOL m_EnAF;
	BOOL m_EnPDAF;
	BOOL m_berase;
	BOOL m_bcheckinf;
	BOOL m_bcheckmac;
	BOOL m_bchecksta;
	BOOL m_dummyburn;

	BYTE *m_proc1data;
	BYTE *m_mtklscdata;
	long m_mtklscsum;
	BYTE *m_quallscdata;
	int Qual_LSC_R_Max;
	int Qual_LSC_Gr_Max;
	int Qual_LSC_Gb_Max;
	int Qual_LSC_B_Max;
	int Qual_LSC_R_Min;
	int Qual_LSC_Gr_Min;
	int Qual_LSC_Gb_Min;
	int Qual_LSC_B_Min;
	int Qual_LSCDistanceSpec;
	long m_quallscsum;
	unsigned short* raw_seq;

	CString FileFolderPath;
	CString m_MTK_INI_Name;
	CString m_Qual_INI_Name;
	int m_shadingitem;

	BOOL SensorLSCSta;
	int m_sensoritem;
	int m_LscTarget;
	UINT m_LscGroup;

	int SaveExp;
	int SavePDAFExp;
    USHORT SaveGain;

	int m_cfaitem;
	int m_quallscitem;
	BOOL m_qualawb;
	BYTE Qual_wb_r;
	BYTE Qual_wb_gr;
	BYTE Qual_wb_gb;
	BYTE Qual_wb_b;

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
	BYTE Qual_GainMapData[SIZE_QUAL_PDAF_PROC1]; //GainMapWidth*Height*2*2+6*2

	int ZC533_Erase_lock;
	int ZC533_LastPage;
};