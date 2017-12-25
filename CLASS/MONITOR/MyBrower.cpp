// MyBrower.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyBrower.h"
#include "afxdialogex.h"
#include <atlbase.h>
CComModule _Module;
#include <atlwin.h>
#include <windows.h>
#include <vector>
using namespace std;
#pragma comment(lib,"atl")
#pragma comment(lib,"User32.lib")
// CMyBrower �Ի���
int g_readnum;
IMPLEMENT_DYNAMIC(CMyBrower, CDialog)

CMyBrower::CMyBrower(CWnd* pParent /*=NULL*/)
	: CDialog(CMyBrower::IDD, pParent)
{
	//�����ַ
	path = CString("http://hotels.ctrip.com/hotel/669353.html");
	//html����ͷ��
	m_filterFrontData.push_back("<DIV class=hrd-title>");
	m_filterFrontData.push_back("<P class=base_box><SPAN class=base_price><DFN>��</DFN>");
	//html����β��
	m_filterBackData.push_back("</DIV>");
	m_filterBackData.push_back("</SPAN></P>");
	//�Ƿ�Ҫ��ʾ��ҳ
	m_bshowWeb = FALSE;
	//���Ӧ�����ٸ����������ж���ҳ�Ƿ��Ѿ��������
	m_nNumOfLessRes = 20;
	//�ˢ��ʱ�䣬������ʱ�仹û�л�õ��㹻��Ľ����ʱ���˳����ڣ��������������
	m_dMaxMillTime = 20000;
	g_readnum = 0;
	m_resTotalNum.clear();
	m_resTotalType.clear();
}

CMyBrower::~CMyBrower()
{
}

void CMyBrower::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_browseer);
	DDX_Control(pDX, IDC_BUTTON1, m_testBotton);
}


BEGIN_MESSAGE_MAP(CMyBrower, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyBrower::OnBnClickedButton1)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()


// CMyBrower ��Ϣ�������


void CMyBrower::OnBnClickedButton1()
{
	if (m_filterBackData.size() != m_filterFrontData.size())
	{
		AfxMessageBox("����ͷ������β��Ŀ��ƥ�䣬����������");
		return;
	}
	//��ʼ���������
	m_res.clear();
	m_resNum.clear();
	for (int i = 0 ; i < m_filterFrontData.size();i++)
	{
		vector<CString> tmp;
		m_res.push_back(tmp);
		vector<int> tmpnum;
		m_resNum.push_back(tmpnum);
	}

	HRESULT hr;  
	IDispatch* lpDispatch;  
	lpDispatch = m_browseer.get_Document();  
	IHTMLDocument2* lpDocument2;  
	hr = lpDispatch->QueryInterface(IID_IHTMLDocument2, (PVOID*)&lpDocument2);  
	if ( hr == S_OK )  
	{  


		IHTMLElement * pBody;  
		lpDocument2->get_body(&pBody);  
		BSTR html;//���htmlԴ����  
		CComBSTR html_t;//���ڽ�BSTRת��Ϊcout���Դ�����ַ���  
		pBody->get_innerHTML(&html);  
		CString strCookie(html);  
		CFile myfile("TMP\\1.txt",CFile::modeWrite|CFile::modeCreate);  
		myfile.Write(strCookie,strCookie.GetLength());  
		myfile.Close();  
		pBody->Release();  
		lpDocument2->Release();  
	}  
	lpDispatch->Release(); 

	CStdioFile m_StFile;
	if (!FileExist("TMP\\1.txt"))
	{
		AfxMessageBox(_T("��ȡ����html��Ϣ����ʧ��!"));
		return;
	}
	int  m_nRows = 0;
	m_StFile.Open("TMP\\1.txt",CFile::modeRead);
	if (m_StFile == NULL)
	{
		AfxMessageBox(_T("��ȡ����html��Ϣ��ʧ��!"));
		return;
	}
	CString str;
	vector<CString> SrcData;
	while (m_StFile.ReadString(str))					//���ж�ȡ
	{
		//TRACE("%s\n",str);
		SrcData.push_back(str);
		m_nRows ++;
	}
	m_StFile.Close();
	for (int i = 0; i < SrcData.size();i++)
	{
		for (int j = 0; j < m_filterFrontData.size();j++)
		{
			int front = SrcData[i].Find(m_filterFrontData[j]);
			int back = SrcData[i].Find(m_filterBackData[j],front==-1?0:front + m_filterFrontData[j].GetLength());
			if ((front != -1)&&(back != -1))
			{
				CString tmp = SrcData[i].Mid(front + m_filterFrontData[j].GetLength(),back - m_filterFrontData[j].GetLength() - front);
				m_res[j].push_back(tmp);
				m_resNum[j].push_back(i);
				m_resTotal.push_back(tmp);
				m_resTotalNum.push_back(i);
				m_resTotalType.push_back(j);
			}
		}
	}
}




void CMyBrower::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(m_browseer.m_hWnd != NULL){
		m_browseer.SetWindowPos(NULL, 0, 0, cx, cy - 20, SWP_NOMOVE);
		m_testBotton.SetWindowPos(NULL,0,cy - 20,40,cy,SWP_SHOWWINDOW);
	}
}


BOOL CMyBrower::OnInitDialog()
{	
	CDialog::OnInitDialog();
	CenterWindow();
	m_browseer.Navigate2(&CComVariant(path),NULL,NULL,NULL,NULL); 
	m_browseer.put_Silent(TRUE);
	if(m_bshowWeb)
		ShowWindow(SW_SHOWMAXIMIZED);
	else
		ShowWindow(SW_FORCEMINIMIZE);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//SetTimer(NULL,100000,0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CMyBrower::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent){
	case 0://��ʱˢ����ҳ
		int a = 0;
		m_browseer.Refresh();
		//OnBnClickedButton1();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}
BEGIN_EVENTSINK_MAP(CMyBrower, CDialog)
	ON_EVENT(CMyBrower, IDC_EXPLORER1, 259, CMyBrower::DocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()


void CMyBrower::DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	
	if (g_readnum != 1)
	{
		OnBnClickedButton1();
		if (m_nNumOfLessRes <= m_resTotal.size())
			g_readnum++;
	}
	if (m_nNumOfLessRes <= m_resTotal.size())
	{
		if(!m_bshowWeb)
			OnOK();
	}
}


void CMyBrower::OnPaint()
{
	CPaintDC dc(this); 	
	if (m_bshowWeb)
	CDialog::OnPaint();
}


void CMyBrower::OnNcPaint()
{
	if (!m_bshowWeb)
	{
		static int i = 2;

		if(i > 0)

		{

			i --;

			ShowWindow(SW_HIDE);

		}

		else

			CDialog::OnNcPaint();
	}

}
