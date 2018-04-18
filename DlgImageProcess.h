#pragma once
#include "resource.h"
#include "XFunCom.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "DlgTabImgChild1.h"
#include "CLASS/MFCCTRL/ImageStatic.h"
#include "Dlgshuzhi.h"
// CDlgImageProcess �Ի���
class CDlgImageProcess : public CDialog
{
	DECLARE_DYNAMIC(CDlgImageProcess)

public:
	CDlgImageProcess(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgImageProcess();

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);


	virtual BOOL OnInitDialog();
	BOOL m_bhadInit;			//û��ʼ���ؼ�֮ǰ,һЩ�����ᵼ�´���
	CImageStatic m_Pic;
	CStatic m_statue;

/************************************************************************/
/*                    TAB������ʼ                                       */
/************************************************************************/
	static const int TABWINNUM = 3;
	CDlgTabImgChild1* m_tabWnd1;
	CDlgshuzhi* m_tabWnd2;
	CTabCtrl m_tabButton;
	void InitTab();
	void ControlDialogShow(int List);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
/************************************************************************/
/*                    TAB��������                                       */
/************************************************************************/

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
