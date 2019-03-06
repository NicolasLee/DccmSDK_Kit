#pragma once

#include  <vector>
using namespace std;

class FocusNode
{
public:
	USHORT m_Code;			// AF寄存器值
	double m_Value;			// 解析力值
	double m_delta;			// 前一个值和后一个值的差值
	int    m_Flag;			// 当前状态
	int    m_Index;			// 第N个信息
	BOOL   m_bNewStartPoint;// 是否是搜索起点
};

class SearchParam
{
public:
	SearchParam(){};
	SearchParam(int DescendTimeLimt, double DiffLimt, int StepCode)
	{
		m_DescendTime = DescendTimeLimt;
		m_Diff = DiffLimt;
		m_StepCode = StepCode;
	}

	int		m_DescendTime;
	double  m_Diff;
	int		m_StepCode;
};

class AFCurve
{
public:
	AFCurve();

	// 初始化
	void Init(void);
	// 添加节点
	void AddNode(double val, int code);
	// 得到下轮搜索的方向
	int GetNextSearchDirection();
	// 得到下轮搜搜的起点
	int GetNextSearchStartCode();
	// 得到当前节点的输出消息
	CString OutputNodeInfo();
	CString OutputNodeInfo(CString szInsert);
	// 返回最后一个节点
	FocusNode GetBackNode(void);
	// 返回上一个（倒数第二个）节点
	FocusNode GetPenultimateNode(void);
	//返回最大解析力值的节点
	FocusNode FindMaxNode(void);
	// 设置搜索停止的目标信息
	void SetNextSearchTarget(int DescendTimeLimit,double DiffLimit);
	// 判断当前节点是否达到了搜索目标信息
	BOOL isSearchTargetReached();
	// 得到拟合的最佳清晰点
	int GetBestCode(void);

	void SaveCurve(CString path);

	vector<FocusNode> m_vFocusNode;

private:
	int		m_CurDescendTimeLimit;
	double  m_CurDiffLimit;			// 当前下降解析力限制
	BOOL	m_bNewStartFlag;



};

