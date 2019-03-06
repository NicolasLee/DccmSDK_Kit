#pragma  once
#include "..\..\OtpBase.h"

class COtpdefineHLT7109 :  public COtpBase
{
public:
	enum normal
	{
#ifndef Raw8Data
		SENSOROB = 64,
#else
		SENSOROB = 16,
#endif 
		AWB_FACTOR = 1024,

		OTPGAP = 32,//only for multi otp group
		QUALLSCCOMPRESS = 1,//0 1768bytes  1 1105 bytes  only for qual lsc
	};
	enum HLT7109_OTP_SIZE
	{
		SIZE_BASIC_GROUP = 20, //除掉Flag的所有数据,用于计算checksum
		SIZE_WB_GROUP = 20, //除掉Flag的所有数据
		SIZE_LSC = SIZE_MTK_LSC,
		SIZE_LSC_GROUP = 1872, //除掉Flag的所有数据
		SIZE_AF_GROUP = 12, //除掉Flag的所有数据
		//MTK
		SIZE_PDAF_PROC1 = SIZE_MTK_PDAF_PROC1,
		SIZE_PDAF_PROC2 = SIZE_MTK_PDAF_PROC2,
		SIZE_PDAF_GROUP =SIZE_MTK_PDAF_PROC1+SIZE_MTK_PDAF_PROC2+9, //除掉Flag的所有数据

		SIZE_TOTAL_GROUP = 3310, //第一个Data到最后一个Data
	};

	enum HLT7109_REG_ADDR
	{
		GAIN_ADDR_GR = 0x020e,
		GAIN_ADDR_R  = 0x0210,
		GAIN_ADDR_B  = 0x0212,
		GAIN_ADDR_GB = 0x0214,
		GAIN_DEFAULT_VAL = 0x0100,

// 	HLT7109_ADDR_GAIN_GR_H = 0x0508,
// 	HLT7109_ADDR_GAIN_GR_L = 0x0509,	
// 	HLT7109_ADDR_GAIN_R_H  = 0x050C,
// 	HLT7109_ADDR_GAIN_R_L  = 0x050D,
// 	HLT7109_ADDR_GAIN_B_H  = 0x050E,
// 	HLT7109_ADDR_GAIN_B_L  = 0x050F,
// 	HLT7109_ADDR_GAIN_GB_H = 0x050A,
// 	HLT7109_ADDR_GAIN_GB_L = 0x050B,
	};

	enum HLT7109_OTP_FLAG
	{
		FLAG_EMPTY = 0x00,
		FLAG_VALID = 0x01,

// 	HLT7109_FLAG_GROUP1 = 0x01,
// 	HLT7109_FLAG_GROUP2 = 0x13,
// 	HLT7109_FLAG_GROUP3 = 0x37,
	};

	enum HLT7109_OTP_REG_ADDR
	{
		REG_BASIC_FLAG = 0x0000,
		REG_BASIC_CHECKSUM = 0x0013,
		REG_WB_FLAG = 0x0773,
		REG_WB_CHECKSUM = 0x0786,
		REG_LSC_FLAG = 0x0022,
		REG_LSC_CHECKSUM = 0x0771,
		REG_AF_FLAG = 0x0015,
		REG_AF_CHECKSUM = 0x0020,

		REG_PDAF_FLAG = 0x0788,
		REG_PDAF_PROC1_FIRSTADDR = 0x0789,
		REG_PDAF_PROC2_FIRSTADDR = 0x097e,
		REG_PDAF_PROC1_SUMADDR = 0x097b,
		REG_PDAF_PROC2_SUMADDR= 0x0cec,

		PAGE_GROUP1_START = 1,
		PAGE_GROUP2_START = 1,

		REG_PDAF_SUMADDR = REG_INVALID,
	};

	enum HLT7109_OTP_Basic_Info
	{

		BASIC_MID = 0x0A,
		BASIC_APID = 0x01,
		BASIC_LENSID = 0x2e,
		BASIC_VCMID = 0x20,
		BASIC_DRIVERICID = 0x0f,
		BASIC_IRID = 0x02,
		BASIC_BLACKMASK = 0x03,

		BASIC_EEPROMID = 0x29,
		BASIC_COLORTEMP = 0x01,
		BASIC_BITEN = 0x0F,

		BASIC_MTKVER1= 0xFF,
		BASIC_MTKVER2= 0x00,
		BASIC_MTKVER3= 0x0B,
		BASIC_MTKVER4= 0x01,

		BASIC_AFFLAG= 0x01,
		BASIC_LIGHTFLAG= 0x02,
		BASIC_LSCVER = 0x21,
		BASIC_CALIVER1 = 0x01,
		BASIC_CALIVER2 = 0,
		BASIC_CALIVER3 = 0,
		BASIC_CALIVER4 = 0,
		BASIC_SERIALNO1 = 0,
		BASIC_SERIALNO2 = 0,
		BASIC_SERIALNO3  = 0,
		BASIC_PDVER = 0x26,
	};

	typedef struct _HLT7109OTPDataSpec//包含数据和Checksum
	{
		BYTE BaseInfo[SIZE_BASIC_GROUP];
		BYTE LSCInfo[SIZE_LSC_GROUP];
		BYTE AWBInfo[SIZE_WB_GROUP];
		BYTE AFInfo[SIZE_AF_GROUP];
		BYTE PDAF1[SIZE_PDAF_PROC1];
	}HLT7109OTPDataSpec;

	typedef union HLT7109EEPROMOtpGroupUnion
	{
		BYTE data[SIZE_TOTAL_GROUP];
		struct
		{
			union
			{
				BYTE data_Basic[SIZE_BASIC_GROUP+1];//+1为Flag
				struct
				{
					BYTE basic_flag;

					BYTE basic_mid;
					BYTE basic_apid;
					BYTE basic_lens;
					BYTE basic_vcm;
					BYTE basic_driverIC;
					BYTE basic_IR;
					BYTE basic_blackmask;
					BYTE basic_resrverd;
					BYTE basic_year;
					BYTE basic_month;
					BYTE basic_day;
					BYTE basic_resrverd2;
					BYTE basic_awbver;
					BYTE basic_lscver;
					BYTE basic_pdafver;
					BYTE basic_pcbver;
					BYTE basic_resrverd3;
					BYTE basic_resrverd4;

					BYTE basic_checkSum[2];
					//////////////////////////////////////////////////////////////////////////
// 					BYTE basic_eepromid;
// 					BYTE basic_temperature;
// 					BYTE basic_biten;
// 					BYTE basic_reservedBasic[6];				

// 				BYTE basic_afType;		
// 				BYTE basic_lightSource;
// 				BYTE basic_lencDll;
// 				BYTE basic_calibrationVersion[4];
// 				BYTE basic_serialNum[3];
// 				BYTE basic_pdDll;
// 				BYTE basic_vendor;
// 				BYTE basic_house;
// 				BYTE basic_sensor;
// 				BYTE basic_FNO;
// 				BYTE basic_checkSum_MCU;

// 					BYTE basic_mtkver1;
// 					BYTE basic_mtkver2;
// 					BYTE basic_mtkver3;
// 					BYTE basic_mtkver4;
				};
			};
			union
			{
				BYTE data_WB[SIZE_WB_GROUP+1];
				struct
				{
					BYTE wb_flag;

					BYTE  wb_currentR_H;
					BYTE  wb_currentR_L;
					BYTE  wb_currentGr_H;
					BYTE  wb_currentGr_L;
					BYTE  wb_currentGb_H;
					BYTE  wb_currentGb_L;
					BYTE  wb_currentB_H;
					BYTE  wb_currentB_L;

					BYTE  wb_goldenR_H;
					BYTE  wb_goldenR_L;
					BYTE  wb_goldenGr_H;
					BYTE  wb_goldenGr_L;
					BYTE  wb_goldenGb_H;
					BYTE  wb_goldenGb_L;
					BYTE  wb_goldenB_H;
					BYTE  wb_goldenB_L;

					BYTE reservedWB[2];

					BYTE wb_checkSum[2];
					//////////////////////////////////////////////////////////////////////////
// 					BYTE  wb_currentRG_L;
// 					BYTE  wb_currentRG_H;
// 					BYTE  wb_currentBG_L;	
// 					BYTE  wb_currentBG_H;
// 					BYTE  wb_currentGbGr_L;
// 					BYTE  wb_currentGbGr_H;
// 
// 					BYTE  wb_goldenRG_L;
// 					BYTE  wb_goldenRG_H;
// 					BYTE  wb_goldenBG_L;
// 					BYTE  wb_goldenBG_H; 
// 					BYTE  wb_goldenGbGr_L;
// 					BYTE  wb_goldenGbGr_H;

// 					BYTE  wb_currentR_L;
// 					BYTE  wb_currentB_L;
// 					BYTE  wb_currentGb_L;
// 					BYTE  wb_currentGr_L;
// 
// 					BYTE  wb_goldenR_L;
// 					BYTE  wb_goldenB_L;
// 					BYTE  wb_goldenGb_L;
// 					BYTE  wb_goldenGr_L;
				};
			};
			union
			{
				BYTE data_Lenc[SIZE_LSC_GROUP+1];
				struct
				{
					BYTE lsc_flag;
					BYTE lsc_data[SIZE_LSC];
					BYTE lsc_rsvd[2];
					BYTE lsc_checksum[2];
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

					BYTE af_mac6_h;
					BYTE af_mac6_l;
					BYTE af_mac10_h;
					BYTE af_mac10_l;
					BYTE af_mid40_h;
					BYTE af_mid40_l;
					BYTE af_inf_h;
					BYTE af_inf_l;

					BYTE af_rsv[2];

					BYTE af_checksum[2];
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
					BYTE pdaf1_flag;
					BYTE pdaf_proc1[SIZE_PDAF_PROC1];
					BYTE pdaf_rsv1[2];
					BYTE pdaf_proc1checksum[2];

					BYTE pdaf2_flag;
					BYTE pdaf_proc2[SIZE_PDAF_PROC2];
					BYTE pdaf_rsv2[2];
					BYTE pdaf_pdaf2checksum[2];
				};
			};
		};
	}OtpGroup;
};
