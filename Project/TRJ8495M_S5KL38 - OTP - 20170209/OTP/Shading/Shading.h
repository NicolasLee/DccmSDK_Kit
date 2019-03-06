#ifndef SHADING_H
#define SHADING_H

#pragma once


enum SHADING_TEST_TYPE
{
	SHADING_TYPE_NORMAL = 0,
	SHADING_TYPE_SAMSUNG,
	SHADING_TYPE_MTK,
	SHADING_TYPE_OV,
	SHADING_TYPE_QUALCOMM,
};

enum SHADING_TEST_RESULT
{
	SHADING_RESULT_PASS = 0,
	SHADING_Y_ERROR = 0x01,
	SHADING_YDIF_ERROR = 0x02,
	SHADING_G_ERROR = 0x04,
	SHADING_RG_ERROR = 0x08,
	SHADING_BG_ERROR = 0x10,
	SHADING_COLORDIF_ERROR = 0x20,
};
	

class CShading
{
public:
	CShading();
	~CShading();

	virtual void Init(UINT imageWidth, UINT imageHeight, BYTE OB, HWND displayWnd, HWND logWnd);
	virtual BOOL Test(BYTE *imageData);

	BYTE GetResult(void);

	virtual void* GetValues(void);
	virtual void* GetParams(void);	

	virtual void ShowTestInfo(void);
	virtual BOOL SaveTestFile(void); 

protected:
	virtual void SetParams(UINT imageWidth, UINT imageHeight) =0;
	virtual void CalcValues(BYTE *imageData, UINT imageWidth, UINT imageHeight, BYTE OB) =0;
	virtual BYTE CheckPass(void) =0;

	BOOL IsOptionLoaded(void); 

private:
	UINT m_imageWidth; 
	UINT m_imageHeight;
	BYTE m_sensorOB;
	BYTE m_result;
	BOOL m_isOptionLoaded;
};

#endif  /* SHADING_H */


