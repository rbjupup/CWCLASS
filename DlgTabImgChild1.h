#pragma once
#include "resource.h"

// CDlgTabImgChild1 对话框

class CDlgTabImgChild1 : public CDialog
{
	DECLARE_DYNAMIC(CDlgTabImgChild1)

public:
	CDlgTabImgChild1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTabImgChild1();

// 对话框数据
	enum { IDD = IDD_TABIMG_1 };
	CWnd* m_papa;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
