// D:\app\rbj\now\CWCLASS\CLASS\MFCCTRL\tab\WATWO.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CLASS\MFCCTRL\tab\WATWO.h"
#include "afxdialogex.h"


// CWATWO �Ի���

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


// CWATWO ��Ϣ�������


void CWATWO::OnBnClickedButton1()
{
	AfxMessageBox("����a�ĵ�2������");
}
