#pragma once
#include "explorer1.h"
#include "afxwin.h"
#include <vector>
using namespace std;
// CMyBrower 对话框

class CMyBrower : public CDialog
{
	DECLARE_DYNAMIC(CMyBrower)

public:
	CMyBrower(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyBrower();

// 对话框数据
	enum { IDD = IDD_DIALOGBROWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	//监控网址
	CString path;
	//html代码头部
	vector<CString> m_filterFrontData;
	//html代码尾部
	vector<CString> m_filterBackData;
	//是否要显示网页
	BOOL m_bshowWeb;
	//结果(最后读取到的数据保存在这里)
	vector<vector<CString>> m_res;
	vector<vector<int>> m_resNum;//找到的数据在第几行
	vector<CString> m_resTotal;//有时候需要顺序信息，就不分开数据了
	vector<int> m_resTotalNum;
	vector<int> m_resTotalType;
	//结果应有最少个数，用来判断网页是否已经加载完成
	int m_nNumOfLessRes;
	//最长刷新时间，超过该时间还没有获得到足够多的结果的时候，退出窗口，让软件继续运行
	double m_dMaxMillTime;
	//注使用vector是为了支持一个网页中提取多种数据
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
};
