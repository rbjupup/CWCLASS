#pragma once
#include "resource.h"

// CW �Ի���

class CW : public CDialogEx
{
	DECLARE_DYNAMIC(CW)

public:
	CW(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CW();

// �Ի�������
	enum { IDD = IDD_DIALOG_TAB_B };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
