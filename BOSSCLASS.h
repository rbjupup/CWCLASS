#pragma once
#include <vector>
using namespace std;

#include "DefineOfString.h"
#include "CLASS/BCG/CWBCG.h"
#include "CLASS/ROB/WROBO.h"
#include "CLASS/MFCCTRL/WMFCBtn.h"
#include "stdafx.h"
#include "CLASS/MONITOR/WmonUrl.h"
#include "CLASS/CV/WPowerCV.h"
#include "DlgDouYin.h"





class BOSSCLASS
{
public:
	BOSSCLASS(void);
	vector<CString> GetSupportClass();
	vector<CString> GetSupportFunCtion(CString ClassName);
	bool CallFunction(CString classname,CString funname,vector<CString> );
	CString GetFunctionHelp(CString classname,CString funname);
	~BOSSCLASS(void);

private:
	vector<CString> m_vecSupportClass;
	vector<vector<CString>> m_vecSupportFunction;
	CWBCG     m_cwBcg;						//BCG测试界面
	CWROBO    m_robo;						//AI类
	CWMFCCtlMan m_cwMfcCtl;					//控件管理类
	CWmonUrl m_monitor;						//网页监听类
	CWPowerCV m_powcv;						//opencv二次开发
	CDlgDouYin m_douyin;						//抖音窗口开发
};

