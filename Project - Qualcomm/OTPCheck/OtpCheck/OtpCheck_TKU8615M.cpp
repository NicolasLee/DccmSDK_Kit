#include "StdAfx.h"
#include "OtpCheck_TKU8615M.h"


OtpCheck_TKU8615M::OtpCheck_TKU8615M(void)
{
	m_flag.empty = 0x00;
	m_flag.validGroup1 = 0x01;
	m_flag.validGroup2 = 0x13;
	m_flag.validGroup3 = 0x37;

	m_group.empty = 0;
	m_group.invalid = -1;
	m_group.group1 = 1;
	m_group.group2 = 2;
	m_group.group3 = 3;

	m_addrFlag.basic	= 0x0501;		
	m_addrFlag.awb			= 0x0F5F;		
	m_addrFlag.lsc			= 0x0535;		
	m_addrFlag.af			= 0x1000;		
	m_addrFlag.pdaf1			= UNEXCEPT_REG;		
	m_addrFlag.pdaf2= UNEXCEPT_REG;		

	m_addrSum.basic	= 0x0512;		
	m_addrSum.awb			= 0xF7D;		
	m_addrSum.lsc			= 0x0898;		
	m_addrSum.af			= 0x1008;		
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

	m_addrAWB .RGL = 0xF61;
	m_addrAWB. RGH = 0xF60;
	m_addrAWB .BGL = 0xF63;
	m_addrAWB. BGH = 0xF62;
	m_addrAWB .GGL = UNEXCEPT_REG;
	m_addrAWB. GGH = UNEXCEPT_REG;
	m_addrAWB .RL = 0xF6D;
	m_addrAWB. RH = 0xF6C;
	m_addrAWB .GrL = 0xF71;
	m_addrAWB. GrH = 0xF70;
	m_addrAWB .GbL = 0xF73;
	m_addrAWB. GbH = 0xF72;
	m_addrAWB .BL = 0xF6F;
	m_addrAWB. BH = 0xF6E;

	m_addrAWB .GOLDRGL = 0xF67;
	m_addrAWB .GOLDRGH = 0xF66;
	m_addrAWB .GOLDBGL = 0xF69;
	m_addrAWB .GOLDBGH = 0xF68;
	m_addrAWB .GOLDGGL = UNEXCEPT_REG;
	m_addrAWB .GOLDGGH = UNEXCEPT_REG;
	m_addrAWB .GOLDRL = 0xF75;
	m_addrAWB .GOLDRH = 0xF74;
	m_addrAWB .GOLDGrL = 0xF79;
	m_addrAWB .GOLDGrH = 0xF78;
	m_addrAWB .GOLDGbL = 0xF7B;
	m_addrAWB .GOLDGbH = 0xF7A;
	m_addrAWB .GOLDBL = 0xF77;
	m_addrAWB .GOLDBH = 0xF76;

	m_afAddrCode.stcL			= UNEXCEPT_VALUE;
	m_afAddrCode.stcH			= UNEXCEPT_VALUE;
	m_afAddrCode.infL			= 0x1004;
	m_afAddrCode.infH			= 0x1003;
	m_afAddrCode.macL		= 0x1006;
	m_afAddrCode.macH		= 0x1005;

	m_bCheckStc = FALSE;	// 起始Code
	m_bCheckInf = TRUE;	// 远景Code
	m_bCheckMac = TRUE;	// 近景Code

	m_gapsize.basic = 17;
	m_gapsize.awb = 30;
	m_gapsize.lsc = 867;
	m_gapsize.af = 8;
	m_gapsize.pdaf1 = UNEXCEPT_VALUE;
	m_gapsize.pdaf2 = UNEXCEPT_VALUE;

	m_otpSize.basic	= m_addrSum.basic - m_addrFlag.basic - 1;
	m_otpSize.awb			= m_addrSum.awb - m_addrFlag.awb - 1;
	m_otpSize.lsc			= m_addrSum.lsc - m_addrFlag.lsc - 1;
	m_otpSize.af			= m_addrSum.af	- m_addrFlag.af - 1;
	m_otpSize.pdaf1		= UNEXCEPT_VALUE;
	m_otpSize.pdaf2		= UNEXCEPT_VALUE;

	m_calcCheckSum.divisor = 255;
	m_calcCheckSum.addend = 1;
}


OtpCheck_TKU8615M::~OtpCheck_TKU8615M(void)
{
}

int OtpCheck_TKU8615M::InitProject()
{
	return InitOTP();
}

int OtpCheck_TKU8615M::EndProject()
{
	return ReleaseOTP();
}

int OtpCheck_TKU8615M::CheckGroup(BYTE flag)
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
	else if (flag == m_flag.validGroup3)
	{
		return m_group.group3;
	}
	else
	{
		return m_group.invalid;//无效group定义为-1
	}
}

int OtpCheck_TKU8615M::CheckSum(BOOL flag,int flagaddr, int sumaddr, int datasize, int gap, CString str, int validgroup)//检测多组checksum
{
	if (!flag) return FUNCTION_PASS; //没勾选检测对应checksum项则直接返回真

	if (str == L"AF")
	{
		return CheckSum_Word(flag,flagaddr,sumaddr,datasize,gap,str,validgroup);
	}

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

int OtpCheck_TKU8615M::CheckSum_Word(BOOL flag,int flagaddr, int sumaddr, int datasize, int gap, CString str, int validgroup)//检测AFchecksum(MSB LSB)
{
	if (!flag) return FUNCTION_PASS; //没勾选检测对应checksum项则直接返回真

	// Calc sum
	BYTE* pVal = new BYTE[datasize-1];
	Read(flagaddr+1+(validgroup-1)*gap,pVal, datasize-1);//获取计算checksum的数据
	WORD calcSum = SumCalc_WORD(pVal, datasize-1,255,1);//计算checksum
	BYTE calcVal[2] = {0};
	calcVal[0] = calcSum >> 8;
	calcVal[1] = calcSum & 0xff;

	// Read sum
	BYTE readVal[2] = {0};
	Read(sumaddr+(validgroup-1)*gap-1,readVal,2);//读取已烧录checksum

	if (readVal[0] != calcVal[0] || readVal[1] != calcVal[1] )//对比检查checksum是否正确
	{
		m_szInfo.Format(L"%s read sum 0x%02x,0x%02x != calc sum 0x%02x,0x%02x", str, readVal[0], readVal[1],calcVal[0],calcVal[1]);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		SAFE_DELETE_ARRAY(pVal);
		return FUNCTION_FAIL;
	}
	SAFE_DELETE_ARRAY(pVal);

	return FUNCTION_PASS;
}

void OtpCheck_TKU8615M::SaveOTPData()//点亮后读取模组内的OTP数据
{
	m_pInterface->AddLog(_T("Read OTP Start!"));
	///////////////////////////////Read OTP////////////////////////////////////

	int valSize = 2840;
	BYTE* pVal = new BYTE[valSize];
	memset(pVal,0xff,valSize*sizeof(BYTE));
	int i = 0; 
	int j = 0;

	for (j=0; j<5; j++)//一口气读2840个数据Hi1332 Sensor会停止工作，需要分成几次来读
	{	
		InitOTP();
		Read(0x501+j*568, pVal+j*568, 568);
		ReleaseOTP();
		Sleep(100);
	}

	Sleep(100);
	//////////////////////////////////////////////////////////////////////////
	CString DirPath = m_pInterface->GetExeDir();
	CString Path ;
	Path.Format(_T("OTP Data_%d.txt"),m_pInterface->GetModuleNum());
	Path = DirPath+Path;
	FILE* file = NULL;

	CStringA temp, num;


	for(i=0; i<valSize; i++)
	{
		num.Format("%04d\t0x%04x,0x%02x\n", i,m_addrFlag.basic+i,(unsigned char)pVal[i]);
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

