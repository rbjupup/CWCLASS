// Dlgshuzhi.cpp : 实现文件
//

#include "stdafx.h"
#include "Dlgshuzhi.h"
#include "afxdialogex.h"
#include "DlgImageProcess.h"


// CDlgshuzhi 对话框

IMPLEMENT_DYNAMIC(CDlgshuzhi, CDialog)

CDlgshuzhi::CDlgshuzhi(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgshuzhi::IDD, pParent)
{
	m_papa = NULL;
}

CDlgshuzhi::~CDlgshuzhi()
{
}

void CDlgshuzhi::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgshuzhi, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgshuzhi::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgshuzhi 消息处理程序


void CDlgshuzhi::OnBnClickedButton1()
{
	((CDlgImageProcess *)m_papa)->m_Pic.ChangeImg(GetDirPathByDialog("",NULL,1),2);
}
