// WMFCBtn.cpp : 实现文件
//

#include "stdafx.h"
#include "CLASSTEST.h"
#include "WMFCBtn.h"
#include "afxdialogex.h"


// CWMFCBtn 对话框

IMPLEMENT_DYNAMIC(CWMFCCtlMan, CDialog)
WCHAR* ToWChar(char *str)  
{  
	//在 GDI＋中，有关字符的参数类型全部都是 WCHAR 类型的  
	//该函数是将传统字符串进行转换  
	static WCHAR buffer[1024];  
	wcsset(buffer,0);  
	MultiByteToWideChar(CP_ACP,0,str,strlen(str),buffer,1024);  
	return buffer;  
}  

int GetImageCLSID(const WCHAR *format, CLSID *pCLSID)  
{  
	UINT num=0;  
	UINT size=0;  
	ImageCodecInfo* pImageCodecInfo=NULL;  
	GetImageEncodersSize(&num,&size);  
	if(size==0)  
		return FALSE; // 编码信息不可用  
	// 分配内存  
	pImageCodecInfo=(ImageCodecInfo*)(malloc(size));  
	if(pImageCodecInfo==NULL)  
		return FALSE; // 分配失败  
	// 获得系统中可用的编码方式的所有信息  
	GetImageEncoders(num,size,pImageCodecInfo);  
	// 在可用编码信息中查找 format 格式是否被支持  

	for(UINT i=0;i<num;++i)  
	{  
		//MimeType: 编码方式的具体描述  
		if (wcscmp(pImageCodecInfo[ i] .MimeType,format)==0)  
		{  
			*pCLSID=pImageCodecInfo[i].Clsid;  
			free(pImageCodecInfo);  
			return TRUE;  
		}  
	}  
	free(pImageCodecInfo);  
	return FALSE;  
}  

CWMFCCtlMan::CWMFCCtlMan(CWnd* pParent /*=NULL*/)
	: CDialog(CWMFCCtlMan::IDD, pParent),
	m_Bg1(ToWChar("bg1.png")),m_Bg2(ToWChar("bg1.png"))
{

}

CWMFCCtlMan::~CWMFCCtlMan()
{
//	GdiplusShutdown(m_pGdiToken);  
}

void CWMFCCtlMan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_UNNORMAL2, m_btnPng);
}


BEGIN_MESSAGE_MAP(CWMFCCtlMan, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_UNNORMAL2, &CWMFCCtlMan::OnBnClickedButtonUnnormal2)
END_MESSAGE_MAP()


// CWMFCBtn 消息处理程序


BOOL CWMFCCtlMan::OnInitDialog()
{
	CDialog::OnInitDialog();
//	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);  
	// TODO:  在此添加额外的初始化
	CMFCButton *pMfcButton = (CMFCButton *)GetDlgItem(IDC_BUTTON_UNNORMAL2);  

// 	m_Bg1.FromFile(ToWChar("bg1.png"));
// 	m_Bg2.FromFile(ToWChar("bg2.png"));
// 	HBITMAP hBmp, hBmpHot;  
// 	m_Bg1.GetHBITMAP(NULL, &hBmp);  
// 	m_Bg2.GetHBITMAP(NULL, &hBmpHot);  
// 	pMfcButton->SetImage(hBmp, TRUE, hBmpHot);  
// 	OnPaint();
// 	pMfcButton->m_bTransparent = TRUE;    // 设置按钮为透明的  
	m_btnPng.LoadStdImage(IDB_LOGINBTN, _T("PNG")); //正常状态 
	m_btnPng.LoadAltImage(IDB_LOGINACTIVE, _T("PNG")); //按下时 
	CRect rect;
	m_btnPng.GetClientRect(rect);
	m_btnPng.MoveWindow(CRect(rect.left,rect.top,rect.left + 128,rect.top + 128));
	m_btnPng.EnableToggle(TRUE); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CWMFCCtlMan::OnPaint()
{
	CPaintDC dc(this); // device context for painting
// 	Graphics graphics(dc);  
// 	Pen pen(Color(255, 0, 255));  
// 	graphics.DrawLine(&pen, 0, 0, 200, 100); 
// 	//Bitmap image(ToWChar("bg1.png"));  
// 	Rect rect =Rect( 0,0,m_Bg1.GetWidth(),m_Bg1.GetHeight()); 
// 	graphics.DrawImage(&m_Bg1,rect);
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}


void CWMFCCtlMan::OnBnClickedButtonUnnormal2()
{
	//GetDlgItem(IDC_BUTTON_UNNORMAL2)->PostMessage(WM_KILLFOCUS, 0, 0);
	int a = 0;
}

void CWMFCCtlMan::OpenTabDlg(int type)
{
	m_dlgTab.showtype = type;
	m_dlgTab.DoModal();
}

void CWMFCCtlMan::OpenStaticDlg()
{
	m_dlgStaticTest.DoModal();
}
