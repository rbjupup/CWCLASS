#pragma once
/************************************************************************/
/*   
����������һЩlistcontrol�Ĳ���
����list��ָ��,ʵ����һЩ����ֱ�Ӵ���

*/
/************************************************************************/
class CWListCON
{
public:
	CWListCON(void);
	CEdit m_edit;
	CListCtrl m_list;

	vector<vector<CString>> vecdata;

	//����edit��¼�ϴε��
	int m_row,m_column;
	//ȫ��ɾ��
	void DelAllMyList();
	//editʧȥ�����ʱ�����
	void KillEditFouce();
	//˫���б��ʱ�����
	//CRect rect;GetDlgItem(IDC_LIST1)->GetWindowRect(&rect);ScreenToClient(&rect);
	void DBClickMyList(NM_LISTVIEW* pNMListView,CRect rect);
	//�������ƶ���
	void ListScrollTo(int pos1);


	//��ʼ����ʱ�����
	void InitMyList(vector<CString> itemName,vector<int> itemsize);
	//���뵽�б�
	void AddToMyList(vector<CString> addContant);
	//���б�ɾ��
	void DelFromMyList(int index);
	void DelCurFromMyList();
	//ˢ���б�
	void RenewList();
	//�����������ݵ�λ��
	void changepos(int pos1,int pos2);
	//��ǰѡ�����ϻ�������
	void CurUpDown(BOOL bUp);

	int GetSelectedIndex();

	/************************************************************************/
	/*                        �����б�ʼ                                  */
	/************************************************************************/
	vector<CString> vecdatasin;

	//��ʼ����ʱ�����
	void InitMyList(CString itemName,int itemsize);
	//���뵽�б�
	void AddToMyList(CString addContant);
	//���б�ɾ��
	void DelFromMyListSin(int index);
	void DelCurFromMyListSin();
	//ˢ���б�
	void RenewListSin();
	//�����������ݵ�λ��
	void changepossin(int pos1,int pos2);


	//��ǰѡ�����ϻ�������
	void CurUpDownSin(BOOL bUp);
	void KillEditFouceSin();
	/************************************************************************/
	/*                        �����б����                                  */
	/************************************************************************/
	~CWListCON(void);
};

