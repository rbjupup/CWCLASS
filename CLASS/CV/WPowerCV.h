#pragma once
#include "opencv2/opencv.hpp"
#include "opencv/cv.h"
#include "stdafx.h"
#define CVTRYSTART try{
#define CVTRYEND 	}catch(cv::Exception err){\
CString opencverr;\
opencverr.Format("%s",err.msg);\
AfxMessageBox(opencverr);\
return FALSE;}
class CWPowerCV
{
public:
	CWPowerCV(void);
	~CWPowerCV(void);


public:
/************************************************************************/
/*                             图像分割开始                             */
/************************************************************************/
	//图像切分
	BOOL SplitByXYNum(CString InputImgPath,CString OutPutDir,int XNum,int YNum);
	//阈值分割
	BOOL SplitByThreshold(CString InputImgPath,CString OutPutPath,int threshold);
	//ROI分割
	BOOL SplitByROI(CString InputImgPath,CString OutPutPath,int x,int y, int width,int height);
/************************************************************************/
/*                             图像分割结束                             */
/************************************************************************/
	//图像加减
	BOOL ImgCal(CString srcfirst,CString srcsecond,CString savePath,int XMove,int YMove,int CalType = 0,BOOL bDir = FALSE);
	//解析csv点集，绘制灰度图
	BOOL ParsePtSet(CString inputPath,CString OutPutPath,int type);
	//找轮廓
	void FindContour(CString InputImgPath,CString OutPutPath,int method);
	//漫水填充测试
	void FillFloodFillTest(CString InpuPath);
};

