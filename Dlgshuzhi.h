#pragma once
#include "resource.h"

// CDlgshuzhi �Ի���

class CDlgshuzhi : public CDialog
{
	DECLARE_DYNAMIC(CDlgshuzhi)

public:
	CDlgshuzhi(CWnd* pParent = NULL);   // ��׼���캯��
	CWnd *m_papa;
	virtual ~CDlgshuzhi();

// �Ի�������
	enum { IDD = IDD_TABIMG_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
