#pragma  once
#include "..\..\OtpBase.h"

class COtpdefineTYD8583M :  public COtpBase
{
public:
	enum normal
	{
#ifndef Raw8Data
		SENSOROB = 64,
#else
		SENSOROB = 16,
#endif 
		AWB_FACTOR = 512,

		OTPGAP_BASIC = 17,//only for multi otp group
		OTPGAP_AWB = 30,//only for multi otp group
		OTPGAP_LSC = 867,//only for multi otp group
		OTPGAP_AF = 8,//only for multi otp group

		QUALLSCCOMPRESS = 1,//0 1768bytes  1 1105 bytes  only for qual lsc
	};
	enum TYD8583M_OTP_SIZE
	{
		SIZE_BASIC_GROUP = 17, //除掉Flag的所有数据,用于计算checksum
		SIZE_WB_GROUP = 30, //除掉Flag的所有数据
		SIZE_LSC = 858,
		SIZE_LSC_GROUP = 867, //除掉Flag的所有数据
		SIZE_AF_GROUP = 8, //除掉Flag的所有数据
		//MTK
		SIZE_PDAF_PROC1 = SIZE_MTK_PDAF_PROC1,
		SIZE_PDAF_PROC2 = SIZE_MTK_PDAF_PROC2,
		SIZE_PDAF_GROUP =SIZE_MTK_PDAF_PROC1+SIZE_MTK_PDAF_PROC2+2, //除掉Flag的所有数据

		SIZE_TOTAL_GROUP = 2840, //第一个Data到最后一个Data
	};

	enum TYD8583M_REG_ADDR
	{
// 		GAIN_ADDR_GR = 0x020e,
// 		GAIN_ADDR_R  = 0x0210,
// 		GAIN_ADDR_B  = 0x0212,
// 		GAIN_ADDR_GB = 0x0214,

	ADDR_GAIN_GR_H = 0x0508,
	ADDR_GAIN_GR_L = 0x0509,	
	ADDR_GAIN_R_H  = 0x050C,
	ADDR_GAIN_R_L  = 0x050D,
	ADDR_GAIN_B_H  = 0x050E,
	ADDR_GAIN_B_L  = 0x050F,
	ADDR_GAIN_GB_H = 0x050A,
	ADDR_GAIN_GB_L = 0x050B,

	GAIN_DEFAULT_VAL = 0x0200,
	};

	enum TYD8583M_OTP_FLAG
	{
		FLAG_EMPTY = 0x00,
		FLAG_VALID = 0x80,

		FLAG_GROUP1 = 0x01,
		FLAG_GROUP2 = 0x13,
		FLAG_GROUP3 = 0x37,
	};

	enum TYD8583M_OTP_REG_ADDR
	{
		REG_BASIC_FLAG = 0x0501,
		REG_BASIC_CHECKSUM = 0x0512,
		REG_WB_FLAG = 0x0F5F,
		REG_WB_CHECKSUM = 0X0F7D,
		REG_LSC_FLAG = 0x0535,
		REG_LSC_CHECKSUM = 0x0898,
		REG_AF_FLAG = 0x1000,
		REG_AF_CHECKSUM = 0x1007,

		REG_PDAF_FLAG = 0x0840,
		REG_PDAF_PROC1_FIRSTADDR = 0x0841,
		REG_PDAF_PROC2_FIRSTADDR = 0x0A31,
		REG_PDAF_PROC1_SUMADDR = 0x0D9D,
		REG_PDAF_PROC2_SUMADDR= 0x0D9E,

		PAGE_GROUP1_START = 1,
		PAGE_GROUP2_START = 1,

		REG_PDAF_SUMADDR = REG_INVALID,
	};

	enum TYD8583M_OTP_Basic_Info
	{
		BASIC_MID = 0x42,
		BASIC_LENSID = 0x32,
		BASIC_SENSOR = 0x00,

		BASIC_VCMID = 0x2B,
		BASIC_DRIVERICID = 0x18,
		BASIC_EEPROMID = 0x29,
		BASIC_COLORTEMP = 0x01,
		BASIC_BITEN = 0x0F,
		BASIC_MTKVER1= 0xFF,
		BASIC_MTKVER2= 0x00,
		BASIC_MTKVER3= 0x0B,
		BASIC_MTKVER4= 0x01,
		BASIC_IRID = 0x01,
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

	enum Reserverd
	{
		RESERVERD_DATA = 0,

		RESERVERD_BASIC = 10,
		RESERVERD_AWB = 1,
		RESERVERD_LSC = 0,
		RESERVERD_AF =0,
		RESERVERD_PDAF1 = 8,
		RESERVERD_PDAF2 = 0,
	};

	enum TYD8583M_OTP_AF_Info
	{
		AF_LENSPIN = 1,
		AF_DIRECTION = 1,

	};

	typedef struct _TYD8583MOTPDataSpec//包含数据和Checksum
	{
		BYTE BaseInfo[SIZE_BASIC_GROUP];
		BYTE LSCInfo[SIZE_LSC_GROUP];
		BYTE AWBInfo[SIZE_WB_GROUP];
		BYTE AFInfo[SIZE_AF_GROUP];
		BYTE PDAF1[SIZE_PDAF_PROC1];
	}TYD8583MOTPDataSpec;

	typedef union TYD8583MEEPROMOtpGroupUnion
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

					BYTE basic_mtkver1;
					BYTE basic_mtkver2;
					BYTE basic_mtkver3;
					BYTE basic_mtkver4;

					BYTE basic_year;
					BYTE basic_month;
					BYTE basic_day;

					BYTE basic_mid;
					BYTE basic_lens;
					BYTE basic_vcm;
					BYTE basic_driverIC;

					BYTE basic_eepromid;
					BYTE basic_temperature;
					BYTE basic_biten;
					BYTE basic_reservedBasic[6];
					BYTE basic_checkSum;
					//////////////////////////////////////////////////////////////////////////
// 				BYTE basic_IR;

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

				};
			};
			union
			{
				BYTE data_WB[SIZE_WB_GROUP+1];
				struct
				{
					BYTE wb_flag;

					BYTE  wb_currentRG_L;
					BYTE  wb_currentRG_H;
					BYTE  wb_currentBG_L;	
					BYTE  wb_currentBG_H;
					BYTE  wb_currentGbGr_L;
					BYTE  wb_currentGbGr_H;

					BYTE  wb_goldenRG_L;
					BYTE  wb_goldenRG_H;
					BYTE  wb_goldenBG_L;
					BYTE  wb_goldenBG_H; 
					BYTE  wb_goldenGbGr_L;
					BYTE  wb_goldenGbGr_H;

					BYTE  wb_currentR_L;
					BYTE  wb_currentB_L;
					BYTE  wb_currentGb_L;
					BYTE  wb_currentGr_L;

					BYTE  wb_goldenR_L;
					BYTE  wb_goldenB_L;
					BYTE  wb_goldenGb_L;
					BYTE  wb_goldenGr_L;

					BYTE wb_checkSum;
					//////////////////////////////////////////////////////////////////////////
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
//					BYTE reservedWB[1];
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
					BYTE pdaf_flag;
					BYTE pdaf_proc1[SIZE_PDAF_PROC1];
					BYTE pdaf_proc2[SIZE_PDAF_PROC2];
					BYTE pdaf_proc1checksum;
					BYTE pdaf_pdaf2checksum;
				};
			};
		};
	}OtpGroup;
};
