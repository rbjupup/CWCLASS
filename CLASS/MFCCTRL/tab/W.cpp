// W.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "W.h"
#include "afxdialogex.h"


// CW �Ի���

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


// CW ��Ϣ�������


void CW::OnBnClickedButton1()
{
	AfxMessageBox("����b�ĵ�һ������");
}
