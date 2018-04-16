#include "StdAfx.h"
#include "WListCON.h"


void DBClickList(NM_LISTVIEW* pNMListView, int &m_row, int &m_column, CListCtrl &m_ListInput, CEdit &m_edit,CRect rect)
{
	CRect rc;
	if(pNMListView->iItem!=-1)
	{
		m_row=pNMListView->iItem;//m_row为被选中行的行序号（int类型成员变量）
		m_column=pNMListView->iSubItem;//m_column为被选中行的列序号（int类型成员变量）
		m_ListInput.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem,LVIR_LABEL,rc);//取得子项的矩形
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
}

void killfouce(CEdit &m_edit, CListCtrl &m_ListInput, int m_row, int m_column,vector<vector<CString>> &data)
{
	CString str;
	m_edit.GetWindowText(str);//取得编辑框的内容
	m_ListInput.SetItemText(m_row,m_column,str);//将该内容更新到CListCtrl中
	m_edit.ShowWindow(SW_HIDE);//隐藏编辑框
	POSITION pos = m_ListInput.GetFirstSelectedItemPosition();
	int num =m_row;//(int)pos - 1;

	if (m_column < 0||m_row < 0)
	{
		return;
	}
	data[m_column][m_row] = str;

}
void killfouceSin(CEdit &m_edit, CListCtrl &m_ListInput, int m_row, int m_column,vector<CString> &data)
{
	CString str;
	m_edit.GetWindowText(str);//取得编辑框的内容
	m_ListInput.SetItemText(m_row,m_column,str);//将该内容更新到CListCtrl中
	m_edit.ShowWindow(SW_HIDE);//隐藏编辑框
	POSITION pos = m_ListInput.GetFirstSelectedItemPosition();
	int num =m_row;//(int)pos - 1;
	if(num < 0)
		return;
	data[num] = str;

}
void InitList(CListCtrl &m_ListInput)
{
	LONG lStyle;
	lStyle = GetWindowLong(m_ListInput.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_ListInput.m_hWnd, GWL_STYLE, lStyle);//设置style
	DWORD dwStyle = m_ListInput.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_ListInput.SetExtendedStyle(dwStyle); //设置扩展风格
}

CWListCON::CWListCON(void)
{
}


CWListCON::~CWListCON(void)
{
}


void CWListCON::InitMyList(vector<CString> itemName,vector<int> itemsize)
{
	InitList(m_list);
	for (int i = 0; i < itemName.size();i++)
	{
		m_list.InsertColumn(i,itemName[i],0,itemsize[i]);
	}
	 m_row = 0;
	 m_column = 0;
}

void CWListCON::InitMyList(CString itemName,int itemsize)
{
	InitList(m_list);
	m_list.InsertColumn(0,itemName,0,itemsize);
	m_row = 0;
	m_column = 0;
}

void CWListCON::DBClickMyList(NM_LISTVIEW* pNMListView,CRect rect)
{
	DBClickList(pNMListView, m_row,m_column, m_list, m_edit,rect);
}

void CWListCON::AddToMyList(vector<CString> addContant)
{
	vecdata.push_back(addContant);
	RenewList();
}

void CWListCON::AddToMyList(CString addContant)
{
	int ncount = m_list.GetItemCount();
	vecdatasin.push_back(addContant);
	RenewListSin();
}

void CWListCON::DelCurFromMyList()
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int num =(int)pos - 1;
	DelFromMyList(num);
	return;
}

void CWListCON::DelFromMyList(int index)
{
	vector<vector<CString>> tmpdata;
	tmpdata.swap(vecdata);
	for (int i = 0; i < tmpdata.size();i++)
	{
		if(i == index)
		{
			continue;
		}
		vecdata.push_back(tmpdata[i]);
	}
	RenewList();
}

void CWListCON::KillEditFouce()
{
	killfouce(m_edit,m_list,m_row,m_column,vecdata);
	RenewList();
	ListScrollTo(m_row);
}
void CWListCON::KillEditFouceSin()
{
	killfouceSin(m_edit,m_list,m_row,m_column,vecdatasin);
	RenewListSin();
	ListScrollTo(m_row);
}
void CWListCON::RenewList()
{
	m_list.DeleteAllItems();
	for (int j = 0; j < vecdata.size();j++)
	{
		for(int i = 0; i < vecdata[j].size();i++)
		{
			if(i == 0)
			{
				m_list.InsertItem(j,vecdata[j][i]);
				continue;
			}
			m_list.SetItemText(j,i,vecdata[j][i]);
		}
	}
}

void CWListCON::RenewListSin()
{
	m_list.DeleteAllItems();
	for (int j = 0; j < vecdatasin.size();j++)
	{
		m_list.InsertItem(j,vecdatasin[j]);
	}
}

void CWListCON::changepos(int pos1,int pos2)
{
	if (pos1 < 0 || pos2 < 0 || vecdata.size()<2||pos1 > vecdata.size() -1||pos2 > vecdata.size()-1)
	{
		return;
	}

	swap(vecdata[pos1] ,vecdata[pos2]);
}

void CWListCON::changepossin(int pos1,int pos2)
{
	if (pos1 < 0 || pos2 < 0 || vecdatasin.size()<2||pos1 > vecdatasin.size() -1||pos2 > vecdatasin.size()-1)
	{
		return;
	}

	swap(vecdatasin[pos1] ,vecdatasin[pos2]);
	RenewListSin();
}

void CWListCON::DelAllMyList()
{
	m_list.DeleteAllItems();
}

void CWListCON::DelFromMyListSin(int index)
{
	vector<CString> tmpdata;
	tmpdata.swap(vecdatasin);
	for (int i = 0; i < tmpdata.size();i++)
	{
		if(i == index)
		{
			continue;
		}
		vecdatasin.push_back(tmpdata[i]);
	}
	RenewListSin();
}

void CWListCON::DelCurFromMyListSin()
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int num =(int)pos - 1;
	DelFromMyListSin(num);
	return;
}

void CWListCON::CurUpDown(BOOL bUp)
{
	int num = GetSelectedIndex();

	int numnew =0; 
	if(bUp)
	{
		numnew = num - 1;
		changepos(num,num - 1);
	}
	else
	{
		numnew = num + 1;
		changepos(num,num + 1);
	}
	ListScrollTo(numnew);
}

void CWListCON::CurUpDownSin(BOOL bUp)
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int num =(int)pos - 1;
	int numnew =0; 
	if(bUp)
	{
		numnew = num - 1;
		changepossin(num,num - 1);
	}
	else
	{
		numnew = num + 1;
		changepossin(num,num + 1);
	}
	ListScrollTo(numnew);
}

void CWListCON::ListScrollTo(int pos1)
{
	m_list.SetItemState(pos1,LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
	m_list.SetFocus();
	CSize cs; 
	cs.cx=0;
	cs.cy=10*pos1;
	m_list.Scroll(cs);
}

int CWListCON::GetSelectedIndex()
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int num =(int)pos - 1;
	return num;
}
