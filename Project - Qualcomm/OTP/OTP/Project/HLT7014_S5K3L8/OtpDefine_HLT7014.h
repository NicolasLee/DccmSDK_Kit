#pragma  once
#include "..\..\OtpBase.h"

class COtpdefineHLT7014 :  public COtpBase
{
	public:
enum normal
{
//#ifndef Raw8Data 
	SENSOROB = 64,//sensor ob
// #else
// 	SENSOROB = 16,//sensor ob
// #endif
	AWB_FACTOR = 1024,//awb calc factor
	QUALLSCCOMPRESS = 1,//only for qual lsc data 0 1768bytes  1 1105 bytes
	OTPGAP = 32,//only for multi otp group
};

enum Reserverd
{
	Reserverd_Data = 0xFF,

	Reserve_Basic = 0,
	Reserve_AWB = 0,
	Reserve_LSC = 0,
	Reserve_AF =0,
	Reserve_PDAF1 = 0,
	Reserve_PDAF2 = 0,
	Reserve_L_PDAF1 = 0,
	Reserve_L_PDAF2 = 0,
};
enum HLT7014_OTP_SIZE
{
	SIZE_LSC = SIZE_QUAL_LSC,
	SIZE_PDAF_PROC1 = SIZE_QUAL_PDAF_PROC1,//SIZE_MTK_PDAF_PROC1,
	SIZE_PDAF_PROC2 = SIZE_QUAL_PDAF_PROC2,//SIZE_MTK_PDAF_PROC2,
	SIZE_L_PDAF_PROC1 = SIZE_QUAL_L_PDAF_PROC1,
	SIZE_L_PDAF_PROC2 = SIZE_QUAL_L_PDAF_PROC2,

	SIZE_BASIC_GROUP = Reserve_Basic+17, //除掉Flag的所有数据
	SIZE_WB_GROUP = Reserve_AWB+8, //除掉Flag的所有数据
	SIZE_LSC_GROUP = Reserve_LSC+1106, //除掉Flag的所有数据
	SIZE_AF_GROUP = Reserve_AF+4, //除掉Flag的所有数据
	SIZE_PDAF_GROUP1 = Reserve_PDAF1+SIZE_PDAF_PROC1, //除掉Flag的所有数据
	SIZE_PDAF_GROUP2 = Reserve_PDAF2+SIZE_PDAF_PROC2+1,//除掉Flag的所有数据
	SIZE_PDAF_GROUP = SIZE_PDAF_GROUP1+SIZE_PDAF_GROUP2,//1为PDAF2 Flag

	SIZE_LPDAF_GROUP1 = Reserve_L_PDAF1+SIZE_L_PDAF_PROC1+1, //除掉Flag的所有数据
	SIZE_LPDAF_GROUP2 = Reserve_L_PDAF2+SIZE_L_PDAF_PROC2+1,//除掉Flag的所有数据
	SIZE_LPDAF_GROUP = SIZE_LPDAF_GROUP1+SIZE_LPDAF_GROUP2,

	SIZE_TOTAL_GROUP = 2130, //第一个Data到最后一个Data
};

enum HLT7014_REG_ADDR
{
	GAIN_ADDR_GR = 0x020e,
	GAIN_ADDR_R  = 0x0210,
	GAIN_ADDR_B  = 0x0212,
	GAIN_ADDR_GB = 0x0214,
	GAIN_DEFAULT_VAL = 0x0100,

// 	HLT7014_ADDR_GAIN_GR_H = 0x0508,
// 	HLT7014_ADDR_GAIN_GR_L = 0x0509,	
// 	HLT7014_ADDR_GAIN_R_H  = 0x050C,
// 	HLT7014_ADDR_GAIN_R_L  = 0x050D,
// 	HLT7014_ADDR_GAIN_B_H  = 0x050E,
// 	HLT7014_ADDR_GAIN_B_L  = 0x050F,
// 	HLT7014_ADDR_GAIN_GB_H = 0x050A,
// 	HLT7014_ADDR_GAIN_GB_L = 0x050B,
};

enum HLT7014_OTP_FLAG
{
	FLAG_EMPTY = 0x00,
	FLAG_VALID = 0x01,
//	FLAG_INVAID = 0x1F,

// 	HLT7014_FLAG_GROUP1 = 0x01,
// 	HLT7014_FLAG_GROUP2 = 0x13,
// 	HLT7014_FLAG_GROUP3 = 0x37,
};

enum HLT7014_OTP_REG_ADDR
{
	REG_BASIC_FLAG = 0x0000,
	REG_BASIC_CHECKSUM = REG_INVALID,

	REG_WB_FLAG = 0x0012,
	REG_WB_CHECKSUM = REG_INVALID,

	REG_LSC_FLAG = 0x001E,
	REG_LSC_CHECKSUM = 0x046F,

	REG_AF_FLAG = 0x001A,
	REG_AF_CHECKSUM = REG_INVALID,

	REG_PDAF_PROC1_FLAG=0x0470,
	REG_PDAF_PROC1_FIRSTADDR = REG_INVALID,
	REG_PDAF_PROC1_SUMADDR = 0x07F2,

	REG_PDAF_PROC2_FLAG=REG_INVALID,
	REG_PDAF_PROC2_FIRSTADDR = REG_INVALID,
	REG_PDAF_PROC2_SUMADDR= REG_INVALID,

	REG_LPDAF_PROC1_FLAG=REG_INVALID,
	REG_LPDAF_PROC1_FIRSTADDR = REG_INVALID,
	REG_LPDAF_PROC1_SUMADDR = REG_INVALID,

	REG_LPDAF_PROC2_FLAG=REG_INVALID,
	REG_LPDAF_PROC2_FIRSTADDR = REG_INVALID,
	REG_LPDAF_PROC2_SUMADDR= REG_INVALID,

	REG_PDAF_FLAG = REG_INVALID,
	REG_PDAF_SUMADDR = REG_INVALID,
	REG_LPDAF_FLAG = REG_INVALID,
	REG_LPDAF_SUMADDR = REG_INVALID,
};

enum HLT7014_OTP_Basic_Info
{
	BASIC_MID = 0x42,
	BASIC_LENSID = 0x01,
	BASIC_VCMID = 0x07,
	BASIC_OISID = 0x00,
	BASIC_CUSTOM = 0x07,
	BASIC_FIRSTPIXEL = 0x01,
	BASIC_MIRRORFLIP = 0,
	BASIC_WIDTH = 4208,
	BASIC_HEIGHT = 3120,
	BASIC_LSCVER = 0x0D,
	BASIC_PDAFVER = 0x04,
	BASIC_AWBAF_H = 0x0F,
	BASIC_AWBAF_L = 0x01,
// 	BASIC_DRIVERICID = 0x17,

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

typedef struct _HLT7014OTPDataSpec//包含数据和Checksum
{
	BYTE BaseInfo[SIZE_BASIC_GROUP];
	BYTE LSCInfo[SIZE_LSC_GROUP];
	BYTE AWBInfo[SIZE_WB_GROUP];
	BYTE AFInfo[SIZE_AF_GROUP];
	BYTE PDAF1[SIZE_PDAF_PROC1];
	BYTE LPDAF1[SIZE_L_PDAF_PROC1];
	BYTE DATE[3];
}HLT7014OTPDataSpec;

typedef union HLT7014EEPROMOtpGroupUnion
{
	BYTE data[SIZE_TOTAL_GROUP];
	struct
	{
		union
		{
			BYTE data_Basic[SIZE_BASIC_GROUP+1];
			struct
			{
				BYTE flag;
				BYTE basic_firstpixel;
				BYTE basic_mirrorflip;
				BYTE basic_width[2];
				BYTE basic_height[2];
				BYTE basic_lscver[2];
				BYTE basic_pdafver[2];
				BYTE basic_mid;
				BYTE basic_lens;
				BYTE basic_year;
				BYTE basic_month;
				BYTE basic_day;
				BYTE basic_awbaf[2];

					//////////////////////////////////////////////////////////////////////////
// 				BYTE basic_vcm;
// 				BYTE basic_ois;
// 				BYTE basic_checkSum;
// 				BYTE basic_reservedBasic[Reserve_Basic];
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
			BYTE data_WB[SIZE_WB_GROUP];
			struct
			{
				BYTE  wb_currentR;
				BYTE  wb_currentGr;
				BYTE  wb_currentGb;			
				BYTE  wb_currentB;

				BYTE  wb_goldenR;
				BYTE  wb_goldenGr;
				BYTE  wb_goldenGb;
				BYTE  wb_goldenB;
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
			BYTE data_AF[SIZE_AF_GROUP];
			struct
			{
// 				BYTE af_mac_h;
// 				BYTE af_mac_l;
// 				BYTE af_inf_h;
// 				BYTE af_inf_l;
				BYTE af_inf_l;
				BYTE af_inf_h;
				BYTE af_mac_l;
				BYTE af_mac_h;

				//////////////////////////////////////////////////////////////////////////
				// 				BYTE af_lens_spin;

			};
		};
		union
		{
			BYTE data_Lenc[SIZE_LSC_GROUP];
			struct
			{
				BYTE lsc_data[SIZE_QUAL_COMPRESS_LSC];
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
//						BYTE pdaf1_flag;
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
			BYTE data_LPDAF[SIZE_LPDAF_GROUP];
			struct
			{
				union
				{
					BYTE data_LPDAF1[SIZE_LPDAF_GROUP1];
					struct
					{
						//						BYTE lpdaf1_flag;
						BYTE lpdaf_proc1[SIZE_L_PDAF_PROC1];
						BYTE lpdaf_proc1checksum;
						//						BYTE reservedpdaf1[Reserve_PDAF1];
					};
				};
				union
				{
					BYTE data_LPDAF2[SIZE_LPDAF_GROUP2];
					struct
					{
						//						BYTE lpdaf2_flag;
						BYTE lpdaf_proc2[SIZE_L_PDAF_PROC2];
						BYTE lpdaf_pdaf2checksum;
						//////////////////////////////////////////////////////////////////////////
						//            	BYTE reservedpdaf2[Reserve_PDAF2];
					};
				};
			};
		};
// 		union
// 		{
// 			BYTE data_date[3];
// 			struct
// 			{
// 				BYTE basic_year;
// 				BYTE basic_month;
// 				BYTE basic_day;
// 			};
// 		};
	};
}OtpGroup;
};