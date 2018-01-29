#pragma once

#include "resource.h"
#include "ImageStatic.h"
#include "afxwin.h"
// CManStaticTest 对话框

class CManStaticTest : public CDialogEx
{
	DECLARE_DYNAMIC(CManStaticTest)

public:
	CManStaticTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CManStaticTest();

// 对话框数据
	enum { IDD = IDD_DIALOG_CTL_STATIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CImageStatic m_show;
	CRect m_ShowRC;
	BOOL m_bLbtnDown;
	CPoint m_stPt;
	CPoint m_endPt;
	BOOL m_bDrawingDeteRC;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
};
