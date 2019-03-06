#ifndef _PDAFCALIBRATIONTOOLS_REVJ_H_
#define _PDAFCALIBRATIONTOOLS_REVJ_H_

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4005)
#include <intsafe.h>
#include <stdint.h>
#pragma warning (pop)
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef PDAFCALIBRATIONTOOLS_REVJ_DLL_EXPORTS
	#define PDAFCALIBRATIONTOOLS_REVJ_DLL_EXPORTS_DECLSPEC    __declspec( dllexport )
#else
	#define PDAFCALIBRATIONTOOLS_REVJ_DLL_EXPORTS_DECLSPEC
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define SS3M2 0
#define SONYIMX258 1
#define SONYIMX258_BinningPDVer 2
#define OVT13853 3

#define OTPFORMAT1D 0
#define OTPFORMAT2D 1
#define OTPFORMATUnited_1D 2
#define OTPFORMATUnited_2D 3

#define GAINMAP1DFORMAT 1
#define GAINMAP2DFORMAT 2

#define MAXLENGTH 256
#define MINIMUM_LUT_LENGTH 10
#define GRIDPIXELS 256  // 512
#define MAXWIDTH 32  // =8K/256
#define MAXHEIGHT 32

#define MAXLENGTH2D MAXWIDTH*MAXHEIGHT
#define LEFTPDAFPIXEL 1
#define RIGHTPDAFPIXEL 2



typedef struct PDAFPatternParam
{
	int x_step;
	int y_step;
	int GlobalShift_x;
	int GlobalShift_y;
	int ImageBits;
	int BlackLevel;
	int SensorGain;
	int DSRatio;
	bool PDAFonG;
	char PDAFPatternFilename[1024];
}PDAFPattern;

typedef struct GainMap1DDataStructtag
{
	int VersionNum;
	unsigned short DSRatio;
	unsigned short ActualLength;
	unsigned short DSLength;
	unsigned short Left_GainMap[MAXLENGTH];
	unsigned short Right_GainMap[MAXLENGTH]; 
}GainMap1DDataStruct;

typedef struct GainMap2DDataStructtag
{
	int VersionNum;
	unsigned short OffsetX;
	unsigned short OffsetY;
	unsigned short RatioX;
	unsigned short RatioY;
	unsigned short MapWidth;
	unsigned short MapHeight;
	unsigned short Left_GainMap[MAXLENGTH2D];
	unsigned short Right_GainMap[MAXLENGTH2D]; 
}GainMap2DDataStruct;

typedef struct GainMapStructtag
{
	void *GainMapData;
	int Flag_1D_2D;
}GainMapStruct;


typedef struct CalibrationKOutputputParamsStructtag
{
	bool MultipleRegions;
	int PDConversionCoef[9];
	float FirstImage_PDShiftValue[9]; 
	float SecondImage_PDShiftValue[9]; 
	int FirstImage_CLValue[9]; 
	int SecondImage_CLValue[9]; 
}CalibrationKOutputParams;

typedef struct GainMapVerifyStructtag
{
	uint16_t MinPixelAvg;
	uint16_t MaxPixelAvg;
	uint16_t MinPDAvg;
	uint16_t MaxPDAvg;
	uint16_t MinGain;
	uint16_t MaxGain;
	uint16_t MaxPixelThresh;
	uint16_t GainDiff;
}GainMapVerifyStruct;

typedef struct VerificationStructtag
{
	float Verification_Lens_Position;
	int PD_ConversionCoeff;
	float Distance50cmTrueFocusPosition;
	float ToleranceError;
}VerificationInputParams;

typedef struct VerificationOutputtag
{
	float EstimationDAC;
	float Err;
	float PDShiftValue;
	int ConfidenceValue;
}VerificationOutputParams;


//PDAFCALIBRATIONTOOLS_REVJ_DLL_EXPORTS_DECLSPEC int ReadSrcData(char *SrcRAWFilename,short *RAWDATA, int nWidth, int nHeight, int Type);

PDAFCALIBRATIONTOOLS_REVJ_DLL_EXPORTS_DECLSPEC int PDAF_Cal_GainMap(short *RAWDATA,int nWidth, int nHeight, PDAFPattern PDAFPatternValue,
	GainMap2DDataStruct *GainMapData2D, GainMap1DDataStruct *GainMapData1D, GainMapVerifyStruct &gmvs);

PDAFCALIBRATIONTOOLS_REVJ_DLL_EXPORTS_DECLSPEC int PDAF_Cal_PDConversionCoef(short *FirstImg,short *SecondImg,int nWidth, int nHeight,float FirstImg_LensPos, float SecondImg_LensPos,
				GainMapStruct GainMap,PDAFPattern PDAFPatternValue, bool MultipleRegions,CalibrationKOutputParams &CalibrationKOutput);

PDAFCALIBRATIONTOOLS_REVJ_DLL_EXPORTS_DECLSPEC bool PDAF_Cal_Verification(short *Distance50cmLensPosition9_Img,int nWidth, int nHeight,GainMapStruct GainMapData, 
	PDAFPattern PDAFPatternValue,VerificationInputParams *VerificationValue,VerificationOutputParams &VerificationOutput);

PDAFCALIBRATIONTOOLS_REVJ_DLL_EXPORTS_DECLSPEC unsigned short GM_Constraint(GainMapVerifyStruct &gmvs, GainMap2DDataStruct *gm2d, GainMap1DDataStruct *gm1d);

PDAFCALIBRATIONTOOLS_REVJ_DLL_EXPORTS_DECLSPEC int OTP_single_defect_1DGainmap(GainMap1DDataStruct &gm1d, int GainDiff);
PDAFCALIBRATIONTOOLS_REVJ_DLL_EXPORTS_DECLSPEC int OTP_single_defect_2DGainmap(GainMap2DDataStruct &gm2d, int GainDiff);

PDAFCALIBRATIONTOOLS_REVJ_DLL_EXPORTS_DECLSPEC void PDAF_Cal_get_lib_version(char *s);
PDAFCALIBRATIONTOOLS_REVJ_DLL_EXPORTS_DECLSPEC void PDAF_Cal_get_lib_features(char *s);

#if defined(__cplusplus)
}
#endif

#endif

