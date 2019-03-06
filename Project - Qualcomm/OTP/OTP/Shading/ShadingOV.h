#ifndef SHADING_OV_H
#define SHADING_OV_H

#pragma once

#include "Shading.h"

#define SHADING_OV_BLOCK_NUM    5

// typedef struct
// {
// 	YRGBStruct avg[SHADING_OV_BLOCK_NUM];
// 	float yRatio[SHADING_OV_BLOCK_NUM];
// 	float rg[SHADING_OV_BLOCK_NUM];
// 	float bg[SHADING_OV_BLOCK_NUM];
// 	float yMax;
// 	float yMin;
// 	float yDif;
// 	float rgMax;
// 	float bgMax;
// } ShadingOVStruct;
// 
// typedef struct
// {
// 	CPoint block[SHADING_OV_BLOCK_NUM];
// 	UINT width;
// 	UINT height;
// 	float yMax;
// 	float yMin;
// 	float yDif;
// 	float rgMax;
// 	float bgMax;
// } ShadingOVSetStruct;
////////////////////////////////////////////
//CaliBration  Standard:
typedef struct
{
	CPoint block_Y[SHADING_OV_BLOCK_NUM];
	CPoint block_Col[SHADING_OV_BLOCK_NUM];
	UINT  yWidth;
	UINT  yHeight;
	UINT  colWidth;
	UINT  colHeight;
	float yMax;
	float yMin;
	float yDif;
	float  rgMax;
	float  bgMax;
}ShadingOVSetStruct;
//////////////////////////////////////////////
//The Value of after Calibration 
typedef struct
{
	YRGBStruct avg_Y[SHADING_OV_BLOCK_NUM];
	YRGBStruct avg_Col[SHADING_OV_BLOCK_NUM];
	float yRatio[SHADING_OV_BLOCK_NUM];
	float rg[SHADING_OV_BLOCK_NUM];
	float bg[SHADING_OV_BLOCK_NUM];
	float yMax;
	float yMin;
	float yDif;
	float rgMax;
	float bgMax;
}ShadingOVStruct;
////////////////////////////////////////////////

class CShadingOV : public CShading
{
public:
	CShadingOV();
	~CShadingOV();

	virtual void LoadOptions(void);
	virtual void SaveOptions(void);

	virtual void* GetValues(void);
	virtual void* GetParams(void);	

	virtual void ShowTestInfo(void);
	virtual BOOL SaveTestFile(void); 

	void  SetLSC_ROI(UINT imageWidth, UINT imageHeight,BYTE RoiMode_Y,BYTE RoiMode_Col);

protected:
	virtual void SetParams(UINT imageWidth, UINT imageHeight);
	virtual void CalcValues(BYTE *imageData, UINT imageWidth, UINT imageHeight, BYTE OB);
	virtual BYTE CheckPass(void);

private:
	ShadingOVStruct m_values;
	ShadingOVSetStruct m_params;

};

#endif  /* SHADING_OV_H */


