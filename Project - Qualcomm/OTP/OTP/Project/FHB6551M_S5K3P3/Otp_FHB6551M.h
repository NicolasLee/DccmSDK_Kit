#pragma once

#include <vector>
#include <Windows.h>

#include "OtpDefine_FHB6551M.h"

class COtpS5K3P3 : public COtpdefineFHB6551M
{
public:
	COtpS5K3P3(void);
	~COtpS5K3P3(void);


	enum NORMAL
	{
		SENSOROB = 64,//sensor ob
		AWB_FACTOR = 1024,//awb calc factor
		QUALLSCCOMPRESS = 0,//only for qual lsc data 0 1768bytes  1 1105 bytes
		OTPGAP = 32,//only for multi otp group
	};

	enum Reserverd
	{
		Reserverd_Data = 0,
		Reserve_Basic = 0,
		Reserve_AWB = 6,
		Reserve_LSC = 0,
		Reserve_AF =0,
		Reserve_PDAF1 = 8,
		Reserve_PDAF2 = 6,
	};
	enum S5K3P3_OTP_SIZE
	{
		SIZE_LSC = SIZE_QUAL_LSC,
		SIZE_PDAF_PROC1 = SIZE_QUAL_PDAF_PROC1,//SIZE_MTK_PDAF_PROC1,
		SIZE_PDAF_PROC2 = SIZE_QUAL_PDAF_PROC2,//SIZE_MTK_PDAF_PROC2,

		SIZE_BASIC_GROUP = Reserve_Basic+11, //除掉Flag的所有数据
		SIZE_WB_GROUP = Reserve_AWB+7, //除掉Flag的所有数据
		SIZE_LSC_GROUP = Reserve_LSC+1769, //除掉Flag的所有数据
		SIZE_AF_GROUP = Reserve_AF+5, //除掉Flag的所有数据
		SIZE_PDAF_GROUP1 = Reserve_PDAF1+1079, //除掉Flag的所有数据
		SIZE_PDAF_GROUP2 = Reserve_PDAF2+3,//除掉Flag的所有数据
		SIZE_PDAF_GROUP = SIZE_PDAF_GROUP1+SIZE_PDAF_GROUP2+1,//1为PDAF2 Flag

		SIZE_TOTAL_GROUP = 3012, //第一个Data到最后一个Data
	};

	enum S5K3P3_REG_ADDR
	{
		GAIN_ADDR_GR = 0x020e,
		GAIN_ADDR_R  = 0x0210,
		GAIN_ADDR_B  = 0x0212,
		GAIN_ADDR_GB = 0x0214,
		GAIN_DEFAULT_VAL = 0x0100,

// 	S5K3P3_ADDR_GAIN_GR_H = 0x0508,
// 	S5K3P3_ADDR_GAIN_GR_L = 0x0509,	
// 	S5K3P3_ADDR_GAIN_R_H  = 0x050C,
// 	S5K3P3_ADDR_GAIN_R_L  = 0x050D,
// 	S5K3P3_ADDR_GAIN_B_H  = 0x050E,
// 	S5K3P3_ADDR_GAIN_B_L  = 0x050F,
// 	S5K3P3_ADDR_GAIN_GB_H = 0x050A,
// 	S5K3P3_ADDR_GAIN_GB_L = 0x050B,
	};

	enum S5K3P3_OTP_FLAG
	{
		FLAG_EMPTY = 0x00,
		FLAG_VALID = 0x01,
		//	FLAG_INVAID = 0x1F,

		// 	S5K3P3_FLAG_GROUP1 = 0x01,
		// 	S5K3P3_FLAG_GROUP2 = 0x13,
		// 	S5K3P3_FLAG_GROUP3 = 0x37,
	};

	enum S5K3P3_OTP_REG_ADDR
	{
		REG_BASIC_FLAG = 0x0000,
		REG_BASIC_CHECKSUM = 0x000B,

		REG_WB_FLAG = 0x0010,
		REG_WB_CHECKSUM = 0x001D,

		REG_LSC_FLAG = 0x0020,
		REG_LSC_CHECKSUM = 0x0709,

		REG_AF_FLAG = 0x0740,
		REG_AF_CHECKSUM = 0x0745,

		REG_PDAF_PROC1_FLAG=0x0780,
		REG_PDAF_PROC1_FIRSTADDR = 0x0781,
		REG_PDAF_PROC1_SUMADDR = 0x0BB7,

		REG_PDAF_PROC2_FLAG=0x0BC0,
		REG_PDAF_PROC2_FIRSTADDR = 0x0BC1,
		REG_PDAF_PROC2_SUMADDR= 0x0BC3,

		REG_PDAF_FLAG = REG_INVALID,
		REG_PDAF_SUMADDR = REG_INVALID,
	};

	enum S5K3P3_OTP_Basic_Info
	{
		BASIC_MID = 0x42,
		BASIC_LENSID = 0x25,
		BASIC_VCMID = 0x25,
		BASIC_DRIVERICID = 0x17,
		BASIC_LSCVER = 0x10,
		BASIC_PDAFVER = 0x0A,
		BASIC_COLORTEMP = 0x32,

		BASIC_IRID = 0x01,
		BASIC_AFFLAG= 0x01,
		BASIC_LIGHTFLAG= 0x02,
		BASIC_CALIVER1 = 0x01,
		BASIC_CALIVER2 = 0,
		BASIC_CALIVER3 = 0,
		BASIC_CALIVER4 = 0,
		BASIC_SERIALNO1 = 0,
		BASIC_SERIALNO2 = 0,
		BASIC_SERIALNO3  = 0,
	};

	typedef struct _S5K3P3OTPDataSpec//包含数据和Checksum  最后一个BYTE存储Checksum
	{
		BYTE BaseInfo[SIZE_BASIC_GROUP];
		BYTE LSCInfo[SIZE_LSC_GROUP];
		BYTE AWBInfo[SIZE_WB_GROUP];
		BYTE AFInfo[SIZE_AF_GROUP];
		BYTE PDAF1[SIZE_PDAF_PROC1];
	}S5K3P3OTPDataSpec;

	typedef union S5K3P3EEPROMOtpGroupUnion
	{
		BYTE data[SIZE_TOTAL_GROUP];
		struct
		{
			union
			{
				BYTE data_Basic[SIZE_BASIC_GROUP+1];
				struct
				{
					BYTE basic_flag;

					BYTE basic_mid;
					BYTE basic_year;
					BYTE basic_month;
					BYTE basic_day;
					BYTE basic_lens;
					BYTE basic_vcm;
					BYTE basic_driverIC;
					BYTE basic_lencDll;
					BYTE basic_pdafDll;
					BYTE basic_colorTemp;

					BYTE basic_checkSum;
					//////////////////////////////////////////////////////////////////////////
// 				BYTE basic_IR;
// 				BYTE basic_afType;		
// 				BYTE basic_lightSource;
// 				BYTE basic_calibrationVersion[4];
// 				BYTE basic_serialNum[3];
// 				BYTE basic_pdDll;
// 				BYTE basic_vendor;
// 				BYTE basic_house;
// 				BYTE basic_sensor;
// 				BYTE basic_FNO;
// 				BYTE basic_checkSum_MCU;
// 				BYTE basic_reservedBasic[3];
				};
			};
			union
			{
				BYTE data_WB[SIZE_WB_GROUP+1];
				struct
				{
					BYTE wb_flag;

					BYTE  wb_currentRG_H;
					BYTE  wb_currentRG_L;
					BYTE  wb_currentBG_H;
					BYTE  wb_currentBG_L;
					BYTE  wb_currentGbGr_H;
					BYTE  wb_currentGbGr_L;

					BYTE reservedWB[Reserve_AWB];
					BYTE wb_checkSum;
					//////////////////////////////////////////////////////////////////////////
// 				BYTE  wb_goldenRG_L;
// 				BYTE  wb_goldenRG_H;
// 				BYTE  wb_goldenBG_L;
// 				BYTE  wb_goldenBG_H; 
// 				BYTE  wb_goldenGbGr_L;
// 				BYTE  wb_goldenGbGr_H;
// 
// 				BYTE  wb_currentR_L;
// 				BYTE  wb_currentR_H;
// 				BYTE  wb_currentB_L;
// 				BYTE  wb_currentB_H;
// 				BYTE  wb_currentGb_L;
// 				BYTE  wb_currentGb_H;
// 				BYTE  wb_currentGr_L;
// 				BYTE  wb_currentGr_H;
// 
// 				BYTE  wb_goldenR_L;
// 				BYTE  wb_goldenR_H;
// 				BYTE  wb_goldenB_L;
// 				BYTE  wb_goldenB_H;
// 				BYTE  wb_goldenGb_L;
// 				BYTE  wb_goldenGb_H;
// 				BYTE  wb_goldenGr_L;
// 				BYTE  wb_goldenGr_H;

				};
			};
			union
			{
				BYTE data_Lenc[SIZE_LSC_GROUP+1];
				struct
				{
					BYTE lsc_flag;
					BYTE lsc_data[SIZE_LSC];
					BYTE lsc_checksum;
					//////////////////////////////////////////////////////////////////////////
//      		BYTE reservedLenc[50];
				};
			};
			union
			{
				BYTE data_AF[SIZE_AF_GROUP+1];
				struct
				{
					BYTE af_flag;
					BYTE af_mac_h;
					BYTE af_mac_l;
					BYTE af_inf_h;
					BYTE af_inf_l;
					BYTE af_checksum;
					//////////////////////////////////////////////////////////////////////////
// 				BYTE af_lens_spin;
// 				BYTE af_cali_direction;
				};
			};
			union
			{
				BYTE data_PDAF[SIZE_PDAF_GROUP+1];
				struct
				{
					union
					{
						BYTE data_PDAF1[SIZE_PDAF_GROUP1+1];
						struct
						{
							BYTE pdaf1_flag;
							BYTE pdaf_proc1[SIZE_PDAF_PROC1];
							BYTE pdaf_proc1checksum;
							BYTE reservedpdaf1[Reserve_PDAF1];
						};
					};
					union
					{
						BYTE data_PDAF2[SIZE_PDAF_GROUP2+1];
						struct
						{
							BYTE pdaf2_flag;
							BYTE pdaf_proc2[SIZE_PDAF_PROC2];
							BYTE pdaf_pdaf2checksum;
							//////////////////////////////////////////////////////////////////////////
							//            	BYTE reservedpdaf2[Reserve_PDAF2];
						};
					};
				};
			};
		};
	}OtpGroup;

	////////////////////////////////Init Otp//////////////////////////////////////
	BOOL EraseAllData(void);
	void   SetWBDefaultGain(void);
	void   S5K3P3_SetWBGain(USHORT gainGr, USHORT gainR, USHORT gainB, USHORT gainGb);

	/////////////////////////////////LSC///////////////////////////////////////

	////////////////////////////////AWB////////////////////////////////////
	BOOL GetAWB(void);
	BOOL CheckOTPAWB(void);
    BOOL GetAWBInfo(tagAwbRatio* wb);
	BOOL S5K3P3_GetAWBData(S5K3P3OTPDataSpec &awbdata);
	BOOL S5K3P3_GetCurrentAWB(tagAwbRatio* wb);
	BOOL S5K3P3_CheckAWB(tagAwbRatio wb,int type);

	/////////////////////////////Burning/////////////////////////////////////////
	void   CreateBurningData(void);
	void   S5K3P3_GetBasicData(void);
	void   S5K3P3_GetAFData(void);
	void   S5K3P3_CalcAllCheckSum(void);
	BOOL S5K3P3_OtpWrite(void);
	BOOL CheckSensorOTPEmpty(void);
	BOOL WriteOTP(void);
	BOOL WriteOTP(vector<double>& vData);
	BOOL ProgrammingBasic(void);
	void Programming_3P3Basic(void);
	BOOL ProgrammingAWB(void);
	void Programming_3P3AWB(void);
	BOOL ProgrammingLSC(void);
	BOOL Programming_3P3LSC(void);
	void S5K3P3_WriteLSCFlagSum(void);
	BOOL ProgrammingAF(void);
	void Programming_3P3AF(void);
	BOOL ProgrammingPDAF(void);

	void SaveOTPData();	
	void SaveOTPData(vector<double>& vData);
	/////////////////////////////////Verify/////////////////////////////////////
	BOOL CheckOTP(void);
	BOOL S5K3P3_OTPVerify(void);
	BOOL S5K3P3_CheckOTPReadWrite(void);
	BOOL Sensor3P3_CheckReadWrite(void);
	BOOL Sensor3P3_CheckLSCReadWrite(void);
	BOOL S5K3P3_VerifyLSC();
	BOOL S5K3P3_VerifyAWB();
	BOOL VerifyLSC(void);
	BOOL VerifyPreWB(void);
	BOOL VerifyAWB(void);
	BOOL GetOTPRGBG(void);

	//////////////////////////////////////////////////////////////////////////
//	BOOL InitProgram(void);
	BOOL StartProgram(void);
//	BOOL EndProgram(void);



protected:
	void PreSubclassOtp(void);
	void SetOtpSensorParams(void);
private:
    OtpGroup Data_Write,Data_Read;
	tagAwbRatio m_wbCurrent;
	S5K3P3OTPDataSpec  OTP_Data_Write,OTP_Data_Read;

public:
	CString info;

	typedef struct tagAddressFlag
	{// Flag的Address
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;
		WORD pdaf1;
		WORD pdaf2;
		WORD pdaf;
	}AddressFlag;

	typedef struct tagAddressSum
	{// Sum的Address
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;
		WORD pdaf1;
		WORD pdaf2;
		WORD pdaf;
	}AddressSum;

	typedef struct tagOtpSize
	{// 每个块占用的空间
		int moduleInfo;
		int awb;
		int lsc;
		int af;
		int pdaf;
	}OtpSize;

	typedef struct tagOtpPage
	{// 有Page类型的空间所在Page
		WORD moduleInfo;
		WORD awb;
		WORD lsc;
		WORD af;
		WORD pdaf;
		WORD pdaf_coef_k;

	}OtpPage;

	typedef struct tagAddressCode
	{// AF Code的Address
		WORD stcL;
		WORD stcH;
		WORD infL;
		WORD infH;
		WORD macL;
		WORD macH;

	}AddressCode;

	AddressFlag m_addrFlag;
	AddressSum	m_addrSum;
	OtpSize		m_otpSize;
	OtpPage		m_otpPage;
	AddressCode	m_addrCode;

	// slave addr
	WORD	m_slaveSensor;
	WORD	m_slaveEeprom;
	BYTE	m_modeSensor;//iic mode
	BYTE	m_modeEeprom;//iic mode
	BYTE	m_regSize;	// used for multiple read

	// valid flag val
	WORD m_flagEmpty;
	WORD m_flagValid;
// 	WORD m_flagGroup1;
// 	WORD m_flagGroup2;
// 	WORD m_flagGroup3;
	
	int proc1sum;

	BYTE S5K3P3_LSCWriteData[512];
	BYTE S5K3P3_LSCReadData[512];
	int S5K3P3_LSCchecksum;
};