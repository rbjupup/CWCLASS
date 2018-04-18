#pragma once
#include "resource.h"
#include "XFunCom.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "DlgTabImgChild1.h"
#include "CLASS/MFCCTRL/ImageStatic.h"
#include "Dlgshuzhi.h"
// CDlgImageProcess 对话框
class CDlgImageProcess : public CDialog
{
	DECLARE_DYNAMIC(CDlgImageProcess)

public:
	CDlgImageProcess(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgImageProcess();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);


	virtual BOOL OnInitDialog();
	BOOL m_bhadInit;			//没初始化控件之前,一些操作会导致错误
	CImageStatic m_Pic;
	CStatic m_statue;

/************************************************************************/
/*                    TAB操作开始                                       */
/************************************************************************/
	static const int TABWINNUM = 3;
	CDlgTabImgChild1* m_tabWnd1;
	CDlgshuzhi* m_tabWnd2;
	CTabCtrl m_tabButton;
	void InitTab();
	void ControlDialogShow(int List);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
/************************************************************************/
/*                    TAB操作结束                                       */
/************************************************************************/

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
