#pragma once
#include <gdiplus.h>  
#include "CLASS/MFCCTRL/MemDC.h"
#include "CLASS/MFCCTRL/CGdiPlusBitmap.h"
#include "CLASS/MFCCTRL/GdipButton.h"
#include "afxbutton.h"


// CWMFCBtn 对话框

class CWMFCBtn : public CDialog
{
	DECLARE_DYNAMIC(CWMFCBtn)

public:
	CWMFCBtn(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWMFCBtn();



// 对话框数据
	enum { IDD = IDD_DIALOG_Btn };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	Gdiplus::Bitmap m_Bg1,m_Bg2;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonUnnormal2();
	CGdipButton m_btnPng;
};
