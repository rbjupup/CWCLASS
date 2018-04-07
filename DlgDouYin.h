#pragma once
#include "cv.h"
#include "highgui.h"
#include "resource.h"
#include "CLASS/CV/WPowerCV.h"
#include "afxwin.h"
#include "CLASS/MFCCTRL/ImageStatic.h"
#include "XFunCom.h"
#include "opencv2\video\video.hpp"
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

// CDlgDouYin 对话框

class CDlgDouYin : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDouYin)

public:
	CDlgDouYin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDouYin();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOUYIN };
	CWPowerCV m_cwcv;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTestCam();
	virtual BOOL OnInitDialog();
	CImageStatic m_picctlSource;
	CImageStatic m_picctlFace;
	CImageStatic m_picctlChange;
	afx_msg void OnBnClickedButtonTestCam2();
	afx_msg void OnBnClickedButtonTestCam3();
	afx_msg void OnBnClickedButtonTestCam4();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_nZmaxchange;
	int m_nchangeLength;
	int m_nChangeTo;
	int m_extentLength;
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
};
