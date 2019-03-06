#pragma once

#include "dt.h"
#include "DccmStruct.h"
#include "DccmDef.h"
#include <vector>
using namespace std;

// 注释模板
// @brief: 说明接口函数含义
// 
// @param 参数1：说明参数1含义
// @param 参数2：说明参数2含义
// 
// @retval 返回值：说明返回值含义

/***********Sensor常用寄存器的功能操作封装，以及OTP操作封装**************/
class ImageSensor_Interface
{
public:
	virtual int Write_OTP_BYTE(USHORT addr, BYTE data, int PageNo = 0) = 0;
	virtual int Write_OTP_Multi_BYTE(USHORT addr, BYTE* data, int Length, int PageNo = 0) = 0;
	virtual int Read_OTP_BYTE(USHORT addr, BYTE* data, int PageNo = 0) = 0;
	virtual int Read_OTP_Multi_BYTE(USHORT addr, BYTE* data, int Length, int PageNo = 0) = 0;

	virtual int ReadExp(int& exp) = 0;		// 读曝光值
	virtual int WriteExp(int exp) = 0;		// 写曝光值
	virtual int GetExposureMaxValue() = 0;	// 得到曝光寄存器的最大值

	virtual int ReadAnalogGain(USHORT& gain) = 0; //读模拟GAIN
	virtual int WriteAnalogGain(USHORT gain) = 0; //写模拟GAIN
	virtual int GetAnalogGainMaxValue() = 0;      //得到模拟GAIN最大值
	virtual int SetAnalogRate(float Rate) = 0;      //设置几X的gain

	virtual int WriteColorGain(int Channel, USHORT gain) = 0; //读某个通道的gain
	virtual int ReadColorGain(int Channel, USHORT* gain) = 0; //写某个通道的gain
	virtual int GetColorGainMaxValue() = 0;           //得到颜色通道的最大值
	virtual int SetCoarseTime(UINT time) = 0;
	virtual char* GetFuseID(void) = 0; 

	virtual int InitOTP() = 0;
	virtual int ReleaseOTP() = 0;
    
};


/***********驱动IC的操作封装 AF驱动和OIS 操作**************/
class DriverIC_Interface
{
public:
	virtual int Init(void) = 0;
	virtual int ReadAF(USHORT* code) = 0;	// 0:OK, !0:NG
	virtual int WriteAF(USHORT code) = 0;	// 0:OK, !0:NG
	virtual int OIS_On(int mode = 0) = 0;		// mode 0 = still  1 = movie
	virtual int OIS_Off(void) = 0;
};

/***********EEPROMIC的操作封装读和写操作**************/
class EepromIC_Interface
{
public:
	virtual int Init(void) = 0;
	virtual int Eeprom_All_Erase() = 0;
	virtual int Eeprom_Page_Erase(BYTE page) = 0;
	virtual int Eeprom_Programing(USHORT address,unsigned char *Data,int size) = 0;
	virtual int Eeprom_Read(USHORT address,unsigned char *Data,int size) = 0;
};

/***********配置管理器的(ini文件读写) 封装**************/
class ConfigManager_Interface	
{
public:
	// 写入指定的分区下某子项的值（STRING）
	virtual void WriteConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR string) = 0;
	// 写入指定的分区下某子项的值(INT)
	virtual void WriteConfigInt(LPCTSTR AppName, LPCTSTR KeyName,int data,int datatype = DATA_DEC) = 0;
	// 读取指定的分区下某子项的值(INT)
	virtual int ReadConfigInt(LPCTSTR AppName, LPCTSTR KeyName, int defVal) = 0;
	// 读取指定的分区下某子项的值(STRING)
	virtual LPCTSTR ReadConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR defVal) = 0;
    // 删除子项(名称和值)
	virtual void ClearConfig(LPCTSTR AppName) = 0; 
    // 删除分区(分区名称和其下所有子项)
	virtual void ClearConfig(LPCTSTR AppName, LPCTSTR KeyName) = 0; 
	// 得到DCCF文件的路径
	virtual LPCTSTR GetConfigPath() = 0;
	// 写入指定的分区下某子项的值(double), precision：小数点位数
	virtual void WriteConfigDouble(LPCTSTR AppName,LPCTSTR Keyname, double data, int precision = 1) = 0;
	// 读取指定的分区下某子项的值(double)
	virtual double ReadConfigDouble(LPCTSTR AppName,LPCTSTR Keyname, double defVal) = 0;

	virtual OSTab* GetOSTab() = 0;
	virtual GeneralSettingTab*	GetGeneralTab() = 0;
};

class BootManager_Interface
{
public:
	// boot.ini写入指定的分区下某子项的值（STRING）
	virtual void WriteBootString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR string) = 0;
	// boot.ini写入指定的分区下某子项的值(INT)
	virtual void WriteBootInt(LPCTSTR AppName, LPCTSTR KeyName,int data,int datatype = DATA_DEC) = 0;
	// boot.ini读取指定的分区下某子项的值(INT)
	virtual int ReadBootInt(LPCTSTR AppName, LPCTSTR KeyName, int defVal) = 0;
	// boot.ini读取指定的分区下某子项的值(STRING)
	virtual LPCTSTR ReadBootString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR defVal) = 0;

	virtual LPCTSTR GetBootPath(void) = 0;
};


class DccmControl_Interface
{
public:
	virtual ConfigManager_Interface* GetConfigManager(void) = 0;
	virtual DriverIC_Interface*      GetDriverIC(void)      = 0;
	virtual EepromIC_Interface*		 GetEepromIC(void)		= 0;
	virtual ImageSensor_Interface*   GetImageSensor(void)   = 0;
	virtual BootManager_Interface*	 GetBootManager(void)   = 0;

/************** I2C 通信*******************/
public:
	// @brief：重置I2C，即复位与Sensor通讯的I2C总线。
	//
	// @retval TRUE：复位I2C操作成功
	// @retval FALSE：复位I2C操作失败
	virtual BOOL ResetI2CBus() = 0;
	virtual BOOL I2C_ReadSensor_Byte(WORD reg, BYTE* val)  = 0; // 从SENSOR指定寄存器读取一个BYTE
	virtual BOOL I2C_ReadSensor_Word(WORD reg, WORD* val)  = 0; // 从SENSOR指定寄存器读取二个BYTE
	virtual BOOL I2C_WriteSensor_Byte(WORD reg, BYTE val)  = 0; // 从SENSOR写入寄存器写入一个BYTE
	virtual BOOL I2C_WriteSensor_Word(WORD reg, WORD val)  = 0; // 从SENSOR写入寄存器写入二个BYTE
	// DT
	virtual BOOL I2C_Read(UCHAR addr, USHORT reg, USHORT* val, BYTE mode = 0)  = 0;
	virtual BOOL I2C_Write(UCHAR addr, USHORT reg, USHORT val, BYTE mode = 0)  = 0;
	virtual BYTE I2C_MultiBytesRead(UCHAR Addr, USHORT reg, BYTE regsize, BYTE *ParaList, USHORT Length, int bStop = 0)  = 0;
	virtual BYTE I2C_MultiBytesWrite(UCHAR Addr, USHORT reg, BYTE regsize, BYTE *ParaList, USHORT Length)  = 0;

/************** 得到当前图像/帧信息 *******************/
public:
	virtual ImageInfo GetImage() = 0;
/************** DC与显示  *******************/
public:
	virtual CDC* GetMemDC_Display() = 0;					// 得到辅助窗口DC
	virtual CDC* GetMemDC_Assistant() = 0;					// 得到主显窗口DC
	virtual void GetRect_Assistant(CRect* rect) = 0;		// 得到辅助窗口Rect
	virtual void GetRect_Display(CRect* rect) = 0;			// 得到主显窗口Rect
	virtual int  ShowMemDC(CDC* pMemDC) = 0;				// 显示DC
	virtual int  CopyBackGroundToMemDC(CDC* pMemDC) = 0;

/************** 文字显示 **************/

	virtual void AddLog(LPCTSTR m_info, COLORREF color = RGB(0,0,0), int FontSize = FONT_SIZE_DEFAULT) = 0;
	virtual void AddDebugLog(LPCTSTR m_info, COLORREF color = COLOR_GREY, int FontSize = FONT_SIZE_DEFAULT) = 0;	//只在Debug模式下输出Log
	virtual void ClearLog() = 0;			// 清空当前Log信息
	virtual void AddFileLog(LPCTSTR lpszFileName, LPCTSTR lpszStr) = 0;	// 在D:\DccmLog文件夹中的lpszFileName文件中，追加"[10/31/16 13:24:59] lpszStr"

/************** 路径 *****************/
public:
    virtual LPCTSTR GetExeDir() = 0;		// 获取当前程序的文件夹路径 Dir Path，带'\'
	virtual LPCTSTR GetReportDir() = 0;		// 获取当前程序的报表Report文件夹路径，不带'\'
	virtual int		GetAppNumber() = 0;		// #[n]
	virtual LPCTSTR	GetConfigTitle() = 0;	// dccf name
	virtual LPCTSTR GetConfigPath() = 0;	// dccf Path

/************** 曝光  *******************/
public:
	virtual int Exposure(int target, int tolerance, int expType = 0, BOOL bShowExpInfo = TRUE, int cx = 50, int cy = 50, double length = 20, int framePitch = 2) = 0;

/************** 测试项私有存储空间 *******************/
	virtual vector<double>* GetMyStorage(LPVOID PT_THIS) = 0;  // 获取本测试项目的存储空间,参数 = THIS 指针
	virtual vector<double>* GetStorage(int ID) = 0;				// 得到某个测试项目的存储空间
	virtual vector<double>* GetStorage(LPCTSTR lpszDllName) = 0;  // 得到某个测试项目的存储空间


/************** 图像采集板 *******************/
public:
	virtual BOOL SetVoltage(int channel,int voltage) = 0;  //sensor 电压控制 HV910D PE910D或以上
	virtual int RestartCamera(int index) = 0; //启动第index套参数启动摄像模组。
	virtual int PauseGrabFrame(void) = 0;     //让图像采集板停止抓取帧
	virtual int ResumeGrabFrame(void) = 0;    //恢复抓帧

	virtual BOOL bSensorEnable(BYTE data, BYTE enable) = 0;

    //自定义ParaList参数，读取后分配空间
	virtual int LoadParaList(LPCTSTR AppName, USHORT** ParaList, USHORT& ParaListSize) = 0;
	//批量读写I2C
	virtual BOOL bInitSensor(USHORT addr, USHORT* ParaList, USHORT Length, BYTE mode) = 0;
/************** 后期扩展 *******************/
	// 调用一次可使一个测试项目测试中画面无锐化等图像处理。同时保存图像的时候就是原始的RGB图像。
	virtual void ShowOriginalImage(void) = 0;  

	// percentage Image rect convert to display rect
	virtual CRect CalcPercentRect_ImageToDisplay(double cx, double cy, double x_length, double y_length) = 0;// 实际图像矩形框转化为主显窗口比例
	// percentage Display rect convert to image rect
	virtual CRect CalcPercentRect_ToImage(double cx, double cy, double x_length, double y_length) = 0;		// 主显窗口矩形框比例转化为实际图像
	virtual CPoint CalcPoint_ImageToDisplay(CPoint imgPoint) = 0;		// Image point convert to display point
	virtual CPoint CalcPoint_DisplayToImage(CPoint disPoint) = 0;		// Display point convert to image point
	virtual CRect CalcRect_ImageToDisplay(CRect imgRect) = 0;			// Image rect convert to display point
	virtual CRect CalcRect_DisplayToImage(CRect disRect) = 0;			// Display point convert to image point

	virtual CRect CalcPercentRect_ImageToAssistant(double cx, double cy, double x_length, double y_length) = 0;// 实际图像矩形框转化为主显窗口比例
	virtual CPoint CalcPoint_ImageToAssistant(CPoint imgPoint) = 0;
	virtual CPoint CalcPoint_AssistantToImage(CPoint disPoint) = 0;
	virtual CRect CalcRect_ImageToAssistant(CRect imgRect) = 0;
	virtual CRect CalcRect_AssistantToImage(CRect disRect) = 0;


	//作废
	virtual void StopExposure(void) = 0;

	//从接口获得CamID
	virtual LPCTSTR GetCamID(void) = 0;
    //设置CamID
	virtual void SetCamID(LPCTSTR ID) = 0;

	virtual void ShowRGBtoDisplayWnd(BYTE* pSrc, int width, int height) = 0;

	//设置中心点区域
	virtual int DrawRect1(double cx, double cy, double x_length, double y_length, COLORREF color = RGB(0,0,255), int lineWidth = 1)=0;
	//0-100% 区域
	virtual int DrawRect2(int l, int t, int r, int b, COLORREF color = RGB(0,0,255), int lineWidth = 1) = 0;
	//a b 椭圆 直径
	virtual int DrawEllipse(int x, int y, int a, int b, COLORREF color = RGB(0,0,255), int lineWidth = 1) =0;

	virtual int GetModuleNum(void) = 0;
	// pathName不需要添加后缀名
	virtual void SaveImage(CString pathName, eSaveImageType sit) = 0;

	virtual void GetCurrent(double* ADValue) = 0;	 //（Interface）Working Current 获取5路电流
	virtual void GetVoltage(double* voltage) = 0;	 //（Interface）获取5路电压
	virtual void GetCurrent_Standby(double* ADValue) = 0;	 //（Interface）获取3路待机电流
	virtual int  OpenShortTest(OSTab *pOSTab, int* OSValueArray) = 0;

	virtual int SendMsg(int CmdID = SM_INVLID, int Ack = SM_INVLID, int Result = SM_INVLID, CString ErrInfo = SM_INVLID_STR, CString TestData = SM_INVLID_STR) = 0;
	virtual int RecvMsg(int& CmdID, int& Status, int& Result, CString& ErrInfo, CString& TestData) = 0;
	virtual int GetRecvMsg(int& CmdID, int& Status, int& Result, CString& ErrInfo, CString& TestData) = 0;
};

