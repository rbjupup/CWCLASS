// MFCTAB.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTAB.h"
#include "afxdialogex.h"


// CMFCTAB 对话框

IMPLEMENT_DYNAMIC(CMFCTAB, CDialog)

CMFCTAB::CMFCTAB(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCTAB::IDD, pParent)
{
	m_pwaOne = NULL;
	m_pwaTwo = NULL;
	m_pwBOne = NULL;
	showtype = 0;
}

CMFCTAB::~CMFCTAB()
{
	if(m_pwaOne != NULL)
		delete m_pwaOne;
	if(m_pwaTwo != NULL)
		delete m_pwaTwo;
	if(m_pwBOne != NULL)
		delete m_pwBOne;
}

void CMFCTAB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabSwitch);
}


BEGIN_MESSAGE_MAP(CMFCTAB, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCTAB::OnBnClickedButton1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMFCTAB::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CMFCTAB 消息处理程序


void CMFCTAB::OnBnClickedButton1()
{
	AfxMessageBox("我是主窗口");
}


void CMFCTAB::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	ControlDialogShow(10000);
	int T = m_TabSwitch.GetCurSel();
	ControlDialogShow(T);
	*pResult = 0;
}
void CMFCTAB::ControlDialogShow(int List)
{
	if(showtype == 0){
		switch(List){
		case 0:
			m_pwaOne->ShowWindow(SW_SHOW);
			break;
		case 1:
			m_pwaTwo->ShowWindow(SW_SHOW);
			break;
		case 10000:
			m_pwaOne->ShowWindow(SW_HIDE);
			m_pwaTwo->ShowWindow(SW_HIDE);
			m_pwBOne->ShowWindow(SW_HIDE);
			break;
		}
	}
	else{
		switch(List){
		case 0:
			m_pwBOne->ShowWindow(SW_SHOW);
			break;
		case 1:
			m_pwaOne->ShowWindow(SW_SHOW);
			break;
		case 2:
			m_pwaTwo->ShowWindow(SW_SHOW);
			break;
		case 10000:
			m_pwaOne->ShowWindow(SW_HIDE);
			m_pwaTwo->ShowWindow(SW_HIDE);
			m_pwBOne->ShowWindow(SW_HIDE);
			break;
		}

	}
}

BOOL CMFCTAB::OnInitDialog()
{
	CDialog::OnInitDialog();
	switch(showtype){
	case 0:
		m_TabSwitch.InsertItem(0,_T("a0"));
		m_TabSwitch.InsertItem(1,_T("a1"));
		break;
	case 1:
		m_TabSwitch.InsertItem(0,_T("b0"));
		m_TabSwitch.InsertItem(1,_T("a0"));
		m_TabSwitch.InsertItem(2,_T("a1"));
		break;
	}
	
	CRect TabRect;
	m_TabSwitch.GetClientRect(&TabRect);	//获取标签控件客户区Rect
	//调整tabRect，使其覆盖范围适合放置标签页
	TabRect.left+=2;
	TabRect.right-=0;
	TabRect.top+=25;
	TabRect.bottom-=0;

	m_pwaOne		= new CWAONE;
	m_pwaOne->Create(IDD_DIALOG_TAB_ONE,&m_TabSwitch);
	m_pwaOne->SetWindowPos(NULL,TabRect.left,TabRect.top,TabRect.Width(),TabRect.Height(),SWP_SHOWWINDOW);
	m_pwaTwo		= new CWATWO;
	m_pwaTwo->Create(IDD_DIALOG_TAB_TWO,&m_TabSwitch);
	m_pwaTwo->SetWindowPos(NULL,TabRect.left,TabRect.top,TabRect.Width(),TabRect.Height(),SWP_SHOWWINDOW);
	m_pwBOne		= new CW;
	m_pwBOne->Create(IDD_DIALOG_TAB_B,&m_TabSwitch);
	m_pwBOne->SetWindowPos(NULL,TabRect.left,TabRect.top,TabRect.Width(),TabRect.Height(),SWP_SHOWWINDOW);

	m_TabSwitch.SetCurSel(0);
	ControlDialogShow(10000);
	ControlDialogShow(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
