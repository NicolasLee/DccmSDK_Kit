#pragma once

#include  <vector>
using namespace std;

class FocusNode
{
public:
	USHORT m_Code;			// AF�Ĵ���ֵ
	double m_Value;			// ������ֵ
	double m_delta;			// ǰһ��ֵ�ͺ�һ��ֵ�Ĳ�ֵ
	int    m_Flag;			// ��ǰ״̬
	int    m_Index;			// ��N����Ϣ
	BOOL   m_bNewStartPoint;// �Ƿ����������
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

	// ��ʼ��
	void Init(void);
	// ��ӽڵ�
	void AddNode(double val, int code);
	// �õ����������ķ���
	int GetNextSearchDirection();
	// �õ��������ѵ����
	int GetNextSearchStartCode();
	// �õ���ǰ�ڵ�������Ϣ
	CString OutputNodeInfo();
	CString OutputNodeInfo(CString szInsert);
	// �������һ���ڵ�
	FocusNode GetBackNode(void);
	// ������һ���������ڶ������ڵ�
	FocusNode GetPenultimateNode(void);
	//������������ֵ�Ľڵ�
	FocusNode FindMaxNode(void);
	// ��������ֹͣ��Ŀ����Ϣ
	void SetNextSearchTarget(int DescendTimeLimit,double DiffLimit);
	// �жϵ�ǰ�ڵ��Ƿ�ﵽ������Ŀ����Ϣ
	BOOL isSearchTargetReached();
	// �õ���ϵ����������
	int GetBestCode(void);

	void SaveCurve(CString path);

	vector<FocusNode> m_vFocusNode;

private:
	int		m_CurDescendTimeLimit;
	double  m_CurDiffLimit;			// ��ǰ�½�����������
	BOOL	m_bNewStartFlag;



};

