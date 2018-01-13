
// CLASSTESTDlg.h : 头文件
//

#pragma once
#include "BOSSCLASS.h"
#include "afxwin.h"
#include "afxcmn.h"

// CCLASSTESTDlg 对话框
#define CDialog CBCGPDialog
class CCLASSTESTDlg : public CDialog
{
// 构造
public:
	CCLASSTESTDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLASSTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:

	BOSSCLASS m_boss;								//主要类，用来调度各个测试类

	afx_msg void OnBnClickedButtonStart();

	void RunSellf();

	CBCGPComboBox m_cbSupportClass;
	CBCGPComboBox m_cbFunction;
	afx_msg void OnCbnSelchangeCombo2();
	CString m_strHelp;
	CString m_strInput;
	afx_msg void OnBnClickedButtonInput();
	CBCGPListCtrl m_ListInput;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton2();
	CEdit m_edit;
	afx_msg void OnEnKillfocusEditTmp();


	int m_row,m_column;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonu();
	afx_msg void OnBnClickedButtond();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonInput2();
	afx_msg void OnBnClickedButtonInput3();
};
