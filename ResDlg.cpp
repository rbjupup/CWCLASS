// ResDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CLASSTEST.h"
#include "ResDlg.h"
#include "afxdialogex.h"
#include "CLASSTESTDlg.h"

// CResDlg 对话框

IMPLEMENT_DYNAMIC(CResDlg, CDialog)

CResDlg::CResDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResDlg::IDD, pParent)
	, m_strRes(_T(""))
	, newValueIndex(0)
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
	DDX_Text(pDX, IDC_EDIT1, newValueIndex);
	DDX_Control(pDX, IDC_SLIDER1, m_newValSlider);
}


BEGIN_MESSAGE_MAP(CResDlg, CDialog)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CResDlg::OnNMReleasedcaptureSlidervalue)
	ON_BN_CLICKED(IDC_BUTTON3, &CResDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CResDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT1, &CResDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON_MAX, &CResDlg::OnBnClickedButtonMax)
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
	m_newValSlider.SetRange(0,255);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CResDlg::OnNMReleasedcaptureSlidervalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData();
	*pResult = 0;
	int newval = m_newValSlider.GetPos();
	CString newstr;
	newstr.Format("%d",newval);
	GetDlgItem(IDC_STATIC_NEWVAL)->SetWindowText(newstr);
	((CCLASSTESTDlg*)(theApp.m_pMainWnd))->m_ListInput.SetItemText(newValueIndex,1,newstr);
	((CCLASSTESTDlg*)(theApp.m_pMainWnd))->OnBnClickedButtonStart();
}


void CResDlg::OnBnClickedButton3()
{
	UpdateData();
	CString before = ((CCLASSTESTDlg*)(theApp.m_pMainWnd))->m_ListInput.GetItemText(newValueIndex,1);
	CString after;
	after.Format("%d",atoi(before)-1);
	((CCLASSTESTDlg*)(theApp.m_pMainWnd))->m_ListInput.SetItemText(newValueIndex,1,after);
	m_newValSlider.SetPos(atoi(after));
	GetDlgItem(IDC_STATIC_NEWVAL)->SetWindowText(after);

	((CCLASSTESTDlg*)(theApp.m_pMainWnd))->OnBnClickedButtonStart();
}


void CResDlg::OnBnClickedButton2()
{
	UpdateData();
	CString before = ((CCLASSTESTDlg*)(theApp.m_pMainWnd))->m_ListInput.GetItemText(newValueIndex,1);
	CString after;
	after.Format("%d",atoi(before)+1);
	((CCLASSTESTDlg*)(theApp.m_pMainWnd))->m_ListInput.SetItemText(newValueIndex,1,after);
	m_newValSlider.SetPos(atoi(after));
	GetDlgItem(IDC_STATIC_NEWVAL)->SetWindowText(after);

	((CCLASSTESTDlg*)(theApp.m_pMainWnd))->OnBnClickedButtonStart();
}


void CResDlg::OnEnChangeEdit1()
{
	UpdateData();
	CString newstr;
	newstr = ((CCLASSTESTDlg*)(theApp.m_pMainWnd))->m_ListInput.GetItemText(newValueIndex,1);
	int newval = atoi(newstr);
	if(newval == 0)
		return
	m_newValSlider.SetPos(newval);
	newstr.Format("%d",newval);
	GetDlgItem(IDC_STATIC_NEWVAL)->SetWindowText(newstr);
}


void CResDlg::OnBnClickedButtonMax()
{
	CString maxval;
	GetDlgItem(IDC_EDIT_SLIDERMAX)->GetWindowText(maxval);
	m_newValSlider.SetRange(0,atoi(maxval),1);
}
