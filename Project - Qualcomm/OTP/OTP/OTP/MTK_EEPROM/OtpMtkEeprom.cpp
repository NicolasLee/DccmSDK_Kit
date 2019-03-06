#include "StdAfx.h"
#include "OtpMtkEeprom.h"
#include <ShellAPI.h>

COtpMtkEeprom::COtpMtkEeprom()
{
	nLscSize = 0;
	input_rawname = _T("");
	shading_cal = _T("");
	shading_val = _T("");
	shading_ver = _T("");
	shading_result = _T("");

}

COtpMtkEeprom::~COtpMtkEeprom()
{
	
}

void COtpMtkEeprom::Init(CString RawName,int nSize)
{
	CString path = m_pInterface->GetExeDir();
	path += "\\MTK_TOOL\\";
	input_rawname = path + RawName;
	shading_cal = path + _T("1_ShadingCalibrationSample.exe");
	shading_ver = path + _T("2_ShadingCorrectionSample.exe");
	shading_val =path + _T("eeprom_slim.txt");
	shading_result =path + _T("result.txt");
	nLscSize = nSize;
}

BOOL COtpMtkEeprom::create_lsc_group_Mtk(BYTE* image_data,BYTE *lencData,UINT width,UINT height)
{
// 	CString path = m_pInterface->GetExeDir() ;
// 	path += "\\MTK_TOOL\\";
// 	if(!save_raw_data(image_data, input_rawname, width, height))
// 	{
// 		m_pInterface->AddLog(_T("Save Raw Image Err"),RGB(255,0,0));
// 	}
// 	Sleep(20);
// 	CString sectionName = _T("_SetOTP");
// 	BOOL isDeleteMtkResult;
// 	CInspectionBase base;
// 	isDeleteMtkResult = base.ReadBool(sectionName,_T("IsDeleteMtkResult"),1);
//   	////////////////////
// 	CFileFind ff;
// 	if (isDeleteMtkResult)
// 	{
// 		if (ff.FindFile(shading_val,0))
// 		{
// 			DeleteFile(shading_val);
// 		}
// 		if (ff.FindFile(shading_result,0))
// 		{
// 			DeleteFile(shading_result);
// 		}
// 	}
// 	else
// 	{
// 		if (ff.FindFile(shading_val, 0))////挑选GS保留结果   
// 		{   
// 			DeleteFile(shading_val); 
// 		}
// 	}
// 	base.WriteBool(sectionName,_T("IsDeleteMtkResult"),isDeleteMtkResult);
// 	int nError = (int)ShellExecute(NULL, _T("open"), shading_cal, NULL, path, SW_HIDE);//SW_SHOWNORMAL);//
// 	if (nError < 32)
// 	{
// 		AfxMessageBox(_T("Failed to open ShadingCalibration.exe"));
// 		return 0;
// 	}
// 	int i;
// 	for ( i=0; i<300; i++)
// 	{
// 		Sleep(50);
// 		if (ff.FindFile(shading_val, 0))   
// 		{   
// 			break;
// 		}
// 	}
// 	if (i==300)
// 	{
// 		AfxMessageBox(_T("Failed to create eeprom_slim.txt!"));
// 		return 0;
// 	}
// 	nError = (int)ShellExecute(NULL, "open", shading_ver, NULL,path , SW_HIDE);//SW_SHOWNORMAL);//
// 	if (nError < 32)
// 	{
// 		AfxMessageBox(_T("Failed to open ShadingCorrection.exe"));
// 		return 0;
// 	}
// 
// 	for (i=0; i<300; i++)
// 	{
// 		Sleep(50);
// 		if (ff.FindFile(shading_result, 0))   
// 		{   
// 			break;
// 		}
// 	}
// 	if (i==300)
// 	{
// 		AfxMessageBox(_T("Failed to create result.txt!"));
// 		return 0;
// 	}
// 
// 	Sleep(50);
// 	CString line = _T("");	 
// 	CStdioFile file;
// 	if(!file.Open(shading_result, CFile::modeRead | CFile::typeText))
// 	{
// 		AfxMessageBox(_T("Failed to open ini file!"));
// 		return 0;
// 	}
// 	file.ReadString(line);
// 	while (file.ReadString(line))
// 	{
// 		if (line.Left(4) != "pass")
// 		{
// 					 
// 			file.Close();
// 			AfxMessageBox(_T("Gen LSC failed!"));
// 			return 0;
// 		}
// 		//////////////////////判断上限标准//////////////////////////////
// 		if(isDeleteMtkResult)
// 		{
// 			file.Close();
// 			CFile f;
// 			f.Open(shading_result,CFile::modeRead|CFile::typeBinary);
// 			int len = (int)f.GetLength();
// 			BYTE* buffer = new BYTE[len];	
// 			f.Read((void*)buffer,len);
// 			CString str = (LPTSTR)buffer;
// 			str.Remove(' ');//去掉空格先
// 
// 			vector<CString> vList;
// 			SeparateString(str, 9, &vList);//09为小段区分,用0x格式查看
// 			CString result;
// 			int j=0;
// 			float Y_decay[13],t;
// 			memset(Y_decay,0,13);
// 			for (UINT i = 0 ; i<vList.size(); i++)
// 			{
// 				if (vList.at(i) == "Y_Decay")
// 				{
// 					result = vList.at(i+1); 
// 					Y_decay[j]=(float)atof(result);
// 					j++;
// 
// 				}
// 			}
// 
// 			for (int x=0; x < 12; x++)
// 			{
// 				for (int y=x; y < 13; y++)
// 				{
// 					if (Y_decay[x] > Y_decay[y])
// 					{
// 						t = Y_decay[x];
// 						Y_decay[x] = Y_decay[y];
// 						Y_decay[y] = t;
// 
// 					}
// 
// 				}
// 			}
// 
// 			if (fabs(Y_decay[0]) < 26.0000)
// 			{
// 				float y;
// 				y=fabs(Y_decay[0]);
// 				CString tt;
// 				tt.Format("Y_Decay=%f is Error !",y);
// 				AfxMessageBox(tt);
// 				f.Close();
// 				return 0;
// 			}
// 			f.Close();
// 			delete []buffer;
// 			break;
// 		} 
// 	}
// 	if(!isDeleteMtkResult)  file.Close();
// 	CFile f;
// 	f.Open(shading_val,CFile::modeRead|CFile::typeBinary);
// 	int len = (int)f.GetLength();
// 	BYTE* buffer = new BYTE[len];	//1388
// 	f.Read((void*)buffer,len);
// 	if(len!=nLscSize)
// 	{
// 		m_pInterface->AddLog(_T("Eeprom数据大小和目标大小不一致"),RGB(255,0,0));
// 		f.Close();
// 		return 0;
// 	}
// 	memcpy(lencData,buffer,sizeof(BYTE)*len); 
// 	delete []buffer;    
// 	f.Close();	
// 	Sleep(20);
	return 1;
}


void COtpMtkEeprom::SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist)
{
	if(SrcString == _T("")) return;
	strlist->clear();

	CString temp, str;
	int nStart = 0, index = 0;
	TCHAR endChar = SrcString.GetAt(SrcString.GetLength()-1); //判断是否已','结尾

	while(1)
	{
		index = SrcString.Find(charSepareate, nStart);
		if (index == -1) 
		{
			if (endChar != charSepareate)//如果最后一个不是分割字符，再把后面的一段保存再退出
			{
				str = SrcString.Mid(nStart,SrcString.GetLength()-nStart);
				
				if (str == _T("")) str = _T("0"); //如果为空，默认“0”,防止数据库字符串为空出错
				strlist->push_back(str);
			}
			break;//如果最后一个就是分割字符，退出
		}
		str = SrcString.Mid(nStart, index-nStart);
		if (str == _T("")) str = _T("0"); //如果为空，默认“0”,防止数据库字符串为空出错
		strlist->push_back(str);
		nStart = index + 1;
	}
	
}

BOOL COtpMtkEeprom::save_raw_data(BYTE* image_data, CString raw_name,UINT width,UINT height)
{
// 	if (image_data == NULL)
// 	{
// 		m_pInterface->AddLog(_T("Raw Data Pointor Err!"),RGB(255,0,0));
// 		return 0;
// 	}
// 	
// 	CFileFind ff;
// 	if (ff.FindFile(raw_name, 0))   
// 	{   
// 		DeleteFile(raw_name);   
// 	}
// 	
// 	HFILE hfile = _lcreat(raw_name, FALSE); 
// 	if (hfile < 0) 
// 	{
// 		AfxMessageBox("File created failed!");
// 		return 0;
// 	}
// 	
// 	if (1)//(sensor_type == D_MIPI_RAW10)
// 	{
// 		BYTE* buf = Raw10_to_Raw8(image_data, width, height);
// 		//BYTE* B_buf= skip_raw8_order(buf, image_width, image_height);
// 		//		raw = buf;
// 		if(buf == NULL)
// 		{
// 			AfxMessageBox("Buffer created failed!");
// 			return 0;
// 		}
// 		
// 		//	UINT len = _lwrite(hfile, (LPSTR)B_buf, (image_width)*(image_height-4)); 
// 		UINT len = _lwrite(hfile, (LPSTR)buf, width*height);
// 		
// 		delete buf;
// 		//	delete B_buf;
// 	}
// 	else
// 	{
// 		UINT len = _lwrite(hfile, (LPSTR)image_data, width*height); 
// 	}
// 	_lclose(hfile);
	
	return 1;
}

