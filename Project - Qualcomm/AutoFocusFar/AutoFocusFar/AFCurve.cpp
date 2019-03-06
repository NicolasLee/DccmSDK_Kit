#include "stdafx.h"
#include "AFCurve.h"

AFCurve::AFCurve()
{
	m_bNewStartFlag = TRUE;
}

void AFCurve::AddNode(double val, int code)
{
	FocusNode newNode;	// newNode
	newNode.m_Value = val;
	newNode.m_Code = code;
	newNode.m_Index  = m_vFocusNode.size();

	if (m_bNewStartFlag)
	{
		newNode.m_bNewStartPoint = TRUE;
		newNode.m_Flag = 0;
		newNode.m_delta = 0;

		m_bNewStartFlag = FALSE;
	}
	else
	{
		newNode.m_bNewStartPoint = FALSE;
		newNode.m_delta = newNode.m_Value - m_vFocusNode.back().m_Value ;		// 前后点的差值
		double ratio = 100- 100*newNode.m_Value/m_vFocusNode.back().m_Value;	// 前后点的比值差异

		newNode.m_Flag = m_vFocusNode.back().m_Flag;
		// 如果是往下降，同时下降幅度大于阈值，状态--;
		if (newNode.m_delta < 0)
		{
			if (ratio > m_CurDiffLimit )
			{		
				newNode.m_Flag--;
			}
		}
		else
		{
			newNode.m_Flag++;
			if (newNode.m_Flag >= 0)	// 只记录下降Flag
				newNode.m_Flag = 0;				
		}
	}

	m_vFocusNode.push_back(newNode);
}


int AFCurve::GetNextSearchDirection()
{
	int size = m_vFocusNode.size();
	if (size < 3) return 0;

	int index = FindMaxNode().m_Index;

	double pre, next;

	if (index == 0)
	{
		pre = m_vFocusNode[0].m_Value;
		next = m_vFocusNode[index+1].m_Value;
	}
	else
	{
		pre = m_vFocusNode[index-1].m_Value;
		next = m_vFocusNode[index+1].m_Value;
	}

	if (next <= pre)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

int AFCurve::GetNextSearchStartCode()
{
	int size = m_vFocusNode.size();
	if (size < 3) return 0;

	int index = FindMaxNode().m_Index;

	double pre, next;

	if (index == 0)
	{
		int step = m_vFocusNode[index].m_Code - m_vFocusNode[0].m_Code;
		return m_vFocusNode[0].m_Code +  abs(step/2);
	}
	else
	{
		pre = m_vFocusNode[index-1].m_Value;
		next = m_vFocusNode[index+1].m_Value;
	}

	//从1/4处开始搜索
	if (next <= pre)
	{
		int step = m_vFocusNode[index].m_Code - m_vFocusNode[index-1].m_Code;
		return m_vFocusNode[index-1].m_Code +  abs(step/4);
	}
	else
	{
		int step = m_vFocusNode[index].m_Code - m_vFocusNode[index+1].m_Code;
		return m_vFocusNode[index+1].m_Code - abs(step/4);
	}
}

CString AFCurve::OutputNodeInfo()
{
	CString szStr;
	FocusNode lastNode = m_vFocusNode.back();	// 最后这个点
	CString arrow;

	if (lastNode.m_delta > 0)
	{
		arrow = _T(" ↑ ");
	}
	else if(lastNode.m_delta < 0)
	{
		arrow = _T(" ↓ ");
	}
	else if(lastNode.m_delta == 0)
	{
		arrow = _T("");
	}

	szStr.Format(_T("No:%2d   code = %4d  Value = %.2lf  Flag = %2d  %s"), lastNode.m_Index, lastNode.m_Code, lastNode.m_Value, lastNode.m_Flag, arrow);

	if (lastNode.m_Index == 0)
	{
		szStr += _T(" [Initial pos]");
	}
	else if(lastNode.m_bNewStartPoint) // 代表新的搜索起点
	{
		szStr += _T("@");
	}

	return szStr;
}


CString AFCurve::OutputNodeInfo(CString szInsert)
{
	CString szStr;
	FocusNode lastNode = m_vFocusNode.back();
	CString arrow;

	if (lastNode.m_delta > 0)
	{
		arrow = _T(" ↑ ");
	}
	else if(lastNode.m_delta < 0)
	{
		arrow = _T(" ↓ ");
	}
	else if(lastNode.m_delta == 0)
	{
		arrow = _T("");
	}

	szStr.Format(_T("No:%3d %s code = %4d  Value = %.2lf  Flag = %2d  %s"), lastNode.m_Index, szInsert, lastNode.m_Code, lastNode.m_Value, lastNode.m_Flag, arrow);

	if (lastNode.m_Index == 0)
	{
		szStr += _T(" [Initial pos]");
	}
	else if(lastNode.m_bNewStartPoint) //代表新的搜索起点
	{
		szStr += _T("@");
	}

	return szStr;
}

void AFCurve::Init(void)
{
	m_vFocusNode.clear();
	m_bNewStartFlag = TRUE;
}


FocusNode AFCurve::GetBackNode(void)
{
	return m_vFocusNode.back();
}

FocusNode AFCurve::GetPenultimateNode(void)
{
	int size = m_vFocusNode.size();
	if (size >= 2)
	{
		return m_vFocusNode.at(size - 2);
	}
	else
	{
		return m_vFocusNode.back();
	}
}

FocusNode AFCurve::FindMaxNode(void)
{
	double maxVal = 0;
	int index = 0;
	for(UINT i = 0; i < m_vFocusNode.size(); i++)
	{
		if(m_vFocusNode[i].m_Value >= maxVal)
		{
			maxVal = m_vFocusNode[i].m_Value;
			index = i;
		}
	}
	return m_vFocusNode[index];
}

void AFCurve::SetNextSearchTarget(int DescendTimeLimt, double DiffLimt)
{
	m_CurDescendTimeLimit = DescendTimeLimt;
	m_CurDiffLimit = DiffLimt;
	m_bNewStartFlag = TRUE;
}

BOOL AFCurve::isSearchTargetReached()
{
	if (GetBackNode().m_Flag <= -m_CurDescendTimeLimit)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

int AFCurve::GetBestCode(void)
{
	int size = m_vFocusNode.size();
	int index = FindMaxNode().m_Index;

	if (size < 3) 	return m_vFocusNode[index].m_Code;

	if (index == 0 || index == (size-1)) return m_vFocusNode[index].m_Code;

	double preValue = m_vFocusNode[index-1].m_Value;
	double nextValue = m_vFocusNode[index+1].m_Value;
	double MaxValue = m_vFocusNode[index].m_Value;

	int Precode = m_vFocusNode[index-1].m_Code;
	int Nextcode = m_vFocusNode[index+1].m_Code;
	int MaxCode = m_vFocusNode[index].m_Code;

	double bestCode = 0;

	if (nextValue <= preValue)
	{// 下降或水平曲线
		double k1 =  (MaxValue - nextValue) / (MaxCode - Nextcode);// val/code
		double b1 = MaxValue - k1*MaxCode;

		double k2 = -1*k1;	// code/val
		double b2 = preValue - k2*Precode;

		bestCode = -(b2 - b1) /(k2 - k1);
	}
	else
	{// 上升
		double k1 =  (MaxValue - preValue) / (MaxCode - Precode);
		double b1 = MaxValue - k1*MaxCode;

		double k2 = -1*k1;
		double b2 = nextValue - k2*Nextcode;

		bestCode = -(b2 - b1) /(k2 - k1);
	}

	return (int)bestCode;
}

void AFCurve::SaveCurve(CString path)
{
	CString szTemp;
	CStdioFile file;
	if (file.Open(path, CFile::modeWrite | CFile::modeCreate))
	{
		file.WriteString(_T("Code, Resolution\n"));
		for(vector<FocusNode>::const_iterator it = m_vFocusNode.begin(); it != m_vFocusNode.end(); ++it)
		{
			szTemp.Format(_T("%d,%5.2f\n"), it->m_Code, it->m_Value);
			file.WriteString(szTemp);
		}
		file.Close();
	}

// 	if (file.Open(path, CFile::modeWrite | CFile::modeCreate| CFile::modeNoTruncate))
// 	{
// 		file.SeekToEnd();	// 先定位到文件尾部
// 		file.WriteString(_T("Code, Resolution\n"));
// 		for(vector<FocusNode>::const_iterator it = m_vFocusNode.begin(); it != m_vFocusNode.end(); ++it)
// 		{
// 			szTemp.Format(_T("%d,%d\n"), it->m_Code, int(it->m_Value));
// 			file.WriteString(szTemp);
// 		}
// 		file.Close();
// 	}
}
