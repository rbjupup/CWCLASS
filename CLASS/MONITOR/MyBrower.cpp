// MyBrower.cpp : 实现文件
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
// CMyBrower 对话框
int g_readnum;
IMPLEMENT_DYNAMIC(CMyBrower, CDialog)

CMyBrower::CMyBrower(CWnd* pParent /*=NULL*/)
	: CDialog(CMyBrower::IDD, pParent)
{
	//监控网址
	path = CString("http://hotels.ctrip.com/hotel/669353.html");
	//html代码头部
	m_filterFrontData.push_back("<DIV class=hrd-title>");
	m_filterFrontData.push_back("<P class=base_box><SPAN class=base_price><DFN>￥</DFN>");
	//html代码尾部
	m_filterBackData.push_back("</DIV>");
	m_filterBackData.push_back("</SPAN></P>");
	//是否要显示网页
	m_bshowWeb = FALSE;
	//结果应有最少个数，用来判断网页是否已经加载完成
	m_nNumOfLessRes = 20;
	//最长刷新时间，超过该时间还没有获得到足够多的结果的时候，退出窗口，让软件继续运行
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


// CMyBrower 消息处理程序


void CMyBrower::OnBnClickedButton1()
{
	if (m_filterBackData.size() != m_filterFrontData.size())
	{
		AfxMessageBox("数据头和数据尾数目不匹配，请重新输入");
		return;
	}
	//初始化结果容器
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
		BSTR html;//存放html源代码  
		CComBSTR html_t;//用于将BSTR转换为cout可以处理的字符串  
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
		AfxMessageBox(_T("读取到的html信息保存失败!"));
		return;
	}
	int  m_nRows = 0;
	m_StFile.Open("TMP\\1.txt",CFile::modeRead);
	if (m_StFile == NULL)
	{
		AfxMessageBox(_T("读取到的html信息打开失败!"));
		return;
	}
	CString str;
	vector<CString> SrcData;
	while (m_StFile.ReadString(str))					//整行读取
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
	// TODO:  在此添加额外的初始化
	//SetTimer(NULL,100000,0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMyBrower::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent){
	case 0://定时刷新网页
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
