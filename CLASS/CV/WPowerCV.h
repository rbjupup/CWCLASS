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
};

