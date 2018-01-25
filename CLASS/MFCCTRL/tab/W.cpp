// W.cpp : 实现文件
//

#include "stdafx.h"
#include "W.h"
#include "afxdialogex.h"


// CW 对话框

IMPLEMENT_DYNAMIC(CW, CDialogEx)

CW::CW(CWnd* pParent /*=NULL*/)
	: CDialogEx(CW::IDD, pParent)
{

}

CW::~CW()
{
}

void CW::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CW, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CW::OnBnClickedButton1)
END_MESSAGE_MAP()


// CW 消息处理程序


void CW::OnBnClickedButton1()
{
	AfxMessageBox("我是b的第一个窗口");
}
