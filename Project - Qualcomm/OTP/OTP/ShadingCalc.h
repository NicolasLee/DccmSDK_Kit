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
		CRect pos;	// ʵ�ʷֱ���
		int cx;		// ROI���� x ����(�ٷֱ�)
		int cy;		// ROI���� y ����(�ٷֱ�)
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
	ShadingROI_Info* pROI;	// ��̬���䣬��֧�ֲ�ͬ����ROI
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
	double m_ymax2;
	double m_ymin2;

	int m_MTKLSCVerItem;

	BOOL m_shadingflag;

	void CalcImageRect(void);
	void MTKCalcImageRect(void);
	BOOL ShadingTest(void);
	BOOL MTKShadingTest(void);
	void ShowShadingTest(BOOL bResult);
	void InitMember();
	void ShowDisplayWnd(BYTE* pSrc, int width, int height);

	DccmControl_Interface* m_pInterface;
	int m_roiCnt;
	vector<double> m_vData;
};

