#ifndef SHADING_QUALCOMM_H
#define SHADING_QUALCOMM_H

#pragma once

#include "Shading.h"

#define SHADING_QUALCOMM_BLOCK_NUM    5

typedef struct
{
	YRGBStruct avg[SHADING_QUALCOMM_BLOCK_NUM];
	float yRatio[SHADING_QUALCOMM_BLOCK_NUM];
	float rg[SHADING_QUALCOMM_BLOCK_NUM];
	float bg[SHADING_QUALCOMM_BLOCK_NUM];
	float yMax;
	float yMin;
	float yDif;
	float rgMax;
	float bgMax;

	float yRel[SHADING_QUALCOMM_BLOCK_NUM];
	float rgRel[SHADING_QUALCOMM_BLOCK_NUM];
	float bgRel[SHADING_QUALCOMM_BLOCK_NUM];
	float yRelMax;
	float rgRelMax;
	float bgRelMax;
} ShadingQualcommStruct;

typedef struct
{
	CPoint block[SHADING_QUALCOMM_BLOCK_NUM];
	UINT width;
	UINT height;
	float yMax;
	float yMin;
	float yDif;
	float rgMax;
	float bgMax;

	float yGolden[SHADING_QUALCOMM_BLOCK_NUM];
	float rgGolden[SHADING_QUALCOMM_BLOCK_NUM];
	float bgGolden[SHADING_QUALCOMM_BLOCK_NUM];
	float yRelMax;
	float rgRelMax;
	float bgRelMax;
} ShadingQualcommSetStruct;
	
class CShadingQualcomm : public CShading
{
public:
	CShadingQualcomm();
	~CShadingQualcomm();

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
	ShadingQualcommStruct m_values;
	ShadingQualcommSetStruct m_params;
};

#endif  /* SHADING_QUALCOMM_H */


