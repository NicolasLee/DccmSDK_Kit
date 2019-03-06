#pragma once
#include "OtpCheckDef.h"
#include "DccmBase.h"

enum eProjectName	//项目名称 PN_ 新项目在此增加
{
	PN_NULL = -1,
	PN_BYD_TKU8615M,
	PN_BYD_TRJ8564M,
	PN_HLT_HLT7038,
	PN_HLT_HLT7178,
	PN_HLT_HLT7215,
	PN_BYD_TYD8592M,
	PN_BYD_TYD8583M,
	PN_HLT_HLT7014,
	PN_BYD_FYD8853M,
	PN_BYD_CMK8440M,
	PN_SAA30L2,
};

class OtpCheckBase //OtpCheckBase里的文件除非必要，尽量不做改动。需要改动可以在具体的派生子类案子里做修改
{
public:
	OtpCheckBase(void);
	~OtpCheckBase(void);

	//DLL简化接口 返回0成功  其余值失败
	//Main接口
	int PauseGrabFrame();
	int ResumeGrabFrame();
	//ImageSensor接口
	int Read_OTP_Multi_BYTE(USHORT addr, BYTE* data, int Length, int PageNo = 0);
	int Read_OTP_BYTE(USHORT addr, BYTE* data, int PageNo = 0);
	int Write_OTP_Multi_BYTE(USHORT addr, BYTE* data, int Length, int PageNo = 0);
	int Write_OTP_BYTE(USHORT addr, BYTE data, int PageNo = 0);
	char*  GetFuseID();//返回Sensor的Slave ID
	int InitOTP();
	int ReleaseOTP();

	//Eeprom接口
	int Eeprom_All_Erase();
	int Eeprom_Page_Erase(BYTE page);
	int Eeprom_Programing(USHORT address,unsigned char *Data,int size);
	int Eeprom_Programing_BYTE(USHORT address, unsigned char Data);
	int Eeprom_Read(USHORT address,unsigned char *Data,int size);
	int Eeprom_Read_BYTE(USHORT address,unsigned char *Data);

	//Sensor与Eeprom统一用同一个函数接口
	int Programing(USHORT address,unsigned char *Data,int size, int PageNo = 0);
	int Programing_BYTE(USHORT address, unsigned char Data, int PageNo = 0);
	int Read(USHORT address,unsigned char *Data,int size, int PageNo = 0);
	int Read_BYTE(USHORT address,unsigned char *Data, int PageNo = 0);
	//////////////////////////////////////////////////////////////////////////
	virtual int InitProject();//少数Sensor比如Hi1332在OTP操作前需初始化
	virtual int EndProject();//少数Sensor比如Hi1332在OTP操作后需释放

	int Check_ModuleInfo();
	int Check_AWB();
	int Check_AWBValue();
	virtual int Check_LSC();
	virtual int Check_AF();
	int CheckCode_Stc();		
	int CheckCode_Inf();
	int CheckCode_Mac();
	int CheckCode_Mid(); 
	int CompareAFCode();
	virtual int Check_PDAF();
	virtual void SaveOTPData();	
	BYTE SumCalc(BYTE* data, int num,int mod,int add);
	WORD SumCalc_WORD(BYTE* data, int num,int mod,int add);

	virtual BOOL i2c_write(USHORT reg, USHORT val);
	virtual BOOL i2c_read(USHORT reg, USHORT *pval);
	virtual BOOL i2c_write_byte(USHORT reg, BYTE val);
	virtual BOOL i2c_read_byte(USHORT reg, BYTE *pval);

private:

	int CheckFlag(BOOL flag,int addr, CString str, int *validgroup);		// Check Flag
	virtual  int CheckGroup(BYTE flag);
	virtual	int CheckSum(BOOL flag,int flagaddr, int sumaddr, int datasize,  int gap,  CString str, int validgroup);//若一个checksum函数无法满足所有checksum情况，请添加其他情况的checksum函数
	virtual  int CheckSum_Word(BOOL flag,int flagaddr, int sumaddr, int datasize,  int gap,  CString str, int validgroup); 

	// Get AF Code 若当前OtpCheckBase内获取AF Code方式无法满足所有情况，请使用虚函数在相应子类内重载新的Getxxxcode函数
	virtual  int GetStcAFCode();
	virtual  int GetInfAFCode();
	virtual  int GetMacAFCode();
	virtual  int GetMidAFCode();

	//Check AWB 若当前OtpCheckBase内检查AWB方式无法满足所有情况，请使用虚函数在相应子类内重载新的GetValue函数
	virtual int CheckValue(); 
	virtual int GetValue(); 

public:
	DccmControl_Interface* m_pInterface;

	Flag      m_flag;
	Group m_group;
	AddressFlag m_addrFlag;
	AddressSum	m_addrSum;
	CalcCheckSum m_calcCheckSum; 
	OtpSize		m_otpSize;
	GapSize       m_gapsize;
	OtpPage		m_otpPage;
	AFAddress	m_afAddrCode;
	AFValue       m_afValue;
	AddressAWB m_addrAWB;
	ValueAWB m_valueAWB;

	CString m_szInfo;

	/////////////////////////////////对话框传进来的变量/////////////////////////////////////////
	BOOL m_OtpType;

	BOOL m_bCheckFlag_ModuleInfo;
	BOOL m_bCheckFlag_AWB;
	BOOL m_bCheckFlag_LSC;
	BOOL m_bCheckFlag_AF;
	BOOL m_bCheckFlag_PDAF;

	BOOL m_bCheckSum_ModuleInfo;
	BOOL m_bCheckSum_AWB;
	BOOL m_bCheckSum_LSC;
	BOOL m_bCheckSum_AF;
	BOOL m_bCheckSum_PDAF;

	// AF Code
	BOOL m_bCompareAF;
	BOOL m_bCheckStc;	// 起始Code
	BOOL m_bCheckInf;	// 远景Code
	BOOL m_bCheckMac;	// 近景Code
	BOOL m_bCheckMid; //中景Code

	UINT m_bAFCompareLimit;
	UINT m_stcLower;	// 起始Code下边界
	UINT m_stcUpper;	// 起始Code上边界
	UINT m_infLower;	
	UINT m_infUpper;	
	UINT m_macUpper;	
	UINT m_macLower;	
	UINT m_midUpper;	
	UINT m_midLower;	

	BOOL m_bCheckRG;
	BOOL m_bCheckBG;
	BOOL m_bCheckGG;
	BOOL m_bCheckR;
	BOOL m_bCheckGr;
	BOOL m_bCheckGb;
	BOOL m_bCheckB;

	UINT m_RGLower;
	UINT m_RGUpper;
	UINT m_BGLower;
	UINT m_BGUpper;
	UINT m_GGLower;
	UINT m_GGUpper;
	UINT m_RLower;
	UINT m_RUpper;
	UINT m_GrLower;
	UINT m_GrUpper;
	UINT m_GbLower;
	UINT m_GbUpper;
	UINT m_BLower;
	UINT m_BUpper;

	BOOL m_bCheckGoldRG;
	BOOL m_bCheckGoldBG;
	BOOL m_bCheckGoldGG;
	BOOL m_bCheckGoldR;
	BOOL m_bCheckGoldGr;
	BOOL m_bCheckGoldGb;
	BOOL m_bCheckGoldB;

	UINT m_GoldRG;
	UINT m_GoldBG;
	UINT m_GoldGG;
	UINT m_GoldR;
	UINT m_GoldGr;
	UINT m_GoldGb;
	UINT m_GoldB;
};	

