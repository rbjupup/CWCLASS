// DlgTabImgChild1.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgTabImgChild1.h"
#include "afxdialogex.h"
#include "DlgImageProcess.h"

// CDlgTabImgChild1 对话框

IMPLEMENT_DYNAMIC(CDlgTabImgChild1, CDialog)

CDlgTabImgChild1::CDlgTabImgChild1(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTabImgChild1::IDD, pParent)
{
	m_papa = NULL;
}

CDlgTabImgChild1::~CDlgTabImgChild1()
{
}

void CDlgTabImgChild1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTabImgChild1, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTabImgChild1::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgTabImgChild1 消息处理程序


void CDlgTabImgChild1::OnBnClickedButton1()
{
	((CDlgImageProcess *)m_papa)->m_Pic.ChangeImg(GetDirPathByDialog("",NULL,1),2);
}
