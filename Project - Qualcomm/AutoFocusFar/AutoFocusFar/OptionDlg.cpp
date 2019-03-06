// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoFocusFar.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "DccmBase.h"
#include "AutoFocusFar_Inspection.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, AutoFocusFar_Inspection* pInspection)
	: CDialog(OptionDlg::IDD, pParent)
	, SaveAF_Curve(FALSE)
	, CheckCornerDiff(false)
{
	//TODO 添加创建对话框
	this->pInspection = pInspection;
	StartCode = 0;
	ExpEnable = FALSE;	
	ExpTolerance = 0;
	ExpTarget = 0;
	AlgorithmType = 0;
	MAX_Y = 0;
	MAX_X = 0;

	FramePitchLimit = 0;


	SearchTime = 1;
	StepCode1 = 1;
	StepCode2 = 1;
	StepCode3 = 1;
	StepCode4 = 1;
	StepDiff1 = 0.0;
	StepDiff2 = 0.0;
	StepDiff3 = 0.0;
	StepDiff4 = 0.0;
	DescendTime1 = 1;
	DescendTime2 = 1;
	DescendTime3 = 1;	
	DescendTime4 = 1;	
	CodeLimitL = 0;
	CodeLimitH = 0;
	SearchArea.StartCode = 0;
	SearchArea.EndCode = 0;
	SearchArea.StepCode = 0;
	SearchArea.Enable = FALSE;
	ClearRecord = FALSE;
	BackToInit = FALSE;
	CheckGlue = FALSE;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// 解像力值算法
	DDX_Radio(pDX, IDC_RADIO_ALGORITHM, AlgorithmType);

	// 曲线显示
	DDX_Text(pDX, IDC_EDIT_MAX_Y, MAX_Y);
	DDX_Text(pDX, IDC_EDIT_MAX_X, MAX_X);

	// 曝光设置
	DDX_Check(pDX, IDC_CHECK_EXPOSURE, ExpEnable);
	DDX_Text(pDX, IDC_EDIT_EXP_TOLERANCE, ExpTolerance);
	DDV_MinMaxInt(pDX, ExpTolerance, 0, 256);
	DDX_Text(pDX, IDC_EDIT_EXP_TARGET, ExpTarget);
	DDV_MinMaxInt(pDX, ExpTarget, 0, 256);

	// 其他
	DDX_Check(pDX, IDC_CHECK_SHOW_ORIGINAL_IMG, ShowOriginalImg);
	DDX_Check(pDX, IDC_CHECK_CLEAR_RECORD, ClearRecord);
	DDX_Check(pDX, IDC_CHECK_SAVE_CURVE, SaveAF_Curve);
	DDX_Check(pDX, IDC_CHECK_BACK_TO_INIT_POS, BackToInit);
	DDX_Check(pDX, IDC_CHECK_GLUE, CheckGlue);

	// AF算法设置
	DDX_Text(pDX, IDC_EDIT_START_CODE, StartCode);
	DDV_MinMaxInt(pDX, StartCode, 0, 1023);
	DDX_CBIndex(pDX, IDC_COMBO_SEARCH_TIME, SearchTime);
	DDV_MinMaxInt(pDX, SearchTime, 0, 3);
	DDX_Text(pDX, IDC_EDIT_FRAME_PITCH, FramePitchLimit);
	DDV_MinMaxInt(pDX, FramePitchLimit, 0, 10);
	DDX_Text(pDX, IDC_EDIT_STEP_CODE_1, StepCode1);
	DDV_MinMaxInt(pDX, StepCode1, 0, 1023);
	DDX_Text(pDX, IDC_EDIT_STEP_CODE_2, StepCode2);
	DDV_MinMaxInt(pDX, StepCode2, 0, 1023);
	DDX_Text(pDX, IDC_EDIT_STEP_CODE_3, StepCode3);
	DDV_MinMaxInt(pDX, StepCode3, 0, 1023);
	DDX_Text(pDX, IDC_EDIT_STEP_CODE_4, StepCode4);
	DDV_MinMaxInt(pDX, StepCode4, 0, 1023);
	DDX_Text(pDX, IDC_EDIT_DESCEND_TIME_1, DescendTime1);
	DDV_MinMaxInt(pDX, DescendTime1, 1, 1000000);
	DDX_Text(pDX, IDC_EDIT_DESCEND_TIME_2, DescendTime2);
	DDV_MinMaxInt(pDX, DescendTime2, 1, 1000000);
	DDX_Text(pDX, IDC_EDIT_DESCEND_TIME_3, DescendTime3);
	DDV_MinMaxInt(pDX, DescendTime3, 1, 1000000);
	DDX_Text(pDX, IDC_EDIT_DESCEND_TIME_4, DescendTime4);
	DDV_MinMaxInt(pDX, DescendTime4, 1, 1000000);
	DDX_Text(pDX, IDC_EDIT_STEP_DIFF_1, StepDiff1);
	DDV_MinMaxDouble(pDX, StepDiff1, 1, 100);
	DDX_Text(pDX, IDC_EDIT_STEP_DIFF_2, StepDiff2);
	DDV_MinMaxDouble(pDX, StepDiff2, 1, 100);
	DDX_Text(pDX, IDC_EDIT_STEP_DIFF_3, StepDiff3);
	DDV_MinMaxDouble(pDX, StepDiff3, 1, 100);
	DDX_Text(pDX, IDC_EDIT_STEP_DIFF_4, StepDiff4);
	DDV_MinMaxDouble(pDX, StepDiff4, 1, 100);

	// 搜索最清晰区间
	DDX_Check(pDX, IDC_CHECK_SEARCH_AREA_ENABLE, SearchArea.Enable);
	DDX_Text(pDX, IDC_CHECK_SEARCH_AREA_START_CODE, SearchArea.StartCode);
	DDV_MinMaxInt(pDX, SearchArea.StartCode, 0, 1024);
	DDX_Text(pDX, IDC_CHECK_SEARCH_AREA_END_CODE, SearchArea.EndCode);
	DDV_MinMaxInt(pDX, SearchArea.EndCode, 0, 1024);
	DDX_Text(pDX, IDC_CHECK_SEARCH_AREA_STEP_CODE, SearchArea.StepCode);
	DDV_MinMaxInt(pDX, SearchArea.StepCode, 0, 1024);

	DDX_Check(pDX, IDC_CHECK_RESOLUTION, ResolutionCheck);
	DDX_Check(pDX, IDC_CHECK_MTF_0, pInspection->sMTF.ROI[0].Enable);
	DDX_Text(pDX, IDC_EDIT_MTF_CX_0, pInspection->sMTF.ROI[0].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[0].cx, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MTF_CY_0, pInspection->sMTF.ROI[0].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[0].cy, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MTF_LIMIT_0, pInspection->sMTF.ROI[0].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[0].limit1, 0, 100);

	DDX_Check(pDX, IDC_CHECK_MTF_1, pInspection->sMTF.ROI[1].Enable);
	DDX_Text(pDX, IDC_EDIT_MTF_CX_1, pInspection->sMTF.ROI[1].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[1].cx, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MTF_CY_1, pInspection->sMTF.ROI[1].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[1].cy, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MTF_LIMIT_1, pInspection->sMTF.ROI[1].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[1].limit1, 0, 100);

	DDX_Check(pDX, IDC_CHECK_MTF_2, pInspection->sMTF.ROI[2].Enable);
	DDX_Text(pDX, IDC_EDIT_MTF_CX_2, pInspection->sMTF.ROI[2].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[2].cx, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MTF_CY_2, pInspection->sMTF.ROI[2].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[2].cy, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MTF_LIMIT_2, pInspection->sMTF.ROI[2].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[2].limit1, 0, 100);

	DDX_Check(pDX, IDC_CHECK_MTF_3, pInspection->sMTF.ROI[3].Enable);
	DDX_Text(pDX, IDC_EDIT_MTF_CX_3, pInspection->sMTF.ROI[3].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[3].cx, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MTF_CY_3, pInspection->sMTF.ROI[3].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[3].cy, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MTF_LIMIT_3, pInspection->sMTF.ROI[3].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[3].limit1, 0, 100);

	DDX_Check(pDX, IDC_CHECK_MTF_4, pInspection->sMTF.ROI[4].Enable);
	DDX_Text(pDX, IDC_EDIT_MTF_CX_4, pInspection->sMTF.ROI[4].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[4].cx, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MTF_CY_4, pInspection->sMTF.ROI[4].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[4].cy, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MTF_LIMIT_4, pInspection->sMTF.ROI[4].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[4].limit1, 0, 100);

	DDX_Text(pDX, IDC_EDIT_ROI_WIDTH, pInspection->sMTF.ROI_width);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI_width, 0, 100);
	DDX_Text(pDX, IDC_EDIT_ROI_HEIGHT, pInspection->sMTF.ROI_height);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI_height, 0, 100);

	DDX_Text(pDX, IDC_EDIT_CODE_LIMIT_H, CodeLimitH);
	DDV_MinMaxInt(pDX, CodeLimitH, 0, 1024);
	DDX_Text(pDX, IDC_EDIT_CODE_LIMIT_L, CodeLimitL);
	DDV_MinMaxInt(pDX, CodeLimitL, 0, 1024);
	DDX_Check(pDX, IDC_CHECK_POSITION, PosPrompt);

	DDX_Text(pDX, IDC_EDIT_SURROUND_DIFF, pInspection->sMTF.surroundDiffLimit);
	DDV_MinMaxInt(pDX, pInspection->sMTF.surroundDiffLimit, 0, 100);
	DDX_Check(pDX, IDC_CHECK_SURROUND_DIFF, CheckCornerDiff);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnKeyCtrlNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
	ON_BN_CLICKED(IDC_CHECK_RESOLUTION, &OptionDlg::OnBnClickedCheckResolution)
	ON_BN_CLICKED(IDC_CHECK_SEARCH_AREA_ENABLE, &OptionDlg::OnBnClickedFrameOffsetEnable)
END_MESSAGE_MAP()


// OptionDlg 消息处理程序


void OptionDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;

	CDialog::PostNcDestroy();
}

LRESULT OptionDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	BOOL	bSaveAndValidate = (BOOL) wParam;

	BOOL ret = UpdateData(bSaveAndValidate);

	// TODO: Add your specialized code here
	if(bSaveAndValidate)
	{
	}
	else
	{
	}

	return ret;
}

LRESULT OptionDlg::OnKeyCtrlNum(WPARAM wParam, LPARAM lParam)
{
	pInspection->OnKeyCtrlNum();

	return 1;
}

BOOL OptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CComboBox* SearchTime = (CComboBox*)GetDlgItem(IDC_COMBO_SEARCH_TIME);
	SearchTime->AddString(_T("1"));
	SearchTime->AddString(_T("2"));
	SearchTime->AddString(_T("3"));
	SearchTime->AddString(_T("4"));

	OnBnClickedFrameOffsetEnable();
	OnBnClickedCheckResolution();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


afx_msg LRESULT OptionDlg::OnCameraStop(WPARAM wParam, LPARAM lParam)
{
	pInspection->ClearStorage(ClearRecord);
	return 0;
}


void OptionDlg::OnBnClickedCheckResolution()
{
	// TODO: 在此添加控件通知处理程序代码
	int Flag = ((CButton*) (GetDlgItem(IDC_CHECK_RESOLUTION)))->GetCheck();
	if (Flag == BST_CHECKED)
	{
		GetDlgItem(IDC_EDIT_CODE_LIMIT_H)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CODE_LIMIT_L)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_POSITION)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_MTF_LIMIT_0)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MTF_LIMIT_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MTF_LIMIT_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MTF_LIMIT_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MTF_LIMIT_4)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_SURROUND_DIFF)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SURROUND_DIFF)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ROI_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ROI_HEIGHT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CODE_LIMIT_H)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CODE_LIMIT_L)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_POSITION)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_MTF_LIMIT_0)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MTF_LIMIT_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MTF_LIMIT_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MTF_LIMIT_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MTF_LIMIT_4)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_SURROUND_DIFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SURROUND_DIFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ROI_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ROI_HEIGHT)->EnableWindow(FALSE);
	}
}


void OptionDlg::OnBnClickedFrameOffsetEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* cb = (CButton* )GetDlgItem(IDC_CHECK_SEARCH_AREA_ENABLE);
	if (cb->GetCheck() == BST_UNCHECKED)
	{
		SearchArea.Enable = FALSE;
		GetDlgItem(IDC_CHECK_SEARCH_AREA_STEP_CODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SEARCH_AREA_START_CODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SEARCH_AREA_END_CODE)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_START_CODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FRAME_PITCH)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SEARCH_TIME)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_STEP_CODE_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STEP_CODE_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STEP_CODE_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STEP_CODE_4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DESCEND_TIME_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DESCEND_TIME_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DESCEND_TIME_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DESCEND_TIME_4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STEP_DIFF_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STEP_DIFF_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STEP_DIFF_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STEP_DIFF_4)->EnableWindow(TRUE);	

		GetDlgItem(IDC_EDIT_MTF_CX_0)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MTF_CX_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MTF_CX_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MTF_CX_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MTF_CX_4)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_MTF_CY_0)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MTF_CY_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MTF_CY_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MTF_CY_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MTF_CY_4)->EnableWindow(TRUE);
		
// 		GetDlgItem(IDC_CHECK_MTF_0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_CHECK_MTF_1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_CHECK_MTF_2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_CHECK_MTF_3)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_CHECK_MTF_4)->EnableWindow(TRUE);

// 		GetDlgItem(IDC_CHECK_SURROUND_DIFF)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_SURROUND_DIFF)->EnableWindow(TRUE);

// 		GetDlgItem(IDC_EDIT_CODE_LIMIT_L)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_CODE_LIMIT_H)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_CHECK_POSITION)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_MTF_LIMIT_0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_MTF_LIMIT_1)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_MTF_LIMIT_2)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_MTF_LIMIT_3)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_MTF_LIMIT_4)->EnableWindow(TRUE);
	}
	else
	{
		SearchArea.Enable = TRUE;
		GetDlgItem(IDC_CHECK_SEARCH_AREA_STEP_CODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SEARCH_AREA_START_CODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SEARCH_AREA_END_CODE)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_ROI_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ROI_HEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_START_CODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FRAME_PITCH)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SEARCH_TIME)->EnableWindow(FALSE);


		GetDlgItem(IDC_EDIT_STEP_CODE_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STEP_CODE_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STEP_CODE_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STEP_CODE_4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DESCEND_TIME_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DESCEND_TIME_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DESCEND_TIME_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DESCEND_TIME_4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STEP_DIFF_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STEP_DIFF_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STEP_DIFF_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STEP_DIFF_4)->EnableWindow(FALSE);	

		GetDlgItem(IDC_EDIT_MTF_CX_0)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MTF_CX_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MTF_CX_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MTF_CX_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MTF_CX_4)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_MTF_CY_0)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MTF_CY_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MTF_CY_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MTF_CY_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MTF_CY_4)->EnableWindow(FALSE);
		
// 		GetDlgItem(IDC_CHECK_MTF_0)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_CHECK_MTF_1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_CHECK_MTF_2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_CHECK_MTF_3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_CHECK_MTF_4)->EnableWindow(FALSE);

// 		GetDlgItem(IDC_CHECK_SURROUND_DIFF)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_EDIT_SURROUND_DIFF)->EnableWindow(FALSE);

// 		GetDlgItem(IDC_EDIT_CODE_LIMIT_L)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_EDIT_CODE_LIMIT_H)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_CHECK_POSITION)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_EDIT_MTF_LIMIT_0)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_EDIT_MTF_LIMIT_1)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_EDIT_MTF_LIMIT_2)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_EDIT_MTF_LIMIT_3)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_EDIT_MTF_LIMIT_4)->EnableWindow(FALSE);
	}
}
