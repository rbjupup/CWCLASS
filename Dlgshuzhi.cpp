// Dlgshuzhi.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dlgshuzhi.h"
#include "afxdialogex.h"
#include "DlgImageProcess.h"


// CDlgshuzhi �Ի���

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


// CDlgshuzhi ��Ϣ�������


void CDlgshuzhi::OnBnClickedButton1()
{
	((CDlgImageProcess *)m_papa)->m_Pic.ChangeImg(GetDirPathByDialog("",NULL,1),2);
}
