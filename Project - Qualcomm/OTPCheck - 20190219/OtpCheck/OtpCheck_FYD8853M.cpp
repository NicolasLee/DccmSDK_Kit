#include "StdAfx.h"
#include "OtpCheck_FYD8853M.h"


OtpCheck_FYD8853M::OtpCheck_FYD8853M(void)
{
	m_flag.empty = 0xFF;
	m_flag.validGroup1 = 0x01;
	m_flag.validGroup2 = UNEXCEPT_VALUE;
	m_flag.validGroup3 = UNEXCEPT_VALUE;

	m_group.empty   = 0;
	m_group.invalid  = -1;
	m_group.group1  = 1;
	m_group.group2 = UNEXCEPT_VALUE;
	m_group.group3 = UNEXCEPT_VALUE;

	m_addrFlag.basic	= 0x0000;		
	m_addrFlag.awb			= 0x0040;		
	m_addrFlag.lsc			= 0x0080;		
	m_addrFlag.af			= 0x0800;		
	m_addrFlag.pdaf1			= 0x0840;		
	m_addrFlag.pdaf2= 0x0A30;		

	m_addrSum.basic	= 0x0015;		
	m_addrSum.awb			= 0x0055;		
	m_addrSum.lsc			= 0x07CD;		
	m_addrSum.af			= 0x0805;		 
	m_addrSum.pdaf1		= 0x0D9D;		
	m_addrSum.pdaf2		= 0x0D9E;

	m_otpPage.basic	= UNEXCEPT_VALUE;
	m_otpPage.awb			= UNEXCEPT_VALUE;
	m_otpPage.lsc			= UNEXCEPT_VALUE;	
	m_otpPage.af			= UNEXCEPT_VALUE;
	m_otpPage.pdaf1		= UNEXCEPT_VALUE;	
	m_otpPage.pdaf1_End = UNEXCEPT_VALUE;
	m_otpPage.pdaf2		= UNEXCEPT_VALUE;	
	m_otpPage.pdaf2_End = UNEXCEPT_VALUE;

	m_addrAWB .RGL = 0x0041;
	m_addrAWB. RGH = 0x0042;
	m_addrAWB .BGL = 0x0043;
	m_addrAWB. BGH = 0x0044;
	m_addrAWB .GGL = 0x0045;
	m_addrAWB. GGH = 0x0046;

	m_addrAWB .RL = 0x004D;
	m_addrAWB. RH = UNEXCEPT_REG;
	m_addrAWB .GrL = 0x004E;
	m_addrAWB. GrH = UNEXCEPT_REG;
	m_addrAWB .GbL = 0x004F;
	m_addrAWB. GbH = UNEXCEPT_REG;
	m_addrAWB .BL = 0x0050;
	m_addrAWB. BH = UNEXCEPT_REG;

	m_addrAWB .GOLDRGL = 0x0047;
	m_addrAWB .GOLDRGH = 0x0048;
	m_addrAWB .GOLDBGL = 0x0049;
	m_addrAWB .GOLDBGH = 0x004A;
	m_addrAWB .GOLDGGL = 0x004B;
	m_addrAWB .GOLDGGH = 0x004C;

	m_addrAWB .GOLDRL = 0x0051;
	m_addrAWB .GOLDRH = UNEXCEPT_REG;
	m_addrAWB .GOLDGrL = 0x0052;
	m_addrAWB .GOLDGrH = UNEXCEPT_REG;
	m_addrAWB .GOLDGbL = 0x0053;
	m_addrAWB .GOLDGbH = UNEXCEPT_REG;
	m_addrAWB .GOLDBL = 0x0054;
	m_addrAWB .GOLDBH = UNEXCEPT_REG;

	m_afAddrCode.stcL			= UNEXCEPT_VALUE;
	m_afAddrCode.stcH			= UNEXCEPT_VALUE;
	m_afAddrCode.infL			= 0x0801;
	m_afAddrCode.infH			= 0x0802;
	m_afAddrCode.macL		= 0x0803;
	m_afAddrCode.macH		= 0x0804;

	m_bCheckStc = FALSE;	// ��ʼCode
	m_bCheckInf = TRUE;	// Զ��Code
	m_bCheckMac = TRUE;	// ����Code

	m_gapsize.basic = UNEXCEPT_VALUE;
	m_gapsize.awb = UNEXCEPT_VALUE;
	m_gapsize.lsc = UNEXCEPT_VALUE;
	m_gapsize.af = UNEXCEPT_VALUE;
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


OtpCheck_FYD8853M::~OtpCheck_FYD8853M(void)
{
}

int OtpCheck_FYD8853M::CheckGroup(BYTE flag)
{
	if (flag == m_flag.empty)
	{
		return m_group.empty; //group Ϊ��
	}
	else if (flag == m_flag.validGroup1)
	{
		return m_group.group1;
	}
	else
	{
		return m_group.invalid;//��Чgroup����Ϊ-1
	}
}

int OtpCheck_FYD8853M::CheckSum(BOOL flag,int flagaddr, int sumaddr, int datasize, int gap, CString str, int validgroup)//������checksum
{
	if (!flag) return FUNCTION_PASS; //û��ѡ����Ӧchecksum����ֱ�ӷ�����

	// Calc sum
	BYTE* pVal = new BYTE[datasize];
	Read(flagaddr+1+(validgroup-1)*gap,pVal, datasize);//��ȡ����checksum������
	BYTE calcVal = SumCalc(pVal, datasize,m_calcCheckSum.divisor,m_calcCheckSum.addend);//����checksum

	// Read sum
	BYTE readVal = 0;
	Read_BYTE(sumaddr+(validgroup-1)*gap,&readVal);//��ȡ����¼checksum

	if (readVal != calcVal)//�Աȼ��checksum�Ƿ���ȷ
	{
		m_szInfo.Format(L"%s read sum 0x%02x != calc sum 0x%02x", str, readVal, calcVal);
		m_pInterface->AddLog(m_szInfo, COLOR_RED);
		SAFE_DELETE_ARRAY(pVal);
		return FUNCTION_FAIL;
	}
	SAFE_DELETE_ARRAY(pVal);

	return FUNCTION_PASS;
}

void OtpCheck_FYD8853M::SaveOTPData()//�������ȡģ���ڵ�OTP����
{
	m_pInterface->AddLog(_T("Read OTP Start!"));
	///////////////////////////////Read OTP////////////////////////////////////
	int valSize = 3519;
	BYTE* pVal = new BYTE[valSize];
	Read(0000, pVal, valSize);

	//////////////////////////////////////////////////////////////////////////
	CString DirPath = m_pInterface->GetExeDir();
	CString Path ;
	Path.Format(_T("OTP Data_%d.txt"),m_pInterface->GetModuleNum());
	Path = DirPath+Path;
	FILE* file = NULL;

	CStringA temp, num;
	for (int i = 0; i<valSize; i++)
	{
		num.Format("%04d\t0x%04x,0x%02x\n", i+1,i,(unsigned char)pVal[i]);
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

