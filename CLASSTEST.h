
// CLASSTEST.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "ResDlg.h"
#include <afxdtctl.h>  
#define ULONG_PTR ULONG  
#include <gdiplus.h>  
using namespace Gdiplus;  
#pragma comment(lib, "gdiplus.lib")  
#ifdef BCG
#include "afxwin.h"
#include "afxcmn.h"
#include "BCGPVisualCtrl.h"
#include "BCGPChartVisualObject.h"
#endif
// CCLASSTESTApp:
// 有关此类的实现，请参阅 CLASSTEST.cpp
//

class CCLASSTESTApp : public CWinApp
{
public:
	CCLASSTESTApp();
	~CCLASSTESTApp();
private:  
	GdiplusStartupInput m_gdiplusStartupInput;  
	ULONG_PTR m_pGdiToken;  
// 重写
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance()
	{
		GdiplusShutdown(m_pGdiToken);  
		return TRUE;
	}
	CResDlg* m_resdlg;
// 实现

	DECLARE_MESSAGE_MAP()
};

extern CCLASSTESTApp theApp;