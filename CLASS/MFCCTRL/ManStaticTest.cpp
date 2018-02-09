// D:\app\rbj\now\CWCLASS\CLASS\MFCCTRL\ManStaticTest.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CLASS\MFCCTRL\ManStaticTest.h"
#include "XFunCom.h"
#include "afxdialogex.h"


// CManStaticTest �Ի���

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
END_MESSAGE_MAP()


// CManStaticTest ��Ϣ�������


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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

	

	m_show.m_move.MyInit(this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
