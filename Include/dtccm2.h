/******************************************************************************
DTCCM Second Version
******************************************************************************/

#ifndef _DTCCM2_H_
#define _DTCCM2_H_

//#define _DTCALL_ _stdcall
#define _DTCALL_ 

#include "imagekit.h"

#ifdef DTCCM2_EXPORTS

	#define DTCCM_API extern "C"  __declspec(dllexport)

#else

	#define DTCCM_API extern "C"  __declspec(dllimport)

#endif

/*! \mainpage DOTHINKEY ImageKit API Manual
*
* \section Product Introduction
*
* - USB2.0 Series
* -# HS128
* -# HS280
* -# HS300
* -# HS300D
* -# HV810
* -# HV810D
* -# HV910
* -# HV910D
* - PCIE  Series
* -# PE300
* -# PE300D
* -# PE810
* -# PE810D
* -# PE910
* -# PE910D
* -# PE350
* -# PE950
* -# MP950
* - USB3.0 Series
* -# UT300
* -# UO300
* -# UV910
* -# UM330
* -# UM900
* -# UH910
* -# DTLC2
* -# UH920
* -# UF920
* -# MU950
* -# DMU956
* -# CMU958
* -# CMU959
* -# DMU927
* - wantrillion network fiber series
* - # CTG970
* \section company website
* http://www.dothinkey.com
* 
* 
*
* \section Document Release Version Records
* -# 2013/8/22 Generating DTCC2APIGuide;\n
*			   DTCCM2APIGuide Version:1.0.0.0;
*
* -# 2014/5/13 New increased ExtI2cWrite and ExtI2cRead function interfaces are used for external extended I2C to read and write;\n
*    		   DTCCM2APIGuide Version:1.0.2.2; 
*
* -# 2014/5/15 The chapter of EEPROM is classified  into the device information section;\n
*			   DTCCM2APIGuide Version:1.0.3.3;
* -# 2018/10/25 comment collation;\ n
*			    DTCCM2APIGuide version: 1.0.4.4;
*/

/* Defines the device ID that is opened by default */
#define DEFAULT_DEV_ID		0

	
/******************************************************************************
 Device Information	Related
******************************************************************************/
/** @defgroup group1  device information
@{

*/

/// @brief Get the version number of this class and the derived class can return its own version number
///
/// @param Version:Version number of this class
///
/// @retval DT_ERROR_OK:Get the version number of this class successfully 
DTCCM_API int _DTCALL_ GetLibVersion(DWORD Version[4],int iDevID=DEFAULT_DEV_ID);

/// @brief Get the version number of the DLL
///
/// @param Version:dll version
///
/// @retval DT_ERROR_OK:Get dll version successfully 
DTCCM_API int _DTCALL_ GetDllVersion(DWORD Version[4],int iDevID=DEFAULT_DEV_ID);

/// @brief Enumerate device, obtain device name and  devices number
///
/// @param DeviceName:Enumerated device name
/// @param iDeviceNumMax:Specifies the maximum number of enumeration devices
/// @param pDeviceNum:Enumerated devices number

///
/// @retval DT_ERROR_OK:Enumeration operation succeed
/// @retval DT_ERROR_FAILED:Enumeration operation failed
/// @retval DT_ERROR_INTERNAL_ERROR:Internal error
///
/// @note The obtained device name string requires the user program to call GlobalFree() to release one by one
DTCCM_API int _DTCALL_ EnumerateDevice(char *DeviceName[],int iDeviceNumMax,int *pDeviceNum);

/// @brief Get firmware version,iChip specifies which chip's firmware
///
/// @param iChip:Chip number
/// @param Version:Firmware version number
///
/// @retval DT_ERROR_OK:Succeed in obtaining the specified chip firmware version number   
/// @retval DT_ERROR_FAILED:Failed to get the specified chip firmware version number
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND:No specified chip firmware number
/// @retval DT_ERROR_COMM_ERROR:Communication error
/// @retval DT_ERROR_TIME_OUT:Get the version number over time
DTCCM_API int _DTCALL_ GetFwVersion(int iChip,DWORD Version[4],int iDevID=DEFAULT_DEV_ID);

/// @brief Gets the unique serial number of the device
///
/// @param pSN:Return device serial number
/// @param iBufferSize:Sets the length of the sequence number byte to be retrieved, up to 32 bytes supported
/// @param pRetLen:Return the length of actual device sequence number byte 
/// 
/// @retval DT_ERROR_OK:Succeed in obtaining the device's serial number 
/// @retval DT_ERROR_FAILED:Failed to get the device's serial number
/// @retval DT_ERROR_COMM_ERROR:Communication error
DTCCM_API int _DTCALL_ GetDeviceSN(BYTE *pSN,int iBufferSize,int *pRetLen,int iDevID=DEFAULT_DEV_ID);

/// @brief Get the hardware version of the device
///
/// @param Version:Hardware version number
///
/// @retval DT_ERROR_OK:Succeed in obtaining the hardware version of the device
/// @retval DT_ERROR_COMM_ERROR:Communication error
DTCCM_API int _DTCALL_ GetHardwareVersion(DWORD Version[4],int iDevID=DEFAULT_DEV_ID);

/// @brief Get the device's hardware information, a readable string 
///
/// @param pBuf:Device hardware information returned
/// @param iBufferSize:Set to get the length of the device hardware information bytes up to 16 bytes supported
/// @param pRetLen:Return the length of the device's serial number byte 
///						
/// @retval DT_ERROR_OK:Succeed in obtaining device hardware information
/// @retval DT_ERROR_FAILED:Failed to get device hardware information
/// @retval DT_ERROR_COMM_ERROR:Communication error 
DTCCM_API int _DTCALL_ GetHardwareInfo(BYTE* pBuf,int iBufferSize,int *pRetLen,int iDevID=DEFAULT_DEV_ID);

/// @brief Write user's defined name
///
/// @param pName:User's defined name
/// @param iSize:Set user's defined name byte length, maximum 32 bytes
///
/// @retval DT_ERROR_OK:Set the user's defined name successfully
/// @retval DT_ERROR_FAILED:Set user's defined name failed
/// @retval DT_ERROR_COMM_ERROR:Communication error 
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND:iSize parameter out of range
DTCCM_API int _DTCALL_ WriteUserDefinedName(char *pName, int iSize, int iDevID=DEFAULT_DEV_ID);

/// @brief Write user's defined name
///
/// @param pName:User's defined name
/// @param iSize:Set user's defined name byte length, maximum 32 bytes
/// @param bMaster:Set the current device to host for 1, and when MUD952 double test box, be careful not to set both devices to host or slave
///
/// @retval DT_ERROR_OK:Set the user's defined name successfully
/// @retval DT_ERROR_FAILED:Set user's defined name failed
/// @retval DT_ERROR_COMM_ERROR:Communication error 
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND:iSize parameter out of range
DTCCM_API int _DTCALL_ WriteUserDefinedNameEx(char *pName, int iSize ,BOOL bMaster, int iDevID=DEFAULT_DEV_ID);

/// @brief Read user's defined name
///
/// @param pName:Return  user's defined name
/// @param iBufferSize:Set user's defined name byte length, maximum 32 bytes
/// @param pRetLen:Return  user's defined name	byte length
///						 
/// @retval DT_ERROR_OK:Succeed in obtaining user's defined name
/// @retval DT_ERROR_FAILED:Failed to get user's defined name
/// @retval DT_ERROR_COMM_ERROR:Communication error  
DTCCM_API int _DTCALL_ ReadUserDefinedName(char *pName, int iBufferSize,int *pRetLen,int iDevID=DEFAULT_DEV_ID);

/// @brief Judge if the device is opened
///
/// @retval DT_ERROR_OK:The device has been connected to open
/// @retval DT_ERROR_FAILED:The device failed to be connected 
/// @retval DT_ERROR_COMM_ERROR:Communication error
DTCCM_API int _DTCALL_ IsDevConnect(int iDevID=DEFAULT_DEV_ID);

/// @brief Return device models, distinguish between different test boards.
///
/// @retval 0x0010:HS128  test board
/// @retval 0x0020:HS230  test board
/// @retval 0x0030:HS300  test board
/// @retval 0x0031:HS300D test board
/// @retval 0x0092:HV910  test board
/// @retval 0x0093:HV910D test board
/// @retval 0x0082:HV810  test board
/// @retval 0x0083:HV810D test board
///
/// @retval 0x0130:PE300  test board
/// @retval 0x0131:PE300D test board
/// @retval 0x0190:PE910  test board
///	@retval 0x0191:PE910D test board
/// @retval 0x0180:PE810  test board
///	@retval 0x0181:PE810D test board
/// @retval 0x0132:PE350  test board
/// @retval 0x0192:PE950  test board
/// @retval 0x0193:MP950  test board
///
///	@retval 0x0231:UT300  test board
/// @retval 0x0232:UO300  test board
/// @retval 0x0233: UM330  test board
///	@retval 0x0292:UV910  test board
///	@retval 0x0293:UH910  test board
///	@retval 0x02A1:DTLC2  test board
/// @retval 0x0295:UF920  test board
/// @retval 0x0295:UM900  test board
/// @retval 0x0296:MU950  test board
/// @retval 0x0297:DMU956 test board
/// @retval 0x0239:ULV330 test board
/// @retval 0x0298: CMU958 test board
/// @retval 0x0299:ULV913 test board
/// @retval 0x029a: ULV966 test board
/// @retval 0x029b: ULM928 test board
/// @retval 0x029e: CMU970 test board
///
///	@retval 0x0294:UH920  test board
///
/// the device name not written on the note does not have this function.
DTCCM_API DWORD _DTCALL_ GetKitType(int iDevID=DEFAULT_DEV_ID);

/// @brief restart the device. After this function is called, the device will be cut off and restarted (not all models are supported, but models developed after KM25/K15D/CMU970/CTG970 are supported)
DTCCM_API int _DTCALL_ SystemRestart(int iDevID=DEFAULT_DEV_ID);

/******************************************************************************
EEPROM Related
******************************************************************************/

/// @brief Read a word from an EEPROM
///
/// @param uAddr:Register address of EEPROM
/// @param pValue:Read words from an EEPROM
///
/// @retval DT_ERROR_OK:Read EEPROM successfully
/// @retval DT_ERROR_FAILD:Read EEPROM failed
/// @retval DT_ERROR_INTERNAL_ERROR:Internal error
/// @retval DT_ERROR_COMM_ERROR:Communication error
DTCCM_API int _DTCALL_ ReadWordFromEEProm(USHORT uAddr, USHORT *pValue,int iDevID=DEFAULT_DEV_ID);

/** @} */ // end of group1



/******************************************************************************
ISP Related
******************************************************************************/
/** @defgroup group2 ISP Related


* @{

*/

/// @brief Get the set value of GAMMA
/// 
/// @param pGamma:The set value of GAMMA returned 
DTCCM_API int _DTCALL_ GetGamma(int *pGamma,int iDevID=DEFAULT_DEV_ID);

/// @brief Set the value of GAMMA
///
/// @param iGamma:The set value of GAMMA
DTCCM_API int _DTCALL_ SetGamma(int iGamma,int iDevID=DEFAULT_DEV_ID);

/// @brief Get the set value of the contrast
///
/// @param pContrast:The set value of the contrast returned 
DTCCM_API int _DTCALL_ GetContrast(int *pContrast,int iDevID=DEFAULT_DEV_ID);

/// @brief Set the value of the contrast
///
/// @param iContrast:The set value of the contrast 
DTCCM_API int _DTCALL_ SetContrast(int iContrast,int iDevID=DEFAULT_DEV_ID);

/// @brief  Get the value of the saturation 
///
/// @param pSaturation:The set value of the saturation returned 
DTCCM_API int _DTCALL_ GetSaturation(int *pSaturation,int iDevID=DEFAULT_DEV_ID);

/// @brief Set the value of the saturation 
///
/// @param iSaturation:The set value of the saturation 
DTCCM_API int _DTCALL_ SetSaturation(int iSaturation,int iDevID=DEFAULT_DEV_ID);

/// @brief Set the value of the sharpness 
///
/// @param iSharpness:The set value of the sharpness 
DTCCM_API int _DTCALL_ SetSharpness(int iSharpness, int iDevID=DEFAULT_DEV_ID);

/// @brief Get the set value of the sharpness
///
/// @param pSharpness:The set value of the sharpness returned
DTCCM_API int _DTCALL_ GetSharpness(int *pSharpness, int iDevID=DEFAULT_DEV_ID);

/// @brief Set the  value of noise
///
/// @param iLevel:The set value of  noise 
DTCCM_API int _DTCALL_ SetNoiseReduction(int iLevel, int iDevID=DEFAULT_DEV_ID);

/// @brief Get the set value of  noise
/// 
/// @param pLevel:The set value of noise returned
DTCCM_API int _DTCALL_ GetNoiseReduction(int *pLevel, int iDevID=DEFAULT_DEV_ID);

/// @brief Turn on or turn off cleaning dead pixels
/// 
/// @param bDeadPixCleanEn:Turn on cleaning dead pixels for ture,turn off cleaning dead pixels for false
DTCCM_API int _DTCALL_ SetDeadPixelsClean(BOOL bDeadPixCleanEn,int iDevID=DEFAULT_DEV_ID);

/// @brief Get the state of cleaning dead pixels
/// 
/// @param pbDeadPixCleanEn:the state of cleaning dead pixels returned
DTCCM_API int _DTCALL_ GetDeadPixelsClean(BOOL *pbDeadPixCleanEn, int iDevID=DEFAULT_DEV_ID);

/// @brief Set the color point threshold
/// 
/// @param iHotCpth:Set color point threshold, the pixel value larger than the threshold is judged as color point
DTCCM_API int _DTCALL_ SetHotCpth(int iHotCpth, int iDevID=DEFAULT_DEV_ID);

/// @brief Get the color point threshold
/// 
/// @param pHotCpth:The set value of color point threshold returned
DTCCM_API int _DTCALL_ GetHotCpth(int *pHotCpth, int iDevID=DEFAULT_DEV_ID);

/// @brief Set the dark point threshold
/// 
/// @param iDeadCpth:Set the dark point threshold, and the pixel points less than the threshold are judged as dark points
DTCCM_API int _DTCALL_ SetDeadCpth(int iDeadCpth, int iDevID=DEFAULT_DEV_ID);

/// @brief Get the value of dark point threshold
/// 
/// @param pHotCpth:The set value of dark point threshold returned
DTCCM_API int _DTCALL_ GetDeadCpth(int *pDeadCpth, int iDevID=DEFAULT_DEV_ID);

/// @brief Set RawtoRGB algorithm.
///
/// @param Algrithm:The setting value of RawtoRGB algorithm,see Macro definition"RawtoRGB algorithm definition "
DTCCM_API int _DTCALL_ SetRaw2RgbAlgorithm(BYTE Algrithm, int iDevID=DEFAULT_DEV_ID);

/// @brief Get the setting value of RawtoRGB algorithm
///
/// @param The setting value of RawtoRGB algorithm returned
DTCCM_API int _DTCALL_ GetRaw2RgbAlgorithm(BYTE *pAlgrithm, int iDevID=DEFAULT_DEV_ID);

/// @brief Set the RGB digital gain
///
/// @param fRGain:R digital gain value
/// @param fGGain:G digital gain value
/// @param fBGain:B digital gain value
DTCCM_API int _DTCALL_ SetDigitalGain(float fRGain,float fGGain,float fBGain,int iDevID=DEFAULT_DEV_ID);

/// @brief Turn on or turn off the color matrix
/// 
/// @param bEnable:Turn on the color matrix	 for ture,turn off the color matrix	for false
DTCCM_API int _DTCALL_ SetMatrixEnable(BOOL bEnable, int iDevID=DEFAULT_DEV_ID);

/// @brief Get the RGB digital gain
/// 
/// @param pRGain:R digital gain value returned
/// @param pGGain:B digital gain value returned
/// @param pBGain:G digital gain value returned
DTCCM_API int _DTCALL_ GetDigitalGain(float *pRGain,float *pGGain,float *pBGain,int iDevID=DEFAULT_DEV_ID);

/// @brief Turn on or turn off white balance
///
/// @param bAWBEn:Turn on white balance	 for ture,turn off white balance for false
DTCCM_API int _DTCALL_ SetAWB(BOOL bAWBEn,int iDevID=DEFAULT_DEV_ID);

/// @brief Get the state of white balance 
///
/// @param pAwb:The state of white balance returned
DTCCM_API int _DTCALL_ GetAWB(BOOL *pAwb,int iDevID=DEFAULT_DEV_ID);

/// @brief Set the color matrix.
///
/// @param Matrix:Set the value of the color matrix.
DTCCM_API int _DTCALL_ SetMatrixData(float Matrix[3][3],int iDevID=DEFAULT_DEV_ID);

/// @brief Get the color matrix.
///
/// @param Matrix: The value of the color matrix returned
DTCCM_API int _DTCALL_ GetMatrixData(float Matrix[3][3],int iDevID=DEFAULT_DEV_ID);

/// @brief  Turn on or turn off MONO mode
///
/// @param bEnable:MONO mode enable, Turn on MONO mode for ture,turn off MONO mode for false
DTCCM_API int _DTCALL_ SetMonoMode(BOOL bEnable,int iDevID=DEFAULT_DEV_ID);

/// @brief Turn on or turn off the image horizontal reverse
///
/// @param bEnable:horizontal reverse enable, Turn on horizontal reverse for ture,turn off horizontal reverse for false
DTCCM_API int _DTCALL_ SetHFlip(BOOL bEnable,int iDevID=DEFAULT_DEV_ID);

/// @brief Turn on or turn off the image vertical reverse
///
/// @param bEnable:vertical reverse enable, Turn on vertical reverse for ture,turn off vertical reverse for false
DTCCM_API int _DTCALL_ SetVFlip(BOOL bEnable,int iDevID=DEFAULT_DEV_ID);

/// @brief Turn on or turn off "+" word line
///
/// @param bEnable:"+" word line enable,Turn on "+" word line for ture,turn off "+" word line for false
DTCCM_API int _DTCALL_ SetCrossOn(BOOL bEnable,int iDevID=DEFAULT_DEV_ID);

//RGB24 to YUV422 (YcbYcr mode)
//pBmp24 :RGB24 data pointer;
//pOut   :YUV422 pointer
//width  :Image width;
//height :Image height;

/// @brief RGB24toYUV422
///
/// @param pBmp24:RGB24 data
/// @param pOut:YUV422 data
/// @param width:Image data width
/// @param height:Image data height
DTCCM_API int _DTCALL_ RGB24ToYUV422(BYTE* pBmp24, BYTE* pOut, USHORT uWidth, USHORT uHeight, int iDevID=DEFAULT_DEV_ID);

//RGB24 to YUV444 (Ycbcr )
//pBmp24 :RGB24 data pointer;
//pOut   :YUV422 pointer
//width  :Image width;
//height :Image height;

/// @brief RGB24toYUV444
///
/// @param pBmp24:RGB24 data
/// @param pOut:YUV422 data
/// @param width:Image data width
/// @param height:Image data height
DTCCM_API int _DTCALL_ RGB24ToYUV444(BYTE *pBmp24, BYTE *pOut, USHORT uWidth, USHORT uHeight, int iDevID=DEFAULT_DEV_ID);


/// @brief  Display RGB image data
///
/// @param pBmp24:Data in RGB24 format to be displayed
/// @param pInfo:Frame information refers to structure "FrameInfo"
///
/// @retval DT_ERROR_OK: Display RGB images successfully
/// @retval DT_ERROR_FAILD:Display RGB image failed 
DTCCM_API int _DTCALL_ DisplayRGB24(BYTE *pBmp24,FrameInfo *pInfo=NULL,int iDevID=DEFAULT_DEV_ID);

/// @brief Display RGB image data
///
/// @param pBmp24:Data in RGB24 format to be displayed
/// @param pInfo:Frame information refers to structure "FrameInfoEx"
///
/// @retval DT_ERROR_OK:Display RGB images successfully
/// @retval DT_ERROR_FAILD:Display RGB image failed 
DTCCM_API int _DTCALL_ DisplayRGB24Ex(BYTE *pBmp24,FrameInfoEx *pInfo,int iDevID=DEFAULT_DEV_ID);

/// @brief RAW/YUVtoRGB The source data format is specified by ImgFormat
///
/// @param pIn:Source image data
/// @param pOut:Data converted to RGB24
/// @param uWidth:Image data width
/// @param uHeight:Image data height
/// @param byImgFormat:The format of source data
DTCCM_API int _DTCALL_ DataToRGB24(BYTE *pIn, BYTE *pOut, USHORT uWidth, USHORT uHeight, BYTE byImgFormat,int iDevID=DEFAULT_DEV_ID);


/// @brief RAW/YUVtoRGB The source data format is specified by ImgFormat.(Double channel product UH920 if is used should be specified the channel)
///
/// @param pIn:Source image data
/// @param pOut:Data converted to RGB24
/// @param uWidth:Image data width
/// @param uHeight:Image data height
/// @param byImgFormat:The format of source data
/// @param byChannel: Specify channels
DTCCM_API int _DTCALL_ DataToRGB24Ex(BYTE *pIn, BYTE *pOut, USHORT uWidth, USHORT uHeight, BYTE byImgFormat,BYTE byChannel,int iDevID=DEFAULT_DEV_ID);

/// @brief Display RAW/YUV image data, and the source data format is specified by the byImgFormat parameter in the Init function
///
/// @param pData:Image data to be displayed
/// @param pInfo:Frame information refers to structure "FrameInfo"
///
/// @retval DT_ERROR_OK:Displays the image successfully
/// @retval DT_ERROR_FAILD:Display image failed
/// @retval DT_ERROR_NOT_INITIALIZED:Not initialized
DTCCM_API int _DTCALL_ DisplayVideo(BYTE *pData,FrameInfo *pInfo=NULL,int iDevID=DEFAULT_DEV_ID);

/// @brief Display RAW/YUV image data, and the source data format is specified by the byImgFormat parameter in the Init function
///
/// @param pData:Image data to be displayed
/// @param pInfo:Frame information refers to structure "FrameInfoEx"
///
/// @retval DT_ERROR_OK:Displays the image successfully
/// @retval DT_ERROR_FAILD:Display image failed
/// @retval DT_ERROR_NOT_INITIALIZED:Not initialized
DTCCM_API int _DTCALL_ DisplayVideoEx(BYTE *pData,FrameInfoEx *pInfo,int iDevID=DEFAULT_DEV_ID);

/// @brief Transferring raw10 raw12, raw16 to RGB is supported, image processing on RAW image data(MONO,WB,ColorChange,Gamma,Contrast)
///
/// @param pImage:RAW image data 
/// @param pBmp24: Data after image processing
/// @param uWidth:Image data width
/// @param uHeight: Image data height
/// @param pInfo: Frame information refers tostructure "FrameInfo"
/// 
/// @retval DT_ERROR_OK:Image processing succeeded
/// @retval DT_ERROR_PARAMETER_INVALID:Parameter of pData  is invalid
/// @retval DT_ERROR_INTERNAL_ERROR:Internal error
DTCCM_API int _DTCALL_ ImageProcess(BYTE *pImage, BYTE *pBmp24, int nWidth, int nHeight,FrameInfo *pInfo,int iDevID=DEFAULT_DEV_ID);

/// @ brief support raw10 raw12, raw16, YUV data transfer RGB, the RAW image data on image processing (MONO, WB, ColorChange, Gamma, Contrast).
///
/// @param pImage:RAW image data 
/// @param pBmp24: Data after image processing
/// @param uWidth:Image data width
/// @param uHeight: Image data height
/// @param pInfo:Frame information refers to structure "FrameInfoEx"
/// 
/// @retval DT_ERROR_OK:Image processing succeeded
/// @retval DT_ERROR_PARAMETER_INVALID:Parameter of pData  is invalid
/// @retval DT_ERROR_INTERNAL_ERROR:Internal error
DTCCM_API int _DTCALL_ ImageProcessEx(BYTE *pImage, BYTE *pBmp24, int nWidth, int nHeight,FrameInfoEx *pInfo,int iDevID=DEFAULT_DEV_ID);

/// @brief Sets RAW format refers to enumeration type "RAW_FORMAT"
///
/// @param byRawMode:RAW format Settings
///
/// @see RAW_FORMAT
DTCCM_API int _DTCALL_ SetRawFormat(BYTE byRawMode,int iDevID=DEFAULT_DEV_ID);

/// @brief Sets YUV format refers to enumeration type "YUV_FORMAT"
///
/// @param byYuvMode:YUV format Settings
///
/// @see YUV_FORMAT
DTCCM_API int _DTCALL_ SetYUV422Format(BYTE byYuvMode,int iDevID=DEFAULT_DEV_ID);

/// @brief Collect image target format Settings, raw10, raw8, rgb24, etc
/// 
/// @param byTargetFormat:Collect image object formatting Setting
DTCCM_API int _DTCALL_ SetTargetFormat(BYTE byTargetFormat, int iDevID=DEFAULT_DEV_ID);

/// @brief Set some coefficients for YUV to RGB.
///
///	@param Y=(Y-yoff)*ycoef
///	@param r=Y+(cr-128)*bdif
///	@param g=Y-cbcoef*(cb-128)-crcoef*(cr-128)
///	@param b=Y+(cb-128)*rdif
DTCCM_API int _DTCALL_ SetYuvCoefficient(int yoff, 
										 float ycoef, 
										 float cbcoef, 
										 float crcoef, 
										 float rdif, 
										 float bdif,
										 int iDevID=DEFAULT_DEV_ID);

/// @brief Initialization supports 2 windows display. If you use 2 sensors, you need using hWndEx to specify the second window 
///
/// @param hWnd:Show A channel image window handle
/// @param uImgWidth: Image data width
/// @param uHeight: Image data height
/// @param byImgFormat:Image data format, such as:RAW/YUV
/// @param hWndEx:hWndEx:Show B channel image window handle
DTCCM_API int _DTCALL_ InitDisplay(HWND hWnd, 
								   USHORT uImgWidth, 
								   USHORT uImgHeight,
								   BYTE byImgFormat,
								   BYTE byChannel=CHANNEL_A,
								   HWND hWndEx=NULL,
								   int iDevID=DEFAULT_DEV_ID);
/// don't support
DTCCM_API int _DTCALL_ GrabDataToRaw8(BYTE *pIn, BYTE *pOut, int uWidth, int uHeight, BYTE ImgFormat, int iDevID=DEFAULT_DEV_ID);
/// don't support
DTCCM_API int _DTCALL_ GrabDataToRaw16(BYTE *pIn, USHORT *pOut, int uWidth, int uHeight, BYTE ImgFormat, int iDevID=DEFAULT_DEV_ID);

/// @brief Image processing on RAW image data(MONO,WB,ColorChange,Gamma,Contrast)
///
/// @param pImage:RAW image data
/// @param pBmp24:Data after image processing
/// @param uWidth:Image data width
/// @param uHeight: Image data height
/// @param pInfo:Frame information refers to structure "FrameInfo"
/// 
/// @retval DT_ERROR_OK:Image processing succeeded
/// @retval DT_ERROR_PARAMETER_INVALID:Parameter of pData is invalid
/// @retval DT_ERROR_INTERNAL_ERROR:Internal error
DTCCM_API int _DTCALL_ ImageProcessForRaw8(BYTE *pImage, BYTE *pBmp24, int nWidth, int nHeight, FrameInfo *pInfo, int iDevID=DEFAULT_DEV_ID);

/// @brief Transfer original image data format 
///
/// @param srcImg:Description of original image data structure, DtImage_t structure description refers to"ImageKit.h"
/// @param desImg:Description of target image data structure, DtImage_t structure description refers to"ImageKit.h"
/// @param roi[]:Set the coordinates and pixel values for the image area that needs to be transfered,DtRoi_t structure description refers to"ImageKit.h"
///
/// @retval DT_ERROR_PARAMETER_INVALID:Parameter is invalid, generally because srcImg or destImg parameter is not set correctly
/// @retval DT_ERROR_FUNCTION_INVALID:Transformation of this format is not supported
/// @retval DT_ERROR_OK:Transformation was 	successful
DTCCM_API int _DTCALL_ ImageTransform(DtImage_t *srcImg, DtImage_t *destImg, DtRoi_t roi[], int roiCount, int iDevID=DEFAULT_DEV_ID);

/** @} */ // end of group2


/******************************************************************************
SENSOR Related
*******************************************************************************/
/** @defgroup group3 SENSOR Related


* @{

*/
/// @brief Reset and Sensor communication of I2C bus.
///
/// @retval DT_ERROR_OK:Reset I2C operation successfully
/// @retval DT_ERROR_FAILED: Reset I2C operation failed
DTCCM_API int _DTCALL_ ResetSensorI2cBus(int iDevID=DEFAULT_DEV_ID);

/// @brief Sets the I2C byte interval
///
/// @brief uInterval:Set byte interval, unit us
DTCCM_API int _DTCALL_ SetI2CInterval(UINT uInterval, int iDevID=DEFAULT_DEV_ID);

/// @brief Set the I2C message ACK timeout wait time.
/// 
/// @param nUsWait:Set the ACK timeout wait time, unit us
///
/// @retval DT_ERROR_OK:Set I2C communication ACK timeout wait time successful
DTCCM_API int _DTCALL_ SetSensorI2cAckWait(UINT uAckWait,int iDevID=DEFAULT_DEV_ID);

/// @brief Set the I2C bus speed for communication with SENSOR, 400Kbps or 100Kbps.
///
/// @param b400K:b400K=TURE,400Kbps;b400K=FALSE,100Kbps
///							 
/// @retval DT_ERROR_OK:Set bus rate operation successful
DTCCM_API int _DTCALL_ SetSensorI2cRate(BOOL b400K,int iDevID=DEFAULT_DEV_ID);

/// @brief  Set the I2C bus speed for communication with SENSOR, range 10khz-2mhz.
///
/// @param uKpbs:Set I2C bus rate, range value is 10-2000
///							 
/// @retval DT_ERROR_OK:Set bus rate operation successful
DTCCM_API int _DTCALL_ SetSensorI2cRateEx(UINT uKpbs,int iDevID=DEFAULT_DEV_ID);

/// @brief Enables the I2C bus that can communicate with the SENSOR to be Rapid mode.
///
/// @param  bRapid=1 Indicates that the forced irrigation current output high level;=0, I2C pin is the input state, which is changed to high level by external pull-up
///							 
/// @retval DT_ERROR_OK:Set I2C bus Rapid mode successfully
DTCCM_API int _DTCALL_ SetSensorI2cRapid(BOOL bRapid,int iDevID=DEFAULT_DEV_ID);

/// @brief Select sensor i2c external pull-up resistor(CMU958PLUS/DMU927P/CMU959P supported)
/// 
/// @param byPullupResistorSel=PULLUP_RESISTOR_1_5K indicates1.5K;=PULLUP_RESISTOR_4_7K indicates 4.7K;=PULLUP_RESISTOR_0_88K indicates 0.88K
///
/// @retval DT_ERROR_OK:Set  successful
/// @retval DT_ERROR_PARAMETER_INVALID:byPullupResistorSel parameter is invalid
/// @retval DT_ERROR_TIME_OUT: Communication timeout
DTCCM_API int _DTCALL_ SetSensorI2cPullupResistor(BYTE byPullupResistorSel,int iDevID=DEFAULT_DEV_ID);

/// @brief ignores the ACK of the process written by sensor i2c, without an ACK response, and will continue to send the next command (only supported by UF920/UV910/PE950)
/// 
/// @param bIgnoreAck=TRUE,ignore ACK;=FALSE,indicate ACK is not ignored
///
/// @retval DT_ERROR_OK:set successful
/// @retval DT_ERROR_NOT_SUPPORTED: unsupported
/// @retval DT_ERROR_TIME_OUT:communication timeout
DTCCM_API int _DTCALL_ SetI2cIgnoreAck(BOOL bIgnoreAck,int iDevID=DEFAULT_DEV_ID);

/// @brief  not send stop commands for the process written by sensor i2c (only supported by PE series )
/// 
/// @param bNoStop=TRUE,not send stop;=FALSE,send stop
///
/// @retval DT_ERROR_OK: set successful
/// @retval DT_ERROR_NOT_SUPPORTED: unsupported
/// @retval DT_ERROR_TIME_OUT:communication timeout
DTCCM_API int _DTCALL_ SetSensorI2cWrNoStop(BOOL bNoStop,int iDevID=DEFAULT_DEV_ID);

/// @brief  write the SENSOR register. The setting value of I2C communication mode byI2cMode refers to the definition of I2CMODE.
/// 
/// @param uAddr:slave device address
/// @param uReg: register address
/// @param uValue:write the value of the register
/// @param byMode:I2C mode 
///
/// @retval DT_ERROR_OK:write the SENSOR register successfully
/// @retval DT_ERROR_COMM_ERROR:communication error
/// @retval DT_ERROR_PARAMETER_INVALID:byMode parameter is invalid
/// @retval DT_ERROR_TIME_OUT:communication timeout
/// @retval DT_ERROR_INTERNAL_ERROR:internal error
///
/// @see I2CMODE
DTCCM_API int _DTCALL_ WriteSensorReg(UCHAR uAddr, USHORT uReg, USHORT uValue, BYTE byMode,int iDevID=DEFAULT_DEV_ID);

/// @brief read the SENSOR register. The byI2cMode value of I2C communication mode refers to the definition of I2CMODE.
///
/// @param uAddr:slave device address
/// @param uReg:register address
/// @param pValue:read the value of the register
/// @param byMode: I2C mode
///
/// @retval DT_ERROR_OK:read the SENSOR register successfully
/// @retval DT_ERROR_COMM_ERROR:communication error
/// @retval DT_ERROR_PARAMETER_INVALID:byMode parameter is invalid
/// @retval DT_ERROR_TIME_OUT:communication timeout
/// @retval DT_ERROR_INTERNAL_ERROR:internal error
///
/// @see I2CMODE
DTCCM_API int _DTCALL_ ReadSensorReg(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode,int iDevID=DEFAULT_DEV_ID);

/// @brief writes the SENSOR register to write data to multiple registers (up to 16 registers).
///
/// @param uAddr:slave device address
/// @param byI2cMode:I2C mode
/// @param uRegNum:number of registers written, maximum 16
/// @param RegAddr[]:register address array
/// @param RegData[]:data written to the register
/// @param RegDelay[]:write the delay between one set of registers and the next set of registers, the maximum delay is 65ms, unit us
///
/// @retval DT_ERROR_OK:writes the SENSOR register successfully
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_INVALID:uSize parameter is invalid
/// @retval DT_ERROR_TIME_OUT:communication timeout
/// @retval DT_ERROR_INTERNAL_ERROR:internal error
DTCCM_API int _DTCALL_ WriteSensorMultiRegsWithDelay(UCHAR uAddr, BYTE byI2cMode, USHORT uRegNum, USHORT RegAddr[], USHORT RegData[],USHORT RegDelay[],int iDevID=DEFAULT_DEV_ID);

/// @brief write the SENSOR register and support writing a block of data (no more than 255 bytes) to a register.
///
/// @param uDevAddr:slave device address
/// @param uRegAddr:register address
/// @param uRegAddrSize:number of bytes of register address
/// @param pData: the data block written to the register
/// @param uSize:written to register the number of bytes of data blocks (no more than 255 bytes (HS300 / HS300D/HV910 / HV910D cannot exceed 253 bytes))
///
/// @retval DT_ERROR_OK:write the SENSOR register block successfully
/// @retval DT_ERROR_COMM_ERROR:communication error
/// @retval DT_ERROR_PARAMETER_INVALID:uSize parameter is invalid
/// @retval DT_ERROR_TIME_OUT:communication timeout
/// @retval DT_ERROR_INTERNAL_ERROR:internal error
DTCCM_API int _DTCALL_ WriteSensorI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID=DEFAULT_DEV_ID);

/// @brief read the SENSOR register and support reading a block of data (no more than 255 bytes) to a register.
///
/// @param uDevAddr:slave device address
/// @param uRegAddr:register address
/// @param uRegAddrSize:number of bytes of register address
/// @param pData:read the value of the register
/// @param uSize:number of bytes of data block of readout register (no more than 255 bytes)
/// @param bNoStop:whether to send an I2C STOP command, which is normally default to FALSE, bNoStop=TRUE means that there is no I2C STOP command for the writing process and bNoStop=FALSE has I2C STOP command
///
/// @retval DT_ERROR_OK:read the SENSOR register block successfully
/// @retval DT_ERROR_COMM_ERROR:communication error
/// @retval DT_ERROR_PARAMETER_INVALID:uSize parameter is invalid
/// @retval DT_ERROR_TIME_OUT:communication timeout
/// @retval DT_ERROR_INTERNAL_ERROR:internal error
DTCCM_API int _DTCALL_ ReadSensorI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop=FALSE,int iDevID=DEFAULT_DEV_ID);

/// @brief write the SENSOR register and support writing a block of data (no more than 255 bytes) to a register.
///
/// @param uDevAddr:slave device address
/// @param uRegAddr:register address
/// @param uRegAddrSize:number of bytes of register address
/// @param pData:the data block written to the register
/// @param uSize: written to register the number of bytes of data blocks (no more than 255 bytes (HS300 / HS300D/HV910 / HV910D cannot exceed 253 bytes))
///
/// @retval DT_ERROR_OK:write the SENSOR register block successfully
/// @retval DT_ERROR_COMM_ERROR:communication error
/// @retval DT_ERROR_PARAMETER_INVALID:uSize parameter is invalid
/// @retval DT_ERROR_TIME_OUT:communication timeout
/// @retval DT_ERROR_INTERNAL_ERROR:internal error
DTCCM_API int _DTCALL_ WriteSensorI2cEx(UCHAR uDevAddr,UINT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID=DEFAULT_DEV_ID);

/// @brief read the SENSOR register and support reading a block of data (no more than 255 bytes) to a register.
///
/// @param uDevAddr:slave device address
/// @param uRegAddr:register address
/// @param uRegAddrSize:number of bytes of register address
/// @param pData:read the value of the register
/// @param uSize:number of bytes of data block of readout register (no more than 255 bytes)
/// @param bNoStop:whether to issue an I2C STOP command, which is normally default to FALSE, bNoStop=TRUE means that there is no I2C STOP command for the writing process and bNoStop=FALSE has I2C STOP command
///
/// @retval DT_ERROR_OK:write the SENSOR register block successfully
/// @retval DT_ERROR_COMM_ERROR:communication error
/// @retval DT_ERROR_PARAMETER_INVALID:uSize parameter is invalid
/// @retval DT_ERROR_TIME_OUT:communication timeout
/// @retval DT_ERROR_INTERNAL_ERROR:internal error
DTCCM_API int _DTCALL_ ReadSensorI2cEx(UCHAR uDevAddr,UINT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop=FALSE,int iDevID=DEFAULT_DEV_ID);
/// @brief reads the SENSOR register
///
/// @param uDevAddr: from device address
/// @param pRegAddr: register address block
/// @param uRegAddrSize: number of bytes of register address (no more than 255 bytes)
/// @param pData: read the value of the register
/// @param uSize: number of bytes of data block of readout register (no more than 255 bytes)
/// @param bNoStop: whether to issue an I2C STOP command, which is normally default to FALSE, bNoStop=TRUE means that there is no I2C STOP command for the writing process, and bNoStop=FALSE has I2C STOP command
///
/// @retval DT_ERROR_OK: read the SENSOR register block successfully
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_INVALID: uSize parameter is invalid
/// @retval DT_ERROR_TIME_OUT: communication timeout
/// @retval DT_ERROR_INTERNAL_ERROR: internal error
/// @brief set the current value of the SENSOR, 100mA or 300mA.
DTCCM_API int _DTCALL_ ReadSensorI2cMultRegAndData(UCHAR uDevAddr,BYTE *pRegAddr,USHORT uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop=FALSE,int iDevID=DEFAULT_DEV_ID);

/// @param iLimit:current value (can only be set to 100 or 300)
/// 
/// @retval DT_ERROR_OK:set the current value successfully
/// @retval DT_ERROR_FAILED:set the current value failed
/// @retval DT_ERROR_INTERNAL_ERROR:internal error
///
/// @note this function is only valid for the set of HS and PE series
DTCCM_API int _DTCALL_ SetSensorCurrentLimit(int iLimit,int iDevID=DEFAULT_DEV_ID);

/// @brief  via Reset,PWDN pin opens or closes the SENSOR.
///
/// @param byPin:Reset, PWDN, PWDN2
/// @param bEnable: turn on or turn off the SENSOR
///
/// @retval DT_ERROR_OK:turn on or turn off the SENSOR	successfully
/// @retval DT_ERROR_FAILED:turn on or turn off the SENSOR	failed
/// @retval DT_ERROR_COMM_ERROR:communication error
DTCCM_API int _DTCALL_ SensorEnable(BYTE byPin, BOOL bEnable,int iDevID=DEFAULT_DEV_ID);

/// @brief  check Reset and  PWDN pin to see whether they are effective for SENSOR(function cannot be used)
///
/// @param pInfo:parameter information of SENSOR
/// @param bRstChkEn:enable check Reset
/// @param bPwdnChkEn:enable check PWDN
/// @param byChannel:A/B channel selection
/// @param pErrorx:Reset and PWDN check information returned
///
/// @retval DT_ERROR_OK:successful operation
DTCCM_API int _DTCALL_ CheckRstPwdnPin(SensorTab *pInfo, BOOL bRstChkEn, BOOL bPwdnChkEn, BYTE byChannel,BYTE *pErrorx,int iDevID=DEFAULT_DEV_ID);

/// @brief check Reset and  PWDN pin to see whether they are effective for SENSOR(function cannot be used)
///
/// @param pInfo:parameter information of SENSOR
/// @param bRstChkEn:enable check Reset
/// @param bPwdnChkEn:enable check PWDN
/// @param byChannel:A/B channel selection
/// @param pErrorx:Reset and PWDN check information returned
///
/// @retval DT_ERROR_OK:successful operation
DTCCM_API int _DTCALL_ CheckRstPwdnPin2(SensorTab2 *pInfo, BOOL bRstChkEn, BOOL bPwdnChkEn, BYTE byChannel,BYTE *pErrorx,int iDevID=DEFAULT_DEV_ID);


/// @brief setting the input clock of the SENSOR (6-100mhz can be set)
///
/// @param bOnOff:enables the SENSOR input clock, opens the input clock for TRUE and closes the input clock for FALSE
/// @param uHundKhz:input clock value of SENSOR, unit is 100Khz
///
/// @retval DT_ERROR_OK: set the SENSOR input clock successfully
/// @retval DT_ERROR_FAILED: set SENSOR input clock failed
/// @retval DT_ERROR_COMM_ERROR:communication error
DTCCM_API int _DTCALL_ SetSensorClock(BOOL bOnOff,USHORT uHundKhz,int iDevID=DEFAULT_DEV_ID);

/// @brief gets the synchronization clock frequency of MIPI interface.
///
/// @param pFreq: return the synchronous clock frequency value of the MIPI interface
/// @param byChannel: A channel /B channel
///
/// @retval DT_ERROR_OK: the synchronization clock frequency for the MIPI interface was successfully obtained
/// @retval DT_ERROR_FAILED: failed to obtain the synchronous clock frequency of MIPI interface
/// @retval DT_ERROR_PARAMETER_INVALID: channel parameter is invalid
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ GetMipiClkFrequency(ULONG *pFreq,BYTE byChannel=CHANNEL_A,int iDevID=DEFAULT_DEV_ID);

// @brief set the MIPI interface receiver clock phase.(function cannot be used)
//
/// @param byPhase: MIPI interface receiver clock phase (the value can be set to 0-7)
///
/// @retval DT_ERROR_OK: set MIPI interface receiver clock phase successfully
/// @retval DT_ERROR_FAILED: setting MIPI interface receiver clock phase failed
/// @retval DT_ERROR_TIME_OUT: sets timeout
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetMipiClkPhase(BYTE byPhase,int iDevID=DEFAULT_DEV_ID);

/// @brief Get the MIPI interface receiver clock phase value(function cannot be used)
///
/// @param pPhase: returns the MIPI interface receiver clock phase value
///
/// @retval DT_ERROR_OK: successfully acquired the MIPI interface receiver clock phase value
/// @retval DT_ERROR_FAILED: failed to obtain the MIPI interface receiver clock phase value
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ GetMipiClkPhase(BYTE *pPhase,int iDevID=DEFAULT_DEV_ID);

/// @brief Set up MIPI interface controller (KM25,K20C,K25D model supported).
///
/// @param dwCtrl: MIPI interface controller operation code, see macro definition "bit definition of MIPI controller features"
///
/// @retval DT_ERROR_OK: set MIPI interface controller successfully
/// @retval DT_ERROR_FAILED: setting MIPI interface controller failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetMipiCtrl(DWORD dwCtrl,int iDevID=DEFAULT_DEV_ID);

/// @brief gets the MIPI interface control code
///
/// @param pdwCtrl: return the MIPI interface control code
///
/// @retval DT_ERRORO_OK: succeed
DTCCM_API int _DTCALL_ GetMipiCtrl(DWORD *pdwCtrl,int iDevID=DEFAULT_DEV_ID);

/// @brief set MIPI pin enabling signal
///
/// @param bEnable: when is False, MIPI enters the HS state; when is TRUE, it enters the LP state
///
/// @retval DT_ERROR_OK: ROI set successful
/// @retval DT_ERROR_FAILD: ROI setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetMipiEnable(BOOL bEnable, int iDevID=DEFAULT_DEV_ID);

/// @brief sets the virtual channel number
///
/// @param uVC: sets the received image channel number, 0/1/2/3
/// @param bVCFileterEn: enables filtering of other virtual channels
///
/// @retval DT_ERROR_OK: set MIPI interface controller successfully
/// @retval DT_ERROR_FAILED: setting MIPI interface controller failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetMipiImageVC(UINT uVC, BOOL bVCFilterEn, BYTE byChannel=CHANNEL_A, int iDevID=DEFAULT_DEV_ID);

/// @brief filters MIPI packets, specifying the ID number of the packets to be filtered and the number of packets to be filtered
///
/// @param PackID[] : sets the ID number of the package to filter
/// @param iCount: number of filter package ids, DMU956 Max 64
///
/// @retval DT_ERROR_OK: ROI set successful
/// @retval DT_ERROR_FAILD: ROI setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetMipiPacketFilter(int PackID[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief gets the ID number of the filtered package
///
/// @param PackID[] : the ID number of the filtered package returned
/// @param pCount: returns the number of filter packets
/// @param MaxCount: sets the maximum number of package ids to be retrieved, and DMU956 maximum 64, that is, the size of the PackID array
///
/// @retval DT_ERROR_OK: ROI set successful
/// @retval DT_ERROR_FAILD: ROI setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ GetMipiPacketFilter(int PackID[], int *pCount, int MaxCount,int iDevID=DEFAULT_DEV_ID);

/// @brief gets the MIPI interface receiver CRC error count.
///
/// @param pCount: returns the CRC error count value of MIPI interface receiver
/// @param byChannel: A/B channel selection
///
/// @retval DT_ERROR_OK: successfully obtained the CRC error count value of MIPI interface receiver
/// @retval DT_ERROR_FAILED: failed to obtain CRC error value of MIPI interface receiver
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ GetMipiCrcErrorCount(UINT *pCount,BYTE byChannel=CHANNEL_A,int iDevID=DEFAULT_DEV_ID);

/// @brief gets the MIPI interface receiver ECC error count.
///
/// @param pCount: returns the MIPI interface receiver ECC error count value
/// @param byChannel: A/B channel selection
///
/// @retval DT_ERROR_OK: successfully got the MIPI interface receiver ECC error count value
/// @retval DT_ERROR_FAILED: failed to get the MIPI interface receiver ECC error meter value
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ GetMipiEccErrorCount(UINT *pCount,BYTE byChannel=CHANNEL_A,int iDevID=DEFAULT_DEV_ID);


/// @brief gets the LOCK state of the MIPI interface.
///
/// @param pMipiLockState: get the LOCK state of the MIPI interface, and represent LANE1, LANE2, LANE3 and LANE4 successively from bit0-bit3
///
/// @retval DT_ERROR_OK: the MIPI interface LOCK state was successfully obtained
/// @retval DT_ERROR_FAILED: failed to get the LOCK state
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ GetMipiLockState(DWORD *pMipiLockState,BYTE byChannel,int iDevID=DEFAULT_DEV_ID);

/// @brief sets up the MIPI interface controller extension interface.Currently only port and lane number configurations are supported, and other functions of the structure are recommended to call functions separately
///
/// @param pMipiCtrl: see MipiCtrlEx structure
DTCCM_API int _DTCALL_ SetMipiCtrlEx(MipiCtrlEx_t *pMipiCtrl,int iDevID=DEFAULT_DEV_ID);

/// @brief gets the parameter configuration value of the current MIPI interface controller.
///
/// @param pMipiCtrl: see MipiCtrlEx structure
DTCCM_API int _DTCALL_ GetMipiCtrlEx(MipiCtrlEx_t *pMipiCtrl,int iDevID=DEFAULT_DEV_ID);

/// @brief sets up the parallel interface receiving controller.
///
/// @param dwCtrl: operation code of parallel interface controller, see the macro definition "bit definition of synchronous parallel interface feature"
///
/// @retval DT_ERROR_OK: set parallel interface controller successfully
/// @retval DT_ERROR_FAILED: set parallel interface controller failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetParaCtrl(DWORD dwCtrl,int iDevID=DEFAULT_DEV_ID);

/// @brief gets the parallel interface receiving controller control code
///
/// @param pdwCtrl: returns the parallel interface receiving controller control code
///
/// @retval DT_ERRORO_OK: succeed
DTCCM_API int _DTCALL_ GetParaCtrl(DWORD *pdwCtrl,int iDevID=DEFAULT_DEV_ID);

/// @brief sets HiSPI interface controller.
///
/// @param dwCtrl: HiSPI interface controller operation code, see macro definition "bit definition of HiSPI interface feature"
///
/// @retval DT_ERROR_OK: set HiSPI interface controller successfully
/// @retval DT_ERROR_FAILED: setting HiSPI interface controller failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetHispiCtrl(DWORD dwCtrl,int iDevID=DEFAULT_DEV_ID);

/// @brief sets up the simulated image controller.
///
/// @param dwCtrl: simulated image controller operation code
/// @param byRefByte: analog image data definition
///
/// @retval DT_ERROR_OK: set the simulated image controller successfully
/// @retval DT_ERROR_FAILED: set the simulated image controller failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetSimCtrl(DWORD dwCtrl,BYTE byRefByte,int iDevID=DEFAULT_DEV_ID);

/// @brief initialization SENSOR.
///
/// @param uDevAddr: address of SENSOR device
/// @param pParaList: parameter list of SENSOR
/// @param uLength: size of pParaList
/// @param byI2cMode: access SENSOR's I2C mode, see enumeration type I2CMODE
///
/// @retval DT_ERROR_OK: initialization succeeded
/// @retval DT_ERROR_FAILED: initialization SENSOR failed
///
/// @ see I2CMODE
DTCCM_API int _DTCALL_ InitSensor(UCHAR uDevAddr,USHORT *pParaList,USHORT uLength,BYTE byI2cMode,int iDevID=DEFAULT_DEV_ID);

/// @brief initialization SENSOR.
///
/// @param uDevAddr: address of SENSOR device
/// @param pParaList: parameter list of SENSOR
/// @param uLength: size of pParaList
/// @param byI2cMode: access SENSOR's I2C mode, see enumeration type I2CMODE
///
/// @retval DT_ERROR_OK: initialization succeeded
/// @retval DT_ERROR_FAILED: initialization SENSOR failed
///
/// @ see I2CMODE
DTCCM_API int _DTCALL_ InitSensor2(UINT uDevAddr,UINT *pParaList,UINT uLength,UINT byI2cMode,int iDevID=DEFAULT_DEV_ID);

/// @brief check whether the inserted SENSOR is currently specified, and reset the SENSOR at the same time.
///
/// @param pInfo: SENSOR information, see SensorTab structure
/// @param byChannel: channel selection, A/B channel, see macro definition "multi-sensor module channel definition"
/// @param bReset: reset the SENSOR(this parameter is invalid)
///
/// @retval DT_ERROR_OK: find the SENSOR
/// @retval DT_ERROR_FAILED: no SENSOR was found
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SensorIsMe(SensorTab *pInfo,BYTE byChannel,BOOL bReset,int iDevID=DEFAULT_DEV_ID);

/// @brief check whether the inserted SENSOR is currently specified, and reset the SENSOR at the same time.
///
/// @param pInfo: SENSOR information, see SensorTab2 structure
/// @param byChannel: channel selection, A/B channel, see macro definition "multi-sensor module channel definition"
/// @param bReset: reset the SENSOR	(this parameter is invalid)
///
/// @retval DT_ERROR_OK: find the SENSOR
/// @retval DT_ERROR_FAILED: no SENSOR was found
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SensorIsMe2(SensorTab2 *pInfo,BYTE byChannel,BOOL bReset,int iDevID=DEFAULT_DEV_ID);

/// @brief channel selection(UH920 only)
///
/// @param byI2cChannel: channel selection, A/B channel, see macro definition "multi-sensor module channel definition"
///
/// @retval DT_ERROR_OK: set successful
/// @retval DT_ERROR_FAILED: setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetChannelSel(BYTE byI2cChannel,int iDevID=DEFAULT_DEV_ID);

/// @brief image channel enables
///
/// @param byI2cChannel: channel selection, A/B channel, see macro definition "multi-sensor module channel definition"
///
/// @retval DT_ERROR_OK: set successful
/// @retval DT_ERROR_FAILED: setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetGrabChannelEnable(BYTE byGrabChannelEnabel,int iDevID=DEFAULT_DEV_ID);

/// @brief sets up flexible interfaces.
///
/// @param PinConfig: flexible interface configuration definition
///
/// @retval DT_ERROR_OK: the flexible interface has been configured successfully
/// @retval DT_ERROR_FAILED: flexible interface configuration failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetSoftPin(BYTE PinConfig[26],int iDevID=DEFAULT_DEV_ID);

/// @brief whether the flexible interface enables pull-up resistance.
///
/// @param bPullup: flexible interface pull up enable, bPullup=TRUE pull up resistor, bPullup=FALSE close pull up resistor
///
/// @retval DT_ERROR_OK: set successful
/// @retval DT_ERROR_FAILED: setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetSoftPinPullUp(BOOL bPullup,int iDevID=DEFAULT_DEV_ID);

/// @brief setting whether the flexible interface is enabled or not; if it is not enabled, it is in a high-resistance state.
///
/// @param bEnable: flexible interface enable
///
/// @retval DT_ERROR_OK: set successful
/// @retval DT_ERROR_FAILED: setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ EnableSoftPin(BOOL bEnable,int iDevID=DEFAULT_DEV_ID);

/// @brief enables GPIO.
///
/// @param bEnable: enables GPIO
///
/// @retval DT_ERROR_OK: set successful
/// @retval DT_ERROR_FAILED: setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ EnableGpio(BOOL bEnable,int iDevID=DEFAULT_DEV_ID);

/// @brief obtain the flexible interface level, one pin per BIT, and bit0 corresponds to PIN_1.
///
/// @param pPinLevel: flexible interface level
///
/// @retval DT_ERROR_OK: successfully acquired flexible interface level
/// @retval DT_ERROR_FAILED: failed to obtain flexible interface level
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ GetSoftPinLevel(DWORD *pPinLevel,int iDevID=DEFAULT_DEV_ID);

/// @brief sets the level of a single GPIO, the number value of iPin starts from 1, and 1 represents GPIO1.
///
/// @param iPin: GPIO number value, the number value of iPin starts from 1, and 1 represents GPIO1
/// @param bLevel: sets the level of GPIO
///
/// @retval DT_ERROR_OK: set the level value of a single GPIO successfully
/// @retval DT_ERROR_FAILED: setting the level value of a single GPIO failed
/// @retval DT_ERROR_PARAMETER_INVALID: GPIO number value parameter is invalid
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetGpioPinLevel(int iPin,BOOL bLevel,int iDevID=DEFAULT_DEV_ID);

/// @brief to obtain the level of a GPIO; if it is input, it is external level;If it is output, set the level. 
///
/// @param iPin: GPIO number value, the number value of iPin starts from 1, and 1 represents GPIO1
/// @param pLevel: level of GPIO
///
/// @retval DT_ERROR_OK: successfully obtained the level value of GPIO
/// @retval DT_ERROR_FAILED: failed to obtain the level value of GPIO
/// @retval DT_ERROR_PARAMETER_INVALID: GPIO number value parameter is invalid
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ GetGpioPinLevel(int iPin,BOOL *pLevel,int iDevID=DEFAULT_DEV_ID);

/// @brief sets the level of multiple GPIO. The number value stored in the Pin starts from 1, and 1 represents GPIO1.
///
/// @param Pin: GPIO number
/// @param Level: GPIO Level
/// @param iCount: number of GPIO to set
///
/// @retval DT_ERROR_OK: set multiple GPIO levels successfully
/// @retval DT_ERROR_FAILED: setting multiple GPIO levels failed
/// @retval DT_ERROR_PARAMETER_INVALID: GPIO number value parameter is invalid
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetMultiGpioPinLevel(int Pin[],BOOL Level[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief sets the IO pin direction of a GPIO, bDir: TRUE for input and FALSE for output.
///
/// @param iPin: GPIO number
/// @param bDir: GPIO pin direction,bDir: TRUE for input, FALSE for output
///
/// @retval DT_ERROR_OK: set the IO pin direction of GPIO to succeed
/// @retval DT_ERROR_FAILED: setting the IO pin direction of GPIO failed
/// @retval DT_ERROR_PARAMETER_INVALID: GPIO number value parameter is invalid
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetGpioPinDir(int iPin, BOOL bDir,int iDevID=DEFAULT_DEV_ID);

/// @brief sets the direction of IO pin of multiple GPIO, Dir: TRUE for input and FALSE for output.
///
/// @param Pin: GPIO number
/// @param Dir: TRUE for input and FALSE for output
/// @param iCount: number of GPIO to set
///
/// @retval DT_ERROR_OK: set the IO pin direction of multiple GPIO to succeed
/// @retval DT_ERROR_FAILED: setting the IO pin direction of multiple GPIO failed
/// @retval DT_ERROR_PARAMETER_INVALID: GPIO number value parameter is invalid
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetMultiGpioPinDir(int Pin[], BOOL Dir[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief to obtain the level of a GPIO; if it is input, it is external level;If it is output, set the level.
///
/// @param Pin: GPIO Pin number
/// @param Level: GPIO Level
/// @param iCount: number of GPIO to set
///
/// @retval DT_ERROR_OK: successful fetch of IO pin direction of a GPIO
/// @retval DT_ERROR_FAILED: failed to obtain the IO pin direction of a GPIO
/// @retval DT_ERROR_PARAMETER_INVALID: GPIO number value parameter is invalid
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ GetMultiGpioPinLevel(int Pin[],BOOL Level[],int iCount,int iDevID=DEFAULT_DEV_ID);
/****************************************************************************************
SPI Bus
*****************************************************************************************/

/// @brief uPort:SPI controller selection
///
/// @brief pSPIConfig:SPI configuration structure, see imagekit.h
///
/// @retval DT_ERROR_OK: SPI configured successfully
/// @retval DT_ERROR_FAILD: SPI configuration failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ MasterSpiConfig(UCHAR uPort,MasterSpiConfig_t *pSPIConfig,int iDevID = DEFAULT_DEV_ID);

/// @brief SPI writes the SENSOR register, supports writing a data block (no more than 255 bytes) to a register, and supports 4K after MU950 updates the firmware.
///
/// @param uDevAddr: from device address, if no device address, set to 0
/// @param uRegAddr: register address, register address supports up to 4 bytes
/// @param uRegAddrSize: number of bytes of register address
/// @param pData: the data block written to the register
/// @param uSize: number of bytes of data blocks written to registers
///
/// @retval DT_ERROR_OK: writes the SENSOR register block successfully
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_INVALID: uSize parameter is invalid
/// @retval DT_ERROR_TIME_OUT: communication timeout
/// @retval DT_ERROR_INTERNAL_ERROR: internal error
DTCCM_API int _DTCALL_ WriteSensorSPI(UCHAR uDevAddr,UINT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID=DEFAULT_DEV_ID);

/// @brief SPI controller enables, speeds, and mode Settings. Note: SPI bus must be enabled before operating SensorSpiRW, and SPI bus must be closed if not operated, so as not to affect the access of SENSOR i2c (only UT300/UV910/UF920 is supported).
///
/// @param uDevAddr: from device address, if no device address, set to 0
/// @param uRegAddr: register address, register address supports up to 4 bytes
/// @param uRegAddrSize: number of bytes of register address
/// @param pData: read the value of the register
/// @param uSize: number of bytes of data block of readout register (no more than 255 bytes)
///
/// @retval DT_ERROR_OK: read the SENSOR register block successfully
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_INVALID: uSize parameter is invalid
/// @retval DT_ERROR_TIME_OUT: communication timeout
/// @retval DT_ERROR_INTERNAL_ERROR: internal error
DTCCM_API int _DTCALL_ ReadSensorSPI(UCHAR uDevAddr,UINT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID=DEFAULT_DEV_ID);

/// @brief SPI controller enables, speeds, and mode Settings (UT300/UV910). Note: SPI bus must be enabled before operating SensorSpiRW; if not, SPI bus must be closed to avoid affecting SENSOR i2c access.
///
/// @param bEnable: enable SPI bus for True and close SPI bus for False
/// @param iRate: True is 500Kbps and False is 250Kbps
/// @param bType: select three-line or four-line communication, False is three-line SPI communication, and True is four-line SPI communication
///
/// @retval DT_ERROR_OK: successful operation
///
/// @note must enable SPI bus before operating SensorSpiRW, and if not, SPI bus must be closed to avoid affecting SENSOR i2c access
DTCCM_API int _DTCALL_ SensorSpiInit(BOOL bEnable, int iRate=0, BOOL bType=FALSE, int iDevID=DEFAULT_DEV_ID);

/// @brief SPI controller interface supports three-line or four-line communication, single-byte transmission and multi-byte continuous transmission, and only UT300/UV910/UF920 are supported reading and writing requirements.
///
/// @param bStart: for True chip select signal to pull low, for False chip select signal to not pull low
/// @param bStop: for True chip select signal to pull up, for False chip select signal to not pull up
/// @param bMsb: True is high first out, False is low first out
/// @param TxData: the data BUFFER written
/// @param RxData: read back the data BUFFER
/// @param TxLen: the size, number of bytes written to the data
/// @param RxLen: size of read data, number of bytes
/// @retval DT_ERROR_OK: successful operation
/// @retval DT_ERROR_FAILD: operation failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SensorSpiRW(BOOL bStart,BOOL bStop,BOOL bMsb,UCHAR *TxData,UCHAR *RxData,UCHAR TxLen,UCHAR RxLen,int iDevID=DEFAULT_DEV_ID);



/****************************************************************************************
Flexible Interface Clock Output
*****************************************************************************************/
/// @brief flexible interface can adjust the clock, and support flexible pin to output 0-200khz adjustable clock(only UF920 is supported)
///
/// @param uHz: output clock size, unit Hz, 0-200khz adjustable
/// @param bOnOff: adjustable clock output enable, True enable and False enable
///
/// @retval DT_ERROR_OK: set the clock output successfully
/// @retval DT_ERROR_FAILD: set the clock output to fail
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetSoftPinAdjClk1(UINT uHz,BOOL bOnOff,int iDevID=DEFAULT_DEV_ID);

/// @brief flexible interface can adjust the clock, and support flexible pin to output 0-18mhz adjustable clock(only UF920 is supported)
///
/// @param uHundkHz: set the output clock size, 100KHz, 0-18mhz adjustable
/// @param bOnOff: adjustable clock output enable, True enable and False enable
///
/// @retval DT_ERROR_OK: set the clock output successfully
/// @retval DT_ERROR_FAILD: set the clock output to fail
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetSoftPinAdjClk2(UINT uHundkHz,BOOL bOnOff,int iDevID=DEFAULT_DEV_ID);

/** @} */ // end of group3

/******************************************************************************
Image Data Collection Related
******************************************************************************/
/** @defgroup group4 Image data Collection  related


* @{

*/
/// @brief to start image data collection.
///
/// @param uImgBytes: image data size, unit byte
///
/// @retval DT_ERROR_OK: successful opening of image data collection
/// @retval DT_ERROR_FAILD: failed to open image data collection
/// @retval DT_ERROR_INTERNAL_ERROR: internal error
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ OpenVideo(UINT uImgBytes,int iDevID=DEFAULT_DEV_ID);

/// @brief close image data collection.
///
/// @retval DT_ERROR_OK: turn off the image data collection successfully
/// @retval DT_ERROR_FAILD: close image data collection failure
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ CloseVideo(int iDevID=DEFAULT_DEV_ID);

/// @brief sets the amount of data collected for one frame
///
/// @param uGrabFrameSize: sets the amount of data per frame, in bytes
DTCCM_API int _DTCALL_ SetGrabFrameSize(ULONG uGrabFrameSize,int iDevID=DEFAULT_DEV_ID);

/// @brief sets the amount of data collected for one frame
///
/// @param uGrabFrameSize: sets the amount of data per frame, in bytes
/// @param bEnable: the amount of data set by the user takes effect
DTCCM_API int _DTCALL_ SetGrabFrameSizeEx(BOOL bEnable,ULONG uGrabFrameSize,int iDevID=DEFAULT_DEV_ID);

/// @brief sets the type of SENSOR image data interface.
///
/// @param byPort: SENSOR image data interface type, see enumeration type "SENSOR_PORT"
/// @param uWidth: image data width
/// @param uHeight: image data height
///
/// @retval DT_ERROR_OK: set the SENSOR image data interface type successfully
/// @retval DT_ERROR_FAILD: set SENSOR image data interface type failed
/// @retval DT_ERROR_PARAMETER_INVALID: invalid image data interface type parameter
///
/// @ see SENSOR_PORT
DTCCM_API int _DTCALL_ SetSensorPort(BYTE byPort,USHORT uWidth,USHORT uHeight,int iDevID=DEFAULT_DEV_ID);

/// @brief reset SENSOR image data interface
///
/// @param byPort: SENSOR image data interface type, see enumeration type "SENSOR_PORT"
///
/// @retval DT_ERROR_OK: set successful
/// @retval DT_ERROR_FAILD: setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
///
/// @ see SENSOR_PORT
DTCCM_API int _DTCALL_ ResetSensorPort(BYTE byPort,int iDevID=DEFAULT_DEV_ID);

/// @brief configures FrameBuffer
///
/// @param pConfig: FrameBuffer configuration structure, the structure description is shown in the imagekit.h header file
DTCCM_API int _DTCALL_ SetFrameBufferConfig(FrameBufferConfig *pConfig,int iDevID=DEFAULT_DEV_ID);

/// @brief gets configuration information for the FrameBuffer
///
/// @param pConfig: FrameBuffer configuration structure, the structure description is shown in the imagekit.h header file
DTCCM_API int _DTCALL_ GetFrameBufferConfig(FrameBufferConfig *pConfig,int iDevID=DEFAULT_DEV_ID);

/// @brief clears the cache of the test box
///
/// @retval DT_ERROR_OK: cleared successfully
/// @retval DT_ERROR_FAILD: clear failure
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ ResetFrameBuffer(int iDevID=DEFAULT_DEV_ID);

/// @brief sets ROI.
///
/// @param roi_x0: start point horizontal coordinate, unit byte
/// @param roi_y0: start point vertical coordinate, unit byte
/// @param roi_hw: horizontal ROI image width, per byte
/// @param roi_vw: vertical ROI height, per byte
/// @param roi_hb: horizontal blank width, unit byte
/// @param roi_vb: horizontal blank height, unit byte
/// @param roi_hnum: number of horizontal rois, unit bytes
/// @param roi_vnum: number of vertical ROI, unit bytes
/// @param roi_en: ROI enabled
///
/// @retval DT_ERROR_OK: ROI set successful
/// @retval DT_ERROR_FAILD: ROI setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
///
/// @note the specified width and horizontal position in this function are in bytes and are guaranteed to be 4 bytes in width.
///
/// @ ROI image HTML. PNG
DTCCM_API int _DTCALL_ SetRoi(USHORT roi_x0, 
							  USHORT roi_y0, 
							  USHORT roi_hw, 
							  USHORT roi_vw, 
							  USHORT roi_hb, 
							  USHORT roi_vb, 
							  USHORT roi_hnum, 
							  USHORT roi_vnum, 
							  BOOL roi_en,
							  int iDevID=DEFAULT_DEV_ID);

/// @brief sets ROI.
///
/// @param roi_x0: start point horizontal coordinate, unit pixel
/// @param roi_y0: start point vertical coordinate, unit pixel
/// @param roi_hw: horizontal ROI image width, unit pixel
/// @param roi_vw: vertical ROI height, unit pixel
/// @param roi_hb: horizontal blank width, unit pixel
/// @param roi_vb: horizontal blank height, unit pixel
/// @param roi_hnum: number of horizontal rois, unit pixels
/// @param roi_vnum: number of vertical ROI, unit pixel
/// @param roi_en: ROI enabled
///
/// @retval DT_ERROR_OK: ROI set successful
/// @retval DT_ERROR_FAILD: ROI setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
///
/// @note the specified width and horizontal position in this function are in pixels, and you want to make sure that the width is an integer multiple of 16 bytes after it is converted to bytes.
///
/// @ ROI image HTML. PNG
DTCCM_API int _DTCALL_ SetRoiEx(USHORT roi_x0, 
								USHORT roi_y0, 
								USHORT roi_hw, 
								USHORT roi_vw, 
								USHORT roi_hb, 
								USHORT roi_vb, 
								USHORT roi_hnum, 
								USHORT roi_vnum, 
								BOOL roi_en,
								int iDevID=DEFAULT_DEV_ID);


/// @brief sets the timeout for collection data, unit: ms. If the image data is not collected within the event, the GrabFrame function returns a timeout error.
///
/// @param uTimeOut: data collection timeout setting, unit ms
DTCCM_API int _DTCALL_ SetGrabTimeout(ULONG uTimeOut,int iDevID=DEFAULT_DEV_ID);

/// @brief set ISP processing mode, see enum ISP_MODE
///
/// @param byMode: ISP processing mode, 0 is normal, 1 is S2DFAST, 2 is S2DFAST GPU
DTCCM_API int _DTCALL_ SetImageProcessMode(BYTE byMode,int iDevID=DEFAULT_DEV_ID);

/// @brief S2DFAST model grabs data and gets RGB data
///
/// @param pBuf: frame buffer
///
/// retval:DT_ERROR_FAILED: fetching failure, get error frames
/// retval:DT_ERROR_TIMEOUT: fetching timeout, no data caught
DTCCM_API int _DTCALL_ S2DGrabFrameDirect(BYTE **ppBuf,FrameInfoEx *pInfo,int iDevID=DEFAULT_DEV_ID);

/// @brief releases buf. Each time S2DGrabFrameDirect is called to catch the data, it needs to be released after it is displayed

DTCCM_API int _DTCALL_ S2DReleaseBufferDirect(BYTE *pBuf,int iDevID=DEFAULT_DEV_ID);

/// @brief calibration sensor received. It is recommended to call after openvideo. After successful calibration, the frame capture operation is conducted again
///
/// @param uTimeOut: calibrate the timeout setting, unit ms
///
/// @retval DT_ERROR_OK: the calibration is successful and the image can be collected
/// @retval DT_ERROR_TIME_OUT: calibration timeout
DTCCM_API int _DTCALL_ CalibrateSensorPort(ULONG uTimeOut,int iDevID=DEFAULT_DEV_ID);

/// @brief collects A frame image and returns some information of the frame. Both channels A and B use the GrabFrame function to obtain the image data. The frame information can distinguish the channels to which the image data belongs.
///
/// @param pInBuffer: capture image BUFFER
/// @param uBufferSize: capture image BUFFER size, unit byte
/// @param pGrabSize: the size of the actual captured image data, in bytes
/// @param pInfo: image data information returned
///
/// @retval DT_ERROR_OK: one image was successfully collected
/// @retval DT_ERROR_FAILD: failed to collect a frame image, probably not the full frame image data
/// @retval DT_ERROR_TIME_OUT: collection timeout
/// @retval DT_ERROR_INTERNAL_ERROR: internal error
/// 
/// @note before calling this function, get a cache size large enough to load the image data.\ n
/// at the same time, the size of the cache needs to be passed into the GrabFrame function as a parameter to prevent out-of-bounds memory operations in the case of an exception. 
DTCCM_API int _DTCALL_ GrabFrame(BYTE *pInBuffer, ULONG uBufferSize,ULONG *pGrabSize,FrameInfo *pInfo,int iDevID=DEFAULT_DEV_ID);

/// @brief collects a frame image and returns some information of the frame. Time stamp, ECC, CRC, etc. of the frame can be obtained through the frame information structure.
///
/// @param pInBuffer: capture image BUFFER
/// @param uBufferSize: capture image BUFFER size, unit byte
/// @param pGrabSize: the size of the actual captured image data, in bytes
/// @param pInfo: image data information returned
///
/// @retval DT_ERROR_OK: one image was successfully collected
/// @retval DT_ERROR_FAILD: failed to collect a frame image, probably not the full frame image data
/// @retval DT_ERROR_TIME_OUT: collection timeout
/// @retval DT_ERROR_INTERNAL_ERROR: internal error
///
/// @note before calling this function, get a cache size large enough to load the image data.\ n
/// at the same time, the size of the cache needs to be passed into the GrabFrameEx function as a parameter to prevent out-of-bounds memory operations. 
DTCCM_API int _DTCALL_ GrabFrameEx(BYTE *pInBuffer, ULONG uBufferSize,ULONG *pGrabSize,FrameInfoEx *pInfo,int iDevID=DEFAULT_DEV_ID);

/// @brief image snapshot setting, set the number of frames that can be and one snapshot, up to 255.
///
/// @param bSnapEn: image capture enabled
/// @param uSnapCount: number of frames per snap
///
/// @retval DT_ERROR_OK: the image capture setting was successful
/// @retval DT_ERROR_FAILD: image capture setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error 
DTCCM_API int _DTCALL_ SetSnap(BOOL bSnapEn, UINT uSnapCount,int iDevID=DEFAULT_DEV_ID);

/// @brief starts a snap.
///
/// @retval DT_ERROR_OK: set to launch a snap successfully
/// @retval DT_ERROR_FAILD: set to launch a snap failure
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ TriggerSnap(int iDevID=DEFAULT_DEV_ID);

/// @brief resend a frame image.
///
/// @retval DT_ERROR_OK: set to resend a frame image successfully
/// @retval DT_ERROR_FAILD: set to replay a frame image failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ ReSendImage(int iDevID=DEFAULT_DEV_ID);

/// @brief to obtain the width and height information of the image collected. Note: *pWidth is in bytes.
///
/// @param pWidth: acquired image width information
/// @param pHeight: the acquired image height information is collected
/// @param byChannel: A/B channel selection
///
/// @retval DT_ERROR_OK: successfully acquired the width and height information of the collected image
/// @retval DT_ERROR_FAILD: failed to obtain the width and height information of the collected image
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ GetGrabImageSize(USHORT *pWidth,USHORT *pHeight,BYTE byChannel,int iDevID=DEFAULT_DEV_ID);

/// @brief returns the size (in bytes) of the actual captured image data.
///
/// @param pGrabSize: returns the size of the actual captured image data
DTCCM_API int _DTCALL_ CalculateGrabSize(ULONG *pGrabSize,int iDevID=DEFAULT_DEV_ID);

/// @brief gets the byte position in the pixel position data.
///
/// @param iPixelPos: pixel position
/// @param byImgFormat: image data format
///
/// Retval returns the byte position in the pixel location data given
DTCCM_API int _DTCALL_ GetByteOffset(int iPixelPos,BYTE byImgFormat,int iDevID=DEFAULT_DEV_ID);

/// @brief set the time point. After successful setting, the time stamp value of the device will be timed from that time point (unit us).
///
/// @param uTimeStamp: time point
///
/// @retval DT_ERROR_OK: set successful
/// @retval DT_ERROR_FAILD: setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetTimerClock(double uTime, int iDevID = DEFAULT_DEV_ID);

/// @brief get timestamp (unit us)
///
/// @param pTimeStamp: timestamp value
///
/// @retval DT_ERROR_OK: set successful
/// @retval DT_ERROR_FAILD: setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ GetTimeStamp(double *pTimeStamp, int iDevID = DEFAULT_DEV_ID);

/// @brief sets TestWindow function to set multiple ROI areas
///
/// @param uFullWidth: full resolution width, unit pixel
/// @param uFullHeight: full resolution height
/// @param pRoi:testwindow structure configuration, see "imagekit.h"
/// @param uCount: set the ROI number
///
/// @retval DT_ERROR_OK: set successful
/// @retval DT_ERROR_FAILD: setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int SetTestWindowRoi(UINT uFullWidth, UINT uFullHeight,DtRoi_t *pRoi,UINT uCount,int iDevID=DEFAULT_DEV_ID);

/// @brief TetsWindo function enables
///
/// @param bEnable:TestWindow enables
///
/// @retval DT_ERROR_OK: set successful
/// @retval DT_ERROR_FAILD: setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int SetTestWindowEnable(BOOL bEnable,int iDevID=DEFAULT_DEV_ID);

/// @brief collection is suspended
DTCCM_API int GrabHold(int iDevID=DEFAULT_DEV_ID);

/// collection reset
///
/// @param uDelayTime: sets the data stream delay output, unit us
/// @param uFrameCnt: set the number of frames to be collected after the restart of collection, and reach the set number of frames to be collected, then automatically GrabHold. If it is 0, it is a continuous output frame
///
/// @retval DT_ERROR_OK: set successful
/// @retval DT_ERROR_FAILD: setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int GrabRestart(double lfDelayTime,UINT uFrameNum=0,int iDevID=DEFAULT_DEV_ID);

/// collection reset
///
/// @param uSkipFrameNum: sets how many frames of output the data stream delays, per frame
/// @param uFrameNum: set the number of frames to be collected after the restart of the collection, and reach the set number of frames to be collected, then automatically GrabHold. If it is 0, it is a continuous output frame
///
/// @retval DT_ERROR_OK: set successful
/// @retval DT_ERROR_FAILD: setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int GrabRestartWithSkip(UINT uSkipFrameNum,UINT uFrameNum=0,int iDevID=DEFAULT_DEV_ID);

/// the new capture frame interface is more efficient than the original acquisition interface. With this interface, users do not need to apply the buffer
DTCCM_API int GrabFrameDirect(BYTE **ppBuf, FrameInfoEx *pInfo, int iDevID=DEFAULT_DEV_ID);

/// capture whether the frame reaches the mark
///
/// @param pIsArrived: TRUE means the frame has been collected, and FALSE means the frame has not been collected
DTCCM_API int GrabFrameIsArrived(BOOL *pIsArrived, int iDevID=DEFAULT_DEV_ID);
/** @} */ // end of group4

/** @defgroup group5 power management unit related


* @{

*/
/// @brief PMU reset may cause the system power to restart and all modules to be reenergized.
///
/// @retval DT_ERROR_OK: PMU reset successfully
/// @retval DT_ERROR_FAILD: PMU reset failed
/// @retval DT_ERROR_INTERNAL_ERROR: internal error
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ PmuReset(int iDevID=DEFAULT_DEV_ID);

/// @brief to obtain the system power status (voltage, current).
///
/// @param Power: system Power type, see enumeration type "SYS_POWER"
/// @param Voltage: obtained system Voltage value, unit mV
/// @param Current: system Current value obtained, unit mA
/// @param iCount: number of paths to acquire system power
///
/// @retval DT_ERROR_OK: the system power status was successfully obtained
/// @retval DT_ERROR_COMM_ERROR: communication error
///
/// @ see SYS_POWER
///
/// @ note this function only UV910 / DTLC2 / UH910 / UH920 / UF920 / PE350 / PE950 support.
DTCCM_API int _DTCALL_ PmuGetSysPowerVA(SYS_POWER Power[],int Voltage[],int Current[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief sets power mode.
///
/// @param Power: Power type, see enumeration type "SENSOR_POWER"
/// @param Mode: power Mode, see enumeration type "POWER_MODE"
/// @param iCount: power path number
///
/// @retval DT_ERROR_OK: set power mode successfully
/// @retval DT_ERROR_FAILD: set power mode failed
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND: parameter is out of range
///
/// @ see SENSOR_POWER
/// @ see POWER_MODE
///
/// @ note this function only UV910 / DTLC2 / UH910 / UH920 / UF920 / PE350 / PE950 support.
DTCCM_API int _DTCALL_ PmuSetMode(SENSOR_POWER Power[],POWER_MODE Mode[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief sets the power supply voltage.
///
/// @param Power: Power type, see enumeration type "SENSOR_POWER"
/// @param Voltage: set power Voltage value, unit mV
/// @param iCount: power path number
///
/// @retval DT_ERROR_OK: set the power voltage successfully
/// @retval DT_ERROR_FAILD: set power voltage failure
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND: parameter is out of range
///
/// @ see SENSOR_POWER
DTCCM_API int _DTCALL_ PmuSetVoltage(SENSOR_POWER Power[],int Voltage[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief to obtain the power supply voltage. If detected, use the detected data as far as possible; otherwise, return the voltage setting value.
///
/// @param Power: Power type, see enumeration type "SENSOR_POWER"
/// @param Voltage: the power supply Voltage obtained, the unit mV
/// @param iCount: power path number
///
/// @retval DT_ERROR_OK: set the power voltage successfully
/// @retval DT_ERROR_FAILD: set power voltage failure
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND: parameter is out of range
///
/// @ see SENSOR_POWER
DTCCM_API int _DTCALL_ PmuGetVoltage(SENSOR_POWER Power[],int Voltage[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief set the current range of power supply.
///
/// @param Power: Power type, see enumeration type "SENSOR_POWER"
/// @param Range: power current Range, see enumeration type "CURRENT_RANGE"
/// @param iCount: power path number
///
/// @retval DT_ERROR_OK: set power current range successfully
/// @retval DT_ERROR_FAILD: failed to set power current range
///
/// @ see SENSOR_POWER
/// @ see CURRENT_RANGE
///
/// @ note this function only UV910 / DTLC2 / UH910 / UH920 / UF920 / PE350 / PE950 support.
DTCCM_API int _DTCALL_ PmuSetCurrentRange(SENSOR_POWER Power[],CURRENT_RANGE Range[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief to obtain the power supply current.
///
/// @param Power: Power type, see enumeration type "SENSOR_POWER"
/// @param Current: Current value, unit nA
/// @param iCount: power path number
///
/// @retval DT_ERROR_OK: successful capture of power current
/// @retval DT_ERROR_FAILD: failed to get power current
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND: parameter is out of range
///
/// @ see SENSOR_POWER
///
/// @ note this function only UV910 / DTLC2 / UH910 / UH920 / UF920 / PE350 / PE950 support.
DTCCM_API int _DTCALL_ PmuGetCurrent(SENSOR_POWER Power[],int Current[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief set voltage rising edge, set value (Rise) unit :mV/mS.
///
/// @param Power: Power type, see enumeration type "SENSOR_POWER"
/// @param Rise: voltage rising edge value, unit :mV/mS
/// @param iCount: power path number
///
/// @retval DT_ERROR_OK: set the voltage rising edge successfully
/// @retval DT_ERROR_FAILD: set voltage rising edge failure
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND: parameter is out of range
///
/// @ see SENSOR_POWER
///
/// @ note this function only UV910 / DTLC2 / UH910 / UH920 / UF920 / PE350 / PE950 support.
DTCCM_API int _DTCALL_ PmuSetRise(SENSOR_POWER Power[],int Rise[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief setting of current acquisition speed, set value (SampleSpeed) of 10-500 units ms (default setting is 100).
///
/// @param Power: Power type, see enumeration type "SENSOR_POWER"
/// @param SampleSpeed: current acquisition speed, range 10-500
/// @param iCount: power path number
///
/// @retval DT_ERROR_OK: set the current acquisition speed successfully
/// @retval DT_ERROR_FAILD: set current collection speed failure
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND: parameter is out of range
///
/// @ see SENSOR_POWER
///
/// @ note this function only UV910 / DTLC2 / UH910 / UH920 / UF920 / PE350 / PE950 support.
DTCCM_API int _DTCALL_ PmuSetSampleSpeed(SENSOR_POWER Power[],int SampleSpeed[],int iCount,int iDevID=DEFAULT_DEV_ID);


/// @brief sets the power switch status.
///
/// @param Power: Power type, see enumeration type "SENSOR_POWER"
/// @param OnOff: sets the power switch status, TRUE is on and FALSE is off
/// @param iCount: power path number
///
/// @retval DT_ERROR_OK: set the power switch status successfully
/// @retval DT_ERROR_FAILD: failed to set power switch status
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND: parameter is out of range
///
/// @ see SENSOR_POWER
DTCCM_API int _DTCALL_ PmuSetOnOff(SENSOR_POWER Power[],BOOL OnOff[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief to obtain the power switch status.
///
/// @param Power: Power type, see enumeration type "SENSOR_POWER"
/// @param OnOff: gets the power switch status, TRUE is on and FALSE is off
/// @param iCount: power path number
///
/// @retval DT_ERROR_OK: the power switch state was successfully retrieved
/// @retval DT_ERROR_FAILD: failed to get power switch status
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND: parameter is out of range
///
/// @ see SENSOR_POWER
DTCCM_API int _DTCALL_ PmuGetOnOff(SENSOR_POWER Power[],BOOL OnOff[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief sets the CurrentLimit for overcurrent protection, and set the CurrentLimit (mA).
///
/// @param Power: Power type, see enumeration type "SENSOR_POWER"
/// @param CurrentLimit: sets the CurrentLimit value for overcurrent protection, mA
/// @param iCount: power path number
///
/// @retval DT_ERROR_OK: set the current limit for overcurrent protection successfully
/// @retval DT_ERROR_FAILD: set current limit for overcurrent protection to fail
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND: parameter is out of range
///
/// @ see SENSOR_POWER
DTCCM_API int _DTCALL_ PmuSetOcpCurrentLimit(SENSOR_POWER Power[],int CurrentLimit[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief setting of OCP Delay (accumulation of overcurrent time) and Delay unit :mS.
///
/// @param Power: Power type, see enumeration type "SENSOR_POWER"
/// @param Delay: OCP Delay setting value
/// @param iCount: power path number
///
/// @retval DT_ERROR_OK: set OCP delay successful
/// @retval DT_ERROR_FAILD: set OCP delay failed
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND: parameter is out of range
///
/// @ see SENSOR_POWER
///
/// @ note this function only UV910 / DTLC2 / UH910 / UH920 / UF920 / PE350 / PE950 support.
DTCCM_API int _DTCALL_ PmuSetOcpDelay(SENSOR_POWER Power[],int Delay[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief setting OCP turn off time (after being judged to be overcurrent protection, turn off for a period of time), setting unit :mS.
///
/// @param Power: Power type, see enumeration type "SENSOR_POWER"
/// @param Hold: OCP shutdown time
/// @param iCount: power path number
///
/// @retval DT_ERROR_OK: set OCP shutdown time successfully
/// @retval DT_ERROR_FAILD: set OCP shutdown time failed
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND: parameter is out of range
///
/// @ see SENSOR_POWER
///
/// @ note this function only UV910 / DTLC2 / UH910 / UH920 / UF920 / PE350 / PE950 support.
DTCCM_API int _DTCALL_ PmuSetOcpHold(SENSOR_POWER Power[],int Hold[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief gets flow status and returns information (State)TRUE: flow protection and FALSE: normal.
///
/// @param Power: Power type, see enumeration type "SENSOR_POWER"
/// @param State: gets the overflow State
/// @param iCount: power path number
///
/// @retval DT_ERROR_OK: successful capture of Ocp state
/// @retval DT_ERROR_FAILD: successfully failed to get Ocp state
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND: parameter is out of range
///
/// @ see SENSOR_POWER
///
/// @ note this function only UV910 / DTLC2 / UH910 / UH920 / UF920 / PE350 / PE950 support.
DTCCM_API int _DTCALL_ PmuGetOcpState(SENSOR_POWER Power[],BOOL State[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief sets the SENSOR power switch.
///
/// @param bOnOff: SENSOR power switch sets the value, TRUE is on and FALSE is off
///
/// @retval DT_ERROR_OK: set the SENSOR power switch successfully
/// @retval DT_ERROR_FAILD: setting SENSOR power switch failed
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_OUT_OF_BOUND: parameter is out of range
///
/// @ note this function only UV910 / DTLC2 / UH910 / UH920 / UF920 / PE350 / PE950 support.
DTCCM_API int _DTCALL_ SetSensorPowerOnOff(BOOL bOnOff,int iDevID=DEFAULT_DEV_ID);


/** @} */ // end of group5


/******************************************************************************
Initialization, Control Related
******************************************************************************/
/** @defgroup group6 initialization control is relevant


* @{

*/
/// @brief to open the equipment. Only after successful opening, the equipment can operate;The device object corresponds to the ID given. IDevID =1 creates the device object m_device[1], and iDevID=0 creates the device object m_device[0].
///
/// @param pszDeviceName: the name of the device to be opened
/// @param pDevID: returns the ID number of the open device
///
/// @retval DT_ERROR_OK: the device was opened successfully
/// @retval DT_ERROR_FAILD: failed to open the device
/// @retval DT_ERROR_INTERNAL_ERROR: internal error
/// @retval DT_ERROR_PARAMETER_INVALID: parameter is invalid
DTCCM_API int _DTCALL_ OpenDevice(const char *pszDeviceName,int *pDevID,int iDevID=DEFAULT_DEV_ID);

/// @brief reconnect the device. Usb3.0 device is effective
///
/// @retval DT_ERROR_OK: the device was opened successfully
/// @retval DT_ERROR_FAILD: failed to open the device
DTCCM_API int _DTCALL_ DeviceReset(int iDevID=DEFAULT_DEV_ID);


/// @brief opens the device in upgrade firmware mode. Only after successful opening can the device be operated.
///
/// @param pszDeviceName: the name of the device to be opened
/// @param pDevID: returns the ID number of the open device
///
/// @retval DT_ERROR_OK: the device was opened successfully
/// @retval DT_ERROR_FAILD: failed to open the device
/// @retval DT_ERROR_INTERNAL_ERROR: internal error
/// @retval DT_ERROR_PARAMETER_INVALID: parameter is invalid
DTCCM_API int _DTCALL_ OpenDeviceUpgradeMode(const char *pszDeviceName,int *pDevID,int iDevID=DEFAULT_DEV_ID);

/// @brief close the equipment. After closing the equipment, it cannot be operated again.
///
/// @retval DT_ERROR_OK: the device was turned off successfully
/// @retval DT_ERROR_FAILD: shutdown device failed
DTCCM_API int _DTCALL_ CloseDevice(int iDevID=DEFAULT_DEV_ID);

/// @brief initialization device. This function is mainly used to initialize the type of SENSOR interface, image format, image width and height information of the device. At the same time, users are required to pass in window handle for video display.
///
/// @param hWnd: window handle showing A channel image
/// @param uImgWidth, uImgHeight: set the width and height information of the SENSOR output (unit: pixel, possible ROI)
/// @param bySensorPort: SENSOR output interface type, such as MIPI/ parallel
/// @param byImgFormat: image data format, such as RAW/YUV
/// @param byChannel: A channel /B channel /AB works simultaneously
/// @param hWndEx: window handle showing image of channel B
///
/// @retval DT_ERROR_OK: initialization succeeded
/// @retval DT_ERROR_FAILD: initialization failed
/// @retval DT_ERROR_PARAMETER_INVALID: bySensorPort parameter is invalid
///
/// @note InitDevice function supports the initialization of double-channel test boards (such as DTLC2/UH910). If you need to use b-channel of such test boards, please do the following additional operations:
/// @note byChannel parameter is passed into CHANNEL_A|CHANNEL_B;The hWndEx parameter is passed into the window handle for the B channel video display.
DTCCM_API int _DTCALL_ InitDevice(HWND hWnd, 
								  USHORT uImgWidth, 
								  USHORT uImgHeight,
								  BYTE bySensorPort,
								  BYTE byImgFormat,
								  BYTE byChannel=CHANNEL_A,
								  HWND hWndEx=NULL,
								  int iDevID=DEFAULT_DEV_ID);

/// @brief gets the key value of the button on the device.
///
/// @param pKey: the key value obtained
///
/// @retval DT_ERROR_OK: successfully got the key value
/// @retval DT_ERROR_FAILED: failed to obtain the key value
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_INTERNAL_ERROR: internal error
DTCCM_API int _DTCALL_ GetKey(DWORD *pKey,int iDevID=DEFAULT_DEV_ID);

/// @brief sets alarm horn.
///
/// @param uBeepTime: buzzer time, in mS
/// @param iBeepCount: number of buzzers
///
/// @retval DT_ERROR_OK: set alarm horn successfully
/// @retval DT_ERROR_FAILED: set alarm horn failed
/// @retval DT_ERROR_INTERNAL_ERROR: internal error
DTCCM_API int _DTCALL_ SetAlarm(USHORT uBeepTime, int iBeepCount,int iDevID=DEFAULT_DEV_ID);
/** @} */ // end of group6

/// @brief initializes ISP
///
/// @param uImgWidth: image data width, unit pixel
/// @param uHeight: image data height
/// @param byImgFormat: image data format, such as RAW/YUV
/// @param byChannel: A/B channel selection
DTCCM_API int _DTCALL_ InitIsp(USHORT uImgWidth,USHORT uImgHeight,BYTE byImgFormat,BYTE byChannel=CHANNEL_A,int iDevID=DEFAULT_DEV_ID);

/// @brief sets ROI.
///
/// @param roi_x0: start point horizontal coordinate, unit pixel
/// @param roi_y0: start point vertical coordinate, unit pixel
/// @param roi_hw: horizontal ROI image width, unit pixel
/// @param roi_vw: vertical ROI height, unit pixel
/// @param roi_hb: horizontal blank width, unit pixel
/// @param roi_vb: horizontal blank height, unit pixel
/// @param roi_hnum: number of horizontal rois, unit pixels
/// @param roi_vnum: number of vertical ROI, unit pixel
/// @param byImgFormat: image data format, such as RAW/YUV
/// @param roi_en: ROI enabled
///
/// @retval DT_ERROR_OK: ROI set successful
/// @retval DT_ERROR_FAILD: ROI setting failed
/// @retval DT_ERROR_COMM_ERROR: communication error
///
/// @note the specified width and horizontal position in this function are in pixels, and you want to make sure that the width is an integer multiple of 16 bytes after it is converted to bytes.
///
/// @ ROI image HTML. PNG
DTCCM_API int InitRoi(USHORT roi_x0, 
					  USHORT roi_y0, 
					  USHORT roi_hw, 
					  USHORT roi_vw, 
					  USHORT roi_hb, 
					  USHORT roi_vb, 
					  USHORT roi_hnum, 
					  USHORT roi_vnum, 
					  BYTE byImgFormat,
					  BOOL roi_en,
					  int iDevID=DEFAULT_DEV_ID);


/// hall sensor state acquisition
DTCCM_API int GetHallState(BOOL *pHallState,int iDevID=DEFAULT_DEV_ID);
/** @} */ // end of group6

/******************************************************************************
AF Related
******************************************************************************/

/** @defgroup group7 AF related


* @{

*/
/// @brief gets the D/A chip model and device address of AF device.
///
/// @param pDevType: get the D/A chip model of AF device
/// @param pDevAddr: get the D/A device address of AF device
DTCCM_API int _DTCALL_ GetAfDevType(UCHAR *pDevType, UCHAR *pDevAddr, int iDevID=DEFAULT_DEV_ID);


/// @brief sets the D/A chip model and device address of AF device.
///
/// @param uDevType: set the D/A chip model of AF device
/// @param uDevAddr: sets the D/A device address of the AF device
DTCCM_API int _DTCALL_ SetAfDevType(UCHAR uDevType, UCHAR uDevAddr, int iDevID=DEFAULT_DEV_ID);

/// @brief sets the output value of D/A of AF device.
///
/// @param uValue: set the D/A output value of AF device
/// @param uMode: the mode of the IC of AF device
///
/// @retval DT_ERROR_OK: set the D/A output value of AF device successfully
/// @retval DT_ERROR_COMM_ERROR: communication error
/// @retval DT_ERROR_PARAMETER_INVALID: I2C mode is invalid
/// @retval DT_ERROR_TIME_OUT: communication timeout
/// @retval DT_ERROR_INTERNAL_ERROR: internal error
/// @retval DT_ERROR_NOT_SUPPORTED: unsupported AF device type
/// @retval DT_ERROR_NOT_INITIALIZED: D/A device address that does not initialize AF device
DTCCM_API int _DTCALL_ WriteAfValue(USHORT uValue, UCHAR uMode, int iDevID=DEFAULT_DEV_ID);


/// @brief search for D/A chip in AF device.
///
/// @retval DT_ERROR_OK: search for D/A chip in AF device successfully
/// @retval DT_ERROR_FAILD: search for D/A chip in AF device failed
DTCCM_API int _DTCALL_ FindAfDevice(int iDevID=DEFAULT_DEV_ID);
/** @} */ // end of group7


/****************************************************************************************
LC/OS Related
****************************************************************************************/

/** @defgroup group8 LC/OS related


* @{

*/
/// @brief LC/OS test operation configuration.
///
/// @param Command: opcode, see macro definition "OS/LC test configuration definition"
/// @param IoMask: valid pin identifier bit, one pin per bit of every byte, if these bits are 1, the corresponding pin will be tested
/// @param PinNum: number of pins, which determines the size of the IoMask array. Generally, the number of bytes in the IoMask is: PinNum/8+(PinNum%8!= 0)
///
/// @retval DT_ERROR_OK: LC/OS test operation configuration was successful
/// @retval DT_ERROR_FAILD: LC/OS test operation configuration failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ LC_OS_CommandConfig(DWORD Command, UCHAR IoMask[], int PinNum, int iDevID=DEFAULT_DEV_ID);

/// @brief OS test parameter configuration.
///
/// @param Voltage: test Voltage, unit uV
/// @param HighLimit: Open test the standard array. Before testing, you should initialize the Open standard of each test pin, unit uV
/// @param LowLimit: Short tests the standard array. Before testing, initialize the open standard of each test pin, unit uV
/// @param PinNum: number of pins, which determines the size of the HighLimit and LowLimit arrays
/// @param PowerCurrent: power pin current, per uA
/// @param GpioCurrent: GPIOpin current, unit uA
///
/// @retval DT_ERROR_OK: the OS test parameters were configured successfully
/// @retval DT_ERROR_FAILD: OS test parameter configuration failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ OS_Config(int Voltage, int HighLimit[], int LowLimit[], int PinNum, int PowerCurrent, int GpioCurrent, int iDevID=DEFAULT_DEV_ID);

/// @brief OS test results read.
///
/// @ param VoltageH: positive pos test results, the uV
/// @ param VoltageH: reverse pos test results, the uV
/// @param Result: open short circuit test results, see macro definition "OS test results definition"
/// @param PosEn: positive test enabled
/// @param NegEn: reverse test enabled
/// @ param PinNum: pin number, the decision VoltageH, VoltageL, Result the array size
///
/// @retval DT_ERROR_OK: the OS test results read successfully
/// @retval DT_ERROR_FAILD: OS test result read failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ OS_Read(int VoltageH[], int VoltageL[], UCHAR Result[], BOOL PosEn, BOOL NegEn, int PinNum, int iDevID=DEFAULT_DEV_ID);

/// @brief LC test parameter configuration.
///
/// @param Voltage: LC test Voltage, unit mV
/// @param Range: LC precision setting, Range of 0, precision of 1uA, Range of 1, precision of 10nA
///
/// @retval DT_ERROR_OK: LC parameter parameter has been configured successfully
/// @retval DT_ERROR_FAILD: LC test parameter configuration failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ LC_Config(USHORT Voltage, UCHAR Range, int iDevID=DEFAULT_DEV_ID);

/// @brief LC test results read.
///
/// @param CurrentH: forward test results unit nA
/// @param CurrentL: reverse test result unit nA
/// @param PinNum: number of pins, which determines CurrentH, CurrentL, and array size
///
/// @retval DT_ERROR_OK: LC test results read successfully
/// @retval DT_ERROR_FAILD: LC test result read failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ LC_Read(ULONG CurrentH[],ULONG CurrentL[],int PinNum,int iDevID=DEFAULT_DEV_ID);

/// @brief switch status value read.
///
/// @param Result: returns the switch status value
/// @param PinNum: number of pins, which determines Result, array size
///
/// @retval DT_ERROR_OK: LC switch status value read successfully
/// @retval DT_ERROR_FAILD: LC switch state value read failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ LC_ReadSwitch(UCHAR Result[],int PinNum,int iDevID=DEFAULT_DEV_ID);
/** @} */ // end of group8

/** @defgroup group9 extended IO


* @{

*/
/************************************************************************
 extended IO                                                                     
************************************************************************/
/// @brief sets the 3.3v voltage output of external extensions.
///
/// @param bEnable: extends the voltage output enable
///
/// @retval DT_ERROR_OK: set the external extended 3.3v voltage output successfully
/// @retval DT_ERROR_FAILD: set the 3.3v voltage output of the external extension failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetExtPowerEnable(BOOL bEnable,int iDevID=DEFAULT_DEV_ID);

/// @brief sets the clock output for external extensions.
///
/// @param bOnOff: enables clock output
/// @param uHundKhz: sets the output value of the clock, unit Khz
///
/// @retval DT_ERROR_OK: set external extension clock output successful
/// @retval DT_ERROR_FAILD: set external extension clock output to fail
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetExtClock(BOOL bOnOff,USHORT uHundKhz,int iDevID=DEFAULT_DEV_ID);

/// @brief sets up the external extension GPIO mode.
///
/// @param Pin[]:GPIO number refers to macro definition "EXT_GPIO"
/// @param Mode[]:Mode setting refers to enumeration type "EXT_GPIO_MODE"
/// @param iCount:set the number of IO
///
/// @retval DT_ERROR_OK:set external extended GPIO mode successfully
/// @retval DT_ERROR_FAILD:fail to set the external extension GPIO mode 
/// @retval DT_ERROR_COMM_ERROR:communication error
DTCCM_API int _DTCALL_ SetExtIoMode(EXT_GPIO Pin[],EXT_GPIO_MODE Mode[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief sets the external extended GPIO level if it is constant level output mode.
///
/// @param Pin[]:GPIO number refers to macro definition "EXT_GPIO"
/// @param Level[]:GPIO Level, FALSE is low level, TRUE  is high level
/// @param iCount:set the number of IO
///
/// @retval DT_ERROR_OK:set external extended GPIO level successfully
/// @retval DT_ERROR_FAILD:fail to set external extension GPIO level
/// @retval DT_ERROR_COMM_ERROR:communication error
DTCCM_API int _DTCALL_ SetExtIoLevel(EXT_GPIO Pin[],BOOL Level[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief gets the level of external extension GPIO
/// 
/// @param Pin[]:GPIO number refers to macro definition "EXT_GPIO"
/// @param Level[]:GPIO Level, FALSE is low level, TRUE  is high level 
/// @param iCount:set the number of IO
///
/// @retval DT_ERROR_OK:get external extended GPIO level successfully
/// @retval DT_ERROR_FAILD:fail to get external extension GPIO level
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ GetExtIoLevel(EXT_GPIO Pin[],BOOL Level[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief extension GPIO sets the time for high and low levels when interactive output is high and low levels.
/// 
/// @param Pin[]: GPIO number refers to definition "EXT_GPIO"
/// @param HighLevelTime[]: corresponding to extended GPIO HighLevelTime, unit us
/// @param LowLevelTime[]: corresponding to extended GPIO LowLevelTime, unit us
/// @param iCount: sets the number of IO
///
/// @retval DT_ERROR_OK: set external extended GPIO high and low level time successfully
/// @retval DT_ERROR_FAILD: set external extension GPIO high and low level time failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetExtIoLevelTime(EXT_GPIO Pin[],int HighLevelTime[],int LowLevelTime[],int iCount,int iDevID=DEFAULT_DEV_ID);

/// @brief  external extension I2C write
///
/// @param uDevAddr:device address
/// @param uRegAddr:device register
/// @param uRegAddrSize:number of bytes of register address
/// @param pData: the data block written to the register
/// @param uSize:the data block size written to the register is 60 bytes at most
/// 
/// @retval DT_ERROR_OK:write successful
/// @retval DT_ERROR_FAILD:write failed
/// @retval DT_ERROR_COMM_ERROR:communication error
DTCCM_API int _DTCALL_ ExtI2cWrite(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID=DEFAULT_DEV_ID);

/// @brief external extension I2C reads
///
/// @param uDevAddr:device address
/// @param uRegAddr:device register
/// @param uRegAddrSize: number of bytes of register address
/// @param pData:read the data block of the register
/// @param uSize:reads the data block size of the register, maximum 60 bytes
/// @param bNoStop:whether to issue an I2C STOP command, which is normally default to FALSE, bNoStop=TRUE means that there is no I2C STOP command for the writing process and bNoStop=FALSE has I2C STOP command
/// 
/// @retval DT_ERROR_OK:read successfully
/// @retval DT_ERROR_FAILD:read failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ ExtI2cRead(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop=FALSE,int iDevID=DEFAULT_DEV_ID);

/// @brief sets the external trigger output interface level
/// 
/// @param iTriggerOutPin:trigger output interface number(MIPIONLY products provides a trigger output IO)
/// @param bFollowTrigIn: trigger output interface following enable signal; if is 1, follow the TriggerIn signal level; if is 0, set level value
/// @param bLevel:level value (0 is the output low level, 1 is the output high level)
///
/// @retval DT_ERROR_OK:read successfully
/// @retval DT_ERROR_FAILD:read failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ SetTriggerOutLevel(int iTriggerOutPin,BOOL bFollowTrigIn,BOOL bLevel,int iDevID=DEFAULT_DEV_ID);

/// @brief Get the external trigger input interface level
/// 
/// @param iTriggerInPin:trigger input interface number(MIPIONLY products provides a trigger output IO)
/// @param pLevel:return level (0 is output low level, 1 is output high level)
///
/// @retval DT_ERROR_OK:read successfully
/// @retval DT_ERROR_FAILD:read failed
/// @retval DT_ERROR_COMM_ERROR: communication error
DTCCM_API int _DTCALL_ GetTriggerInLevel(int iTriggerInPin,BOOL *pLevel,int iDevID=DEFAULT_DEV_ID);

/** @} */ // end of group9

/*************************************************************************
Temperature sensors	
*************************************************************************/

///Set the upper and lower limit temperature value of fan rotation, and do not rotate the fan when it is lower than the lower limit value, higher than the lower limit value, lower than the upper limit value, lower than the upper limit value, and higher than the upper limit value.
///
/// @param iUpperLimit: maximum temperature value
/// @param iLowerLimit: lower limit temperature value
/// @benable: =TRUE, manually enable the fan to rotate, =FALSE, and automatically adjust the device fan to rotate according to the temperature value at upper and lower limits
DTCCM_API int _DTCALL_ SetTempRange(int iUpperLimit, int iLowerLimit, BOOL bEnable,int iDevID=DEFAULT_DEV_ID);

///Read the current temperature value
///
/// @param pTemp: temperature value of the current device
DTCCM_API int _DTCALL_ GetCurrentTemp(int *pTemp,int iDevID=DEFAULT_DEV_ID);

/// read the temperature range currently set
DTCCM_API int _DTCALL_ GetTempRange(int *pUpperLimit,int *pLowLimit,int iDevID=DEFAULT_DEV_ID);

/*************************************************************************
Status Box
*************************************************************************/
/** @defgroup group10 debug status box


* @{

*/

/// status information interface to monitor the data flow of the equipment after drawing
///
/// @param hwnd: handle to the user program
DTCCM_API int _DTCALL_ ShowInternalStatusDialog(HWND hwnd,int iDevID=DEFAULT_DEV_ID);

/// status information interface to monitor the data flow of the equipment after drawing
///
/// @param hwnd: handle to the user program
/// @param pRetHwnd: handle that returns the internal status information box interface
DTCCM_API int _DTCALL_ ShowInternalStatusDialogEx(HWND hwnd ,HWND *pRetHwnd,int iDevID=DEFAULT_DEV_ID);

/// property box interface
DTCCM_API int _DTCALL_ ShowMipiControllerDialog(HWND hwnd ,HWND *pRetHwnd,int iDevID=DEFAULT_DEV_ID);

/// property box interface
DTCCM_API int _DTCALL_ ShowFrameBufferControlDialog(HWND hwnd ,HWND *pRetHwnd,int iDevID=DEFAULT_DEV_ID);
/** @} */ // end of group10

//DTCCM_API int _DTCALL_ SaveParameter(int iDevID=DEFAULT_DEV_ID);

//DTCCM_API int _DTCALL_ LoadParameter(int iDevID=DEFAULT_DEV_ID);
/****************************************************************************************
Other
****************************************************************************************/
//  Extend control interface
// dwCtrl:	Control code, possibly defined by derived classes
// dwParam:	Control code related parameters
// pIn:		Cata read
// pInSize:	The number of data bytes to read ,the caller specifies the number of bytes to read and if success return the number of bytes actually read
// pOut:	Data written
// pOutSize:The number of data bytes to write ,the caller specifies the number of bytes to write and if success return the number of bytes actually write
DTCCM_API int _DTCALL_ ExCtrl(DWORD dwCtrl,DWORD dwParam,PVOID pIn,int *pInSize,PVOID pOut,int *pOutSize,int iDevID=DEFAULT_DEV_ID);


#endif