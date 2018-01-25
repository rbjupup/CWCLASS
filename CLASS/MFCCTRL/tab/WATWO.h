#pragma once
#include "resource.h"

// CWATWO 对话框

class CWATWO : public CDialogEx
{
	DECLARE_DYNAMIC(CWATWO)

public:
	CWATWO(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWATWO();

// 对话框数据
	enum { IDD = IDD_DIALOG_TAB_TWO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
