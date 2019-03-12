#pragma once

/*************************************************************/
//
// File Name	: ImageAcquistion_Interface.h
// Function		: Header File
// CharSet      : Unicode
// Author       : 
// Copyright (C) Nicolas Lee all rights reserved.
// 
/********************* Nicolas Lee Confidential ********************/

#pragma once

#ifdef IMAGE_ACQUISITION_EXPORTS
#define IMAGE_ACQUISITION_API __declspec(dllexport)
#else
#define IMAGE_ACQUISITION_API __declspec(dllimport)
#endif


#include <map>
#include <vector>
#include <string>
#include "DccmBase.h"
#include "dtccm2.h"
#pragma comment(lib, "dtccm2.lib")
#pragma comment(lib, "DtccmKit.lib")

using namespace std;
class DccmControl_Interface;

void WaitTime(unsigned short msec);

class ImageAcquisition
{
public:
	ImageAcquisition(void);
	virtual ~ImageAcquisition(void);

	virtual int OpenBoardCard();
	virtual int OpenBoardCard(wstring devName);
	virtual int OpenBoardCard(int devID);
	virtual int CloseBoardCard();

	virtual bool IsConnected(); // ���Ժ��Ƿ�������

	virtual int OpenCamera();
	virtual int CloseCamera();

	virtual int OpenShortTest(OSTab *pOSTab, int* OSValueArray);




	virtual wstring GetDllVersion(void);
	virtual wstring GetLibVersion(void);
	virtual wstring GetFWVersion(void);	// return firmware version �̼��汾��
	virtual wstring GetKitTypeName(void);	// return device type ���Ժ��ͺ�
	virtual wstring GetDeviceSN();

	virtual void LoadSensorTab(SensorTab* pSensortTab);
	virtual void LoadSoftPinDef(BYTE* softPinDef);
	virtual void LoadPowerSequence(SensorPowerOnSequence* sPowerOn, SensorPowerOffSequence* sPowerOff);
	
	// ����Ҫ֪��SENSOR TAB
	virtual bool ResetI2CBus();
	virtual bool I2C_ReadSensor_Byte(USHORT reg, UCHAR* val);
	virtual bool I2C_ReadSensor_Word(USHORT reg, USHORT* val); // USHORT == WORD
	virtual bool I2C_WriteSensor_Byte(USHORT reg, UCHAR val); // UCHAR == BYTE 
	virtual bool I2C_WriteSensor_Word(USHORT reg, USHORT val);

	// DT
	virtual bool I2C_Read(UCHAR addr, USHORT reg, USHORT* val, UCHAR mode = 0);
	virtual bool I2C_Write(UCHAR addr, USHORT reg, USHORT val, UCHAR mode = 0);   
	virtual BYTE I2C_MultiBytesRead(UCHAR Addr, USHORT reg, UCHAR regsize, UCHAR *ParaList, USHORT Length, int bStop = 0);
	virtual BYTE I2C_MultiBytesWrite(UCHAR Addr, USHORT reg, UCHAR regsize, UCHAR *ParaList, USHORT Length);

	virtual bool SetVoltage(int channel,int voltage);

	virtual void CloseSensorPower(void);
	virtual void GetVoltage(double* voltage);
	virtual void GetCurrent(double* ADValue);
	virtual void GetCurrent_Standby(double* ADValue);
	virtual void SoftPinPowerUp();
	virtual void SoftPinPoweroff();
	virtual int SetSensorPowerWithSeq(int bOnOff);
	virtual int SetSensorPower(int bOnOff);
	virtual void GetMIPIClkFrequency(ULONG* freq);

	virtual void SetDeviceID(int ID);
	virtual int EnumerateDevice(vector<wstring> &vDevName);
	virtual wstring GetCurDevName(void);

	virtual int  InitDTDevice(void);
	virtual int bGrabFrame(BYTE* inbuffer, ULONG buffersize);
	virtual int bSensorEnable(BYTE data, BYTE enable);

	virtual ULONG ReadSoftPin(void);

	virtual void SetDccmControl(DccmControl_Interface* pCtrl);
public:
	vector<wstring> m_vDevName;		// �رճ���֮ǰ��������
	SensorTab* m_pCurSensor;
	DWORD m_FWVer;	// �̼��汾
	HWND m_hMainWnd;
	bool m_isOpenDevice;  //���Ժ��Ƿ�������
	bool m_isInitDevice;
	BYTE* m_pPinDef;    //MIPI SOFT�ӿڶ���
	int m_nDevID;    // ����ʱ���趨��DeviceID

	wchar_t* m_IniPath;
	DccmControl_Interface* m_pCtrl;
};

// Factory function that creates instances of the Xyz object.  
extern "C"
{
	//�õ�DLL������IC�ͺ��嵥�����ָ�
	IMAGE_ACQUISITION_API const wchar_t* GetSupportListOfImageAcquisition(void);
	//ͨ��Ini��������ʵ��
	IMAGE_ACQUISITION_API ImageAcquisition* CreateImageAcquisition(const wchar_t* Ini_path);
	//ͨ��ָ�������� ��������ʵ��
	IMAGE_ACQUISITION_API ImageAcquisition* CreateImageAcquisitionWithName(const wchar_t* name, const wchar_t* Ini_path);
	IMAGE_ACQUISITION_API void DeleteImageAcquisition(ImageAcquisition* pImgAcquisition);


}
