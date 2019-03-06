#ifndef SHADING_MTK_H
#define SHADING_MTK_H

#pragma once

#include "Shading.h"

#define SHADING_MTK_BLOCK_NUM    13

typedef struct
{
	YRGBStruct avg[SHADING_MTK_BLOCK_NUM];
	float yRatio[SHADING_MTK_BLOCK_NUM];
	float gRatio[SHADING_MTK_BLOCK_NUM];
	float rg[SHADING_MTK_BLOCK_NUM];
	float bg[SHADING_MTK_BLOCK_NUM];
	float yMax;
	float yMin;
	float yDif;
	float gMax;
	float gMin;
	float rgMax;
	float bgMax;
} ShadingMTKStruct;

typedef struct
{
	CPoint block[SHADING_MTK_BLOCK_NUM];
	UINT width;
	UINT height;
	float yMax;
	float yMin;
	float yDif;
	float gMax;
	float gMin;
	float rgMax;
	float bgMax;
} ShadingMTKSetStruct;
	
class CShadingMTK : public CShading
{
public:
	CShadingMTK();
	~CShadingMTK();

	virtual void LoadOptions(void);
	virtual void SaveOptions(void);
	
	virtual void* GetValues(void);
	virtual void* GetParams(void);	

	virtual void ShowTestInfo(void);
	virtual BOOL SaveTestFile(void); 
	
protected:
	virtual void SetParams(UINT imageWidth, UINT imageHeight);
	virtual void CalcValues(BYTE *imageData, UINT imageWidth, UINT imageHeight, BYTE OB);
	virtual BYTE CheckPass(void);

private:
	ShadingMTKStruct m_values;
	ShadingMTKSetStruct m_params;
};

#endif  /* SHADING_MTK_H */


