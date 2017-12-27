#pragma once
#include "opencv2/opencv.hpp"
#include "opencv/cv.h"
class CWPowerCV
{
public:
	CWPowerCV(void);
	~CWPowerCV(void);


public:
	BOOL SplitIMG(CString InputImgPath,CString OutPutDir,int XNum,int YNum);
};

