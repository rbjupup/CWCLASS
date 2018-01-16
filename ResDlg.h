#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "CLASS/MFCCTRL/ImageStatic.h"
// CResDlg 对话框
#ifdef BCG
#define CDialog CBCGPDialog
#endif
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
	CImageStatic m_Show;
	virtual BOOL OnInitDialog();
	int newValueIndex;
#ifdef BCG
	CBCGPSliderCtrl m_newValSlider;
#else
	CSliderCtrl m_newValSlider;

#endif
	afx_msg void OnNMReleasedcaptureSlidervalue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButtonMax();
};
