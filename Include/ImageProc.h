#pragma once

#define  PI 3.14159265

#define	IMG_OUTFORMAT_RGGB  0
#define	IMG_OUTFORMAT_GRBG  1
#define	IMG_OUTFORMAT_GBRG  2 
#define	IMG_OUTFORMAT_BGGR  3

#define	IMG_OUTFORMAT_YCbYCr  0
#define	IMG_OUTFORMAT_YCrYCb  1
#define	IMG_OUTFORMAT_CbYCrY  2 
#define	IMG_OUTFORMAT_CrYCbY  3

#define CLIP(X)       (BYTE)(((X) > 255) ? 255 : (((X) < 0) ? 0 : (X)))

namespace ImageProc
{
	//压缩RGB24图像
	BOOL ZoomOutRGBImage(BYTE *pSrc, int width, int height, BYTE* pDst, int ratio, int flag = 0);

	//度信采集板出的压缩raw格式转化为标准RAW
	BOOL DTRaw10toRaw8(BYTE *pSrc, BYTE* pDst, int width, int height);
	BOOL DTRaw8toRaw(BYTE* pSrc, USHORT* pDst, int width, int height);
	BOOL DTRaw10toRaw(BYTE* pSrc, USHORT* pDst, int width, int height);
	BOOL DTRaw12toRaw(BYTE* pSrc, USHORT* pDst, int width, int height);

	// mipi raw: 4个pixel占5个byte
	// common raw: 4个pixel占8个byte
	BOOL RawToDtRaw10(USHORT* pSrc, BYTE* pDst, int width, int height);	// 20160509 mipi raw -> DtRaw10


	//得到图像Y亮度图
	BOOL RGB24toY(BYTE *pSrc, BYTE* pDst, int width, int height);
	//基于Y亮度得到某个区域的平均亮度
	int GetAveY_ROI(BYTE *pSrc, int width, int height, CRect rect);

	//计算灰度图像光学中心  ratio 高亮区域百分比%
	CPoint  GetOpticalCenter(BYTE *pSrc, int width, int height, int ratio = 7);
	//得到0-255直方图
	int* GetHistogram(BYTE *pSrc, int width, int height );
	int GetHistogramThreshold(BYTE *pSrc, int width, int height, int ratio);

	//MTF Truly、Dmegc
	double GetMTF(BYTE *pSrc, int width, int height, CRect rect);
	//ContrastAF
	double GetContrastAF(BYTE *pSrc, int width, int height, CRect rect);
	//FabsSharpness 
	double GetFabsSharpness(BYTE *pSrc, int width, int height, CRect rect);
	//SquSharpness
	double GetSquSharpness(BYTE *pSrc, int width, int height, CRect rect);
	//计算黑白比重的MTF
	double GetMTF_ratio(BYTE *pSrc, int width, int height, CRect rect, int ratio);
	//带迭代算法的MTF  Truly
	double GetMTF_Iteration(BYTE *pSrc, int width, int height, CRect rect);
	//MTF OFilm、Holitech
	double GetMTF_Holitech(BYTE *pSrc, int width, int height, CRect rect);
	//FV算法
	double GetResolution_FV(BYTE* pSrc, int width, int height, CRect rect);

	/*************************BAYER插值*************************************/

	//Raw8转RGB24  3X3 双线性插值 (bayer8)
	BOOL Raw8toRGB24(BYTE *pSrc, BYTE* pDst, int width, int height, BYTE outformat);
	BOOL Interpolation_BGGR(BYTE* pSrc, BYTE* pDst, int width, int height);
	BOOL Interpolation_RGGB(BYTE* pSrc, BYTE* pDst, int width, int height);
	BOOL Interpolation_GRBG(BYTE* pSrc, BYTE* pDst, int width, int height);

	//Raw转RGB24  3X3 双线性插值 (bayer16)
	BOOL RawtoRGB24(USHORT *pSrc, BYTE* pDst, int width, int height, BYTE outformat, BYTE Bits);
	BOOL Interpolation_BGGR(USHORT* pSrc, BYTE* pDst, int width, int height, BYTE Bits);
	BOOL Interpolation_RGGB(USHORT* pSrc, BYTE* pDst, int width, int height, BYTE Bits);
	BOOL Interpolation_GRBG(USHORT* pSrc, BYTE* pDst, int width, int height, BYTE Bits);
	BOOL Interpolation_GBRG(USHORT* pSrc, BYTE *pDst, int width, int height, BYTE Bits);

//度信的Raw转RGB24，add by zl 2013.11.29
//（ 注： 反汇编推演出度信插值公式：
//      在G通道算RGB时，G分量有做均值处理 G(x,y) != G(x,y) 而是 G(x,y) = (G(x,y) + G(x+1,y-1)/2)，其他同双线性插值
	BOOL RawtoRGB24_DT(USHORT *pSrc, BYTE* pDst, int width, int height, BYTE outformat, BYTE Bits);
	BOOL Interpolation_BGGR_DT(USHORT* pSrc, BYTE* pDst, int width, int height, BYTE Bits);
	BOOL Interpolation_RGGB_DT(USHORT* pSrc, BYTE* pDst, int width, int height, BYTE Bits);
	BOOL Interpolation_GRBG_DT(USHORT* pSrc, BYTE* pDst, int width, int height, BYTE Bits);
	BOOL Interpolation_GBRG_DT(USHORT* pSrc, BYTE *pDst, int width, int height, BYTE Bits);

	//RGB24转Raw 点对点复原
	BOOL RGB24toRaw(BYTE *pSrc, USHORT* pDst, int width, int height, BYTE outformat, BYTE Bits);
    BOOL RGB24toRaw_RGGB(BYTE* pSrc,USHORT* pDst, int width, int height, BYTE Bits);
	BOOL RGB24toRaw_GBRG(BYTE* pSrc,USHORT* pDst, int width, int height, BYTE Bits);
	BOOL RGB24toRaw_BGGR(BYTE* pSrc,USHORT* pDst, int width, int height, BYTE Bits);
	BOOL RGB24toRaw_GRBG(BYTE* pSrc,USHORT* pDst, int width, int height, BYTE Bits);

	//二值化图像
	BOOL  Binary(BYTE *pSrc, int width, int height, int Threshold);
	//二值化图像
	BOOL  Binary(int *pSrc, int width, int height, int Threshold);

	//得到某个通道的buffer
	USHORT* GetGr_Channel(USHORT* pSrc, int width, int height, int outformat);
	USHORT* GetGb_Channel(USHORT* pSrc, int width, int height, int outformat);
	USHORT* GetR_Channel(USHORT* pSrc, int width, int height, int outformat);
	USHORT* GetB_Channel(USHORT* pSrc, int width, int height, int outformat);

	//得到某个区域的标准差
	int GetStandardDeviation(BYTE* pSrc, int width, int height, CRect rect);

	//得到平均帧数据
	BOOL AverageFrameData(BYTE* pSrc, int width, int height, int framelevel);
	BOOL AverageFrameData(USHORT* pSrc, int width, int height, int framelevel);
	BOOL AverageFrameData_RGB24(BYTE* pSrc, int width, int height, int framelevel);
	
    //单通道平均值
	int GetAve_ROI(USHORT *pSrc, int width, int height, CRect rect);

	//得到某个通道的区域平均值
	int GetAveR_ROI_Raw(USHORT *pSrc, int width, int height, CRect rect, int outformat);
	int GetAveG_ROI_Raw(USHORT *pSrc, int width, int height, CRect rect, int outformat);
	int GetAveB_ROI_Raw(USHORT *pSrc, int width, int height, CRect rect, int outformat);
	int GetAveGb_ROI_Raw(USHORT *pSrc, int width, int height, CRect rect, int outformat);
	int GetAveGr_ROI_Raw(USHORT *pSrc, int width, int height, CRect rect, int outformat);

	//得到RGB某个区域的平均值
	int GetAveY_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect);
	int GetAveR_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect);
	int GetAveG_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect);
	int GetAveB_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect);

	// DL: Devil Lee
	BOOL YUV422toRGB24_DL(BYTE* pSrc, BYTE* pDst, int width, int height, int outformat);
	BOOL YUV422toRGB24_YCbYCr_DL(BYTE* pSrc, BYTE* pDst, int width, int height);
	BOOL YUV422toRGB24_YCrYCb_DL(BYTE* pSrc, BYTE* pDst, int width, int height);
	BOOL YUV422toRGB24_CbYCrY_DL(BYTE* pSrc, BYTE* pDst, int width, int height);
	BOOL YUV422toRGB24_CrYCbY_DL(BYTE* pSrc, BYTE* pDst, int width, int height);

	BOOL RGB24toYUV422_DL(BYTE* pSrc, BYTE* pDst, int width, int height, int outformat);
	BOOL RGB24_YCbYCrToYUV422_DL(BYTE* pSrc, BYTE* pDst, int width, int height);
	BOOL RGB24_YCrYCbToYUV422_DL(BYTE* pSrc, BYTE* pDst, int width, int height);
	BOOL RGB24_CbYCrYToYUV422_DL(BYTE* pSrc, BYTE* pDst, int width, int height);
	BOOL RGB24_CrYCbYToYUV422_DL(BYTE* pSrc, BYTE* pDst, int width, int height);

	BOOL YUV422toRGB24(BYTE* pSrc, BYTE* pDst, int width, int height, int outformat);
	BOOL YUV422toRGB24_YCbYCr(BYTE* pSrc, BYTE* pDst, int width, int height);
	BOOL YUV422toRGB24_YCrYCb(BYTE* pSrc, BYTE* pDst, int width, int height);
	BOOL YUV422toRGB24_CbYCrY(BYTE* pSrc, BYTE* pDst, int width, int height);
	BOOL YUV422toRGB24_CrYCbY(BYTE* pSrc, BYTE* pDst, int width, int height);

	BOOL YUV422toY(BYTE* pSrc, BYTE* pDst, int width, int height, int outformat);


	BOOL Sharpen(BYTE* pSrc, BYTE* pDst, int width, int height, int* Template); //定值锐化

	BOOL Smooth(BYTE* pSrc, BYTE* pDst, int width, int height, int* Template);//定值平滑

	BOOL Template3x3(BYTE* pSrc, BYTE* pDst, int width, int height, int* Template);

	//RGB转LAB
	BOOL RGB24ToLab(BYTE *pScr, double *pDst, int width, int height);//采用photoshop的算法
	double GetAveL_LabROI_RGB24(BYTE* pScr, int width, int height, CRect rect);//采用photoshop的算法
	double GetAvea_LabROI_RGB24(BYTE* pScr, int width, int height, CRect rect);//采用photoshop的算法
	double GetAveb_LabROI_RGB24(BYTE* pScr, int width, int height, CRect rect);//采用photoshop的算法
	BOOL RGBToLab_Pixel(BYTE R, BYTE G, BYTE B, double &lab_l, double &lab_a, double &lab_b);//采用photoshop的算法

	BOOL RGBToHSL(BYTE *pScr, float *pDst, int width, int height);
	BOOL HSLToRGB(float *pScr, BYTE *pDst, int width, int height);
	BOOL ChangeHSL(float hue, float saturation, float Luminosity, BYTE *pScr, int width, int height);

	int DPC_Raw(USHORT* pScr, int width, int height, int RawBit, int Format, int threshold);

	int GetWhiteAreaAveY(BYTE *pSrc, int width, int height, CRect rect, int ratio);
	int GetWhiteAreaAveG(USHORT *pSrc, int width, int height, int outformat, int RawBits, CRect rect, int ratio);

	// RG:Rolongo
	int RolongoRaw10toRaw(USHORT* pSrc, USHORT* pDst, int width, int height, BOOL bIgnore = FALSE);
};


#include <vector>
using namespace std;

/************************************ Search Algrithm ****************************************/
// SFR
// Search Mark Pos
namespace AlgMark
{
	typedef struct tagHotPixelStack
	{
		USHORT x;
		USHORT y;
		USHORT Retpt;  //模拟EIP,周边8个点的位置编号
		/*检测周边8个点
		1 2 3
		4 0 5
		6 7 8  */
	}HotPixelStack;
	
	// Point of double (x,y)
	typedef struct tagDPoint
	{
		double x;    //亮点在图像的横坐标 x
		double y;    //亮点在图像的横坐标 y

	}DPoint;

	// size and pos
	typedef struct tagBlockInfo
	{
		double x;		//亮点在图像的横坐标 x
		double y;		//亮点在图像的横坐标 y
		double totalNum;//亮点大小
		double Value;   //亮点的亮度
		CRect Rect;

		// 20170722
		double x_l;		// x left center cross edge
		double x_r;		// x right
		double y_t;		// y top
		double y_b;		// y bottom
	}BlockInfo;

	// 计算rect内的SFR值
	// 调用此函数需要有"ISOSFR.lib"库
	//cyclepixel 0~1 default=0.125
	//gamma (0.5,1)  default=1 
	double GetSFR(BYTE *pSrc, int width, int height, CRect t_rect, double cyclePixel = 0.125, double gamma = 0.5);

	// 目标区域有多个BlockInfo，进行搜索，返回多个BlockInfo特征
	BOOL GetTargetInfo(BYTE* pSrc, int Src_W, int Src_H, CRect ROI, BOOL bBlack, int SegmentThres, vector<BlockInfo>& vBlockInfo);

	// 目标区域有单个BlockInfo，进行搜索，返回单个BlockInfo特征
	BOOL GetTargetInfo(BYTE* pSrc, int Src_W, int Src_H, CRect ROI, BOOL bBlack, int SegmentThres, BlockInfo& blockInfo);


	template <typename T>
	bool tCalcBlockSizeAndPos(int x0, int y0, T* buffer, int width, int height, HotPixelStack* pHPS, BlockInfo& BI);

	template <typename T>
	void tGetBlockInfo(T* buffer, int width, int height, vector<BlockInfo>* pvBI);

	template <typename T>
	BOOL tSearchMaxObject(T* ROIbuf, int ROI_W, int ROI_H, int level, BlockInfo& Target);

	template <typename T>
	bool tCalcBlockCrossPos(T* buffer, int width, int height, BlockInfo& BI);

	template <typename T>
	void tGetBlockInfo(T* buffer, int width, int height, BlockInfo* pBI);

	template <typename T>
	BOOL tGetROIBuffer(T* pSrc, int SrcW, int SrcH, CRect ROI, T* Dst, int DstLen);
}
