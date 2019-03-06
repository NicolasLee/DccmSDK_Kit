#pragma once

#include "dt.h"
#include "DccmStruct.h"
#include "DccmDef.h"
#include <vector>
using namespace std;

// ע��ģ��
// @brief: ˵���ӿں�������
// 
// @param ����1��˵������1����
// @param ����2��˵������2����
// 
// @retval ����ֵ��˵������ֵ����

/***********Sensor���üĴ����Ĺ��ܲ�����װ���Լ�OTP������װ**************/
class ImageSensor_Interface
{
public:
	virtual int Write_OTP_BYTE(USHORT addr, BYTE data, int PageNo = 0) = 0;
	virtual int Write_OTP_Multi_BYTE(USHORT addr, BYTE* data, int Length, int PageNo = 0) = 0;
	virtual int Read_OTP_BYTE(USHORT addr, BYTE* data, int PageNo = 0) = 0;
	virtual int Read_OTP_Multi_BYTE(USHORT addr, BYTE* data, int Length, int PageNo = 0) = 0;

	virtual int ReadExp(int& exp) = 0;		// ���ع�ֵ
	virtual int WriteExp(int exp) = 0;		// д�ع�ֵ
	virtual int GetExposureMaxValue() = 0;	// �õ��ع�Ĵ��������ֵ

	virtual int ReadAnalogGain(USHORT& gain) = 0; //��ģ��GAIN
	virtual int WriteAnalogGain(USHORT gain) = 0; //дģ��GAIN
	virtual int GetAnalogGainMaxValue() = 0;      //�õ�ģ��GAIN���ֵ
	virtual int SetAnalogRate(float Rate) = 0;      //���ü�X��gain

	virtual int WriteColorGain(int Channel, USHORT gain) = 0; //��ĳ��ͨ����gain
	virtual int ReadColorGain(int Channel, USHORT* gain) = 0; //дĳ��ͨ����gain
	virtual int GetColorGainMaxValue() = 0;           //�õ���ɫͨ�������ֵ
	virtual int SetCoarseTime(UINT time) = 0;
	virtual char* GetFuseID(void) = 0; 

	virtual int InitOTP() = 0;
	virtual int ReleaseOTP() = 0;
    
};


/***********����IC�Ĳ�����װ AF������OIS ����**************/
class DriverIC_Interface
{
public:
	virtual int Init(void) = 0;
	virtual int ReadAF(USHORT* code) = 0;	// 0:OK, !0:NG
	virtual int WriteAF(USHORT code) = 0;	// 0:OK, !0:NG
	virtual int OIS_On(int mode = 0) = 0;		// mode 0 = still  1 = movie
	virtual int OIS_Off(void) = 0;
};

/***********EEPROMIC�Ĳ�����װ����д����**************/
class EepromIC_Interface
{
public:
	virtual int Init(void) = 0;
	virtual int Eeprom_All_Erase() = 0;
	virtual int Eeprom_Page_Erase(BYTE page) = 0;
	virtual int Eeprom_Programing(USHORT address,unsigned char *Data,int size) = 0;
	virtual int Eeprom_Read(USHORT address,unsigned char *Data,int size) = 0;
};

/***********���ù�������(ini�ļ���д) ��װ**************/
class ConfigManager_Interface	
{
public:
	// д��ָ���ķ�����ĳ�����ֵ��STRING��
	virtual void WriteConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR string) = 0;
	// д��ָ���ķ�����ĳ�����ֵ(INT)
	virtual void WriteConfigInt(LPCTSTR AppName, LPCTSTR KeyName,int data,int datatype = DATA_DEC) = 0;
	// ��ȡָ���ķ�����ĳ�����ֵ(INT)
	virtual int ReadConfigInt(LPCTSTR AppName, LPCTSTR KeyName, int defVal) = 0;
	// ��ȡָ���ķ�����ĳ�����ֵ(STRING)
	virtual LPCTSTR ReadConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR defVal) = 0;
    // ɾ������(���ƺ�ֵ)
	virtual void ClearConfig(LPCTSTR AppName) = 0; 
    // ɾ������(�������ƺ�������������)
	virtual void ClearConfig(LPCTSTR AppName, LPCTSTR KeyName) = 0; 
	// �õ�DCCF�ļ���·��
	virtual LPCTSTR GetConfigPath() = 0;
	// д��ָ���ķ�����ĳ�����ֵ(double), precision��С����λ��
	virtual void WriteConfigDouble(LPCTSTR AppName,LPCTSTR Keyname, double data, int precision = 1) = 0;
	// ��ȡָ���ķ�����ĳ�����ֵ(double)
	virtual double ReadConfigDouble(LPCTSTR AppName,LPCTSTR Keyname, double defVal) = 0;

	virtual OSTab* GetOSTab() = 0;
	virtual GeneralSettingTab*	GetGeneralTab() = 0;
};

class BootManager_Interface
{
public:
	// boot.iniд��ָ���ķ�����ĳ�����ֵ��STRING��
	virtual void WriteBootString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR string) = 0;
	// boot.iniд��ָ���ķ�����ĳ�����ֵ(INT)
	virtual void WriteBootInt(LPCTSTR AppName, LPCTSTR KeyName,int data,int datatype = DATA_DEC) = 0;
	// boot.ini��ȡָ���ķ�����ĳ�����ֵ(INT)
	virtual int ReadBootInt(LPCTSTR AppName, LPCTSTR KeyName, int defVal) = 0;
	// boot.ini��ȡָ���ķ�����ĳ�����ֵ(STRING)
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

/************** I2C ͨ��*******************/
public:
	// @brief������I2C������λ��SensorͨѶ��I2C���ߡ�
	//
	// @retval TRUE����λI2C�����ɹ�
	// @retval FALSE����λI2C����ʧ��
	virtual BOOL ResetI2CBus() = 0;
	virtual BOOL I2C_ReadSensor_Byte(WORD reg, BYTE* val)  = 0; // ��SENSORָ���Ĵ�����ȡһ��BYTE
	virtual BOOL I2C_ReadSensor_Word(WORD reg, WORD* val)  = 0; // ��SENSORָ���Ĵ�����ȡ����BYTE
	virtual BOOL I2C_WriteSensor_Byte(WORD reg, BYTE val)  = 0; // ��SENSORд��Ĵ���д��һ��BYTE
	virtual BOOL I2C_WriteSensor_Word(WORD reg, WORD val)  = 0; // ��SENSORд��Ĵ���д�����BYTE
	// DT
	virtual BOOL I2C_Read(UCHAR addr, USHORT reg, USHORT* val, BYTE mode = 0)  = 0;
	virtual BOOL I2C_Write(UCHAR addr, USHORT reg, USHORT val, BYTE mode = 0)  = 0;
	virtual BYTE I2C_MultiBytesRead(UCHAR Addr, USHORT reg, BYTE regsize, BYTE *ParaList, USHORT Length, int bStop = 0)  = 0;
	virtual BYTE I2C_MultiBytesWrite(UCHAR Addr, USHORT reg, BYTE regsize, BYTE *ParaList, USHORT Length)  = 0;

/************** �õ���ǰͼ��/֡��Ϣ *******************/
public:
	virtual ImageInfo GetImage() = 0;
/************** DC����ʾ  *******************/
public:
	virtual CDC* GetMemDC_Display() = 0;					// �õ���������DC
	virtual CDC* GetMemDC_Assistant() = 0;					// �õ����Դ���DC
	virtual void GetRect_Assistant(CRect* rect) = 0;		// �õ���������Rect
	virtual void GetRect_Display(CRect* rect) = 0;			// �õ����Դ���Rect
	virtual int  ShowMemDC(CDC* pMemDC) = 0;				// ��ʾDC
	virtual int  CopyBackGroundToMemDC(CDC* pMemDC) = 0;

/************** ������ʾ **************/

	virtual void AddLog(LPCTSTR m_info, COLORREF color = RGB(0,0,0), int FontSize = FONT_SIZE_DEFAULT) = 0;
	virtual void AddDebugLog(LPCTSTR m_info, COLORREF color = COLOR_GREY, int FontSize = FONT_SIZE_DEFAULT) = 0;	//ֻ��Debugģʽ�����Log
	virtual void ClearLog() = 0;			// ��յ�ǰLog��Ϣ
	virtual void AddFileLog(LPCTSTR lpszFileName, LPCTSTR lpszStr) = 0;	// ��D:\DccmLog�ļ����е�lpszFileName�ļ��У�׷��"[10/31/16 13:24:59] lpszStr"

/************** ·�� *****************/
public:
    virtual LPCTSTR GetExeDir() = 0;		// ��ȡ��ǰ������ļ���·�� Dir Path����'\'
	virtual LPCTSTR GetReportDir() = 0;		// ��ȡ��ǰ����ı���Report�ļ���·��������'\'
	virtual int		GetAppNumber() = 0;		// #[n]
	virtual LPCTSTR	GetConfigTitle() = 0;	// dccf name
	virtual LPCTSTR GetConfigPath() = 0;	// dccf Path

/************** �ع�  *******************/
public:
	virtual int Exposure(int target, int tolerance, int expType = 0, BOOL bShowExpInfo = TRUE, int cx = 50, int cy = 50, double length = 20, int framePitch = 2) = 0;

/************** ������˽�д洢�ռ� *******************/
	virtual vector<double>* GetMyStorage(LPVOID PT_THIS) = 0;  // ��ȡ��������Ŀ�Ĵ洢�ռ�,���� = THIS ָ��
	virtual vector<double>* GetStorage(int ID) = 0;				// �õ�ĳ��������Ŀ�Ĵ洢�ռ�
	virtual vector<double>* GetStorage(LPCTSTR lpszDllName) = 0;  // �õ�ĳ��������Ŀ�Ĵ洢�ռ�


/************** ͼ��ɼ��� *******************/
public:
	virtual BOOL SetVoltage(int channel,int voltage) = 0;  //sensor ��ѹ���� HV910D PE910D������
	virtual int RestartCamera(int index) = 0; //������index�ײ�����������ģ�顣
	virtual int PauseGrabFrame(void) = 0;     //��ͼ��ɼ���ֹͣץȡ֡
	virtual int ResumeGrabFrame(void) = 0;    //�ָ�ץ֡

	virtual BOOL bSensorEnable(BYTE data, BYTE enable) = 0;

    //�Զ���ParaList��������ȡ�����ռ�
	virtual int LoadParaList(LPCTSTR AppName, USHORT** ParaList, USHORT& ParaListSize) = 0;
	//������дI2C
	virtual BOOL bInitSensor(USHORT addr, USHORT* ParaList, USHORT Length, BYTE mode) = 0;
/************** ������չ *******************/
	// ����һ�ο�ʹһ��������Ŀ�����л������񻯵�ͼ����ͬʱ����ͼ���ʱ�����ԭʼ��RGBͼ��
	virtual void ShowOriginalImage(void) = 0;  

	// percentage Image rect convert to display rect
	virtual CRect CalcPercentRect_ImageToDisplay(double cx, double cy, double x_length, double y_length) = 0;// ʵ��ͼ����ο�ת��Ϊ���Դ��ڱ���
	// percentage Display rect convert to image rect
	virtual CRect CalcPercentRect_ToImage(double cx, double cy, double x_length, double y_length) = 0;		// ���Դ��ھ��ο����ת��Ϊʵ��ͼ��
	virtual CPoint CalcPoint_ImageToDisplay(CPoint imgPoint) = 0;		// Image point convert to display point
	virtual CPoint CalcPoint_DisplayToImage(CPoint disPoint) = 0;		// Display point convert to image point
	virtual CRect CalcRect_ImageToDisplay(CRect imgRect) = 0;			// Image rect convert to display point
	virtual CRect CalcRect_DisplayToImage(CRect disRect) = 0;			// Display point convert to image point

	virtual CRect CalcPercentRect_ImageToAssistant(double cx, double cy, double x_length, double y_length) = 0;// ʵ��ͼ����ο�ת��Ϊ���Դ��ڱ���
	virtual CPoint CalcPoint_ImageToAssistant(CPoint imgPoint) = 0;
	virtual CPoint CalcPoint_AssistantToImage(CPoint disPoint) = 0;
	virtual CRect CalcRect_ImageToAssistant(CRect imgRect) = 0;
	virtual CRect CalcRect_AssistantToImage(CRect disRect) = 0;


	//����
	virtual void StopExposure(void) = 0;

	//�ӽӿڻ��CamID
	virtual LPCTSTR GetCamID(void) = 0;
    //����CamID
	virtual void SetCamID(LPCTSTR ID) = 0;

	virtual void ShowRGBtoDisplayWnd(BYTE* pSrc, int width, int height) = 0;

	//�������ĵ�����
	virtual int DrawRect1(double cx, double cy, double x_length, double y_length, COLORREF color = RGB(0,0,255), int lineWidth = 1)=0;
	//0-100% ����
	virtual int DrawRect2(int l, int t, int r, int b, COLORREF color = RGB(0,0,255), int lineWidth = 1) = 0;
	//a b ��Բ ֱ��
	virtual int DrawEllipse(int x, int y, int a, int b, COLORREF color = RGB(0,0,255), int lineWidth = 1) =0;

	virtual int GetModuleNum(void) = 0;
	// pathName����Ҫ��Ӻ�׺��
	virtual void SaveImage(CString pathName, eSaveImageType sit) = 0;

	virtual void GetCurrent(double* ADValue) = 0;	 //��Interface��Working Current ��ȡ5·����
	virtual void GetVoltage(double* voltage) = 0;	 //��Interface����ȡ5·��ѹ
	virtual void GetCurrent_Standby(double* ADValue) = 0;	 //��Interface����ȡ3·��������
	virtual int  OpenShortTest(OSTab *pOSTab, int* OSValueArray) = 0;

	virtual int SendMsg(int CmdID = SM_INVLID, int Ack = SM_INVLID, int Result = SM_INVLID, CString ErrInfo = SM_INVLID_STR, CString TestData = SM_INVLID_STR) = 0;
	virtual int RecvMsg(int& CmdID, int& Status, int& Result, CString& ErrInfo, CString& TestData) = 0;
	virtual int GetRecvMsg(int& CmdID, int& Status, int& Result, CString& ErrInfo, CString& TestData) = 0;
};

