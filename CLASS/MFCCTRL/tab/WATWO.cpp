// D:\app\rbj\now\CWCLASS\CLASS\MFCCTRL\tab\WATWO.cpp : 实现文件
//

#include "stdafx.h"
#include "CLASS\MFCCTRL\tab\WATWO.h"
#include "afxdialogex.h"


// CWATWO 对话框

IMPLEMENT_DYNAMIC(CWATWO, CDialogEx)

CWATWO::CWATWO(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWATWO::IDD, pParent)
{

}

CWATWO::~CWATWO()
{
}

void CWATWO::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWATWO, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWATWO::OnBnClickedButton1)
END_MESSAGE_MAP()


// CWATWO 消息处理程序


void CWATWO::OnBnClickedButton1()
{
	AfxMessageBox("我是a的第2个窗口");
}
