#pragma once
#include "OtpCheckDef.h"
#include "DccmBase.h"

enum eProjectName	//��Ŀ���� PN_ ����Ŀ�ڴ�����
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

class OtpCheckBase //OtpCheckBase����ļ����Ǳ�Ҫ�����������Ķ�����Ҫ�Ķ������ھ�����������స�������޸�
{
public:
	OtpCheckBase(void);
	~OtpCheckBase(void);

	//DLL�򻯽ӿ� ����0�ɹ�  ����ֵʧ��
	//Main�ӿ�
	int PauseGrabFrame();
	int ResumeGrabFrame();
	//ImageSensor�ӿ�
	int Read_OTP_Multi_BYTE(USHORT addr, BYTE* data, int Length, int PageNo = 0);
	int Read_OTP_BYTE(USHORT addr, BYTE* data, int PageNo = 0);
	int Write_OTP_Multi_BYTE(USHORT addr, BYTE* data, int Length, int PageNo = 0);
	int Write_OTP_BYTE(USHORT addr, BYTE data, int PageNo = 0);
	char*  GetFuseID();//����Sensor��Slave ID
	int InitOTP();
	int ReleaseOTP();

	//Eeprom�ӿ�
	int Eeprom_All_Erase();
	int Eeprom_Page_Erase(BYTE page);
	int Eeprom_Programing(USHORT address,unsigned char *Data,int size);
	int Eeprom_Programing_BYTE(USHORT address, unsigned char Data);
	int Eeprom_Read(USHORT address,unsigned char *Data,int size);
	int Eeprom_Read_BYTE(USHORT address,unsigned char *Data);

	//Sensor��Eepromͳһ��ͬһ�������ӿ�
	int Programing(USHORT address,unsigned char *Data,int size, int PageNo = 0);
	int Programing_BYTE(USHORT address, unsigned char Data, int PageNo = 0);
	int Read(USHORT address,unsigned char *Data,int size, int PageNo = 0);
	int Read_BYTE(USHORT address,unsigned char *Data, int PageNo = 0);
	//////////////////////////////////////////////////////////////////////////
	virtual int InitProject();//����Sensor����Hi1332��OTP����ǰ���ʼ��
	virtual int EndProject();//����Sensor����Hi1332��OTP���������ͷ�

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
	virtual	int CheckSum(BOOL flag,int flagaddr, int sumaddr, int datasize,  int gap,  CString str, int validgroup);//��һ��checksum�����޷���������checksum�������������������checksum����
	virtual  int CheckSum_Word(BOOL flag,int flagaddr, int sumaddr, int datasize,  int gap,  CString str, int validgroup); 

	// Get AF Code ����ǰOtpCheckBase�ڻ�ȡAF Code��ʽ�޷����������������ʹ���麯������Ӧ�����������µ�Getxxxcode����
	virtual  int GetStcAFCode();
	virtual  int GetInfAFCode();
	virtual  int GetMacAFCode();
	virtual  int GetMidAFCode();

	//Check AWB ����ǰOtpCheckBase�ڼ��AWB��ʽ�޷����������������ʹ���麯������Ӧ�����������µ�GetValue����
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

	/////////////////////////////////�Ի��򴫽����ı���/////////////////////////////////////////
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
	BOOL m_bCheckStc;	// ��ʼCode
	BOOL m_bCheckInf;	// Զ��Code
	BOOL m_bCheckMac;	// ����Code
	BOOL m_bCheckMid; //�о�Code

	UINT m_bAFCompareLimit;
	UINT m_stcLower;	// ��ʼCode�±߽�
	UINT m_stcUpper;	// ��ʼCode�ϱ߽�
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

