
// CLASSTESTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CLASSTEST.h"
#include "CLASSTESTDlg.h"
#include "afxdialogex.h"
#include "YCIni.h"
#include "XFunCom.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCLASSTESTDlg 对话框




CCLASSTESTDlg::CCLASSTESTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCLASSTESTDlg::IDD, pParent)
	, m_strHelp(_T(""))
	, m_strInput(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_row = 0;
	m_column = 0;

}

void CCLASSTESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbSupportClass);
	DDX_Control(pDX, IDC_COMBO2, m_cbFunction);
	DDX_Text(pDX, IDC_EDIT1, m_strHelp);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_strInput);
	DDX_Control(pDX, IDC_LIST1, m_ListInput);
	DDX_Control(pDX, IDC_EDIT_TMP, m_edit);
}

BEGIN_MESSAGE_MAP(CCLASSTESTDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CCLASSTESTDlg::OnBnClickedButtonStart)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CCLASSTESTDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON_INPUT, &CCLASSTESTDlg::OnBnClickedButtonInput)
	ON_BN_CLICKED(IDC_BUTTON1, &CCLASSTESTDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CCLASSTESTDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCLASSTESTDlg::OnBnClickedButton2)
	ON_EN_KILLFOCUS(IDC_EDIT_TMP, &CCLASSTESTDlg::OnEnKillfocusEditTmp)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CCLASSTESTDlg::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CCLASSTESTDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CCLASSTESTDlg::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTONU, &CCLASSTESTDlg::OnBnClickedButtonu)
	ON_BN_CLICKED(IDC_BUTTOND, &CCLASSTESTDlg::OnBnClickedButtond)
	ON_BN_CLICKED(IDC_BUTTON_INPUT2, &CCLASSTESTDlg::OnBnClickedButtonInput2)
	ON_BN_CLICKED(IDC_BUTTON_INPUT3, &CCLASSTESTDlg::OnBnClickedButtonInput3)
	ON_BN_CLICKED(IDC_BUTTON_VISION, &CCLASSTESTDlg::OnBnClickedButtonVision)
END_MESSAGE_MAP()


// CCLASSTESTDlg 消息处理程序

UINT AFXSINGALRN(void* para){
	CCLASSTESTDlg* dlg = (CCLASSTESTDlg* )para;
	dlg->RunSellf();
	return 0;
}
BOOL CCLASSTESTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
#ifdef BCG
	CBCGPVisualManager2010::SetStyle (CBCGPVisualManager2010::VS2010_Blue);
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2010));
	EnableVisualManagerStyle();
#endif
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_cbSupportClass.ResetContent();
	m_cbFunction.ResetContent();
	vector<CString> supportclass;
	supportclass = m_boss.GetSupportClass();
	for (int  i = 0 ; i < supportclass.size(); i++)
	{
		m_cbSupportClass.AddString(supportclass[i]);
		if(supportclass[i] == CLASSNAME1)
			m_cbSupportClass.SetCurSel(i);

	}
	//m_cbSupportClass.SetCurSel(0);

	vector<CString> supportFunction;
	CString curClassName;
	m_cbSupportClass.GetLBText(m_cbSupportClass.GetCurSel(),curClassName);
	supportFunction = m_boss.GetSupportFunCtion(curClassName);
	for (int  i = 0 ; i < supportFunction.size(); i++)
	{
		m_cbFunction.AddString(supportFunction[i]);
	}
	m_cbFunction.SetCurSel(0);
	OnCbnSelchangeCombo2();

	LONG lStyle;
	lStyle = GetWindowLong(m_ListInput.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_ListInput.m_hWnd, GWL_STYLE, lStyle);//设置style
	DWORD dwStyle = m_ListInput.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_ListInput.SetExtendedStyle(dwStyle); //设置扩展风格
	m_ListInput.InsertColumn(0,"输入编号",0,80);
	m_ListInput.InsertColumn(1,"输入内容",0,400);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCLASSTESTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCLASSTESTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCLASSTESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCLASSTESTDlg::OnBnClickedButtonStart()
{
	//RunSellf();
	AfxBeginThread(AFXSINGALRN,this);
}


//选择函数改变了
void CCLASSTESTDlg::OnCbnSelchangeCombo2()
{
	//改变帮助函数
	CString surClass,surFun;
	m_cbSupportClass.GetLBText(m_cbSupportClass.GetCurSel(),surClass);
	m_cbFunction.GetLBText(m_cbFunction.GetCurSel(),surFun);
	theApp.m_resdlg->m_strRes = CString("");
	m_strHelp.Format("%s",m_boss.GetFunctionHelp(surClass,surFun));
	UpdateData(FALSE);
}


void CCLASSTESTDlg::OnBnClickedButtonInput()
{
	UpdateData(TRUE);
	int ncount = m_ListInput.GetItemCount();

	CString numIndex;
	numIndex.Format("%d",ncount+1);
	m_ListInput.InsertItem(ncount,numIndex);
	m_ListInput.SetItemText(ncount,1,m_strInput);
	m_strInput = CString("");
	UpdateData(FALSE);
}


void CCLASSTESTDlg::OnBnClickedButton1()
{
	m_ListInput.DeleteAllItems();
	UpdateData(FALSE);
}

//选择的测试类改变了
void CCLASSTESTDlg::OnCbnSelchangeCombo1()
{
	//改变帮助函数


	vector<CString> supportFunction;
	CString curClassName;
	m_cbSupportClass.GetLBText(m_cbSupportClass.GetCurSel(),curClassName);
	supportFunction = m_boss.GetSupportFunCtion(curClassName);
	m_cbFunction.ResetContent();
	for (int  i = 0 ; i < supportFunction.size(); i++)
	{
		m_cbFunction.AddString(supportFunction[i]);
	}
	m_cbFunction.SetCurSel(0);
	OnCbnSelchangeCombo2();
}

//删除选中
void CCLASSTESTDlg::OnBnClickedButton2()
{
	POSITION pos = m_ListInput.GetFirstSelectedItemPosition();
	int num =(int)pos - 1;
	vector<CString> param;
	int ncount = m_ListInput.GetItemCount();
	for (int i = 0 ; i < ncount; i++)
	{
		if(i == num)
			continue;
		CString strText =  m_ListInput.GetItemText(i,1);
		param.push_back(strText);
	}
	m_ListInput.DeleteAllItems();
	for (int i = 0; i < param.size(); i++)
	{
		CString numIndex;
		numIndex.Format("%d",i+1);
		m_ListInput.InsertItem(i,numIndex);
		m_ListInput.SetItemText(i,1,param[i]);
	}
	return;
}


void CCLASSTESTDlg::OnEnKillfocusEditTmp()
{
	CString str;
	m_edit.GetWindowText(str);//取得编辑框的内容
	m_ListInput.SetItemText(m_row,m_column,str);//将该内容更新到CListCtrl中
	m_edit.ShowWindow(SW_HIDE);//隐藏编辑框
	POSITION pos = m_ListInput.GetFirstSelectedItemPosition();
	int num =m_row;//(int)pos - 1;

	if (m_column == 0||m_row == 0)
	{
		return;
	}

	vector<CString> param;
	int ncount = m_ListInput.GetItemCount();
	for (int i = 0 ; i < ncount; i++)
	{
		CString strText =  m_ListInput.GetItemText(i,1);
		if(i == num)
			param.push_back(str);
		else
			param.push_back(strText);
	}
	m_ListInput.DeleteAllItems();
	for (int i = 0; i < param.size(); i++)
	{
		CString numIndex;
		numIndex.Format("%d",i+1);
		m_ListInput.InsertItem(i,numIndex);
		m_ListInput.SetItemText(i,1,param[i]);
	}

	m_ListInput.SetItemState(num - 1,LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
	m_ListInput.SetFocus();

	CSize cs; 
	cs.cx=0;
	cs.cy=10*num;
	m_ListInput.Scroll(cs);
	return;
}


void CCLASSTESTDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	CRect rc,rect;
	if(pNMListView->iItem!=-1)
	{
		m_row=pNMListView->iItem;//m_row为被选中行的行序号（int类型成员变量）
		m_column=pNMListView->iSubItem;//m_column为被选中行的列序号（int类型成员变量）
		m_ListInput.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem,LVIR_LABEL,rc);//取得子项的矩形
		GetDlgItem(IDC_LIST1)->GetWindowRect(&rect);//获取控件的屏幕坐标
		ScreenToClient(&rect);//转换为对话框上的客户坐标
		rc.left+=3;
		rc.left+=rect.left;
		rc.top+=2;
		rc.top+=rect.top;
		rc.right+=3;
		rc.right+=rect.left;
		rc.bottom+=2;
		rc.bottom+=rect.top;

		char * ch=new char [128];
		m_ListInput.GetItemText(pNMListView->iItem, pNMListView->iSubItem,ch,128);//取得子项的内容
		m_edit.SetWindowText(ch);//将子项的内容显示到编辑框中
		m_edit.ShowWindow(SW_SHOW);//显示编辑框
		m_edit.MoveWindow(&rc);//将编辑框移动到子项上面，覆盖在子项上
		m_edit.SetFocus();//使编辑框取得焦点
		m_edit.BringWindowToTop();
		m_edit.CreateSolidCaret(1,rc.Height()-5);//创建一个光标
		m_edit.ShowCaret();//显示光标
		m_edit.SetSel(-1);//使光标移到最后面
	}
	*pResult = 0;
}


void CCLASSTESTDlg::OnBnClickedButtonSave()
{
	CString m_strPath;
	CFileDialog dlg(FALSE, _T("*.*"), NULL,OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("All Files (*.*)|*.*||"), NULL);                        
	dlg.m_ofn.lpstrTitle = _T("保存输入文件");    // 打开文件对话框的标题名
	if( dlg.DoModal() != IDOK )                    // 判断是否获得图片
		return;
	else
	{
		m_strPath = dlg.GetPathName();           // 获取图片路径
	}

	vector<CString> param;
	int ncount = m_ListInput.GetItemCount();
	for (int i = 0 ; i < ncount; i++)
	{
		CString strText =  m_ListInput.GetItemText(i,1);
		param.push_back(strText);
	}
	YCIni ini;
	ini.Read(m_strPath);
	ini.SetValue("[输入数据]","输入总数",(int)param.size());
	for (int i = 0; i < param.size(); i++)
	{
		CString numIndex;
		numIndex.Format("%d",i+1);
		ini.SetValue("[输入数据]",numIndex,param[i]);
	}
	ini.Write(m_strPath);
}


void CCLASSTESTDlg::OnBnClickedButtonRead()
{
	CString m_strPath;
	CFileDialog dlg(TRUE, _T("*.*"), NULL,OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("All Files (*.*)|*.*||"), NULL);                        
	dlg.m_ofn.lpstrTitle = _T("打开输入文件");    // 打开文件对话框的标题名
	if( dlg.DoModal() != IDOK )                    // 判断是否获得图片
		return;
	else
	{
		m_strPath = dlg.GetPathName();           // 获取图片路径
	}

	vector<CString> param;
	int ncount = m_ListInput.GetItemCount();
	for (int i = 0 ; i < ncount; i++)
	{
		CString strText =  m_ListInput.GetItemText(i,1);
		param.push_back(strText);
	}
	m_ListInput.DeleteAllItems();
	YCIni ini;
	ini.Read(m_strPath);
	int numOfInpt = 0;
	ini.GetValue("[输入数据]","输入总数",numOfInpt);
	for (int i = 0; i < numOfInpt; i++)
	{
		CString numIndex;
		numIndex.Format("%d",i+1);
		CString strdata;
		ini.GetValue("[输入数据]",numIndex,strdata);
		m_ListInput.InsertItem(i,numIndex);
		m_ListInput.SetItemText(i,1,strdata);
	}
	ini.Write(m_strPath);
}


void CCLASSTESTDlg::OnBnClickedButtonu()
{
	POSITION pos = m_ListInput.GetFirstSelectedItemPosition();
	if ((int)pos == 1||(int)pos == 0)
	{
		return;
	}
	int num =(int)pos - 1;
	vector<CString> param;
	int ncount = m_ListInput.GetItemCount();
	for (int i = 0 ; i < ncount; i++)
	{
		CString strText =  m_ListInput.GetItemText(i,1);
		param.push_back(strText);
	}
	m_ListInput.DeleteAllItems();
	for (int i = 0; i < param.size(); i++)
	{
		CString numIndex;
		numIndex.Format("%d",i+1);
		m_ListInput.InsertItem(i,numIndex);
		if(i == num - 1)
			m_ListInput.SetItemText(i,1,param[i+1]);
		else if( i == num)
			m_ListInput.SetItemText(i,1,param[i-1]);
		else
			m_ListInput.SetItemText(i,1,param[i]);
	}
	m_ListInput.SetItemState(num - 1,LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
	m_ListInput.SetFocus();

	CSize cs; 
	cs.cx=0;
	cs.cy=10*num-1;
	m_ListInput.Scroll(cs);
	return;
}


void CCLASSTESTDlg::OnBnClickedButtond()
{
	POSITION pos = m_ListInput.GetFirstSelectedItemPosition();
	if ((int)pos == 1||(int)pos == 0)
	{
		return;
	}
	int num =(int)pos - 1;
	vector<CString> param;
	int ncount = m_ListInput.GetItemCount();
	if ((int)pos == ncount||(int)pos == 0)
	{
		return;
	}
	for (int i = 0 ; i < ncount; i++)
	{
		CString strText =  m_ListInput.GetItemText(i,1);
		param.push_back(strText);
	}
	m_ListInput.DeleteAllItems();
	for (int i = 0; i < param.size(); i++)
	{
		CString numIndex;
		numIndex.Format("%d",i+1);
		m_ListInput.InsertItem(i,numIndex);
		if(i == num + 1)
			m_ListInput.SetItemText(i,1,param[i-1]);
		else if( i == num)
			m_ListInput.SetItemText(i,1,param[i+1]);
		else
			m_ListInput.SetItemText(i,1,param[i]);
	}
	m_ListInput.SetItemState(num + 1,LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
	m_ListInput.SetFocus();
	CSize cs; 
	cs.cx=0;
	cs.cy=10*num+1;
	m_ListInput.Scroll(cs);
	return;
}


BOOL CCLASSTESTDlg::PreTranslateMessage(MSG* pMsg)
{	
	if (WM_KEYFIRST <= pMsg-> message && pMsg-> message <= WM_KEYLAST)   
{  
	if(pMsg-> wParam==VK_RETURN )   
	{  
		UpdateData(TRUE);    
		OnEnKillfocusEditTmp();
		return TRUE;
	}   
}  

return CDialog::PreTranslateMessage(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}
void CCLASSTESTDlg::RunSellf()
{
	CString surClass,surFun;
	m_cbSupportClass.GetLBText(m_cbSupportClass.GetCurSel(),surClass);
	m_cbFunction.GetLBText(m_cbFunction.GetCurSel(),surFun);
	theApp.m_resdlg->m_strRes = CString("");
	vector<CString> param;
	int ncount = m_ListInput.GetItemCount();
	for (int i = 0 ; i < ncount; i++)
	{
		CString strText =  m_ListInput.GetItemText(i,1);
		param.push_back(strText);
	}

	// 显示非模态对话框   
	theApp.m_resdlg->ShowWindow(SW_SHOW);   
	m_boss.CallFunction(surClass,surFun,param);
	theApp.m_resdlg->AddStatus("finish");

}


void CCLASSTESTDlg::OnBnClickedButtonInput2()
{
	UpdateData(TRUE);
	int ncount = m_ListInput.GetItemCount();

	CString numIndex;
	numIndex.Format("%d",ncount+1);
	m_ListInput.InsertItem(ncount,numIndex);

	CString dirPath = GetDirPathByDialog("",this,0);
	if(dirPath == "")
		return;
	m_ListInput.SetItemText(ncount,1,dirPath);
	m_strInput = CString("");
	UpdateData(FALSE);
}


void CCLASSTESTDlg::OnBnClickedButtonInput3()
{
	UpdateData(TRUE);
	int ncount = m_ListInput.GetItemCount();

	CString numIndex;
	numIndex.Format("%d",ncount+1);
	m_ListInput.InsertItem(ncount,numIndex);

	CString dirPath = GetDirPathByDialog("",this,1);
	if(dirPath == "")
		return;

	m_ListInput.SetItemText(ncount,1,dirPath);
	m_strInput = CString("");
	UpdateData(FALSE);
}


void CCLASSTESTDlg::OnBnClickedButtonVision()
{
	//打开对话框,并让其最大化
	CDlgImageProcess dlg;
	dlg.DoModal();
}
