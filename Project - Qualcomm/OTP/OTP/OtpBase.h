#pragma once

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4005)
#include <intsafe.h>
#include <stdint.h>
#pragma warning (pop)
#endif

#include "OtpDef.h"
#include "DccmBase.h"
#include "ImageProc.h"
#include "ShadingCalc.h"

//QualPDAF
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "PDAFCalibrationTools_EEPROM.h"

 //#define  TestVersion //cnw
 //#define Raw8Data 不再使用

// base otp class of PlatfomrTool and inner Sensor
#define I2C_RETRY_NUM    4
#define I2C_RETRY_WAIT   1  
#define  msleep(p) Sleep(p*m_sleeptime); 

//QualPDAF
#define GAIN_MAP_MAX_SZ 256
#define DCC_MAP_MAX_SZ 144
#define MAX_PD_PIXELS_PER_BLOCK 128

#define GAIN_MAP_HEIGHT 13
#define GAIN_MAP_WIDTH 17
#define GAIN_MAP_Q_FMT 8

#define DCC_MAP_HEIGHT 6
#define DCC_MAP_WIDTH 8
#define DCC_MAP_Q_FMT 4

#define DCC_FIR {-1, -4, -6, -4, 0, 4, 6, 4, 1}
#define DCC_FIR_SZ 9

#define DCC_STACK_SZ_L3 8
#define DCC_STACK_SZ_L4 10
#define DCC_SEARCH_RANGE 48
#define DCC_FIT_EXCLUSION 1
#define DCC_MIN_PD_RANGE_L3 4
#define DCC_MIN_PD_RANGE_L4 1
#define DCC_MAP_OVERLAP_L3 0.2
#define DCC_MAP_OVERLAP_L4 0.5

#define DPD_PIXEL_UNIT 2
#define DPD_MIN_SENSOR_GAIN 1.9
#define DPD_MAX_SENSOR_GAIN 3.1

#define LINECHART 0
#define DIAMONDCHART 1

#define NORMALIZE 0
#define WEIGHTED 1

#define DCC_LINEARITY_LEVEL_THRESHOLD 0.1

#define LSCLEN 360

//////////////////////////////////////////////
// DCC VALIDATION TOLERANCE 
#define DCC_VAL_TOL_SPARSE \
  { 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, \
  0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, \
  0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, \
  0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, \
  0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, \
  0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20 }

#define DCC_VAL_TOL_2BY1 \
  { 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, \
  0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, \
  0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, \
  0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, \
  0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, \
  0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15 }

#define DCC_VAL_TOL_DPD \
  {	0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, \
  0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, \
  0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, \
  0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, \
  0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, \
  0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10 }
//////////////////////////////////////////////
typedef enum _cfa_pattern_t {
	UNSPECIFIED=0,
	RGGB,
	BGGR,
	GRBG,
	GBRG
} cfa_t; 

typedef struct _sensor_config_t
{
	cfa_t    cfa;
	uint16_t image_width;
	uint16_t image_height;
	uint16_t black_lvl;
	uint16_t bit_depth;
	uint16_t pd_block_width;
	uint16_t pd_block_height;
	uint16_t pd_block_start_x;
	uint16_t pd_block_start_y;
	uint16_t pd_block_n_pd_pairs;
	uint16_t pd_block_l_x[MAX_PD_PIXELS_PER_BLOCK];
	uint16_t pd_block_l_y[MAX_PD_PIXELS_PER_BLOCK];
	uint16_t pd_block_r_x[MAX_PD_PIXELS_PER_BLOCK];
	uint16_t pd_block_r_y[MAX_PD_PIXELS_PER_BLOCK];
	uint16_t fv_measure_channel; //1:left-top, 2:right-top, 3:left_bottom, 4:right_bottom
} sensor_config_t;


typedef struct _gainmap_limits_t
{
	uint16_t gain_max_limit;
	uint16_t pd_min_limit;
	uint16_t pd_max_limit;
} gainmap_limits_t;

typedef struct _gainmap_t
{
	int8_t cal_version;
	uint16_t width;
	uint16_t height;
	uint16_t gm_l[GAIN_MAP_MAX_SZ];
	uint16_t gm_r[GAIN_MAP_MAX_SZ];
} gainmap_t;

typedef struct _dccmap_t
{
	int8_t cal_version;
	uint16_t q_format;
	uint16_t width;
	uint16_t height;
	int16_t dm[DCC_MAP_MAX_SZ];
} dccmap_t;

typedef struct _pdmap_t
{
	uint16_t width;
	uint16_t height;
	float pd[DCC_MAP_MAX_SZ];
} pdmap_t;


typedef struct _dll_config_t_l3 {
	int8_t cal_version;
	int8_t chart_type;
	int8_t verify_type;
	int8_t gain_map_height;
	int8_t gain_map_width;
	int8_t gain_map_q_fmt;
	int8_t dcc_map_height;
	int8_t dcc_map_width;
	int8_t dcc_map_q_fmt;
	int8_t dcc_fir[31];
	int8_t dcc_fir_sz;
	int8_t dcc_stack_sz;
	int8_t dcc_search_range;
	int8_t dcc_fit_exclusion;
	int8_t dcc_min_pd_range;
	float  dcc_map_overlap;
	float  dcc_tolerance_table[48];
	int8_t dpd_pixel_unit;
	float  dpd_min_sensor_gain;
	float  dpd_max_sensor_gain;
} dll_config_t_l3;

typedef struct _dll_config_t_l4 {
	int8_t cal_version;
	int8_t chart_type;
	int8_t verify_type;
	int8_t gain_map_height;
	int8_t gain_map_width;
	int8_t gain_map_q_fmt;
	int8_t dcc_map_height;
	int8_t dcc_map_width;
	int8_t dcc_map_q_fmt;
	float dcc_fir[31];
	int8_t dcc_fir_sz;
	int8_t dcc_stack_sz;
	int8_t dcc_search_range;
	int8_t dcc_fit_exclusion;
	int8_t dcc_min_pd_range;
	float  dcc_map_overlap;
	float  dcc_tolerance_table[48];
	float  dcc_linearity_level_theshold;
	int8_t dpd_pixel_unit;
	float  dpd_min_sensor_gain;
	float  dpd_max_sensor_gain;
} dll_config_t_l4;

typedef struct _dll_config_t_l5 {
	int8_t cal_version;
	int8_t chart_type;
	int8_t verify_type;
	int8_t gain_map_height;
	int8_t gain_map_width;
	int8_t gain_map_q_fmt;
	int8_t dcc_map_height;
	int8_t dcc_map_width;
	int8_t dcc_map_q_fmt;
	float dcc_fir[31];
	int8_t dcc_fir_sz;
	int8_t dcc_stack_sz;
	int8_t dcc_search_range;
	int8_t dcc_fit_exclusion;
	int8_t dcc_min_pd_range;
	float  dcc_map_overlap;
	float  dcc_tolerance_table[48];
	float  dcc_linearity_level_theshold;
	int8_t dpd_pixel_unit;
	float  dpd_min_sensor_gain;
	float  dpd_max_sensor_gain;
} dll_config_t_l5;

typedef struct
{
	unsigned short addr;
	unsigned short value;
	unsigned short bits; //new
}REG;

typedef struct _sensor_R {
	int8_t cal_version;
	int8_t chart_type;
	int8_t verify_type;
	int8_t gain_map_height;
	int8_t gain_map_width;
	int8_t gain_map_q_fmt;
	int8_t dcc_map_height;
	int8_t dcc_map_width;
	int8_t dcc_map_q_fmt;
	float dcc_fir[31];
	int8_t dcc_fir_sz;
	int8_t dcc_stack_sz;
	int8_t dcc_search_range;
	int8_t dcc_fit_exclusion;
	int8_t dcc_min_pd_range;
	float  dcc_map_overlap;
	float  dcc_tolerance_table[48];
	float  dcc_linearity_level_theshold;
	int8_t dpd_pixel_unit;
	float  dpd_min_sensor_gain;
	float  dpd_max_sensor_gain;
} sensor_R;

typedef struct _sensor_W {
	int8_t cal_version;
	int8_t chart_type;
	int8_t verify_type;
	int8_t gain_map_height;
	int8_t gain_map_width;
	int8_t gain_map_q_fmt;
	int8_t dcc_map_height;
	int8_t dcc_map_width;
	int8_t dcc_map_q_fmt;
	float dcc_fir[31];
	int8_t dcc_fir_sz;
	int8_t dcc_stack_sz;
	int8_t dcc_search_range;
	int8_t dcc_fit_exclusion;
	int8_t dcc_min_pd_range;
	float  dcc_map_overlap;
	float  dcc_tolerance_table[48];
	float  dcc_linearity_level_theshold;
	int8_t dpd_pixel_unit;
	float  dpd_min_sensor_gain;
	float  dpd_max_sensor_gain;
} sensor_W;

typedef struct _ID_addr {
	int8_t cal_version;
	int8_t chart_type;
	int8_t verify_type;
	int8_t gain_map_height;
	int8_t gain_map_width;
	int8_t gain_map_q_fmt;
	int8_t dcc_map_height;
	int8_t dcc_map_width;
	int8_t dcc_map_q_fmt;
	float dcc_fir[31];
	int8_t dcc_fir_sz;
	int8_t dcc_stack_sz;
	int8_t dcc_search_range;
	int8_t dcc_fit_exclusion;
	int8_t dcc_min_pd_range;
	float  dcc_map_overlap;
	float  dcc_tolerance_table[48];
	float  dcc_linearity_level_theshold;
	int8_t dpd_pixel_unit;
	float  dpd_min_sensor_gain;
	float  dpd_max_sensor_gain;
} ID_addr;

typedef struct _AWB_addr {
	int8_t cal_version;
	int8_t chart_type;
	int8_t verify_type;
	int8_t gain_map_height;
	int8_t gain_map_width;
	int8_t gain_map_q_fmt;
	int8_t dcc_map_height;
	int8_t dcc_map_width;
	int8_t dcc_map_q_fmt;
	float dcc_fir[31];
	int8_t dcc_fir_sz;
	int8_t dcc_stack_sz;
	int8_t dcc_search_range;
	int8_t dcc_fit_exclusion;
	int8_t dcc_min_pd_range;
	float  dcc_map_overlap;
	float  dcc_tolerance_table[48];
	float  dcc_linearity_level_theshold;
	int8_t dpd_pixel_unit;
	float  dpd_min_sensor_gain;
	float  dpd_max_sensor_gain;
} AWB_addr;

typedef struct _LSC_addr {
	int8_t cal_version;
	int8_t chart_type;
	int8_t verify_type;
	int8_t gain_map_height;
	int8_t gain_map_width;
	int8_t gain_map_q_fmt;
	int8_t dcc_map_height;
	int8_t dcc_map_width;
	int8_t dcc_map_q_fmt;
	float dcc_fir[31];
	int8_t dcc_fir_sz;
	int8_t dcc_stack_sz;
	int8_t dcc_search_range;
	int8_t dcc_fit_exclusion;
	int8_t dcc_min_pd_range;
	float  dcc_map_overlap;
	float  dcc_tolerance_table[48];
	float  dcc_linearity_level_theshold;
	int8_t dpd_pixel_unit;
	float  dpd_min_sensor_gain;
	float  dpd_max_sensor_gain;
} LSC_addr;

typedef struct _AF_addr {
	int8_t cal_version;
	int8_t chart_type;
	int8_t verify_type;
	int8_t gain_map_height;
	int8_t gain_map_width;
	int8_t gain_map_q_fmt;
	int8_t dcc_map_height;
	int8_t dcc_map_width;
	int8_t dcc_map_q_fmt;
	float dcc_fir[31];
	int8_t dcc_fir_sz;
	int8_t dcc_stack_sz;
	int8_t dcc_search_range;
	int8_t dcc_fit_exclusion;
	int8_t dcc_min_pd_range;
	float  dcc_map_overlap;
	float  dcc_tolerance_table[48];
	float  dcc_linearity_level_theshold;
	int8_t dpd_pixel_unit;
	float  dpd_min_sensor_gain;
	float  dpd_max_sensor_gain;
} AF_addr;

typedef struct _PDAF1_addr {
	int8_t cal_version;
	int8_t chart_type;
	int8_t verify_type;
	int8_t gain_map_height;
	int8_t gain_map_width;
	int8_t gain_map_q_fmt;
	int8_t dcc_map_height;
	int8_t dcc_map_width;
	int8_t dcc_map_q_fmt;
	float dcc_fir[31];
	int8_t dcc_fir_sz;
	int8_t dcc_stack_sz;
	int8_t dcc_search_range;
	int8_t dcc_fit_exclusion;
	int8_t dcc_min_pd_range;
	float  dcc_map_overlap;
	float  dcc_tolerance_table[48];
	float  dcc_linearity_level_theshold;
	int8_t dpd_pixel_unit;
	float  dpd_min_sensor_gain;
	float  dpd_max_sensor_gain;
} PDAF1_addr;

typedef struct _OTP_General {	
	_sensor_R sensorR;
	_sensor_W sensorW;
	_ID_addr IDaddr;
	_AWB_addr AWBaddr;
	_LSC_addr LSCaddr;
	_AF_addr AFaddr;
	_PDAF1_addr PDAF1addr;
} OTP_General;



enum eProjectName	//项目名称 PN_
{
	PN_NULL = -1,
	PN_FHB6551M = 0 ,
	PN_PLQ8579M,
	PN_TRJ8564M,
	PN_HLTE1M,
	PN_HLT7017,
	PN_TYD8635M,
	PN_TYD8583M,
	PN_HLT7012,
	PN_HLT7014,
	PN_HLT7109,
	PN_HLT7025,
	PN_SAA3_0L2K,
	PN_CB801C,
};

enum PDAF_SIZE	
{
	//MTK
	SIZE_MTK_LSC = 1868,
	SIZE_MTK_PDAF_PROC1 = 496,
	SIZE_MTK_PDAF_PROC2 = 876,

	//Qual
	SIZE_QUAL_LSC = 1768,//1768
	SIZE_QUAL_COMPRESS_LSC = 1105,
	SIZE_QUAL_PDAF_PROC1 = 898,//3P3 1078, 3L8 1d 530, 2d 897，898
	SIZE_QUAL_PDAF_PROC2 = 4,//2

	SIZE_QUAL_L_PDAF_PROC1 = 890,
	SIZE_QUAL_L_PDAF_PROC2 = 102,//2
};

typedef enum
{
	PD_ERR_OK = 0,
	PD_ERR_FILE_NOT_FOUND, 
	PD_ERR_INIT_FAIL,
	PD_ERR_INVALID_PARAM,
	PD_ERR_OUTPUT_MEM_NOT_SUFFICIENT,
	PD_ERR_NOT_INITIALIZED,
	PD_ERR_INPUT_DATA_VERIFIED_FAIL,
	PD_ERR_OUTPUT_DATA_VERIFIED_FAIL,
	PD_ERR_QUALITY_VERIFIED_FAIL,
	PD_ERR_GENERAL_ERROR
} PD_ERR_CODE;

enum ZC533_Type
{
	ZC533_READ		 = 0, //0x00
	ZC533_WRITE	 = 1, //0x01
	ZC533_PAGE_ERASE= 2, //0x10
	ZC533_ALL_ERASE = 3, //0x11
};

enum ShadingItem
{
	SHADING_ITEM_5,
	SHADING_ITEM_14,
	SHADING_ITEM_49,
};

class COtpBase
{
public:
	COtpBase(void);
	~COtpBase(void);

	DccmControl_Interface* m_pInterface;
	CString m_szStr;


	vector<REG> m_vecLSCSram;
	vector<REG> m_vecLSCOTP;
	BYTE WriteLencData[LSCLEN];
	BYTE ReadLencData[LSCLEN];
	BYTE LscWriteChecksum;
	BYTE LscReadChecksum;
	BYTE LscChecksum[2]; //读出累加CheckSum

	void SetRoi(CRect &roi, UINT width, UINT height, BYTE roiCalcMode, UINT x=0, UINT y=0, UINT roiWidth=0, UINT roiHeight=0);
	void FormatRoiRect(CRect &rt, UINT width, UINT height);
	BOOL CheckEqual(BYTE *data1, BYTE *data2, int num);
public:	
	virtual BOOL InitProgram(void);
	virtual BOOL StartProgram(void);
	virtual BOOL EndProgram(void);
	///////////////////////////烧录流程/////////////////////////////////////////
	virtual BOOL LoadDll(void);
	virtual BOOL EraseAllData(void);
	virtual void SetWBDefaultGain(void);
	virtual BOOL InitOTP(void);

	BOOL GetLSC(void);
	virtual BOOL GetAWB(void) = 0 ;
	virtual BOOL CheckOTPAWB(void) = 0 ;
	virtual BOOL WriteOTP(void) = 0;
	virtual BOOL WriteOTP(vector<double>& vData) = 0;
	virtual void DisableLenc(void);
	virtual BOOL CheckOTP(void) = 0;
	virtual BOOL VerifyLSC(void) = 0;
	virtual BOOL VerifyPreWB(void) = 0;
	virtual BOOL VerifyAWB(void) = 0;	
    virtual BOOL FreeDll(void);

	////////////////////////////////AWB///////////////////////////////////////
	BOOL SaveAwbData(tagAwbRatio* wb, int type);//type 0 apply befor,1 apply after, 2 otp awb apply
	virtual BOOL InitAWB(void);
     virtual BOOL GetAWBInfo(tagAwbRatio* wb);
	void WriteWBReg(USHORT addrHigh, USHORT addrLow, USHORT gain);

	//Qual
	BOOL Qual_GetAWBData();
	BOOL Qual_GetAWBInfo(tagAwbRatio* wb);

	//Sensor
	void S5K3P3_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG);
	void S5K5E8_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG);
	void S5K3L8_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG);
	void S5K4H8_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG);
	void S5K4H7_ApplyAWB(tagAwbRatio wb, int GoldRG, int GoldBG);
	void OV13855_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG);
	void HI1332_ApplyAWB(tagAwbRatio wb,int GoldRG, int GoldBG);
	///////////////////////////////LSC/////////////////////////////////////////
	//MTK
    BOOL MTK_GetLSC(void);
// 	BOOL MTK_V16RGBGetLSC(void);
// 	BOOL MTK_V16MONOGetLSC(void);
	BOOL MTK_GetLSCData(void);
	BOOL MTK_ApplyLSC(void);
	BOOL MTK_SaveCorrectedRaw(void);

	//Qual
	BOOL Qual_GetLSC_P(void);
	BOOL Qual_GetLSC_R(void);
    BOOL ReadQualLSCDataFromTXT(float *R_Channel,float *Gr_Channel,float *Gb_channel,float *B_Channel);
	BOOL ReadQualOnlyLSCLSCDataFromTXT();
	BOOL ReadQualRverLSCDataFromTXT(float *R_Channel,float *Gr_Channel,float *Gb_channel,float *B_Channel);
	BOOL QualLSCDataCheck(BYTE *data);

	//Sensor LSC
	//S5K3L8
	BOOL S5K3L8_GetLSC(void);
	BOOL S5K3L8_ApplySRAM(void);

	//S5K5E8
	BOOL S5K5E8_GetLSC(void);
	BOOL S5K5E8_ApplySRAM(void);

	//S5K4H8
	BOOL S5K4H8_GetLSC(void);
	BOOL S5K4H8_ApplySRAM(void);

	//S5K4H7
	BOOL S5K4H7_GetLSC(void);
	BOOL S5K4H7_ApplySRAM(void);
	BOOL ParseLSCOTP(const char *pOtpData, vector<REG> &vecLSCOTP);
	BOOL ParseLSCSram(const char *pSramData);
	BOOL ParseRegisters(CString s, vector<REG> &vecReg);

	//HI1332
	BOOL HI1332_GetLSC(void);
	BOOL HI1332_ApplySRAM(Hi1332CaliBuffer &Hi1332LSCCaliBuffer);
	//////////////////////////////Basic///////////////////////////////////////

	///////////////////////////////AF////////////////////////////////////////
	//af
	virtual BOOL CheckAF();
	BOOL GetAFData(void);
	BOOL GetOTPAF(void);

	/////////////////////////////////PDAF////////////////////////////////////
	//MTK
	void PreProc1(void);
    int PDAF_Proc1(void);
	BOOL MTK_Ver201_GetProc1(void);
	BOOL MTK_Ver300_GetProc1(void);
	BOOL MTK_Ver301_GetProc1(void);
	void PDAFSaveProc1Bin(char* data, int size);
	void PDAFSaveBin(char* data, int size);

	//Qual
	void InitQualParam();
	void SaveQualParam();

	BOOL Qual_GainMapCal_J();
	int Qual_ReadSrcData(char* SrcRAWFilename, short *RAWDATA, int width, int height);
	int Qual_Display_GmCheckResult(int GainMapFlag);

	int32_t Qual_GainMapCal_L3();
	int write_gainmap_file(char *filename, gainmap_t *gain_map);
	void print_return_code_L3(int32_t rc);
	int read_raw_file(char *filename, uint16_t *img, int width, int height);
	void get_dll_cfg_L3(dll_config_t_l3 *dll_cfg );

	int32_t Qual_GainMapCal_L4();
	void get_dll_cfg_L4(dll_config_t_l4 *dll_cfg );
	void print_return_code_L4(int32_t rc);

	int32_t Qual_GainMapCal_L5();
	void get_dll_cfg_L5(dll_config_t_l5 *dll_cfg);
	void print_return_code_L5(int32_t rc);
	///////////////////////////////////Normal///////////////////////////////////////
	virtual void SaveCurrentRaw10(CString filename,USHORT *buff, int width, int height);
	virtual void SaveCurrentRaw8(CString filename,BYTE *buff, int width, int height);
	virtual void Raw10toRaw8(USHORT *Src, BYTE *Dest, long number);
	BOOL Raw8ToBMP(CString sfilename, BYTE *pBMPBuffer, UINT width, UINT height, UINT RawFormat);
	virtual CString GetModulePath();

	////////////////////////////////EEPROM////////////////////////////////
	BOOL GT24C64_All_Erase(int size);
	virtual BOOL GT24C64_Read(USHORT address,unsigned char *Data,int size);
	virtual BOOL GT24C64_Programming(USHORT address,unsigned char *Data,int size);

	virtual BOOL FM24C64_Read(USHORT address,unsigned char *Data,int size);
	virtual BOOL FM24C64_Programming(USHORT address,unsigned char *Data,int size);

	virtual BOOL ZC533_Page_Erase(BYTE page);
	virtual BOOL ZC533_All_Erase();
	virtual BOOL ZC533_Read(USHORT address,unsigned char *Data,int size);//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop /* = FALSE */
	virtual BOOL ZC533_Programming(USHORT address,unsigned char *Data,int size);//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page

	virtual BOOL DW9763_Page_Erase(BYTE page);
	virtual BOOL DW9763_All_Erase();
	virtual BOOL DW9763_Read(USHORT address,unsigned char *Data,int size);//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop /* = FALSE */
	virtual BOOL DW9763_Programming(USHORT address,unsigned char *Data,int size);//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page

	BOOL DW9767_Page_Erase(BYTE page);
	BOOL DW9767_All_Erase();
	BOOL DW9767_Read(USHORT address,unsigned char *Data,int size);//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page, BOOL bStop /* = FALSE */
	BOOL DW9767_Programming(USHORT address,unsigned char *Data,int size);//WORD reg, BYTE regSize, BYTE *pVal, WORD valSize, BYTE page
	//////////////////////////////////3L8/////////////////////////////////////
	void S5K3L8_ReadOTPPage(int page, BYTE *Buff);
	void S5K3L8_WriteOTPPage(int page, BYTE *Buff);
	void S5K3L8_SetDefaultAwb(void);
	void Sensor3L8_StreamOn(void);
	//////////////////////////////////5E8/////////////////////////////////////
	BOOL S5K5E8ReadOTPpage(int Page, USHORT address, BYTE *Data, int length);
	BOOL S5K5E8WriteOTPpage(int Page, USHORT address, BYTE *Data, int length);
	//////////////////////////////////4H8/////////////////////////////////////
	BOOL S5K4H8ReadOTPpage(int Page, USHORT address, BYTE *Data,int length);
	BOOL S5K4H8WriteOTPpage(int Page, USHORT address,BYTE *Data, int length);
	void S5K4H8_SetDefaultAwb(void);


	//////////////////////////////////4H7/////////////////////////////////////
	BOOL S5K4H7ReadOTPpage(int pageidx, BYTE *pdata);
	BOOL S5K4H7ReadOTPByte(UINT Page, USHORT startAddr, BYTE *data, int length);
	BOOL S5K4H7WriteMultiByte(UINT Page, USHORT startAddr, BYTE *data, int nstart, int length);
	BOOL S5K4H7WriteSingleByte(UINT Page, USHORT startAddr, BYTE data);
	void S5K4H7_SetDefaultAwb(void);

	///////////////////////////////OV13855///////////////////////////////////////////
	void OV13855_SetDefaultAwb(void);

	/////////////////////////////////////Hi1332/////////////////////////////////////
	void HI1332InitOTP();
	void HI1332ReleaseOTP();
	void HI1332SingleWrite(BYTE *pBuf, int addr_start,int addr_end);
	void HI1332ContinuousWrite(BYTE *pBuf, int addr_start, int addr_end);
	BOOL HI1332LargeWrite(BYTE *pBuf, int addr_start, int addr_end);
	void HI1332SingleRead(BYTE *pBuf, int addr_start, int addr_end);
	void HI1332ContinuousRead(BYTE *pBuf, int addr_start, int addr_end);
	void HI1332_SetDefaultAwb(void);

public: 

	virtual void PreSubclassOtp(void) = 0;
	virtual void SetOtpSensorParams(void) = 0;


public:	// 
	CString m_otpName;
	CString SetOtpName(CString otpName);
	CString GetOtpName(void);

	int SumCalc(BYTE *data, int num, int mod,int add);
	void SetSensorOB(BYTE OB);	
	BYTE GetSensorOB(void) const;

	static OtpSensorStruct m_otpSensor;
	static OtpPowerStruct m_otpPower;
	static tagOtpCtrl m_otpCtrl;
	static OtpSensorStruct* GetOtpSensorParams(void);
	static OtpPowerStruct* GetOtpPowerParams(void);
	static tagOtpCtrl* GetControlParams(void);

	static OtpIDStruct m_IDGroup;
	OtpIDStruct m_IDCurrent;
	static OtpIDStruct* GetIDGroup(void);	
	OtpIDStruct* GetIDCurrent(void);		

	BYTE m_sensorOB;
	static tagAwbOtpCtrl m_wbCtrl;
	static WBGainRegStruct m_wbGainReg;
	static tagAwbRatio m_wbGolden;
// tagAwbRatio m_wbCurrentGolden;
// tagAwbRatio m_wbCorrect;
	tagAwbRatio m_wbCurrent;

	static tagAwbOtpCtrl* GetWBControlParams(void);
	static WBGainRegStruct* GetWBGainRegister(void);					 
	static tagAwbRatio* GetWBGoldenValue(void);	
	tagAwbRatio* GetWBCurrentGoldenValue(void);					 
	tagAwbRatio* GetWBCurrentValue(void);		

	static LencOtpControlStruct m_lencCtrl;
//	static LencRegStruct m_lencReg;
	BYTE *m_lencData;
	static LencOtpControlStruct* GetLencControlParams(void);
//	static LencRegStruct* GetLencRegister(void);	

	static ExpControlStruct m_expCtrl;
	static ExpRegisterStruct m_expRegister;
	static ExpRangeStruct m_expRange;
	ExpStatusStruct m_expStatus;
	static MTKPDAFStruct m_mtkpdaf;
	static ExpControlStruct* GetExpControlParams(void); 
	static ExpRegisterStruct* GetExpRegister(void);					 
	static ExpRangeStruct* GetExpRangeParams(void); 
	static MTKPDAFStruct* GetMTKPDAFParams(void);
	EepromStruct m_Eeprom;
	void SetEepromParams(EepromStruct *eeprom);

	BOOL eeprom_write(USHORT addr, USHORT reg, BYTE val);
	BOOL eeprom_read(USHORT addr, USHORT reg, BYTE *pval);	
	BOOL eeprom_multi_write(USHORT addr, USHORT reg, BYTE *pval, USHORT length);
	BOOL eeprom_multi_read(USHORT addr, USHORT reg, BYTE *pval, USHORT length);

	BOOL eeprom_write(USHORT reg, BYTE val);
	BOOL eeprom_read(USHORT reg, BYTE *pval);	
	BOOL eeprom_multi_write(USHORT reg, BYTE *pval, USHORT length);
	BOOL eeprom_multi_read(USHORT reg, BYTE *pval, USHORT length);

	BOOL i2c_write(USHORT reg, USHORT val);
	BOOL i2c_read(USHORT reg, USHORT *pval);
	BOOL i2c_write_byte(USHORT reg, BYTE val);
	BOOL i2c_read_byte(USHORT reg, BYTE *pval);
	BOOL i2c_multi_write(USHORT reg, BYTE *pval, USHORT length);
	BOOL i2c_multi_read(USHORT reg, BYTE *pval, USHORT length, BOOL noStop=FALSE);
	BOOL i2c_write(USHORT addr, USHORT reg, USHORT val, BYTE mode);
	BOOL i2c_read(USHORT addr, USHORT reg, USHORT *pval, BYTE mode);
	BOOL i2c_multi_write(USHORT addr, USHORT reg, BYTE regSize, BYTE* pval, USHORT length);
	BOOL i2c_multi_read(USHORT addr, USHORT reg, BYTE regSize, BYTE* pval, USHORT length, BOOL noStop=FALSE);

	CString info;

	UINT m_goldenrg;
	UINT m_goldenbg;
	UINT m_goldengg;

	UINT m_awb_size;
	UINT m_rgainh;
	UINT m_rgainl;
	UINT m_bgainh;
	UINT m_bgainl;
	UINT m_goldenr;
	UINT m_goldenb;
	UINT m_goldengr;
	UINT m_goldengb;
	float m_rgaindif;
	float m_bgaindif;
	float m_orgdif;

	BOOL m_SaveAWB;

	UINT m_afread;
	UINT m_InfH;
	UINT m_InfL;
	UINT m_MacH;
	UINT m_MacL;
	UINT m_StcH;
	UINT m_StcL;
	UINT m_AFDif;
	USHORT m_infCode;
	USHORT m_macCode;
	USHORT m_stcCode;

	UINT m_sleeptime;

	UINT m_grouplimit;
	BOOL m_PDAFItem;
	BOOL m_EnBasic;
	BOOL m_EnQVL;
	BOOL m_EnLSC;
	int m_LSCItem;
	BOOL m_EnAWB;
	BOOL m_EnAF;
	BOOL m_EnPDAF;
	BOOL m_berase;
	BOOL m_bcheckinf;
	BOOL m_bcheckmac;
	BOOL m_bchecksta;
	BOOL m_dummyburn;

	BYTE *m_proc1data;
	BYTE *m_mtklscdata;
	long m_mtklscsum;
	BYTE *m_quallscdata;
	int Qual_LSC_R_Max;
	int Qual_LSC_Gr_Max;
	int Qual_LSC_Gb_Max;
	int Qual_LSC_B_Max;
	int Qual_LSC_R_Min;
	int Qual_LSC_Gr_Min;
	int Qual_LSC_Gb_Min;
	int Qual_LSC_B_Min;
	int Qual_LSCDistanceSpec;
	long m_quallscsum;
	long m_Rver_quallscsum;
	unsigned short* raw_seq;

	int m_MTKPDAFitem;
	int  m_QUALPDAFitem;
	CString FileFolderPath;
	CString m_MTK_INI_Name;
	CString m_Qual_INI_Name;
	int m_shadingitem;

	BOOL SensorLSCSta;
	int m_sensoritem;
	int m_LscTarget;
	UINT m_LscGroup;

	int SaveExp;
	int SavePDAFExp;
    USHORT SaveGain;

	CString m_MTKLSCBasePath;
	BOOL m_SaveMTKraw;
	int m_MTKLSCVerItem;
	CString m_QualLSCBasePath;
	int m_QUALLSCVerItem;
	int m_cfaitem;
	int m_quallscitem;
	BOOL m_qualawb;
	BYTE Qual_wb_r;
	BYTE Qual_wb_gr;
	BYTE Qual_wb_gb;
	BYTE Qual_wb_b;

	//Qual PDAF
	int MinPixelAvg;
	int MaxPixelAvg;
	int MinPDAvg;
	int MaxPDAvg;
	int MinGain;
	int MaxGain;
	int MaxPixelThresh;
	int GainDiff;
	int OTPFormat_Type; //0 --> Previous 1D, 1 ---> Previous 2D, 2 ---> Unitied Version 1D or 2 ---> Unitied Version 2D
	BYTE Qual_J_GainMapData[SIZE_QUAL_PDAF_PROC1]; //GainMapWidth*Height*2*2+6*2
	BYTE Qual_L3_GainMapData[SIZE_QUAL_L_PDAF_PROC1];
	BYTE Qual_L4_GainMapData[SIZE_QUAL_L_PDAF_PROC1];
	BYTE Qual_L5_GainMapData[SIZE_QUAL_L_PDAF_PROC1];

	int ZC533_Erase_lock;
	int ZC533_LastPage;
	BOOL m_sensorotpisempty;

	//PDAF L3
	BOOL LockJVer;
	int FrameCnt;
	int CaptureSize;
	int CaptureFrame;
	USHORT **RawArray;

	//4H8
	BYTE S5K4H8_WriteData[64];

	//Hi1332
	BYTE Hi1332_LSCData[867];

	//4H7
	BYTE S5K4H7_LSCData[360];

	//数据保存
	int Applyawbdata[7];
	CString m_OTPType;

	//通用烧录
	BOOL m_General;
	int m_Brunto;
	BYTE m_SlaveID;
	BOOL m_spage;
	int m_totalgroup;
};