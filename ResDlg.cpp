// ResDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CLASSTEST.h"
#include "ResDlg.h"
#include "afxdialogex.h"


// CResDlg 对话框

IMPLEMENT_DYNAMIC(CResDlg, CDialog)

CResDlg::CResDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResDlg::IDD, pParent)
	, m_strRes(_T(""))
{

}

CResDlg::~CResDlg()
{
}

void CResDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RESSTR, m_strRes);
	DDX_Control(pDX, IDC_SHOW, m_Show);
}


BEGIN_MESSAGE_MAP(CResDlg, CDialog)
END_MESSAGE_MAP()


CCriticalSection g_statuslock;
void CResDlg::AddStatus(CString status)
{
	g_statuslock.Lock();
	CFile test;
	CString ttt;
	SYSTEMTIME time;
	GetLocalTime(&time);
	ttt.Format("%d/%d/%d/ %d:%d:%d:%s\r\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,status);
	TRACE(ttt);
// 	test.Open(_T("LOG\\log.txt"),CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite);
// 	test.SeekToEnd();
// 	test.Write(ttt,ttt.GetLength());
// 	test.Close();

	GetDlgItemText(IDC_EDIT_RESSTR,m_strRes);
	m_strRes += ttt;
	SetDlgItemText(IDC_EDIT_RESSTR,m_strRes);
	g_statuslock.Unlock();
}


// CResDlg 消息处理程序


BOOL CResDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	EnableVisualManagerStyle();
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
