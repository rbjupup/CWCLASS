#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "CLASS/MFCCTRL/ImageStatic.h"
// CResDlg �Ի���

#define CDialog CBCGPDialog
class CResDlg : public CDialog
{
	DECLARE_DYNAMIC(CResDlg)

public:
	CResDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CResDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SHOWRESULT };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	void AddStatus(CString);
	CString m_strRes;
	CImageStatic m_Show;
	virtual BOOL OnInitDialog();
	int newValueIndex;
	CBCGPSliderCtrl m_newValSlider;
	afx_msg void OnNMReleasedcaptureSlidervalue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButtonMax();
};
