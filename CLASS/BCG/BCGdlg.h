#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "BCGPVisualCtrl.h"
#include "BCGPChartVisualObject.h"


// ABCGDlg 对话框
#ifdef BCG
#define CDialog CBCGPDialog
#endif

#define MAX_SERIES 5
class ABCGDlg : public CDialog
{
	DECLARE_DYNAMIC(ABCGDlg)

public:
	ABCGDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ABCGDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_BCG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

#ifdef BCG
//波形图绘制所需
	CBCGPChartCtrl m_wndChart;
	CBCGPChartSeries*	m_arSeries[1];
//波形图绘制所需End
#endif

	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart();
};
