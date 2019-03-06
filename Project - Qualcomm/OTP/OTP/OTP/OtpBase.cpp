#include "stdafx.h"

#include "OtpBase.h"


OtpSensorStruct COtpBase::m_otpSensor;
OtpPowerStruct COtpBase::m_otpPower;
tagOtpCtrl COtpBase::m_otpCtrl;

OtpIDStruct	COtpBase::m_IDGroup;

LencOtpControlStruct COtpBase::m_lencCtrl;
LencRegStruct COtpBase::m_lencReg;

tagAwbOtpCtrl COtpBase::m_wbCtrl;
WBGainRegStruct COtpBase::m_wbGainReg;
tagAwbRatio COtpBase::m_wbGolden;	
ExpControlStruct COtpBase::m_expCtrl;
ExpRegisterStruct COtpBase::m_expRegister;
ExpRangeStruct COtpBase::m_expRange;


CString COtpBase::SetOtpName(CString otpName)
{
	CString oldName = m_otpName;
	m_otpName = otpName;
	return oldName;
}

CString COtpBase::GetOtpName(void)
{
	return m_otpName;
}

OtpSensorStruct* COtpBase::GetOtpSensorParams(void)
{
	return &m_otpSensor;
}

OtpPowerStruct* COtpBase::GetOtpPowerParams(void)
{
	return &m_otpPower;
}

tagOtpCtrl* COtpBase::GetControlParams(void)
{
	return &m_otpCtrl;
}


BOOL COtpBase::CreateWBGroup(const tagAwbRatio *wbCurrent, const tagAwbRatio *wbGolden)
{
	return TRUE;
}

BOOL COtpBase::LoadWBSettings(tagAwbRatio *wbCurrent, tagAwbRatio *wbCurrentGolden)
{
	return TRUE;
}
void COtpBase::ShowUpdateInfoAWB(void)
{
}
BOOL COtpBase::CreateBasicGroup(const OtpIDStruct *ID)
{
	return TRUE;
}

BOOL COtpBase::LoadBasicSettings(OtpIDStruct *IDCurrent)
{
	return TRUE;
}

void COtpBase::ShowUpdateInfoBasic(void)
{

}
void COtpBase::SetEepromParams(EepromStruct *eeprom)
{
	m_Eeprom.deviceID = eeprom->deviceID;
	m_Eeprom.addrByteSize = eeprom->addrByteSize;
	m_Eeprom.pageSize = eeprom->pageSize;
	m_Eeprom.msWriteTime = eeprom->msWriteTime;
}
ExpControlStruct* COtpBase::GetExpControlParams(void) 
{
	return &m_expCtrl;
}

ExpRegisterStruct* COtpBase::GetExpRegister(void)				 
{
	return &m_expRegister;
}

ExpRangeStruct* COtpBase::GetExpRangeParams(void)
{
	return &m_expRange;
}

OtpIDStruct* COtpBase::GetIDGroup(void)
{
	return &m_IDGroup;
}

OtpIDStruct* COtpBase::GetIDCurrent(void)					 
{
	return &m_IDCurrent;
}


void COtpBase::SetSensorOB(BYTE OB)
{
	m_sensorOB = OB;
}

BYTE COtpBase::GetSensorOB(void) const	
{
	return m_sensorOB;
}

tagAwbOtpCtrl* COtpBase::GetWBControlParams(void)
{
	return &m_wbCtrl;
}

WBGainRegStruct* COtpBase::GetWBGainRegister(void)
{
	return &m_wbGainReg;
}		

tagAwbRatio* COtpBase::GetWBGoldenValue(void)					 
{
	return &m_wbGolden;
}

tagAwbRatio* COtpBase::GetWBCurrentGoldenValue(void)					 
{
	return &m_wbCurrentGolden;
}

tagAwbRatio* COtpBase::GetWBCurrentValue(void)					 
{
	return &m_wbCurrent;
}

LencOtpControlStruct* COtpBase::GetLencControlParams(void)
{
	return &m_lencCtrl;
}

LencRegStruct* COtpBase::GetLencRegister(void)
{
	return &m_lencReg;
}		

void COtpBase::EnableLenc(void)
{
	if (m_lencReg.addrCtrl != REG_INVALID)
	{
		i2c_write(m_lencReg.addrCtrl, m_lencReg.lencEnable);
	}
}

void COtpBase::DisableLenc(void)
{
	if (m_lencReg.addrCtrl != REG_INVALID)
	{
		i2c_write(m_lencReg.addrCtrl, m_lencReg.lencDisable);
	}
}

BOOL COtpBase::InitLSC(void)
{
	SAFE_DELETE_ARRAY(m_lencData);

	if (m_lencReg.dataSize != 0)
	{
		m_lencData = new BYTE[m_lencReg.dataSize];
		if (m_lencData == NULL)
		{
			return FALSE;
		}
		memset(m_lencData, 0, sizeof(m_lencData));	
	}

	return TRUE;
}

BOOL COtpBase::ProcLSC(void)
{
	if (!InitLSC())	return FALSE;
	BOOL ret = TRUE;

	if (m_lencCtrl.enDynamicInput)
		ret = GetDynamicLenc();
	else
	{
		if (!GetLSC(m_lencData, m_lencCtrl.inputTarget))	
			return FALSE;

		if (m_lencCtrl.enCheckLenc)
			ret = CheckLSC();
	}

// 	if (m_lencCtrl.enSaveLencFile && m_lencCtrl.enCheckLenc)
// 		SaveShadingFile();

	if (!ret)
		return FALSE;

	if (!CreateLencGroup(m_lencData))
		return FALSE;

	return TRUE;
}

BOOL COtpBase::GetLSC(BYTE *lencData, int inputTarget)
{
	return TRUE;
}

BOOL COtpBase::ApplyLSC(BYTE *data, BYTE *lencData)
{
	return TRUE;
}

BOOL COtpBase::ApplyLSC(const BYTE *lencData)
{
	if ((lencData != NULL) && (m_lencReg.addrDataStart != REG_INVALID))
	{
		for (USHORT i=0; i<m_lencReg.dataSize; i++)
		{
			i2c_write(m_lencReg.addrDataStart+i, lencData[i]);
		}
	}

	EnableLenc();

	m_pInterface->AddLog(_T("LSC Setting Applied"));
	return TRUE;
}

BOOL COtpBase::CheckLSC(void)
{
// 	ApplyLSC(m_lencData);
// 	BOOL ret = TestShading();
// 	DisableLenc();
// 	return ret;
	return TRUE;
}

BOOL COtpBase::GetDynamicLenc(void)
{
	for (int i=m_lencCtrl.inputTarget; i>=m_lencCtrl.inputLow; i--)
	{
		if (GetLSC(m_lencData, i) && CheckLSC())
		{
			return TRUE;
		}
	}

	for (int j=m_lencCtrl.inputTarget+1; j<=m_lencCtrl.inputHigh; j++)
	{
		if (GetLSC(m_lencData, j) && CheckLSC())
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL COtpBase::CreateLencGroup(const BYTE *lencData)
{
	return TRUE;
}

BOOL COtpBase::LoadLencSettings(BYTE *lencData)
{
	return TRUE;
}

void COtpBase::ShowUpdateInfoLSC(void)
{
}

BOOL COtpBase::UpdateLSC(void)
{
	if (!InitLSC())
	{
		return FALSE;
	}

	if (!LoadLencSettings(m_lencData))
	{
		return FALSE;
	}

	ShowUpdateInfoLSC();

	if (!CheckLSC())
	{
		return FALSE;
	}

	if (m_lencCtrl.enUpdateLenc)
	{
		if (!ApplyLSC(m_lencData))
		{
			return FALSE;
		}
	}

	return TRUE;
}

BYTE* COtpBase::GetLencData(void) const
{
	return m_lencData;
}


BOOL COtpBase::eeprom_write(USHORT addr, USHORT reg, BYTE val)
{
	BOOL ret = i2c_multi_write(addr, reg, m_Eeprom.addrByteSize, &val, 1);
	Sleep(m_Eeprom.msWriteTime);
	return ret;
}

BOOL COtpBase::eeprom_read(USHORT addr, USHORT reg, BYTE *pval)
{
	return i2c_multi_read(addr, reg, m_Eeprom.addrByteSize, pval, 1);
}

BOOL COtpBase::eeprom_multi_write(USHORT addr, USHORT reg, BYTE *pval, USHORT length)
{
	BOOL ret = TRUE;

	if (length < m_Eeprom.pageSize)
	{
		ret = i2c_multi_write(addr, reg, m_Eeprom.addrByteSize, pval, length);
		Sleep(m_Eeprom.msWriteTime);
		return ret;
	}

	BYTE *dst = pval;
	int num = length;

	if (reg % m_Eeprom.pageSize != 0)
	{
		int mod = m_Eeprom.pageSize - reg % m_Eeprom.pageSize;

		ret = i2c_multi_write(addr, reg, m_Eeprom.addrByteSize, dst, mod);
		Sleep(m_Eeprom.msWriteTime);

		dst += mod;
		reg += mod;

		num = length - mod;
	}

	int cycles = num / m_Eeprom.pageSize;

	for (int i=0; i<cycles; i++)
	{
		ret = i2c_multi_write(addr, reg, m_Eeprom.addrByteSize, dst, m_Eeprom.pageSize);
		Sleep(m_Eeprom.msWriteTime);

		reg += m_Eeprom.pageSize;
		dst += m_Eeprom.pageSize;
	}

	int mod = num - cycles * m_Eeprom.pageSize;
	if (mod != 0)
	{		
		ret = i2c_multi_write(addr, reg, m_Eeprom.addrByteSize, dst, mod);
		Sleep(m_Eeprom.msWriteTime);		
	}

	return ret;
}

BOOL COtpBase::eeprom_multi_read(USHORT addr, USHORT reg, BYTE *pval, USHORT length)
{
	if (length < m_Eeprom.pageSize)
	{
		return i2c_multi_read(addr, reg, m_Eeprom.addrByteSize, pval, length);
	}

	BOOL ret = TRUE;

	BYTE *dst = pval;
	int num = length;

	if (reg % m_Eeprom.pageSize != 0)
	{
		int mod = m_Eeprom.pageSize - reg % m_Eeprom.pageSize;

		ret = i2c_multi_read(addr, reg, m_Eeprom.addrByteSize, dst, mod);		
		dst += mod;
		reg += mod;

		num = length - mod;
	}

	int cycles = num / m_Eeprom.pageSize;

	for (int i=0; i<cycles; i++)
	{
		ret = i2c_multi_read(addr, reg, m_Eeprom.addrByteSize, dst, m_Eeprom.pageSize);		
		reg += m_Eeprom.pageSize;
		dst += m_Eeprom.pageSize;
	}

	int mod = num - cycles * m_Eeprom.pageSize;
	if (mod != 0)
	{		
		ret = i2c_multi_read(addr, reg, m_Eeprom.addrByteSize, dst, mod);
	}

	return ret;
}

BOOL COtpBase::eeprom_write(USHORT reg, BYTE val)
{
	return eeprom_write(m_Eeprom.deviceID, reg, val);
}

BOOL COtpBase::eeprom_read(USHORT reg, BYTE *pval)
{
	return eeprom_read(m_Eeprom.deviceID, reg, pval);
}

BOOL COtpBase::eeprom_multi_write(USHORT reg, BYTE *pval, USHORT length)
{
	return eeprom_multi_write(m_Eeprom.deviceID, reg, pval, length);
}

BOOL COtpBase::eeprom_multi_read(USHORT reg, BYTE *pval, USHORT length)
{
	return eeprom_multi_read(m_Eeprom.deviceID, reg, pval, length);
}
BOOL COtpBase::i2c_multi_read(USHORT addr, USHORT reg, BYTE regSize, BYTE *pval, USHORT length, BOOL noStop/*=FALSE*/)
{
	if ((pval == NULL))	return FALSE;

	for (int i=0; i<I2C_RETRY_NUM; i++)
	{
		if (m_pInterface->I2C_MultiBytesRead(addr, reg, regSize, pval, length, noStop))
			return TRUE;

		m_szStr.Format(_T("I2C Multi Read ERROR: %02X, %X, %d bytes"), addr, reg, length);
		m_pInterface->AddLog(m_szStr);

		Sleep(I2C_RETRY_WAIT);
	}

	m_szStr.Format(_T("I2C Multi Read ERROR: %02X, %X, %d bytes"), addr, reg, length);
	m_pInterface->AddLog(m_szStr, COLOR_RED);

	return FALSE;
}

BOOL COtpBase::i2c_write(USHORT reg, USHORT val)
{
	for (int i=0; i<I2C_RETRY_NUM; i++)
	{
		if (m_pInterface->I2C_WriteSensor_Byte(reg, val))
		{
			return TRUE;
		}

		CString info;
		info.Format(_T("I2C Write ERROR: %X, %X"), reg, val);
		m_pInterface->AddLog(info);		

		Sleep(I2C_RETRY_WAIT);
	}

	CString info;
	info.Format(_T("I2C Write ERROR: %X, %X"), reg, val);
	m_pInterface->AddLog(info, COLOR_RED);		

	return FALSE;
}

BOOL COtpBase::i2c_read(USHORT reg, USHORT *pval)
{
	if ((pval == NULL))
	{
		return FALSE;
	}

	for (int i=0; i<I2C_RETRY_NUM; i++)
	{
		if (m_pInterface->I2C_ReadSensor_Word(reg, pval))
		{
			return TRUE;
		}

		CString info;
		info.Format(_T("I2C Read ERROR: %X"), reg);
		m_pInterface->AddLog(info);

		Sleep(I2C_RETRY_WAIT);
	}

	CString info;
	info.Format(_T("I2C Read ERROR: %X"), reg);
	m_pInterface->AddLog(info, COLOR_RED);

	return FALSE;
}

BOOL COtpBase::i2c_multi_write(USHORT reg, BYTE *pval, USHORT length)
{
	if ((pval == NULL))
	{
		return FALSE;
	}

	for (int i=0; i<I2C_RETRY_NUM; i++)
	{
		if (i2c_multi_write(reg, pval, length))
		{
			return TRUE;
		}

		CString info;
		info.Format(_T("I2C Multi Write ERROR: %X, %d bytes"), reg, length);
		m_pInterface->AddLog(info);

		Sleep(I2C_RETRY_WAIT);
	}

	CString info;
	info.Format(_T("I2C Multi Write ERROR: %X, %d bytes"), reg, length);
	m_pInterface->AddLog(info, COLOR_RED);

	return FALSE;
}

BOOL COtpBase::i2c_multi_write(USHORT addr, USHORT reg, BYTE regSize, BYTE *pval, USHORT length)
{
	if ( (pval == NULL))
	{
		return FALSE;
	}

	for (int i=0; i<I2C_RETRY_NUM; i++)
	{
		if (m_pInterface->I2C_MultiBytesWrite(addr, reg, regSize, pval, length))
		{
			return TRUE;
		}

		CString info;
		info.Format(_T("I2C Multi Write ERROR: %02X, %X, %d bytes"), addr, reg, length);
		m_pInterface->AddLog(info);

		Sleep(I2C_RETRY_WAIT);
	}

	CString info;
	info.Format(_T("I2C Multi Write ERROR: %02X, %X, %d bytes"), addr, reg, length);
	m_pInterface->AddLog(info, COLOR_RED);

	return FALSE;
}

BOOL COtpBase::i2c_multi_read(USHORT reg, BYTE *pval, USHORT length, BOOL noStop/*=FALSE*/)
{
	if ((pval == NULL))
	{
		return FALSE;
	}

	for (int i=0; i<I2C_RETRY_NUM; i++)
	{
		if (i2c_multi_read(reg, pval, length, noStop))
		{
			return TRUE;
		}

		CString info;
		info.Format(_T("I2C Multi Read ERROR: %X, %d bytes"), reg, length);
		m_pInterface->AddLog(info);		

		Sleep(I2C_RETRY_WAIT);
	}

	CString info;
	info.Format(_T("I2C Multi Read ERROR: %X, %d bytes"), reg, length);
	m_pInterface->AddLog(info, COLOR_RED);		

	return FALSE;
}

int COtpBase::CalcCheckSum(BYTE *data, int num, int mod)
{
	ASSERT(data != NULL);

	ULONG  sum = 0;

	for (int i=0; i<num; i++)
	{
		sum += data[i];
	}

	return sum % mod;
}

BOOL COtpBase::InitProgram(void)
{
	PreOtp();

	if (CheckOtpEmpty() || m_otpCtrl.enForceCalibrate)
	{
	
	}
	else
	{				
		UpdateOtp();
	}

	return TRUE;
}

BOOL COtpBase::PreOtp(void)					 
{
	if (m_otpCtrl.enWB)
	{
		SetWBDefaultGain();	
	}

	if (m_otpCtrl.enLenc)
	{
		DisableLenc();
	}

	return TRUE;
}

BOOL COtpBase::StartProgram()
{
	if (m_otpCtrl.enLenc)
	{
		if (!ProcLSC())
		{
			m_pInterface->AddLog(_T("Lenc Proc ERROR"), COLOR_RED);
			return FALSE;
		}
	}

	if (m_otpCtrl.enWB)
	{
		if (!ProcAWB())
		{
			m_pInterface->AddLog(_T("WB Proc ERROR"), COLOR_RED);
			return FALSE;
		}
	}

// 	if (m_otpCtrl.enAF)
// 	{
// 		if (!CreateAFGroup())
// 		{
// 			m_pInterface->AddLog(_T("AF Proc ERROR"), COLOR_RED);
// 			return FALSE;
// 		}
// 	}

	if (!CreateBasicGroup(&m_IDGroup))
	{
		return FALSE;
	}

	BOOL bRet = OtpWrite();
	return TRUE;
}

BOOL COtpBase::UpdateOtp(void)
{
	BOOL bRet = TRUE;

	bRet &= UpdateBasic();
	bRet &= UpdateAWB();
	bRet &= UpdateLSC();
	bRet &= UpdateAF();

	return bRet;
}

BOOL COtpBase::UpdateBasic()
{
	if (!InitBasic())
	{
		return FALSE;
	}

	if (!LoadBasicSettings(&m_IDCurrent))
	{
		return FALSE;
	}

	ShowUpdateInfoBasic();

	if (!CheckBasic())
	{
		return FALSE;
	}

	return TRUE;
}
BOOL COtpBase::UpdateAWB()
{
	if (!InitAWB())
	{
		return FALSE;
	}

	if (!LoadWBSettings(&m_wbCurrent, &m_wbCurrentGolden))
	{
		return FALSE;
	}

	ShowUpdateInfoAWB();

	if (!CheckAWB())
	{
		return FALSE;
	}

	if (m_wbCtrl.enUpdateWB)
	{
		if (!ApplyAWB(&m_wbCurrent, &m_wbCurrentGolden))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL COtpBase::UpdateAF()
{
	return TRUE;
}

USHORT COtpBase::WBRatioToRegGain(double ratio)
{
	return USHORT(m_wbGainReg.dataDefaultGain * ratio);
}

void COtpBase::SetWBDefaultGain(void)					 
{
	USHORT gainDefault = m_wbGainReg.dataDefaultGain;
	SetWBGain(gainDefault, gainDefault, gainDefault, gainDefault);
}

void COtpBase::SetWBGain(USHORT gainGr, USHORT gainR, USHORT gainB, USHORT gainGb)
{
 	WriteWBReg(m_wbGainReg.addrGrHigh, m_wbGainReg.addrGrLow, gainGr);
	WriteWBReg(m_wbGainReg.addrRHigh,  m_wbGainReg.addrRLow,  gainR);
	WriteWBReg(m_wbGainReg.addrBHigh,  m_wbGainReg.addrBLow,  gainB);
	WriteWBReg(m_wbGainReg.addrGbHigh, m_wbGainReg.addrGbLow, gainGb);
}

void COtpBase::WriteWBReg(USHORT addrHigh, USHORT addrLow, USHORT gain)
{
	//HIBYTE
	if ((addrHigh != REG_INVALID) && (addrLow != REG_INVALID))
	{
		i2c_write(addrHigh, HIBYTE(gain));
		i2c_write(addrLow,  LOBYTE(gain));
	}
	else if (addrHigh != REG_INVALID)
	{
		i2c_write(addrHigh, gain);
	}
	else if (addrLow != REG_INVALID)
	{
		i2c_write(addrLow, gain);
	}
	
}

BOOL COtpBase::ProcAWB(void)
{
	if (!InitAWB())
	{
		return FALSE;
	}

	if (m_wbCtrl.enLencBasedWB)
	{
		ApplyLSC(m_lencData);	
	}


	if (!GetAWB(&m_wbCurrent))
	{
		return FALSE;
	}

	if (m_wbCtrl.enCheckWB)
	{
		if (!CheckWBDifference())
		{
			return FALSE;
		}
		if(!CheckWBCurrentValue())
		{
			return FALSE;
		}
	}	

	if (!CreateWBGroup(&m_wbCurrent, &m_wbGolden))
	{
		return FALSE;
	}	

// 	if (m_wbCtrl.enSaveWBFile)
// 	{
// 		SaveWBFile();
// 	}	

	return FALSE;
}

BOOL COtpBase::InitAWB(void)
{
	memcpy(&m_wbCurrentGolden, &m_wbGolden, sizeof(m_wbCurrentGolden));

	memset(&m_wbCurrent, 0, sizeof(m_wbCurrent));
	memset(&m_wbCorrect, 0, sizeof(m_wbCorrect));

	ImageInfo imgInfo = m_pInterface->GetImage();
	if (m_wbCtrl.roiCalcMode != ROI_CALC_USER)
	{
		SetRoi(m_wbCtrl.roi, imgInfo.width, imgInfo.height, m_wbCtrl.roiCalcMode);
	}
	FormatRoiRect(m_wbCtrl.roi, imgInfo.width, imgInfo.height);

	return TRUE;
}
BOOL COtpBase::ApplyAWB(const tagAwbRatio *wbCurrent, const tagAwbRatio *wbGolden)
{
	UINT goldenRG = wbGolden->rg;
	UINT goldenBG = wbGolden->bg;

	UINT currentRG = wbCurrent->rg;
	UINT currentBG = wbCurrent->bg;

	if ((goldenRG==0) || (goldenBG==0) || (currentRG==0) || (currentBG==0))
	{
		m_pInterface->AddLog(_T("Golden Value can not be Zero"), COLOR_RED);
		return FALSE;
	}

	double ratioR = 1.0;
	double ratioG = 1.0;
	double ratioB = 1.0;

	if (!GetWBGainRatio(goldenRG, goldenBG, currentRG, currentBG, ratioR, ratioG, ratioB))
	{
		m_pInterface->AddLog(_T("GetWBGainRatio ERROR"), COLOR_RED);
		return FALSE;
	}

	CString info;
	info.Format(_T("ratioR=%.2lf\r\nratioG=%.2lf\r\nratioB=%.2lf"), ratioR, ratioG, ratioB);
	m_pInterface->AddLog(info);

	// Change sensor WB gain to register settings
	USHORT gainR = WBRatioToRegGain(ratioR);
	USHORT gainG = WBRatioToRegGain(ratioG);
	USHORT gainB = WBRatioToRegGain(ratioB);	

	info.Format(_T("gainR=0x%04X\r\ngainG=0x%04X\r\ngainB=0x%04X"), gainR, gainG, gainB);
	m_pInterface->AddLog(info);

	// Write sensor WB gain to registers
	SetWBGain(gainG, gainR, gainB, gainG);

	m_pInterface->AddLog(_T("WB Setting Applied"));
	return TRUE;
}

BOOL COtpBase::GetAWB(tagAwbRatio* wb)
{
	// getwb
	ImageInfo imgInfo = m_pInterface->GetImage();
	int width = imgInfo.width;
	int height= imgInfo.height;

	wb->r = ImageProc::GetAveR_ROI_Raw(imgInfo.Raw_Buf, imgInfo.width, imgInfo.height, m_wbCtrl.roi, imgInfo.RawFormat);
	wb->g = ImageProc::GetAveG_ROI_Raw(imgInfo.Raw_Buf, imgInfo.width, imgInfo.height, m_wbCtrl.roi, imgInfo.RawFormat);
	wb->b = ImageProc::GetAveB_ROI_Raw(imgInfo.Raw_Buf, imgInfo.width, imgInfo.height, m_wbCtrl.roi, imgInfo.RawFormat);
	wb->gb = ImageProc::GetAveGb_ROI_Raw(imgInfo.Raw_Buf, imgInfo.width, imgInfo.height, m_wbCtrl.roi, imgInfo.RawFormat);
	wb->gr = ImageProc::GetAveGr_ROI_Raw(imgInfo.Raw_Buf, imgInfo.width, imgInfo.height, m_wbCtrl.roi, imgInfo.RawFormat);

	int sensorOB = 64;
	int amplify_factor = 1024;	// 增加系数
	wb->rg		= (UINT)(1.0 * wb->r / wb->g*amplify_factor + 0.5);
	wb->bg		= (UINT)(1.0 * wb->b / wb->g*amplify_factor + 0.5);
	wb->gb_gr	= (UINT)(1.0 * wb->gb/ wb->gr*amplify_factor+ 0.5);

	m_szStr.Format(_T("R/G: %d B/G: %d"), wb->rg, wb->bg);
	m_pInterface->AddLog(m_szStr, COLOR_BLUE);
	return TRUE;
}

BOOL COtpBase::CheckWBDifference(void)
{	
	ApplyAWB(&m_wbCurrent, &m_wbGolden);

	if (!GetAWB(&m_wbCorrect))	return FALSE;

	SetWBDefaultGain();

	float rgDif = (1.0f * m_wbCorrect.rg / m_wbGolden.rg - 1) * 100;
	float bgDif = (1.0f * m_wbCorrect.bg / m_wbGolden.bg - 1) * 100;

	m_szStr.Format(_T("rgDif=%.2f, bgDif=%.2f"), rgDif, bgDif);
	m_pInterface->AddLog(m_szStr);

	if (fabs(rgDif) > m_wbCtrl.percentDifRG || fabs(bgDif) > m_wbCtrl.percentDifBG)
	{
		m_pInterface->AddLog(_T("WB Difference 超出范围"), COLOR_RED);
		return FALSE;
	}

	return TRUE;
}

BOOL COtpBase::CheckWBCurrentValue(void)
{
	if ((m_wbCurrent.rg==0) || (m_wbCurrent.bg==0))
	{
		m_pInterface->AddLog(_T("Current Value can not be Zero"), COLOR_RED);
		return FALSE;
	}

	float rgDif = (1.0f * m_wbCurrent.rg / m_wbGolden.rg - 1) * 100;
	float bgDif = (1.0f * m_wbCurrent.bg / m_wbGolden.bg - 1) * 100;

	float gDif = 0;
	if (m_wbGolden.gb_gr != 0)
	{
		gDif = (1.0f * m_wbCurrent.gb_gr / m_wbGolden.gb_gr - 1) * 100;
	}

	CString info;
	info.Format(_T("OriginDif_RG=%.2f"), rgDif);
	m_pInterface->AddLog(info);

	info.Format(_T("OriginDif_BG=%.2f"), bgDif);
	m_pInterface->AddLog(info);

	info.Format(_T("OriginDif_GbGr=%.2f"), gDif);
	m_pInterface->AddLog(info);

	if (fabs(rgDif) > m_wbCtrl.originDif || fabs(bgDif) > m_wbCtrl.originDif  || fabs(gDif) > m_wbCtrl.originDif)
	{

		m_pInterface->AddLog(_T("WB Current Value ERROR"), COLOR_RED);
		//bIsInvalidWB = TRUE;
		return FALSE;//0718
	}	

	return TRUE;
}

void COtpBase::SetRoi(CRect &roi, UINT width, UINT height, BYTE roiCalcMode, UINT x/*=0*/, UINT y/*=0*/, UINT roiWidth/*=0*/, UINT roiHeight/*=0*/)
{
	if (roiCalcMode == ROI_CALC_USER)
	{
		roi.left = x;
		roi.top  = y;
		roi.right  = x + roiWidth;
		roi.bottom = y + roiHeight;

		FormatRoiRect(roi, width, height);
		return;
	}

	switch (roiCalcMode)
	{
	case ROI_CALC_CENTER:
		roiWidth  = width;
		roiHeight = 2;
		break;

	case ROI_CALC_1:		
	case ROI_CALC_2:	
	case ROI_CALC_4:		
	case ROI_CALC_5:		
	case ROI_CALC_8:			
	case ROI_CALC_10:		
		roiWidth  = width  / roiCalcMode;     
		roiHeight = height / roiCalcMode;     
		break;

	default:
		break;
	}
	roi.left = (width - roiWidth) / 2;
	roi.top  = (height - roiHeight) / 2;
	roi.right = roi.left + roiWidth;
	roi.bottom = roi.top + roiHeight;

	FormatRoiRect(roi, width, height);
}


void COtpBase::FormatRoiRect(CRect &rt, UINT width, UINT height)
{
	if (rt.left > (int)width)
	{
		rt.left = width - 2;
	}

	if (rt.top > (int)height)
	{
		rt.top = height - 2;
	}

	if (rt.left + rt.Width() > (int)width)
	{
		rt.right = width - 1;
	}

	if (rt.top + rt.Height() > (int)height)
	{
		rt.bottom = height - 1;
	}

	if ((rt.left % 2) != 0)
	{
		rt.left--;
	}

	if ((rt.top % 2) != 0)
	{
		rt.top--;
	}

	if ((rt.Width() % 2) != 0)
	{
		rt.right--;
	}

	if ((rt.Height() % 2) != 0)
	{
		rt.bottom--;
	}	
}




BOOL COtpBase::GetWBGainRatio(UINT goldenRG, UINT goldenBG, UINT currentRG, UINT currentBG, double &ratioR, double &ratioG, double &ratioB)
{
	// Calculate R, G, B gain of current module
	ratioR = 1.0 * goldenRG / currentRG;
	ratioB = 1.0 * goldenBG / currentBG;
	ratioG = 1.0;

	if (m_wbCtrl.gainCalcMode == WB_CALC_SINGLE_RB)
	{
		return TRUE;
	}
	else if (m_wbCtrl.gainCalcMode == WB_CALC_BASE_MIN)
	{
		double ratioBase;

		// ratioBase is min(ratioR, ratioG, ratioB)
		if ((ratioR > ratioG) && (ratioB > ratioG))
		{
			// R/G < R/G golden, B/G < B/G golden
			ratioBase = ratioG;
		}
		else if (ratioR > ratioB)
		{
			// B/G >= B/G golden, R/G < R/G golden
			ratioBase = ratioB;
		}
		else
		{
			// R/G >= R/G golden, B/G < B/G golden
			ratioBase = ratioR;
		}

		ratioR = ratioR / ratioBase;	
		ratioG = ratioG / ratioBase;
		ratioB = ratioB / ratioBase;
	}		

	return TRUE;
}

BOOL COtpBase::InitBasic(void)
{
	memset(&m_IDCurrent, 0, sizeof(m_IDCurrent));	
	return TRUE;
}

BOOL COtpBase::CheckBasic(void)
{
	if (!CheckEqual((BYTE*)&m_IDCurrent, (BYTE*)&m_IDGroup, sizeof(m_IDGroup)))
	{
		m_pInterface->AddLog(_T("ID ERROR"), COLOR_RED);
		return FALSE;
	}
	return TRUE;
}

BOOL COtpBase::CheckEqual(BYTE *data1, BYTE *data2, int num)
{
	if (data1 == NULL || data2 == NULL)
	{
		return FALSE;
	}

	if (num <= 0)
	{
		return FALSE;
	}	

	for (int i=0; i<num; i++)
	{
		if (data1[i] != data2[i])
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL COtpBase::CheckAWB(void)
{
	if (CheckWBDifference() && CheckWBGoldenValue() && CheckWBCurrentValue())
		return TRUE;

	return FALSE;
}

BOOL COtpBase::CheckWBGoldenValue(void)
{
	if ((m_wbCurrentGolden.rg==0) || (m_wbCurrentGolden.bg==0))
	{
		m_pInterface->AddLog(_T("Golden Value can not be Zero"), COLOR_RED);
		return FALSE;
	}

	if (!CheckEqual((BYTE*)&m_wbCurrentGolden, (BYTE*)&m_wbGolden, sizeof(m_wbGolden)))
	{
		m_pInterface->AddLog(_T("WB Golden Value ERROR"), COLOR_RED);
		return FALSE;
	}

	//	m_pInterface->AddLog(_T("WB Golden Value PASS"), LOG_NORMAL);
	return TRUE;
}