#pragma  once
#include "..\..\OtpBase.h"

class COtpdefineHLT7012 :  public COtpBase
{
	public:
enum normal
{
#ifndef Raw8Data 
	SENSOROB = 64,//sensor ob
#else
	SENSOROB = 16,//sensor ob
#endif
	AWB_FACTOR = 1024,//awb calc factor
	QUALLSCCOMPRESS = 0,//only for qual lsc data 0 1768bytes  1 1105 bytes
	OTPGAP = 32,//only for multi otp group
};

enum Reserverd
{
	Reserverd_Data = 0xFF,

	Reserve_Basic = 1,
	Reserve_AWB = 8,
	Reserve_LSC = 0,
	Reserve_AF =0,
	Reserve_PDAF1 = 0,
	Reserve_PDAF2 = 0,
	Reserve_L_PDAF1 = 0,
	Reserve_L_PDAF2 = 0,
};
enum HLT7012_OTP_SIZE
{
	SIZE_LSC = SIZE_QUAL_LSC,
	SIZE_PDAF_PROC1 = SIZE_QUAL_PDAF_PROC1,//SIZE_MTK_PDAF_PROC1,
	SIZE_PDAF_PROC2 = SIZE_QUAL_PDAF_PROC2,//SIZE_MTK_PDAF_PROC2,
	SIZE_L_PDAF_PROC1 = SIZE_QUAL_L_PDAF_PROC1,
	SIZE_L_PDAF_PROC2 = SIZE_QUAL_L_PDAF_PROC2,

	SIZE_BASIC_GROUP = Reserve_Basic+9, //除掉Flag的所有数据
	SIZE_WB_GROUP = Reserve_AWB+7, //除掉Flag的所有数据
	SIZE_LSC_GROUP = Reserve_LSC+1769, //除掉Flag的所有数据
	SIZE_AF_GROUP = Reserve_AF+6, //除掉Flag的所有数据
	SIZE_PDAF_GROUP1 = Reserve_PDAF1+SIZE_PDAF_PROC1+1, //除掉Flag的所有数据
	SIZE_PDAF_GROUP2 = Reserve_PDAF2+SIZE_PDAF_PROC2+1,//除掉Flag的所有数据
	SIZE_PDAF_GROUP = SIZE_PDAF_GROUP1+SIZE_PDAF_GROUP2+1,//1为PDAF2 Flag

	SIZE_L3PDAF_GROUP1 = Reserve_L_PDAF1+SIZE_L_PDAF_PROC1+1, //除掉Flag的所有数据
	SIZE_L3PDAF_GROUP2 = Reserve_L_PDAF2+SIZE_L_PDAF_PROC2+1,//除掉Flag的所有数据
	SIZE_L3PDAF_GROUP = SIZE_L3PDAF_GROUP1+SIZE_L3PDAF_GROUP2+1,//1为PDAF2 Flag

	SIZE_TOTAL_GROUP = 3719, //第一个Data到最后一个Data
};

enum HLT7012_REG_ADDR
{
	GAIN_ADDR_GR = 0x020e,
	GAIN_ADDR_R  = 0x0210,
	GAIN_ADDR_B  = 0x0212,
	GAIN_ADDR_GB = 0x0214,
	GAIN_DEFAULT_VAL = 0x0100,

// 	HLT7012_ADDR_GAIN_GR_H = 0x0508,
// 	HLT7012_ADDR_GAIN_GR_L = 0x0509,	
// 	HLT7012_ADDR_GAIN_R_H  = 0x050C,
// 	HLT7012_ADDR_GAIN_R_L  = 0x050D,
// 	HLT7012_ADDR_GAIN_B_H  = 0x050E,
// 	HLT7012_ADDR_GAIN_B_L  = 0x050F,
// 	HLT7012_ADDR_GAIN_GB_H = 0x050A,
// 	HLT7012_ADDR_GAIN_GB_L = 0x050B,
};

enum HLT7012_OTP_FLAG
{
	FLAG_EMPTY = 0xFF,
	FLAG_VALID = 0x01,
//	FLAG_INVAID = 0x1F,

// 	HLT7012_FLAG_GROUP1 = 0x01,
// 	HLT7012_FLAG_GROUP2 = 0x13,
// 	HLT7012_FLAG_GROUP3 = 0x37,
};

enum HLT7012_OTP_REG_ADDR
{
	REG_BASIC_FLAG = 0x0000,
	REG_BASIC_CHECKSUM = 0x000A,

	REG_WB_FLAG = 0x0010,
	REG_WB_CHECKSUM = 0x001F,

	REG_LSC_FLAG = 0x0027,
	REG_LSC_CHECKSUM = 0x0710,

	REG_AF_FLAG = 0x0020,
	REG_AF_CHECKSUM = 0x0026,

	REG_PDAF_PROC1_FLAG=0x0711,
	REG_PDAF_PROC1_FIRSTADDR = 0x0712,
	REG_PDAF_PROC1_SUMADDR = 0x0A93,

	REG_PDAF_PROC2_FLAG=0x0A94,
	REG_PDAF_PROC2_FIRSTADDR = 0x0A95,
	REG_PDAF_PROC2_SUMADDR= 0x0A99,

	REG_L3PDAF_PROC1_FLAG=0x0AA0,
	REG_L3PDAF_PROC1_FIRSTADDR = 0x0AA1,
	REG_L3PDAF_PROC1_SUMADDR = 0x0E1B,

	REG_L3PDAF_PROC2_FLAG=0x0E1C,
	REG_L3PDAF_PROC2_FIRSTADDR = 0x0E1D,
	REG_L3PDAF_PROC2_SUMADDR= 0x0E83,

	REG_PDAF_FLAG = REG_INVALID,
	REG_PDAF_SUMADDR = REG_INVALID,
	REG_L3PDAF_FLAG = REG_INVALID,
	REG_L3PDAF_SUMADDR = REG_INVALID,
};

enum HLT7012_OTP_Basic_Info
{
	BASIC_MID = 0x99,
	BASIC_LENSID = 0x01,
	BASIC_VCMID = 0x07,
	BASIC_OISID = 0x00,
	BASIC_CUSTOM = 0x07,
// 	BASIC_DRIVERICID = 0x17,
// 	BASIC_LSCVER = 0x10,
// 	BASIC_PDAFVER = 0x0A,
// 	BASIC_COLORTEMP = 0x32,
// 
// 	BASIC_IRID = 0x01,
// 	BASIC_AFFLAG= 0x01,
// 	BASIC_LIGHTFLAG= 0x02,
// 	BASIC_CALIVER1 = 0x01,
// 	BASIC_CALIVER2 = 0,
// 	BASIC_CALIVER3 = 0,
// 	BASIC_CALIVER4 = 0,
// 	BASIC_SERIALNO1 = 0,
// 	BASIC_SERIALNO2 = 0,
// 	BASIC_SERIALNO3  = 0,
};

typedef struct _HLT7012OTPDataSpec//包含数据和Checksum
{
	BYTE BaseInfo[SIZE_BASIC_GROUP];
	BYTE LSCInfo[SIZE_LSC_GROUP];
	BYTE AWBInfo[SIZE_WB_GROUP];
	BYTE AFInfo[SIZE_AF_GROUP];
	BYTE PDAF1[SIZE_PDAF_PROC1];
	BYTE L3PDAF1[SIZE_L_PDAF_PROC1];

	BYTE DATE[3];
}HLT7012OTPDataSpec;

typedef union HLT7012EEPROMOtpGroupUnion
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
				BYTE basic_lens;
				BYTE basic_vcm;
				BYTE basic_ois;
				BYTE basic_year;
				BYTE basic_month;
				BYTE basic_day;
				BYTE basic_custom;
				BYTE basic_reservedBasic[Reserve_Basic];
				BYTE basic_checkSum;
				//////////////////////////////////////////////////////////////////////////
// 				BYTE basic_driverIC;
// 				BYTE basic_lencDll;
// 				BYTE basic_pdafDll;
// 				BYTE basic_colorTemp;
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

// 				BYTE  wb_currentRG_H;
// 				BYTE  wb_currentRG_L;
// 				BYTE  wb_currentBG_H;
// 				BYTE  wb_currentBG_L;
// 				BYTE  wb_currentGbGr_H;
// 				BYTE  wb_currentGbGr_L;

				BYTE  wb_currentRG_L;
				BYTE  wb_currentRG_H;
				BYTE  wb_currentBG_L;
				BYTE  wb_currentBG_H;
				BYTE  wb_currentGbGr_L;
				BYTE  wb_currentGbGr_H;

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
			BYTE data_AF[SIZE_AF_GROUP+1];
			struct
			{
				BYTE af_flag;
				BYTE af_cali_direction;
// 				BYTE af_mac_h;
// 				BYTE af_mac_l;
// 				BYTE af_inf_h;
// 				BYTE af_inf_l;
				BYTE af_inf_l;
				BYTE af_inf_h;
				BYTE af_mac_l;
				BYTE af_mac_h;
				
				BYTE af_checksum;
				//////////////////////////////////////////////////////////////////////////
				// 				BYTE af_lens_spin;

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
//						BYTE reservedpdaf1[Reserve_PDAF1];
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
//						BYTE reservedpdaf2[Reserve_PDAF2];
						//////////////////////////////////////////////////////////////////////////
//            	BYTE reservedpdaf2[Reserve_PDAF2];
					};
				};
			};
		};
		union
		{
			BYTE data_L3PDAF[SIZE_L3PDAF_GROUP+1];
			struct
			{
				union
				{
					BYTE data_L3PDAF1[SIZE_L3PDAF_GROUP1+1];
					struct
					{
						BYTE l3pdaf1_flag;
						BYTE l3pdaf_proc1[SIZE_L_PDAF_PROC1];
						BYTE l3pdaf_proc1checksum;
//						BYTE reservedpdaf1[Reserve_PDAF1];
					};
				};
				union
				{
					BYTE data_L3PDAF2[SIZE_L3PDAF_GROUP2+1];
					struct
					{
						BYTE l3pdaf2_flag;
						BYTE l3pdaf_proc2[SIZE_L_PDAF_PROC2];
						BYTE l3pdaf_pdaf2checksum;
						//////////////////////////////////////////////////////////////////////////
//            	BYTE reservedpdaf2[Reserve_PDAF2];
					};
				};
			};
		};
		union
		{
			BYTE data_date[3];
			struct
			{
				BYTE basic_year;
				BYTE basic_month;
				BYTE basic_day;
			};
		};
	};
}OtpGroup;
};