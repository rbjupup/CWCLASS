#include "StdAfx.h"
#include "WListCON.h"


void DBClickList(NM_LISTVIEW* pNMListView, int &m_row, int &m_column, CListCtrl &m_ListInput, CEdit &m_edit,CRect rect)
{
	CRect rc;
	if(pNMListView->iItem!=-1)
	{
		m_row=pNMListView->iItem;//m_rowΪ��ѡ���е�����ţ�int���ͳ�Ա������
		m_column=pNMListView->iSubItem;//m_columnΪ��ѡ���е�����ţ�int���ͳ�Ա������
		m_ListInput.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem,LVIR_LABEL,rc);//ȡ������ľ���
		rc.left+=3;
		rc.left+=rect.left;
		rc.top+=2;
		rc.top+=rect.top;
		rc.right+=3;
		rc.right+=rect.left;
		rc.bottom+=2;
		rc.bottom+=rect.top;

		char * ch=new char [128];
		m_ListInput.GetItemText(pNMListView->iItem, pNMListView->iSubItem,ch,128);//ȡ�����������
		m_edit.SetWindowText(ch);//�������������ʾ���༭����
		m_edit.ShowWindow(SW_SHOW);//��ʾ�༭��
		m_edit.MoveWindow(&rc);//���༭���ƶ����������棬������������
		m_edit.SetFocus();//ʹ�༭��ȡ�ý���
		m_edit.BringWindowToTop();
		m_edit.CreateSolidCaret(1,rc.Height()-5);//����һ�����
		m_edit.ShowCaret();//��ʾ���
		m_edit.SetSel(-1);//ʹ����Ƶ������
	}
}

void killfouce(CEdit &m_edit, CListCtrl &m_ListInput, int m_row, int m_column,vector<vector<CString>> &data)
{
	CString str;
	m_edit.GetWindowText(str);//ȡ�ñ༭�������
	m_ListInput.SetItemText(m_row,m_column,str);//�������ݸ��µ�CListCtrl��
	m_edit.ShowWindow(SW_HIDE);//���ر༭��
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
	m_edit.GetWindowText(str);//ȡ�ñ༭�������
	m_ListInput.SetItemText(m_row,m_column,str);//�������ݸ��µ�CListCtrl��
	m_edit.ShowWindow(SW_HIDE);//���ر༭��
	POSITION pos = m_ListInput.GetFirstSelectedItemPosition();
	int num =m_row;//(int)pos - 1;
	if(num < 0)
		return;
	data[num] = str;

}
void InitList(CListCtrl &m_ListInput)
{
	LONG lStyle;
	lStyle = GetWindowLong(m_ListInput.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_ListInput.m_hWnd, GWL_STYLE, lStyle);//����style
	DWORD dwStyle = m_ListInput.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	m_ListInput.SetExtendedStyle(dwStyle); //������չ���
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
