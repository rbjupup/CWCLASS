#pragma once
#include "resource.h"

// CW 对话框

class CW : public CDialogEx
{
	DECLARE_DYNAMIC(CW)

public:
	CW(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CW();

// 对话框数据
	enum { IDD = IDD_DIALOG_TAB_B };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
