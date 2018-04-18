// DlgImageProcess.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgImageProcess.h"
#include "afxdialogex.h"


// CDlgImageProcess 对话框

IMPLEMENT_DYNAMIC(CDlgImageProcess, CDialog)

CDlgImageProcess::CDlgImageProcess(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImageProcess::IDD, pParent)
{
	m_bhadInit = FALSE;
}

CDlgImageProcess::~CDlgImageProcess()
{
	if(m_tabWnd1 != NULL)
	{
		delete m_tabWnd1;
	}
	if(m_tabWnd2 != NULL)
	{
		delete m_tabWnd2;
	}
}

void CDlgImageProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabButton);
	DDX_Control(pDX, IDC_PICCON, m_Pic);
	DDX_Control(pDX, IDC_STATIC_STATUE, m_statue);
}


BEGIN_MESSAGE_MAP(CDlgImageProcess, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDlgImageProcess::OnTcnSelchangeTab1)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()




void CDlgImageProcess::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (m_bhadInit)
	{
		//动态设置控件的大小
		CRect rect = CWGetWndRect(this,1);
		int lenOfStatue = 20;
		int lenOfTab = 120;
		CRect rectTab =		CRect(0,0,rect.Width(),lenOfTab);
		CRect rectImage =	CRect(0,lenOfTab,rect.Width(),rect.Height() - lenOfStatue);
		CRect rectText =	CRect(0,rect.Height() - lenOfStatue,rect.Width(),rect.Height());
		m_tabButton.MoveWindow(&rectTab);
		m_Pic.MoveWindow(&rectImage);
		m_statue.MoveWindow(&rectText);
		//链接状态栏到Tab
		if(m_tabWnd1 != NULL)
		{
			CRect TabRect = rectTab;
			//调整tabRect，使其覆盖范围适合放置标签页
			TabRect.left+=2;
			TabRect.right-=0;
			TabRect.top+=25;
			TabRect.bottom-=0;
			m_tabWnd1->SetWindowPos(NULL,TabRect.left,TabRect.top,TabRect.Width(),TabRect.Height(),SWP_SHOWWINDOW);
			m_tabWnd2->SetWindowPos(NULL,TabRect.left,TabRect.top,TabRect.Width(),TabRect.Height(),SWP_SHOWWINDOW);
			m_tabButton.SetCurSel(0);
			ControlDialogShow(10000);
			ControlDialogShow(0);
		}
		//图片看下是否需要重新初始化
		m_Pic.Init(this);
	}
}


BOOL CDlgImageProcess::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitTab();
	m_Pic.Init(this);
	m_bhadInit = TRUE;
	ShowWindow(SW_SHOWMAXIMIZED);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgImageProcess::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	ControlDialogShow(10000);
	int T = m_tabButton.GetCurSel();
	ControlDialogShow(T);
	*pResult = 0;
}

void CDlgImageProcess::ControlDialogShow(int List)
{
	switch(List){
	case 0:
		m_tabWnd1->ShowWindow(SW_SHOW);
		break;
	case 1:
		m_tabWnd2->ShowWindow(SW_SHOW);
		break;
	case 10000:
		m_tabWnd1->ShowWindow(SW_HIDE);
		m_tabWnd2->ShowWindow(SW_HIDE);
		break;
	}
}

void CDlgImageProcess::InitTab()
{
	static const char TAB1NAME[MAX_PATH] = "常规操作";
	static const char TAB2NAME[MAX_PATH] = "功能操作";
	static const char TAB3NAME[MAX_PATH] = "测试窗口3";
	m_tabButton.InsertItem(0,TAB1NAME);
	m_tabButton.InsertItem(1,TAB2NAME);
	CRect TabRect;
	m_tabButton.GetClientRect(&TabRect);	//获取标签控件客户区Rect
	//调整tabRect，使其覆盖范围适合放置标签页
	TabRect.left+=2;
	TabRect.right-=0;
	TabRect.top+=25;
	TabRect.bottom-=0;

	m_tabWnd1 = NULL;
	m_tabWnd1		= new CDlgTabImgChild1;
	m_tabWnd1->Create(IDD_TABIMG_1,&m_tabButton);
	m_tabWnd1->m_papa = this;
	m_tabWnd1->SetWindowPos(NULL,TabRect.left,TabRect.top,TabRect.Width(),TabRect.Height(),SWP_SHOWWINDOW);
	m_tabWnd2 = NULL;
	m_tabWnd2		= new CDlgshuzhi;
	m_tabWnd2->Create(IDD_TABIMG_2,&m_tabButton);
	m_tabWnd2->m_papa = this;
	m_tabWnd2->SetWindowPos(NULL,TabRect.left,TabRect.top,TabRect.Width(),TabRect.Height(),SWP_SHOWWINDOW);

	m_tabButton.SetCurSel(0);
	ControlDialogShow(10000);
	ControlDialogShow(0);
}


void CDlgImageProcess::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_Pic.m_move.MyMouseMove(nFlags,this,point);
	CDialog::OnMouseMove(nFlags, point);
}


BOOL CDlgImageProcess::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_Pic.m_move.MyMouseWheel(nFlags,this,zDelta,pt);
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CDlgImageProcess::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Pic.m_move.MyLButtonDown(nFlags,this,point);
	CDialog::OnLButtonDown(nFlags, point);
}


void CDlgImageProcess::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_Pic.m_move.MyLButtonUp(nFlags,this,point);
	CDialog::OnLButtonUp(nFlags, point);
}
