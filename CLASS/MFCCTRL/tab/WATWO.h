#pragma once
#include "resource.h"

// CWATWO �Ի���

class CWATWO : public CDialogEx
{
	DECLARE_DYNAMIC(CWATWO)

public:
	CWATWO(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWATWO();

// �Ի�������
	enum { IDD = IDD_DIALOG_TAB_TWO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
