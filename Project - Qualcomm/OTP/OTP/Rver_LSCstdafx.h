// Copyright (c) 2014 Qualcomm Technologies, Inc.  All Rights Reserved.
// Qualcomm Technologies Proprietary and Confidential.

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#define RGGB_PATTERN	0
#define GRBG_PATTERN	1
#define BGGR_PATTERN	2
#define GBRG_PATTERN	3

typedef struct CalibrationDataStruct
{
	unsigned short int * R_LSC;
	unsigned short int * Gr_LSC;
	unsigned short int * Gb_LSC;
	unsigned short int * B_LSC;
	unsigned short int * AWB;
} CalibrationDataStruct;

#ifdef __cplusplus
extern "C"
{
#if 0
int LensCorrectionLib(unsigned char * pImage, int nWidth, int nHeight,
    int CFA_pattern, int nPercentageR, int nPercentageGr,
    int nPercentageGb, int nPercentageB, unsigned char * pCalcParam, int * pLength);
#endif

_declspec(dllexport) int LensCorrectionLib(unsigned char * pImage, int nWidth, int nHeight,
    int CFA_pattern, short R_black_offset, short Gr_black_offset,
    short Gb_black_offset, short B_black_offset, bool bMode9x7, CalibrationDataStruct *pCalData);

}
#endif
// TODO: reference additional headers your program requires here
