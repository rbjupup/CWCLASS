#pragma once
#include "resource.h"
#include "afxwin.h"

// CResDlg 对话框

#define CDialog CBCGPDialog
class CResDlg : public CDialog
{
	DECLARE_DYNAMIC(CResDlg)

public:
	CResDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SHOWRESULT };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	void AddStatus(CString);
	CString m_strRes;
	CStatic m_Show;
	virtual BOOL OnInitDialog();
};
