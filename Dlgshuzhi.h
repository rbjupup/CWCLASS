#pragma once
#include "resource.h"

// CDlgshuzhi 对话框

class CDlgshuzhi : public CDialog
{
	DECLARE_DYNAMIC(CDlgshuzhi)

public:
	CDlgshuzhi(CWnd* pParent = NULL);   // 标准构造函数
	CWnd *m_papa;
	virtual ~CDlgshuzhi();

// 对话框数据
	enum { IDD = IDD_TABIMG_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
