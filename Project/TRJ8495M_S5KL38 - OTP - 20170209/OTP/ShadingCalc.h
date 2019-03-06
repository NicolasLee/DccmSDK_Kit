#pragma once

#include "DccmDef.h"
#include "DccmStruct.h"
#include "DccmBase.h"
#include "ImageProc.h"

class ShadingCalc
{
public:
	ShadingCalc(DccmControl_Interface* pInterface, const int roiCnt);
	~ShadingCalc();

	typedef struct tagShadingROI_Info
	{
		BOOL Enable;
		CRect pos;	// 实际分辨率
		int cx;		// ROI中心 x 坐标(百分比)
		int cy;		// ROI中心 y 坐标(百分比)
		int aveY;
		int aveR;
		int aveG;
		int aveB;

		double deltaR;
		double deltaG;
		double deltaB;
		double deltaY;

		double deltaRGdif;
		double deltaBGdif;

		int RG_result;
		int BG_result;
		int deltaY_result;
		int result;
	}ShadingROI_Info;

	// ROI
	ShadingROI_Info* pROI;	// 动态分配，以支持不同数量ROI
	int m_ROI_height;	// percentage of width
	int m_ROI_width;	// percentage of height
	// Standard
	double m_deltaY_max;
	double m_deltaY_min;
	double m_Ydif_limit;
	double m_Ydif_max;
	int	m_Ydif_result;

	double m_RGdif_limit;
	double m_BGdif_limit;
	double m_RGdif_max;
	double m_BGdif_max;
	double m_PPdif_max;

	double m_ydif;
	double m_ymax;
	double m_ymin;
	double m_rgdiff;
	double m_bgdiff;

	void CalcImageRect(void);
	BOOL ShadingTest(void);
	void ShowShadingTest(BOOL bResult);
	void InitMember();

	DccmControl_Interface* m_pInterface;
	int m_roiCnt;
	vector<double> m_vData;
};

