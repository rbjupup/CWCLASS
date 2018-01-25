#pragma once
#include "resource.h"

// CWAONE 对话框

class CWAONE : public CDialogEx
{
	DECLARE_DYNAMIC(CWAONE)

public:
	CWAONE(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWAONE();

// 对话框数据
	enum { IDD = IDD_DIALOG_TAB_ONE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
