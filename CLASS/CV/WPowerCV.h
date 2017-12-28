#pragma once
#include "opencv2/opencv.hpp"
#include "opencv/cv.h"
#include "stdafx.h"
class CWPowerCV
{
public:
	CWPowerCV(void);
	~CWPowerCV(void);


public:
	BOOL SplitIMG(CString InputImgPath,CString OutPutDir,int XNum,int YNum);
	BOOL ImgCal(CString srcfirst,CString srcsecond,CString savePath,int XMove,int YMove,int CalType = 0,BOOL bDir = FALSE);
};

