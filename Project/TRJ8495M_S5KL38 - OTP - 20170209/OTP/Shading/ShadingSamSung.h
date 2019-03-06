#ifndef SHADING_SAMSUNG_H
#define SHADING_SAMSUNG_H

#pragma once

#include "Shading.h"

#define SHADING_SAMSUNG_BLOCK_NUM    49

typedef struct
{
	YRGBStruct avg[SHADING_SAMSUNG_BLOCK_NUM];
	float yRatio[SHADING_SAMSUNG_BLOCK_NUM];
	float rRatio[SHADING_SAMSUNG_BLOCK_NUM];
	float gRatio[SHADING_SAMSUNG_BLOCK_NUM];
	float bRatio[SHADING_SAMSUNG_BLOCK_NUM];
	float yMax;
	float yMin;
	float yDif;
	float rgMax;
	float bgMax;
	float colorDif;
} ShadingSamSungStruct;

typedef struct
{
	CPoint block[SHADING_SAMSUNG_BLOCK_NUM];
	UINT width;
	UINT height;
	float yMax;
	float yMin;
	float yDif;
	float rgMax;
	float bgMax;
	float colorDif;
} ShadingSamSungSetStruct;
	
class CShadingSamSung : public CShading
{
public:
	CShadingSamSung();
	~CShadingSamSung();

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
	ShadingSamSungStruct m_values;
	ShadingSamSungSetStruct m_params;
};

#endif  /* SHADING_SAMSUNG_H */


