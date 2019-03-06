#ifndef SHADING_8858R2A_H
#define SHADING_8858R2A_H

#pragma once

#include "Shading.h"

#define SHADING_8858R2A_BLOCK_NUM     10

typedef struct
{
	YRGBStruct avg[SHADING_8858R2A_BLOCK_NUM];
	float yRatio[SHADING_8858R2A_BLOCK_NUM];
	float rg[SHADING_8858R2A_BLOCK_NUM];
	float bg[SHADING_8858R2A_BLOCK_NUM];
	float yMax;
	float yMin;
	float yDif;
	float rgMax;
	float bgMax;
} Shading8858R2AStruct;

typedef struct
{
	CPoint block[SHADING_8858R2A_BLOCK_NUM];
	UINT width_Y;
	UINT height_Y;
	UINT width_C;
	UINT height_C;
	float yMax;
	float yMin;
	float yDif;
	float rgMax;
	float bgMax;
} Shading8858R2ASetStruct;

class C8858R2A_Shading : public CShading
{
public:
	C8858R2A_Shading();
	~C8858R2A_Shading();
	
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
	Shading8858R2AStruct m_values;
	Shading8858R2ASetStruct m_params;
};

#endif  /* SHADING_OV_H */
