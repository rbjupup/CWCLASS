#pragma once
#include "resource.h"

// CDlgTabImgChild1 �Ի���

class CDlgTabImgChild1 : public CDialog
{
	DECLARE_DYNAMIC(CDlgTabImgChild1)

public:
	CDlgTabImgChild1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTabImgChild1();

// �Ի�������
	enum { IDD = IDD_TABIMG_1 };
	CWnd* m_papa;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
