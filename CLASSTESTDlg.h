
// CLASSTESTDlg.h : ͷ�ļ�
//

#pragma once
#include "BOSSCLASS.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "DlgImageProcess.h"
// CCLASSTESTDlg �Ի���
#ifdef BCG
#define CDialog CBCGPDialog
#endif
class CCLASSTESTDlg : public CDialog
{
// ����
public:
	CCLASSTESTDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLASSTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:

	BOSSCLASS m_boss;								//��Ҫ�࣬�������ȸ���������

	afx_msg void OnBnClickedButtonStart();

	void RunSellf();
#ifdef BCG
	CBCGPComboBox m_cbSupportClass;
	CBCGPComboBox m_cbFunction;
	CBCGPListCtrl m_ListInput;
#else
	CComboBox m_cbSupportClass;
	CComboBox m_cbFunction;
	CListCtrl m_ListInput;
#endif
	afx_msg void OnCbnSelchangeCombo2();
	CString m_strHelp;
	CString m_strInput;
	afx_msg void OnBnClickedButtonInput();
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
	afx_msg void OnBnClickedButtonVision();
};
