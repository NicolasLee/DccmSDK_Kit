#pragma once

//Qual PDAF
 #include "PDAFCalibrationTools_RevJ.h"//J version
// #include "PDAFCalibrationTools_Dll.h"//L3 version
// 
 #pragma  comment(lib,"PDAFCalibrationTools_RevJ_Dll.lib")
// #pragma  comment(lib,"PDAFCalibrationTools_RevL_Dll.lib")

#if 0
	#define EN_OTP_LOADING
#else
	#define EN_OTP
#endif

#define WM_OTP_PREVIEW_START     WM_USER+ 0x61
#define WM_OTP_PREVIEW_STOP      WM_USER+ 0x62
#define WM_OTP_EXIT              WM_USER+ 0x63
#define WM_OTP_START             WM_USER+ 0x64
#define WM_OTP_MANUAL_START      WM_USER+ 0x65
#define WM_OTP_UPDATE_DATA       WM_USER+ 0x66
#define WM_OTP_UPDATE_UI         WM_USER+ 0x67
#define WM_OTP_SHOW_FPS          WM_USER+ 0x68
#define WM_OTP_SHOW_AD_VALUE     WM_USER+ 0x69

#define WM_OTP_MULTI_CHANGE_LED  WM_USER+ 0x6A
#define WM_OTP_MULTI_STOP        WM_USER+ 0x6B
#define WM_OTP_MULTI_EXIT        WM_USER+ 0x6C

#define WM_DTHSDEMO_RESULT		 WM_USER+ 0x7D
#define WM_DTHSDEMO_TO_OTP		 WM_USER+ 0x7E
#define WM_DTHSDEMO_TO_OPRATE	 WM_USER+ 0x7F
#define WM_DTHSDEMO_STATUS_PLAY	 WM_USER+ 0x80
#define WM_DTHSDEMO_STATUS_STOP	 WM_USER+ 0x81


#ifndef SAFE_DELETE
	#define SAFE_DELETE(p)			{	if(p) { delete (p);						(p)=NULL; }	}
	#define SAFE_DELETE_ARRAY(p)	{	if(p) { delete [] (p);					(p)=NULL; }	}
	#define SAFE_FREE(p)			{	if(p) { free(p);						(p)=NULL; }	}
	#define SAFE_RELEASE(p)			{	if(p) { (p)->Release();					(p)=NULL; }	}
	#define SAFE_CLOSE_HANDLE(p)	{	if(p) { ::CloseHandle(p);				(p)=NULL; }	}
	#define SAFE_DELETE_OBJECT(p)	{	if(p) { ::DeleteObject((HGDIOBJ) p);	(p)=NULL; }	}
#endif  /* SAFE_DELETE */

#define REG_INVALID    0xFFFF
#define VAL_INVALID     0x0000

enum OTP_STATUS
{
	OTP_STATUS_NULL = 0,
	OTP_STATUS_EMPTY,
	OTP_STATUS_PROGRAMED,
	OTP_STATUS_ERROR,
};

enum OTP_PROGRAM_RESULT
{
	OTP_PROGRAM_NULL = 0,
	OTP_PROGRAM_SUCCESS,
	OTP_PROGRAM_FAILED,
	OTP_PROGRAM_SHADING_FAILED,
	OTP_WB_INVALID,
	OTP_CHECK_PASS,
	OTP_CHECK_FAIL,
};

enum OTP_UI_TYPE
{
	OTP_UI_PLAY_SUCCESS = 1,
	OTP_UI_PLAY_FAILED,
	OTP_UI_STOP,
	OTP_UI_PROGRAM_RESULT,
	OTP_UI_MACHINE_FAILED,
	OTP_UI_ID_FAILED,
};

enum OTP_CALIBRATION_STATE
{
	OTP_CALIBRATION_NULL = 0,
	OTP_CALIBRATION_CONTINUE,
	OTP_CALIBRATION_FINISHED,
	OTP_CALIBRATION_ERROR,
};

enum OTP_LED_STATE
{
	OTP_LED_NULL = 0,
	OTP_LED_ERROR,
	OTP_LED_CH3000K,
	OTP_LED_CH4000K,
	OTP_LED_CH5100K,
	OTP_LED_CH6500K,
	OTP_LED_CH3100K,
	OTP_LED_CH5600K,
};

enum WB_CALC_MODE
{
	WB_CALC_BASE_MIN = 0,
	WB_CALC_SINGLE_RB,
};

enum OTP_AF_STATE
{
	OTP_AF_NULL = 0,
	OTP_AF_RUNNING,
	OTP_AF_DONE,
	OTP_AF_ERROR,
};

enum OTP_AF_SCAN_STATE
{
	OTP_AF_SCAN_NULL = 0,
	OTP_AF_SCAN_START,
	OTP_AF_SCANNING,
	OTP_AF_SCAN_DONE,
	OTP_AF_SCAN_ERROR,
};

enum OTP_AF_SCAN_INDEX
{
	OTP_AF_MACRO = 0,
	OTP_AF_INFINITY,
	OTP_AF_MIDDLE,
	OTP_AF_START_CURRENT,
	OTP_AF_SCAN_NOTHING,
};

const CString OTP_AF_SCAN_DISTANCE[3] = {_T("Macro"), _T("Infinity"), _T("Middle")};


enum EXPOSURE_TYPE
{
	EXP_LINEAR = 0,
	EXP_AUTO,
};

enum EXP_FILTER_CHANNEL
{
	EXP_CHANNEL_G = 0,
	EXP_CHANNEL_Y,
	EXP_CHANNEL_GR,
	EXP_CHANNEL_GB,
	EXP_CHANNEL_MAX,
};

enum EXPOSURE_STATE
{
	EXP_CONTINUE = 0,
	EXP_OK_ONCE,
	EXP_FINISHED,
};

enum POWER_CHANNEL
{
	PWR_AVDD = 0,
	PWR_DOVDD,
	PWR_DVDD,
	PWR_AFVCC,
	PWR_VPP,
};

enum PIN_LEVEL
{
	PIN_LOW = 0,
	PIN_HIGH,
};

enum I2C_MODE
{
	I2C_MODE_A8D8 = 0,
	I2C_MODE_A8D8_S,
	I2C_MODE_A8D16,	
	I2C_MODE_A16D8,
	I2C_MODE_A16D16,
};

enum I2C_SPEED
{
	I2C_SPEED_100K = 0,
	I2C_SPEED_400K,
};

enum MODULE_ID_TYPE
{
	MODULE_ID_INPUT= 0,
	MODULE_ID_OTP,
	MODULE_ID_SENSOR_LOT,
	MODULE_ID_COMM,
};


typedef struct FrameGrabStruct
{
	ULONG okCount;
	UINT errorCount;
	UINT deviceErrorCount;
	UINT msUseTime;
} FrameGrabStruct;	

typedef struct
{
	UINT width;
	UINT height;
	BYTE format;
	BYTE sequence;
	BYTE bpp;
	ULONG size;
} VideoParamsStruct;

typedef struct
{
	BYTE addr;
	BYTE mode;
	BYTE speed;
} I2CStruct;

typedef struct
{
	UINT width;
	UINT height;
	BYTE format;
	BYTE sequence;
	int mhzMCLK;
	USHORT *params;
	USHORT paramSize;
} OtpSensorInitStruct;

typedef struct
{
	TCHAR *name;

	BYTE RESET;
	BYTE PWDN;

	int mvAVDD;
	int mvDOVDD;
	int mvCORE;
	int mvAFVCC;
	int mvVPP;

	I2CStruct i2c;

	USHORT flagReg1;
	USHORT flagData1;
	USHORT flagMask1;

	USHORT flagReg2;
	USHORT flagData2;
	USHORT flagMask2;

	BYTE port;

	BOOL polarPCLK;
	BOOL polarHSYNC;

	BYTE regEffectFrame;

	BYTE OB;
	UINT OBLine;

	OtpSensorInitStruct otpInit;
	OtpSensorInitStruct verifyInit;
	OtpSensorInitStruct afInit;		
} OtpSensorStruct;

typedef struct
{
	BYTE channel;
	UINT mvProgram;
	UINT mvNormal;
	UINT msStableTime;		
} OtpPowerStruct;

struct tagOtpCtrl
{
	BOOL manualReInit;
	BOOL enProgram;
	BOOL enForceCalibrate;
	BOOL enSaveRawBMP;
	BOOL enSaveCorrectBMP;
	BOOL enShowVideoOnOtp;
	BOOL enShowVideoAfterOtp;
	BOOL enLightTest;
	BOOL enLenc;
	BOOL enWB;
	BOOL enAF;
	BOOL EnYTLed;
	BOOL EnLedFixDevice;
	BOOL enCheckAF;
	BOOL enerase;
	BOOL enMTKLSC;
	int sensorob;
	int quallsccompress;
} ;

struct OtpIDStruct
{
	BYTE mid;
	BYTE apid;
	BYTE lens;
	BYTE vcm;
	BYTE driverIC;
	BYTE IR;
	BYTE blackmask;

	BYTE eeprom;
	BYTE colortemp;
	BYTE biten;
	BYTE mtkver1;
	BYTE mtkver2;
	BYTE mtkver3;
	BYTE mtkver4;
    BYTE sensor;
	BYTE FNO;
	BYTE aftype;
	BYTE vendor;
	BYTE pcb;
	BYTE platform;
	BYTE version;
	BYTE house;
} ;

typedef struct
{
	BOOL enDynamicInput;
	int inputHigh;
	int inputTarget;
	int inputLow;
	BOOL enCheckLenc;
	BOOL enShadingTestAfterOtp;
	BYTE shadingTestType;
	BOOL enUpdateLenc;
	BOOL enSaveLencFile;
	CString lencFileName;
	BOOL enSaveRawLencFile;
	CString lencRawFileName;
	BYTE Y_roiCalcMode;
	BYTE Col_roiCalcMode;
} LencOtpControlStruct;

typedef struct
{
	USHORT addrCtrl;
	USHORT lencEnable;
	USHORT lencDisable;
	USHORT addrDataStart;
	int dataSize;
} LencRegStruct;

typedef struct
{
	BOOL enLencBasedWB;
	BOOL enSaveRawLencBMP;
	BOOL enCheckWB;
	float percentDifRG;
	float percentDifBG;
	float percentDifRG_Pre;
	float percentDifBG_Pre;
	BOOL enUpdateWB;
	float originDif;
	BOOL enSaveWBFile;
	CString wbFileName;
	BYTE roiCalcMode;		
	CRect roi;		
	UINT factorAmplify;
	BOOL en8bit;
	BOOL enKeepOB;
	BYTE gainCalcMode;
} tagAwbOtpCtrl;

struct WBGainRegStruct
{
	USHORT addrGr;
	USHORT addrR;
	USHORT addrB;
	USHORT addrGb;

	USHORT addrGrHigh;
	USHORT addrGrLow;
	USHORT addrRHigh;
	USHORT addrRLow;
	USHORT addrBHigh;
	USHORT addrBLow;
	USHORT addrGbHigh;
	USHORT addrGbLow;
	USHORT dataDefaultGain;
} ;

struct tagAwbRatio
{
	UINT r;
	UINT g;
	UINT b;
	UINT gr;
	UINT gb;
	UINT rg;
	UINT bg;
	UINT gb_gr;
} ;

typedef struct
{
	BOOL enStartCurrent;
	BOOL enInf;
	BOOL enMiddle;
	BOOL enMacro;
	BYTE roiCalcMode;		
	CRect roi;		
	BOOL enSaveAFFile;
	CString afFileName;
	CString driverName;
} OtpAFControlStruct;

typedef struct
{
	int codeStart;
	int codeEnd;
	int codePerStep;
	UINT msStableTime;
	BYTE stableFrame;
} OtpAFRegionStruct;

typedef struct
{
	OtpAFRegionStruct region;
	BYTE algorithm;
	float upThres;
	BYTE upTimes;
	int codeMin;
	int offset;		
	int codeReset;
} OtpAFStartStruct;

typedef struct
{
	BYTE state;		
	int currentCode;
	float currentFV;
	float origFV;
	BYTE upCnt;				
	BYTE stableFrameCnt;				
} OtpAFStartStatusStruct;

typedef struct
{
	OtpAFRegionStruct region;		
	BYTE algorithm;
	float downThres;
	BYTE downTimes;
	float minFV;
	int codeReset;
} OtpAFScanStruct;

typedef struct
{
	BYTE state;		
	int currentCode;
	float currentFV;
	int peakCode;
	float peakFV;
	float lastFV;
	BYTE downCnt;				
	BYTE stableFrameCnt;				
} OtpAFScanStatusStruct;

typedef struct
{
	BYTE type;	
	BYTE roiCalcMode;
	CRect roi;		
	BYTE filterChannel;		
	UINT target;
	UINT lowLimit;
	UINT highLimit;
	BYTE effectFrame;
	BYTE stableFrame;
} ExpControlStruct;

typedef struct
{
	UINT mask;
	USHORT addrHigh;
	USHORT addrMid;
	USHORT addrLow;
} ExpRegisterStruct;

typedef struct
{
	UINT def;
	UINT max;
	UINT min;
} ExpRangeStruct;

typedef struct
{
	BOOL isFinished;
	UINT expCurrent;
	UINT filterValue;
	BYTE effectFrameCnt;
	BYTE stableCnt;
} ExpStatusStruct;

typedef struct
{
	int type;
} OtpYTLedStruct;

typedef struct
{
	BOOL enDatabase;
	BOOL enCheckLastWork;
} DatabaseControlStruct;

typedef struct
{
	USHORT Pdaf_FlagAddr;
	USHORT Proc1_FirstAddr;
	USHORT Proc2_FirstAddr;
	USHORT Proc1_SumAddr;
	USHORT Proc2_SumAddr;
	USHORT Pdaf_SumAddr;
	USHORT Proc1Size;
	USHORT Proc2Size;
	USHORT TotalSize;
}MTKPDAFStruct;

enum ROI_CALC_MODE
{
	ROI_CALC_CENTER = 0,
	ROI_CALC_1 = 1,    // width * height
	ROI_CALC_2 = 2,    // 1/2 width * 1/2 height
	ROI_CALC_4 = 4,    // 1/4 width * 1/4 height
	ROI_CALC_5 = 5,    // 1/5 width * 1/5 height
	ROI_CALC_8 = 8,    // 1/8 width * 1/8 height
	ROI_CALC_10 = 10,  // 1/10 width * 1/10 height
	ROI_CALC_10_8 = 11,
	ROI_CALC_12_10 = 12,
	ROI_CALC_USER,
};

enum SHADING_TEST_TYPE
{
	SHADING_TYPE_NORMAL = 0,
	SHADING_TYPE_SAMSUNG,
	SHADING_TYPE,
	SHADING_TYPE_OV,
	SHADING_TYPE_QUALCOMM,
};

typedef struct
{
	BYTE deviceID;
	BYTE addrByteSize;
	int pageSize;
	int msWriteTime;
} EepromStruct;

enum IMAGE_FORMAT_OTP_OTP
{
	IMAGE_FORMAT_OTP_RAW8 = 1,
	IMAGE_FORMAT_OTP_YUV,
	IMAGE_FORMAT_OTP_RAW16,
	IMAGE_FORMAT_OTP_RGB565,
	IMAGE_FORMAT_OTP_YUVSPI,
	IMAGE_FORMAT_OTP_RAW10,
	IMAGE_FORMAT_OTP_RAW12,
	IMAGE_FORMAT_OTP_GRAY,
	IMAGE_FORMAT_OTP_RGB24,
};

enum IMAGE_SEQUENCE_YUV
{
	IMAGE_SEQUENCE_YCbYCr = 0,
	IMAGE_SEQUENCE_YCrYCb,
	IMAGE_SEQUENCE_CbYCrY,
	IMAGE_SEQUENCE_CrYCbY,
};

enum IMAGE_SEQUENCE_RAW
{
	IMAGE_SEQUENCE_RGGB = 0,
	IMAGE_SEQUENCE_GRBG,
	IMAGE_SEQUENCE_GBRG,
	IMAGE_SEQUENCE_BGGR,
};

typedef struct _IMG_INFO{
	int		Width;
	int		Height;
	int		RoiSize[2];	//Width, Height
	int		RoiPos[4];	//StrX, StrY, EndX, EndY
	LPBYTE	bRawBuf;
	int		PxlOrder;
	//LSC inform
	int		LscCorRatio[4];		//LscR, LscGr, LscGb, LscB
	//Color ratio inform
	BOOL	RefModule;
}HI1332_IMG_INFO;

typedef struct _Hi1332CalibrationBuf
{
	WORD wColorRatio[8];
	float qColorRatio[6];
	DWORD dwLscParaGain[572];

}Hi1332CaliBuffer;

typedef struct _Hi1332PreLSCBuf
{
	USHORT bSramLscParaGain_R_0x5000[143];
	USHORT bSramLscParaGain_B_0x5000[143];
	USHORT bSramLscParaGain_Gr_0x6000[143];
	USHORT bSramLscParaGain_Gb_0x6000[143];


}Hi1332PreLSCBuffer;