#pragma once
/************************************************************************/
/*   
这个类包含了一些listcontrol的操作
输入list的指针,实现了一些函数直接处理

*/
/************************************************************************/
class CWListCON
{
public:
	CWListCON(void);
	CEdit m_edit;
	CListCtrl m_list;

	vector<vector<CString>> vecdata;

	//用来edit记录上次点击
	int m_row,m_column;
	//全部删除
	void DelAllMyList();
	//edit失去焦点的时候调用
	void KillEditFouce();
	//双击列表的时候调用
	//CRect rect;GetDlgItem(IDC_LIST1)->GetWindowRect(&rect);ScreenToClient(&rect);
	void DBClickMyList(NM_LISTVIEW* pNMListView,CRect rect);
	//滑动块移动到
	void ListScrollTo(int pos1);


	//初始化的时候调用
	void InitMyList(vector<CString> itemName,vector<int> itemsize);
	//加入到列表
	void AddToMyList(vector<CString> addContant);
	//从列表删除
	void DelFromMyList(int index);
	void DelCurFromMyList();
	//刷新列表
	void RenewList();
	//交换两个数据的位置
	void changepos(int pos1,int pos2);
	//当前选项向上或者向下
	void CurUpDown(BOOL bUp);

	int GetSelectedIndex();

	/************************************************************************/
	/*                        单列列表开始                                  */
	/************************************************************************/
	vector<CString> vecdatasin;

	//初始化的时候调用
	void InitMyList(CString itemName,int itemsize);
	//加入到列表
	void AddToMyList(CString addContant);
	//从列表删除
	void DelFromMyListSin(int index);
	void DelCurFromMyListSin();
	//刷新列表
	void RenewListSin();
	//交换两个数据的位置
	void changepossin(int pos1,int pos2);


	//当前选项向上或者向下
	void CurUpDownSin(BOOL bUp);
	void KillEditFouceSin();
	/************************************************************************/
	/*                        单列列表结束                                  */
	/************************************************************************/
	~CWListCON(void);
};

