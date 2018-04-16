// D:\app\rbj\now\CWCLASS\CLASS\MFCCTRL\ManStaticTest.cpp : 实现文件
//

#include "stdafx.h"
#include "CLASS\MFCCTRL\ManStaticTest.h"
#include "CLASS\CV\WPowerCV.h"
#include "XFunCom.h"
#include "afxdialogex.h"


// CManStaticTest 对话框

IMPLEMENT_DYNAMIC(CManStaticTest, CDialogEx)

CManStaticTest::CManStaticTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManStaticTest::IDD, pParent)
{

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
	ON_BN_CLICKED(IDC_BUTTON3, &CManStaticTest::OnBnClickedButton3)
	ON_WM_TIMER()
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

	m_show.m_move.MyLButtonDown(nFlags,this,point);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CManStaticTest::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_show.m_move.MyMouseMove(nFlags,this,point);

	CDialogEx::OnMouseMove(nFlags, point);
}


void CManStaticTest::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_show.m_move.MyLButtonUp(nFlags,this,point);

	CDialogEx::OnLButtonUp(nFlags, point);
}


BOOL CManStaticTest::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_show.m_move.MyMouseWheel(nFlags,this,zDelta,pt);
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CManStaticTest::OnBnClickedButton2()
{
	m_show.m_Rects[0][0] = DRAWRECT();
	m_show.m_Circle[0][0] = DRAWCircle();
	m_show.m_move.m_bDrawingDeteRC = TRUE;
}


BOOL CManStaticTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetTimer(1,1000,NULL);
	m_show.Init(this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CManStaticTest::OnBnClickedButton3()
{
	CWPowerCV cwcv;
	//cv::Mat mat;
	//cwcv.FileToCv(mat,"D:\\tmp\\1.xml",1);

	cv::Mat smat(500,500,CV_8UC1);
	cv::Mat mat = imread("D:\\tmp\\941349.bmp",0);

	smat = mat(Range(349,/*349+500*/471),Range(941,/*941+500*/1078));
	m_show.ChangeImgGL(smat);
	m_show.ShowImageGL();
}


void CManStaticTest::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//m_show.ShowImageGL();
	CDialogEx::OnTimer(nIDEvent);
}
