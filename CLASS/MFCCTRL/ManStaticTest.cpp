// D:\app\rbj\now\CWCLASS\CLASS\MFCCTRL\ManStaticTest.cpp : 实现文件
//

#include "stdafx.h"
#include "CLASS\MFCCTRL\ManStaticTest.h"
#include "XFunCom.h"
#include "afxdialogex.h"


// CManStaticTest 对话框

IMPLEMENT_DYNAMIC(CManStaticTest, CDialogEx)

CManStaticTest::CManStaticTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManStaticTest::IDD, pParent)
{
	m_bDrawingDeteRC = FALSE;
	m_bLbtnDown = FALSE;
}

CManStaticTest::~CManStaticTest()
{
}

void CManStaticTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOW, m_show);
}


BEGIN_MESSAGE_MAP(CManStaticTest, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CManStaticTest::OnBnClickedButton1)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON2, &CManStaticTest::OnBnClickedButton2)
END_MESSAGE_MAP()


// CManStaticTest 消息处理程序


void CManStaticTest::OnBnClickedButton1()
{
	CString picPath = GetDirPathByDialog("",NULL,1);
	m_show.ChangeImg(picPath);
}





void CManStaticTest::OnLButtonDown(UINT nFlags, CPoint point)
{
	UpdateData();
	CRect PicRect;
	if (m_ShowRC.PtInRect(point))
	{
		if(m_bDrawingDeteRC){
			point.x-=m_ShowRC.left;
			point.y-=m_ShowRC.top;
			m_bLbtnDown = TRUE;
			m_stPt.x=m_endPt.x=point.x;///m_show.ImageScale2+0.5;
			m_stPt.y=m_endPt.y=point.y;///m_show.ImageScale2+0.5;

			m_show.ShowImage();
			return;
		}
		else if(TranParentToChild(m_show,this, point))
			m_show.OnLButtonDown(nFlags,point);

	} 
	
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CManStaticTest::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_ShowRC.PtInRect(point))
	{
		if (m_bLbtnDown&&(m_bDrawingDeteRC))
		{

			point.x-=m_ShowRC.left;
			point.y-=m_ShowRC.top;


			m_endPt.x=point.x;///m_show.ImageScale2+0.5;
			m_endPt.y=point.y;///m_show.ImageScale2+0.5;
			m_show.m_Rects[0].clear();
			m_show.m_Circle[0].clear();
			//m_show.myRectangle(CRect(m_stPt,m_endPt),RGB(255,255,255));
			m_show.myCircle(m_stPt.x,m_stPt.y,sqrt(pow(double(m_endPt.x-m_stPt.x),2.0)+pow(double(m_endPt.y-m_stPt.y),2.0)));
			m_show.ShowImage();
			Invalidate();
			return;
		}else if(TranParentToChild(m_show,this, point)){
			m_show.OnMouseMove(nFlags,point);
			Invalidate();
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CManStaticTest::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bLbtnDown&&m_bDrawingDeteRC){
		CRect rect(m_stPt,m_endPt);
		m_bLbtnDown = FALSE;
		m_bDrawingDeteRC = FALSE;
	}
	else if(TranParentToChild(m_show,this, point))
		m_show.OnLButtonUp(nFlags,point);
	CDialogEx::OnLButtonUp(nFlags, point);
}


BOOL CManStaticTest::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(TranParentToChild(m_show,this, pt,1))
		m_show.OnMouseWheel(nFlags, zDelta, pt);
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CManStaticTest::OnBnClickedButton2()
{
	m_bDrawingDeteRC = TRUE;
}


BOOL CManStaticTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	m_show.GetWindowRect(m_ShowRC);
	ScreenToClient(m_ShowRC);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
