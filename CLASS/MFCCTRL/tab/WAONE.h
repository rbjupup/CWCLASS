#pragma once
#include "resource.h"

// CWAONE �Ի���

class CWAONE : public CDialogEx
{
	DECLARE_DYNAMIC(CWAONE)

public:
	CWAONE(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWAONE();

// �Ի�������
	enum { IDD = IDD_DIALOG_TAB_ONE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
