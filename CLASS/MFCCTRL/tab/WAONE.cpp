// D:\app\rbj\now\CWCLASS\CLASS\MFCCTRL\tab\WAONE.cpp : 实现文件
//

#include "stdafx.h"
#include "D:\app\rbj\now\CWCLASS\CLASS\MFCCTRL\tab\WAONE.h"
#include "afxdialogex.h"


// CWAONE 对话框

IMPLEMENT_DYNAMIC(CWAONE, CDialogEx)

CWAONE::CWAONE(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWAONE::IDD, pParent)
{

}

CWAONE::~CWAONE()
{
}

void CWAONE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWAONE, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWAONE::OnBnClickedButton1)
END_MESSAGE_MAP()


// CWAONE 消息处理程序


void CWAONE::OnBnClickedButton1()
{
	AfxMessageBox("我是a的第一个窗口");
}
