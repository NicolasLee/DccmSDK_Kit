#include "StdAfx.h"
#include "OtpCheck_CMK8440.h"


OtpCheck_CMK8440::OtpCheck_CMK8440(void)
{
	m_flag.empty = 0x00;
	m_flag.validGroup1 = 0x40;
	m_flag.validGroup2 = 0xd0;
	m_flag.validGroup3 = UNEXCEPT_VALUE;

	m_group.empty   = 0;
	m_group.invalid  = -1;
	m_group.group1  = 1;
	m_group.group2 = 2;
	m_group.group3 = UNEXCEPT_VALUE;

	m_addrFlag.basic	= UNEXCEPT_REG;		
	m_addrFlag.awb			= 0x7010;		
	m_addrFlag.lsc			= 0x7028;		
	m_addrFlag.af			= 0x7021;		
	m_addrFlag.pdaf1			= UNEXCEPT_REG;		
	m_addrFlag.pdaf2= UNEXCEPT_REG;		

	m_addrSum.basic	= UNEXCEPT_REG;		
	m_addrSum.awb			= UNEXCEPT_REG;		
	m_addrSum.lsc			= 0x7119;		
	m_addrSum.af			= UNEXCEPT_REG;		 
	m_addrSum.pdaf1		= UNEXCEPT_REG;		
	m_addrSum.pdaf2		= UNEXCEPT_REG;

	m_otpPage.basic	= UNEXCEPT_VALUE;
	m_otpPage.awb			= UNEXCEPT_VALUE;
	m_otpPage.lsc			= UNEXCEPT_VALUE;	
	m_otpPage.af			= UNEXCEPT_VALUE;
	m_otpPage.pdaf1		= UNEXCEPT_VALUE;	
	m_otpPage.pdaf1_End = UNEXCEPT_VALUE;
	m_otpPage.pdaf2		= UNEXCEPT_VALUE;	
	m_otpPage.pdaf2_End = UNEXCEPT_VALUE;

	m_addrAWB .RGL = 0x7018;
	m_addrAWB. RGH = 0x7016;
	m_addrAWB .BGL = 0x7018;
	m_addrAWB. BGH = 0x7017;
	m_addrAWB .GGL = UNEXCEPT_REG;
	m_addrAWB. GGH = UNEXCEPT_REG;

	m_addrAWB .RL = UNEXCEPT_REG;
	m_addrAWB. RH = UNEXCEPT_REG;
	m_addrAWB .GrL = UNEXCEPT_REG;
	m_addrAWB. GrH = UNEXCEPT_REG;
	m_addrAWB .GbL = UNEXCEPT_REG;
	m_addrAWB. GbH = UNEXCEPT_REG;
	m_addrAWB .BL = UNEXCEPT_REG;
	m_addrAWB. BH = UNEXCEPT_REG;

	m_addrAWB .GOLDRGL = UNEXCEPT_REG;
	m_addrAWB .GOLDRGH = UNEXCEPT_REG;
	m_addrAWB .GOLDBGL = UNEXCEPT_REG;
	m_addrAWB .GOLDBGH = UNEXCEPT_REG;
	m_addrAWB .GOLDGGL = UNEXCEPT_REG;
	m_addrAWB .GOLDGGH = UNEXCEPT_REG;

	m_addrAWB .GOLDRL = UNEXCEPT_REG;
	m_addrAWB .GOLDRH = UNEXCEPT_REG;
	m_addrAWB .GOLDGrL = UNEXCEPT_REG;
	m_addrAWB .GOLDGrH = UNEXCEPT_REG;
	m_addrAWB .GOLDGbL = UNEXCEPT_REG;
	m_addrAWB .GOLDGbH = UNEXCEPT_REG;
	m_addrAWB .GOLDBL = UNEXCEPT_REG;
	m_addrAWB .GOLDBH = UNEXCEPT_REG;

	m_afAddrCode.stcL			= UNEXCEPT_VALUE;
	m_afAddrCode.stcH			= UNEXCEPT_VALUE;
	m_afAddrCode.infL			= 0x7024;
	m_afAddrCode.infH			= 0x7022;
	m_afAddrCode.macL		= 0x7024;
	m_afAddrCode.macH		= 0x7023;

	m_bCheckStc = FALSE;	// 起始Code
	m_bCheckInf = TRUE;	// 远景Code
	m_bCheckMac = TRUE;	// 近景Code

	m_gapsize.basic = UNEXCEPT_VALUE;
	m_gapsize.awb = 8;
	m_gapsize.lsc = 241;
	m_gapsize.af = 3;
	m_gapsize.pdaf1 = UNEXCEPT_VALUE;
	m_gapsize.pdaf2 = UNEXCEPT_VALUE;

	m_otpSize.basic	= m_addrSum.basic - m_addrFlag.basic - 1;
	m_otpSize.awb			= m_addrSum.awb - m_addrFlag.awb - 1;
	m_otpSize.lsc			= m_addrSum.lsc - m_addrFlag.lsc - 1;
	m_otpSize.af			= m_addrSum.af	- m_addrFlag.af - 1;
	m_otpSize.pdaf1		= 496;
	m_otpSize.pdaf2		= 876;

	m_calcCheckSum.divisor = 255;
	m_calcCheckSum.addend = 1;
}


OtpCheck_CMK8440::~OtpCheck_CMK8440(void)
{
}

int OtpCheck_CMK8440::CheckGroup(BYTE flag)
{
	if (flag == m_flag.empty)
	{
		return m_group.empty; //group 为空
	}
	else if (flag == m_flag.validGroup1)
	{
		return m_group.group1;
	}
	else if (flag == m_flag.validGroup2)
	{
		return m_group.group2;
	}
	else
	{
		return m_group.invalid;//无效group定义为-1
	}
}

int OtpCheck_CMK8440::CheckSum(BOOL flag,int flagaddr, int sumaddr, int datasize, int gap, CString str, int validgroup)//检测多组checksum
{
	if (!flag) return FUNCTION_PASS; //没勾选检测对应checksum项则直接返回真

	// Calc sum
	BYTE* pVal = new BYTE[datasize];
	Read(flagaddr+1+(validgroup-1)*gap,pVal, datasize);//获取计算checksum的数据
	BYTE calcVal = SumCalc(pVal, datasize,m_calcCheckSum.divisor,m_calcCheckSum.addend);//计算checksum

	// Read sum
	BYTE readVal = 0;
	Read_BYTE(sumaddr+(validgroup-1)*gap,&readVal);//读取已烧录checksum

	if (readVal != calcVal)//对比检查checksum是否正确
	{
		m_szInfo.Format(L"%s read sum 0x%02x != calc sum 0x%02x", str, readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		SAFE_DELETE_ARRAY(pVal);
		return FUNCTION_FAIL;
	}
	SAFE_DELETE_ARRAY(pVal);

	return FUNCTION_PASS;
}

void OtpCheck_CMK8440::SaveOTPData()//点亮后读取模组内的OTP数据
{
	m_pInterface->AddLog(_T("Read OTP Start!"));
	///////////////////////////////Read OTP////////////////////////////////////
	int valSize = 507;
	BYTE* pVal = new BYTE[valSize];
	Read(0x7010, pVal, valSize);

	//////////////////////////////////////////////////////////////////////////
	CString DirPath = m_pInterface->GetExeDir();
	CString Path ;
	Path.Format(_T("OTP Data_%d.txt"),m_pInterface->GetModuleNum());
	Path = DirPath+Path;
	FILE* file = NULL;

	CStringA temp, num;
	for (int i = 0; i<valSize; i++)
	{
		num.Format("%04d\t0x%04x,0x%02x\n", i+1,i+0x7010,(unsigned char)pVal[i]);
		temp += num;
	}

	char* p = temp.GetBuffer();
	if (_wfopen_s(&file,Path.GetBuffer(),_T("wt+"))==0)
	{
		fwrite(p,1,strlen(p),file);
		fclose(file);
		Path = Path+_T(" saved");
		m_pInterface->AddLog(Path,COLOR_BLUE);
	}

	m_pInterface->AddLog(_T("Read OTP End!"));
	return;
}


int OtpCheck_CMK8440::CheckValue()//检查AWB值是否满足设定范围
{
	if (m_addrAWB.RGH == UNEXCEPT_REG)
	{
		m_valueAWB.RG = m_valueAWB.RGL;
	}
	else
	{
		m_valueAWB.RG = (m_valueAWB.RGH<<2) |((m_valueAWB.RGL&0xc0)>>6);
	}

	if (m_addrAWB.BGH == UNEXCEPT_REG)
	{
		m_valueAWB.BG = m_valueAWB.BGL;
	}
	else
	{
		m_valueAWB.BG = (m_valueAWB.BGH<<2) |((m_valueAWB.BGL&0x30)>>4);
	}

	if (m_addrAWB.GGH == UNEXCEPT_REG)
	{
		m_valueAWB.GG = m_valueAWB.GGL;
	}
	else
	{
		m_valueAWB.GG = m_valueAWB.GGH<<8 |m_valueAWB.GGL;
	}

	if (m_addrAWB.RH == UNEXCEPT_REG)
	{
		m_valueAWB.R = m_valueAWB.RL;
	}
	else
	{
		m_valueAWB.R = m_valueAWB.RH<<8 |m_valueAWB.RL;
	}

	if (m_addrAWB.BH == UNEXCEPT_REG)
	{
		m_valueAWB.B = m_valueAWB.BL;
	}
	else
	{
		m_valueAWB.B = m_valueAWB.BH<<8 |m_valueAWB.BL;
	}

	if (m_addrAWB.GrH == UNEXCEPT_REG)
	{
		m_valueAWB.Gr = m_valueAWB.GrL;
	}
	else
	{
		m_valueAWB.Gr = m_valueAWB.GrH<<8 |m_valueAWB.GrL;
	}

	if (m_addrAWB.GbH == UNEXCEPT_REG)
	{
		m_valueAWB.Gb = m_valueAWB.GbL;
	}
	else
	{
		m_valueAWB.Gb = m_valueAWB.GbH<<8 |m_valueAWB.GbL;
	}

	if (m_addrAWB.GOLDRGH == UNEXCEPT_REG)
	{
		m_valueAWB.GOLDRG = m_valueAWB.GOLDRGL;
	}
	else
	{
		m_valueAWB.GOLDRG = m_valueAWB.GOLDRGH<<8 |m_valueAWB.GOLDRGL;
	}

	if (m_addrAWB.GOLDBGH == UNEXCEPT_REG)
	{
		m_valueAWB.GOLDBG = m_valueAWB.GOLDBGL;
	}
	else
	{
		m_valueAWB.GOLDBG = m_valueAWB.GOLDBGH<<8 |m_valueAWB.GOLDBGL;
	}

	if (m_addrAWB.GOLDGGH == UNEXCEPT_REG)
	{
		m_valueAWB.GOLDGG = m_valueAWB.GOLDGGL;
	}
	else
	{
		m_valueAWB.GOLDGG = m_valueAWB.GOLDGGH<<8 |m_valueAWB.GOLDGGL;
	}

	if (m_addrAWB.GOLDRH == UNEXCEPT_REG)
	{
		m_valueAWB.GOLDR = m_valueAWB.GOLDRL;
	}
	else
	{
		m_valueAWB.GOLDR = m_valueAWB.GOLDRH<<8 |m_valueAWB.GOLDRL;
	}

	if (m_addrAWB.GOLDBH == UNEXCEPT_REG)
	{
		m_valueAWB.GOLDB = m_valueAWB.GOLDBL;
	}
	else
	{
		m_valueAWB.GOLDB = m_valueAWB.GOLDBH<<8 |m_valueAWB.GOLDBL;
	}

	if (m_addrAWB.GOLDGrH == UNEXCEPT_REG)
	{
		m_valueAWB.GOLDGr = m_valueAWB.GOLDGrL;
	}
	else
	{
		m_valueAWB.GOLDGr = m_valueAWB.GOLDGrH<<8 |m_valueAWB.GOLDGrL;
	}

	if (m_addrAWB.GOLDGbH == UNEXCEPT_REG)
	{
		m_valueAWB.GOLDGb = m_valueAWB.GOLDGbL;
	}
	else
	{
		m_valueAWB.GOLDGb = m_valueAWB.GOLDGbH<<8 |m_valueAWB.GOLDGbL;
	}

	if (m_bCheckRG)
	{
		if (m_valueAWB.RG<m_RGLower || m_valueAWB.RG>m_RGUpper)
		{
			m_szInfo.Format(L"RG %d 不在范围 (%d~%d)", m_valueAWB.RG, m_RGLower, m_RGUpper);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL; 
		}
	}

	if (m_bCheckBG)
	{
		if (m_valueAWB.BG<m_BGLower ||m_valueAWB.BG>m_BGUpper)
		{
			m_szInfo.Format(L"BG %d 不在范围 (%d~%d)", m_valueAWB.BG, m_BGLower, m_BGUpper);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL; 
		}
	}

	if (m_bCheckGG)
	{
		if (m_valueAWB.GG<m_GGLower ||m_valueAWB.GG>m_GGUpper)
		{
			m_szInfo.Format(L"GG %d 不在范围 (%d~%d)", m_valueAWB.GG, m_GGLower, m_GGUpper);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL; 
		}
	}

	if (m_bCheckR)
	{
		if (m_valueAWB.R<m_RLower || m_valueAWB.R>m_RUpper)
		{
			m_szInfo.Format(L"R %d 不在范围 (%d~%d)", m_valueAWB.R, m_RLower, m_RUpper);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGr)
	{
		if (m_valueAWB.Gr<m_GrLower ||m_valueAWB.Gr>m_GrUpper)
		{
			m_szInfo.Format(L"Gr %d 不在范围 (%d~%d)", m_valueAWB.Gr, m_GrLower, m_GrUpper);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}
	if (m_bCheckGb)
	{
		if (m_valueAWB.Gb<m_GbLower || m_valueAWB.Gb>m_GbUpper)
		{
			m_szInfo.Format(L"Gb %d 不在范围 (%d~%d)", m_valueAWB.Gb, m_GbLower, m_GbUpper);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckB)
	{
		if (m_valueAWB.B<m_BLower ||m_valueAWB.B>m_BUpper)
		{
			m_szInfo.Format(L"B %d 不在范围 (%d~%d)", m_valueAWB.B, m_BLower, m_BUpper);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGoldRG)
	{
		if (m_valueAWB.GOLDRG != m_GoldRG)
		{
			m_szInfo.Format(L"Golden RG %d 与 设定值 %d 不一致!", m_valueAWB.GOLDRG, m_GoldRG);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGoldBG)
	{
		if (m_valueAWB.GOLDBG != m_GoldBG)
		{
			m_szInfo.Format(L"Golden BG %d 与 设定值 %d 不一致!", m_valueAWB.GOLDBG, m_GoldBG);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGoldGG)
	{
		if (m_valueAWB.GOLDGG != m_GoldGG)
		{
			m_szInfo.Format(L"Golden GG %d 与 设定值 %d 不一致!", m_valueAWB.GOLDGG, m_GoldGG);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGoldR)
	{
		if (m_valueAWB.GOLDR != m_GoldR)
		{
			m_szInfo.Format(L"Golden R %d 与 设定值 %d 不一致!", m_valueAWB.GOLDR, m_GoldR);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGoldGr)
	{
		if (m_valueAWB.GOLDGr != m_GoldGr)
		{
			m_szInfo.Format(L"Golden Gr %d 与 设定值 %d 不一致!", m_valueAWB.GOLDGr, m_GoldGr);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGoldGb)
	{
		if (m_valueAWB.GOLDGb != m_GoldGb)
		{
			m_szInfo.Format(L"Golden Gb %d 与 设定值 %d 不一致!", m_valueAWB.GOLDGb, m_GoldGb);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	if (m_bCheckGoldB)
	{
		if (m_valueAWB.GOLDB != m_GoldB)
		{
			m_szInfo.Format(L"Golden B %d 与 设定值 %d 不一致!", m_valueAWB.GOLDB, m_GoldB);
			m_pInterface->AddLog(m_szInfo, COLOR_RED);
			return FUNCTION_FAIL;
		}
	}

	return FUNCTION_PASS;
} 

int OtpCheck_CMK8440::GetMacAFCode()
{
	int bRet = FUNCTION_PASS;
	// Read flag 
	BYTE readVal_H = 0, readVal_L = 0;
	USHORT page = m_otpPage.af;
	bRet |= Read_BYTE(m_afAddrCode.macH+(m_group.af-1)*m_gapsize.af, &readVal_H);
	bRet |= Read_BYTE(m_afAddrCode.macL+(m_group.af-1)*m_gapsize.af, &readVal_L);
	m_afValue.m_bMacCode = (readVal_H<<2) |((readVal_L&0xc0)>>6);

	if(bRet)
	{
		m_pInterface->AddLog(L"读取近景code值失败");
	}
	return bRet;
}

int OtpCheck_CMK8440::GetInfAFCode()
{
	int bRet = FUNCTION_PASS;
	// Read flag
	BYTE readVal_H = 0, readVal_L = 0;
	USHORT page = m_otpPage.af;
	bRet |= Read_BYTE(m_afAddrCode.infH+(m_group.af-1)*m_gapsize.af, &readVal_H);
	bRet |= Read_BYTE(m_afAddrCode.infL+(m_group.af-1)*m_gapsize.af, &readVal_L);
	m_afValue.m_bInfCode = (readVal_H<<2) |((readVal_L&0x30)>>4);

	if(bRet)
	{
		m_pInterface->AddLog(L"读取远景code值失败");
	}
	return bRet;
}