#pragma  once
#include "..\..\OtpBase.h"

class COtpdefineSAA3_0L2K :  public COtpBase
{
public:
	enum normal
	{
#ifndef Raw8Data
		SENSOROB = 64,
#else
		SENSOROB = 16,
#endif 
		AWB_FACTOR = 1023, // OTP Guide中的 Convert float to integer , 如果没有就默认1023

		// 多组烧录时填写组间gap
		// EEProgram时没有分组所以为0
		OTPGAP_BASIC = 0,//only for multi otp group
		OTPGAP_AF = 0,

		QUALLSCCOMPRESS = 0,//0 1768bytes  1 1105 bytes  only for qual lsc
	};

	enum Reserverd
	{
		Reserverd_Data = 0,

		Reserve_Basic = 0,
		Reserve_AWB = 0,
		Reserve_LSC = 0,
		Reserve_AF =0,
		Reserve_PDAF1 = 0,
		Reserve_PDAF2 = 0,
	};

	// SIZE_xxx_GROUP 不算Flag
	enum SAA3_0L2K_OTP_SIZE
	{
		SIZE_BASIC_GROUP = 0x08-0x00, //除掉Flag的所有数据,用于计算checksum //
		SIZE_WB_GROUP = 0x26-0x19, //除掉Flag的所有数据

		SIZE_LSC_GROUP = SIZE_QUAL_LSC+1, //除掉Flag的所有数据
		SIZE_AF_GROUP = 0x16 - 0x0c, //除掉Flag的所有数据

		SIZE_PDAF_GROUP1 = Reserve_PDAF1 + SIZE_QUAL_L_PDAF_PROC1+1, //除掉Flag的所有数据
		SIZE_PDAF_GROUP2 = Reserve_PDAF2 + SIZE_QUAL_L_PDAF_PROC2+1,//除掉Flag的所有数据
		SIZE_PDAF_GROUP = SIZE_PDAF_GROUP1 + SIZE_PDAF_GROUP2,//1为PDAF2 Flag

		SIZE_TOTAL_GROUP = SIZE_BASIC_GROUP + SIZE_WB_GROUP + SIZE_LSC_GROUP + SIZE_AF_GROUP + SIZE_PDAF_GROUP1,
	};

	enum SAA3_0L2K_REG_ADDR
	{
		GAIN_ADDR_GR = 0x020e,
		GAIN_ADDR_R  = 0x0210,
		GAIN_ADDR_B  = 0x0212,
		GAIN_ADDR_GB = 0x0214,
		GAIN_DEFAULT_VAL = 0x0100,

// 	SAA3_0L2K_ADDR_GAIN_GR_H = 0x0508,
// 	SAA3_0L2K_ADDR_GAIN_GR_L = 0x0509,	
// 	SAA3_0L2K_ADDR_GAIN_R_H  = 0x050C,
// 	SAA3_0L2K_ADDR_GAIN_R_L  = 0x050D,
// 	SAA3_0L2K_ADDR_GAIN_B_H  = 0x050E,
// 	SAA3_0L2K_ADDR_GAIN_B_L  = 0x050F,
// 	SAA3_0L2K_ADDR_GAIN_GB_H = 0x050A,
// 	SAA3_0L2K_ADDR_GAIN_GB_L = 0x050B,
	};

	enum SAA3_0L2K_OTP_FLAG
	{
		FLAG_EMPTY = 0x00,
		FLAG_VALID = 0x01,

// 	SAA3_0L2K_FLAG_GROUP1 = 0x01,
// 	SAA3_0L2K_FLAG_GROUP2 = 0x13,
// 	SAA3_0L2K_FLAG_GROUP3 = 0x37,
	};

	enum SAA3_0L2K_OTP_REG_ADDR
	{
		REG_BASIC_FLAG = 0x0000,
		REG_BASIC_CHECKSUM = 0x0008,
		REG_WB_FLAG = 0x0017,
		REG_WB_CHECKSUM = 0x0026,
		REG_LSC_FLAG = 0x0027,
		REG_LSC_CHECKSUM = 0x0710,
		REG_AF_FLAG = 0x000C,
		REG_AF_CHECKSUM = 0x0016,

		REG_PDAF_PROC1_FLAG = 0x0711,
		REG_PDAF_PROC1_CHECKSUM = 0x0A8C,
		REG_PDAF_PROC2_FLAG = 0x0A8D,
		REG_PDAF_PROC2_CHECKSUM = 0x0AF4,

// 		REG_PDAF_PROC1_FIRSTADDR = REG_INVALID,
// 		REG_PDAF_PROC2_FIRSTADDR = REG_INVALID,
// 		REG_PDAF_PROC1_SUMADDR = REG_INVALID,
// 		REG_PDAF_PROC2_SUMADDR= REG_INVALID,

		PAGE_GROUP1_START = 1,
		PAGE_GROUP2_START = 1,

		REG_PDAF_SUMADDR = REG_INVALID,
	};

	enum SAA3_0L2K_OTP_Basic_Info
	{
		BASIC_MID = 0x23,
		BASIC_LENSID = 0x00,
		BASIC_VCMID = 0x50,
		BASIC_DRIVERICID = 0x40,

		BASIC_SENSOR = 0x04,
		BASIC_EEPROMID = 0x19,
		BASIC_COLORTEMP = 0x01,
		BASIC_BITEN = 0x0F,
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
		BASIC_FNO = 0,
	};

	typedef struct tagSAA3_0L2K_OTPDataSpec//包含数据和Checksum
	{
		BYTE BaseInfo[SIZE_BASIC_GROUP];
		BYTE LSCInfo[SIZE_LSC_GROUP];
		BYTE AWBInfo[SIZE_WB_GROUP];
		BYTE AFInfo[SIZE_AF_GROUP];
		BYTE PDAF1[SIZE_PDAF_GROUP1];
	}SAA3_0L2K_OTPDataSpec;

	typedef union SAA3_0L2K_EEPROMOtpGroupUnion
	{
		BYTE data[SIZE_TOTAL_GROUP];
		struct
		{
			// basic
			union
			{
				BYTE data_Basic[SIZE_BASIC_GROUP+4];//+1为Flag
				struct
				{
					BYTE basic_flag;
					BYTE basic_mid;
					BYTE basic_year;
					BYTE basic_month;
					BYTE basic_day;
					BYTE basic_lens;
					BYTE basic_driverIC;
					BYTE basic_vcm;
 					BYTE basic_checkSum;

// 
// 					BYTE basic_eepromid;
// 					BYTE basic_temperature;
// 					BYTE basic_biten;
// 					BYTE basic_reservedBasic[6];

					//////////////////////////////////////////////////////////////////////////
// 					BYTE basic_mtkver1;
// 					BYTE basic_mtkver2;
// 					BYTE basic_mtkver3;
// 					BYTE basic_mtkver4;
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
			// awb
			union
			{
				BYTE data_WB[SIZE_WB_GROUP+3];
				struct
				{
					BYTE wb_flag;
					BYTE  wb_currentRG_H;
					BYTE  wb_currentRG_L;
					BYTE  wb_currentBG_H;	
					BYTE  wb_currentBG_L;	
					BYTE  wb_currentGbGr_H;
					BYTE  wb_currentGbGr_L;

					BYTE  wb_goldenRG_H;
					BYTE  wb_goldenRG_L;
					BYTE  wb_goldenBG_H; 
					BYTE  wb_goldenBG_L;
					BYTE  wb_goldenGbGr_H;
					BYTE  wb_goldenGbGr_L;

					BYTE wb_checkSum;

// 					BYTE  wb_currentR_L;
// 					BYTE  wb_currentB_L;
// 					BYTE  wb_currentGb_L;
// 					BYTE  wb_currentGr_L;
// 
// 					BYTE  wb_goldenR_L;
// 					BYTE  wb_goldenB_L;
// 					BYTE  wb_goldenGb_L;
// 					BYTE  wb_goldenGr_L;


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
//				BYTE reservedWB[1];
				};
			};
			// lsc
			union
			{
				BYTE data_Lenc[SIZE_LSC_GROUP+1];
				struct
				{
					BYTE lsc_flag;
					BYTE lsc_data[SIZE_LSC_GROUP-1];
					BYTE lsc_checksum;
	      		//	BYTE reservedLenc[50];
				};
			};
			// af
			union
			{
				BYTE data_AF[SIZE_AF_GROUP+1];
				struct
				{
					BYTE af_flag;
 					BYTE af_cali_direction;
					BYTE af_inf_h;
					BYTE af_inf_l;
					BYTE af_mac_h;
					BYTE af_mac_l;
					BYTE af_start_h;
					BYTE af_start_l;
					BYTE af_reserved[2];
					BYTE af_checksum;
	 			//	BYTE af_lens_spin;

				};
			};
			// pdaf
			union
			{
				BYTE data_PDAF[SIZE_PDAF_GROUP + 1];
				struct
				{
					union
					{
						BYTE data_PDAF1[SIZE_PDAF_GROUP1 + 1];
						struct
						{
							BYTE pdaf1_flag;
							BYTE pdaf_proc1[SIZE_PDAF_GROUP1];
							BYTE pdaf_proc1checksum;
						//	BYTE reservedpdaf1[Reserve_PDAF1];
						};
					};
					union
					{
						BYTE data_PDAF2[SIZE_PDAF_GROUP2 + 1];
						struct
						{
							BYTE pdaf2_flag;
							BYTE pdaf_proc2[SIZE_PDAF_GROUP2];
							BYTE pdaf_pdaf2checksum;
						//  BYTE reservedpdaf2[Reserve_PDAF2];
						};
					};
				};
			};
		};
	}OtpGroup;
};
