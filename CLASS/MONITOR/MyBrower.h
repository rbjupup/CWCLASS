#pragma once
#include "explorer1.h"
#include "afxwin.h"
#include <vector>
using namespace std;
// CMyBrower �Ի���

class CMyBrower : public CDialog
{
	DECLARE_DYNAMIC(CMyBrower)

public:
	CMyBrower(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyBrower();

// �Ի�������
	enum { IDD = IDD_DIALOGBROWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CExplorer1 m_browseer;
	CBCGPButton m_testBotton;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL);
	DECLARE_EVENTSINK_MAP()


public:
	//�����ַ
	CString path;
	//html����ͷ��
	vector<CString> m_filterFrontData;
	//html����β��
	vector<CString> m_filterBackData;
	//�Ƿ�Ҫ��ʾ��ҳ
	BOOL m_bshowWeb;
	//���(����ȡ�������ݱ���������)
	vector<vector<CString>> m_res;
	vector<vector<int>> m_resNum;//�ҵ��������ڵڼ���
	vector<CString> m_resTotal;//��ʱ����Ҫ˳����Ϣ���Ͳ��ֿ�������
	vector<int> m_resTotalNum;
	vector<int> m_resTotalType;
	//���Ӧ�����ٸ����������ж���ҳ�Ƿ��Ѿ��������
	int m_nNumOfLessRes;
	//�ˢ��ʱ�䣬������ʱ�仹û�л�õ��㹻��Ľ����ʱ���˳����ڣ��������������
	double m_dMaxMillTime;
	//עʹ��vector��Ϊ��֧��һ����ҳ����ȡ��������
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
};
