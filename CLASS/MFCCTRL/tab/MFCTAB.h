#pragma once
#include "resource.h"
#include "WAONE.h"
#include "WATWO.h"
#include "W.h"
#include "afxcmn.h"
// CMFCTAB �Ի���

class CMFCTAB : public CDialog
{
	DECLARE_DYNAMIC(CMFCTAB)

public:
	CMFCTAB(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMFCTAB();

// �Ի�������
	enum { IDD = IDD_DIALOG_TAB_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	void ControlDialogShow(int List);
	virtual BOOL OnInitDialog();
	CWAONE* m_pwaOne;
	CWATWO* m_pwaTwo;
	CW* m_pwBOne;
	int showtype;
	CTabCtrl m_TabSwitch;
};
