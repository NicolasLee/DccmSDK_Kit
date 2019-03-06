#pragma  once
enum S5K3L8_KB_PD_REG_ADDR
{
	S5K3L8_KB_PD_ADDR_EXP_H = 0x0202,
	S5K3L8_KB_PD_ADDR_EXP_L = 0x0203,

	S5K3L8_KB_PD_ADDR_GAIN_GR_H = 0x020E,
	S5K3L8_KB_PD_ADDR_GAIN_GR_L = 0x020F,
	S5K3L8_KB_PD_ADDR_GAIN_R_H  = 0x0210,
	S5K3L8_KB_PD_ADDR_GAIN_R_L  = 0x0211,
	S5K3L8_KB_PD_ADDR_GAIN_B_H  = 0x0212,
	S5K3L8_KB_PD_ADDR_GAIN_B_L  = 0x0213,
	S5K3L8_KB_PD_ADDR_GAIN_GB_H = 0x0214,
	S5K3L8_KB_PD_ADDR_GAIN_GB_L = 0x0215,

	S5K3L8_KB_PD_DATA_DEFAULT_GAIN = 0x0100,
};

enum S5K3L8_KB_PD_EEPROM_FLAG
{
	S5K3L8_KB_PD_FLAG_VALID = 0x80,
	S5K3L8_KB_PD_FLAG_INVALID = 0xFF,
	S5K3L8_KB_PD_FLAG_EMPTY = 0x00,
};

enum S5K3L8_KB_PD_MTK_OTP_SIZE
{
	S5K3L8_KB_PD_MTK_OTP_GROUP_SIZE = 2048,
	S5K3L8_KB_PD_MTK_OTP_BASIC_GROUP_SIZE = 64,
	S5K3L8_KB_PD_MTK_OTP_WB_GROUP_SIZE = 64,
	S5K3L8_KB_PD_MTK_OTP_LSC_GROUP_SIZE = 1920,
	S5K3L8_KB_PD_MTK_OTP_LSC_SIZE = 1868,
};

typedef union S5K3L8_KB_PDEEPROMOtpGroupUnion
{
	BYTE data[S5K3L8_KB_PD_MTK_OTP_GROUP_SIZE];
	struct
	{
		union
		{
			BYTE dataBasic[S5K3L8_KB_PD_MTK_OTP_BASIC_GROUP_SIZE];
			struct
			{
				BYTE flag;
				BYTE year;
				BYTE month;
				BYTE date;
				BYTE vendor;
				BYTE lens;
				BYTE vcm;
				BYTE driverIC;
				BYTE IR;
				BYTE temperature;
				BYTE afType;
				BYTE lightSource;
				BYTE lencDll;
				BYTE calibrationVersion[4];
				BYTE serialNum[3];
				BYTE pdDll;
				BYTE checkSumBasic;
				BYTE reservedBasic[42];
			};
		};
		union
		{
			BYTE dataWB[S5K3L8_KB_PD_MTK_OTP_WB_GROUP_SIZE];
			struct
			{
				BYTE flagWB;
				BYTE currentRG_L;
				BYTE currentRG_H;
				BYTE currentBG_L;
				BYTE currentBG_H;
				BYTE currentGbGr_L;
				BYTE currentGbGr_H;
				BYTE checkSumWB;
				BYTE reservedWB[56];
			};
		};
		union
		{
			BYTE dataLenc[S5K3L8_KB_PD_MTK_OTP_LSC_GROUP_SIZE];
			struct
			{
				BYTE flagLenc;
				BYTE lenc[S5K3L8_KB_PD_MTK_OTP_LSC_SIZE];
				BYTE checkSumLenc;
				BYTE reservedLenc[50];
			};
		};
	};
}S5K3L8_KB_PDEEPROMOtpGroup;